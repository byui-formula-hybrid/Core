#include "transmitter.h"

namespace CAN {

Transmitter& Transmitter::get_instance() {
    static Transmitter instance;
    return instance;
}

void Transmitter::set_service(Service* s) {
    service = s;
}

void Transmitter::set_queue(Core::IQueue<Frame>* queue) {
    queue_tx = queue;
}

bool Transmitter::send(const Frame& frame) {
    if (service == nullptr) {
        // Service not set, cannot send
        LOG_ERR("Transmitter", "Service not set, cannot send frame with ID: %u", frame.identifier);
        return false;
    }

    return service->send(frame);
}

void Transmitter::transmit() {
    if (queue_tx == nullptr) {
        // Service or queue not set, cannot process
        LOG_ERR("Transmitter", "Queue not set, cannot transmit frames");
        return;
    }
    
    if (service == nullptr) {
        // Service or queue not set, cannot process
        LOG_ERR("Transmitter", "Service not set, cannot transmit frames");
        return;
    }

    Frame frame;
    if (queue_tx->dequeue(frame)) {
        service->send(frame);
    }
};

} // namespace CAN