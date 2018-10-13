#include "lighting.h"

struct RGBInitTypeDef brg;

void BrighetRGBAllFade(uint8_t step, uint8_t level){
  for(uint16_t i = 0x00; i < LEDS_BRG * 0x03; i++){
    if(ws2811BRG[i] > level){
      if(ws2811BRG[i] > step){
        ws2811BRG[i] -= step;
      }else{
        ws2811BRG[i] = 0x00;
      }
    }
  }
  Ws2811BRGUpdate();
}

void RGBLeftToRighet(void){
  if(0x00 < brg.wait){
    brg.pause++;
    if(0x20 < brg.pause){
      brg.set = 0x00;
      brg.wait = 0x00;
      brg.pause = 0x00;
      brg.effect = 0x02;
    }
  }else{
    uint16_t y = brg.set * 0x03;
    ws2811BRG[y] = 0xFF;
    ws2811BRG[y + 0x01] = 0xFF;
    ws2811BRG[y + 0x02] = 0xFF;
    brg.set++;
    if(LEDS_BRG <= brg.set){
      brg.wait = 0x01;
    }
  }
  BrighetRGBAllFade(0x08, 0x00);
}

void RGBRighetToLeft(void){
  if(0x00 < brg.wait){
    brg.pause++;
    if(0x20 < brg.pause){
      brg.set = 0x00;
      brg.wait = 0x00;
      brg.pause = 0x00;
      brg.effect = 0x03;
    }
  }else{
    uint16_t y = brg.set * 0x03;
    uint16_t x = LEDS_BRG * 0x03 - 0x03;
    ws2811BRG[x - y] = 0xFF;
    ws2811BRG[x - y + 0x01] = 0xFF;
    ws2811BRG[x - y + 0x02] = 0xFF;
    brg.set++;
    if(LEDS_BRG <= brg.set){
      brg.wait = 0x01;
    }
  }
  BrighetRGBAllFade(0x08, 0x00);
}

void RGBRighetLeftToCenter(void){
  uint16_t y = brg.set * 0x03;
  uint16_t x = LEDS_BRG * 0x03 - 0x03;
  if(0x00 < brg.wait){
    brg.pause++;
    if(0x20 < brg.pause){
      brg.set = 0x00;
      brg.wait = 0x00;
      brg.pause = 0x00;
      brg.effect = 0x04;
    }
  }else{
    ws2811BRG[y] = 0xFF;
    ws2811BRG[y + 0x01] = 0xFF;
    ws2811BRG[y + 0x02] = 0xFF;
    ws2811BRG[x - y] = 0xFF;
    ws2811BRG[x - y + 0x01] = 0xFF;
    ws2811BRG[x - y + 0x02] = 0xFF;
    brg.set++;
    if(LEDS_BRG / 0x02 < brg.set){
      brg.wait = 0x01;
    }
  }
  BrighetRGBAllFade(0x08, 0x00);
}

void RGBCenterToRighetLeft(void){
  if(0x00 < brg.wait){
    brg.pause++;
    if(0xA0 < brg.pause){
//    if(0x20 < brg.pause){
      brg.set = 0x00;
      brg.wait = 0x00;
      brg.pause = 0x00;
      brg.effect = 0x00;
    }
  }else{
    uint16_t y = brg.set * 0x03;
    uint16_t x = LEDS_BRG * 0x03 - 0x03;
    uint16_t z = x / 0x02;
    ws2811BRG[z + y + 0x02] = 0xFF;
    ws2811BRG[z + y + 0x03] = 0xFF;
    ws2811BRG[z + y + 0x04] = 0xFF;
    ws2811BRG[z - y + 0x01] = 0xFF;
    ws2811BRG[z - y + 0x00] = 0xFF;
    ws2811BRG[z - y - 0x01] = 0xFF;
    brg.set++;
    if(LEDS_BRG / 0x02 < brg.set){
      brg.wait = 0x01;
    }
  }
  Ws2811BRGUpdate();
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
  switch(brg.effect){
    case 0x01: RGBLeftToRighet();
    break;
    case 0x02: RGBRighetToLeft();
    break;
    case 0x03: RGBRighetLeftToCenter();
    break;
    case 0x04: RGBCenterToRighetLeft();
    break;
    case 0x00: default: BrighetRGBAllFade(0x01, 0x00);
    break;
  }
}

void BrighetInit(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  TIM7->PSC = 0x1F3F; // 7999 80000000:8000=10000Hz
  TIM7->ARR = 0xC7; // 50Hz
  TIM7->SR = 0x00;
  TIM7->DIER |= TIM_DIER_UIE;
  TIM7->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  NVIC_SetPriority(TIM7_IRQn, PRIORITY_BRIGHET);
  NVIC_EnableIRQ(TIM7_IRQn);
 }
