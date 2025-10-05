# Core Utilities and Shared Functionality

## Purpose
This document defines the core utilities and shared functionality that provide the foundation for all other modules in the ESP32 Formula Hybrid Core Library.

## Scope
- System initialization and configuration
- Common data types and enumerations
- Utility functions and helpers
- Error handling and logging
- Time management and scheduling
- Memory management utilities

## Safety Considerations
⚠️ **CRITICAL**: Core utilities form the foundation of all safety-critical systems. Any failure in core functionality could cascade to all dependent modules.

## Module Interface

### Class Definition
```cpp
class Core {
public:
    // System lifecycle
    bool begin();
    void update();
    void shutdown();
    
    // System status
    bool isSystemReady();
    SystemState getSystemState();
    uint32_t getUptime();
    
    // Error handling
    void logError(ErrorLevel level, const char* message);
    void logWarning(const char* message);
    void logInfo(const char* message);
    ErrorCode getLastError();
    
    // Time management
    uint32_t getMillis();
    uint32_t getMicros();
    void delayMs(uint32_t ms);
    void delayUs(uint32_t us);
    
    // Configuration
    bool loadConfiguration();
    bool saveConfiguration();
    void resetToDefaults();
    
    // Watchdog
    void feedWatchdog();
    void enableWatchdog(uint32_t timeoutMs);
    void disableWatchdog();
    
private:
    // Implementation details
};
```

### System Configuration
```cpp
struct SystemConfig {
    // Hardware configuration
    uint8_t hardwareVersion;
    uint8_t softwareVersion[3];  // Major.Minor.Patch
    uint32_t serialNumber;
    
    // Communication settings
    uint32_t canBaudRate;
    uint32_t serialBaudRate;
    bool debugMode;
    
    // Safety parameters
    uint32_t watchdogTimeout;
    uint32_t emergencyTimeout;
    bool safetySystemEnabled;
    
    // Performance settings
    uint16_t mainLoopFrequency;
    uint16_t canUpdateFrequency;
    uint16_t displayUpdateFrequency;
    
    // Calibration data
    float voltageCalibration;
    float currentCalibration;
    float temperatureOffset;
};
```

## Common Data Types

### System States
```cpp
enum SystemState {
    STATE_UNINITIALIZED = 0,
    STATE_INITIALIZING = 1,
    STATE_READY = 2,
    STATE_RUNNING = 3,
    STATE_WARNING = 4,
    STATE_FAULT = 5,
    STATE_EMERGENCY = 6,
    STATE_SHUTDOWN = 7
};
```

### Error Handling
```cpp
enum ErrorLevel {
    ERROR_INFO = 0,
    ERROR_WARNING = 1,
    ERROR_ERROR = 2,
    ERROR_CRITICAL = 3,
    ERROR_FATAL = 4
};

enum ErrorCode {
    ERR_NONE = 0,
    ERR_INIT_FAILED = 1,
    ERR_COMM_TIMEOUT = 2,
    ERR_SENSOR_FAULT = 3,
    ERR_OVERCURRENT = 4,
    ERR_OVERVOLTAGE = 5,
    ERR_OVERTEMPERATURE = 6,
    ERR_MEMORY_FAULT = 7,
    ERR_WATCHDOG_TIMEOUT = 8,
    ERR_SAFETY_VIOLATION = 9,
    ERR_CONFIG_INVALID = 10
};
```

### Safety Status
```cpp
struct SafetyStatus {
    bool imdStatus;           // Insulation monitoring
    bool emergencyStop;       // Emergency stop button
    bool thermalProtection;   // Temperature limits
    bool currentProtection;   // Current limits
    bool voltageProtection;   // Voltage limits
    bool systemIntegrity;     // Overall system health
    uint32_t lastCheck;       // Last safety check timestamp
};
```

## Utility Functions

### Math Utilities
```cpp
class MathUtils {
public:
    // Filtering
    static float lowPassFilter(float input, float previous, float alpha);
    static float movingAverage(float input, float* buffer, uint8_t size);
    static float medianFilter(float* values, uint8_t count);
    
    // Scaling and conversion
    static float mapFloat(float value, float inMin, float inMax, float outMin, float outMax);
    static uint16_t adcToVoltage(uint16_t adcValue, float vRef, uint8_t bits);
    static float celsiusToFahrenheit(float celsius);
    static float fahrenheitToCelsius(float fahrenheit);
    
    // Validation
    static bool isInRange(float value, float min, float max);
    static bool isValidFloat(float value);
    static float constrain(float value, float min, float max);
    
    // Statistics
    static float calculateRMS(float* values, uint8_t count);
    static float calculateMean(float* values, uint8_t count);
    static float calculateStdDev(float* values, uint8_t count);
};
```

