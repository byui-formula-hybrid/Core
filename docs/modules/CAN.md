# CAN Communication Implementation

## Purpose
This document defines the implementation details for the CAN communication module in the ESP32 Formula Hybrid Core Library, including message definitions, protocols, and hardware interface.

## Scope
- CAN bus hardware interface implementation
- Message parsing and transmission
- Protocol definitions and timing
- Error handling and diagnostics
- Integration with vehicle systems

## Safety Considerations
⚠️ **CRITICAL**: CAN bus is the primary communication backbone for safety-critical systems. Any communication failure could result in loss of vehicle control or safety system functionality.

## Module Interface

### Class Definition
```cpp
class CANManager {
public:
    bool begin(uint32_t baudRate = 500000);
    void update();
    
    // Message transmission
    bool sendMessage(uint32_t id, uint8_t* data, uint8_t length);
    bool sendBatteryStatus();
    bool sendInverterCommand();
    bool sendDashboardData();
    
    // Message reception
    bool receiveMessage(CANMessage& message);
    bool hasNewMessage();
    uint16_t getMessageCount();
    
    // System status
    bool isBusActive();
    CANError getLastError();
    uint32_t getErrorCount();
    void clearErrors();
    
    // Configuration
    void setMessageFilter(uint32_t id, uint32_t mask);
    void enableLoopback(bool enable);
    void setRetryCount(uint8_t retries);
    
private:
    // Implementation details
};
```

### Message Structure
```cpp
struct CANMessage {
    uint32_t id;            // CAN identifier
    uint8_t data[8];        // Data payload (0-8 bytes)
    uint8_t length;         // Data length
    uint32_t timestamp;     // Receive timestamp
    bool extended;          // Extended ID format
    bool remote;            // Remote transmission request
};
```

## Hardware Implementation

### MCP2515 CAN Controller
- **Interface**: SPI communication with ESP32
- **Clock frequency**: 16MHz crystal oscillator
- **Baud rate**: 500kbps (Formula Hybrid standard)
- **Buffer size**: 3 transmit, 2 receive buffers

### Pin Connections
```
ESP32 Pin | MCP2515 Pin | Function
----------|-------------|----------
GPIO18    | SCK         | SPI Clock
GPIO19    | SO          | SPI MISO
GPIO23    | SI          | SPI MOSI
GPIO5     | CS          | Chip Select
GPIO4     | INT         | Interrupt
VCC       | VDD         | 3.3V Power
GND       | VSS         | Ground
```

### CAN Transceiver (TJA1050)
- **High-speed CAN**: ISO 11898 compliant
- **Bus termination**: 120Ω resistors at each end
- **Common mode range**: -12V to +12V
- **Differential voltage**: 1.5V to 3V

## Message Definitions

### Battery Management (0x100-0x10F)
```cpp
// Battery Status (ID: 0x100)
struct BatteryStatusMsg {
    uint16_t voltage;       // Pack voltage (0.1V resolution)
    int16_t current;        // Pack current (0.1A resolution)
    uint8_t soc;           // State of charge (1% resolution)
    uint8_t temperature;    // Max temperature (1°C, +40 offset)
    uint8_t status;        // Status flags
    uint8_t faults;        // Fault codes
};

// Battery Details (ID: 0x101)
struct BatteryDetailsMsg {
    uint16_t minCellV;     // Min cell voltage (1mV resolution)
    uint16_t maxCellV;     // Max cell voltage (1mV resolution)
    uint16_t capacity;     // Remaining capacity (0.1Ah resolution)
    uint16_t timeToEmpty;  // Time to empty (1 min resolution)
};
```

