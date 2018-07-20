#ifndef _RTC_H
#define _RTC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Includes ------------------------------------------------------------------*/

void rtcInit(void);
uint32_t rtcGetCounter(void);
void rtcSetCounter(uint32_t count);

#endif /* _RTC_H */
