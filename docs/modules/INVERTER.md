# Motor Inverter Control Interface

## Purpose
This document defines the interface for motor inverter control in the ESP32 Formula Hybrid Core Library, providing precise motor control, monitoring, and safety functions for the electric drivetrain.

## Scope
- Inverter control and command interface
- Motor status monitoring and feedback
- Torque control and speed limiting
- Thermal management and protection
- Fault detection and safety responses

## Safety Considerations
⚠️ **CRITICAL**: Motor inverter controls high-voltage, high-current systems capable of producing dangerous torque levels. Any control failure could result in unintended acceleration, loss of braking, or electrical hazards.

## Module Interface

### Class Definition
```cpp
class InverterManager {
public:
    bool begin();
    void update();
    
    // Motor control
    void setTorqueRequest(float torque);
    void setSpeedLimit(uint16_t maxRPM);
    void setDirection(MotorDirection direction);
    void enableInverter(bool enable);
    
    // Status monitoring
    InverterStatus getStatus();
    float getActualTorque();
    uint16_t getActualSpeed();
    float getMotorTemperature();
    float getInverterTemperature();
    
    // Power and efficiency
    float getMotorPower();
    float getInverterEfficiency();
    float getDCBusVoltage();
    float getDCBusCurrent();
    
    // Fault handling
    InverterFault getFaultStatus();
    const char* getFaultDescription();
    void clearFaults();
    bool isFaultActive();
    
    // Configuration
    void setTorqueLimit(float maxTorque);
    void setCurrentLimit(float maxCurrent);
    void setTemperatureLimit(float maxTemp);
    void calibrateEncoder();
    
private:
    // Implementation details
};
```

### Configuration Structure
```cpp
struct InverterConfig {
    // Control parameters
    float maxTorque;            // Maximum torque (Nm)
    float maxCurrent;           // Maximum motor current (A)
    uint16_t maxSpeed;          // Maximum speed (RPM)
    float maxPower;             // Maximum power (kW)
    
    // Safety limits
    float motorTempLimit;       // Motor temperature limit (°C)
    float inverterTempLimit;    // Inverter temperature limit (°C)
    float dcBusVoltageMin;      // Minimum DC bus voltage (V)
    float dcBusVoltageMax;      // Maximum DC bus voltage (V)
    
    // Communication
    uint32_t canCommandID;      // CAN ID for commands
    uint32_t canStatusID;       // CAN ID for status
    uint16_t updateRate;        // Control update rate (Hz)
    uint16_t timeoutMs;         // Communication timeout
    
    // Hardware interface
    uint8_t enablePin;          // Inverter enable GPIO pin
    uint8_t faultPin;           // Fault input GPIO pin
    uint8_t readyPin;           // Ready status GPIO pin
};
```

## Motor Control Commands

### Torque Control
```cpp
enum MotorDirection {
    DIRECTION_FORWARD = 0,
    DIRECTION_REVERSE = 1,
    DIRECTION_NEUTRAL = 2
};

struct TorqueCommand {
    float requestedTorque;      // Torque request (Nm, positive = forward)
    uint16_t speedLimit;        // Speed limit (RPM)
    MotorDirection direction;   // Motor direction
    bool enableRequest;         // Inverter enable request
    uint8_t commandCounter;     // Rolling counter for message validation
};
```

### Control Modes
```cpp
enum ControlMode {
    MODE_TORQUE = 0,           // Direct torque control
    MODE_SPEED = 1,            // Speed regulation
    MODE_POWER = 2,            // Power limiting
    MODE_CURRENT = 3,          // Current control
    MODE_REGEN = 4             // Regenerative braking
};

struct ControlParameters {
    ControlMode mode;
    float primarySetpoint;     // Torque, speed, power, or current
    float secondaryLimit;      // Secondary constraint
    uint16_t rampRate;         // Rate of change limit
    bool enableField;          // Field weakening enable
};
```

## Status Monitoring

### Inverter Status
```cpp
enum InverterStatus {
    STATUS_DISABLED = 0,
    STATUS_ENABLING = 1,
    STATUS_READY = 2,
    STATUS_RUNNING = 3,
    STATUS_FAULT = 4,
    STATUS_WARNING = 5,
    STATUS_COOLDOWN = 6
};

struct InverterState {
    InverterStatus status;
    float actualTorque;         // Measured torque (Nm)
    uint16_t actualSpeed;       // Measured speed (RPM)
    float motorTemp;            // Motor temperature (°C)
    float inverterTemp;         // Inverter temperature (°C)
    float dcBusVoltage;         // DC bus voltage (V)
    float dcBusCurrent;         // DC bus current (A)
    float acCurrent;            // AC motor current (A RMS)
    uint16_t statusFlags;       // Status bit flags
    uint32_t timestamp;         // Status timestamp
};
```

