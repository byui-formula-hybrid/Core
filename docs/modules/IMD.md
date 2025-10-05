# Insulation Monitoring Device Interface

## Purpose
This document defines the interface for the Insulation Monitoring Device (IMD) in the ESP32 Formula Hybrid Core Library, ensuring electrical safety through continuous insulation resistance monitoring.

## Scope
- IMD status monitoring and interpretation
- Insulation resistance measurement
- Fault detection and response
- PWM frequency analysis
- Safety shutdown procedures

## Safety Considerations
⚠️ **CRITICAL**: The IMD is a mandatory safety device in Formula Hybrid vehicles. Any IMD fault must result in immediate system shutdown. This is a life-safety system that prevents electrical shock hazards.

## Module Interface

### Class Definition
```cpp
class IMDManager {
public:
    bool begin();
    void update();
    
    // Status monitoring
    IMDStatus getStatus();
    bool isFaultActive();
    bool isSystemSafe();
    
    // Resistance measurement
    float getInsulationResistance();
    float getMinResistance();
    float getMaxResistance();
    
    // PWM analysis
    float getPWMFrequency();
    float getPWMDutyCycle();
    bool isPWMValid();
    
    // Fault handling
    IMDFault getFaultCode();
    const char* getFaultDescription();
    void clearFault();
    uint32_t getFaultTimestamp();
    
    // Configuration
    void setResistanceThreshold(float threshold);
    void setResponseTime(uint16_t timeMs);
    void enableDebugMode(bool enable);
    
private:
    // Implementation details
};
```

### IMD Configuration
```cpp
struct IMDConfig {
    float minResistance;        // Minimum acceptable resistance (kΩ)
    uint16_t responseTime;      // Maximum response time (ms)
    uint8_t statusPin;          // Digital status input pin
    uint8_t pwmPin;             // PWM frequency input pin
    float pwmFreqMin;          // Minimum valid PWM frequency (Hz)
    float pwmFreqMax;          // Maximum valid PWM frequency (Hz)
    bool enableWatchdog;       // Enable IMD watchdog monitoring
    uint16_t watchdogTimeout;  // Watchdog timeout (ms)
};
```

## Hardware Interface

### Digital Status Input
- **Pin**: GPIO12 (configurable)
- **Logic levels**: 
  - HIGH (3.3V): IMD OK / No fault
  - LOW (0V): IMD Fault detected
- **Input protection**: Pull-up resistor, optical isolation recommended
- **Response time**: <100ms for fault detection

### PWM Frequency Input
- **Pin**: GPIO13 (configurable)
- **Frequency range**: 10Hz - 50Hz (typical)
- **Duty cycle**: ~50% for normal operation
- **Voltage levels**: 0V - 3.3V (may require level shifting)
- **Frequency encoding**: Resistance value encoded in PWM frequency

### Optical Isolation
```
IMD Output    →    Optocoupler    →    ESP32 Input
    |                   |                 |
  Status              4N35              GPIO12
    |                   |                 |
PWM Freq              4N35              GPIO13
```

## IMD Status and Fault Codes

### Status Enumeration
```cpp
enum IMDStatus {
    IMD_STATUS_OK = 0,          // Normal operation
    IMD_STATUS_WARNING = 1,     // Low resistance warning
    IMD_STATUS_FAULT = 2,       // Insulation fault
    IMD_STATUS_ERROR = 3,       // IMD device error
    IMD_STATUS_UNKNOWN = 4      // Communication error
};

enum IMDFault {
    IMD_FAULT_NONE = 0x00,
    IMD_FAULT_LOW_RESISTANCE = 0x01,
    IMD_FAULT_DEVICE_ERROR = 0x02,
    IMD_FAULT_COMMUNICATION = 0x04,
    IMD_FAULT_PWM_INVALID = 0x08,
    IMD_FAULT_TIMEOUT = 0x10,
    IMD_FAULT_CALIBRATION = 0x20
};
```

