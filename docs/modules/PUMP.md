# Pump Control and Monitoring

## Purpose
This document defines the interface for pump control and monitoring in the ESP32 Formula Hybrid Core Library, managing cooling systems, fuel systems, and other fluid management components.

## Scope
- Cooling pump control and monitoring
- Fuel pump management (if applicable)
- Flow rate and pressure monitoring
- Temperature-based pump control
- Fault detection and protection
- Energy-efficient pump operation

## Safety Considerations
⚠️ **CRITICAL**: Pump systems are essential for cooling and lubrication. Pump failure could lead to overheating, component damage, or fire hazards. All pump systems must have fail-safe modes and emergency shutdown capabilities.

## Module Interface

### Class Definition
```cpp
class PumpManager {
public:
    bool begin();
    void update();
    
    // Pump control
    void setPumpSpeed(PumpType pump, float speed);  // 0.0-1.0
    void enablePump(PumpType pump, bool enable);
    void setPumpMode(PumpType pump, PumpMode mode);
    
    // Status monitoring  
    float getPumpSpeed(PumpType pump);
    float getPumpCurrent(PumpType pump);
    bool isPumpRunning(PumpType pump);
    PumpStatus getPumpStatus(PumpType pump);
    
    // Flow and pressure monitoring
    float getFlowRate(FlowSensor sensor);
    float getPressure(PressureSensor sensor);
    float getTemperature(TempSensor sensor);
    
    // Fault handling
    PumpFault getFaultStatus(PumpType pump);
    void clearFaults(PumpType pump);
    bool isFaultActive();
    
    // Automatic control
    void enableAutomaticControl(bool enable);
    void setTemperatureSetpoint(float temperature);
    void setPressureSetpoint(float pressure);
    
private:
    // Implementation details
};
```

### Configuration Structure
```cpp
struct PumpConfig {
    // Pump hardware configuration
    uint8_t pwmPin;             // PWM control pin
    uint8_t enablePin;          // Pump enable pin
    uint8_t feedbackPin;        // Speed/current feedback pin
    uint8_t faultPin;           // Fault input pin
    
    // Control parameters
    float maxSpeed;             // Maximum pump speed (0-1.0)
    float minSpeed;             // Minimum pump speed (0-1.0)
    uint16_t pwmFrequency;      // PWM frequency (Hz)
    uint8_t pwmResolution;      // PWM resolution (bits)
    
    // Safety limits
    float maxCurrent;           // Maximum pump current (A)
    float maxTemperature;       // Maximum fluid temperature (°C)
    float maxPressure;          // Maximum system pressure (bar)
    uint16_t stallTimeout;      // Stall detection timeout (ms)
    
    // Automatic control
    float tempSetpoint;         // Temperature setpoint (°C)
    float tempHysteresis;       // Temperature control hysteresis
    float pressureSetpoint;     // Pressure setpoint (bar)
    float pressureHysteresis;   // Pressure control hysteresis
};
```

## Pump Types and Applications

### Cooling System Pumps
```cpp
enum PumpType {
    PUMP_COOLING_PRIMARY = 0,   // Main cooling loop
    PUMP_COOLING_SECONDARY = 1, // Secondary/radiator loop
    PUMP_BATTERY_COOLING = 2,   // Battery thermal management
    PUMP_MOTOR_COOLING = 3,     // Motor cooling loop
    PUMP_FUEL = 4,              // Fuel system (if applicable)
    PUMP_HYDRAULIC = 5,         // Hydraulic systems
    PUMP_COUNT = 6
};

enum PumpMode {
    MODE_MANUAL = 0,            // Manual speed control
    MODE_TEMPERATURE = 1,       // Temperature-based control
    MODE_PRESSURE = 2,          // Pressure-based control
    MODE_FLOW = 3,              // Flow rate control
    MODE_EMERGENCY = 4          // Emergency/fail-safe mode
};
```

### Pump Status and Faults
```cpp
enum PumpStatus {
    PUMP_STOPPED = 0,
    PUMP_STARTING = 1,
    PUMP_RUNNING = 2,
    PUMP_STOPPING = 3,
    PUMP_FAULT = 4,
    PUMP_STALLED = 5
};

enum PumpFault {
    FAULT_NONE = 0x00,
    FAULT_OVERCURRENT = 0x01,
    FAULT_STALL = 0x02,
    FAULT_OVERTEMPERATURE = 0x04,
    FAULT_OVERPRESSURE = 0x08,
    FAULT_COMMUNICATION = 0x10,
    FAULT_SENSOR = 0x20,
    FAULT_POWER = 0x40
};
```