### Motor Control (0x110-0x11F)
```cpp
// Inverter Command (ID: 0x110)
struct InverterCommandMsg {
    int16_t torqueRequest;  // Torque request (0.1Nm resolution)
    uint16_t speedLimit;    // Speed limit (1 RPM resolution)
    uint8_t direction;      // 0=Forward, 1=Reverse
    uint8_t enable;         // Inverter enable flag
    uint16_t reserved;      // Reserved for future use
};

// Inverter Status (ID: 0x111)
struct InverterStatusMsg {
    int16_t actualTorque;   // Actual torque (0.1Nm resolution)
    uint16_t actualSpeed;   // Actual speed (1 RPM resolution)
    int16_t motorTemp;      // Motor temperature (0.1°C resolution)
    uint8_t faults;         // Inverter fault codes
    uint8_t warnings;       // Warning flags
};
```

### Vehicle Systems (0x120-0x12F)
```cpp
// Pedal Positions (ID: 0x120)
struct PedalPositionsMsg {
    uint16_t throttle1;     // Throttle sensor 1 (0.1% resolution)
    uint16_t throttle2;     // Throttle sensor 2 (0.1% resolution)
    uint16_t brake;         // Brake pressure (0.1% resolution)
    uint8_t throttleFlags;  // Throttle status flags
    uint8_t brakeFlags;     // Brake status flags
};

// Dashboard Data (ID: 0x121)
struct DashboardDataMsg {
    uint16_t speed;         // Vehicle speed (0.1 km/h resolution)
    uint8_t gear;          // Current gear
    uint8_t mode;          // Drive mode (Eco, Normal, Sport)
    uint16_t efficiency;    // Current efficiency (0.1 km/kWh)
    uint16_t range;        // Remaining range (0.1 km resolution)
};
```

### Safety Systems (0x130-0x13F)
```cpp
// IMD Status (ID: 0x130)
struct IMDStatusMsg {
    uint8_t status;         // IMD status (0=OK, 1=Fault)
    uint16_t resistance;    // Insulation resistance (kΩ)
    uint8_t frequency;      // PWM frequency (Hz)
    uint32_t reserved;      // Reserved for future use
};

// Emergency Status (ID: 0x131)
struct EmergencyStatusMsg {
    uint8_t emergencyStop;  // Emergency stop status
    uint8_t imdFault;      // IMD fault flag
    uint8_t overtemp;      // Overtemperature flag
    uint8_t overcurrent;   // Overcurrent flag
    uint32_t timestamp;    // Fault timestamp
};
```

## Communication Protocol

### Message Priorities
```
Priority | ID Range  | Description
---------|-----------|------------------
0 (High) | 0x000-0x0FF | Emergency/Safety
1        | 0x100-0x1FF | Battery/Power
2        | 0x200-0x2FF | Motor Control
3        | 0x300-0x3FF | Vehicle Systems
4 (Low)  | 0x400-0x7FF | Diagnostics/Debug
```

### Transmission Schedule
```
Message Type        | Period | Priority
--------------------|--------|----------
Emergency Status    | 10ms   | Highest
Battery Status      | 50ms   | High
Inverter Command    | 20ms   | High
Pedal Positions     | 20ms   | High
Dashboard Data      | 100ms  | Medium
Diagnostic Data     | 1000ms | Low
```

### Error Handling
```cpp
enum CANError {
    CAN_OK = 0,
    CAN_FAIL = 1,
    CAN_ALLTXBUSY = 2,
    CAN_SENDMSGTIMEOUT = 3,
    CAN_GETMSGTIMEOUT = 4,
    CAN_BUS_ERROR = 5,
    CAN_PASSIVE_ERROR = 6,
    CAN_BUS_OFF = 7
};
```

## Implementation Details

### Initialization Sequence
```cpp
bool CANManager::begin(uint32_t baudRate) {
    // Initialize SPI interface
    SPI.begin();
    
    // Reset MCP2515
    mcp2515.reset();
    
    // Configure bit timing for specified baud rate
    if (mcp2515.setBitrate(baudRate) != MCP2515::ERROR_OK) {
        return false;
    }
    
    // Set normal operating mode
    mcp2515.setNormalMode();
    
    // Configure message filters
    setupMessageFilters();
    
    return true;
}
```

