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
#define XPT2046_T0          0x0084
#define XPT2046_T1          0x00F4

#define TOUCH_PRESSED       0x00
#define TOUCH_RELEASED      0x01

struct xpt2046ReadInitTypeDef{
  uint8_t status;
  uint8_t count;
  uint16_t raw[0x09];
  uint8_t rawCount;
  
  
  uint8_t send[0x04];
  uint8_t read[0x03];
  uint8_t sendStep;
  uint8_t sendCoun;
  uint16_t readSend[0x04];
};

struct xpt2046InitTypeDef{
  uint16_t x;
  uint16_t y;
  uint16_t z1;
  uint16_t z2;
  uint16_t bat;
  uint16_t brg;
  uint16_t t0;
  uint16_t t1;
};

extern struct xpt2046InitTypeDef xpt2046;

void Xpt2046GetTouch(void);
void Xpt2046Init(void);

#endif /* _XPT2046_H */
