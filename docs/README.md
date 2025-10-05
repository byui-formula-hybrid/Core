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

## 🔧 **Technical Documentation Requirements**

### **Code Documentation**
```cpp
/**
 * @brief Brief description of function/class
 * @details Detailed explanation including:
 *          - Expected behavior
 *          - Safety considerations
 *          - Performance characteristics
 * 
 * @param parameter_name Description and valid ranges
 * @return Description of return value and error conditions
 * 
 * @safety Critical/Non-critical classification
 * @tested Yes/No with test reference
 * 
 * @example
 * // Usage example
 * BatteryManager battery;
 * bool status = battery.initialize();
 */
```

### **Hardware Documentation**
- **Pin assignments** with electrical characteristics
- **Communication protocols** with timing diagrams
- **Power requirements** and consumption analysis
- **Environmental specifications** (temperature, vibration, etc.)
- **Connector specifications** and mating parts

### **Safety Documentation**
- **Risk assessment** for each component/function
- **Failure modes** and mitigation strategies
- **Testing requirements** for safety validation
- **Compliance mapping** to racing regulations

## 🏁 **Formula Hybrid Specific Requirements**

### **Safety Standards**
All documentation must address:
- **FMEA (Failure Mode and Effects Analysis)** for critical systems
- **Fault detection and handling** mechanisms
- **Emergency shutdown** procedures
- **Driver safety** considerations
- **Spectator and crew safety** measures

### **Performance Requirements**
- **Real-time constraints** and timing analysis
- **Power consumption** and battery life impact
- **Temperature operating ranges**
- **Vibration and shock resistance**
- **EMI/EMC considerations** for race environment

### **Regulatory Compliance**
- **Formula Hybrid rules** compliance documentation
- **FIA/SAE standards** where applicable
- **Local racing authority** requirements
- **Technical inspection** preparation guides

## 📊 **Documentation Templates**

### **Module Documentation Template**
```markdown
# Module Name

## Purpose
Brief description of what this module does and why it exists.

## Safety Classification
- [ ] Safety Critical (affects vehicle control or driver safety)
- [ ] Safety Related (monitoring or diagnostic functions)
- [ ] Non-Safety Critical (convenience features)

## Hardware Interface
- Pins used and electrical specifications
- Communication protocols and timing
- Power requirements

## Software Interface
- Public API with examples
- Configuration parameters
- Error handling and return codes

## Safety Considerations
- Failure modes and detection
- Fault handling procedures
- Testing requirements

## Performance Characteristics
- Timing constraints
- Resource usage (memory, CPU)
- Power consumption

## Testing
- Unit test coverage
- Integration test procedures
- Hardware-in-the-loop testing

## References
- Relevant standards and regulations
- Vendor documentation
- Related modules and dependencies
```

### **API Documentation Template**
```markdown
## Function Name

**Purpose:** What this function does

**Safety:** Critical/Related/Non-Critical

**Signature:**
```cpp
return_type function_name(param1_type param1, param2_type param2);
```

**Parameters:**
- `param1`: Description, valid range, units
- `param2`: Description, valid range, units

**Returns:**
- Success condition and return value
- Error conditions and error codes

**Example:**
```cpp
// Practical usage example
```

**Safety Notes:**
- Any safety-critical behavior
- Failure handling requirements
```

## ✅ **Documentation Quality Checklist**

Before submitting documentation, verify:

### **Technical Accuracy**
- [ ] All code examples compile and run
- [ ] Hardware specifications match actual implementation
- [ ] Safety classifications are correct
- [ ] Performance claims are validated

### **Completeness**
- [ ] All required sections are present
- [ ] API documentation covers all public functions
- [ ] Safety considerations are thoroughly addressed
- [ ] Examples demonstrate real-world usage

### **Clarity**
- [ ] Language is clear and unambiguous
- [ ] Technical terms are defined or linked to glossary
- [ ] Diagrams and code examples support the text
- [ ] Document flows logically from concept to implementation

### **Maintenance**
- [ ] Version information is current
- [ ] Links to external resources are valid
- [ ] References to other modules are accurate
- [ ] TODO items are tracked and assigned

## 🚀 **Getting Started**

1. **Read this standards document** thoroughly
2. **Review existing documentation** for examples (noting that current docs may not meet these standards)
3. **Install recommended VS Code extensions** (run `.scripts/install.sh` to auto-install)
4. **Start with safety documentation** for any new modules
5. **Use the templates** provided above
6. **Preview Mermaid diagrams** using the Markdown Mermaid Preview extension
7. **Get peer review** before finalizing documentation

### **Recommended VS Code Extensions**

The install script automatically installs these extensions:
- **PlatformIO IDE** - ESP32 development and debugging
- **C/C++ Extension Pack** - Code editing and IntelliSense  
- **Wokwi Simulator** - ESP32 simulation and testing
- **Markdown Mermaid Preview** - Live preview of Mermaid diagrams
- **YAML Language Support** - Enhanced YAML editing for configs

## 📞 **Questions and Feedback**

For questions about documentation standards or to suggest improvements:
- Open an issue in the project repository
- Discuss in team meetings
- Propose changes via pull request

---

**Remember:** Good documentation saves lives in racing applications. Take the time to do it right.