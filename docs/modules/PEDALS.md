# Pedal Position and Safety Systems

## Purpose
This document defines the interface for pedal position monitoring and safety systems in the ESP32 Formula Hybrid Core Library, ensuring safe and accurate driver input processing for throttle and brake controls.

## Scope
- Accelerator pedal position sensing (dual redundant)
- Brake pedal position and pressure monitoring  
- Pedal safety interlocks and validation
- APPS (Accelerator Pedal Position Sensor) plausibility checks
- BSE (Brake System Encoder) monitoring
- Emergency response procedures

## Safety Considerations
⚠️ **CRITICAL**: Pedal systems are primary safety controls. Any failure could result in unintended acceleration or loss of braking capability. Formula Hybrid regulations mandate specific safety requirements including dual redundant throttle sensors and brake/throttle interlocks.

## Module Interface

### Class Definition
```cpp
class PedalManager {
public:
    bool begin();
    void update();
    
    // Throttle control
    float getThrottlePosition();        // 0.0-1.0 (0-100%)
    float getThrottleVoltage1();        // Primary sensor voltage
    float getThrottleVoltage2();        // Secondary sensor voltage
    bool isThrottlePlausible();         // APPS plausibility check
    
    // Brake control
    float getBrakePosition();           // 0.0-1.0 (0-100%)
    float getBrakePressure();           // Brake pressure (bar/psi)
    bool isBrakePressed();              // Brake activation threshold
    
    // Safety status
    PedalSafetyStatus getSafetyStatus();
    bool isEmergencyStopActive();
    bool isBrakePedalPressed();
    uint32_t getLastErrorTime();
    
    // Calibration
    void calibrateThrottle();
    void calibrateBrake();
    bool saveCalibration();
    bool loadCalibration();
    
    // Configuration
    void setThrottleDeadband(float deadband);
    void setBrakeThreshold(float threshold);
    void enableDualThrottle(bool enable);
    
private:
    // Implementation details
};
```

### Configuration Structure
```cpp
struct PedalConfig {
    // Throttle configuration
    uint8_t throttle1Pin;           // Primary throttle sensor pin
    uint8_t throttle2Pin;           // Secondary throttle sensor pin
    float throttle1MinVoltage;      // Min voltage for sensor 1
    float throttle1MaxVoltage;      // Max voltage for sensor 1
    float throttle2MinVoltage;      // Min voltage for sensor 2  
    float throttle2MaxVoltage;      // Max voltage for sensor 2
    float throttleDeadband;         // Dead band at pedal start
    float throttlePlausibilityTolerance; // APPS tolerance (%)
    
    // Brake configuration
    uint8_t brakePositionPin;       // Brake position sensor pin
    uint8_t brakePressurePin;       // Brake pressure sensor pin
    float brakeMinVoltage;          // Min brake sensor voltage
    float brakeMaxVoltage;          // Max brake sensor voltage
    float brakeThreshold;           // Brake activation threshold
    float brakePressureScale;       // Pressure sensor scaling
    
    // Safety parameters
    bool enableDualThrottle;        // Enable dual throttle checking
    uint16_t plausibilityTimeout;   // APPS fault timeout (ms)
    uint16_t sensorTimeout;         // Sensor signal timeout (ms)
    bool enableBrakeInterlock;      // Enable brake/throttle interlock
    float brakeInterlockThreshold;  // Brake interlock activation level
};
```

## Safety Requirements (Formula Hybrid)

### APPS (Accelerator Pedal Position Sensor) Requirements
Per Formula Hybrid regulations:
- **Dual redundant sensors** required
- **10% plausibility check** - sensors must agree within 10%
- **Immediate shutdown** if sensors disagree for >100ms
- **5-second lockout** after APPS fault before reset allowed

### BSE (Brake System Encoder) Requirements  
- **Hard wired brake light** activation required
- **Brake/throttle interlock** - throttle disabled when brake >25%
- **Mechanical brake backup** independent of electronic systems