### String Utilities
```cpp
class StringUtils {
public:
    static void formatFloat(char* buffer, float value, uint8_t decimals);
    static void formatTime(char* buffer, uint32_t milliseconds);
    static void formatHex(char* buffer, uint32_t value, uint8_t digits);
    static bool parseFloat(const char* str, float& value);
    static bool parseInt(const char* str, int& value);
    static void trim(char* str);
    static int findSubstring(const char* haystack, const char* needle);
};
```

### Memory Management
```cpp
class MemoryManager {
public:
    static size_t getFreeHeap();
    static size_t getMinFreeHeap();
    static size_t getMaxAllocHeap();
    static void* allocateSafe(size_t size);
    static void deallocateSafe(void* ptr);
    static bool checkHeapIntegrity();
    static void logMemoryUsage();
};
```

## Logging System

### Log Configuration
```cpp
struct LogConfig {
    ErrorLevel consoleLevel;    // Minimum level for console output
    ErrorLevel storageLevel;    // Minimum level for storage
    bool timestampEnabled;      // Include timestamps
    bool moduleNameEnabled;     // Include module names
    uint16_t maxLogSize;        // Maximum log entries
    bool circularBuffer;        // Overwrite old entries
};
```

### Log Entry Structure
```cpp
struct LogEntry {
    uint32_t timestamp;         // Milliseconds since boot
    ErrorLevel level;           // Log level
    char module[16];           // Module name
    char message[128];         // Log message
    uint16_t lineNumber;       // Source line number
    char filename[32];         // Source filename
};
```

### Logging Implementation
```cpp
void Core::logError(ErrorLevel level, const char* message) {
    LogEntry entry;
    entry.timestamp = getMillis();
    entry.level = level;
    strncpy(entry.message, message, sizeof(entry.message) - 1);
    entry.message[sizeof(entry.message) - 1] = '\0';
    
    // Console output
    if (level >= logConfig.consoleLevel) {
        printLogEntry(entry);
    }
    
    // Storage
    if (level >= logConfig.storageLevel) {
        storeLogEntry(entry);
    }
    
    // Critical error handling
    if (level >= ERROR_CRITICAL) {
        handleCriticalError(entry);
    }
}
```

## Time Management

### Timer Implementation
```cpp
class Timer {
private:
    uint32_t startTime;
    uint32_t interval;
    bool running;
    
public:
    Timer(uint32_t intervalMs) : interval(intervalMs), running(false) {}
    
    void start() {
        startTime = millis();
        running = true;
    }
    
    void stop() {
        running = false;
    }
    
    bool isExpired() {
        if (!running) return false;
        return (millis() - startTime) >= interval;
    }
    
    uint32_t remaining() {
        if (!running) return 0;
        uint32_t elapsed = millis() - startTime;
        return (elapsed < interval) ? (interval - elapsed) : 0;
    }
    
    void reset() {
        startTime = millis();
    }
};
```

### Scheduler Implementation
```cpp
class TaskScheduler {
private:
    struct Task {
        void (*function)();
        uint32_t interval;
        uint32_t lastExecution;
        bool enabled;
    };
    
    Task tasks[MAX_TASKS];
    uint8_t taskCount;
    
public:
    bool addTask(void (*function)(), uint32_t intervalMs);
    bool removeTask(void (*function)());
    void enableTask(void (*function)(), bool enable);
    void update();
    uint8_t getTaskCount();
    float getCpuUsage();
};
```

## Configuration Management

### EEPROM/Flash Storage
```cpp
class ConfigManager {
public:
    bool loadConfiguration(SystemConfig& config);
    bool saveConfiguration(const SystemConfig& config);
    bool resetToDefaults();
    bool validateConfiguration(const SystemConfig& config);
    uint32_t calculateChecksum(const SystemConfig& config);
    
private:
    static const uint16_t CONFIG_VERSION = 1;
    static const uint16_t CONFIG_MAGIC = 0xCAFE;
    static const size_t CONFIG_SIZE = sizeof(SystemConfig);
};
```

