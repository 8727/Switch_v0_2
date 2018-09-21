#ifndef _LIGHTING_H
#define _LIGHTING_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "ws2811.h"


/* Define --------------------------------------------------------------------*/

struct RGBInitTypeDef{
  uint8_t effect;
  uint8_t wait;
  uint8_t pause;
  uint8_t set;
  
};
extern struct RGBInitTypeDef rgb;

void BrighetInit(void);

#endif /* _LIGHTING_H */
