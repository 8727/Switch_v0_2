#include "xpt2046.h"

struct xpt2046InitTypeDef xpt2046;
struct xpt2046ReadInitTypeDef xpt2046Read;

static uint16_t xpt2046Send[0x07] = {XPT2046_BAT, XPT2046_BRG, XPT2046_A, XPT2046_B, XPT2046_Z1, XPT2046_Z2, XPT2046_PRESSED};

void TIM6_IRQHandler(void){
  TIM6->SR = 0x00;
  xpt2046Read.status = XPT2046_RELEASED;
  if(GPIOB->IDR & GPIO_IDR_IDR11){
    xpt2046Read.status = XPT2046_RELEASED;
    xpt2046Read.count = 0x03;
  }else{
    xpt2046Read.status = XPT2046_PRESSED;
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
      xpt2046.x = xpt2046Read.raw[0x03];
      xpt2046.y = xpt2046Read.raw[0x04];
      xpt2046.z1 = xpt2046Read.raw[0x05];
      xpt2046.z2 = xpt2046Read.raw[0x06];
    }
      
      
      
    }
  }
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
  
  NVIC_SetPriority(SPI2_IRQn, PRIORITY_XPT2046);
  NVIC_EnableIRQ(SPI2_IRQn);
  
  NVIC_SetPriority(TIM6_IRQn, PRIORITY_XPT2046);
  NVIC_EnableIRQ(TIM6_IRQn);
}
