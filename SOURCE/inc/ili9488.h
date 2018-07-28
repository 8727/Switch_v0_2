#ifndef _ILI9488_H
#define _ILI9488_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Define --------------------------------------------------------------------*/
#define ILI9488_RESET_LOW         GPIOD->BRR = GPIO_BRR_BR3
#define ILI9488_RESET_HIGHT       GPIOD->BSRR = GPIO_BSRR_BS3

#define LCD_DATA                  *(uint16_t *) (0x60020000)
#define LCD_REG                   *(uint16_t *) (0x60000000)

#define BLACK         0x0000      /*   0,   0,   0 */
#define NAVY          0x000F      /*   0,   0, 128 */
#define DARKGREEN     0x03E0      /*   0, 128,   0 */
#define DARKCYAN      0x03EF      /*   0, 128, 128 */
#define MAROON        0x7800      /* 128,   0,   0 */
#define PURPLE        0x780F      /* 128,   0, 128 */
#define OLIVE         0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY     0xC618      /* 192, 192, 192 */
#define DARKGREY      0x7BEF      /* 128, 128, 128 */
#define BLUE          0x001F      /*   0,   0, 255 */
#define GREEN         0x07E0      /*   0, 255,   0 */
#define CYAN          0x07FF      /*   0, 255, 255 */
#define RED           0xF800      /* 255,   0,   0 */
#define MAGENTA       0xF81F      /* 255,   0, 255 */
#define YELLOW        0xFFE0      /* 255, 255,   0 */
#define WHITE         0xFFFF      /* 255, 255, 255 */
#define ORANGE        0xFD20      /* 255, 165,   0 */
#define GREENYELLOW   0xAFE5      /* 173, 255,  47 */

void LcdSetWindows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
void LcdInitt(void);
void BrighetLcdInit(void);

#endif /* _ILI9488_H */
