#include "ws2811.h"

extern uint8_t ws2811RGB[LEDS_RGB + 0x02][LEDS_SEGMENTS][0x08];
extern uint8_t ws2811W[LEDS_W + 0x02][LEDS_SEGMENTS][0x08];

void LEDstrip_init(void){
  uint8_t i, j, k;
  for(i = 0x02; i < LEDS_RGB + 0x02; i++){
    for (j = 0x00; j < LEDS_SEGMENTS; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811RGB[i][j][k] = LED_LOW;
      }
    }
  }
  for(i = 0x02; i < LEDS_W + 0x02; i++){
    for (j = 0x00; j < LEDS_SEGMENTS; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811W[i][j][k] = LED_LOW;
      }
    }
  }
  for(i = 0x00; i < 0x02; i++){
    for (j = 0x00; j < LEDS_SEGMENTS; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811RGB[i][j][k] = 0x00;
        ws2811W[i][j][k] = 0x00;
      }
    }
  }
  
  GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3);
  GPIOA->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3;
  GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  TIM5->PSC = 0x00;
  TIM5->ARR = 99;
  TIM5->CCR3 = 0x00;
  TIM5->CCR4 = 0x00;
  TIM5->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
  TIM5->DIER = TIM_DIER_CC3DE;
  TIM5->DIER = TIM_DIER_CC4DE;
  TIM5->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
  
  
  
  
  
  
  
  RCC->AHBENR |= RCC_AHBENR_DMA2EN;
 // зазадем адрес приемника данных
 DMA2_Channel1->CPAR = (uint32_t) &TIM5->CCR4;
 // задаем адрес источника данных
 DMA2_Channel1->CMAR = (uint32_t) &ws2811RGB[0x00];
 // указываем число пересылаемых данных
 DMA2_Channel1->CNDTR = DMA_BUFF_SIZE;
 // разрешаем работу + режим
 DMA2_Channel1->CCR|= DMA_CCR1_MINC|     // инкремент памяти
       DMA_CCR1_DIR |     // направление передачи данных (из памяти в периферию)
       DMA_CCR1_CIRC |    // Режим цикличности
          DMA_CCR1_PSIZE_0 | // Размер данных периферии  16 бит
                      DMA_CCR1_MSIZE_0 | // Размер данных в памяти   16 бит
       DMA_CCR1_TCIE  ;   // РАзрешаем прерывание по окончанию передачи
 //DMA_CCR1_EN;
  
  
  
  
  
  
}


