 ## DOs (Permitted / Required)

#### DO: Software calibration and operating parameter changes allowed (T1.2.2(e),(o),(p), page ~14)
Teams may adjust software calibration and operating parameters once the car has passed inspection.This includes engine parameters such as fuel mixture and ignition timing, motor controller settings, andvehicle software. These changes are critical for fine-tuning vehicle performance and adapting to trackconditions.
#### DO: Independent CAN watchdog required if CAN is used for safety (EV7.1.1(10), pages ~104–106)
If CAN communication is used to transmit safety-critical messages, a dedicated CAN watchdog device must be installed. This device will open a relay if expected CAN messages are not received, ensuring the shutdown system is not compromised by a frozen or failed software bus.
#### DO: Document software safety logic (EV13.1, EV13.2, A9.1, A10(f), pages ~12–16, 124)
All safety-related software logic must be documented in the Electrical System Forms (ESF-1 and ESF-2) and included in the Program Submission. Inspectors rely on these documents to validate that software-dependent safety functions are properly implemented.
#### DO: AMS testing may involve software setpoints (EV10.4.1, page ~118)
The Accumulator Management System (AMS) can be tested by adjusting software trip thresholds to simulate conditions where cell voltage exceeds limits. This demonstrates that the AMS responds correctly to both real and simulated unsafe states.
#### DO: Provide fallback if CAN-based safety fails (EV8.1.6, page ~113)
If CANbus is used as part of the safety shutdown system, teams must show a hardware-equivalent fallback mechanism. This ensures the system can still shut down safely if CAN communication is interrupted. An example is a separate CAN monitoring device with its own relay output.

## DOs (Permitted / Required)

#### DON'T: Shutdown buttons may not act through logic (EV7.5.5, EV7.6.5, page ~108)
Both side-mounted and cockpit shutdown buttons must act directly on the shutdown loop. They cannot pass through software, logic controllers, or relays. This guarantees immediate hardware shutdown when activated.
#### DON'T: Accumulator voltage indicator must be analog only (EV9.5.2, page ~115)
The accumulator voltage indicator must be directly wired to the accumulator voltage using analog electronics. Software must not influence or process this indicator, ensuring that inspectors see a true, unfiltered measurement.
#### DON'T: Stored energy systems may not back-feed GLV after shutdown (EV7.1.6, page ~106)
If a system contains internal energy storage (capacitors, batteries), it must not be able to feed power back into the low-voltage system after shutdown. This requirement must be satisfied in hardware, not software, to guarantee safe shutdown under all conditions.