## Hardware Interface

### PWM Control
```
Pump Type         | ESP32 Pin | PWM Channel | Frequency | Resolution
------------------|-----------|-------------|-----------|------------
Cooling Primary   | GPIO13    | Channel 0   | 20kHz     | 12-bit
Cooling Secondary | GPIO14    | Channel 1   | 20kHz     | 12-bit
Battery Cooling   | GPIO25    | Channel 2   | 20kHz     | 12-bit
Motor Cooling     | GPIO26    | Channel 3   | 20kHz     | 12-bit
```

### Digital Control
```
Signal            | ESP32 Pin | Direction | Function
------------------|-----------|-----------|------------------------
Pump Enable 1     | GPIO16    | Output    | Primary pump enable
Pump Enable 2     | GPIO17    | Output    | Secondary pump enable
Fault Input 1     | GPIO18    | Input     | Primary pump fault
Fault Input 2     | GPIO19    | Input     | Secondary pump fault
Emergency Stop    | GPIO21    | Input     | Emergency shutdown
```

### Analog Monitoring
```
Parameter         | ESP32 Pin | Range     | Resolution | Sensor Type
------------------|-----------|-----------|------------|------------------
Flow Rate 1       | GPIO32    | 0-50 L/min| 12-bit     | Hall effect
Flow Rate 2       | GPIO33    | 0-50 L/min| 12-bit     | Hall effect
Pressure 1        | GPIO34    | 0-10 bar  | 12-bit     | Pressure transducer
Pressure 2        | GPIO35    | 0-10 bar  | 12-bit     | Pressure transducer
Pump Current 1    | GPIO36    | 0-20A     | 12-bit     | Current sensor
Pump Current 2    | GPIO39    | 0-20A     | 12-bit     | Current sensor
```

## Implementation Details

### Initialization
```cpp
bool PumpManager::begin() {
    // Configure PWM channels
    for (int i = 0; i < PUMP_COUNT; i++) {
        ledcSetup(i, pumpConfigs[i].pwmFrequency, pumpConfigs[i].pwmResolution);
        ledcAttachPin(pumpConfigs[i].pwmPin, i);
        ledcWrite(i, 0); // Start with pumps off
    }
    
    // Configure digital I/O
    for (int i = 0; i < PUMP_COUNT; i++) {
        pinMode(pumpConfigs[i].enablePin, OUTPUT);
        pinMode(pumpConfigs[i].faultPin, INPUT_PULLUP);
        digitalWrite(pumpConfigs[i].enablePin, LOW);
    }
    
    // Initialize analog inputs
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
    
    // Load configuration
    loadPumpConfiguration();
    
    // Initialize control loops
    initializeControlLoops();
    
    // Perform self-test
    if (!performSelfTest()) {
        return false;
    }
    
    return true;
}
```

### Main Update Loop
```cpp
void PumpManager::update() {
    uint32_t currentTime = millis();
    
    // Read all sensor values
    updateSensorReadings();
    
    // Update pump status
    for (int i = 0; i < PUMP_COUNT; i++) {
        updatePumpStatus((PumpType)i);
    }
    
    // Check for faults
    checkFaultConditions();
    
    // Run automatic control loops
    if (automaticControlEnabled) {
        runAutomaticControl();
    }
    
    // Update pump outputs
    updatePumpOutputs();
    
    // Send CAN status messages
    if (currentTime - lastCANUpdate >= CAN_UPDATE_INTERVAL) {
        sendPumpStatusMessages();
        lastCANUpdate = currentTime;
    }
    
    // Log performance data
    if (currentTime - lastLogUpdate >= LOG_UPDATE_INTERVAL) {
        logPumpPerformance();
        lastLogUpdate = currentTime;
    }
}
```

