#include "watchdog.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

Watchdog::Watchdog(ErrorCallback callback) : onErrorCallback(callback) {}

Status Watchdog::digestFrame(const Frame& frame) const {
    uint32_t id = frame.identifier >> 5;

    switch(id) {
        case 0x22:
            return handleMessage22(frame);
        default:
            break;
    }
    return Status::OK;
}

Status Watchdog::handleMessage22(const Frame& frame) const {
    auto message = frame.decode<Message22>();

    // We can expand this to cover all errors, status just doesn't have much support yet
    if(message->fault_code != FaultCodes::NONE) {
        return Status::FATAL;
    }

    return Status::OK;
}

void Watchdog::onError(const Status& status) {
    onErrorCallback(status);
}

uint32_t* Watchdog::getIDs() {
    uint32_t ids[] = { 0x52 } ;
    return ids;
}

uint32_t Watchdog::idCount() {
    return 1;
}

};
};