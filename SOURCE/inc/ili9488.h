#ifndef _ILI9488_H
#define _ILI9488_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"

/* Define --------------------------------------------------------------------*/
#define ILI9488_RESET_LOW         GPIOD->BRR = GPIO_BRR_BR3
#define ILI9488_RESET_HIGHT       GPIOD->BSRR = GPIO_BSRR_BS3

#define LCD_DATA                  *(uint16_t *) (0x60020000)
#define LCD_REG                   *(uint16_t *) (0x60000000)

void LcdInitt(void);

#endif /* _ILI9488_H */
