#ifndef _WS2811_H
#define _WS2811_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Define --------------------------------------------------------------------*/
#define LEDS_RGB    50
#define LEDS_W      0x0A
#define COLRS       0x03
#define SEGMENTS    0x03

#define LED_LOW     0x03
#define LED_HIGHT   0x03

//struct Ws2811InitTypeDef{
//  uint16_t dataBuff[0x2A];
//  uint16_t humidity;
//  uint16_t temperature;
//};

//extern struct Ws2811InitTypeDef ws2811RGB[LEDS_RGB + 0x02][COLRS][0x08];
//extern struct Ws2811InitTypeDef ws2811W[LEDS_W + 0x02][SEGMENTS][0x08];
#endif /* _WS2811_H */
