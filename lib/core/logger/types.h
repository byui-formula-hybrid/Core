#ifndef CORE_LOGGER_TYPES_H
#define CORE_LOGGER_TYPES_H

#include <stdint.h>

namespace Core {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    CRITICAL
};

struct LogEntry {
    LogLevel level;
    uint32_t timestamp; // System millis
    int line;
    const char* file;   // Pointer to string literal (Flash)
    char tag[16];
    char msg[64];       // Fixed size to avoid heap allocation
};

} // namespace Core

#endif // CORE_LOGGER_TYPES_H