### Resistance Thresholds
```cpp
// Formula Hybrid regulations typically require:
#define IMD_RESISTANCE_MINIMUM     100.0f  // kΩ minimum
#define IMD_RESISTANCE_WARNING     150.0f  // kΩ warning threshold
#define IMD_RESISTANCE_NORMAL      500.0f  // kΩ normal range
#define IMD_RESISTANCE_MAXIMUM     2000.0f // kΩ maximum measurable
```

## PWM Frequency Analysis

### Frequency-to-Resistance Mapping
Different IMD manufacturers use different encoding schemes:

#### ISOMETER IR155-3203/3204 (Bender)
```cpp
float IMDManager::calculateResistanceFromFrequency(float frequency) {
    // Bender IR155 encoding (typical)
    if (frequency >= 10.0 && frequency <= 50.0) {
        // Linear mapping: 10Hz = 0kΩ, 50Hz = 2000kΩ
        return (frequency - 10.0) * 50.0;  // kΩ
    }
    return -1.0; // Invalid frequency
}
```

#### Custom/Generic Encoding
```cpp
float IMDManager::calculateResistanceGeneric(float frequency) {
    // Example logarithmic encoding
    if (frequency >= 15.0 && frequency <= 45.0) {
        float normalized = (frequency - 15.0) / 30.0;
        return 100.0 * pow(10.0, normalized * 2.0); // 100Ω to 10MΩ
    }
    return -1.0;
}
```

### PWM Measurement Implementation
```cpp
class PWMFrequencyMeter {
private:
    volatile uint32_t lastRiseTime;
    volatile uint32_t periodSum;
    volatile uint16_t periodCount;
    volatile bool newDataAvailable;
    
public:
    void begin(uint8_t pin);
    float getFrequency();
    float getDutyCycle();
    bool isValid();
    
    // Interrupt service routine
    static void IRAM_ATTR handleInterrupt();
};

void IRAM_ATTR PWMFrequencyMeter::handleInterrupt() {
    uint32_t currentTime = micros();
    uint32_t period = currentTime - lastRiseTime;
    
    if (period > 10000 && period < 200000) { // 5Hz to 100Hz range
        periodSum += period;
        periodCount++;
        if (periodCount >= 10) {
            newDataAvailable = true;
        }
    }
    
    lastRiseTime = currentTime;
}
```

## Implementation Details

### Initialization
```cpp
bool IMDManager::begin() {
    // Configure status pin as input with pull-up
    pinMode(config.statusPin, INPUT_PULLUP);
    
    // Initialize PWM frequency measurement
    pwmMeter.begin(config.pwmPin);
    
    // Attach interrupt for status monitoring
    attachInterrupt(digitalPinToInterrupt(config.statusPin), 
                   statusChangeISR, CHANGE);
    
    // Initialize fault tracking
    clearFault();
    lastStatusCheck = millis();
    
    // Verify IMD is responding
    if (!performSelfTest()) {
        return false;
    }
    
    return true;
}
```

### Main Update Loop
```cpp
void IMDManager::update() {
    uint32_t currentTime = millis();
    
    // Read current status
    bool statusPin = digitalRead(config.statusPin);
    
    // Update PWM measurements
    updatePWMAnalysis();
    
    // Determine overall IMD status
    IMDStatus newStatus = determineStatus(statusPin);
    
    // Check for status changes
    if (newStatus != currentStatus) {
        handleStatusChange(currentStatus, newStatus);
        currentStatus = newStatus;
    }
    
    // Watchdog monitoring
    if (config.enableWatchdog) {
        checkWatchdog(currentTime);
    }
    
    // Periodic self-test
    if (currentTime - lastSelfTest > SELF_TEST_INTERVAL) {
        performSelfTest();
        lastSelfTest = currentTime;
    }
    
    lastStatusCheck = currentTime;
}
```