### Safety Interlock Logic
```cpp
enum PedalSafetyState {
    PEDAL_SAFE = 0,
    PEDAL_APPS_IMPLAUSIBLE = 1,
    PEDAL_BRAKE_INTERLOCK = 2,
    PEDAL_SENSOR_FAULT = 3,
    PEDAL_EMERGENCY_STOP = 4
};

struct PedalSafetyStatus {
    PedalSafetyState state;
    bool appsPlausible;
    bool brakeInterlockActive;
    bool emergencyStopActive;
    uint32_t faultTimestamp;
    uint32_t lastValidTime;
};
```

## Hardware Interface

### Analog Sensor Connections
```
Sensor               | ESP32 Pin | Voltage Range | Resolution
---------------------|-----------|---------------|------------
Throttle Sensor 1    | GPIO36    | 0.5-4.5V     | 12-bit ADC
Throttle Sensor 2    | GPIO39    | 0.5-4.5V     | 12-bit ADC  
Brake Position       | GPIO34    | 0.5-4.5V     | 12-bit ADC
Brake Pressure       | GPIO35    | 0-5V         | 12-bit ADC
```

### Digital Outputs
```
Signal               | ESP32 Pin | Function
---------------------|-----------|------------------------
Brake Light          | GPIO2     | Brake light activation
APPS Fault LED       | GPIO4     | APPS fault indicator
Emergency Stop       | GPIO5     | Emergency stop output
Throttle Enable      | GPIO16    | Throttle system enable
```

### Sensor Specifications

#### Throttle Position Sensors
- **Type**: Potentiometric or Hall effect
- **Dual redundant**: Two independent sensors required
- **Voltage range**: 0.5V to 4.5V (avoid rail voltages)
- **Linearity**: ±1% over full range
- **Response time**: <10ms

#### Brake Sensors
- **Position sensor**: Potentiometric or magnetic
- **Pressure sensor**: 0-5V output, 0-100 bar range
- **Brake light switch**: Mechanical backup required
- **Response time**: <5ms for safety-critical functions

## Implementation Details

### Initialization and Calibration
```cpp
bool PedalManager::begin() {
    // Configure analog pins
    analogReadResolution(12); // 12-bit resolution
    analogSetAttenuation(ADC_11db); // 0-3.3V range
    
    // Configure digital outputs
    pinMode(BRAKE_LIGHT_PIN, OUTPUT);
    pinMode(APPS_FAULT_LED_PIN, OUTPUT);
    pinMode(EMERGENCY_STOP_PIN, OUTPUT);
    pinMode(THROTTLE_ENABLE_PIN, OUTPUT);
    
    // Load calibration data
    if (!loadCalibration()) {
        // Use default calibration if none saved
        setDefaultCalibration();
    }
    
    // Initialize safety state
    safetyStatus.state = PEDAL_SAFE;
    safetyStatus.lastValidTime = millis();
    
    // Perform self-test
    if (!performSelfTest()) {
        return false;
    }
    
    return true;
}
```

### Main Update Loop
```cpp
void PedalManager::update() {
    uint32_t currentTime = millis();
    
    // Read all sensor values
    readThrottleSensors();
    readBrakeSensors();
    
    // Perform APPS plausibility check
    bool appsOK = checkAPPSPlausibility();
    
    // Check brake interlock
    bool brakeInterlockOK = checkBrakeInterlock();
    
    // Update safety status
    updateSafetyStatus(appsOK, brakeInterlockOK, currentTime);
    
    // Process safety responses
    processSafetyResponses();
    
    // Update outputs
    updateBrakeLight();
    updateThrottleEnable();
    updateStatusLEDs();
    
    // Send CAN messages
    if (currentTime - lastCANUpdate >= CAN_UPDATE_INTERVAL) {
        sendPedalStatusMessage();
        lastCANUpdate = currentTime;
    }
}
```

