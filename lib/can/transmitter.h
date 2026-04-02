#ifndef CAN_TRANSMITTER_H
#define CAN_TRANSMITTER_H

#include <queue.h>
#include <logger.h>

#include "service.h"
#include "types.h"

namespace CAN {

class Transmitter {
public:
    static Transmitter& get_instance();
    Transmitter(const Transmitter&) = delete;
    Transmitter& operator=(const Transmitter&) = delete;

    bool send(const Frame& frame);
    void set_service(Service* service);
    void set_queue(Core::IQueue<Frame>* queue);
    void transmit();

private:
    Transmitter();
    Core::IQueue<Frame>* queue_tx;
    Service* service;
};

}

#endif // CAN_TRANSMITTER_H