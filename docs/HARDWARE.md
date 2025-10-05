# Hardware Interfaces and Specifications

## Purpose
This document defines the hardware interfaces, pinouts, and electrical specifications for the ESP32 Formula Hybrid Core Library implementation.

## Scope
- ESP32 DevKit V1 pin assignments and usage
- Electrical interface specifications
- Hardware safety considerations
- Connection diagrams and pinouts
- Power requirements and distribution

## Safety Considerations
⚠️ **CRITICAL**: All hardware connections must be validated before powering the system. Incorrect wiring can damage components or create safety hazards in a race car environment.

## ESP32 DevKit V1 Pinout

### Power Pins
- **VIN**: 5V input from race car power system
- **3V3**: 3.3V regulated output (500mA max)
- **GND**: System ground reference

### Digital I/O Pins
```
GPIO Pin | Function              | Module        | Notes
---------|----------------------|---------------|--------
GPIO2    | Built-in LED         | Core          | Status indicator
GPIO4    | CAN TX               | CAN           | MCP2515 interface
GPIO5    | CAN RX               | CAN           | MCP2515 interface
GPIO12   | IMD Status           | IMD           | Digital input
GPIO13   | Pump Control         | Pump          | PWM output
GPIO14   | Brake Pressure       | Pedals        | Analog input
GPIO15   | Throttle Position    | Pedals        | Analog input
GPIO16   | Dashboard TX         | Dash          | UART communication
GPIO17   | Dashboard RX         | Dash          | UART communication
GPIO18   | SPI CLK              | Multiple      | Shared SPI bus
GPIO19   | SPI MISO             | Multiple      | Shared SPI bus
GPIO21   | I2C SDA              | Multiple      | Shared I2C bus
GPIO22   | I2C SCL              | Multiple      | Shared I2C bus
GPIO23   | SPI MOSI             | Multiple      | Shared SPI bus
GPIO25   | Battery Voltage      | Battery       | Analog input
GPIO26   | Battery Current      | Battery       | Analog input
GPIO27   | Inverter Enable      | Inverter      | Digital output
```

### Analog Inputs (ADC)
- **Resolution**: 12-bit (0-4095 counts)
- **Reference voltage**: 3.3V
- **Input range**: 0-3.3V (use voltage dividers for higher voltages)

### Communication Interfaces

#### UART (Serial Communication)
- **UART0**: USB programming/debugging
- **UART2**: Dashboard communication (GPIO16/17)

#### SPI (Serial Peripheral Interface)
- **CLK**: GPIO18
- **MISO**: GPIO19  
- **MOSI**: GPIO23
- **CS pins**: Module-specific GPIO assignments

#### I2C (Inter-Integrated Circuit)
- **SDA**: GPIO21
- **SCL**: GPIO22
- **Pull-up resistors**: 4.7kΩ required

## Power Requirements

### ESP32 DevKit V1
- **Operating voltage**: 3.3V (regulated on-board)
- **Input voltage**: 5V via VIN pin
- **Current consumption**: 
  - Active (WiFi): ~160mA
  - Active (no WiFi): ~80mA
  - Deep sleep: ~10μA

### External Components
- **CAN Transceiver**: 5V, ~50mA
- **Sensors**: Various (see individual module specs)
- **Display**: TBD based on dashboard selection

## Interface Specifications

### CAN Bus Interface
- **Protocol**: CAN 2.0B
- **Baud rate**: 500kbps (Formula Hybrid standard)
- **Termination**: 120Ω resistors at bus ends
- **Connector**: TBD (likely DB9 or Phoenix terminal)

### Sensor Interfaces
- **Analog sensors**: 0-3.3V input range
- **Digital sensors**: 3.3V logic levels
- **Current sensors**: Hall effect or shunt-based
- **Voltage sensors**: Resistive divider networks

## Safety Hardware

### Insulation Monitoring Device (IMD)
- **Input**: Digital status signal (high = fault)
- **Response time**: <100ms for fault detection
- **Isolation**: Optically isolated input recommended

### Emergency Shutdown
- **Master switch**: Hardware disconnect capability
- **Software monitoring**: Watchdog timer implementation
- **Fault conditions**: Automatic system shutdown

## PCB Layout Considerations

### Signal Integrity
- Separate analog and digital ground planes
- Keep high-speed signals short and direct
- Use proper decoupling capacitors (0.1μF + 10μF per IC)

### EMI/EMC Compliance
- Follow automotive EMC standards
- Use ferrite beads on power and signal lines
- Proper shielding for CAN bus wiring

### Thermal Management
- Ensure adequate ventilation around ESP32
- Consider heat dissipation in race car environment
- Operating temperature range: -40°C to +85°C

## Connection Diagrams

*TODO: Add detailed wiring diagrams for each module interface*

## Testing and Validation

### Hardware Testing
- Continuity testing for all connections
- Power supply validation (voltages, current draw)
- Signal integrity verification (oscilloscope)
- EMI testing in race car environment

### Safety Testing
- Insulation resistance testing
- Fault injection testing
- Emergency shutdown verification
- Environmental stress testing

## References
- [ESP32 DevKit V1 Datasheet](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)
- [Formula Hybrid Electrical Regulations](https://www.sae.org/programs/student-competitions/formula-hybrid/)
- [Automotive EMC Standards](https://en.wikipedia.org/wiki/Electromagnetic_compatibility)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Before each competition season*