### Status Determination
```cpp
IMDStatus IMDManager::determineStatus(bool statusPin) {
    // Check digital status first
    if (!statusPin) {
        currentFault |= IMD_FAULT_LOW_RESISTANCE;
        return IMD_STATUS_FAULT;
    }
    
    // Analyze PWM frequency
    float frequency = pwmMeter.getFrequency();
    if (!pwmMeter.isValid() || frequency < config.pwmFreqMin || frequency > config.pwmFreqMax) {
        currentFault |= IMD_FAULT_PWM_INVALID;
        return IMD_STATUS_ERROR;
    }
    
    // Calculate resistance from frequency
    float resistance = calculateResistanceFromFrequency(frequency);
    currentResistance = resistance;
    
    // Check resistance thresholds
    if (resistance < IMD_RESISTANCE_MINIMUM) {
        currentFault |= IMD_FAULT_LOW_RESISTANCE;
        return IMD_STATUS_FAULT;
    } else if (resistance < IMD_RESISTANCE_WARNING) {
        return IMD_STATUS_WARNING;
    }
    
    // All checks passed
    currentFault = IMD_FAULT_NONE;
    return IMD_STATUS_OK;
}
```

### Fault Response
```cpp
void IMDManager::handleStatusChange(IMDStatus oldStatus, IMDStatus newStatus) {
    // Log status change
    logStatusChange(oldStatus, newStatus);
    
    // Handle fault conditions
    if (newStatus == IMD_STATUS_FAULT) {
        // CRITICAL: Immediate shutdown required
        emergencyShutdown("IMD FAULT DETECTED");
        
        // Send CAN message
        sendIMDFaultMessage();
        
        // Activate warning indicators
        activateWarningLEDs();
        
        // Record fault timestamp
        faultTimestamp = millis();
    }
    
    // Handle warning conditions
    else if (newStatus == IMD_STATUS_WARNING) {
        // Send warning CAN message
        sendIMDWarningMessage();
        
        // Activate warning indicators
        activateWarningLEDs();
    }
    
    // Handle recovery
    else if (oldStatus >= IMD_STATUS_WARNING && newStatus == IMD_STATUS_OK) {
        // Clear warnings
        clearWarningLEDs();
        
        // Send recovery message
        sendIMDRecoveryMessage();
    }
}
```

## CAN Message Integration

### IMD Status Message (ID: 0x130)
```cpp
struct IMDStatusMessage {
    uint8_t status;             // IMD status code
    uint16_t resistance;        // Insulation resistance (kΩ)
    uint8_t frequency;          // PWM frequency (Hz)
    uint8_t faultCode;          // Fault code bitfield
    uint16_t timestamp;         // Message timestamp
    uint8_t reserved;           // Reserved for future use
};

void IMDManager::sendStatusMessage() {
    IMDStatusMessage msg;
    msg.status = (uint8_t)currentStatus;
    msg.resistance = (uint16_t)(currentResistance);
    msg.frequency = (uint8_t)(pwmMeter.getFrequency());
    msg.faultCode = (uint8_t)currentFault;
    msg.timestamp = (uint16_t)(millis() & 0xFFFF);
    msg.reserved = 0;
    
    canManager.sendMessage(0x130, (uint8_t*)&msg, sizeof(msg));
}
```

### Emergency Fault Message (ID: 0x131)
```cpp
void IMDManager::sendIMDFaultMessage() {
    uint8_t emergencyData[8] = {
        0x01,                           // Emergency type: IMD fault
        (uint8_t)currentFault,          // Fault code
        (uint8_t)(currentResistance),   // Last known resistance
        0x00, 0x00, 0x00, 0x00, 0x00   // Reserved
    };
    
    canManager.sendMessage(0x131, emergencyData, 8);
}
```

## Self-Test and Diagnostics

### Power-On Self-Test
```cpp
bool IMDManager::performSelfTest() {
    // Check pin configuration
    if (!verifyPinConfiguration()) {
        return false;
    }
    
    // Verify PWM signal presence
    if (!verifyPWMSignal()) {
        currentFault |= IMD_FAULT_COMMUNICATION;
        return false;
    }
    
    // Check status pin responsiveness
    if (!verifyStatusPin()) {
        currentFault |= IMD_FAULT_DEVICE_ERROR;
        return false;
    }
    
    // Verify resistance calculation
    if (!verifyResistanceCalculation()) {
        currentFault |= IMD_FAULT_CALIBRATION;
        return false;
    }
    
    return true;
}
```