### PWM Speed Control
```cpp
void PumpManager::setPumpSpeed(PumpType pump, float speed) {
    // Validate input
    speed = constrain(speed, 0.0, 1.0);
    
    // Apply minimum speed threshold
    if (speed > 0.0 && speed < pumpConfigs[pump].minSpeed) {
        speed = pumpConfigs[pump].minSpeed;
    }
    
    // Apply maximum speed limit
    if (speed > pumpConfigs[pump].maxSpeed) {
        speed = pumpConfigs[pump].maxSpeed;
    }
    
    // Check if pump is enabled
    if (!pumpStates[pump].enabled) {
        speed = 0.0;
    }
    
    // Calculate PWM duty cycle
    uint16_t pwmValue = (uint16_t)(speed * ((1 << pumpConfigs[pump].pwmResolution) - 1));
    
    // Set PWM output
    ledcWrite(pump, pwmValue);
    
    // Update pump state
    pumpStates[pump].commandedSpeed = speed;
    pumpStates[pump].lastSpeedChange = millis();
    
    // Log significant speed changes
    if (abs(speed - pumpStates[pump].lastLoggedSpeed) > SPEED_LOG_THRESHOLD) {
        logSpeedChange(pump, pumpStates[pump].lastLoggedSpeed, speed);
        pumpStates[pump].lastLoggedSpeed = speed;
    }
}
```

### Temperature-Based Control
```cpp
void PumpManager::runTemperatureControl() {
    for (int i = 0; i < PUMP_COUNT; i++) {
        PumpType pump = (PumpType)i;
        
        if (pumpStates[pump].mode != MODE_TEMPERATURE) {
            continue;
        }
        
        // Get relevant temperature reading
        float currentTemp = getRelevantTemperature(pump);
        float setpoint = pumpConfigs[pump].tempSetpoint;
        float hysteresis = pumpConfigs[pump].tempHysteresis;
        
        // PID-like control with hysteresis
        float error = currentTemp - setpoint;
        float speed = 0.0;
        
        if (error > hysteresis) {
            // Temperature too high - increase pump speed
            speed = mapFloat(error, hysteresis, hysteresis + 20.0, 
                           pumpConfigs[pump].minSpeed, pumpConfigs[pump].maxSpeed);
        } else if (error > 0) {
            // Near setpoint - proportional control
            speed = mapFloat(error, 0.0, hysteresis,
                           pumpConfigs[pump].minSpeed * 0.5, pumpConfigs[pump].minSpeed);
        } else {
            // Below setpoint - minimum speed or off
            speed = (currentTemp > setpoint - hysteresis) ? pumpConfigs[pump].minSpeed * 0.3 : 0.0;
        }
        
        // Apply rate limiting
        speed = applyRateLimit(pump, speed);
        
        // Set pump speed
        setPumpSpeed(pump, speed);
    }
}
```

### Fault Detection
```cpp
void PumpManager::checkFaultConditions() {
    for (int i = 0; i < PUMP_COUNT; i++) {
        PumpType pump = (PumpType)i;
        uint32_t currentTime = millis();
        
        // Check overcurrent
        if (pumpStates[pump].current > pumpConfigs[pump].maxCurrent) {
            if (pumpStates[pump].overcurrentStartTime == 0) {
                pumpStates[pump].overcurrentStartTime = currentTime;
            } else if (currentTime - pumpStates[pump].overcurrentStartTime > OVERCURRENT_TIMEOUT) {
                triggerPumpFault(pump, FAULT_OVERCURRENT);
            }
        } else {
            pumpStates[pump].overcurrentStartTime = 0;
        }
        
        // Check stall condition
        if (pumpStates[pump].commandedSpeed > 0.1 && pumpStates[pump].measuredSpeed < 0.05) {
            if (pumpStates[pump].stallStartTime == 0) {
                pumpStates[pump].stallStartTime = currentTime;
            } else if (currentTime - pumpStates[pump].stallStartTime > pumpConfigs[pump].stallTimeout) {
                triggerPumpFault(pump, FAULT_STALL);
            }
        } else {
            pumpStates[pump].stallStartTime = 0;
        }
        
        // Check hardware fault pin
        if (!digitalRead(pumpConfigs[pump].faultPin)) {
            triggerPumpFault(pump, FAULT_POWER);
        }
        
        // Check sensor validity
        if (!isSensorDataValid(pump)) {
            triggerPumpFault(pump, FAULT_SENSOR);
        }
    }
}
```

