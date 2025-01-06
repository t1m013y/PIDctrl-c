/*
  PIDctrl-c
  v1.2
  https://github.com/t1m013y/PIDctrl-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
  This code is licensed under MIT license
*/

#ifndef INCLUDED_t1m013y_PIDctrl_h
#define INCLUDED_t1m013y_PIDctrl_h

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __cplusplus
#include <stdbool.h>  // Include stdbool.h if not c++
#endif

struct PIDctrl_config {
  double kP;
  double kD;
  double kI;
  
  double timestep;
  
  double minOut;
  double maxOut;
};

typedef struct {
  bool _wInit;
  
  struct PIDctrl_config config;
  
  double prevErr;
  double integrator;
} PIDctrl_t;

short PIDctrl_Init(PIDctrl_t *ctx, const struct PIDctrl_config config);
short PIDctrl_Deinit(PIDctrl_t *ctx);

bool PIDctrl_IsInit(PIDctrl_t *ctx);

short PIDctrl_Reset(PIDctrl_t *ctx);

short PIDctrl_SetConfig(PIDctrl_t *ctx, const struct PIDctrl_config config);
struct PIDctrl_config PIDctrl_GetConfig(PIDctrl_t *ctx);

double PIDctrl_Calculate(PIDctrl_t *ctx, double setpoint, double measurement);
double PIDctrl_Calculate_RO(PIDctrl_t *ctx, double setpoint, double measurement);

bool PIDctrl__CheckConfig(const struct PIDctrl_config *config);  // Auxiliary function, not for usage outside library's code


#ifdef __cplusplus
}
#endif

#endif // INCLUDED_t1m013y_PIDctrl_h