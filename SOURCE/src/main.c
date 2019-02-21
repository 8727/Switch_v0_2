#include "main.h"

int main(void){
  Start();
  Setting();
  while(0x01){
    brg.effect = 0x01;
    DelayMs(1000);
    GuiLoadImg(0x00, 0x00, 0x01);
    DelayMs(1000);
    GuiLoadImg(0x00, 0x00, 0x02);
  }
}
