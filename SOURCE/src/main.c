#include "main.h"

int main(void){
  Start();
  Setting();
  while(0x01){
    brg.effect = 0x01;
//    DelayMs(1000);
//    GuiLoadImg(0x00, 0x00, 0x00);
    DelayMs(5000);
    GuiLoadImg(0x00, 0x00, 0x01);
    DelayMs(5000);
    GuiLoadImg(0x00, 0x00, 0x02);
    DelayMs(62);
    GuiBr();
    
//    DelayMs(1000);
//    W25QxxEraseBlocks();
    DelayMs(10000);
  }
}