### Continuous Monitoring
```cpp
void IMDManager::checkWatchdog(uint32_t currentTime) {
    // Check for communication timeout
    if (currentTime - lastValidMeasurement > config.watchdogTimeout) {
        currentFault |= IMD_FAULT_TIMEOUT;
        
        // Force fault state
        currentStatus = IMD_STATUS_ERROR;
        
        // Trigger emergency response
        emergencyShutdown("IMD COMMUNICATION TIMEOUT");
    }
    
    // Check PWM signal stability
    if (!pwmMeter.isValid()) {
        invalidPWMCount++;
        if (invalidPWMCount > MAX_INVALID_PWM_COUNT) {
            currentFault |= IMD_FAULT_PWM_INVALID;
        }
    } else {
        invalidPWMCount = 0;
        lastValidMeasurement = currentTime;
    }
}
```

## Calibration and Configuration

### Resistance Calibration
```cpp
void IMDManager::calibrateResistance() {
    // Measure known resistance values
    float knownResistances[] = {100.0, 500.0, 1000.0, 2000.0}; // kΩ
    float measuredFrequencies[4];
    
    for (int i = 0; i < 4; i++) {
        // Prompt user to connect known resistance
        Serial.printf("Connect %0.1f kΩ resistor and press any key\n", knownResistances[i]);
        while (!Serial.available()) delay(100);
        Serial.read();
        
        // Measure frequency
        delay(1000); // Allow settling
        measuredFrequencies[i] = pwmMeter.getFrequency();
    }
    
    // Calculate calibration coefficients
    calculateCalibrationCoefficients(knownResistances, measuredFrequencies);
    
    // Save calibration to EEPROM
    saveCalibration();
}
```

### Configuration Management
```cpp
void IMDManager::loadConfiguration() {
    // Load from EEPROM or use defaults
    if (!EEPROM.read(IMD_CONFIG_ADDRESS, (uint8_t*)&config, sizeof(config))) {
        // Use default configuration
        config.minResistance = IMD_RESISTANCE_MINIMUM;
        config.responseTime = 100;
        config.statusPin = 12;
        config.pwmPin = 13;
        config.pwmFreqMin = 10.0;
        config.pwmFreqMax = 50.0;
        config.enableWatchdog = true;
        config.watchdogTimeout = 5000;
    }
}
```

## Testing and Validation

### Unit Tests
- PWM frequency measurement accuracy
- Resistance calculation algorithms
- Fault detection logic
- CAN message formatting
- Watchdog functionality

### Integration Tests
- Communication with actual IMD device
- Emergency shutdown procedures
- CAN bus integration
- LED indicator functionality
- System recovery testing

### Validation Tests
- Known resistance verification
- Response time measurement
- Environmental stress testing
- EMI immunity testing
- Long-term reliability testing

## Performance Requirements

### Response Times
- **Fault detection**: <100ms from IMD signal change
- **Emergency shutdown**: <50ms from fault detection
- **CAN message transmission**: <10ms
- **PWM frequency update**: <500ms for stable reading

### Accuracy Requirements
- **Resistance measurement**: ±5% of reading above 100kΩ
- **Frequency measurement**: ±0.1Hz resolution
- **Fault detection**: 100% reliability for safety-critical faults

## Troubleshooting Guide

### Common Issues
1. **No PWM signal**: Check wiring, IMD power, signal levels
2. **Erratic readings**: Check grounding, EMI shielding
3. **False faults**: Verify thresholds, calibration
4. **Communication timeout**: Check signal integrity, timing

### Debug Procedures
1. **Oscilloscope analysis**: Verify PWM signal quality
2. **Multimeter verification**: Check resistance calculations
3. **CAN bus monitoring**: Verify message transmission
4. **Logic analyzer**: Check timing and digital signals

## References
- [Formula Hybrid Electrical Regulations](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [Bender IR155 IMD Datasheet](https://www.bender.de/en/products/insulation-monitoring/ir155-3203-3204/)
- [ISO 6469-3: Electric Vehicle Safety](https://www.iso.org/standard/68665.html)
- [ESP32 Interrupt Handling](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/intr_alloc.html)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*