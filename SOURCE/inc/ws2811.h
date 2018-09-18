#ifndef _WS2811_H
#define _WS2811_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Define --------------------------------------------------------------------*/
#define LEDS_RGB         0x32
#define LEDS_W           0x03
#define LEDS_SEGMENTS    0x03

#define LED_LOW          0x20
#define LED_HIGHT        0x40

void LEDstrip_init(void);

#endif /* _WS2811_H */
