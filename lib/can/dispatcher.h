#pragma once

#include <map>
#include <memory>
#include <queue.h>
#include <logger.h>

#include "types.h"

class watchdog;

namespace CAN {

class Dispatcher {
public:
    static Dispatcher& get_instance();
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

    void enqueue(const Frame& data);
    void register_route(uint32_t id, IHandler* handler);
    void dispatch();
    void set_queue(Core::IQueue<Frame>* q) { queue_rx = q; }
private:
    Dispatcher();
    Core::IQueue<Frame>* queue_rx;
    // O(1) Lookup: Array of pointers to handlers (Size 2048 for 11-bit IDs)
    IHandler* routes[2048];
};

} // namespace CAN