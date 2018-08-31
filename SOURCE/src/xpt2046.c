#include "xpt2046.h"

struct xpt2046InitTypeDef xpt2046;

uint8_t Xpt2046Read(uint8_t a){
  uint8_t i;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = a;
  while(!(SPI2->SR & SPI_SR_RXNE));
  i = SPI2->DR;
  return i;
}

uint16_t Xpt2046GetTouch(uint8_t adress){
  uint16_t data = 0x00;
  XPT2046_CS_LOW;
  Xpt2046Read(adress);
  Xpt2046Read(0X00);
  Xpt2046Read(0X00);
  Xpt2046Read(adress);
  data = Xpt2046Read(0X00);
  data <<= 0x08;
  data |= Xpt2046Read(0X00);
  data >>= 0x03;
  XPT2046_CS_HIGHT;
  return data;
}

void Xpt2046TouchXY(void){
  uint16_t a, b, pressure;
  a = Xpt2046GetTouch(0xB0);
  b = 0x0FFF - Xpt2046GetTouch(0xC0);
  pressure = a + b;
  if(pressure > 0x0200){
    a = Xpt2046GetTouch(XPT2046_A);
    b = Xpt2046GetTouch(XPT2046_B);
    if((a >= 0x0A) && (b >= 0x0A)){
      xpt2046.y = a;
      xpt2046.x = b;
    }
  }else{
    pressure = 0x00;
  }
}

void Xpt2046TouchBatBrg(void){
  xpt2046.bat = 0.0006103515625 * Xpt2046GetTouch(XPT2046_BAT);
  xpt2046.brg = Xpt2046GetTouch(XPT2046_BRG);
  xpt2046.t0 = Xpt2046GetTouch(0x84);
  xpt2046.t1 = Xpt2046GetTouch(0xF4);
}

void EXTI15_10_IRQHandler(void){
  if(EXTI->PR & EXTI_PR_PR11){
    Xpt2046TouchXY();
    EXTI->PR |= EXTI_PR_PR11;
  }
}

void Xpt2046Init(void){
  GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF15);
  GPIOB->CRH |= GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1;   
  GPIOB->CRH |= GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15;
  XPT2046_CS_HIGHT;

  AFIO->EXTICR [0x02] |= AFIO_EXTICR3_EXTI11_PB;
  EXTI->FTSR |= EXTI_FTSR_TR11;
  EXTI->IMR |= EXTI_IMR_MR11;
  
  NVIC_SetPriority(EXTI15_10_IRQn, PRIORITY_XPT2046);
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  SPI2->CR1 |= SPI_CR1_BR_2;
  SPI2->CR1 &= ~SPI_CR1_CPOL;
  SPI2->CR1 &= ~SPI_CR1_CPHA;
  SPI2->CR1 &= ~SPI_CR1_DFF;
  SPI2->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI2->CR1 |= SPI_CR1_MSTR;
  SPI2->CR1 |= SPI_CR1_SPE;
}
