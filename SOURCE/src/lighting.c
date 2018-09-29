#include "lighting.h"

struct RGBInitTypeDef rgb;

void BrighetRGBAllFade(uint8_t step){
  for(uint16_t i = 0x00; i < LEDS_RGB * 0x03; i++){
    if(ws2811RGB[i] > step){
      ws2811RGB[i] -= step;
    }else{
      ws2811RGB[i] = 0x00;
    }
  }
}

void RGBLeftToRighet(void){
  if(0x00 < rgb.wait){
    rgb.pause++;
    if(0x06 < rgb.pause){
      rgb.set = 0x00;
      rgb.wait = 0x00;
      rgb.pause = 0x00;
      rgb.effect = 0x03;
    }
  }else{
    uint8_t y = rgb.set * 0x03;
    ws2811RGB[y] = 0xFF;
    ws2811RGB[y + 0x01] = 0xFF;
    ws2811RGB[y + 0x02] = 0xFF;
    rgb.pause++;
    if(0x06 < rgb.pause){
      rgb.pause = 0x00;
      rgb.set++;
    }
    if(LEDS_RGB <= rgb.set){
      rgb.wait = 0x01;
    }
    BrighetRGBAllFade(0x04);
  }
}

void RGBRighetToLeft(void){
  if(0x00 < rgb.wait){
    rgb.pause++;
    if(0x06 < rgb.pause){
      rgb.set = 0x00;
      rgb.wait = 0x00;
      rgb.pause = 0x00;
      rgb.effect = 0x03;
    }
  }else{
    uint8_t y = rgb.set * 0x03;
    uint16_t x = LEDS_RGB * 0x03 - 0x03;
    ws2811RGB[x - y] = 0xFF;
    ws2811RGB[x - y + 0x01] = 0xFF;
    ws2811RGB[x - y + 0x02] = 0xFF;
    rgb.pause++;
    if(0x06 < rgb.pause){
      rgb.pause = 0x00;
      rgb.set++;
    }
    if(LEDS_RGB <= rgb.set){
      rgb.wait = 0x01;
    }
    BrighetRGBAllFade(0x04);
  }
}

void RGBRighetLeftToCenter(void){
  uint8_t y = rgb.set * 0x03;
  uint16_t x = LEDS_RGB * 0x03 - 0x03;
  if(0x00 < rgb.wait){
    rgb.pause++;
    ws2811RGB[x / 0x02] = 0xFF;
    ws2811RGB[x / 0x02 + 0x01] = 0xFF;
    ws2811RGB[x / 0x02 + 0x02] = 0xFF;
    if(0x06 < rgb.pause){
      rgb.set = 0x00;
      rgb.wait = 0x00;
      rgb.pause = 0x00;
      rgb.effect = 0x04;
    }
  }else{
    ws2811RGB[y] = 0xFF;
    ws2811RGB[y + 0x01] = 0xFF;
    ws2811RGB[y + 0x02] = 0xFF;
    ws2811RGB[x - y] = 0xFF;
    ws2811RGB[x - y + 0x01] = 0xFF;
    ws2811RGB[x - y + 0x02] = 0xFF;
    rgb.pause++;
    if(0x06 < rgb.pause){
      rgb.pause = 0x00;
      rgb.set++;
    }
    if(LEDS_RGB / 0x02 < rgb.set){
      rgb.wait = 0x01;
    }
    BrighetRGBAllFade(0x04);
  }
}

void RGBCenterToRighetLeft(void){
  if(0x00 < rgb.wait){
    rgb.pause++;
    if(0xFE < rgb.pause){
      rgb.set = 0x00;
      rgb.wait = 0x00;
      rgb.pause = 0x00;
      rgb.effect = 0x00;
    }
  }else{
    uint8_t y = rgb.set * 0x03;
    uint16_t x = LEDS_RGB * 0x03 - 0x03;
    uint8_t z = x / 0x02;
    ws2811RGB[z + y] = 0xFF;
    ws2811RGB[z + y + 0x01] = 0xFF;
    ws2811RGB[z + y + 0x02] = 0xFF;
    ws2811RGB[y - z + 0x02] = 0xFF;
    ws2811RGB[y - z + 0x01] = 0xFF;
    ws2811RGB[y - z] = 0xFF;
    rgb.pause++;
    if(0x06 < rgb.pause){
      rgb.pause = 0x00;
      rgb.set++;
    }
    if(LEDS_RGB / 0x02 < rgb.set){
      rgb.wait = 0x01;
    }
  }
}


void TIM7_IRQHandler(void){
  TIM7->SR &= ~TIM_SR_UIF;
  uint8_t i;
  for(i = 0x00; i < LEDS_W; i++){
    if(BKP->DR1 & (0x01 << i)){
      if(ws2811W[i] < settings.brightnessW[i]) ws2811W[i]++;
    }else{
      if(ws2811W[i] > settings.brightnessW[i]) ws2811W[i]--;
    }
  }
  for(i = 0x00; i < LEDS_P; i++){
    if(BKP->DR2 & (0x01 << i)){
      if(ws2811P[i] < settings.brightnessP[i]) ws2811P[i]++;
    }else{
      if(ws2811P[i] > settings.brightnessP[i]) ws2811P[i]--;
    }
  }
  switch(rgb.effect){
    case 0x01: RGBLeftToRighet();
    break;
    case 0x02: RGBRighetToLeft();
    break;
    case 0x03: RGBRighetLeftToCenter();
    break;
    case 0x04: RGBCenterToRighetLeft();
    break;
    case 0x00: default: BrighetRGBAllFade(0x01);
    break;
  }
}

void BrighetInit(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  TIM7->PSC = 0x1F3F; // 7999 80000000:8000=10000Hz
  TIM7->ARR = 0x50; // 125Hz
  TIM7->SR = 0x00;
  TIM7->DIER |= TIM_DIER_UIE;
  TIM7->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  NVIC_SetPriority(TIM7_IRQn, PRIORITY_BRIGHET);
  NVIC_EnableIRQ(TIM7_IRQn);
 }
