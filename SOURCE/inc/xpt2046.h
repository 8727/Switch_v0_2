#ifndef _XPT2046_H
#define _XPT2046_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"


/* Define --------------------------------------------------------------------*/
#define XPT2046_CS_LOW      GPIOB->BRR = GPIO_BRR_BR12
#define XPT2046_CS_HIGHT    GPIOB->BSRR = GPIO_BSRR_BS12

#define XPT2046_A           0x0090
#define XPT2046_B           0x00D0
#define XPT2046_Z1          0x00B0
#define XPT2046_Z2          0x00C0
#define XPT2046_BAT         0x00A4
#define XPT2046_BRG         0x00E4

#define XPT2046_PRESSED     0x0000
#define XPT2046_RELEASED    0x0001

#define TOUCH_FACTOR        0x2710

struct xpt2046ReadInitTypeDef{
  uint8_t status;
  uint8_t count;
  uint16_t raw[0x07];
  uint8_t rawCount;
};

struct xpt2046InitTypeDef{
  uint16_t x;
  uint16_t y;
  uint16_t z1;
  uint16_t z2;
  uint16_t bat;
  uint16_t brg;
};

extern struct xpt2046InitTypeDef xpt2046;

void Xpt2046Calibration(void);
void Xpt2046Init(void);

#endif /* _XPT2046_H */
