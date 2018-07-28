#ifndef GUI_LCD_H
#define GUI_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "ili9488.h"
#include "w25qxx.h"
#include "gui_font.h"
#include "stdlib.h"

/* Includes ------------------------------------------------------------------*/

#define CH_HEIGHT     0x10


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
