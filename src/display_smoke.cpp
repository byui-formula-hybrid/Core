#include "display_smoke.h"

#include <stdint.h>

#include "driver/i2c.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_log.h"

namespace {

static const char *TAG = "display_smoke";

static constexpr int LCD_H_RES = 800;
static constexpr int LCD_V_RES = 480;
static constexpr int PIXEL_CLOCK_HZ = 16 * 1000 * 1000;

static constexpr i2c_port_t BL_I2C_PORT = I2C_NUM_0;
static constexpr int BL_SDA = 8;
static constexpr int BL_SCL = 9;

static esp_lcd_panel_handle_t g_panel = nullptr;

static esp_err_t init_backlight_i2c()
{
    i2c_config_t cfg = {};
    cfg.mode = I2C_MODE_MASTER;
    cfg.sda_io_num = BL_SDA;
    cfg.scl_io_num = BL_SCL;
    cfg.sda_pullup_en = GPIO_PULLUP_ENABLE;
    cfg.scl_pullup_en = GPIO_PULLUP_ENABLE;
    cfg.master.clk_speed = 400000;

    esp_err_t err = i2c_param_config(BL_I2C_PORT, &cfg);
    if (err != ESP_OK) {
        return err;
    }

    err = i2c_driver_install(BL_I2C_PORT, cfg.mode, 0, 0, 0);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    return ESP_OK;
}

static esp_err_t backlight_on()
{
    // CH422G: set output mode, then set LCD backlight line high.
    uint8_t mode = 0x01;
    esp_err_t err = i2c_master_write_to_device(BL_I2C_PORT, 0x24, &mode, 1, pdMS_TO_TICKS(1000));
    if (err != ESP_OK) {
        return err;
    }

    uint8_t on = 0x1E;
    return i2c_master_write_to_device(BL_I2C_PORT, 0x38, &on, 1, pdMS_TO_TICKS(1000));
}

static esp_err_t backlight_off()
{
    uint8_t mode = 0x01;
    esp_err_t err = i2c_master_write_to_device(BL_I2C_PORT, 0x24, &mode, 1, pdMS_TO_TICKS(1000));
    if (err != ESP_OK) {
        return err;
    }

    uint8_t off = 0x1A;
    return i2c_master_write_to_device(BL_I2C_PORT, 0x38, &off, 1, pdMS_TO_TICKS(1000));
}

} // namespace

namespace dash {
namespace display {

esp_err_t init_smoke_display()
{
    esp_lcd_rgb_panel_config_t panel_cfg = {};
    panel_cfg.clk_src = LCD_CLK_SRC_DEFAULT;
    panel_cfg.timings.pclk_hz = PIXEL_CLOCK_HZ;
    panel_cfg.timings.h_res = LCD_H_RES;
    panel_cfg.timings.v_res = LCD_V_RES;
    panel_cfg.timings.hsync_pulse_width = 4;
    panel_cfg.timings.hsync_back_porch = 8;
    panel_cfg.timings.hsync_front_porch = 8;
    panel_cfg.timings.vsync_pulse_width = 4;
    panel_cfg.timings.vsync_back_porch = 8;
    panel_cfg.timings.vsync_front_porch = 8;
    panel_cfg.timings.flags.pclk_active_neg = 1;

    panel_cfg.data_width = 16;
    panel_cfg.bits_per_pixel = 16;
    panel_cfg.num_fbs = 1;
    panel_cfg.bounce_buffer_size_px = LCD_H_RES * 10;
    panel_cfg.sram_trans_align = 4;
    panel_cfg.psram_trans_align = 64;

    panel_cfg.hsync_gpio_num = GPIO_NUM_46;
    panel_cfg.vsync_gpio_num = GPIO_NUM_3;
    panel_cfg.de_gpio_num = GPIO_NUM_5;
    panel_cfg.pclk_gpio_num = GPIO_NUM_7;
    panel_cfg.disp_gpio_num = -1;

    static const int data_pins[16] = {
        GPIO_NUM_14, GPIO_NUM_38, GPIO_NUM_18, GPIO_NUM_17,
        GPIO_NUM_10, GPIO_NUM_39, GPIO_NUM_0,  GPIO_NUM_45,
        GPIO_NUM_48, GPIO_NUM_47, GPIO_NUM_21, GPIO_NUM_1,
        GPIO_NUM_2,  GPIO_NUM_42, GPIO_NUM_41, GPIO_NUM_40
    };
    for (int i = 0; i < 16; ++i) {
        panel_cfg.data_gpio_nums[i] = data_pins[i];
    }
    panel_cfg.flags.fb_in_psram = 1;

    ESP_LOGI(TAG, "Creating RGB panel");
    esp_err_t err = esp_lcd_new_rgb_panel(&panel_cfg, &g_panel);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_new_rgb_panel failed: %d", static_cast<int>(err));
        return err;
    }

    err = esp_lcd_panel_init(g_panel);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_panel_init failed: %d", static_cast<int>(err));
        return err;
    }

    err = init_backlight_i2c();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "backlight I2C init failed: %d", static_cast<int>(err));
        return err;
    }

    err = backlight_on();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "backlight enable failed: %d", static_cast<int>(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t draw_color_bars()
{
    if (g_panel == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    // One scanline buffer to keep memory low.
    uint16_t *row = static_cast<uint16_t *>(heap_caps_malloc(sizeof(uint16_t) * LCD_H_RES, MALLOC_CAP_8BIT));
    if (row == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    static constexpr uint16_t bars[8] = {
        0xF800, // red
        0x07E0, // green
        0x001F, // blue
        0xFFE0, // yellow
        0xF81F, // magenta
        0x07FF, // cyan
        0xFFFF, // white
        0x0000  // black
    };

    for (int y = 0; y < LCD_V_RES; ++y) {
        for (int x = 0; x < LCD_H_RES; ++x) {
            int idx = (x * 8) / LCD_H_RES;
            row[x] = bars[idx];
        }
        esp_lcd_panel_draw_bitmap(g_panel, 0, y, LCD_H_RES, y + 1, row);
    }

    free(row);
    ESP_LOGI(TAG, "Color bars drawn");
    return ESP_OK;
}

esp_err_t draw_solid_color(uint16_t rgb565)
{
    if (g_panel == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    uint16_t *row = static_cast<uint16_t *>(heap_caps_malloc(sizeof(uint16_t) * LCD_H_RES, MALLOC_CAP_8BIT));
    if (row == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    for (int x = 0; x < LCD_H_RES; ++x) {
        row[x] = rgb565;
    }

    for (int y = 0; y < LCD_V_RES; ++y) {
        esp_lcd_panel_draw_bitmap(g_panel, 0, y, LCD_H_RES, y + 1, row);
    }

    free(row);
    return ESP_OK;
}

esp_err_t set_backlight(bool on)
{
    return on ? backlight_on() : backlight_off();
}

} // namespace display
} // namespace dash