### Emergency Response
```cpp
void PumpManager::triggerPumpFault(PumpType pump, PumpFault fault) {
    // Set fault status
    pumpStates[pump].faultStatus |= fault;
    pumpStates[pump].status = PUMP_FAULT;
    pumpStates[pump].faultTimestamp = millis();
    
    // Emergency response based on fault type
    switch (fault) {
        case FAULT_OVERCURRENT:
        case FAULT_POWER:
            // Immediate shutdown
            setPumpSpeed(pump, 0.0);
            enablePump(pump, false);
            break;
            
        case FAULT_STALL:
            // Try restart sequence
            attemptPumpRestart(pump);
            break;
            
        case FAULT_OVERTEMPERATURE:
            // Reduce speed but keep running
            setPumpSpeed(pump, pumpConfigs[pump].minSpeed);
            break;
            
        case FAULT_SENSOR:
            // Switch to manual mode
            pumpStates[pump].mode = MODE_MANUAL;
            break;
    }
    
    // Send fault CAN message
    sendPumpFaultMessage(pump, fault);
    
    // Log the fault
    logPumpFault(pump, fault);
    
    // Check if critical system pump
    if (isCriticalPump(pump)) {
        // Trigger emergency shutdown
        emergencyShutdown("CRITICAL PUMP FAULT");
    }
}
```

## Sensor Integration

### Flow Rate Measurement
```cpp
float PumpManager::measureFlowRate(FlowSensor sensor) {
    // Read frequency from flow sensor
    float frequency = flowSensors[sensor].getFrequency();
    
    // Convert frequency to flow rate (sensor-specific calibration)
    float flowRate = frequency * flowSensors[sensor].calibrationFactor;
    
    // Apply temperature compensation if available
    if (flowSensors[sensor].tempCompensationEnabled) {
        float temperature = getTemperature(flowSensors[sensor].tempSensor);
        flowRate = applyTemperatureCompensation(flowRate, temperature);
    }
    
    // Apply filtering
    flowRate = lowPassFilter(flowRate, flowSensors[sensor].lastValue, 0.1);
    flowSensors[sensor].lastValue = flowRate;
    
    return flowRate;
}
```

### Pressure Monitoring
```cpp
float PumpManager::measurePressure(PressureSensor sensor) {
    // Read ADC value
    uint16_t adcValue = analogRead(pressureSensors[sensor].pin);
    
    // Convert to voltage
    float voltage = (adcValue / 4095.0) * 3.3;
    
    // Apply sensor calibration
    float pressure = mapFloat(voltage, 
                             pressureSensors[sensor].minVoltage,
                             pressureSensors[sensor].maxVoltage,
                             pressureSensors[sensor].minPressure,
                             pressureSensors[sensor].maxPressure);
    
    // Apply offset and scale corrections
    pressure = (pressure + pressureSensors[sensor].offset) * pressureSensors[sensor].scale;
    
    // Validate range
    if (pressure < 0 || pressure > pressureSensors[sensor].maxPressure * 1.1) {
        // Invalid reading
        pressureSensors[sensor].faultCount++;
        return pressureSensors[sensor].lastValidValue;
    }
    
    pressureSensors[sensor].lastValidValue = pressure;
    pressureSensors[sensor].faultCount = 0;
    
    return pressure;
}
```

## CAN Message Integration

### Pump Status Message (ID: 0x140)
```cpp
struct PumpStatusMessage {
    uint8_t pumpSpeeds[4];      // Pump speeds (0-255 = 0-100%)
    uint8_t pumpStatus[2];      // Status flags for all pumps
    uint16_t totalCurrent;      // Total pump current (0.1A resolution)
    uint8_t faultFlags;         // Fault status bitfield
    uint8_t reserved;           // Reserved for future use
};

void PumpManager::sendPumpStatusMessage() {
    PumpStatusMessage msg;
    
    // Pack pump speeds
    for (int i = 0; i < 4; i++) {
        msg.pumpSpeeds[i] = (uint8_t)(pumpStates[i].commandedSpeed * 255);
    }
    
    // Pack status flags
    msg.pumpStatus[0] = 0;
    msg.pumpStatus[1] = 0;
    for (int i = 0; i < PUMP_COUNT; i++) {
        uint8_t statusBits = (uint8_t)pumpStates[i].status;
        if (i < 4) {
            msg.pumpStatus[0] |= (statusBits << (i * 2));
        } else {
            msg.pumpStatus[1] |= (statusBits << ((i - 4) * 2));
        }
    }
    
    // Calculate total current
    float totalCurrent = 0;
    for (int i = 0; i < PUMP_COUNT; i++) {
        totalCurrent += pumpStates[i].current;
    }
    msg.totalCurrent = (uint16_t)(totalCurrent * 10);
    
    // Pack fault flags
    msg.faultFlags = 0;
    for (int i = 0; i < PUMP_COUNT; i++) {
        if (pumpStates[i].faultStatus != FAULT_NONE) {
            msg.faultFlags |= (1 << i);
        }
    }
    
    msg.reserved = 0;
    
    canManager.sendMessage(0x140, (uint8_t*)&msg, sizeof(msg));
}
```

