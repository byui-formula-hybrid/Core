# Mathematical Formulas and Calculations

## Purpose
This document provides mathematical formulas, calculations, and algorithms used throughout the ESP32 Formula Hybrid Core Library for accurate system modeling and control.

## Scope
- Power and energy calculations
- Thermal modeling equations
- Control system mathematics
- Sensor calibration formulas
- Performance metrics calculations

## Power and Energy Calculations

### Battery System Formulas

#### State of Charge (SOC) - Coulomb Counting
```
SOC(t) = SOC₀ + (1/C_rated) ∫[I(τ)]dτ from 0 to t

Where:
- SOC(t) = State of charge at time t (0-1)
- SOC₀ = Initial state of charge
- C_rated = Rated battery capacity (Ah)
- I(τ) = Current at time τ (A, positive for discharge)
```

#### Battery Power Calculation
```
P_battery = V_pack × I_pack

Where:
- P_battery = Battery power (W)
- V_pack = Pack voltage (V)
- I_pack = Pack current (A)
```

#### Energy Consumption
```
E_consumed = ∫[P(t)]dt from t₀ to t₁

Where:
- E_consumed = Energy consumed (Wh)
- P(t) = Power at time t (W)
- t₀, t₁ = Start and end times (h)
```

### Motor Control Formulas

#### Torque-Speed Relationship
```
P_motor = T × ω = T × (2π × RPM) / 60

Where:
- P_motor = Motor power (W)
- T = Torque (Nm)
- ω = Angular velocity (rad/s)
- RPM = Rotational speed (rev/min)
```

#### Motor Efficiency
```
η_motor = P_mechanical / P_electrical × 100%

Where:
- η_motor = Motor efficiency (%)
- P_mechanical = Mechanical output power (W)
- P_electrical = Electrical input power (W)
```

#### Vehicle Speed from Motor RPM
```
v_vehicle = (π × d_wheel × RPM) / (60 × r_gear)

Where:
- v_vehicle = Vehicle speed (m/s)
- d_wheel = Wheel diameter (m)
- RPM = Motor rotational speed (rev/min)
- r_gear = Gear ratio (unitless)
```

## Thermal Modeling

### Heat Transfer Calculations

#### Newton's Law of Cooling
```
q = h × A × (T_surface - T_ambient)

Where:
- q = Heat transfer rate (W)
- h = Heat transfer coefficient (W/m²·K)
- A = Surface area (m²)
- T_surface = Surface temperature (K)
- T_ambient = Ambient temperature (K)
```

#### Thermal Time Constant
```
τ = R_thermal × C_thermal

Where:
- τ = Thermal time constant (s)
- R_thermal = Thermal resistance (K/W)
- C_thermal = Thermal capacitance (J/K)
```

#### Temperature Rise Prediction
```
T(t) = T_ambient + (P × R_thermal) × (1 - e^(-t/τ))

Where:
- T(t) = Temperature at time t (K)
- T_ambient = Ambient temperature (K)
- P = Power dissipated (W)
- R_thermal = Thermal resistance (K/W)
- τ = Thermal time constant (s)
```

### Battery Thermal Model

#### Battery Heat Generation
```
Q_battery = I² × R_internal + |I × V_ocv - I × V_terminal|

Where:
- Q_battery = Heat generation rate (W)
- I = Battery current (A)
- R_internal = Internal resistance (Ω)
- V_ocv = Open circuit voltage (V)
- V_terminal = Terminal voltage (V)
```

#### Battery Temperature Rise
```
dT/dt = (Q_battery - q_cooling) / (m × c_p)

Where:
- dT/dt = Rate of temperature change (K/s)
- Q_battery = Heat generation (W)
- q_cooling = Heat removal by cooling (W)
- m = Battery mass (kg)
- c_p = Specific heat capacity (J/kg·K)
```

## Control System Mathematics

### PID Control Algorithm
```
u(t) = K_p × e(t) + K_i × ∫e(τ)dτ + K_d × de(t)/dt

Where:
- u(t) = Control output
- e(t) = Error signal (setpoint - measured value)
- K_p = Proportional gain
- K_i = Integral gain
- K_d = Derivative gain
```

