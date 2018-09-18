#include "ws2811.h"

extern uint8_t ws2811W[LEDS_W];
extern uint8_t ws2811RGB[LEDS_RGB][RGB];

uint8_t ws2811wDMA[LEDS_W + 0x06][0x08];
uint8_t ws2811rgbDMA[LEDS_RGB + 0x02][RGB][0x08];

void LEDstrip_init(void){
  uint8_t i, j, k;
  for(i = 0x06; i < LEDS_W + 0x06; i++){
    for (k = 0x00; k < 0x08; k++){
      ws2811wDMA[i][k] = LED_HIGHT;
    }
  }
  for(i = 0x02; i < LEDS_RGB + 0x02; i++){
    for (j = 0x00; j < RGB; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811rgbDMA[i][j][k] = LED_HIGHT;
      }
    }
  }
  
  GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_CNF3);
  GPIOA->CRL |= GPIO_CRL_MODE2 | GPIO_CRL_MODE3;
  GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1;
  
  RCC->AHBENR |= RCC_AHBENR_DMA2EN;
  DMA2_Channel1->CPAR = (uint32_t) &TIM5->CCR4;
  DMA2_Channel1->CMAR = (uint32_t) &ws2811rgbDMA[0x00];
  DMA2_Channel1->CNDTR = (LEDS_RGB + 0x02) * RGB * 0x08;
  DMA2_Channel1->CCR |= DMA_CCR1_MINC | DMA_CCR1_DIR | DMA_CCR1_PSIZE_0 | DMA_CCR1_CIRC | DMA_CCR1_EN;
  
  DMA2_Channel2->CPAR = (uint32_t) &TIM5->CCR3;
  DMA2_Channel2->CMAR = (uint32_t) &ws2811wDMA[0x00];
  DMA2_Channel2->CNDTR = (LEDS_W + 0x06) * 0x08;
  DMA2_Channel2->CCR |= DMA_CCR2_MINC | DMA_CCR2_DIR | DMA_CCR2_PSIZE_0 | DMA_CCR2_CIRC | DMA_CCR2_EN;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  TIM5->PSC = 0x00;
  TIM5->ARR = 0x63;
  TIM5->CCR3 = 0x00;
  TIM5->CCR4 = 0x00;
  TIM5->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
  TIM5->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
  TIM5->DIER = TIM_DIER_CC3DE | TIM_DIER_CC4DE;
  TIM5->CCER = TIM_CCER_CC3E | TIM_CCER_CC4E;
  TIM5->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
}
