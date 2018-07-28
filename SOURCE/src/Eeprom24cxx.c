#include "Eeprom24cxx.h"

struct eeprom24cxxInitTypeDef eeprom24cxx;





void EepromInitt(void){
  GPIOB->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9);
  GPIOB->CRH |= (GPIO_CRH_CNF8 | GPIO_CRH_CNF9);
  
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
  
  
  
  
  
  
}
