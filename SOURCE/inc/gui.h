#ifndef _GUI_H
#define _GUI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "calib.h"

/* Define --------------------------------------------------------------------*/

void GuiCalibDraw(uint16_t x, uint16_t y);
void GuiCalibErase(uint16_t x, uint16_t y);
void GuiSetWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void GuiFullWindow(uint16_t color);

#endif /* _GUI_H */
