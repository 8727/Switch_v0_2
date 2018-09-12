#include "gui.h"

void GuiSetWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
  if(settings.maxX < width) width = settings.maxX;
  if(settings.maxY < height) height = settings.maxY;
  if(settings.maxX < (x + width)) x = settings.maxX - width;
  if(settings.maxY <(y + height)) y = settings.maxY - height;
  
  LCD_REG = 0x2A;
  LCD_DATA = x >> 0x08;
  LCD_DATA = x;
  LCD_DATA = (x + width - 0x01) >> 0x08;
  LCD_DATA = x + width - 0x01;
  LCD_REG = 0x2B;
  LCD_DATA = y >> 0x08;
  LCD_DATA = y;
  LCD_DATA = (y + height - 0x01) >> 0x08;
  LCD_DATA = y + height - 0x01;
  LCD_REG = 0x2C;
}

void GuiCalibDraw(uint16_t x, uint16_t y){
  uint32_t c;
  GuiSetWindow(x - 0x0F, y - 0x0F, 0x20, 0x20);
  for(uint8_t i = 0x00; i < 0x20; i++){
    c = calibDraw[i];
    for(uint8_t z = 0x00; z < 0x20; z++){
      if(c & 0x80000000){
        LCD_DATA = BLACK;
      }else{
        LCD_DATA = WHITE;
      }
      c <<= 0x01;
    }
  }
}

void GuiCalibErase(uint16_t x, uint16_t y){
  GuiSetWindow(x - 0x0F, y - 0x0F, 0x20, 0x20);
  uint16_t i = 0x0400;
  while(i--){
    LCD_DATA = WHITE;
  }
}

void GuiFullWindow(uint16_t color){
  uint32_t i = 0x025800;
  while(i--){
    LCD_DATA = color;
  }
}