#### Discrete PID Implementation
```
u[n] = K_p × e[n] + K_i × Σe[k] × Δt + K_d × (e[n] - e[n-1])/Δt

Where:
- u[n] = Control output at sample n
- e[n] = Error at sample n
- Δt = Sample time (s)
- Σe[k] = Sum of all previous errors
```

### Low-Pass Filter (First Order)
```
y[n] = α × x[n] + (1-α) × y[n-1]

Where:
- y[n] = Filtered output at sample n
- x[n] = Input at sample n
- α = Filter coefficient (0 < α < 1)
- α = 1 / (1 + RC × f_s) for analog equivalent
```

### Moving Average Filter
```
y[n] = (1/N) × Σ(x[n-k]) for k = 0 to N-1

Where:
- y[n] = Filtered output
- x[n-k] = Input samples
- N = Number of samples in average
```

## Sensor Calibration

### Linear Sensor Calibration
```
Physical_Value = (ADC_Reading - Offset) × Scale_Factor

Where:
- Physical_Value = Calibrated sensor reading
- ADC_Reading = Raw ADC value
- Offset = Zero-point offset
- Scale_Factor = Sensitivity (units per count)
```

#### Two-Point Calibration
```
Scale_Factor = (Value₂ - Value₁) / (ADC₂ - ADC₁)
Offset = ADC₁ - (Value₁ / Scale_Factor)

Where:
- Value₁, Value₂ = Known physical values
- ADC₁, ADC₂ = Corresponding ADC readings
```

### Thermistor Temperature Calculation (Steinhart-Hart)
```
1/T = A + B × ln(R) + C × [ln(R)]³

Where:
- T = Temperature (Kelvin)
- R = Resistance (Ω)
- A, B, C = Steinhart-Hart coefficients
```

#### Simplified Beta Formula
```
T = 1 / (1/T₀ + (1/β) × ln(R/R₀))

Where:
- T = Temperature (Kelvin)
- T₀ = Reference temperature (Kelvin)
- R = Measured resistance (Ω)
- R₀ = Resistance at reference temperature (Ω)
- β = Beta coefficient (K)
```

### Pressure Sensor Calibration
```
Pressure = ((V_sensor - V_min) / (V_max - V_min)) × (P_max - P_min) + P_min

Where:
- Pressure = Calibrated pressure reading
- V_sensor = Sensor voltage output
- V_min, V_max = Minimum and maximum sensor voltages
- P_min, P_max = Corresponding pressure range
```

## Efficiency and Performance Metrics

### Overall System Efficiency
```
η_system = (P_mechanical / P_battery) × 100%

Where:
- η_system = Overall system efficiency (%)
- P_mechanical = Mechanical power output (W)
- P_battery = Battery power input (W)
```

### Energy Efficiency (Vehicle)
```
Efficiency = Distance / Energy_Consumed (km/kWh or mi/kWh)

Where:
- Distance = Distance traveled (km or mi)
- Energy_Consumed = Energy consumed from battery (kWh)
```

### Range Estimation
```
Range = (SOC × C_rated × V_nominal × η_system) / P_average

Where:
- Range = Estimated remaining range (km)
- SOC = Current state of charge (0-1)
- C_rated = Battery capacity (Ah)
- V_nominal = Nominal battery voltage (V)
- η_system = System efficiency (0-1)
- P_average = Average power consumption (W)
```

## Statistical Calculations

### Root Mean Square (RMS)
```
RMS = √[(1/N) × Σ(x_i²)] for i = 1 to N

Where:
- RMS = Root mean square value
- N = Number of samples
- x_i = Individual sample values
```

### Standard Deviation
```
σ = √[(1/N) × Σ(x_i - μ)²] for i = 1 to N

Where:
- σ = Standard deviation
- μ = Mean value
- N = Number of samples
- x_i = Individual sample values
```

### Weighted Average
```
Weighted_Average = Σ(w_i × x_i) / Σ(w_i) for i = 1 to N

Where:
- w_i = Weight for sample i
- x_i = Sample value i
- N = Number of samples
```

## Digital Signal Processing

### ADC Resolution and Quantization
```
Resolution = V_ref / (2^n - 1)

Where:
- Resolution = ADC resolution (V/count)
- V_ref = Reference voltage (V)
- n = Number of bits
```

### Frequency from Period Measurement
```
Frequency = 1 / Period

Where:
- Frequency = Signal frequency (Hz)
- Period = Signal period (s)
```

