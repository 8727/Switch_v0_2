#include "lighting.h"

struct LightingInitTypeDef lighting;

void TIM7_IRQHandler(void){
  TIM7->SR &= ~TIM_SR_UIF;
  lighting.counter++;
  if(lighting.counter >= lighting.countCH1){
    LED_CH1_LOW;
  }else{
    LED_CH1_HIGHT;
  }
  if(lighting.counter >= lighting.countCH2){
    LED_CH2_LOW;
  }else{
    LED_CH2_HIGHT;
  }
  for(uint8_t i = 0x00; i < LEDS_W; i++){
    if(BKP->DR1 & (0x01 << i)){
      if(ws2811W[i] < settings.brightness[i]) ws2811W[i]++;
    }else{
      if(ws2811W[i] > settings.brightness[i]) ws2811W[i]--;
    }
  }
  if(BKP->DR2 & 0x01){
    if(lighting.countCH1 < settings.brightnessCH1) lighting.countCH1++;
  }else{
    if(lighting.countCH1 > settings.brightnessCH1) lighting.countCH1--;
  }
  if(BKP->DR2 & 0x02){
    if(lighting.countCH2 < settings.brightnessCH2) lighting.countCH2++;
  }else{
    if(lighting.countCH2 > settings.brightnessCH2) lighting.countCH2--;
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
  
  NVIC_SetPriority(TIM7_IRQn, PRIORITY_BRIGHET);
  NVIC_EnableIRQ(TIM7_IRQn);
 }
