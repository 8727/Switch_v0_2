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
  GPIOA->CRL |= GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1;
  
  RCC->AHBENR |= RCC_AHBENR_DMA2EN;
  
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM5RST;    // Сброс таймера 4
  RCC->APB1RSTR &= RCC_APB1RSTR_TIM5RST;   
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;     // Разрешаем тактирование таймера 4
  TIM5->CR1 = BIT(7);          //  1: TIMx_ARR register is buffered.
  TIM5->CR2 = 0x00;               
  TIM5->PSC = 0x00;               // Предделитель генерирует частоту 72 МГц
  TIM5->ARR = 0x63;          // Перегрузка таймера каждые 1.25 мкс
  TIM5->CCMR2 = 0x00
               + LSHIFT(6, 4) // OC3M: Output compare 3 mode | 110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive.
               + LSHIFT(1, 3) // OC3PE: Output compare 3 preload enable
               + LSHIFT(0, 0) // CC3S: Capture/Compare 3 selection | 00: CC3 channel is configured as output
  ; 
  TIM5->CNT = 0;
  TIM5->CCR3 = 0;
  TIM5->DIER = BIT(11);        // Bit 11 CC3DE: Capture/Compare 3 DMA request enable. Разрешаем запросы DMA
  TIM5->CR1 |= BIT(0);         // Запускаем таймер
  TIM5->CCER = BIT(8);         // Разрешаем работы выхода, чтобы возникали сигналы для DMA
  
  
  
  
  
  
  
}


