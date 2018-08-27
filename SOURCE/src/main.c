#include "main.h"

//void Gui_time (void){
//  char spr_buf [0x20];
//  sprintf(spr_buf,"Date:%0.2d.%0.2d.%0.4d\r\nTime:%0.2d:%0.2d:%0.2d", 
//          rtc.day, rtc.month, rtc.year, rtc.hour, rtc.min, rtc.sec);
//  Gui_Print_Chars(0x0000, 0x0000, WHITE, BLACK, 0x44, (uint8_t*)spr_buf);
//  
//}

int main(void){
  Start();
  Setting();
  while(1){
//    RtcCounterToTime(RtcGetCounter()+10800);
//    Gui_time();
//    DelayMs(500);
  }
}
