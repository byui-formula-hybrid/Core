# Formula Hybrid + Electric Rulebook Information Reguarding Software

The following information reguarding the rules and regulations of the software has been extracted and placed in this document as a helpful reference for the software engineers of the team.  

Reference to the actual rulebook are given at the end of the short summary incase further questions arise. 

## DO's (Allowed or Required Actions)

**DO #1**  
Adjustment to engine operating parameters, e.g. fuel mixture and ignition timing and any software calibration changes are permitted once the vehicle is approved to compete in dynamic events. (Page: 151, Paragraph: T1.2.2(e)).

**DO #2**  
Changes to vehicle’s software and operating parameters are permitted once the vehicle is approved to compete in dynamic events. (Page: 153, Paragraph: T1.2.2(p)).

**DO #3**  
Rear wheel steering, which can be electrically actuated, is permitted but only if mechanical stops limit the range of angular movement of the rear wheels to a maximum of six degrees (6°) (Page: 293, Paragraph: T6.5.5).

**DO #4**  
If choosing to use steering systems using cables for actuation, the team must submit a failure modes and effects analysis report with design details of the proposed system as part of the Structural Equivalency Spreadsheet (SES) (Page: 294, 295, Paragraph: T6.5.8).

**DO #5**  
Accelerator control may be actuated mechanically, electrically or electronically, i.e. electrical Accelerator control (ETC) or “drive-by-wire” is acceptable, provided the system is designed and constructed as a "fail safe" system (Page: 358, Paragraph: IC1.6.2).

**DO #6**  
If electrical or electronic throttle actuation is used, the methodology used to ensure fail-safe operation must be included as a required appendix to the Design Report (Page 362, Paragraph: IC1.6.4).

**DO #7**  
Pre-charge timing must be based on a voltage measurement of the intermediate circuit voltage compared to the voltage of the accumulator (Page: 420, Paragraph: EV2.10.1).

**DO #8**  
Teams may design and build their own Accumulator Management Systems (AMS) (Page: 432, Paragraph: EV2.11.9).

**DO #9**  
The AMS circuit board for microprocessor-based systems must include a watchdog timer (Page: 433, Paragraph: EV2.11.9(2)).

**DO #10**  
All acceleration control signals (between the accelerator pedal and the motor controller) must have error checking. For digital acceleration control signals, this error checking must detect a loss of communication (Page: 456, 457, Paragraph: EV3.5.4).

**DO #11**  
TS drive motors must spin freely when the Tractive System (TS) is in a deactivated state, and when transitioned to a deactivated state (Page: 459, Paragraph: EV3.5.9).

**DO #12**  
The cockpit shutdown button must be driver resettable (must be able to restore system operation by pulling the button back out) (Page: 495, Paragraph: EV7.6.4).

**DO #13**  
Teams must demonstrate that their vehicle operates according to the standard shutdown system state diagram (Figure 41), OR obtain approval for an alternative state diagram by submitting an electrical rules query (Page: 497, Paragraph: EV7.8.2).

**DO #15**  
Latching circuitry added by teams to comply with IMD/AMS faults must be implemented using electro-mechanical relays (Page: 500, Paragraph: EV7.9.5).

**DO #16**  
If charging on the vehicle, AMS and IMD systems must be active during charging. The external charging system must shut down if there is an AMS or IMD fault (Page: 532, Paragraph: EV12.2.15).

## DON'Ts (Not Allowed or Restrictions on Implementation)

**DON'T #1**  
"Steer-by-wire” or electrically actuated steering is prohibited. The steering wheel must be mechanically connected to the wheels (Page: 292, Paragraph: T6.5.1).

**DON'T #2**  
"Brake-by-wire” systems are prohibited (Page: 302, Paragraph: T7.1.5).

**DON'T #3**  
The brake over-travel switch must not operate through programmable logic controllers, engine control units, or digital controllers (Page: 307, Paragraph: T7.3.4).

**DON'T #4**  
Digital logic or microcontrollers may not be used to implement the Accumulator Management System (AMS) latching function; a mechanical relay circuit must be used (Page: 426, Paragraph: EV2.11.3).

**DON'T #5**  
The processor for microprocessor-based AMSs must be dedicated to the AMS function only (Page: 433, Paragraph: EV2.11.9(1)).

**DON'T #6**  
TS circuitry, even at low voltage or current levels, is not allowed in the cockpit. All control signals referenced to TS must be galvanically isolated and referenced to GLV ground (Page: 458, Paragraph: EV3.5.7).

**DON'T #7**  
Digital logic or microcontrollers may not be used for the safety shutdown loop function; it must be implemented as a series connection (logical AND) of all required safety devices (Page: 486, Paragraph: EV7.1.3).

**DON'T #8**  
Electronic systems that contain internal energy storage must be prevented from feeding power back into the vehicle GLV circuits in the event of GLV shutdown (Page: 487, Paragraph: EV7.1.6).

**DON'T #9**  
Side-mounted shutdown buttons (BRBs) may not act through logic such as a micro-controller or relays (Page: 493, Paragraph: EV7.5.5).

**DON'T #10**  
Cockpit shut-down buttons may not act through logic such as a micro-controller or relays (Page: 496, Paragraph: EV7.6.5).

**DON'T #11**  
In case of an IMD-related fault, the tractive system must remain disabled until manually reset by a person other than the driver. It must not be possible for the driver to re-activate the tractive system from within the car (Page: 500, Paragraph: EV7.9.5).

**DON'T #12**  
Vehicles that use CANbus as a fundamental element of the safety shutdown system (e.g., AMS communication) will be required to document and demonstrate shutdown equivalent to BRB operation if CAN communication is interrupted (Page: 505, 506, Paragraph: EV8.1.6).

**DON'T #13**  
The accumulator voltage indicator (for removable containers) must be directly controlled by voltage present at the container connectors using analog electronics. No software control is permitted (Page: 513, Paragraph: EV9.5.2)



