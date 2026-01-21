#pragma once

#include <stdint.h>

namespace CAN {

typedef uint32_t Tick;
typedef uint32_t Alert;

enum class Result {
    OK                      = 0,      /*!< esp_err_t value indicating success (no error) */
    FAIL                    = -1,     /*!< Generic esp_err_t code indicating failure */

    ERR_NO_MEM              = 0x101,  /*!< Out of memory */
    ERR_INVALID_ARG         = 0x102,  /*!< Invalid argument */
    ERR_INVALID_STATE       = 0x103,  /*!< Invalid state */
    ERR_INVALID_SIZE        = 0x104,  /*!< Invalid size */
    ERR_NOT_FOUND           = 0x105,  /*!< Requested resource not found */
    ERR_NOT_SUPPORTED       = 0x106,  /*!< Operation or feature not supported */
    ERR_TIMEOUT             = 0x107,  /*!< Operation timed out */
    ERR_INVALID_RESPONSE    = 0x108,  /*!< Received response was invalid */
    ERR_INVALID_CRC         = 0x109,  /*!< CRC or checksum was invalid */
    ERR_INVALID_VERSION     = 0x10A,  /*!< Version was invalid */
    ERR_INVALID_MAC         = 0x10B,  /*!< MAC address was invalid */
    ERR_NOT_FINISHED        = 0x10C,  /*!< There are items remained to retrieve */


