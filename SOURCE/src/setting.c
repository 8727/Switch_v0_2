#include "setting.h"

static __IO uint32_t msTicks;

void SysTick_Handler(void){
  msTicks++;
}

uint32_t GetTick(void){
  return msTicks;
}

void DelayMs(uint32_t ms){
  uint32_t tickstart = GetTick();
  while((GetTick() - tickstart) < ms){}
}

void Setting(void){
  SysTick_Config(SystemCoreClock / 1000);   //1ms
  
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  
  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE + AFIO_MAPR_TIM2_REMAP_FULLREMAP + AFIO_MAPR_I2C1_REMAP;
  
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
  
  RtcInit();
  BrighetLcdInit();
  EepromInitt();
  LcdInitt();
  TIM2->CCR1 = 0x10;
  Xpt2046Init();
  
  W25QxxInit();
  W25QxxReadImgTable();
  
  
  
  

}