### Fault Detection
```cpp
enum InverterFault {
    FAULT_NONE = 0x0000,
    FAULT_OVERCURRENT = 0x0001,
    FAULT_OVERVOLTAGE = 0x0002,
    FAULT_UNDERVOLTAGE = 0x0004,
    FAULT_OVERTEMP_MOTOR = 0x0008,
    FAULT_OVERTEMP_INVERTER = 0x0010,
    FAULT_ENCODER_FAULT = 0x0020,
    FAULT_COMMUNICATION = 0x0040,
    FAULT_EMERGENCY_STOP = 0x0080,
    FAULT_PRECHARGE_FAIL = 0x0100,
    FAULT_CONTACTOR_FAIL = 0x0200,
    FAULT_DESAT_FAULT = 0x0400,
    FAULT_SOFTWARE_FAULT = 0x0800
};
```

## Hardware Interface

### Digital I/O Connections
```
Signal         | ESP32 Pin | Direction | Function
---------------|-----------|-----------|---------------------------
Enable         | GPIO27    | Output    | Inverter enable control
Fault          | GPIO12    | Input     | Fault status (active low)
Ready          | GPIO14    | Input     | Ready status
Emergency Stop | GPIO15    | Input     | Emergency stop input
Precharge      | GPIO16    | Output    | Precharge control
Contactor+     | GPIO17    | Output    | Positive contactor control
Contactor-     | GPIO18    | Output    | Negative contactor control
```

### CAN Communication
- **Command messages**: 20ms interval (50Hz)
- **Status messages**: 10ms interval (100Hz)
- **Fault messages**: Immediate transmission
- **Baud rate**: 500kbps (Formula Hybrid standard)

### Analog Inputs
```
Parameter      | GPIO Pin | Range    | Resolution | Scaling
---------------|----------|----------|------------|------------------
DC Bus Voltage | GPIO25   | 0-600V   | 12-bit     | 147:1 divider
DC Bus Current | GPIO26   | ±200A    | 12-bit     | Hall sensor
Motor Temp     | GPIO32   | -40-150°C| 12-bit     | NTC thermistor
Inverter Temp  | GPIO33   | -40-150°C| 12-bit     | NTC thermistor
```

## Implementation Details

### Initialization Sequence
```cpp
bool InverterManager::begin() {
    // Configure GPIO pins
    pinMode(config.enablePin, OUTPUT);
    pinMode(config.faultPin, INPUT_PULLUP);
    pinMode(config.readyPin, INPUT_PULLUP);
    
    // Initialize control state
    digitalWrite(config.enablePin, LOW);
    currentState.status = STATUS_DISABLED;
    
    // Initialize CAN communication
    if (!setupCANCommunication()) {
        return false;
    }
    
    // Configure analog inputs
    setupAnalogInputs();
    
    // Perform self-test
    if (!performSelfTest()) {
        return false;
    }
    
    // Initialize control timers
    lastCommandTime = millis();
    lastStatusTime = millis();
    
    return true;
}
```

### Control Update Loop
```cpp
void InverterManager::update() {
    uint32_t currentTime = millis();
    
    // Read inverter status
    updateStatusFromCAN();
    updateAnalogInputs();
    updateDigitalInputs();
    
    // Check for faults
    checkFaultConditions();
    
    // Update control logic
    if (currentState.status == STATUS_READY || currentState.status == STATUS_RUNNING) {
        updateControlLoop();
    }
    
    // Send control commands
    if (currentTime - lastCommandTime >= COMMAND_INTERVAL) {
        sendControlCommand();
        lastCommandTime = currentTime;
    }
    
    // Send status updates
    if (currentTime - lastStatusTime >= STATUS_INTERVAL) {
        sendStatusMessage();
        lastStatusTime = currentTime;
    }
    
    // Watchdog monitoring
    checkCommunicationTimeout(currentTime);
}
```

### Torque Control Implementation
```cpp
void InverterManager::setTorqueRequest(float torque) {
    // Validate torque request
    torque = constrain(torque, -config.maxTorque, config.maxTorque);
    
    // Apply safety limits
    if (currentState.status != STATUS_READY && currentState.status != STATUS_RUNNING) {
        torque = 0.0;
    }
    
    // Check temperature derating
    torque = applyTemperatureDerating(torque);
    
    // Apply power limiting
    torque = applyPowerLimiting(torque);
    
    // Store command
    torqueCommand.requestedTorque = torque;
    torqueCommand.commandCounter++;
    
    // Log significant changes
    if (abs(torque - lastTorqueRequest) > TORQUE_LOG_THRESHOLD) {
        logTorqueChange(lastTorqueRequest, torque);
        lastTorqueRequest = torque;
    }
}
```

