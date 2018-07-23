#include "setting.h"

void SysTick_Handler(void){
  
  
}

void Setting(void){
  
  SysTick_Config(SystemCoreClock / 1000); //1ms
  
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  
  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE + AFIO_MAPR_TIM2_REMAP_FULLREMAP;
  
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
  
  RtcInit();
  BrighetLcdInit();
  LcdInitt();
  Xpt2046Init();
  
  W25QxxInit();
  
  
  
  

}