    ERR_WIFI_BASE           = 0x3000, /*!< Starting number of WiFi error codes */
    ERR_MESH_BASE           = 0x4000, /*!< Starting number of MESH error codes */
    ERR_FLASH_BASE          = 0x6000, /*!< Starting number of flash error codes */
    ERR_HW_CRYPTO_BASE      = 0xc000, /*!< Starting number of HW cryptography module error codes */
    ERR_MEMPROT_BASE        = 0xd000 /*!< Starting number of Memory Protection API error codes */
};

enum class State {
    STOPPED,             /**< Stopped state. The TWAI controller will not participate in any TWAI bus activities */
    RUNNING,             /**< Running state. The TWAI controller can transmit and receive messages */
    BUS_OFF,             /**< Bus-off state. The TWAI controller cannot participate in bus activities until it has recovered */
    RECOVERING,          /**< Recovering state. The TWAI controller is undergoing bus recovery */
};

enum class Mode {
    NORMAL,               /**< Normal operating mode where TWAI controller can send/receive/acknowledge messages */
    NO_ACK,               /**< Transmission does not require acknowledgment. Use this mode for self testing */
    LISTEN_ONLY,          /**< The TWAI controller will not influence the bus (No transmissions or acknowledgments) but can receive messages */
};

enum PIN {
    UNUSED = -1,    /*!< Use to signal not connected to S/W */
    NUM_0 = 0,     /*!< GPIO0, input and output */
    NUM_1 = 1,     /*!< GPIO1, input and output */
    NUM_2 = 2,     /*!< GPIO2, input and output */
    NUM_3 = 3,     /*!< GPIO3, input and output */
    NUM_4 = 4,     /*!< GPIO4, input and output */
    NUM_5 = 5,     /*!< GPIO5, input and output */
    NUM_6 = 6,     /*!< GPIO6, input and output */
    NUM_7 = 7,     /*!< GPIO7, input and output */
    NUM_8 = 8,     /*!< GPIO8, input and output */
    NUM_9 = 9,     /*!< GPIO9, input and output */
    NUM_10 = 10,   /*!< GPIO10, input and output */
    NUM_11 = 11,   /*!< GPIO11, input and output */
    NUM_12 = 12,   /*!< GPIO12, input and output */
    NUM_13 = 13,   /*!< GPIO13, input and output */
    NUM_14 = 14,   /*!< GPIO14, input and output */
    NUM_15 = 15,   /*!< GPIO15, input and output */
    NUM_16 = 16,   /*!< GPIO16, input and output */
    NUM_17 = 17,   /*!< GPIO17, input and output */
    NUM_18 = 18,   /*!< GPIO18, input and output */
    NUM_19 = 19,   /*!< GPIO19, input and output */
    NUM_20 = 20,   /*!< GPIO20, input and output */
    NUM_21 = 21,   /*!< GPIO21, input and output */
    NUM_26 = 26,   /*!< GPIO26, input and output */
    NUM_27 = 27,   /*!< GPIO27, input and output */
    NUM_28 = 28,   /*!< GPIO28, input and output */
    NUM_29 = 29,   /*!< GPIO29, input and output */
    NUM_30 = 30,   /*!< GPIO30, input and output */
    NUM_31 = 31,   /*!< GPIO31, input and output */
    NUM_32 = 32,   /*!< GPIO32, input and output */
    NUM_33 = 33,   /*!< GPIO33, input and output */
    NUM_34 = 34,   /*!< GPIO34, input and output */
    NUM_35 = 35,   /*!< GPIO35, input and output */
    NUM_36 = 36,   /*!< GPIO36, input and output */
    NUM_37 = 37,   /*!< GPIO37, input and output */
    NUM_38 = 38,   /*!< GPIO38, input and output */
    NUM_39 = 39,   /*!< GPIO39, input and output */
    NUM_40 = 40,   /*!< GPIO40, input and output */
    NUM_41 = 41,   /*!< GPIO41, input and output */
    NUM_42 = 42,   /*!< GPIO42, input and output */
    NUM_43 = 43,   /*!< GPIO43, input and output */
    NUM_44 = 44,   /*!< GPIO44, input and output */
    NUM_45 = 45,   /*!< GPIO45, input and output */
    NUM_46 = 46,   /*!< GPIO46, input and output */
    NUM_47 = 47,   /*!< GPIO47, input and output */
    NUM_48 = 48,   /*!< GPIO48, input and output */
    NUM_MAX,
};

struct Frame {
    union {
        struct {
            // The order of these bits must match deprecated message flags for compatibility reasons
            uint32_t extd: 1;           /**< Extended Frame Format (29bit ID) */
            uint32_t rtr: 1;            /**< Message is a Remote Frame */
            uint32_t ss: 1;             /**< Transmit as a Single Shot Transmission. Unused for received. */
            uint32_t self: 1;           /**< Transmit as a Self Reception Request. Unused for received. */
            uint32_t dlc_non_comp: 1;   /**< Message's Data length code is larger than 8. This will break compliance with ISO 11898-1 */
            uint32_t reserved: 27;      /**< Reserved bits */
        };
        uint32_t flags;                 /**< Deprecated: Alternate way to set bits using message flags */
    };
    uint32_t identifier;                /**< 11 or 29 bit identifier */
    uint8_t data_length_code;           /**< Data length code */
    uint8_t data[8];    /**< Data bytes (not relevant in RTR frame) */

    Frame() {}

    template<typename T>
    Frame(uint32_t identifier, T* message) {
        this->data_length_code = 8;
        this->identifier = identifier;

        const uint8_t* msg_ptr = (const uint8_t*) message;
        this->data[0] = msg_ptr[0];
        this->data[1] = msg_ptr[1];
        this->data[2] = msg_ptr[2];
        this->data[3] = msg_ptr[3];
        this->data[4] = msg_ptr[4];
        this->data[5] = msg_ptr[5];
        this->data[6] = msg_ptr[6];
        this->data[7] = msg_ptr[7];
    }

    Frame(uint32_t identifier, uint8_t* data) {
        this->data_length_code = 8;
        this->identifier = identifier;
        this->data[0] = data[0];
        this->data[1] = data[1];
        this->data[2] = data[2];
        this->data[3] = data[3];
        this->data[4] = data[4];
        this->data[5] = data[5];
        this->data[6] = data[6];
        this->data[7] = data[7];
    }

