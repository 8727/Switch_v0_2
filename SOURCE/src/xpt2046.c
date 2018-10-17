#include "xpt2046.h"

struct xpt2046InitTypeDef xpt2046;
struct xpt2046ReadInitTypeDef xpt2046Read;

void TIM6_IRQHandler(void){
  TIM6->SR &= ~TIM_SR_UIF;
  if(GPIOB->IDR & GPIO_IDR_IDR11){
    xpt2046Read.count = 0x03;
  }else{
    xpt2046Read.count = 0x05;
  }
  XPT2046_CS_LOW;
  xpt2046Read.rawCount = 0x00;
  SPI2->CR2 |= SPI_CR2_RXNEIE;
  SPI2->DR = xpt2046Send[xpt2046Read.rawCount];
  UpdateBrightnessW();
}

void SPI2_IRQHandler(void){
  if(SPI2->SR & SPI_SR_RXNE){
  xpt2046Read.raw[xpt2046Read.rawCount] = (SPI2->DR >> 0x03);
  xpt2046Read.rawCount++;
   if(xpt2046Read.rawCount < xpt2046Read.count){
      SPI2->DR = xpt2046Send[xpt2046Read.rawCount];
    }else{
      SPI2->CR2 &= ~SPI_CR2_RXNEIE;
      XPT2046_CS_HIGHT;
      xpt2046.bat = xpt2046Read.raw[0x01] * 0.329;
      xpt2046.brg = xpt2046Read.raw[0x02] * 0.0245;
      if(0x04 < xpt2046Read.count){
        xpt2046Read.status = XPT2046_PRESSED;
        xpt2046Read.raw[0x03] = 0x0FFF - xpt2046Read.raw[0x03];
        xpt2046.x = xpt2046Read.raw[0x03] / settings.ax + settings.bx;
        xpt2046.y = xpt2046Read.raw[0x04] / settings.ay + settings.by;
        if(0x00 > xpt2046.x) xpt2046.x = 0x00;
        if(0x00 > xpt2046.y) xpt2046.y = 0x00;
        if(settings.maxX < xpt2046.x) xpt2046.x = settings.maxX;
        if(settings.maxY < xpt2046.y) xpt2046.y = settings.maxY;
      }else{
        xpt2046Read.status = XPT2046_RELEASED;
      }
    }
  }
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
  GuiSetWindow(0x00, 0x00, settings.maxX, settings.maxY);
  GuiFullWindow(WHITE);
  DelayMs(60);
  while(!xpt2046Read.status){}
  for(i = 0x00; i < 0x04; i++){
    GuiCalibDraw(drawX[i], drawY[i]);
    while(xpt2046Read.status){}
    while(!xpt2046Read.status){}
    touchX[i] = xpt2046Read.raw[0x03];
    touchY[i] = xpt2046Read.raw[0x04];
    GuiCalibErase(drawX[i], drawY[i]);
  }
  settings.ax = (touchX[0x02] - touchX[0x00]) / (drawX[0x02] - drawX[0x00]);
  settings.bx = drawX[0x00] - touchX[0x00] / settings.ax;
  settings.ay = (touchY[0x02] - touchY[0x00]) / (drawY[0x02] - drawY[0x00]);
  settings.by = drawY[0x00] - touchY[0x00] / settings.ay;
  
  GuiSetWindow(0x00, 0x00, settings.maxX, settings.maxY);
  GuiFullWindow(BLACK);
}

void Xpt2046Init(void){
  GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF15);
  GPIOB->CRH |= GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1;
  GPIOB->CRH |= GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15;
  
  xpt2046Read.status = XPT2046_RELEASED;
  
  XPT2046_CS_HIGHT;

  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  SPI2->CR1 |= SPI_CR1_BR_2;
  SPI2->CR1 &= ~SPI_CR1_CPOL;
  SPI2->CR1 &= ~SPI_CR1_CPHA;
  SPI2->CR1 &= ~SPI_CR1_DFF;
  SPI2->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI2->CR1 |= SPI_CR1_DFF;
  SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI2->CR1 |= SPI_CR1_MSTR;
  SPI2->CR1 |= SPI_CR1_SPE;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
  TIM6->PSC = 0x1F3F; // 7999 80000000:8000=10000Hz
//  TIM6->ARR = 0xC7; // 50Hz
  TIM6->ARR = 0x63; // 100Hz
  TIM6->SR = 0x00;
  TIM6->DIER |= TIM_DIER_UIE;
  TIM6->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  NVIC_SetPriority(SPI2_IRQn, PRIORITY_XPT2046_LIGHT);
  NVIC_EnableIRQ(SPI2_IRQn);
  
  NVIC_SetPriority(TIM6_IRQn, PRIORITY_XPT2046_LIGHT);
  NVIC_EnableIRQ(TIM6_IRQn);
}
