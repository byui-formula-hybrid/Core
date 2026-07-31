#ifndef CORE_LOGGER_SERIAL_LOGGER_H
#define CORE_LOGGER_SERIAL_LOGGER_H

#include <stdio.h>
#include "core/logger.h"

class SerialLogger : public Core::ILogger {
public:
    void log(const Core::LogEntry& e) override {
        // Format: [Timestamp] [LEVEL] [TAG] [File:Line] Message
        printf(
            "[%lu] [%s] [%s] [%s:%d] %s\n", 
            e.timestamp, 
            levelToString(e.level), 
            e.tag, 
            e.file, 
            e.line, 
            e.msg
        );
    }

private:
    const char* levelToString(Core::LogLevel l) {
        switch(l) {
            case Core::LogLevel::INFO: return "INFO";
            case Core::LogLevel::WARN: return "WARN";
            case Core::LogLevel::ERR:  return "ERROR";
            default: return "DEBUG";
        }
    }
};

#endif // CORE_LOGGER_SERIAL_LOGGER_H