    template<typename T>
    T* decode() {
        return (T*) data;
    }
};

struct FilterConfig {
    uint32_t acceptance_code;       /**< 32-bit acceptance code */
    uint32_t acceptance_mask;       /**< 32-bit acceptance mask */
    bool single_filter;             /**< Use Single Filter Mode (see documentation) */

    FilterConfig() {
        acceptance_code = 0;
        acceptance_mask = 0xFFFFFFFF;
        single_filter = true;
    }
};

struct TimingConfig {
    uint32_t brp;                   /**< Baudrate prescaler (i.e., APB clock divider). Any even number from 2 to 128 for ESP32, 2 to 32768 for ESP32S2.
                                         For ESP32 Rev 2 or later, multiples of 4 from 132 to 256 are also supported */
    uint8_t tseg_1;                 /**< Timing segment 1 (Number of time quanta, between 1 to 16) */
    uint8_t tseg_2;                 /**< Timing segment 2 (Number of time quanta, 1 to 8) */
    uint8_t sjw;                    /**< Synchronization Jump Width (Max time quanta jump for synchronize from 1 to 4) */
    bool triple_sampling;           /**< Enables triple sampling when the TWAI controller samples a bit */

    TimingConfig() {
        brp = 8;
        tseg_1 = 15; 
        tseg_2 = 4; 
        sjw = 3; 
        triple_sampling = false;
    }
};

struct StatusInfo {
    State state;             /**< Current state of TWAI controller (Stopped/Running/Bus-Off/Recovery) */
    uint32_t msgs_to_tx;            /**< Number of messages queued for transmission or awaiting transmission completion */
    uint32_t msgs_to_rx;            /**< Number of messages in RX queue waiting to be read */
    uint32_t tx_error_counter;      /**< Current value of Transmit Error Counter */
    uint32_t rx_error_counter;      /**< Current value of Receive Error Counter */
    uint32_t tx_failed_count;       /**< Number of messages that failed transmissions */
    uint32_t rx_missed_count;       /**< Number of messages that were lost due to a full RX queue (or errata workaround if enabled) */
    uint32_t rx_overrun_count;      /**< Number of messages that were lost due to a RX FIFO overrun */
    uint32_t arb_lost_count;        /**< Number of instances arbitration was lost */
    uint32_t bus_error_count;       /**< Number of instances a bus error has occurred */
};

struct GeneralConfig {
    Mode mode;                      /**< Mode of TWAI controller */
    PIN tx_io;                      /**< Transmit GPIO number */
    PIN rx_io;                      /**< Receive GPIO number */
    PIN clkout_io;                  /**< CLKOUT GPIO number (optional, set to -1 if unused) */
    PIN bus_off_io;                 /**< Bus off indicator GPIO number (optional, set to -1 if unused) */
    uint32_t tx_queue_len;          /**< Number of messages TX queue can hold (set to 0 to disable TX Queue) */
    uint32_t rx_queue_len;          /**< Number of messages RX queue can hold */
    uint32_t alerts_enabled;        /**< Bit field of alerts to enable (see documentation) */
    uint32_t clkout_divider;        /**< CLKOUT divider. Can be 1 or any even number from 2 to 14 (optional, set to 0 if unused) */
    int intr_flags;                 /**< Interrupt flags to set the priority of the driver's ISR. Note that to use the ESP_INTR_FLAG_IRAM, the CONFIG_TWAI_ISR_IN_IRAM option should be enabled first. */

    GeneralConfig(PIN transmit_pin, PIN recieve_pin, Mode mode) {
        mode = mode;
        tx_io = transmit_pin;
        rx_io = recieve_pin;
        clkout_io = UNUSED;
        bus_off_io = UNUSED;
        tx_queue_len = 5; 
        rx_queue_len = 5;
        alerts_enabled = 0; // NONE  
        clkout_divider = 0;
        intr_flags = (1<<1); // LEVEL 1
    }
};

} // namespace CAN