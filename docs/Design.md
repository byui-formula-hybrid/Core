# High-level architecture

* **Pedal Board (Throttle / Brake inputs)**

  * Reads pedal positions and velocity (accelerator + regen request + brake).
  * Outputs: redundant pedal position signals, brake pedal switch, push-to-fail-safe heartbeat.
  * Communicates to Brain over vehicle bus (CAN).
  * Local safety: dual independent sensors + plausibility checking + ASIL-like behavior (see Safety below).

* **Display / UI Board**

  * Handles driver displays, gauges, touch/buttons, infotainment.
  * Reads non-critical sensors (cabin temp) and sends UI commands to Brain.
  * Receives state updates from Brain.
  * Separate safety domain: can show warnings but not take drivetrain actions.

* **Brain (Master ECU / Vehicle Controller)**

  * Central authority for traction, motor control commands, regen logic, battery requests, high-level state machine (drive/park/fault).
  * Aggregates inputs (pedals, wheel speed sensors, BMS, ABS/ESC nodes).
  * Sends motor controller torque/velocity commands or interfaces to dedicated motor controllers/inverters.
  * Runs safety & watchdog, fault management, logging, and vehicle state transitions.

# Communication & network

* **Primary bus:** CAN (ISO 11898) — robust, deterministic arbitration, automotive-proven. Consider CAN FD if you need larger payloads.
* **Bus topology:** single bus with termination at both ends; use isolated transceivers if grounds can be different (battery stacks).
* **Message principles:** small, frequent messages for control; heartbeat every 10–100 ms; lowest-latency paths for safety-critical commands.

Example CAN message set (illustrative):

* 0x100 — Pedal Status (8 bytes)

  * Byte0: flags (validity bits, failsafe active)
  * Bytes1-2: accel_pos_mV (0..65535) -> decode to 0..100%
  * Bytes3-4: brake_pos_mV
  * Bytes5-6: accel_vel (optional)
  * Byte7: checksum/rolling counter
* 0x200 — Brain Command (8 bytes) from Brain to Motor Controller

  * Bytes0-1: torque_request (signed int16)
  * Bytes2-3: regen_request
  * Byte4: drive_mode
  * Byte5: safety_state
  * Bytes6-7: counter/checksum
* 0x300 — Heartbeat (1 byte)

  * sender ID, state (OK/FAULT)
* 0x400 — Diagnostics / Faults (8 bytes)

  * Fault codes, timestamp, severity

Use a rolling counter + CRC per message for replay protection + receiver sanity checks.

# Sensors & redundancy

* **Pedal board**

  * Use *dual* independent position sensors per pedal (e.g., two Hall-effect sensors or a potentiometer + Hall) with different physical measurands so single-sensor failure is detectable.
  * Sample at >= 1 kHz for responsiveness; debounce and filter but keep latency low.
  * Provide a hard brake switch (mechanical) for instant detection and an independent brake pressure or ABS signal if available.

* **Other sensors**

  * Wheel speeds: 4x encoders or hall sensors.
  * Current sensing: high-side or shunt amplifiers with isolation for battery pack.
  * Voltage and temperature: multiple points in battery packs (handled by BMS, but Brain should monitor aggregated BMS state).

# Safety & fault handling

* **Fail-safe design**: identify safe state per failure (e.g., reduce torque to zero, enable limp-home mode, allow regen-only).
* **Watchdogs**: hardware watchdog on every MCU + external watchdog for the Brain. Heartbeats required; loss of heartbeat triggers safe state.
* **Plausibility checks**: cross-check sensors (e.g., accel demand vs brake pressed — if both present with non-plausible values, go to safe limit).
* **Graceful degradation**: display warns driver and log faults.
* **Logging**: non-volatile fault logs for post-mortem (circular buffer + dump via diagnostics).
* **Functional safety standard**: design with ISO 26262 concepts in mind even if you do not certify — e.g., redundancy, diagnostics, failure rate analysis.

# Security

* **Secure boot** for Brain and important ECUs (verify firmware signatures).
* **CAN message authentication**: at minimum, use message counters and CRCs; ideally use CAN-FD with MACs or a secure gateway if you need cryptographic authenticity.
* **Access control**: privileged diagnostics only over authenticated channels.
* **OTA caution**: if you enable firmware updates, require cryptographic signatures and staged rollbacks.

# MCU / hardware recommendations (categories)

* **Brain (safety/performance)**: automotive-grade MCUs: Cortex-M7/M33 or automotive MCUs (NXP S32K family, Renesas RH850, TI TMS570 for ISO-26262). Pick one with:

  * Hardware FPU, multiple CAN controllers, DMA, plenty of flash/RAM, and ECC RAM if possible.
* **Pedal board**: Cortex-M4/M0+/STM32G or similar — needs ADCs, external Hall sensor interfaces, and CAN transceiver.
* **Display**: MCU with LCD/graphics support or a small SoC (e.g., Cortex-A if complex UI) — separates infotainment from safety-critical domain.

Hardware notes:

* Use **isolated CAN transceivers** if you have differing ground domains or large battery domains.
* Supply: robust power regulators with transient suppression, large decoupling capacitors. Use buck regulators for MCU + LDO for analog refs.
* ADC accuracy: choose ADC resolution and input conditioning so you get at least 12–14 bits effective for pedal sensing.
* PCB considerations: star ground for analog circuits, separate digital/analog ground pours, EMI filtering.

# Firmware architecture & patterns

* **State machine / modes**: OFF, STANDBY, READY, DRIVE, FAULT. Centralize transition rules in Brain.
* **Real-time tasks**: separate ISR for bus reception, sensor sampling at fixed intervals, control loop (e.g., 1 ms or 5 ms loop), logging at lower priority.
* **Watchdog & watchdog feeding policy**: only feed when main tasks completed and health checks pass.
* **Diagnostics & self-test**: power-on self-test (POST) and continuous built-in tests.
* **Bootloader**: small, secure bootloader with signed images and rollback protection.

# Testing & verification checklist

1. Unit test sensor reading / plausibility logic (simulate sensor faults).
2. Integration test pedal-to-brain latency (measure worst-case ms).
3. Fault injection: loss of pedal board, corrupted CAN messages, shorted ADC inputs — verify safe state engaged.
4. EMI testing: run near motors and switching components to validate bus noise immunity.
5. Thermal test: run boards under expected temperature extremes.
6. End-to-end driving test at low speed with safety driver and kill-switch.

# Quick example — pedal plausibility rules (pseudo)

* If |accel_pos_A − accel_pos_B| > threshold → fault (sensor disagreement).
* If brake_pressed && accel_pos > small_threshold → if both sustained → reduce torque and alert driver.
* On loss of pedal heartbeat > X ms → commanded torque = 0 and vehicle state → FAULT.

# Next practical steps (pick any)

* I can draft a detailed CAN message map (IDs, bitfields, scaling).
* Or produce a recommended bill of materials (MCU, transceivers, ADCs, isolated DC/DC).
* Or write the pedal-board firmware state machine pseudo-code and safety checks.
