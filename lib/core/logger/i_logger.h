#ifndef CORE_LOGGER_I_LOGGER_H
#define CORE_LOGGER_I_LOGGER_H

#include "types.h"

namespace Core {

/**
 * @brief Interface for hardware-specific log outputs (Serial, SD Card, etc.)
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
     * @brief Consumes a pre-filled LogEntry.
     * @param entry The entry containing msg, timestamp, file, line, and level.
     */
    virtual void log(const LogEntry& entry) = 0;

    /**
     * @brief Optional: Flush any pending writes (useful for SD cards before a crash)
     */
    virtual void flush() {}
};

} // namespace Core

#endif // CORE_LOGGER_I_LOGGER_H