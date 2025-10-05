# ESP32 Formula Hybrid Core Library - Documentation Standards

This directory contains technical documentation for the ESP32 Formula Hybrid Core Library. This document establishes standards and expectations for all documentation in this project.

## 📋 **Documentation Standards**

### **Writing Style**
- **Technical but accessible** - Assume reader has basic embedded/automotive knowledge
- **Concise and actionable** - Every document should have clear takeaways
- **Safety-focused** - Always emphasize safety considerations for race car applications
- **Version-aware** - Include version/date information where relevant

### **Required Structure**
All major documentation files must include:

1. **Purpose statement** - What this document covers and why it exists
2. **Scope and limitations** - What's included/excluded
3. **Safety considerations** - Race car specific safety requirements
4. **Technical specifications** - Concrete implementation details
5. **Examples and code samples** - Practical usage demonstrations
6. **References and links** - External standards, datasheets, regulations

## 📁 **Documentation Organization**

### **Core Documentation Files**
- `README.md` - This standards document  
- `Design.md` - **Vehicle system design** - Overall car architecture and major subsystems
- `ARCHITECTURE.md` - **ESP32 library architecture** - Internal code structure and module organization
- `SAFETY.md` - Safety requirements, fault handling, and compliance
- `CAN.md` - CAN bus message definitions and communication protocols
- `HARDWARE.md` - Hardware interfaces, pinouts, and electrical specs
- `TESTING.md` - Test procedures, validation, and simulation

### **Module-Specific Documentation**
Each library module should have corresponding documentation:
- `modules/BATTERY.md` - Battery management system interface
- `modules/CAN.md` - CAN communication implementation
- `modules/CORE.md` - Core utilities and shared functionality
- `modules/DASH.md` - Dashboard and display interface
- `modules/IMD.md` - Insulation monitoring device interface
- `modules/INVERTER.md` - Motor inverter control interface
- `modules/PEDALS.md` - Pedal position and safety systems
- `modules/PUMP.md` - Pump control and monitoring

### **Reference Documentation**
- `references/FORMULAS.md` - Mathematical formulas and calculations
- `references/STANDARDS.md` - Automotive and racing standards compliance
- `references/GLOSSARY.md` - Technical terms and abbreviations
- `references/WIRING.md` - Electrical diagrams and connection specs