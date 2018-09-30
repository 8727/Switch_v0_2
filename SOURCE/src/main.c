#include "main.h"

int main(void){
  Start();
  Setting();
  while(0x01){
//    DelayMs(1000);
//    GuiLoadImg(0x00, 0x00, 0x00);
    DelayMs(1000);
    GuiLoadImg(0x00, 0x00, 0x01);
    DelayMs(1000);
    GuiLoadImg(0x00, 0x00, 0x02);
//    DelayMs(100);
//    W25QxxEraseBlocks();
//    DelayMs(100000);
  }
}
