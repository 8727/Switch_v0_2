#ifndef _TIMERS_H
#define _TIMERS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "floor.h"


/* Define --------------------------------------------------------------------*/

struct TimersTypeDef{
  uint8_t settings;
  uint8_t value;
  uint8_t wday;      /* 0..6 (Sun..Sat) */
  uint8_t hour;    /* 0..23 */
  uint8_t min;     /* 0..59 */
};

extern struct TimersTypeDef timesJob[0x20];

void TimersMatch(void);

#endif /* _TIMERS_H */
