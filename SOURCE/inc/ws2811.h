#ifndef _WS2811_H
#define _WS2811_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"

/* Define --------------------------------------------------------------------*/
#define LED_LOW          0x20
#define LED_HIGHT        0x40

extern uint8_t ws2811W[];
extern uint8_t ws2811BRG[];

void Ws2811WUpdate(void);
void Ws2811BRGUpdate(void);
void Ws2811Init(void);

#endif /* _WS2811_H */
