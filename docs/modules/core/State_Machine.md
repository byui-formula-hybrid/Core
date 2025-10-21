# State Machines

## 1-Summary

State machines are extremely useful in understanding how the systems and subsystems of a car work. With state machines, it will be easier to follow how the car tracks certain vital information, so that the car keeps running smoothly.

As the team tracks what needs to change in the code for the car, state machines will help the team easily identify which state presents the problem and the team will fix the problem accordingly.

## 2-Purpose and Directions

The purpose of state machines is to give an outline for what systems in a car need to do in order to run properly and orderly.

State machines also allow for better communication between users and their systems, because a clear outline will be used to keep track of all procedures.

In addition, if complex systems are ever needed, or if a system needs to be expanded upon, then state machines allow for a more clean flow of information, making it easier for users to understand how a system works.

Because of this more user-friendly approach to coding, state machines allow more control through simpler debuggability and modification of certain variables.

To make a state machine, use flow charts like the one in Section 3 of this file, and label the different states and conditions for swtiching states, so that all information is clearly diagramed across the system.

To code a state machine, use a structure like the on presented in Section 4 of this file. First, set the states that the car can be in and set the conditions for which states are switched.

## 3-Example Diagram + State Machine Explanation

The below diagram is a diagram representing state machine tracking in a tractive system for the car.

Here, there are four states that the car can be in: Idle, Precharge, Drive, and Fault.

There are also four actions that move the car from one state to another, which are represented by the arrows and their descriptive condition for changing from one state to another.

This kind of flow chart can represent different subsystems of the car like control systems (cooling and braking), safety and fault handling, and user interfaces.

```
┌────────┐        Precharge button         ┌────────────┐
│  Idle  │ ──────────────────────────────▶ │ Precharge  │
└────────┘                                 └────────────┘
     ▲                                          │
     │                                          │ Voltage stable                                          |
     │                                          ▼
┌────────┐     Reset button             ┌────────┐
│ Fault  │ ◀─────────────────────────── │ Drive  │
└────────┘       Fault detected         └────────┘
```

## 4-Example Code + Explanation

    enum class SystemState {
        GLV_OFF,
        TS_OFF_GLV_ON,
        PRECHARGE,
        TS_ENERGIZED_NOT_RDY,
        TS_ENERGIZED_RDY,
        FAULT
    };
    void handleEvent(SystemState& state, const std::string& event) {
        switch (state) {
            case SystemState::GLV_OFF:
                if (event == "GLV_ON") state =      SystemState::TS_OFF_GLV_ON;
                break;
            case SystemState::TS_OFF_GLV_ON:
                if (event == "TSMS_ON") state = SystemState::PRECHARGE;
                break;
            case SystemState::PRECHARGE:
                if (event == "CHARGED") state = SystemState::TS_ENERGIZED_NOT_RDY;
                break;
            case SystemState::TS_ENERGIZED_NOT_RDY:
                if (event == "MC_ACTIVATE") state = SystemState::TS_ENERGIZED_RDY;
                break;
            case SystemState::TS_ENERGIZED_RDY:
                if (event == "FAULT") state = SystemState::FAULT;
                break;
            case SystemState::FAULT:
                if (event == "RESET") state = SystemState::TS_OFF_GLV_ON;
                break;
        }
    }

This subsystem has 6 different states as defined by the enum section of the code: GLV_OFF, TS_OFF_GLV_ON, PRECHARGE, TS_ENERGIZED_NOT_RDY, TS_ENERGIZED_RDY, and FAULT.

This code tells the car under what conditions does the car switch from one state to another and what states to switch to. This code in particular will cycle through the states listed above in sequential order, indicating specific moments describing when the subsystem should switch states.

## 5-Conclusion

State machines are implemented to keep every bit of code orgranized, so that when the code calls up a variable or a function or a feature involved with running the racecar, then the state machine will help us understand where the car is at in the program while the team is running the code.

State machines keep all information about systems in an orderly fasion, so that the users can easily identify problems and improve the car's systems.