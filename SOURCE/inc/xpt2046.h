#ifndef _XPT2046_H
#define _XPT2046_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"


/* Define --------------------------------------------------------------------*/
#define XPT2046_CS_LOW      GPIOB->BRR = GPIO_BRR_BR12
#define XPT2046_CS_HIGHT    GPIOB->BSRR = GPIO_BSRR_BS12

#define XPT2046_A           0x90
#define XPT2046_B           0xD0
#define XPT2046_Z1          0xB0
#define XPT2046_Z2          0xC0
#define XPT2046_BAT         0xA0
#define XPT2046_BRG         0xE0

struct xpt2046InitTypeDef{
  uint16_t x;
  uint16_t y;
  uint16_t bat;
  uint16_t brg;
  uint8_t start;
};
extern struct xpt2046InitTypeDef xpt2046;

void Xpt2046TouchBatBrg(void);
void Xpt2046Init(void);

#endif /* _XPT2046_H */