### Duty Cycle Calculation
```
Duty_Cycle = (T_high / T_period) × 100%

Where:
- Duty_Cycle = PWM duty cycle (%)
- T_high = High time duration (s)
- T_period = Total period (s)
```

## Implementation Examples (C++)

### Temperature Conversion Functions
```cpp
// Celsius to Fahrenheit
float celsiusToFahrenheit(float celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

// Thermistor resistance to temperature (Beta formula)
float thermistorToTemperature(float resistance, float r0, float t0, float beta) {
    float t0_kelvin = t0 + 273.15;
    float temp_kelvin = 1.0 / (1.0/t0_kelvin + (1.0/beta) * log(resistance/r0));
    return temp_kelvin - 273.15; // Convert to Celsius
}
```

### Power and Energy Calculations
```cpp
// Calculate instantaneous power
float calculatePower(float voltage, float current) {
    return voltage * current;
}

// Update energy consumption (trapezoidal integration)
float updateEnergyConsumption(float lastPower, float currentPower, float deltaTime) {
    return (lastPower + currentPower) * deltaTime / 2.0; // Wh if deltaTime in hours
}

// SOC calculation using coulomb counting
float updateSOC(float currentSOC, float current, float deltaTime, float capacity) {
    float deltaSOC = (current * deltaTime) / (capacity * 3600.0); // deltaTime in seconds
    return currentSOC - deltaSOC; // Subtract for discharge (positive current)
}
```

### Control Algorithms
```cpp
// PID Controller implementation
class PIDController {
private:
    float kp, ki, kd;
    float integral, lastError;
    float outputMin, outputMax;
    
public:
    float calculate(float setpoint, float measured, float deltaTime) {
        float error = setpoint - measured;
        
        // Proportional term
        float proportional = kp * error;
        
        // Integral term
        integral += error * deltaTime;
        float integralTerm = ki * integral;
        
        // Derivative term
        float derivative = (error - lastError) / deltaTime;
        float derivativeTerm = kd * derivative;
        
        // Calculate output
        float output = proportional + integralTerm + derivativeTerm;
        
        // Apply output limits
        output = constrain(output, outputMin, outputMax);
        
        lastError = error;
        return output;
    }
};
```

### Filtering Functions
```cpp
// Low-pass filter
float lowPassFilter(float input, float lastOutput, float alpha) {
    return alpha * input + (1.0 - alpha) * lastOutput;
}

// Moving average filter
class MovingAverage {
private:
    float* buffer;
    int size;
    int index;
    float sum;
    
public:
    MovingAverage(int bufferSize) {
        size = bufferSize;
        buffer = new float[size];
        index = 0;
        sum = 0;
        for (int i = 0; i < size; i++) buffer[i] = 0;
    }
    
    float update(float input) {
        sum -= buffer[index];
        buffer[index] = input;
        sum += input;
        index = (index + 1) % size;
        return sum / size;
    }
};
```

## Constants and Reference Values

### Physical Constants
```
π = 3.14159265359
e = 2.71828182846
g = 9.80665 m/s² (standard gravity)
```

### Unit Conversions
```
1 kW = 1000 W
1 kWh = 3.6 MJ
1 bar = 100,000 Pa = 14.504 psi
1 mph = 1.60934 km/h
1 ft·lb = 1.35582 Nm
°F = (°C × 9/5) + 32
°K = °C + 273.15
```

### Common Vehicle Parameters
```
Wheel diameter (typical): 0.5 m
Gear ratio (typical): 10:1
Air density (sea level): 1.225 kg/m³
Rolling resistance coefficient: 0.01-0.02
Drag coefficient (typical car): 0.25-0.35
```

## References
- [Battery Management System Design](https://www.sciencedirect.com/topics/engineering/battery-management-system)
- [Electric Vehicle Powertrain Design](https://ieeexplore.ieee.org/document/6243883)
- [Thermal Management in EVs](https://www.mdpi.com/1996-1073/14/5/1364)
- [Control Systems Engineering](https://www.wiley.com/en-us/Control+Systems+Engineering%2C+8th+Edition-p-9781119474227)

---
*Last updated: October 2025*  
*Document version: 1.0*  
*Review cycle: Annually*