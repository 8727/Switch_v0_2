#include "xpt2046.h"

struct xpt2046InitTypeDef xpt2046;
struct xpt2046ReadInitTypeDef xpt2046Read;

static uint16_t xpt2046Send[0x08] = {XPT2046_T0, XPT2046_T1, XPT2046_BAT, XPT2046_BRG, XPT2046_A, XPT2046_B, XPT2046_Z1, XPT2046_Z2};

void Xpt2046GetTouch(void){
  xpt2046Read.status = TOUCH_RELEASED; // Зачитать прикосновение
  XPT2046_CS_LOW;
  xpt2046Read.rawCount = 0x00;
  SPI2->CR2 |= SPI_CR2_RXNEIE;
  SPI2->DR = xpt2046Send[xpt2046Read.rawCount];
  if(GPIOB->IDR & GPIO_IDR_IDR11){
    xpt2046Read.status = TOUCH_RELEASED;
    xpt2046Read.count = 0x05;
  }else{
    xpt2046Read.status = TOUCH_PRESSED;
    xpt2046Read.count = 0x09;
  }
}

void SPI2_IRQHandler(void){
  if(SPI2->SR & SPI_SR_RXNE){
  xpt2046Read.raw[xpt2046Read.rawCount] = SPI2->DR;
  xpt2046Read.rawCount++;
  if(xpt2046Read.rawCount < xpt2046Read.count){
    SPI2->DR = xpt2046Send[xpt2046Read.rawCount];
  }else{
    SPI2->CR2 &= ~SPI_CR2_RXNEIE;
    XPT2046_CS_HIGHT;
    xpt2046Read.rawCount = 0x00;
    xpt2046.t0 = xpt2046Read.raw[0x01];
    xpt2046.t1 = xpt2046Read.raw[0x02];
    xpt2046.bat = xpt2046Read.raw[0x03] * 0.329;
    xpt2046.brg = xpt2046Read.raw[0x04] * 0.0245;
    if(0x06 < xpt2046Read.count){
      xpt2046.x = xpt2046Read.raw[0x05];
      xpt2046.y = xpt2046Read.raw[0x06];
      xpt2046.z1 = xpt2046Read.raw[0x07];
      xpt2046.z2 = xpt2046Read.raw[0x08];
    }
//    P_Touch_CalcPos(); // Преобразование данных тачскрина в координаты на дисплее
//    if(P_Touch_CheckPos() == SUCCESS) xpt2046Read.status = TOUCH_PRESSED;// Проверка действительности данных
      
      
      
    }
  }
}

void Xpt2046Init(void){
  GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF15);
  GPIOB->CRH |= GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1;   
  GPIOB->CRH |= GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15;
  XPT2046_CS_HIGHT;
  
  NVIC_SetPriority(SPI2_IRQn, PRIORITY_XPT2046);
  NVIC_EnableIRQ(SPI2_IRQn);

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
}
