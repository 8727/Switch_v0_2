#ifndef GUI_LCD_H
#define GUI_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "w25qxx.h"
#include "gui_font.h"
#include "gui_font_0_9.h"
#include "stdlib.h"

/* Includes ------------------------------------------------------------------*/

#define CH_HEIGHT     0x10

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


void GuiPrintDigits(uint16_t x, uint16_t y, uint16_t ink, uint16_t paper, uint8_t str);
void LcdSetWindows(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void Gui_Draw_Pixel(uint16_t color, uint16_t size);
void Gui_Draw_Line (uint16_t x, uint16_t y, uint16_t x_end, uint16_t y_end, uint8_t size, uint16_t color);
void Gui_Draw_HLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color);
void Gui_Draw_VLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color);
void Gui_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint8_t size, uint16_t color);
void Gui_Draw_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color);
void Gui_Draw_Triangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint16_t color);
void Gui_Draw_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint16_t color);
void Gui_Draw_Circle_Helper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint16_t color);
void Gui_Draw_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint16_t color);
void Gui_Draw_Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void Gui_Draw_Fill_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint16_t color);
void Gui_Load_Color_Img(uint16_t x, uint16_t y, uint8_t img);
void Gui_Load_Mono_Img(uint16_t x, uint16_t y, uint8_t img, uint16_t ink, uint16_t paper);
void Gui_Print_Char(uint16_t x, uint16_t y, uint16_t ink, uint16_t paper, uint8_t zoom, uint8_t str);
void Gui_Print_Chars(uint16_t x, uint16_t y, uint16_t ink, uint16_t paper, uint8_t zoom, const uint8_t str[]);


#endif /* GUI_LCD_H */