### Throttle Sensor Reading
```cpp
void PedalManager::readThrottleSensors() {
    // Read raw ADC values
    uint16_t adc1 = analogRead(config.throttle1Pin);
    uint16_t adc2 = analogRead(config.throttle2Pin);
    
    // Convert to voltages
    throttleVoltage1 = (adc1 / 4095.0) * 3.3;
    throttleVoltage2 = (adc2 / 4095.0) * 3.3;
    
    // Apply calibration and scaling
    throttlePosition1 = mapFloat(throttleVoltage1, 
                                config.throttle1MinVoltage,
                                config.throttle1MaxVoltage,
                                0.0, 1.0);
    
    throttlePosition2 = mapFloat(throttleVoltage2,
                                config.throttle2MinVoltage, 
                                config.throttle2MaxVoltage,
                                0.0, 1.0);
    
    // Constrain to valid range
    throttlePosition1 = constrain(throttlePosition1, 0.0, 1.0);
    throttlePosition2 = constrain(throttlePosition2, 0.0, 1.0);
    
    // Apply deadband
    if (throttlePosition1 < config.throttleDeadband) {
        throttlePosition1 = 0.0;
    }
    if (throttlePosition2 < config.throttleDeadband) {
        throttlePosition2 = 0.0;
    }
}
```

### APPS Plausibility Check
```cpp
bool PedalManager::checkAPPSPlausibility() {
    if (!config.enableDualThrottle) {
        return true; // Skip check if dual throttle disabled
    }
    
    // Calculate difference between sensors
    float difference = abs(throttlePosition1 - throttlePosition2);
    float tolerance = config.throttlePlausibilityTolerance;
    
    // Check if difference exceeds tolerance
    bool plausible = (difference <= tolerance);
    
    // Handle implausible condition
    if (!plausible) {
        if (appsImplausibleStartTime == 0) {
            // First detection of implausible condition
            appsImplausibleStartTime = millis();
        } else if (millis() - appsImplausibleStartTime >= config.plausibilityTimeout) {
            // Timeout exceeded - trigger APPS fault
            triggerAPPSFault();
            return false;
        }
    } else {
        // Reset implausible timer
        appsImplausibleStartTime = 0;
        
        // Check if we're in APPS fault state
        if (safetyStatus.state == PEDAL_APPS_IMPLAUSIBLE) {
            // Check if 5-second lockout period has elapsed
            if (millis() - safetyStatus.faultTimestamp >= APPS_LOCKOUT_TIME) {
                clearAPPSFault();
            }
        }
    }
    
    return plausible;
}
```

### Brake Interlock Logic
```cpp
bool PedalManager::checkBrakeInterlock() {
    // Read brake position
    float brakePos = getBrakePosition();
    
    // Check if brake is pressed above interlock threshold
    bool brakeActive = (brakePos > config.brakeInterlockThreshold);
    
    // Get current throttle position (average of both sensors if dual)
    float throttlePos = getThrottlePosition();
    
    // Brake interlock rule: If brake >25% and throttle >5%, fault
    if (brakeActive && throttlePos > 0.05) {
        if (brakeInterlockStartTime == 0) {
            brakeInterlockStartTime = millis();
        } else if (millis() - brakeInterlockStartTime >= BRAKE_INTERLOCK_TIMEOUT) {
            triggerBrakeInterlock();
            return false;
        }
    } else {
        brakeInterlockStartTime = 0;
        
        // Clear brake interlock if conditions are safe
        if (safetyStatus.state == PEDAL_BRAKE_INTERLOCK && 
            (!brakeActive || throttlePos <= 0.05)) {
            clearBrakeInterlock();
        }
    }
    
    return true;
}
```

