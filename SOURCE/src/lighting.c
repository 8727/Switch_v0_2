#include "lighting.h"

struct LightingInitTypeDef lighting;

void TIM7_IRQHandler(void){
  TIM7->SR &= ~TIM_SR_UIF;
  lighting.counter++;
  if(lighting.counter >= lighting.ch1){
    LED_CH1_LOW;
  }else{
    LED_CH1_HIGHT;
  }
  if(lighting.counter >= lighting.ch2){
    LED_CH2_LOW;
  }else{
    LED_CH2_HIGHT;
  }
}

void BrighetInit(void){
  GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
  GPIOA->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  TIM7->PSC = 0x00;
  TIM7->ARR = 0x0619; // 200Hz
  TIM7->SR = 0x00;
  TIM7->DIER |= TIM_DIER_UIE;
  TIM7->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  lighting.ch1 = 0x40;
  lighting.ch2 = 0xC0;
  
  NVIC_SetPriority(TIM7_IRQn, PRIORITY_BRIGHET);
  NVIC_EnableIRQ(TIM7_IRQn);
 }
