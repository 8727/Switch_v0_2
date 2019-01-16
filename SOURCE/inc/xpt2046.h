#ifndef _XPT2046_H
#define _XPT2046_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "gui.h"



/* Define --------------------------------------------------------------------*/
#define XPT2046_CS_LOW      GPIOB->BSRR = GPIO_BSRR_BR12
#define XPT2046_CS_HIGHT    GPIOB->BSRR = GPIO_BSRR_BS12

#define XPT2046_Y           0x90
#define XPT2046_X           0xD0
#define XPT2046_BAT         0xA4
#define XPT2046_BRG         0xE4

#define XPT2046_PRESSED     0x00
#define XPT2046_RELEASED    0x01

struct xpt2046InitTypeDef{
  uint8_t pressed;
  uint16_t x;
  uint16_t y;
  uint16_t bat;
  uint16_t brg;
};

extern struct xpt2046InitTypeDef xpt2046;

void Xpt2046Calibration(void);
void Xpt2046Init(void);

#endif /* _XPT2046_H */
