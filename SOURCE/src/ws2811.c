#include "ws2811.h"

uint8_t ws2811W[LEDS_W];
uint8_t ws2811RGB[LEDS_RGB][RGB];

uint8_t ws2811wDMA[LEDS_W + 0x06][0x08];
uint8_t ws2811rgbDMA[LEDS_RGB + 0x02][RGB][0x08];

void Ws2811WUpdate(void){
  uint8_t i, k;
  for(i = 0x00; i < LEDS_W; i++){
    for(k = 0; k < 0x08; k++){
      ws2811wDMA[i + 0x06][k] = (ws2811W[i] & (1 << (0x07 - k))) ? LED_HIGHT : LED_LOW;
    }
  }
}

void Ws2811RGBUpdate(void){
  uint8_t i, j, k;
  for(i = 0; i < LEDS_RGB; i++){
    for(j = 0; j < RGB; j++){
      for(k = 0; k < 0x08; k++){
        ws2811rgbDMA[i + 0x02][j][k] = (ws2811RGB[i][j] & (1 << (0x07 - k))) ? LED_HIGHT : LED_LOW;
      }
    }
  }
}

void Ws2811Init(void){
  GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_CNF3);
  GPIOA->CRL |= GPIO_CRL_MODE2 | GPIO_CRL_MODE3;
  GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1;
  
  RCC->AHBENR |= RCC_AHBENR_DMA2EN;
  DMA2_Channel1->CPAR = (uint32_t) &TIM5->CCR4;
  DMA2_Channel1->CMAR = (uint32_t) &ws2811rgbDMA[0x00];
  DMA2_Channel1->CNDTR = (LEDS_RGB + 0x02) * RGB * 0x08;
  DMA2_Channel1->CCR |= DMA_CCR1_MINC | DMA_CCR1_DIR | DMA_CCR1_PSIZE_0 |DMA_CCR1_CIRC; 
  
  DMA2_Channel2->CPAR = (uint32_t) &TIM5->CCR3;
  DMA2_Channel2->CMAR = (uint32_t) &ws2811wDMA[0x00];
  DMA2_Channel2->CNDTR = (LEDS_W + 0x06) * 0x08;
  DMA2_Channel2->CCR |= DMA_CCR2_MINC | DMA_CCR2_DIR | DMA_CCR2_PSIZE_0 |DMA_CCR2_CIRC;  
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  TIM5->PSC = 0x00;
  TIM5->ARR = 0x63;
  TIM5->CCR3 = 0x00;
  TIM5->CCR4 = 0x00;
  TIM5->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
  TIM5->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
  TIM5->DIER = TIM_DIER_CC3DE | TIM_DIER_CC4DE;
  TIM5->CCER = TIM_CCER_CC3E | TIM_CCER_CC4E;
  
  ws2811W[0x00] = BKP->DR2;
  ws2811W[0x01] = BKP->DR3;
  ws2811W[0x02] = BKP->DR4;
  ws2811W[0x03] = BKP->DR5;
  ws2811W[0x04] = BKP->DR6;
  ws2811W[0x05] = BKP->DR7;
  ws2811W[0x06] = BKP->DR8;
  ws2811W[0x07] = BKP->DR9;
  ws2811W[0x08] = BKP->DR10;
  Ws2811WUpdate();
  
  uint8_t i, j, k;
  for(i = 0x02; i < LEDS_RGB + 0x02; i++){
    for (j = 0x00; j < RGB; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811rgbDMA[i][j][k] = LED_LOW;
      }
    }
  }
  
  DMA2_Channel1->CCR |= DMA_CCR1_EN;
  DMA2_Channel2->CCR |= DMA_CCR2_EN;
  TIM5->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
}
