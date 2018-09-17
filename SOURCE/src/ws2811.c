#include "ws2811.h"

extern uint8_t ws2811RGB[LEDS_RGB + 0x02][COLRS][0x08];
extern uint8_t ws2811W[LEDS_W + 0x02][SEGMENTS][0x08];

void LEDstrip_init(void){
  uint8_t i, j, k;
  for(i = 0x02; i < LEDS_RGB + 0x02; i++){
    for (j = 0x00; j < COLRS; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811RGB[i][j][k] = LED_LOW;
      }
    }
  }
  for(i = 0x02; i < LEDS_W + 0x02; i++){
    for (j = 0x00; j < SEGMENTS; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811W[i][j][k] = LED_LOW;
      }
    }
  }
  for(i = 0x00; i < 0x02; i++){
    for (j = 0x00; j < COLRS; j++){
      for (k = 0x00; k < 0x08; k++){
        ws2811RGB[i][j][k] = 0x00;
        ws2811W[i][j][k] = 0x00;
      }
    }
  }
  
  
  RCC->AHBENR |= RCC_AHBENR_DMA2EN;
  
  
  
  
  
  
  
  
  
}