### Configuration Validation
```cpp
bool ConfigManager::validateConfiguration(const SystemConfig& config) {
    // Version check
    if (config.hardwareVersion > MAX_HARDWARE_VERSION) {
        return false;
    }
    
    // Range checks
    if (config.canBaudRate < MIN_CAN_BAUD || config.canBaudRate > MAX_CAN_BAUD) {
        return false;
    }
    
    if (config.mainLoopFrequency < MIN_LOOP_FREQ || config.mainLoopFrequency > MAX_LOOP_FREQ) {
        return false;
    }
    
    // Safety parameter validation
    if (config.watchdogTimeout < MIN_WATCHDOG_TIMEOUT) {
        return false;
    }
    
    return true;
}
```

## Watchdog Implementation

### Hardware Watchdog
```cpp
class WatchdogManager {
private:
    uint32_t timeout;
    uint32_t lastFeed;
    bool enabled;
    
public:
    void enable(uint32_t timeoutMs);
    void disable();
    void feed();
    bool isEnabled();
    uint32_t getTimeRemaining();
    
    // Software watchdog for task monitoring
    void addMonitoredTask(const char* taskName, uint32_t maxExecutionTime);
    void taskStart(const char* taskName);
    void taskEnd(const char* taskName);
    bool checkTaskTimeouts();
};
```

## Initialization Sequence

### System Startup
```cpp
bool Core::begin() {
    // 1. Hardware initialization
    if (!initializeHardware()) {
        logError(ERROR_CRITICAL, "Hardware initialization failed");
        return false;
    }
    
    // 2. Load configuration
    if (!loadConfiguration()) {
        logWarning("Using default configuration");
        resetToDefaults();
    }
    
    // 3. Initialize logging
    initializeLogging();
    
    // 4. Enable watchdog
    if (systemConfig.safetySystemEnabled) {
        enableWatchdog(systemConfig.watchdogTimeout);
    }
    
    // 5. Initialize task scheduler
    scheduler.addTask(updateSafetySystems, 10);  // 100Hz
    scheduler.addTask(updateCommunication, 20);   // 50Hz
    scheduler.addTask(updateDisplay, 100);        // 10Hz
    
    systemState = STATE_READY;
    logInfo("System initialization complete");
    return true;
}
```

## Safety System Integration

### Emergency Shutdown
```cpp
void Core::emergencyShutdown(const char* reason) {
    // Log the emergency
    logError(ERROR_CRITICAL, reason);
    
    // Disable all outputs
    disableAllOutputs();
    
    // Send emergency CAN message
    sendEmergencyMessage();
    
    // Enter safe state
    systemState = STATE_EMERGENCY;
    
    // Flash LED pattern to indicate emergency
    while (true) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
        feedWatchdog();
    }
}
```

### Safety Monitoring
```cpp
SafetyStatus Core::checkSafetyStatus() {
    SafetyStatus status;
    
    status.imdStatus = digitalRead(IMD_STATUS_PIN);
    status.emergencyStop = digitalRead(ESTOP_PIN);
    status.thermalProtection = checkTemperatureLimits();
    status.currentProtection = checkCurrentLimits();
    status.voltageProtection = checkVoltageLimits();
    status.systemIntegrity = checkSystemIntegrity();
    status.lastCheck = getMillis();
    
    // Check for any safety violations
    if (!status.imdStatus || !status.emergencyStop || 
        !status.thermalProtection || !status.currentProtection ||
        !status.voltageProtection || !status.systemIntegrity) {
        
        systemState = STATE_FAULT;
        logError(ERROR_CRITICAL, "Safety violation detected");
    }
    
    return status;
}
```

## Performance Monitoring

### CPU Usage Tracking
```cpp
class PerformanceMonitor {
private:
    uint32_t idleTime;
    uint32_t busyTime;
    uint32_t lastMeasurement;
    
public:
    void startMeasurement();
    void recordIdleTime();
    void recordBusyTime();
    float getCpuUsage();
    uint32_t getLoopTime();
    uint32_t getMaxLoopTime();
    void resetStatistics();
};
```

## Testing and Validation

### Unit Tests
- Configuration load/save functionality
- Logging system verification
- Timer and scheduler accuracy
- Error handling robustness
- Memory management safety

### Integration Tests
- System initialization sequence
- Inter-module communication
- Safety system integration
- Performance under load
- Emergency shutdown procedures

## References
- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [Arduino Core for ESP32](https://github.com/espressif/arduino-esp32)
- [Formula Hybrid Safety Requirements](https://www.sae.org/programs/student-competitions/formula-hybrid/)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*