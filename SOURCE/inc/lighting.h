#ifndef _LIGHTING_H
#define _LIGHTING_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "ws2811.h"


/* Define --------------------------------------------------------------------*/
#define W_POWER_ON        GPIOA->BSRR = GPIO_BSRR_BR0
#define W_POWER_OFF       GPIOA->BSRR = GPIO_BSRR_BS0

#define BRG_POWER_ON      GPIOA->BSRR = GPIO_BSRR_BR1
#define BRG_POWER_OFF     GPIOA->BSRR = GPIO_BSRR_BS1

struct BRGInitTypeDef{
  uint8_t effect;
  uint8_t wait;
  uint8_t pause;
  uint8_t set;
};
extern struct BRGInitTypeDef brg;

void UpdateBrightnessW(void);

#endif /* _LIGHTING_H */
