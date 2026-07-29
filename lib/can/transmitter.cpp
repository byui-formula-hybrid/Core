#include "transmitter.h"

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

bool Transmitter::send(const Frame& frame) {
    if (service == nullptr) {
        // Service not set, cannot send
        //LOG_ERR("Transmitter", "Service not set, cannot send frame with ID: %u", frame.identifier);
        printf("Transmitter: Service not set, cannot send frame with ID: %u\n", frame.identifier);
        return false;
    }

    return queue_tx->enqueue(frame);
}

void Transmitter::transmit(void* data) {
    Transmitter* self = (Transmitter*)data;
    while(true) {

#ifdef IS_NATIVE
        if(self->should_kill_thread) {
            break;
        }
#endif

        if (self->queue_tx == nullptr) {
            // Service or queue not set, cannot process
            //LOG_ERR("Transmitter", "Queue not set, cannot transmit frames");
            printf("Transmitter: Queue not set, cannot transmit frames");
            continue;
        }

        if (self->service == nullptr) {
            // Service or queue not set, cannot process
            //LOG_ERR("Transmitter", "Service not set, cannot transmit frames");
            printf("Transmitter: Service not set, cannot transmit frames");
            continue;
        }

        if(!self->service->can_send())
            // TxMailbox is likely full, we just have to continue until it's open again
            continue;

        Frame frame;
        if (self->queue_tx->dequeue(frame)) {
            self->service->send(frame);
        }
    }
}

} // namespace CAN
