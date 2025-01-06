# PIDctrl-c v1.2 documentation

A proportional–integral–derivative controller (PID controller or three-term controller) is a control loop mechanism employing feedback that is widely used in industrial control systems and a variety of other applications requiring continuously modulated control. A PID controller continuously calculates an error value e ( t ) {\displaystyle e(t)} as the difference between a desired setpoint (SP) and a measured process variable (PV) and applies a correction based on proportional, integral, and derivative terms (denoted P, I, and D respectively), hence the name. [\(Wikipedia\)](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller)

## Index

* [Compiling](#compiling)
* [Adding to your project](#adding-to-your-project)
* [Proper usage](#proper-usage)
* [Basic usage example](#basic-usage-example)
* [Library interface documentation](#library-interface-documentation)

## Compiling

Requires C99 or later, GCC/G++ compiler is recommended but should work with most other compilers.

## Adding to your project

To add this library to your project, just include the `src/PIDctrl.h` file and add `src/PIDctrl.c` to sources.

**Example:**  
`main.c`:  
```c
#include "PIDctrl.h"

int main() {
    /* Your code here */
}
```
Compile command: `gcc main.c PIDctrl.c -o main.exe`

## Proper usage

When declaring `PIDctrl_t` variable, initialize it with `{0}`: `PIDctrl_t PID = {0};` (`{}` can be used instead of `{0}` since C23).  
When allocating it on the heap, fill it with zeros using `memset`: `memset(PID_ptr, 0, sizeof(PIDctrl_t))` (`PID_ptr` is pointer to the `PIDctrl_t` allocated on the heap)  
If it is not initialized with zero value, it **may result in undefined behavior!**

## Basic usage example

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

## Library interface documentation

### struct PIDctrl_config
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
`timestep` – calculation period (also known as `dt`), must be greater than `0`  
`minOut` – minimum possible output value of the controller (must be less than or equal to `maxOut`)  
`maxOut` – maximum possible output value of the controller (must be greater than or equal to `minOut`)

Configuration for a PID controller. Stores controller coefficients and other settings. Can be used to create one or multiple controllers with same parameters.

**Example:** see [PIDctrl_Init()](#PIDctrl_Init)

### PIDctrl_t
`typedef struct { ... } PIDctrl_t;`  
The PID controller typedef. Used to create the controller and passed as the first argument to library functions. Stores data about the controller.

**Example:** see [PIDctrl_Init()](#PIDctrl_Init)

**Note:** Don't forget to initialize the controller with `PIDctrl_Init()` before usage! Calling any function excluding `PIDctrl_Init()` before controller initialization or initialization the controller twice without deinitialization can cause undefined behavior on some systems.

**Important!** Follow the rules specified in [Proper usage](#proper-usage) to avoid problems and undefined behavior cases when using the controller.

### PIDctrl_Init()
`short PIDctrl_Init(PIDctrl_t *ctx, const struct PIDctrl_config config)`  
`ctx` – pointer to `PIDctrl_t`  
`config` – configuration structure  
Return value – Return value – `1` if initialization is successful, `0` otherwise

Initializes the PID controller. Returns `0` if the controller is already initialized or the configuration is invalid (see [struct PIDctrl_config](struct-pidctrl_config)).

**Example**:
```c
PIDctrl_t PID = {0};  // create PID controller and initialize it with zero
struct PIDctrl_config PIDconfig = {kP: 0.05, kD: 0.3, kI: 0.0001, timestep: 1, minOut: -65535, maxOut: 65535};  // Configuration structure

while (!PIDctrl_Init(&PID, PIDconfig)) {}  // This code will try to initialize the controller until success. Make sure that this controller was not already initialized, otherwise the loop will be infinite.
```

### PIDctrl_Deinit()
`short PIDctrl_Deinit(PIDctrl_t *ctx)`  
`ctx` – pointer to `PIDctrl_t`  
Return value – Always returns `1`

Deinitialize the controller. Use this function if you don't need the controller anymore. If the controller is already deinitialized, returns `1`. After deinitialization, the controller can be initialized and used again.

**Example:**  
```c
PIDctrl_Deinit(&PID);  // Deinitialize the controller
```

### PIDctrl_IsInit()
`bool PIDctrl_IsInit(PIDctrl_t *ctx)`  
`ctx` – pointer to `PIDctrl_t`  
Return value – `true` if the controller is initialized, `false` if the controller is not initialized

Checks if the controller is initialized.

**Example:**  
```c
if (!PIDctrl_IsInit(&PID)) {  // If the controller is not initialized
    PIDctrl_Init(&PID, PIDconfig);  // Initialize it
}
```

### PIDctrl_Reset()
`short PIDctrl_Reset(PIDctrl_t *ctx)`  
`ctx` – pointer to `PIDctrl_t`  
Return value – `1` if successful, `0` otherwise

Resets controller's previous error value and integrator. Does not reset controller configuration.

**Example:**  
```c
PIDctrl_Reset(&PID);  // Reset the controller
```

### PIDctrl_SetConfig()
`short PIDctrl_SetConfig(PIDctrl_t *ctx, const struct PIDctrl_config config)`  
`ctx` – pointer to `PIDctrl_t`  
`config` – configuration structure  
Return value – `1` if successful, `0` otherwise

Changes the configuration of the controller. Returns `0` if the configuration is invalid (see [struct PIDctrl_config](struct-pidctrl_config)).

**Example:**  
```c
struct PIDctrl_config new_config = {kP: 0.09, kD: 0.2, kI: 0.0014, timestep: 1, minOut: -65535, maxOut: 65535};

PIDctrl_SetConfig(&PID, new_config)  // Update controller configuration
```

### PIDctrl_GetConfig()
`struct PIDctrl_config PIDctrl_GetConfig(PIDctrl_t *ctx)`  
`ctx` – pointer to `PIDctrl_t`  
Return value – current configuration of the controller

Returns current configuration of the given PID controller. All elements of the returned structure woll be zero if the controller is not initialized.

### PIDctrl_Calculate()
`double PIDctrl_Calculate(PIDctrl_t *ctx, double setpoint, double measurement)`  
`ctx` – pointer to `PIDctrl_t`  
`setpoint` – setpoint  
`measurement` – current measurement  
Return value – controller calculation result. `0` in case of an error

PID controller step function. Calculates control value and returns the result of the calculation. Returns `0` in case of an error.

**Example:**  
```c
// Arduino example
double sensor_value = analogRead(3);  // Read sensor measurement
double PID_out = PIDctrl_Calculate(&PID, 32, sensor_value);  // calculate PID output value
analogWrite(9, PID_out);  // Write value to pin
// Don't forget to set minOut and maxOut to -255 and 255 for Arduino's analogWrite() function
```

### PIDctrl_Calculate_RO()
`double PIDctrl_Calculate_RO(PIDctrl_t *ctx, double setpoint, double measurement)`  
`ctx` – pointer to `PIDctrl_t`  
`setpoint` – setpoint  
`measurement` – current measurement  
Return value – controller calculation result. `0` in case of an error

Calculates control value but doesn't modify controller's variables (integrator and previous error).
