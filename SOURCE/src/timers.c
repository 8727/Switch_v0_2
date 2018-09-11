#include "timers.h"

TimerTypeDef unixTims;

struct TimersTypeDef timesJob[0x10];

void TimersLighting(uint8_t number){
  
}

void TimersWarmFloor(uint8_t number){
  
}

void TimersMatch(void){
  UpdateTimer(&unixTims);
  for(uint8_t i = 0x00; i < 0x10; i++){
    if(timesJob[i].jobActivity & 0x01){
      if(unixTims.wday == timesJob[i].wday && unixTims.hour == timesJob[i].hourOn && unixTims.min == timesJob[i].minOn){
        timesJob[i].jobActivity |= 0x10;
      }
      if(unixTims.wday == timesJob[i].wday && unixTims.hour == timesJob[i].hourOff && unixTims.min == timesJob[i].minOff){
        timesJob[i].jobActivity &= 0x0F;
      }
    }
  }
}
