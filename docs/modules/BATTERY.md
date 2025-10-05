# Battery Management System Interface

## Purpose
This document defines the interface and implementation details for the Battery Management System (BMS) module in the ESP32 Formula Hybrid Core Library.

## Scope
- Battery voltage and current monitoring
- Cell balancing interface
- Temperature monitoring
- Safety shutdown procedures
- State of charge (SOC) estimation

## Safety Considerations
⚠️ **CRITICAL**: Battery systems in Formula Hybrid vehicles operate at dangerous voltages (up to 600V). All BMS interfaces must include proper isolation and safety interlocks.

## Module Interface

### Class Definition
```cpp
class BatteryManager {
public:
    bool begin();
    void update();
    
    // Voltage monitoring
    float getPackVoltage();
    float getCellVoltage(uint8_t cellIndex);
    float getMinCellVoltage();
    float getMaxCellVoltage();
    
    // Current monitoring
    float getPackCurrent();
    float getPower();
    
    // Temperature monitoring
    float getMaxTemperature();
    float getMinTemperature();
    float getTemperature(uint8_t sensorIndex);
    
    // State estimation
    float getStateOfCharge();
    uint32_t getRemainingCapacity();
    uint32_t getTimeToEmpty();
    
    // Safety status
    bool isSystemSafe();
    BatteryFault getFaultStatus();
    void emergencyShutdown();
    
private:
    // Implementation details
};
```

### Configuration Parameters
```cpp
struct BatteryConfig {
    uint8_t numCells;           // Number of battery cells
    uint8_t numTempSensors;     // Number of temperature sensors
    float nominalCapacity;      // Battery capacity in Ah
    float maxVoltage;           // Maximum pack voltage
    float minVoltage;           // Minimum pack voltage
    float maxCurrent;           // Maximum discharge current
    float maxTemperature;       // Maximum operating temperature
    uint16_t updateInterval;    // Update frequency in ms
};
```

## Hardware Interface

### Analog Inputs
- **Pack voltage**: GPIO25 (via voltage divider)
- **Pack current**: GPIO26 (via current sensor)
- **Temperature sensors**: I2C or analog inputs

### Digital I/O
- **Contactor control**: Digital outputs for main contactors
- **Fault signals**: Digital inputs from BMS controller
- **Enable/disable**: Safety interlock signals

### Communication
- **CAN messages**: Battery status broadcasts
- **I2C devices**: Temperature sensors, cell monitors
- **Serial communication**: Debug and configuration interface

## Measurement Specifications

### Voltage Measurement
- **Resolution**: 12-bit ADC (0.8mV resolution with proper scaling)
- **Accuracy**: ±0.5% of reading
- **Range**: 0-600V (via voltage divider)
- **Update rate**: 100Hz minimum

### Current Measurement
- **Resolution**: 12-bit ADC
- **Accuracy**: ±1% of reading
- **Range**: ±200A (bidirectional)
- **Response time**: <10ms

### Temperature Measurement
- **Sensors**: NTC thermistors or digital sensors
- **Resolution**: 0.1°C
- **Accuracy**: ±2°C
- **Range**: -40°C to +85°C

## Safety Features

### Overvoltage Protection
- **Cell level**: Individual cell monitoring
- **Pack level**: Total pack voltage limits
- **Response**: Immediate charging system shutdown

### Undervoltage Protection
- **Cutoff voltage**: Prevents deep discharge damage
- **Warning levels**: Graduated warnings to driver
- **Response**: Reduced power or system shutdown

### Overcurrent Protection
- **Discharge limit**: Maximum motor current
- **Charge limit**: Maximum regen current
- **Response**: Current limiting or system shutdown

### Thermal Protection
- **Temperature limits**: Cell and ambient monitoring
- **Thermal runaway detection**: Rapid temperature rise
- **Response**: Cooling system activation or shutdown

## State Estimation Algorithms

