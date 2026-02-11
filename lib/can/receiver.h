#pragma once 

#include <functional>
#include <stdint.h>

#include "types.h"

namespace CAN {

/**
 * @brief A generic enum for the status of CAN devices
 * @note Device specific errors should be converted into a status error
 * 
 * @note Less than 0 is Fatal Errors
 * @note Greater than 0 is non-fatal errors/warnings
 */
enum class Status {
    OK = 0,
    // Warnings/Non-Fatal Errors
    WARN = 1,
    COMM_WARN = 2,
    // Fatal Errors
    FATAL = -1,
    COMM_ERR = -2, // TODO: Make a list of errors that should kill the car and not

};
typedef std::function<void(const Status&)> ErrorCallback;

class Receiver {
private:
    ErrorCallback onErrorCallback;
public:
    virtual Status digestFrame(const Frame&) const = 0;
    virtual void onError(const Status&) = 0;
    virtual uint32_t* getIDs() = 0;
    virtual uint32_t idCount() = 0;
};

};