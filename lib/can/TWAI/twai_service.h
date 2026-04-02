#ifndef TWAI_SERVICE_H
#define TWAI_SERVICE_H

#if defined(ENV_ESP32)

#include <driver/twai.h>
#include <driver/gpio.h>
#include <logger.h>

#include "service.h"

using namespace CAN;

struct TWAIConfig: public BaseServiceConfig {
    const twai_general_config_t *general_config;
    const twai_timing_config_t *timing_config;
    const twai_filter_config_t *filter_config;
}

class TWAIService : public Service {
public:
    bool setup(const void * config) override;
    bool tear_down() override;
    bool send(Frame& frame) override;
    const Frame* read() override;
    bool recover() override;

    ~TWAIService() = default;
};

#endif // ENV_ESP32

#endif // TWAI_SERVICE_H