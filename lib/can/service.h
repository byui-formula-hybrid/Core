
#ifndef CAN_SERVICE_H
#define CAN_SERVICE_H

#include <stdint.h>
#include <memory>
#include "types.h"
#include "dispatcher.h"

// TODO: Handle Errors

namespace CAN {

class Old_Service {
public: 
    virtual const Result install_driver(const GeneralConfig *g_config, const TimingConfig *t_config, const FilterConfig *f_config) = 0;
    virtual const Result uninstall_driver() = 0;
    virtual const Result start() = 0;
    virtual const Result stop() = 0;
    virtual const Result transmit(const Frame *frame, Tick ticks_to_wait) = 0;
    virtual const Result receive(Frame *frame, Tick ticks_to_wait) = 0;
    virtual const Result alerts(Alert *alerts, Tick ticks_to_wait) = 0;
    virtual const Result reconfigure_alerts(Alert alerts_enabled, Alert *current_alerts) = 0;
    virtual const Result initiate_recovery() = 0;
    virtual const Result status_info(StatusInfo *status_info) = 0;
    virtual const Result clear_transmit_queue() = 0;
    virtual const Result clear_receive_queue() = 0;
    virtual const Result reset_pin(const PIN pin) = 0;

    virtual ~Old_Service() = default;
};

class Service {
public:
    virtual void setup() = 0;
    virtual void stop_listening() = 0;
    virtual void start_listening() = 0;
    virtual void digest_read() = 0; // Grabs from dispatcher
    virtual void raw_send(void *data) = 0;
    virtual void raw_read(void *data) = 0;
    virtual void recover() = 0;

    ~Service() = default;
private:
    std::shared_ptr<Dispatcher> m_dispatcher; // dispatcher is a singleton, so Service doesn't own it
};


} // namespace CAN

#endif // CAN_SERVICE_H