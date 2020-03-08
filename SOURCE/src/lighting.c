#include "lighting.h"

struct BRGInitTypeDef brg;

void BRGBrighetAllFade(uint8_t step, uint8_t level){
  uint8_t z = 0x00;
  for(uint16_t i = 0x00; i < LEDS_BRG * 0x03; i++){
    if(ws2811BRG[i] > level){
      if(ws2811BRG[i] > step){
        ws2811BRG[i] -= step;
      }else{
        ws2811BRG[i] = 0x00;
      }
      z += ws2811BRG[i];
    }
    if(0x00 == z) BRG_POWER_OFF;
  }
  Ws2811BRGUpdate();
}

void BRGLeftToRighet(void){
  BRG_POWER_ON;
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
    ws2811BRG[y] = LEDS_BRG_B;
    ws2811BRG[y + 0x01] = LEDS_BRG_R;
    ws2811BRG[y + 0x02] = LEDS_BRG_G;
    brg.set++;
    if(LEDS_BRG <= brg.set){
      brg.wait = 0x01;
    }
  }
  BRGBrighetAllFade(0x08, 0x00);
}

void BRGRighetToLeft(void){
  BRG_POWER_ON;
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
    ws2811BRG[x - y] = LEDS_BRG_B;
    ws2811BRG[x - y + 0x01] = LEDS_BRG_R;
    ws2811BRG[x - y + 0x02] = LEDS_BRG_G;
    brg.set++;
    if(LEDS_BRG <= brg.set){
      brg.wait = 0x01;
    }
  }
  BRGBrighetAllFade(0x08, 0x00);
}

void BRGRighetLeftToCenter(void){
  BRG_POWER_ON;
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
    ws2811BRG[y] = LEDS_BRG_B;
    ws2811BRG[y + 0x01] = LEDS_BRG_R;
    ws2811BRG[y + 0x02] = LEDS_BRG_G;
    ws2811BRG[x - y] = LEDS_BRG_B;
    ws2811BRG[x - y + 0x01] = LEDS_BRG_R;
    ws2811BRG[x - y + 0x02] = LEDS_BRG_G;
    brg.set++;
    if(LEDS_BRG / 0x02 < brg.set){
      brg.wait = 0x01;
    }
  }
  BRGBrighetAllFade(0x08, 0x00);
}

void BRGCenterToRighetLeft(void){
  BRG_POWER_ON;
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
    ws2811BRG[z + y + 0x02] = LEDS_BRG_B;
    ws2811BRG[z + y + 0x03] = LEDS_BRG_R;
    ws2811BRG[z + y + 0x04] = LEDS_BRG_G;
    ws2811BRG[z - y + 0x01] = LEDS_BRG_G;
    ws2811BRG[z - y + 0x00] = LEDS_BRG_R;
    ws2811BRG[z - y - 0x01] = LEDS_BRG_B;
    brg.set++;
    if(LEDS_BRG / 0x02 < brg.set){
      brg.wait = 0x01;
    }
  }
  Ws2811BRGUpdate();
}


void UpdateBrightnessW(void){
  uint8_t i;
  uint8_t z = 0x00;
  for(i = 0x00; i < LEDS_W; i++){
    if(BKP->DR1 & (0x01 << i)){
      if(ws2811W[i] < settings.brightnessW[i]) ws2811W[i]++;
      W_POWER_ON;
    }else{
      if(ws2811W[i] > settings.brightnessW[i]){
        ws2811W[i]--;
        z += ws2811W[i];
      }
    }
  }
  Ws2811WUpdate();
  if(0x00 == z) W_POWER_OFF;
  switch(brg.effect){
    case 0x01: BRGLeftToRighet();
    break;
    case 0x02: BRGRighetToLeft();
    break;
    case 0x03: BRGRighetLeftToCenter();
    break;
    case 0x04: BRGCenterToRighetLeft();
    break;
    case 0x00: default: BRGBrighetAllFade(0x01, 0x00);
    break;
  }
}

void StartLighting(void){
  
}

void StopLighting(void){
  
}
