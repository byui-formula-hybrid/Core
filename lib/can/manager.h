#pragma once

#include "twai_provider.h"
#include <stdint.h>

using namespace TWAI;

namespace CAN {

/*
 * Manager class for handling CAN operations.
 */
class Manager {
public:
    // Indicates whether the CAN manager is currently running.
    bool is_running = false;
    // TWAI status information.
    PIN transmit_pin;
    // Receive pin for the CAN manager.
    PIN receive_pin;
    // Transmit queue size.
    uint16_t transmit_queue_size = 5;
    // Receive queue size.
    uint16_t receive_queue_size = 5;
    // Filter configuration for the CAN manager.
    FilterConfig filter_config = FilterConfig(); // TWAI_FILTER_CONFIG_ACCEPT_ALL();
    // Timing configuration for the CAN manager.
    TimingConfig timing_config = TimingConfig(); // TWAI_TIMING_CONFIG_500KBITS();
    // Status information for the CAN manager.
    StatusInfo status;

    Manager(TwaiProvider* provider, PIN transmit_pin, PIN receive_pin) : transmit_pin(transmit_pin), receive_pin(receive_pin) {}
    Manager(TwaiProvider* provider) : provider(provider), transmit_pin(UNUSED), receive_pin(UNUSED) {}

    ~Manager() = default;

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

    private:
    // Provides a wrapped implementation of the TWAI interface
    TwaiProvider* provider;

    /*
     * Sets the current status of the CAN manager.
     * @returns true if status was set successfully, false otherwise.
     */
    bool set_status();
};

} // namespace CAN
