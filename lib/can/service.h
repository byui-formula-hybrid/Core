
#ifndef CAN_SERVICE_H
#define CAN_SERVICE_H

#include <stdint.h>
#include <memory>
#include "types.h"
#include "dispatcher.h"

namespace CAN {

class Service {
public:
    virtual bool setup(const void * config) = 0;
    virtual bool tear_down() = 0;
    virtual bool send(const Frame& frame) = 0;
    virtual const Frame* read() = 0;
    virtual bool recover() = 0;

    ~Service() = default;
private:
    Dispatcher& dispatcher = Dispatcher::get_instance();
};


} // namespace CAN

#endif // CAN_SERVICE_H