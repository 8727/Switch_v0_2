#include "xpt2046.h"

struct xpt2046InitTypeDef xpt2046;

uint16_t Xpt2046Read(uint8_t addr){
  uint16_t data;
  XPT2046_CS_LOW;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = addr;
  while(!(SPI2->SR & SPI_SR_RXNE));
  (void) SPI2->DR;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = 0x00;
  while(!(SPI2->SR & SPI_SR_RXNE));
  data = SPI2->DR;
  data <<= 0x08;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = 0x00;
  while(!(SPI2->SR & SPI_SR_RXNE));
  data |= SPI2->DR;
  data >>= 0x03;
  data &= 0x0FFF;
  XPT2046_CS_HIGHT;
  return data;
}

void TIM6_IRQHandler(void){
  TIM6->SR &= ~TIM_SR_UIF;
  
  
  xpt2046.brg = Xpt2046Read(XPT2046_BRG);
  xpt2046.bat = Xpt2046Read(XPT2046_BAT);
  if(GPIOB->IDR & GPIO_IDR_IDR11){
    xpt2046.pressed = XPT2046_RELEASED;
  }else{
    xpt2046.x = Xpt2046Read(XPT2046_X);
    xpt2046.y = Xpt2046Read(XPT2046_Y);
    xpt2046.pressed = XPT2046_PRESSED;
  }
  UpdateBrightnessW();
}

void Xpt2046Calibration(void){
  uint16_t drawX[0x04];
  uint16_t drawY[0x04];
  uint16_t touchX[0x04];
  uint16_t touchY[0x04];
  uint8_t i, x, y;
  x = settings.maxX / 0x08;
  y = settings.maxY / 0x08;
  drawX[0x00] = x;
  drawY[0x00] = y;
  drawX[0x01] = x * 0x07;
  drawY[0x01] = y;
  drawX[0x02] = x * 0x07;
  drawY[0x02] = y * 0x07;
  drawX[0x03] = x;
  drawY[0x03] = y * 0x07;
  
  TIM2->CCR1 = 0xFF;
  GuiFullWindow(WHITE);
  DelayMs(60);
  while(!xpt2046.pressed){}
  for(i = 0x00; i < 0x04; i++){
    GuiCalibDraw(drawX[i], drawY[i]);
    while(xpt2046.pressed){}
    while(!xpt2046.pressed){}
    
    touchX[i] = Xpt2046Read(XPT2046_X);
    touchY[i] = Xpt2046Read(XPT2046_Y);
    
    GuiCalibErase(drawX[i], drawY[i]);
  }
  settings.ax = (touchX[0x02] - touchX[0x00]) / (drawX[0x02] - drawX[0x00]);
  settings.bx = drawX[0x00] - touchX[0x00] / settings.ax;
  settings.ay = (touchY[0x02] - touchY[0x00]) / (drawY[0x02] - drawY[0x00]);
  settings.by = drawY[0x00] - touchY[0x00] / settings.ay;
  
  GuiFullWindow(BLACK);
}

void Xpt2046Init(void){
  GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF15);
  GPIOB->CRH |= GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1;
  GPIOB->CRH |= GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15;
  
  xpt2046.pressed = XPT2046_RELEASED;
  
  XPT2046_CS_HIGHT;

  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  SPI2->CR1 |= SPI_CR1_BR_2;
  SPI2->CR1 &= ~SPI_CR1_CPOL;
  SPI2->CR1 &= ~SPI_CR1_CPHA;
  SPI2->CR1 &= ~SPI_CR1_DFF;
  SPI2->CR1 &= ~SPI_CR1_LSBFIRST;
//  SPI2->CR1 |= SPI_CR1_DFF;
  SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI2->CR1 |= SPI_CR1_MSTR;
  SPI2->CR1 |= SPI_CR1_SPE;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
  TIM6->PSC = 0x1F3F; // 7999 80000000:8000=10000Hz
  TIM6->ARR = 0xC7; // 50Hz
//  TIM6->ARR = 0x63; // 100Hz
  TIM6->SR = 0x00;
  TIM6->DIER |= TIM_DIER_UIE;
  TIM6->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  NVIC_SetPriority(TIM6_IRQn, PRIORITY_XPT2046_LIGHT);
  NVIC_EnableIRQ(TIM6_IRQn);
}
