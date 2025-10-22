#pragma once

#include <driver/twai.h>

namespace CAN {

typedef twai_message_t Frame;

enum Speed: uint8_t {
    KBPS_1,
    KBPS_5,
    KBPS_10,
    KBPS_12_5,
    KBPS_16,
    KBPS_20,
    KBPS_50,
    KBPS_100,
    KBPS_125,
    KBPS_250,
    KBPS_500,
    KBPS_800,
    KBPS_1000
};

template<typename T>
Frame encode(uint32_t id, const T& message);

template<typename T>
T decode(const Frame& frame);

class Manager {
public:
    Manager() = default;

    void setSpeed(Speed speed);
    void setTransmitPin(uint8_t pin);
    void setReceivePin(uint8_t pin);
    void setTransmitQueueSize(uint16_t size);
    void setReceiveQueueSize(uint16_t size);

    bool begin();
    bool recover();
    bool restart();
    bool end();

    bool transmit(const Frame& frame, uint32_t timeout = portMAX_DELAY);
    bool receive(Frame& frame, uint32_t timeout = portMAX_DELAY);

    ~Manager() = default;

private:
    uint8_t transmit_pin = 21;
    uint8_t receive_pin = 22;

    uint8_t transmit = 5;
    uint8_t receive = 4;

    uint16_t transmit_queue_size = 0;
    uint16_t receive_queue_size = 0;

    twai_general_config_t general_config = TWAI_GENERAL_CONFIG_DEFAULT(transmit_pin, receive_pin, TWAI_MODE_NORMAL);
    twai_filter_config_t filter_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_timing_config_t timing_config = TWAI_TIMING_CONFIG_500KBITS();

    Speed speed = KBPS_500;
};

} // namespace CAN
