#pragma once

#include "../../can/types.h"

#include "messages.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

class Watchdog {
public:
    ErrorCallback onErrorCallback;
public:
    Watchdog(ErrorCallback callback);

    Status readFrame(const Frame& frame);

    void onError(const Status& error);
private:

    Status handleMessage22(const Frame& frame);
};

};
};