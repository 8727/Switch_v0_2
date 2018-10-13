#include "ws2811.h"

uint8_t ws2811W[LEDS_W];
uint8_t ws2811P[LEDS_P];
uint8_t ws2811BRG[LEDS_BRG * 0x03];

uint8_t ws2811wDMA[LEDS_W + 0x06][0x08];
uint8_t ws2811pDMA[LEDS_P + 0x06][0x08];
uint8_t ws2811brgDMA[LEDS_BRG + 0x02][0x03][0x08];

void Ws2811WUpdate(void){
  uint8_t i, k;
  for(i = 0x00; i < LEDS_W; i++){
    for(k = 0; k < 0x08; k++){
      ws2811wDMA[i + 0x06][k] = (ws2811W[i] & (0x01 << (0x07 - k))) ? LED_HIGHT : LED_LOW;
    }
  }
}

void Ws2811PUpdate(void){
  uint8_t i, k;
  for(i = 0x00; i < LEDS_P; i++){
    for(k = 0; k < 0x08; k++){
      ws2811pDMA[i + 0x06][k] = (ws2811P[i] & (0x01 << (0x07 - k))) ? LED_HIGHT : LED_LOW;
    }
  }
}

void Ws2811BRGUpdate(void){
  uint8_t i, j, k;
  for(i = 0; i < LEDS_BRG; i++){
    for(j = 0; j < 0x03; j++){
      for(k = 0; k < 0x08; k++){
        ws2811brgDMA[i + 0x02][j][k] = (ws2811BRG[i * 0x03 + j] & (0x01 << (0x07 - k))) ? LED_HIGHT : LED_LOW;
      }
    }
  }
}

void Ws2811Init(void){
  GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3);
  GPIOA->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3;
  GPIOA->CRL |= GPIO_CRL_CNF0_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1;
  
  RCC->AHBENR |= RCC_AHBENR_DMA2EN;
  DMA2_Channel1->CPAR = (uint32_t) &TIM5->CCR4;
  DMA2_Channel1->CMAR = (uint32_t) &ws2811brgDMA[0x00];
  DMA2_Channel1->CNDTR = (LEDS_BRG + 0x02) * 0x18;
  DMA2_Channel1->CCR |= DMA_CCR1_MINC | DMA_CCR1_DIR | DMA_CCR1_PSIZE_0 |DMA_CCR1_CIRC;
  
  DMA2_Channel2->CPAR = (uint32_t) &TIM5->CCR3;
  DMA2_Channel2->CMAR = (uint32_t) &ws2811wDMA[0x00];
  DMA2_Channel2->CNDTR = (LEDS_W + 0x06) * 0x08;
  DMA2_Channel2->CCR |= DMA_CCR2_MINC | DMA_CCR2_DIR | DMA_CCR2_PSIZE_0 |DMA_CCR2_CIRC;
  
  DMA2_Channel5->CPAR = (uint32_t) &TIM5->CCR1;
  DMA2_Channel5->CMAR = (uint32_t) &ws2811pDMA[0x00];
  DMA2_Channel5->CNDTR = (LEDS_P + 0x06) * 0x08;
  DMA2_Channel5->CCR |= DMA_CCR5_MINC | DMA_CCR5_DIR | DMA_CCR5_PSIZE_0 |DMA_CCR5_CIRC;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  TIM5->PSC = 0x00;
  TIM5->ARR = 0x63;
  TIM5->CCR3 = 0x00;
  TIM5->CCR4 = 0x00;
  TIM5->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
  TIM5->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
  TIM5->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
  TIM5->DIER = TIM_DIER_CC1DE | TIM_DIER_CC3DE | TIM_DIER_CC4DE;
  TIM5->CCER = TIM_CCER_CC1E | TIM_CCER_CC3E | TIM_CCER_CC4E;
  
  uint8_t i, j, k;
  for(i = 0x00; i < LEDS_W; i++){
    ws2811W[i] = settings.brightnessW[i];
  }
  Ws2811WUpdate();
  for(i = 0x00; i < LEDS_P; i++){
    ws2811P[i] = settings.brightnessP[i];
  }
  Ws2811PUpdate();
  for(i = 0x02; i < LEDS_BRG + 0x02; i++){
    for (j = 0x00; j < 0x03; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811brgDMA[i][j][k] = LED_LOW;
      }
    }
  }
  
  DMA2_Channel1->CCR |= DMA_CCR1_EN;
  DMA2_Channel2->CCR |= DMA_CCR2_EN;
  DMA2_Channel5->CCR |= DMA_CCR5_EN;
  TIM5->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
}
