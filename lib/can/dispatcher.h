#pragma once

#include <map>
#include <memory>

#include "../core/queue/queue.h"
#include "types.h"

class watchdog;

namespace CAN {

class Dispatcher {
public:
    const void send(const Frame* data);
    const void digest_read(void *data);
    // This means that the dispatcher owns the watchdogs, not sure if that is what we want
    void register_route(std::unique_ptr<watchdog> watchdog);
    void unregister_route();

    const dispatcher* getInstance();
    dispatcher(const dispatcher&) = delete;
    dispatcher& operator=(const dispatcher&) = delete;
private:
    dispatcher();
    Core::queue queue_rx;
    Core::queue queue_tx;
    std::map<uint32_t id, std::unique_ptr<watchdog>> node_routes;
};

} // namespace CAN