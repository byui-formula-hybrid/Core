#pragma once

#include <driver/twai.h>

namespace CAN {

typedef twai_message_t Frame;

/*
 * Encodes a message of type T into a CAN frame.
 * @param id The identifier for the CAN frame.
 * @param message The message of type T to encode.
 * @returns The encoded CAN frame.
 */
template<typename T>
Frame encode(uint32_t id, const T& message) {
    const uint8_t* msg_ptr = (const uint8_t*) &message;
    Frame frame;
    frame.identifier = id;
    frame.data_length_code = 8;
    frame.data[0] = msg_ptr[0];
    frame.data[1] = msg_ptr[1];
    frame.data[2] = msg_ptr[2];
    frame.data[3] = msg_ptr[3];
    frame.data[4] = msg_ptr[4];
    frame.data[5] = msg_ptr[5];
    frame.data[6] = msg_ptr[6];
    frame.data[7] = msg_ptr[7];
    return frame;
}

/*
 * Decodes a CAN frame into a message of type T.
 * @param frame The CAN frame to decode.
 * @returns The decoded message of type T.
 */ 
template<typename T>
T decode(const Frame& frame) {
    T message;
    uint8_t* msg_ptr = (uint8_t*) &message;
    msg_ptr[0] = frame.data[0];
    msg_ptr[1] = frame.data[1];
    msg_ptr[2] = frame.data[2];
    msg_ptr[3] = frame.data[3];
    msg_ptr[4] = frame.data[4];
    msg_ptr[5] = frame.data[5];
    msg_ptr[6] = frame.data[6];
    msg_ptr[7] = frame.data[7];
    return message;
}

/*
 * Manager class for handling CAN operations.
 */
struct Manager {
    /*
     * Sets the current status of the CAN manager.
     * @returns true if status was set successfully, false otherwise.
     */
    bool set_status();

    /*
     * Initializes the CAN manager.
     * @returns true if initialization was successful, false otherwise.
     */
    bool begin();

    /*
     * Recovers the CAN manager from a bus-off state.
     * @returns true if recovery was successful, false otherwise.
     */
    bool recover();

    /*
     * Restarts the CAN manager.
     * @returns true if restart was successful, false otherwise.
     */
    bool restart();


    /*
     * Ends the CAN manager.
     * @returns true if end was successful, false otherwise.
     */
    bool end();

    /*
     * Transmits a CAN frame.
     * @param frame The CAN frame to transmit.
     * @param timeout The timeout for transmission in milliseconds.
     * @returns true if transmission was successful, false otherwise.
     */
    bool transmit(const Frame& frame, uint32_t timeout = 1000);
    
    /*
     * Receives a CAN frame.
     * @param frame The CAN frame to receive.
     * @param timeout The timeout for reception in milliseconds.
     * @returns true if reception was successful, false otherwise.
     */
    bool receive(Frame& frame, uint32_t timeout = 1000);

    /*
     * Installs the CAN driver.
     * @returns true if installation was successful, false otherwise.
     */
    bool install_driver();

    /*
     * Uninstalls the CAN driver.
     * @returns true if uninstallation was successful, false otherwise.
     */
    bool uninstall_driver();

    // Indicates whether the CAN manager is currently running.
    bool is_running = false;
    // TWAI status information.
    uint8_t transmit_pin = -1;
    // Receive pin for the CAN manager.
    uint8_t receive_pin = -1;
    // Transmit queue size.
    uint16_t transmit_queue_size = 5;
    // Receive queue size.
    uint16_t receive_queue_size = 5;
    // Filter configuration for the CAN manager.
    twai_filter_config_t filter_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    // Timing configuration for the CAN manager.
    twai_timing_config_t timing_config = TWAI_TIMING_CONFIG_500KBITS();
    // Status information for the CAN manager.
    twai_status_info_t status;
};

} // namespace CAN
