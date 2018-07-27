#ifndef _SETTING_H
#define _SETTING_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "rtc.h"
#include "ili9488.h"
#include "xpt2046.h"

#include "w25qxx.h"


/* Define --------------------------------------------------------------------*/

uint32_t GetTick(void);
void DelayMs(uint32_t ms);

void Setting(void);

#endif /* _SETTING_H */
