# PIDctrl-c

![license](https://img.shields.io/github/license/t1m013y/PIDctrl-c)

PID (Proportional-integral-derivative) controller for C and C++ languages

## About
A proportional–integral–derivative controller (PID controller or three-term controller) is a control loop mechanism employing feedback that is widely used in industrial control systems and a variety of other applications requiring continuously modulated control. A PID controller continuously calculates an error value e ( t ) {\displaystyle e(t)} as the difference between a desired setpoint (SP) and a measured process variable (PV) and applies a correction based on proportional, integral, and derivative terms (denoted P, I, and D respectively), hence the name. [\(Wikipedia\)](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)

A block diagram of a PID controller:  
<img src="https://upload.wikimedia.org/wikipedia/commons/4/43/PID_en.svg" alt="Ring buffer demonstration gif" width=400></img>  
[\(Wikipedia\)](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)

## Documentation
[English \[English\]](DOCS.md)

## Usage
First, create and initialize the controller.
```c
PIDctrl_t PID = {0};  // create PID controller and initialize it with zero
struct PIDctrl_config PIDconfig = {kP: 0.05, kD: 0.3, kI: 0.0001, timestep: 1, minOut: -65535, maxOut: 65535};  // Configuration structure

while (!PIDctrl_Init(&PID, PIDconfig)) {}  // This line will try to initialize the controller until success
```

Use `PIDctrl_Calculate()` to calculate controller value.
```c
// Arduino example
double sensor_value = analogRead(3);  // Read sensor measurement
double PID_out = PIDctrl_Calculate(&PID, 32, sensor_value);  // calculate PID output value; 32 is setpoint
analogWrite(9, PID_out);  // Write value to pin
// Don't forget to set minOut and maxOut to -255 and 255 for Arduino's analogWrite() function
```
