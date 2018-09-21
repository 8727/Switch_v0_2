#ifndef _LIGHTING_H
#define _LIGHTING_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "ws2811.h"


/* Define --------------------------------------------------------------------*/
#define LED_CH1_LOW       GPIOA->BRR = GPIO_BRR_BR0
#define LED_CH2_LOW       GPIOA->BRR = GPIO_BRR_BR1
#define LED_CH1_HIGHT     GPIOA->BSRR = GPIO_BSRR_BS0
#define LED_CH2_HIGHT     GPIOA->BSRR = GPIO_BSRR_BS1

struct LightingInitTypeDef{
  uint8_t counter;
  uint8_t countCH1;
  uint8_t countCH2;
  uint8_t mode;
  
};

extern struct LightingInitTypeDef lighting;

void BrighetInit(void);

#endif /* _LIGHTING_H */