### Flow and Pressure Message (ID: 0x141)
```cpp
struct FlowPressureMessage {
    uint16_t flowRate1;         // Flow rate 1 (0.1 L/min resolution)
    uint16_t flowRate2;         // Flow rate 2 (0.1 L/min resolution)
    uint16_t pressure1;         // Pressure 1 (0.01 bar resolution)
    uint16_t pressure2;         // Pressure 2 (0.01 bar resolution)
};

void PumpManager::sendFlowPressureMessage() {
    FlowPressureMessage msg;
    msg.flowRate1 = (uint16_t)(getFlowRate(FLOW_SENSOR_1) * 10);
    msg.flowRate2 = (uint16_t)(getFlowRate(FLOW_SENSOR_2) * 10);
    msg.pressure1 = (uint16_t)(getPressure(PRESSURE_SENSOR_1) * 100);
    msg.pressure2 = (uint16_t)(getPressure(PRESSURE_SENSOR_2) * 100);
    
    canManager.sendMessage(0x141, (uint8_t*)&msg, sizeof(msg));
}
```

## Performance Optimization

### Energy Efficiency
```cpp
void PumpManager::optimizeEnergyEfficiency() {
    // Variable speed control based on cooling requirements
    for (int i = 0; i < PUMP_COUNT; i++) {
        PumpType pump = (PumpType)i;
        
        if (pumpStates[pump].mode == MODE_TEMPERATURE) {
            // Use minimum effective speed
            float requiredFlow = calculateRequiredFlow(pump);
            float optimalSpeed = calculateOptimalSpeed(pump, requiredFlow);
            
            // Apply efficiency curve
            optimalSpeed = applyEfficiencyCurve(pump, optimalSpeed);
            
            setPumpSpeed(pump, optimalSpeed);
        }
    }
}
```

### Predictive Control
```cpp
void PumpManager::runPredictiveControl() {
    // Predict temperature rise based on current conditions
    float predictedTemp = predictTemperature(TEMP_SENSOR_PRIMARY, 30); // 30 seconds ahead
    
    // Pre-emptively adjust pump speed
    if (predictedTemp > pumpConfigs[PUMP_COOLING_PRIMARY].tempSetpoint) {
        float preemptiveSpeed = calculatePreemptiveSpeed(predictedTemp);
        setPumpSpeed(PUMP_COOLING_PRIMARY, preemptiveSpeed);
    }
}
```

## Testing and Validation

### Pump Performance Tests
- Flow rate vs. speed characterization
- Pressure-flow curves
- Power consumption analysis
- Thermal response testing
- Fault response validation

### System Integration Tests
- Multi-pump coordination
- Thermal system validation
- Emergency shutdown testing
- CAN communication verification
- Long-term reliability testing

### Environmental Tests
- Temperature cycling
- Vibration resistance
- Fluid compatibility
- EMI immunity
- Pressure cycling

## Performance Requirements

### Response Times
- **Pump speed change**: <500ms to reach 90% of target
- **Fault detection**: <100ms for overcurrent, <1s for stall
- **Emergency shutdown**: <50ms
- **Temperature control**: <5s settling time

### Efficiency Targets
- **Overall system efficiency**: >85%
- **Pump control efficiency**: >95%
- **Standby power**: <5% of rated power

## Troubleshooting Guide

### Common Issues
1. **Pump not starting**: Check power, enable signals, fault conditions
2. **Erratic speed control**: Verify PWM signals, check for EMI
3. **Overheating**: Check coolant levels, flow restrictions, pump performance
4. **High current draw**: Inspect for mechanical binding, worn components

### Diagnostic Procedures
1. **Electrical testing**: Verify voltages, currents, PWM signals
2. **Mechanical inspection**: Check for leaks, blockages, wear
3. **Performance testing**: Measure flow rates, pressures, temperatures
4. **System analysis**: Review control loops, sensor calibration

## References
- [Grundfos Pump Handbook](https://www.grundfos.com/service-support/encyclopedia-search/pump-handbook.html)
- [Formula Hybrid Cooling Requirements](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [ESP32 PWM Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html)
- [Automotive Cooling System Design](https://www.sae.org/standards/content/j2723_201202/)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*