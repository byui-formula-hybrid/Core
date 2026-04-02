#if defined(ENV_ESP32)

#include "twai_service.h"

using namespace CORE;

// This is a link to the documentation for the ESP32 TWAI driver, which is what this service is built on top of.
// https://docs.espressif.com/projects/esp-idf/en/v4.2/esp32/api-reference/peripherals/twai.html

bool TWAIService::setup(const void * config) {
    if (!config) return false;

    // 1. Validate Config Type
    const BaseConfig* base = static_cast<const BaseConfig*>(config);
    if (base->type != ServiceConfigType::TWAI) return false;

    // 2. Ensure a clean state
    // We don't care if tear_down "fails," we just want the driver uninstalled.
    tear_down(); 

    const TWAIConfig* twai_config = static_cast<const TWAIConfig*>(config);

    // 3. Install Driver
    // This allocates the RX/TX Ring Buffers in internal RAM.
    esp_err_t install_res = twai_driver_install(
        twai_config->general_config,
        twai_config->timing_config,
        twai_config->filter_config
    );

    if (install_res != ESP_OK) {
        // Log: install_res (e.g., out of memory or invalid pins)
        LOG_ERR("TWAIService", "Failed to install TWAI driver: %d", install_res);
        return false;
    }

    // 4. Start the Driver
    // This transitions the hardware from 'Stopped' to 'Running' (Alert/Listen mode)
    esp_err_t start_res = twai_start();
    if (start_res != ESP_OK) {
        LOG_ERR("TWAIService", "Failed to start TWAI driver: %d", start_res);
        esp_err_t uninstall_res = twai_driver_uninstall(); // Cleanup if start fails
        if (uninstall_res != ESP_OK) {
            LOG_ERR("TWAIService", "Failed to uninstall TWAI driver after failed start: %d", uninstall_res);
        }
        return false;
    }

    return true;
}

bool TWAIService::tear_down() {
    twai_status_info_t status;
    // If the driver is not installed, twai_get_status_info returns ESP_ERR_INVALID_STATE
    if (get_status(status) == ESP_ERR_INVALID_STATE) {
        return true; 
    }

    // 1. If we are running or recovering, we must STOP first
    if (status.state == TWAI_STATE_RUNNING || status.state == TWAI_STATE_RECOVERING) {
        esp_err_t stop_res = twai_stop();
        if (stop_res != ESP_OK) {
            // Log: Failed to stop, but we will try to uninstall anyway
            LOG_ERR("TWAIService", "Failed to stop TWAI driver during tear_down: %d", stop_res);
        }
    }

    // 2. Uninstall the driver to free up the RX/TX ring buffers (RAM)
    esp_err_t uninstall_res = twai_driver_uninstall();
    if (uninstall_res != ESP_OK) {
        // Log: Fatal error during uninstall
        LOG_ERR("TWAIService", "Failed to uninstall TWAI driver: %d", uninstall_res);
        return false;
    }

    return true;
}

bool TWAIService::send(Frame& frame) {
    twai_status_info_t status;
    if (get_status(status) != ESP_OK || status.state != TWAI_STATE_RUNNING) {
        return false; 
    }

    twai_message_t twai_frame;
    twai_frame.identifier = frame.identifier;
    twai_frame.data_length_code = frame.data_length_code;
    for (int i = 0; i < frame.data_length_code; i++) {
        twai_frame.data[i] = frame.data[i];
    }
    
    esp_err_t result = twai_transmit(twai_frame, portMAX_DELAY);
    if (result != ESP_OK) {
        LOG_ERR("TWAIService", "Failed to transmit TWAI frame: %d", result);
        return false;
    }

    return true;
}

const Frame* TWAIService::read() {
    twai_status_info_t status;
    if (get_status(status) != ESP_OK || status.state != TWAI_STATE_RUNNING) {
        return nullptr; 
    }

    twai_message_t twai_msg;
    // Use a small timeout or portMAX_DELAY depending on your Dispatcher task strategy
    esp_err_t result = twai_receive(&twai_msg, 100 / portTICK_PERIOD_MS);
    if (result != ESP_OK) {
        if (result != ESP_ERR_TIMEOUT) {
            LOG_ERR("TWAIService", "Failed to receive TWAI frame: %d", result);
        }
        return nullptr;
    }

    // IMPORTANT: Avoid 'Frame* frame = {};' as that is a null pointer.
    // Use a static or member variable to return the address, or better yet, 
    // change the interface to 'bool read(Frame& outFrame)'.
    static Frame frame_buffer; 
    frame_buffer.identifier = twai_msg.identifier;
    frame_buffer.data_length_code = twai_msg.data_length_code;
    memcpy(frame_buffer.data, twai_msg.data, twai_msg.data_length_code);

    return &frame_buffer;
}

bool TWAIService::recover() {
    twai_status_info_t status;
    if (get_status(status) != ESP_OK) {
        return false;
    }

    switch (status.state) {
        case TWAI_STATE_BUS_OFF:
            // The hardware is 'dead'. Start the 128-pulse countdown.
            if (twai_initiate_recovery() == ESP_OK) {
                LOG_WARN("TWAIService", "Bus off detected. Initiating recovery...");
                return true;
            }
            break;

        case TWAI_STATE_STOPPED:
            // Recovery finished! But we are still 'Stopped'.
            // We need to re-start to get back to 'Running'.
            if (twai_start() == ESP_OK) {
                LOG_INFO("TWAIService", "Recovery complete. Bus is back online.");
                return true;
            }
            break;

        case TWAI_STATE_RECOVERING:
            // We are currently waiting for those 128 idle sequences.
            // Just wait; don't trigger anything else.
            break;

        case TWAI_STATE_RUNNING:
            // Everything is fine.
            break;
    }

    return true;
}

esp_err_t TWAIService::get_status(twai_status_info_t& status) {
    esp_err_t res = twai_get_status_info(&status);
    if (res != ESP_OK) {
        // Log: Failed to get status info
        LOG_ERR("TWAIService", "Failed to get TWAI status info: %d", res);
    }
    return res;
}

#endif