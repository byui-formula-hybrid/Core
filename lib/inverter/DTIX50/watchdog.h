#pragma once

#include "../../can/receiver.h"

#include "messages.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

class Watchdog : public Receiver {
public:
    ErrorCallback onErrorCallback;
public:
    Watchdog(ErrorCallback callback);

    Status digestFrame(const Frame& frame) const override;

    void onError(const Status& error);
private:

    Status handleMessage22(const Frame& frame) const;
};

};
};