### Temperature Derating
```cpp
float InverterManager::applyTemperatureDerating(float requestedTorque) {
    float motorTemp = currentState.motorTemp;
    float inverterTemp = currentState.inverterTemp;
    
    // Motor temperature derating
    float motorDerate = 1.0;
    if (motorTemp > MOTOR_TEMP_DERATE_START) {
        motorDerate = 1.0 - ((motorTemp - MOTOR_TEMP_DERATE_START) / 
                            (config.motorTempLimit - MOTOR_TEMP_DERATE_START));
        motorDerate = constrain(motorDerate, 0.0, 1.0);
    }
    
    // Inverter temperature derating
    float inverterDerate = 1.0;
    if (inverterTemp > INVERTER_TEMP_DERATE_START) {
        inverterDerate = 1.0 - ((inverterTemp - INVERTER_TEMP_DERATE_START) / 
                               (config.inverterTempLimit - INVERTER_TEMP_DERATE_START));
        inverterDerate = constrain(inverterDerate, 0.0, 1.0);
    }
    
    // Apply the most restrictive derating
    float totalDerate = min(motorDerate, inverterDerate);
    return requestedTorque * totalDerate;
}
```

## CAN Message Definitions

### Inverter Command Message (0x110)
```cpp
struct InverterCommandMessage {
    int16_t torqueRequest;      // Torque request (0.1 Nm resolution)
    uint16_t speedLimit;        // Speed limit (1 RPM resolution)
    uint8_t direction;          // Direction and enable flags
    uint8_t controlMode;        // Control mode selection
    uint8_t commandCounter;     // Rolling counter
    uint8_t checksum;           // Message checksum
};

void InverterManager::sendControlCommand() {
    InverterCommandMessage cmd;
    cmd.torqueRequest = (int16_t)(torqueCommand.requestedTorque * 10.0);
    cmd.speedLimit = torqueCommand.speedLimit;
    cmd.direction = (torqueCommand.direction << 1) | (torqueCommand.enableRequest ? 1 : 0);
    cmd.controlMode = (uint8_t)currentControlMode;
    cmd.commandCounter = torqueCommand.commandCounter;
    cmd.checksum = calculateChecksum((uint8_t*)&cmd, sizeof(cmd) - 1);
    
    canManager.sendMessage(config.canCommandID, (uint8_t*)&cmd, sizeof(cmd));
}
```

### Inverter Status Message (0x111)
```cpp
struct InverterStatusMessage {
    int16_t actualTorque;       // Actual torque (0.1 Nm resolution)
    uint16_t actualSpeed;       // Actual speed (1 RPM resolution)
    int16_t motorTemp;          // Motor temperature (0.1°C resolution)
    int16_t inverterTemp;       // Inverter temperature (0.1°C resolution)
    uint16_t faultCode;         // Fault code bitfield
    uint8_t status;             // Inverter status
    uint8_t responseCounter;    // Response counter
};

void InverterManager::processStatusMessage(const uint8_t* data) {
    InverterStatusMessage* msg = (InverterStatusMessage*)data;
    
    // Update current state
    currentState.actualTorque = msg->actualTorque / 10.0;
    currentState.actualSpeed = msg->actualSpeed;
    currentState.motorTemp = msg->motorTemp / 10.0;
    currentState.inverterTemp = msg->inverterTemp / 10.0;
    currentState.status = (InverterStatus)msg->status;
    currentFault = (InverterFault)msg->faultCode;
    
    // Update timestamp
    currentState.timestamp = millis();
    lastMessageTime = currentState.timestamp;
    
    // Check for new faults
    if (msg->faultCode != 0 && msg->faultCode != lastFaultCode) {
        handleNewFault((InverterFault)msg->faultCode);
        lastFaultCode = msg->faultCode;
    }
}
```

## Safety Features

### Emergency Shutdown
```cpp
void InverterManager::emergencyShutdown(const char* reason) {
    // Immediately disable inverter
    digitalWrite(config.enablePin, LOW);
    
    // Set zero torque command
    torqueCommand.requestedTorque = 0.0;
    torqueCommand.enableRequest = false;
    
    // Send emergency CAN message
    sendEmergencyMessage(reason);
    
    // Update status
    currentState.status = STATUS_FAULT;
    
    // Log the shutdown
    logEmergencyShutdown(reason);
    
    // Set fault flag
    currentFault |= FAULT_EMERGENCY_STOP;
}
```

