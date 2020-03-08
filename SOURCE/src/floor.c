#include "floor.h"

uint8_t flor;
uint8_t florJob;
uint8_t florTemperature;

void Adjustment(void){
  if(0x00 != flor){
    if( florJob){
      
    }
    
  }else{
    
  }
  if(0x80 & florJob){
    if(florTemperature > (000 + 111)){
      if(0x00 == florJob && florJob & 0x0F) SIMISTOR_POWER_ON;
      if(0x01 == florJob && florJob & 0x0F) RELAY_POWER_ON;
      if(0x02 == florJob && florJob & 0x0F) SIMISTOR_POWER_OFF;
      if(0x02 > (florJob & 0x0F)) florJob++;
    }else{
      if(0x02 == florJob && florJob & 0x0F) SIMISTOR_POWER_ON;
      if(0x01 == florJob && florJob & 0x0F) RELAY_POWER_OFF;
      if(0x00 == florJob && florJob & 0x0F) SIMISTOR_POWER_OFF;
      if(0x00 != (florJob & 0x0F)) florJob--;
    }
  }
}
