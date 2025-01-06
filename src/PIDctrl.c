/*
  PIDctrl-c
  v1.2
  https://github.com/t1m013y/PIDctrl-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
  This code is licensed under MIT license
*/

#include "PIDctrl.h"

short PIDctrl_Init(PIDctrl_t *ctx, const struct PIDctrl_config config)
{
  if (!PIDctrl__CheckConfig(&config))
    return 0;
  
  if (ctx->_wInit)
    return 0;
  
  ctx->config = config;
  
  ctx->prevErr = 0;
  ctx->integrator = 0;
  
  ctx->_wInit = true;
  return 1;
}

short PIDctrl_Deinit(PIDctrl_t *ctx)
{
  if (!ctx->_wInit)
    return 1;
  
  ctx->_wInit = false;
  
  return 1;
}

bool PIDctrl_IsInit(PIDctrl_t *ctx)
{
  return ctx->_wInit;
}

short PIDctrl_Reset(PIDctrl_t *ctx)
{
  if (!ctx->_wInit)
    return 0;
  
  ctx->prevErr = 0;
  ctx->integrator = 0;
  
  return 1;
}

short PIDctrl_SetConfig(PIDctrl_t *ctx, const struct PIDctrl_config config)
{
  if (!PIDctrl__CheckConfig(&config))
    return 0;
  
  if (!ctx->_wInit)
    return 0;
  
  ctx->config = config;
  
  return 1;
}

struct PIDctrl_config PIDctrl_GetConfig(PIDctrl_t *ctx)
{
  if (!ctx->_wInit) {
    const struct PIDctrl_config c = {0};
    return c;
  }
  
  return ctx->config;
}

double PIDctrl_Calculate(PIDctrl_t *ctx, double setpoint, double measurement)
{
  if (!ctx->_wInit)
    return 0;
  
  double err = setpoint - measurement;
  
  ctx->integrator += err * ctx->config.timestep * ctx->config.kI;
  if (ctx->integrator > ctx->config.maxOut)
    ctx->integrator = ctx->config.maxOut;
  if (ctx->integrator < ctx->config.minOut)
    ctx->integrator = ctx->config.minOut;
  
  double P = err * ctx->config.kP;
  double D = (err - ctx->prevErr) * ctx->config.kD / ctx->config.timestep;
  double I = ctx->integrator;
  
  double out = P + D + I;
  
  ctx->prevErr = err;
  
  if (out > ctx->config.maxOut)
    out = ctx->config.maxOut;
  if (out < ctx->config.minOut)
    out = ctx->config.minOut;
  
  return out;
}

double PIDctrl_Calculate_RO(PIDctrl_t *ctx, double setpoint, double measurement)
{
  if (!ctx->_wInit)
    return 0;
  
  double err = setpoint - measurement;
  
  double P = err * ctx->config.kP;
  double D = (err - ctx->prevErr) * ctx->config.kD / ctx->config.timestep;
  double I = ctx->integrator;
  
  double out = P + D + I;
  
  if (out > ctx->config.maxOut)
    out = ctx->config.maxOut;
  if (out < ctx->config.minOut)
    out = ctx->config.minOut;
  
  return out;
}

bool PIDctrl__CheckConfig(const struct PIDctrl_config *config)  // Auxiliary function, not for usage outside library's code
{
  if (config->timestep <= 0 || config->minOut > config->maxOut)
    return false;
  
  return true;
}
