#include "transmitter.h"
#include "core/core_error.h"
#include "core/queue/i_queue.h"

namespace CAN {

Transmitter* Transmitter::get_instance() {
    static Transmitter instance;
    return &instance;
}

void Transmitter::set_service(Service* s) {
    service = s;
}

void Transmitter::set_queue(Core::IQueue<Frame>* queue) {
    queue_tx = queue;
}

Core::Result Transmitter::send(const Frame& frame) {
    if (service == nullptr) {
        // Service not set, cannot send
        LOG_ERR("Transmitter", "Service not set, cannot send frame with ID: %u", frame.identifier);
        return Core::Result(0, Core::ErrorType::CLASS_NOT_INITIALIZED);
    }

    auto res = queue_tx->enqueue(frame);

    if(res == Core::QueueError::SUCCESS)
        return Core::Result(0, Core::ErrorType::SUCCESS);
    else {
        return Core::Result(static_cast<int>(res), Core::ErrorType::QUEUE_ERROR);
    }
}

void Transmitter::transmit(void* data) {
    Transmitter* self = (Transmitter*)data;
    while(true) {

#ifdef IS_NATIVE
        // This is purely for native testing environments
        if(self->should_kill_thread) {
            break;
        }
#endif

        if (self->queue_tx == nullptr) {
            // Service or queue not set, cannot process
            LOG_ERR("Transmitter", "Queue not set, cannot transmit frames");
            continue;
        }

        if (self->service == nullptr) {
            // Service or queue not set, cannot process
            LOG_ERR("Transmitter", "Service not set, cannot transmit frames");
            continue;
        }

        if(!self->service->can_send())
            // TxMailbox is likely full, we just have to continue until it's open again
            continue;

        Frame frame;
        if (self->queue_tx->dequeue(frame) == Core::QueueError::SUCCESS) {
            self->service->send(frame);
        }
    }
}

} // namespace CAN
