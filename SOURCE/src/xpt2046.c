#include "xpt2046.h"

struct xpt2046InitTypeDef xpt2046;
struct xpt2046ReadInitTypeDef xpt2046Read;

static uint16_t xpt2046Send[0x07] = {XPT2046_BAT, XPT2046_BRG, XPT2046_A, XPT2046_B, XPT2046_Z1, XPT2046_Z2, XPT2046_PRESSED};

void TIM6_IRQHandler(void){
  TIM6->SR = 0x00;
  if(GPIOB->IDR & GPIO_IDR_IDR11){
    xpt2046Read.count = 0x03;
  }else{
    xpt2046Read.count = 0x07;
  }
  XPT2046_CS_LOW;
  xpt2046Read.rawCount = 0x00;
  SPI2->CR2 |= SPI_CR2_RXNEIE;
  SPI2->DR = xpt2046Send[xpt2046Read.rawCount];
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
      if(0x06 < xpt2046Read.count){
        xpt2046Read.status = XPT2046_PRESSED;
        xpt2046.x = xpt2046Read.raw[0x03];
        xpt2046.y = xpt2046Read.raw[0x04];
        xpt2046.z1 = xpt2046Read.raw[0x05];
        xpt2046.z2 = xpt2046Read.raw[0x06];
        xpt2046.x = (uint16_t)((settings.ax * xpt2046Read.raw[0x03] + settings.bx * xpt2046Read.raw[0x04] + settings.cx) / TOUCH_FACTOR);
        xpt2046.y = (uint16_t)((settings.ay * xpt2046Read.raw[0x03] + settings.by * xpt2046Read.raw[0x04] + settings.cy) / TOUCH_FACTOR);
      }else{
        xpt2046Read.status = XPT2046_RELEASED;
      }
    }
  }
}

