#include "ili9488.h"

#pragma O0
void LcdInit(void){
  GPIOA->CRH &= ~(GPIO_CRH_CNF15);
  GPIOA->CRH |= GPIO_CRH_CNF15_1;
  GPIOA->CRH |= GPIO_CRH_MODE15;
  
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
  
  GPIOD->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
  GPIOD->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
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
  
  FSMC_Bank1->BTCR[0x00] = FSMC_BCR1_MWID_0 | FSMC_BCR1_WREN | FSMC_BCR1_MBKEN;
  FSMC_Bank1->BTCR[0x01] = FSMC_BTR1_DATAST_0;
  
  ILI9488_RESET_LOW;
  DelayMs(0x19); // 25ms
  ILI9488_RESET_HIGHT;
  DelayMs(0x0A); // 10ms
  
  LCD_REG = 0xE0;
  LCD_DATA = 0x00;
  LCD_DATA = 0x07;
  LCD_DATA = 0x10;
  LCD_DATA = 0x09;
  LCD_DATA = 0x17;
  LCD_DATA = 0x0B;
  LCD_DATA = 0x40;
  LCD_DATA = 0x8A;
  LCD_DATA = 0x4B;
  LCD_DATA = 0x0A;
  LCD_DATA = 0x0D;
  LCD_DATA = 0x0F;
  LCD_DATA = 0x15;
  LCD_DATA = 0x16;
  LCD_DATA = 0x0F;
  LCD_REG = 0xE1;
  LCD_DATA = 0x00;
  LCD_DATA = 0x1A;
  LCD_DATA = 0x1B;
  LCD_DATA = 0x02;
  LCD_DATA = 0x0D;
  LCD_DATA = 0x05;
  LCD_DATA = 0x30;
  LCD_DATA = 0x35;
  LCD_DATA = 0x43;
  LCD_DATA = 0x02;
  LCD_DATA = 0x0A;
  LCD_DATA = 0x09;
  LCD_DATA = 0x32;
  LCD_DATA = 0x36;
  LCD_DATA = 0x0F;
  LCD_REG = 0xB1;
  LCD_DATA = 0xA0;
  LCD_REG = 0xB4;
  LCD_DATA = 0x02;
  LCD_REG = 0xC0;
  LCD_DATA = 0x17;
  LCD_DATA = 0x15;
  LCD_REG = 0xC1;
  LCD_DATA = 0x41;
  LCD_REG = 0xC5;
  LCD_DATA = 0x00;
  LCD_DATA = 0x0A;
  LCD_DATA = 0x80;
  
  LCD_REG = 0xB6;
  LCD_DATA = 0x02;
  LCD_REG = 0xB7;
  LCD_DATA = 0x06;
  LCD_REG = 0x36;
  switch(settings.rotation){
    case 0x27: LCD_DATA = 0xE8; //Dspl_Rotation_270
    break;
    case 0x18: LCD_DATA = 0x88;  //Dspl_Rotation_180
    break;
    case 0x09: LCD_DATA = 0x38;  //Dspl_Rotation_90
    break;
    default: LCD_DATA = 0x48;  //Dspl_Rotation_0
    break;
  }
  LCD_REG = 0x3A;
  LCD_DATA = 0x55;
  LCD_REG = 0xE9;
  LCD_DATA = 0x00;
  LCD_REG = 0XF7;
  LCD_DATA = 0xA9;
  LCD_DATA = 0x51;
  LCD_DATA = 0x2C;
  LCD_DATA = 0x82;
  LCD_REG = 0x11;
  DelayMs(0x78); // 120ms
  LCD_REG = 0x29;
  
  GuiSetWindow(0x00, 0x00, settings.maxX, settings.maxY);
  GuiFullWindow(BLACK);
}
