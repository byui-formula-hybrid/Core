#include "dispatcher.h"

namespace CAN {

Dispatcher& Dispatcher::get_instance() {
    static Dispatcher instance;
    return instance;
}

void Dispatcher::enqueue(const Frame& data) {
    if (queue_rx == nullptr) {
        // Queue not set, cannot enqueue
        LOG_ERR("Dispatcher", "Queue not set, cannot enqueue frame with ID: %u", data.identifier);
        return;
    }

    queue_rx->enqueue(data);
}

void Dispatcher::dispatch() {
    if (queue_rx == nullptr) {
        // Queue not set, cannot dispatch
        LOG_ERR("Dispatcher", "Queue not set, cannot dispatch frames");
        return;
    }

    Frame data;
    if (queue_rx->dequeue(data) && data.identifier < 2048 && routes[data.identifier] != nullptr) {
        routes[data.identifier]->handle(data);
    }
}

void Dispatcher::register_route(uint32_t id, IHandler* handler) {
    if (id < 2048) routes[id] = handler;
}

} // namespace CAN