# State Machines

## 1-Summary



## 2-Purpose



## 3-Example Diagram + Explanation

The below diagram is 

```
┌────────┐        Precharge button         ┌────────────┐
│  Idle  │ ──────────────────────────────▶ │ Precharge  │
└────────┘                                 └────────────┘
     ▲                                          │
     │                                          │ Voltage stable
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



## 5-Conclusion