### Safety Response Implementation
```cpp
void PedalManager::triggerAPPSFault() {
    safetyStatus.state = PEDAL_APPS_IMPLAUSIBLE;
    safetyStatus.faultTimestamp = millis();
    safetyStatus.appsPlausible = false;
    
    // Disable throttle output
    digitalWrite(THROTTLE_ENABLE_PIN, LOW);
    
    // Activate fault LED
    digitalWrite(APPS_FAULT_LED_PIN, HIGH);
    
    // Send emergency CAN message
    sendAPPSFaultMessage();
    
    // Log the fault
    logPedalFault("APPS IMPLAUSIBLE", throttlePosition1, throttlePosition2);
    
    // Trigger emergency shutdown if configured
    if (config.enableEmergencyShutdown) {
        emergencyShutdown("APPS FAULT");
    }
}

void PedalManager::clearAPPSFault() {
    safetyStatus.state = PEDAL_SAFE;
    safetyStatus.appsPlausible = true;
    
    // Re-enable throttle
    digitalWrite(THROTTLE_ENABLE_PIN, HIGH);
    
    // Clear fault LED
    digitalWrite(APPS_FAULT_LED_PIN, LOW);
    
    // Send recovery message
    sendAPPSRecoveryMessage();
    
    // Log recovery
    logPedalRecovery("APPS FAULT CLEARED");
}
```

## CAN Message Integration

### Pedal Status Message (ID: 0x120)
```cpp
struct PedalStatusMessage {
    uint16_t throttle1;         // Throttle sensor 1 (0.1% resolution)
    uint16_t throttle2;         // Throttle sensor 2 (0.1% resolution)
    uint16_t brake;             // Brake position (0.1% resolution)
    uint8_t throttleFlags;      // Throttle status flags
    uint8_t brakeFlags;         // Brake status flags
    uint8_t safetyState;        // Safety state enumeration
    uint8_t reserved;           // Reserved for future use
};

void PedalManager::sendPedalStatusMessage() {
    PedalStatusMessage msg;
    msg.throttle1 = (uint16_t)(throttlePosition1 * 1000);
    msg.throttle2 = (uint16_t)(throttlePosition2 * 1000);
    msg.brake = (uint16_t)(getBrakePosition() * 1000);
    msg.throttleFlags = buildThrottleFlags();
    msg.brakeFlags = buildBrakeFlags();
    msg.safetyState = (uint8_t)safetyStatus.state;
    msg.reserved = 0;
    
    canManager.sendMessage(0x120, (uint8_t*)&msg, sizeof(msg));
}
```

### Emergency Pedal Message (ID: 0x121)
```cpp
void PedalManager::sendAPPSFaultMessage() {
    uint8_t emergencyData[8] = {
        0x02,                                           // Emergency type: APPS fault
        (uint8_t)(throttlePosition1 * 100),            // Sensor 1 position (%)
        (uint8_t)(throttlePosition2 * 100),            // Sensor 2 position (%)
        (uint8_t)(abs(throttlePosition1 - throttlePosition2) * 100), // Difference (%)
        (uint8_t)(millis() >> 24),                      // Timestamp bytes
        (uint8_t)(millis() >> 16),
        (uint8_t)(millis() >> 8),
        (uint8_t)(millis())
    };
    
    canManager.sendMessage(0x121, emergencyData, 8);
}
```

## Calibration Procedures

### Throttle Calibration
```cpp
void PedalManager::calibrateThrottle() {
    Serial.println("Throttle Calibration Procedure");
    Serial.println("================================");
    
    // Calibrate minimum position (pedal released)
    Serial.println("Release throttle pedal completely. Press any key when ready.");
    waitForKeyPress();
    
    float minVoltage1 = 0, minVoltage2 = 0;
    for (int i = 0; i < 100; i++) {
        minVoltage1 += (analogRead(config.throttle1Pin) / 4095.0) * 3.3;
        minVoltage2 += (analogRead(config.throttle2Pin) / 4095.0) * 3.3;
        delay(10);
    }
    config.throttle1MinVoltage = minVoltage1 / 100.0;
    config.throttle2MinVoltage = minVoltage2 / 100.0;
    
    // Calibrate maximum position (pedal fully pressed)
    Serial.println("Press throttle pedal fully. Press any key when ready.");
    waitForKeyPress();
    
    float maxVoltage1 = 0, maxVoltage2 = 0;
    for (int i = 0; i < 100; i++) {
        maxVoltage1 += (analogRead(config.throttle1Pin) / 4095.0) * 3.3;
        maxVoltage2 += (analogRead(config.throttle2Pin) / 4095.0) * 3.3;
        delay(10);
    }
    config.throttle1MaxVoltage = maxVoltage1 / 100.0;
    config.throttle2MaxVoltage = maxVoltage2 / 100.0;
    
    // Validate calibration
    if (validateThrottleCalibration()) {
        saveCalibration();
        Serial.println("Throttle calibration completed successfully!");
    } else {
        Serial.println("Throttle calibration failed. Using default values.");
    }
}
```