void Xpt2046Calibration(void){
  uint16_t drawX[0x05];
  uint16_t drawY[0x05];
  uint16_t touchX[0x05];
  uint16_t touchY[0x05];
  double ka[0x03], kb[0x03], kc[0x03], kd[0x03];
  double d, dx1, dx2, dx3, dy1, dy2, dy3;
  uint8_t i;
  uint16_t z;
  // Устанавливаем координаты пяти точек, по которым будет производится калибровка экрана
  drawX[0x00] = (settings.maxX / 0x02);
  z = (settings.maxX / 0x05);
  drawX[0x01] = 0x01 * z;
  drawX[0x02] = 0x04 * z;
  drawX[0x03] = 0x04 * z;
  drawX[0x04] = 0x01 * z;
  drawY[0x00] = (settings.maxY / 0x02);
  z = (settings.maxY / 0x05);
  drawY[0x01] = 0x01 * z;
  drawY[0x02] = 0x01 * z;
  drawY[0x03] = 0x04 * z;
  drawY[0x04] = 0x04 * z;
   // Поочередно выводим перекрестье для каждой из пяти точек и считываем координаты нажатия
  DelayMs(200);
  while(!xpt2046Read.status){} // Ожидаем отпускания нажатия на сенсорный экран
  TIM2->CCR1 = 0xFF;
  for(i = 0x00; i < 0x05; i++){
    GuiCalibDraw(drawX[i], drawY[i]); // Выводим перекрестье
    while(xpt2046Read.status){} // Ожидаем нажатия на сенсорный экран
    touchX[i] = xpt2046Read.raw[0x03];
    touchY[i] = xpt2046Read.raw[0x04];
    while(!xpt2046Read.status){} // Ожидаем отпускания нажатия на сенсорный экран
    GuiCalibErase(drawX[i], drawY[i]); // Выводим перекрестье
  }
  for(i = 0x00; i < 0x03; i++){  // Производим расчёты
    ka[i] = 0x00; 
    kb[i] = 0x00; 
    kc[i] = 0x00; 
    kd[i] = 0x00; 
  }
  for(i = 0x00; i < 0x05; i++){
    ka[0x02] = ka[0x02] + (double)(touchX[i]); 
    kb[0x02] = kb[0x02] + (double)(touchY[i]); 
    kc[0x02] = kc[0x02] + (double)(drawX[i]); 
    kd[0x02] = kd[0x02] + (double)(drawY[i]); 
    ka[0x00] = ka[0x00] + (double)(touchX[i]) * (double)(touchX[i]); 
    ka[0x01] = ka[0x01] + (double)(touchX[i]) * (double)(touchY[i]); 
    kb[0x00] = ka[0x01]; 
    kb[0x01] = kb[0x01] + (double)(touchY[i]) * (double)(touchY[i]); 
    kc[0x00] = kc[0x00] + (double)(touchX[i]) * (double)(drawX[i]); 
    kc[0x01] = kc[0x01] + (double)(touchY[i]) * (double)(drawX[i]); 
    kd[0x00] = kd[0x00] + (double)(touchX[i]) * (double)(drawY[i]); 
    kd[0x01] = kd[0x01] + (double)(touchY[i]) * (double)(drawY[i]); 
  } 
  ka[0x00] = ka[0x00] / ka[0x02]; 
  ka[0x01] = ka[0x01] / kb[0x02]; 
  kb[0x00] = kb[0x00] / ka[0x02]; 
  kb[0x01] = kb[0x01] / kb[0x02]; 
  kc[0x00] = kc[0x00] / ka[0x02]; 
  kc[0x01] = kc[0x01] / kb[0x02]; 
  kd[0x00] = kd[0x00] / ka[0x02]; 
  kd[0x01] = kd[0x01] / kb[0x02]; 
  ka[0x02] = ka[0x02] / 0x05; 
  kb[0x02] = kb[0x02] / 0x05; 
  kc[0x02] = kc[0x02] / 0x05; 
  kd[0x02] = kd[0x02] / 0x05; 
  d = (ka[0x00] - ka[0x02]) * (kb[0x01] - kb[0x02]) - (ka[0x01] - ka[0x02]) * (kb[0x00] - kb[0x02]); 
  dx1 = ((kc[0x00] - kc[0x02]) * (kb[0x01] - kb[0x02]) - (kc[0x01] - kc[0x02]) * (kb[0x00] - kb[0x02])) / d; 
  dx2 = ((kc[0x01] - kc[0x02]) * (ka[0x00] - ka[0x02]) - (kc[0x00] - kc[0x02]) * (ka[0x01] - ka[0x02])) / d; 
  dx3 = (kb[0x00] * (ka[0x02] * kc[0x01] - ka[0x01] * kc[0x02]) + kb[0x01] * (ka[0x00] * kc[0x02] - ka[0x02] * kc[0x00]) + kb[0x02] * (ka[0x01] * kc[0x00] - ka[0x00] * kc[0x01])) / d; 
  dy1 = ((kd[0x00] - kd[0x02]) * (kb[0x01] - kb[0x02]) - (kd[0x01] - kd[0x02]) * (kb[0x00] - kb[0x02])) / d; 
  dy2 = ((kd[0x01] - kd[0x02]) * (ka[0x00] - ka[0x02]) - (kd[0x00] - kd[0x02]) * (ka[0x01] - ka[0x02])) / d; 
  dy3 = (kb[0x00] * (ka[0x02] * kd[0x01] - ka[0x01] * kd[0x02]) + kb[0x01] * (ka[0x00] * kd[0x02] - ka[0x02] * kd[0x00]) + kb[0x02] * (ka[0x01] * kd[0x00] - ka[0x00] * kd[0x01])) / d;
  // Сохраняем коэффициенты
  settings.ax = (uint32_t)(dx1 * TOUCH_FACTOR);
  settings.bx = (uint32_t)(dx2 * TOUCH_FACTOR);
  settings.cx = (uint32_t)(dx3 * TOUCH_FACTOR);
  settings.ay = (uint32_t)(dy1 * TOUCH_FACTOR);
  settings.by = (uint32_t)(dy2 * TOUCH_FACTOR);
  settings.cy = (uint32_t)(dy3 * TOUCH_FACTOR);
  TIM2->CCR1 = 0x20;
}

void Xpt2046Init(void){
  GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF15);
  GPIOB->CRH |= GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1;
  GPIOB->CRH |= GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15;
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
  TIM6->PSC = 0x1F3F; // 7999
  TIM6->ARR =  0x01F3; // 499
  TIM4->SR = 0x00;
  TIM6->DIER |= TIM_DIER_UIE;
  TIM6->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
  
  xpt2046Read.status = XPT2046_RELEASED;
  
  NVIC_SetPriority(SPI2_IRQn, PRIORITY_XPT2046);
  NVIC_EnableIRQ(SPI2_IRQn);
  
  NVIC_SetPriority(TIM6_IRQn, PRIORITY_XPT2046);
  NVIC_EnableIRQ(TIM6_IRQn);
}
