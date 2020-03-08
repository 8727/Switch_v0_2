#include "timers.h"

TimerTypeDef unixTims;

struct TimersTypeDef timesJob[0x20];

void TimersMatch(void){
  UpdateTimer(&unixTims);
  for(uint8_t i = 0x00; i < 0x020; i++){
    if(timesJob[i].settings & TIMER_ON_OFF){
      if(unixTims.wday == timesJob[i].wday && unixTims.hour == timesJob[i].hour && unixTims.min == timesJob[i].min && unixTims.sec == 0x00){
        if(timesJob[i].settings & TIMER_LIGHT_FLOOR){
          if(timesJob[i].settings & TIMER_SWITCH){
            StartLighting(timesJob[i].value);
          }else{
            StopLighting();
          }
        }else{
          if(timesJob[i].settings & TIMER_SWITCH){
            flor = 0x80;
            florTemperature = timesJob[i].value;
          }else{
            flor = 0x00;
//            uint8_t florTemperature = timesJob[i].value;
          }
        }
      }
    }
  }
}
