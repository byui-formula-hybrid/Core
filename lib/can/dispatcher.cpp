#include "dispatcher.h"
#include "core/queue/i_queue.h"

namespace CAN {

Dispatcher* Dispatcher::get_instance() {
    static Dispatcher instance;
    return &instance;
}

void Dispatcher::enqueue(const Frame& data) {
    if (queue_rx == nullptr) {
        // Queue not set, cannot enqueue
        LOG_ERR("Dispatcher", "Queue not set, cannot enqueue frame with ID: %u", data.identifier);
        return;
    }

    queue_rx->enqueueFromISR(data);
}

void Dispatcher::dispatch(void* data) {
    Dispatcher* self = (Dispatcher*)data;
    while(true) {
        if (self->queue_rx == nullptr) {
            // Queue not set, cannot dispatch
            LOG_ERR("Dispatcher", "Queue not set, cannot dispatch frames");
            continue;
        }

        Frame data;
        if (self->queue_rx->dequeue(data) == Core::QueueError::SUCCESS ) {
            printf("Recieved CAN Frame with id: %x", data.identifier); // Temporary

            // TODO: Not working
            if(data.identifier < 2048 && self->routes[data.identifier] != nullptr)
               self->routes[data.identifier]->handle(data);
        }
    }
}

void Dispatcher::register_route(uint32_t id, IHandler* handler) {
    if (id < 2048) routes[id] = handler;
}

} // namespace CAN
