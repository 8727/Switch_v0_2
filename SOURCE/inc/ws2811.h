#ifndef _WS2811_H
#define _WS2811_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"

/* Define --------------------------------------------------------------------*/
#define LED_LOW          0x14
#define LED_HIGHT        0x30

extern uint8_t ws2811W[];
extern uint8_t ws2811RGB[][0x03];

void Ws2811WUpdate(void);
void Ws2811RGBUpdate(void);
void Ws2811Init(void);

#endif /* _WS2811_H */