### Pre-charge Sequence
```cpp
bool InverterManager::performPrechargeSequence() {
    // Check initial conditions
    if (currentState.dcBusVoltage > MAX_RESIDUAL_VOLTAGE) {
        return false; // Bus not discharged
    }
    
    // Start precharge
    digitalWrite(PRECHARGE_PIN, HIGH);
    uint32_t startTime = millis();
    
    // Monitor precharge progress
    while (millis() - startTime < PRECHARGE_TIMEOUT) {
        updateAnalogInputs();
        
        // Check if precharge is complete
        if (currentState.dcBusVoltage > (batteryVoltage * PRECHARGE_THRESHOLD)) {
            // Close main contactors
            digitalWrite(CONTACTOR_POSITIVE_PIN, HIGH);
            delay(10);
            digitalWrite(CONTACTOR_NEGATIVE_PIN, HIGH);
            
            // Disable precharge
            digitalWrite(PRECHARGE_PIN, LOW);
            
            return true;
        }
        
        delay(10);
    }
    
    // Precharge timeout
    digitalWrite(PRECHARGE_PIN, LOW);
    currentFault |= FAULT_PRECHARGE_FAIL;
    return false;
}
```

### Fault Recovery
```cpp
void InverterManager::clearFaults() {
    // Only allow fault clearing if system is safe
    if (!isSystemSafeForReset()) {
        return;
    }
    
    // Clear internal fault flags
    currentFault = FAULT_NONE;
    
    // Send fault reset command to inverter
    sendFaultResetCommand();
    
    // Reset state if successful
    if (waitForFaultClearConfirmation()) {
        currentState.status = STATUS_DISABLED;
        lastFaultCode = 0;
    }
}
```

## Performance Monitoring

### Power and Efficiency Calculations
```cpp
void InverterManager::updatePerformanceMetrics() {
    // Calculate motor power
    float motorPower = (currentState.actualTorque * currentState.actualSpeed * 
                       2.0 * PI) / (60.0 * 1000.0); // kW
    
    // Calculate DC power
    float dcPower = currentState.dcBusVoltage * currentState.dcBusCurrent / 1000.0; // kW
    
    // Calculate efficiency
    if (dcPower > 0.1) { // Avoid division by zero
        inverterEfficiency = (motorPower / dcPower) * 100.0; // Percentage
    } else {
        inverterEfficiency = 0.0;
    }
    
    // Update running averages
    updateRunningAverages(motorPower, dcPower);
    
    // Check power limits
    checkPowerLimits(motorPower, dcPower);
}
```

### Thermal Management
```cpp
void InverterManager::updateThermalManagement() {
    float motorTemp = currentState.motorTemp;
    float inverterTemp = currentState.inverterTemp;
    
    // Check thermal limits
    if (motorTemp > config.motorTempLimit) {
        emergencyShutdown("MOTOR OVERTEMPERATURE");
        return;
    }
    
    if (inverterTemp > config.inverterTempLimit) {
        emergencyShutdown("INVERTER OVERTEMPERATURE");
        return;
    }
    
    // Thermal derating warnings
    if (motorTemp > MOTOR_TEMP_WARNING || inverterTemp > INVERTER_TEMP_WARNING) {
        sendThermalWarning();
    }
    
    // Update cooling control if available
    updateCoolingControl(motorTemp, inverterTemp);
}
```

## Testing and Validation

### Unit Tests
- Torque command validation and limiting
- Temperature derating calculations
- CAN message formatting and parsing
- Fault detection and response logic
- Safety shutdown procedures

### Hardware-in-the-Loop Tests
- Communication with actual inverter
- Torque control accuracy
- Fault injection testing
- Emergency shutdown timing
- Thermal protection verification

### Performance Tests
- Control loop timing and jitter
- Torque response characteristics
- Efficiency measurements
- Thermal behavior under load
- Long-term reliability testing

## Troubleshooting Guide

### Common Issues
1. **No communication**: Check CAN wiring, baud rate, message IDs
2. **Torque command ignored**: Verify enable signals, fault status
3. **Erratic behavior**: Check grounding, EMI shielding
4. **Thermal shutdown**: Verify cooling system, temperature sensors

### Diagnostic Procedures
1. **CAN bus analysis**: Monitor command and status messages
2. **Oscilloscope testing**: Verify enable and fault signals
3. **Temperature verification**: Cross-check with external sensors
4. **Power analysis**: Measure actual vs. reported values

## References
- [Formula Hybrid Electrical Regulations](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [Curtis 1238 Inverter Manual](https://www.curtisinstruments.com/products/motor-controllers/)
- [Rinehart Motion Systems PM100](https://rinehart-motion.com/products/pm100/)
- [CAN Bus Specification ISO 11898](https://www.iso.org/standard/33422.html)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*