### State of Charge (SOC)
```cpp
// Coulomb counting method
float calculateSOC() {
    static float remainingCapacity = nominalCapacity;
    float current = getPackCurrent();
    float deltaTime = getTimeDelta();
    
    // Update remaining capacity
    remainingCapacity -= (current * deltaTime / 3600.0);
    
    // Apply voltage-based correction
    float voltageSOC = voltageToSOC(getPackVoltage());
    remainingCapacity = applyVoltageCorrection(remainingCapacity, voltageSOC);
    
    return (remainingCapacity / nominalCapacity) * 100.0;
}
```

### Power Estimation
- **Available power**: Based on current SOC and temperature
- **Regeneration limits**: Maximum charging power
- **Derating factors**: Temperature and age compensation

## CAN Message Definitions

### Battery Status Message (0x100)
```
Byte 0-1: Pack voltage (0.1V resolution)
Byte 2-3: Pack current (0.1A resolution)
Byte 4:   State of charge (1% resolution)
Byte 5:   Maximum temperature (1°C offset by 40)
Byte 6:   Battery status flags
Byte 7:   Fault codes
```

### Battery Details Message (0x101)
```
Byte 0-1: Min cell voltage (1mV resolution)
Byte 2-3: Max cell voltage (1mV resolution)
Byte 4-5: Remaining capacity (0.1Ah resolution)
Byte 6-7: Time to empty (1 minute resolution)
```

## Fault Detection and Handling

### Fault Types
```cpp
enum BatteryFault {
    FAULT_NONE = 0x00,
    FAULT_OVERVOLTAGE = 0x01,
    FAULT_UNDERVOLTAGE = 0x02,
    FAULT_OVERCURRENT = 0x04,
    FAULT_OVERTEMPERATURE = 0x08,
    FAULT_COMMUNICATION = 0x10,
    FAULT_IMBALANCE = 0x20,
    FAULT_SYSTEM = 0x40
};
```

### Fault Response Procedures
1. **Immediate shutdown faults**: Overvoltage, overcurrent
2. **Warning faults**: Temperature rise, cell imbalance
3. **Degraded operation**: Reduced power limits
4. **Fault logging**: Record all fault events with timestamps

## Calibration Procedures

### Voltage Calibration
- **Reference voltage**: Use precision voltage source
- **Scaling factors**: Calculate voltage divider ratios
- **Offset correction**: Eliminate systematic errors

### Current Calibration
- **Zero current calibration**: Establish baseline
- **Load testing**: Verify accuracy at various currents
- **Temperature compensation**: Adjust for sensor drift

### SOC Calibration
- **Full charge calibration**: Establish 100% SOC point
- **Capacity measurement**: Determine actual capacity
- **Aging compensation**: Account for capacity fade

## Testing Procedures

### Unit Tests
- Voltage and current reading accuracy
- SOC calculation algorithms
- Fault detection logic
- CAN message formatting

### Integration Tests
- Communication with external BMS
- Safety interlock verification
- Emergency shutdown procedures
- Data logging and retrieval

### Validation Tests
- Bench testing with actual battery pack
- Load testing under various conditions
- Thermal cycling tests
- Long-term reliability testing

## Performance Requirements

### Response Times
- **Fault detection**: <100ms
- **Emergency shutdown**: <50ms
- **CAN message transmission**: <10ms
- **Display updates**: <500ms

### Accuracy Requirements
- **Voltage measurement**: ±0.5%
- **Current measurement**: ±1.0%
- **SOC estimation**: ±5% (after calibration)
- **Temperature measurement**: ±2°C

## References
- [Formula Hybrid Battery Regulations](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [Battery Management System Standards](https://www.iso.org/standard/71420.html)
- [Lithium Battery Safety Guidelines](https://batteryuniversity.com/article/bu-304a-safety-concerns-with-li-ion)
- [ESP32 ADC Calibration](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*