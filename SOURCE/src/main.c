#include "main.h"

int main(void){
  Start();
  Setting();
  while(0x01){
  GuiSetWindow(xpt2046.x - 1, xpt2046.y -1, 3, 3);
  uint8_t i = 0x09;
  while(i--){
    LCD_DATA = WHITE;
  }
  DelayMs(250);
  
  }
}
