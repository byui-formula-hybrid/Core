#ifndef CORE_LOGGER_H
#define CORE_LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <queue.h>

#include "i_logger.h"

namespace Core {

class Logger {
public:
    // This is the underlying function the Macro will call
    static void log(LogLevel level, const char* file, int line, const char* tag, const char* format, ...) {
        LogEntry entry;
        entry.level = level;
        entry.file = file;
        entry.line = line;
        entry.timestamp = get_system_millis(); // Implement based on ESP32 or STM32
        
        strncpy(entry.tag, tag, sizeof(entry.tag));

        va_list args;
        va_start(args, format);
        vsnprintf(entry.msg, sizeof(entry.msg), format, args);
        va_end(args);

        // Fire and forget: Push to the non-blocking queue
        get_instance()._queue->enqueue(entry);
    }

    static void set_output(ILogger* backend) {
        get_instance()._backend = backend;
    }

    static void set_queue(IQueue<LogEntry>* queue) {
        get_instance()._queue = queue;
    }

    // Call this from a low-priority background task/thread
    static void process() {
        LogEntry entry;
        // Block until a log arrives
        if (get_instance()._queue->dequeue(entry, 100)) { 
            if (get_instance()._backend) {
                // The backend handles the actual string printing
                get_instance()._backend->log(entry); 
            }
        }
    }

private:
    Logger();
    static Logger& get_instance() {
        static Logger instance;
        return instance;
    }

    IQueue<LogEntry>* _queue; 
    ILogger* _backend = nullptr;

    // Platform specific: millis() for Arduino/ESP32 or HAL_GetTick() for STM32
    static uint32_t get_system_millis(); 
};

} // namespace Core

// --- THE MACRO INTERFACE ---
// This replaces the call site with the correct File and Line info
#define LOG_DEBUG(tag, fmt, ...) Core::Logger::log(Core::LogLevel::DEBUG, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)
#define LOG_INFO(tag, fmt, ...)  Core::Logger::log(Core::LogLevel::INFO, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)
#define LOG_WARN(tag, fmt, ...)  Core::Logger::log(Core::LogLevel::WARNING, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)
#define LOG_ERR(tag, fmt, ...)   Core::Logger::log(Core::LogLevel::CRITICAL,  __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)

#endif // CORE_LOGGER_H