# ESP32 Formula Hybrid Core Library - Technical Glossary

## Purpose
This document defines technical terms, abbreviations, and concepts used throughout the ESP32 Formula Hybrid Core Library documentation.

## Automotive and Racing Terms

**ASIL (Automotive Safety Integrity Level)** - Safety classification system from ISO 26262 (A=lowest, D=highest risk)

**BMS (Battery Management System)** - Electronic system that manages rechargeable battery pack safety and performance

**CAN (Controller Area Network)** - Automotive communication protocol for robust vehicle bus communication

**ECU (Electronic Control Unit)** - Embedded computer that controls one or more electrical systems in a vehicle

**FMEA (Failure Mode and Effects Analysis)** - Systematic method for evaluating potential failure modes

**Formula Hybrid** - Collegiate engineering competition combining internal combustion engine with electric drivetrain

**HMI (Human-Machine Interface)** - User interface enabling interaction between driver and vehicle systems

**IMD (Insulation Monitoring Device)** - Safety device that monitors electrical insulation integrity

**OBD (On-Board Diagnostics)** - Vehicle self-diagnostic and reporting capability

**TWAI (Two-Wire Automotive Interface)** - ESP32's implementation of CAN bus protocol

## Safety and Standards Terms

**Fail-Safe** - Design principle where system defaults to safe state upon failure

**Functional Safety** - Achievement of overall safety through correct functioning of safety systems

**Heartbeat** - Periodic signal indicating system health and communication status

**Plausibility Check** - Validation that sensor readings are reasonable and consistent

**Safe State** - Predetermined system condition that minimizes risk during faults

**Safety Critical** - Functions whose failure could result in harm to people or environment

**Watchdog** - Hardware/software mechanism that monitors system operation and resets on failure

## Electronic and Communication Terms

**ADC (Analog-to-Digital Converter)** - Circuit that converts analog signals to digital values

**Arbitration** - CAN bus mechanism for determining message transmission priority

**Bootloader** - Program that loads and starts the main application firmware

**Bus-Off** - CAN error state where node stops transmitting due to excessive errors

**DLC (Data Length Code)** - CAN message field indicating number of data bytes

**GPIO (General Purpose Input/Output)** - Configurable digital pin on microcontroller

**I2C (Inter-Integrated Circuit)** - Two-wire serial communication protocol

**IDE (Identifier Extension)** - CAN frame bit indicating extended (29-bit) vs standard (11-bit) ID

**ISR (Interrupt Service Routine)** - Function executed in response to hardware interrupt

**RTOS (Real-Time Operating System)** - OS designed for applications with strict timing requirements

**SPI (Serial Peripheral Interface)** - High-speed synchronous communication protocol

**UART (Universal Asynchronous Receiver-Transmitter)** - Serial communication interface

## Software Development Terms

**API (Application Programming Interface)** - Set of functions and protocols for software integration

**HAL (Hardware Abstraction Layer)** - Software layer that provides uniform interface to hardware

**Mock** - Simulated object used for testing that mimics behavior of real components

**Mutex (Mutual Exclusion)** - Synchronization primitive preventing concurrent access to shared resources

**Race Condition** - Flaw where system behavior depends on timing of uncontrollable events

**Semaphore** - Synchronization mechanism controlling access to shared resources

**Task** - Independent thread of execution in real-time operating system

**Unit Test** - Testing individual software components in isolation

## Formula Hybrid Specific Terms

**Energy Storage System (ESS)** - Battery pack and associated safety systems

**Motor Generator Unit (MGU)** - Electric motor/generator for hybrid drivetrain

**Powertrain Control Module (PCM)** - Central controller for engine and motor coordination

**Regen (Regenerative Braking)** - Process of recovering energy during braking

**State of Charge (SoC)** - Battery charge level expressed as percentage

**Traction Control** - System preventing wheel slip during acceleration

**Vehicle Dynamics** - Study of forces and motion affecting vehicle handling

## ESP32 Specific Terms

**Deep Sleep** - Low-power mode with minimal current consumption

**Flash Memory** - Non-volatile storage for program code and configuration

**FreeRTOS** - Real-time operating system used by ESP32

**OTA (Over-The-Air)** - Wireless firmware update capability

**PSRAM (Pseudo Static RAM)** - External memory expansion for ESP32

**WiFi Manager** - Library for managing WiFi connections and configuration

## Measurement Units and Abbreviations

**A** - Ampere (electric current)  
**°C** - Degrees Celsius (temperature)  
**Hz** - Hertz (frequency)  
**kbps** - Kilobits per second (data rate)  
**km/h** - Kilometers per hour (speed)  
**kW** - Kilowatt (power)  
**ms** - Milliseconds (time)  
**Nm** - Newton-meter (torque)  
**RPM** - Revolutions per minute (rotational speed)  
**V** - Volt (electric potential)  
**W** - Watt (power)  
**Ω** - Ohm (electrical resistance)  

## Error Codes and Status Values

**0x00** - No error/success status  
**0xFF** - General error/invalid value  
**CRC** - Cyclic Redundancy Check (error detection)  
**EOF** - End of File  
**NAK** - Negative Acknowledgment  
**SOF** - Start of Frame  
**STD** - Standard (11-bit CAN identifier)  
**XTD** - Extended (29-bit CAN identifier)  

## Timing and Performance Terms

**Deterministic** - System behavior that is predictable and repeatable

**Jitter** - Variation in timing of periodic events

**Latency** - Delay between stimulus and response

**Real-Time** - System constraint where correctness depends on timing

**Throughput** - Rate of successful data transmission

## Development Tools

**CANalyzer** - Professional CAN bus analysis tool

**JTAG** - Hardware debugging interface

**PlatformIO** - Cross-platform IDE for embedded development

**Unity** - C unit testing framework

**Wokwi** - Online ESP32 simulator and development platform

---

**Note:** This glossary is a living document. Add new terms as they are introduced in the project documentation.