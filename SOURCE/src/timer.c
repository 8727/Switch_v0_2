#include "timer.h"

TimerTypeDef unixTims;

TimersTypeDef timesJob[0x10];

void TimersInit(void){

}

void TimersMatch(void){
  UpdateTimer(&unixTims);
  for(uint8_t i = 0x00; i < 0x10; i++){
    if(timesJob[i].activity > 0x01){
      if(timesJob[i].type == 0x01){
      
      }else{
      
      }
    }
  }
}