### Message Transmission
```cpp
bool CANManager::sendMessage(uint32_t id, uint8_t* data, uint8_t length) {
    can_frame frame;
    frame.can_id = id;
    frame.can_dlc = length;
    memcpy(frame.data, data, length);
    
    // Attempt transmission with retry logic
    for (uint8_t retry = 0; retry < maxRetries; retry++) {
        if (mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK) {
            return true;
        }
        delay(1); // Brief delay before retry
    }
    
    // Log transmission failure
    errorCount++;
    lastError = CAN_SENDMSGTIMEOUT;
    return false;
}
```

### Message Reception
```cpp
void CANManager::update() {
    can_frame frame;
    
    // Check for received messages
    while (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
        CANMessage message;
        message.id = frame.can_id;
        message.length = frame.can_dlc;
        message.timestamp = millis();
        memcpy(message.data, frame.data, frame.can_dlc);
        
        // Process message based on ID
        processReceivedMessage(message);
    }
    
    // Check for bus errors
    checkBusStatus();
}
```

## Diagnostic Features

### Bus Monitoring
- **Message count tracking**: Transmitted and received
- **Error rate monitoring**: Bus-off, passive error conditions
- **Timing analysis**: Message latency and jitter
- **Bus utilization**: Percentage of bus bandwidth used

### Debug Interface
```cpp
void CANManager::printDiagnostics() {
    Serial.println("=== CAN Bus Diagnostics ===");
    Serial.printf("Messages sent: %d\n", txCount);
    Serial.printf("Messages received: %d\n", rxCount);
    Serial.printf("Error count: %d\n", errorCount);
    Serial.printf("Bus utilization: %.1f%%\n", getBusUtilization());
    Serial.printf("Last error: %s\n", errorToString(lastError));
}
```

### Message Logging
- **SD card logging**: Store all CAN traffic for analysis
- **Selective filtering**: Log only specific message types
- **Timestamp precision**: Microsecond-level timing
- **Circular buffer**: In-memory storage for recent messages

## Performance Optimization

### Buffer Management
- **Transmit buffering**: Queue messages during high traffic
- **Receive filtering**: Hardware filters to reduce CPU load
- **Interrupt handling**: Efficient ISR for real-time response
- **Memory management**: Avoid dynamic allocation in critical paths

### Timing Considerations
- **Message prioritization**: Critical messages sent first
- **Bus arbitration**: Let CAN protocol handle conflicts
- **Update frequency**: Balance responsiveness with CPU usage
- **Interrupt latency**: Keep ISR execution time minimal

## Testing Procedures

### Unit Tests
- Message formatting and parsing
- Error detection and recovery
- Buffer management
- Filter configuration

### Integration Tests
- Multi-node communication
- Bus loading tests
- Error injection testing
- Performance benchmarking

### Validation Tests
- Real vehicle testing
- EMI/EMC compliance
- Temperature cycling
- Vibration testing

## Troubleshooting Guide

### Common Issues
1. **No communication**: Check wiring, termination, baud rate
2. **Message corruption**: Verify signal integrity, grounding
3. **High error rate**: Check for EMI, bus loading
4. **Bus-off condition**: Reduce traffic, fix error sources

### Debug Procedures
1. **Oscilloscope analysis**: Check CAN_H/CAN_L signals
2. **Bus analyzer**: Monitor all traffic and timing
3. **Loopback testing**: Verify controller functionality
4. **Isolation testing**: Test individual nodes

## References
- [CAN Bus Specification (ISO 11898)](https://www.iso.org/standard/33422.html)
- [MCP2515 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP2515-Stand-Alone-CAN-Controller-with-SPI-20001801J.pdf)
- [Formula Hybrid Electrical Regulations](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [ESP32 SPI Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*