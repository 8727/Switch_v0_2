#ifndef _RTC_H
#define _RTC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Define --------------------------------------------------------------------*/

void RtcInit(void);
uint32_t RtcGetCounter(void);
void RtcSetCounter(uint32_t count);

#endif /* _RTC_H */
