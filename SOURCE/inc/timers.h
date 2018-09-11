#ifndef _TIMERS_H
#define _TIMERS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"


/* Define --------------------------------------------------------------------*/

struct TimersTypeDef{
  uint8_t jobActivity;
  uint8_t typeChannel;
  uint8_t value;
  uint8_t wday;      /* 0..6 (Sun..Sat) */
  uint8_t hourOn;    /* 0..23 */
  uint8_t minOn;     /* 0..59 */
  uint8_t hourOff;   /* 0..23 */
  uint8_t minOff;    /* 0..59 */
};

extern struct TimersTypeDef timesJob[0x10];

void TimersMatch(void);

#endif /* _TIMERS_H */