### Brake Calibration
```cpp
void PedalManager::calibrateBrake() {
    Serial.println("Brake Calibration Procedure");
    Serial.println("===========================");
    
    // Calibrate minimum position (pedal released)
    Serial.println("Release brake pedal completely. Press any key when ready.");
    waitForKeyPress();
    
    float minVoltage = 0;
    for (int i = 0; i < 100; i++) {
        minVoltage += (analogRead(config.brakePositionPin) / 4095.0) * 3.3;
        delay(10);
    }
    config.brakeMinVoltage = minVoltage / 100.0;
    
    // Calibrate maximum position (pedal fully pressed)
    Serial.println("Press brake pedal fully. Press any key when ready.");
    waitForKeyPress();
    
    float maxVoltage = 0;
    for (int i = 0; i < 100; i++) {
        maxVoltage += (analogRead(config.brakePositionPin) / 4095.0) * 3.3;
        delay(10);
    }
    config.brakeMaxVoltage = maxVoltage / 100.0;
    
    // Calibrate brake threshold (25% for interlock)
    config.brakeThreshold = 0.25; // 25% as per regulations
    
    saveCalibration();
    Serial.println("Brake calibration completed!");
}
```

## Testing and Validation

### Self-Test Procedures
```cpp
bool PedalManager::performSelfTest() {
    bool testPassed = true;
    
    // Test sensor voltage ranges
    if (!testSensorVoltages()) {
        Serial.println("FAIL: Sensor voltage test");
        testPassed = false;
    }
    
    // Test APPS plausibility logic
    if (!testAPPSLogic()) {
        Serial.println("FAIL: APPS plausibility test");
        testPassed = false;
    }
    
    // Test brake interlock
    if (!testBrakeInterlock()) {
        Serial.println("FAIL: Brake interlock test");
        testPassed = false;
    }
    
    // Test digital outputs
    if (!testDigitalOutputs()) {
        Serial.println("FAIL: Digital output test");
        testPassed = false;
    }
    
    return testPassed;
}
```

### Unit Tests
- Sensor reading accuracy and linearity
- APPS plausibility checking logic
- Brake interlock timing and thresholds
- Calibration data validation
- CAN message formatting

### Integration Tests
- Full pedal system operation
- Safety response timing
- Emergency shutdown procedures
- Inter-module communication
- Fault recovery testing

## Performance Requirements

### Response Times
- **Sensor reading**: <5ms update rate
- **APPS fault detection**: <100ms from threshold violation
- **Brake interlock**: <50ms response time
- **Emergency shutdown**: <25ms from fault detection

### Accuracy Requirements
- **Throttle position**: ±1% over full range
- **Brake position**: ±2% over full range
- **APPS agreement**: ±10% maximum difference
- **Brake threshold**: ±5% repeatability

## Troubleshooting Guide

### Common Issues
1. **APPS implausible**: Check sensor wiring, calibration, mechanical alignment
2. **Brake interlock active**: Verify brake sensor, check for simultaneous pedal press
3. **Erratic readings**: Check grounding, EMI shielding, connector integrity
4. **Calibration drift**: Re-calibrate sensors, check for wear

### Diagnostic Procedures
1. **Voltage measurement**: Verify sensor output voltages
2. **Mechanical inspection**: Check pedal assembly and linkages
3. **Electrical continuity**: Test all connections and grounds
4. **Signal integrity**: Use oscilloscope to check for noise

## References
- [Formula Hybrid Electrical Regulations](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [APPS Requirements EV.5.6](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [BSE Requirements EV.5.7](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [ESP32 ADC Characteristics](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*