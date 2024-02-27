/*
  PIDctrl-c
  v1.1
  https://github.com/t1m013y/PIDctrl-c
  By Timofey Fomin (https://github.com/t1m013y, t1m013y@gmail.com)
*/

#ifndef INCLUDED_t1m013y_PIDctrl_h
#define INCLUDED_t1m013y_PIDctrl_h

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

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

int PIDctrl_Init(PIDctrl_t* controller, struct PIDctrl_config config);
int PIDctrl_Deinit(PIDctrl_t* controller);

int PIDctrl_Reset(PIDctrl_t* controller);

int PIDctrl_SetConfig(PIDctrl_t* controller, struct PIDctrl_config config);
struct PIDctrl_config PIDctrl_GetConfig(PIDctrl_t* controller);

double PIDctrl_Calculate(PIDctrl_t* controller, double setpoint, double measurement);


#ifdef __cplusplus
}
#endif

#endif // INCLUDED_t1m013y_PIDctrl_h