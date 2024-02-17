/*
  PIDctrl-c
  v1.0
  https://github.com/t1m013y/PIDctrl-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
*/

#include "PIDctrl.h"

int PIDctrl_Init(PIDctrl_t* controller, struct PIDctrl_config config) {
  if (!(config.timestep > 0 && config.minOut <= config.maxOut))
    return 0;
  
  controller->config = config;
  
  controller->prevErr = 0;
  controller->integrator = 0;
  
  controller->_wInit = true;
  return 1;
}

int PIDctrl_Deinit(PIDctrl_t* controller) {
  if (!controller->_wInit)
    return 1;
  
  controller->_wInit = false;
  
  return 1;
}

int PIDctrl_Reset(PIDctrl_t* controller) {
  if (!controller->_wInit)
    return 0;
  
  controller->prevErr = 0;
  controller->integrator = 0;
  
  return 1;
}

int PIDctrl_SetConfig(PIDctrl_t* controller, struct PIDctrl_config config) {
  if (!controller->_wInit)
    return 0;
  
  controller->config = config;
  
  return 1;
}

struct PIDctrl_config PIDctrl_GetConfig(PIDctrl_t* controller) {
  if (!controller->_wInit)
    return (struct PIDctrl_config){0};
  
  return controller->config;
}

double PIDctrl_Calculate(PIDctrl_t* controller, double setpoint, double measurement) {
  if (!controller->_wInit)
    return 0;
  
  double err = setpoint - measurement;
  
  controller->integrator += err * controller->config.timestep;
  if (controller->integrator > controller->config.maxOut)
    controller->integrator = controller->config.maxOut;
  if (controller->integrator < controller->config.minOut)
    controller->integrator = controller->config.minOut;
  
  double P = err * controller->config.kP;
  double D = (err - controller->prevErr) * controller->config.kD / controller->config.timestep;
  double I = controller->integrator * controller->config.kI;
  
  double out = P + D + I;
  
  controller->prevErr = err;
  
  if (out > controller->config.maxOut)
    out = controller->config.maxOut;
  if (out < controller->config.minOut)
    out = controller->config.minOut;
  
  return out;
}
