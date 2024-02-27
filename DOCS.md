# PIDctrl-c v1.1 documentation

A proportional–integral–derivative controller (PID controller or three-term controller) is a control loop mechanism employing feedback that is widely used in industrial control systems and a variety of other applications requiring continuously modulated control. A PID controller continuously calculates an error value e ( t ) {\displaystyle e(t)} as the difference between a desired setpoint (SP) and a measured process variable (PV) and applies a correction based on proportional, integral, and derivative terms (denoted P, I, and D respectively), hence the name. [\(Wikipedia\)](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)

## struct PIDctrl_config
```c
struct PIDctrl_config {
  double kP;
  double kD;
  double kI;
  
  double timestep;
  
  double minOut;
  double maxOut;
}
```

`kP` – proportional coefficient  
`kD` – derivative coefficient  
`kI` – integral coefficient  
`timestep` – calculation period (must be greater than `0`)  
`minOut` – minimum possible output value of the controller (must be less than or equal to `maxOut`)  
`maxOut` – maximum possible output value of the controller (must be greater than or equal to `minOut`)

Configuration for PID controller. Stores controller coefficients and other settings. Can be used to create one or multiple controllers with same parameters.

**Example:** see [PIDctrl_Init()](#PIDctrl_Init)

## PIDctrl_t
`typedef struct { ... } PIDctrl_t;`  
The PID controller type. Used to create the controller and by library functions. Stores data about the controller.

**Example:** see [PIDctrl_Init()](#PIDctrl_Init)

**Note:** Don't forget to initialize the controller with `PIDctrl_Init()` before usage! Calling any function excluding `PIDctrl_Init()` before controller initialization or initialization the controller twice without deinitialization can cause undefined behavior on some systems.

**Important!** Don't modify or read any element of the controller structure manually! It can break the controller!

## PIDctrl_Init()
`int PIDctrl_Init(PIDctrl_t* controller, struct PIDctrl_config config)`  
`controller` – pointer to `PIDctrl_t`  
`config` – configuration structure  
Return value – Return value – `1` if initialization is successful, `0` otherwise

Initializes the PID controller.

**Example**:
```c
PIDctrl_t PID;  // create PID controller
struct PIDctrl_config PIDconfig = {kP: 0.05, kD: 0.3, kI: 0.0001, timestep: 1, minOut: -65535, maxOut: 65535};  // Configuration structure

while (!PIDctrl_Init(&PID, PIDconfig)) {}  // This code will try to initialize the controller until success
```

## PIDctrl_Deinit()
`int PIDctrl_Deinit(PIDctrl_t* controller)`  
`controller` – pointer to `PIDctrl_t`  
Return value – Return value – `1` if deinitialization is successful, `0` otherwise

Deinitialize the controller. Use this function if you don't need the controller anymore. If the controller is already deinitialized, returns `1`. After deinitialization, the controller can be initialized and used again.

**Example:**  
```c
PIDctrl_Deinit(&PID);  // Deinitialize the controller
```

## PIDctrl_Reset()
`int PIDctrl_Reset(PIDctrl_t* controller)`  
`config` – configuration structure  
Return value – `1` if successful, `0` otherwise

Resets controller parameters like previous error value and integrator. Does not reset controller config.

**Example:**  
```c
PIDctrl_Reset(&PID);  // Reset the controller
```

## PIDctrl_SetConfig()
`int PIDctrl_SetConfig(PIDctrl_t* controller, struct PIDctrl_config config)`  
`controller` – pointer to `PIDctrl_t`  
`config` – configuration structure  
Return value – `1` if successful, `0` otherwise

Changes configuration of the controller.

**Example:**  
```c
struct PIDctrl_config new_config = {kP: 0.09, kD: 0.2, kI: 0.0014, timestep: 1, minOut: -65535, maxOut: 65535};

while (!PIDctrl_SetConfig(&PID, new_config)) {}  // This code will try to change config of the controller until success
```

## PIDctrl_GetConfig()
`struct PIDctrl_config PIDctrl_GetConfig(PIDctrl_t* controller)`  
`controller` – pointer to `PIDctrl_t`  
Return value – current configuration of the controller

Returns current configuration of the given PID controller. Undefined behavior if the controller wasn't initialized.

## PIDctrl_Calculate()
`double PIDctrl_Calculate(PIDctrl_t* controller, double setpoint, double measurement)`  
`controller` – pointer to `PIDctrl_t`  
`setpoint` – setpoint  
`measurement` – current measurement  
Return value – controller calculation result. `0` in case of error.

Calculates controller and returns the result of the calculation. Returns `0` in case of error.

**Example:**  
```c
// Arduino example
double sensor_value = analogRead(3);  // Read sensor measurement
double PID_out = PIDctrl_Calculate(&PID, 32, sensor_value);  // calculate PID output value
analogWrite(9, PID_out);  // Write value to pin
// Don't forget to set minOut and maxOut to -255 and 255 for Arduino's analogWrite() function
```
