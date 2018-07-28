#include "ili9488.h"

void LcdSendCommand(uint16_t reg){
  LCD_REG = reg; 
}

void LcdSendData(uint16_t data){
  LCD_DATA = data;
}

void LcdSetWindows(uint16_t x, uint16_t y, uint16_t width, uint16_t height){

//  if(hx8347i.width < width) width = hx8347i.width;
//  if(hx8347i.height < height) height = hx8347i.height;
//  if(hx8347i.width < (x + width)) x = hx8347i.width - width;
//  if(hx8347i.height <(y + height)) y = hx8347i.height - height;

  LcdSendCommand(0x2A); 
  LcdSendData(x >> 8);
  LcdSendData(x);
  LcdSendData((x + width - 0x01) >> 8);
  LcdSendData(x + width - 0x01);
  LcdSendCommand(0x2B); 
  LcdSendData(y >> 8);
  LcdSendData(y);
  LcdSendData((y + height - 0x01) >> 8);
  LcdSendData(y + height - 0x01);
  LcdSendCommand(0x2C);
}

void LcdDelay (uint32_t timedelay){
  while (timedelay--) {}
}
 
void LcdInitt(void){
  GPIOD->CRL |= GPIO_CRL_MODE3;
  GPIOD->CRL &= ~(GPIO_CRL_CNF3);
  
  GPIOD->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
  GPIOD->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
  GPIOD->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1);
  
  GPIOD->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15);
  GPIOD->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF14 | GPIO_CRH_CNF15);
  GPIOD->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1 | GPIO_CRH_CNF14_1 | GPIO_CRH_CNF15_1);
  
  GPIOE->CRL |= GPIO_CRL_MODE7;
  GPIOE->CRL &= ~(GPIO_CRL_CNF7);
  GPIOE->CRL |= GPIO_CRL_CNF7_1;
  
  GPIOE->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15);
  GPIOE->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF14 | GPIO_CRH_CNF15);
  GPIOE->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1 | GPIO_CRH_CNF12_1 | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF14_1 | GPIO_CRH_CNF15_1);
  
  RCC->AHBENR |= RCC_AHBENR_FSMCEN;
  
  FSMC_Bank1->BTCR[0] = FSMC_BCR1_MWID_0 | FSMC_BCR1_WREN | FSMC_BCR1_MBKEN;
  FSMC_Bank1->BTCR[1] = FSMC_BTR1_DATAST_0;
  
  ILI9488_RESET_LOW;
  LcdDelay (312500);      //25ms
  ILI9488_RESET_HIGHT;
  LcdDelay (125000);      //10ms
  
  LcdSendCommand(0xE0);
  LcdSendData(0x00);
  LcdSendData(0x07);
  LcdSendData(0x10);
  LcdSendData(0x09);
  LcdSendData(0x17);
  LcdSendData(0x0B);
  LcdSendData(0x40);
  LcdSendData(0x8A);
  LcdSendData(0x4B);
  LcdSendData(0x0A);
  LcdSendData(0x0D);
  LcdSendData(0x0F);
  LcdSendData(0x15);
  LcdSendData(0x16);
  LcdSendData(0x0F);
  LcdSendCommand(0xE1);
  LcdSendData(0x00);
  LcdSendData(0x1A);
  LcdSendData(0x1B);
  LcdSendData(0x02);
  LcdSendData(0x0D);
  LcdSendData(0x05);
  LcdSendData(0x30);
  LcdSendData(0x35);
  LcdSendData(0x43);
  LcdSendData(0x02);
  LcdSendData(0x0A);
  LcdSendData(0x09);
  LcdSendData(0x32);
  LcdSendData(0x36);
  LcdSendData(0x0F);
  LcdSendCommand(0xB1);
  LcdSendData(0xA0);
  LcdSendCommand(0xB4);
  LcdSendData(0x02);
  LcdSendCommand(0xC0);
  LcdSendData(0x17);
  LcdSendData(0x15);
  LcdSendCommand(0xC1);
  LcdSendData(0x41);
  LcdSendCommand(0xC5);
  LcdSendData(0x00);
  LcdSendData(0x0A);
  LcdSendData(0x80);
  
  LcdSendCommand(0xB6);
  LcdSendData(0x02);
  LcdSendCommand(0xB7);
  LcdSendData(0x06);
  LcdSendCommand(0x36);
//  LcdSendData(0x48);   //Dspl_Rotation_0_degr
  LcdSendData(0x38);   //Dspl_Rotation_90_degr
//  LcdSendData(0x88);   //Dspl_Rotation_180_degr
//  LcdSendData(0xE8);   //Dspl_Rotation_270_degr
  LcdSendCommand(0x3A);
  LcdSendData(0x55);
  LcdSendCommand(0xE9);
  LcdSendData(0x00);
  LcdSendCommand(0XF7);
  LcdSendData(0xA9);
  LcdSendData(0x51);
  LcdSendData(0x2C);
  LcdSendData(0x82);
  LcdSendCommand(0x11);
  LcdDelay (1500000);      //120ms
  LcdSendCommand(0x29);
  
  LcdSetWindows(0x0000, 0x0000, 0x01E0, 0x0140);
  uint32_t i = 0x00025800;
  while(i--){
    LcdSendData(BLACK);
  }
  LcdSetWindows(239, 159, 240, 160);
  i = 0x04;
  while(i--){
    LcdSendData(WHITE);
  }
}

void BrighetLcdInit(void){
  GPIOA->CRH |= GPIO_CRH_MODE15;
  GPIOA->CRH &= ~(GPIO_CRH_CNF15);
  GPIOA->CRH |= GPIO_CRH_CNF15_1;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 0x0FFF;
  TIM2->ARR = 0xFF;
  TIM2->CCR1 = 0x00;  // Brighet LCD
  TIM2->CCER |= TIM_CCER_CC1E;
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;
  TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
  TIM2->CR1 &= ~TIM_CR1_DIR;
  TIM2->CR1 &= ~TIM_CR1_CMS;
  TIM2->CR1 |= TIM_CR1_ARPE;
  TIM2->CR1 |= TIM_CR1_CEN; // Timer Brighet
}
