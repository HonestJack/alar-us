#ifndef TIMER_H
#define TIMER_H

#include "utils.h"

#define START_HOUR    18
#define START_MINUTE  59
#define START_SECOND  00
#define START_TIME    (START_HOUR*(long)3600 + START_MINUTE*(long)60 + START_SECOND)

#define HORAS_EM_UM_DIA (long)24*60*60

class Timer
{
public:
  Timer();
  ~Timer();

  volatile long getTime();
  void setTime(long newTime);
  void controla_pisca_led(bool sensor_disparado);
  volatile static long counter;
  volatile static short aux_counter;
  volatile bool static pisca_led;
};

#endif