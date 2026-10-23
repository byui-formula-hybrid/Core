// This is the umbrella header for the CAN library. It includes all the necessary headers for using the CAN library.
#ifndef CORE_CAN_H
#define CORE_CAN_H

#if defined(ENABLE_VALIDATE_CAN)
#if defined(IS_NATIVE)

#include <unity.h>
#define VALIDATE_CAN(condition) \
            do { \
                TEST_ASSERT_TRUE_MESSAGE((condition), "CAN Validation failed: "#condition); \
            } while(false)

#else

// Error_Handler should never return, it really should force a shutdown if we get here or just hang in debug situation
[[noreturn]] extern void Error_Handler(); // Use extern so that the linker will look for a matching function

#define VALIDATE_CAN(condition)         \
            do {                        \
                if(!(condition)) {      \
                    error_handler();    \
                }                       \
            } while(false)
#endif

#else

#define VALIDATE_CAN(condition) do {} while(false)

#endif

// NOTE: Due to the inclusion of multiple lib folders in hardware, these need to be relative
#include "../lib/can/dispatcher.h"
#include "../lib/can/service.h"
#include "../lib/can/transmitter.h"
#include "../lib/can/types.h"

#endif // CAN_H
