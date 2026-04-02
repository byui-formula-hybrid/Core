#ifndef CORE_LOGGER_H
#define CORE_LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <queue.h>

#include "i_logger.h"
#include "i_timestamp_provider.h"
#include "types.h"

namespace Core {

// TODO: Use the flush method as necessary
/**
* @brief A simple logger class for handling log entries
*/
class Logger {
public:
    /**
     * @brief Logs a message with the specified level and metadata
     * @param level: The log level
     * @param file: The file name where the log is called
     * @param line: The line number where the log is called
     * @param tag: A tag for categorizing the log
     * @param format: The format string for the log message
     * @param ...: Variable arguments for the format string
     */
    static void log(LogLevel level, const char* file, int line, const char* tag, const char* format, ...) {
        Logger instance = get_instance();
        LogEntry entry;
        entry.level = level;
        entry.file = file;
        entry.line = line;
        entry.timestamp = instance.time_provider->get_timestamp(); // Implement based on ESP32 or STM32
        
        strncpy(entry.tag, tag, sizeof(entry.tag));

        va_list args;
        va_start(args, format);
        vsnprintf(entry.msg, sizeof(entry.msg), format, args);
        va_end(args);

        // Fire and forget: Push to the non-blocking queue
        instance.queue->enqueue(entry);
    }

    /**
     * @brief Sets the output backend for the logger
     * @param backend: The logger backend to use
     */
    static void set_output(ILogger* backend) {
        get_instance().backend = backend;
    }

    /**
     * @brief Sets the queue for the logger
     * @param queue: The queue to use
     */
    static void set_queue(IQueue<LogEntry>* queue) {
        get_instance().queue = queue;
    }

    static void set_time_provider(ITimeStampProvider* provider) {
        get_instance().time_provider = provider;
    }

    /**
     * @brief Processes the log queue
     * This should be called in a loop in the main thread to ensure logs are printed.
     */
    static void process() {
        Logger instance = get_instance();
        LogEntry entry;
        // Block until a log arrives
        if (instance.queue->dequeue(entry, instance.max_timeout_ms)) { 
            if (instance.backend) {
                // The backend handles the actual string printing
                instance.backend->log(entry); 
            }
        }
    }

    /**
     * @brief Sets the maximum timeout for log processing
     * @param timeout_ms: The maximum timeout in milliseconds
     */
    static void set_max_timeout(uint32_t timeout_ms) {
        get_instance().max_timeout_ms = timeout_ms;
    }

private:
    /**
     * @brief Constructor for the logger.
     */
    Logger();

    /**
     * @brief Returns the singleton instance of the logger.
     * @return instance: the singleton instance of the logger.
     */
    static Logger& get_instance() {
        static Logger instance;
        return instance;
    }

    /**
     * @brief The queue for storing log entries.
     */
    IQueue<LogEntry>* queue; 
    
    /**
     * @brief The logger backend for outputting log entries.
     */
    ILogger* backend = nullptr;
    
    /**
     * @brief The time stamp provider for generating time stamps.
     */
    ITimeStampProvider* time_provider;
    
    /**
     * @brief The maximum timeout for log processing.
     */
    uint32_t max_timeout_ms = 100;
};

} // namespace Core

// --- THE MACRO INTERFACE ---
// This replaces the call site with the correct File and Line info
#define LOG_DEBUG(tag, fmt, ...) Core::Logger::log(Core::LogLevel::DEBUG, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)
#define LOG_INFO(tag, fmt, ...)  Core::Logger::log(Core::LogLevel::INFO, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)
#define LOG_WARN(tag, fmt, ...)  Core::Logger::log(Core::LogLevel::WARNING, __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)
#define LOG_ERR(tag, fmt, ...)   Core::Logger::log(Core::LogLevel::CRITICAL,  __FILE__, __LINE__, tag, fmt, ##__VA_ARGS__)

#endif // CORE_LOGGER_H