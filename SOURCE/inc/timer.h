#ifndef _TIMER_H
#define _TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
//#include "rtc.h"


/* Define --------------------------------------------------------------------*/

typedef struct{
  uint8_t activity;
  uint8_t type;
  uint8_t value;
  uint8_t wday;      /* 0..6 (Sun..Sat) */
  uint8_t hourOn;    /* 0..23 */
  uint8_t minOn;     /* 0..59 */
  uint8_t hourOff;   /* 0..23 */
  uint8_t minOff;    /* 0..59 */
}TimersTypeDef;

void TimersInit(void);
void TimersMatch(void);

#endif /* _TIMER_H */
