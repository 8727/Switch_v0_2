#include "xpt2046.h"

void Xpt2046ReadAdc(uint8_t a, uint8_t b, uint16_t *data_a,  uint16_t *data_b){
  XPT2046_CS_LOW;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = a;
  while(!(SPI2->SR & SPI_SR_RXNE));
  (void) SPI2->DR;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = 0x00;
  while(!(SPI2->SR & SPI_SR_RXNE));
  *data_a = SPI2->DR;
  *data_a <<= 0x08;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = b;
  while(!(SPI2->SR & SPI_SR_RXNE));
  *data_a |= SPI2->DR;
  *data_a >>= 0x03;
  *data_a &= 0x0FFF;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = 0x00;
  while(!(SPI2->SR & SPI_SR_RXNE));
  *data_b = SPI2->DR;
  *data_b <<= 0x08;
  while(!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = b;
  while(!(SPI2->SR & SPI_SR_RXNE));
  *data_b |= SPI2->DR;
  *data_b >>= 0x03;
  *data_b &= 0x0FFF;
  XPT2046_CS_HIGHT;
}

void Xpt2046Init(void){
  GPIOB->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF15);  
  GPIOB->CRH |= GPIO_CRH_CNF11_1 | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1;   
  GPIOB->CRH |= GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15;  //GPIO SPI XPT2046
 // GPIOB->BSRR |= GPIO_BSRR_BS11;  //GPIO SPI XPT2046
  XPT2046_CS_HIGHT;
  
  AFIO->EXTICR [2] |= AFIO_EXTICR3_EXTI11_PB;
  EXTI->FTSR |= EXTI_FTSR_TR11;
  EXTI->IMR |= EXTI_IMR_MR11;
  NVIC_SetPriority(EXTI15_10_IRQn, 2); // IRQ Seting XPT2046
  NVIC_EnableIRQ(EXTI15_10_IRQn); // IRQ XPT2046

  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
//  SPI2->CR1 &= ~SPI_CR1_BR;
  SPI2->CR1 |= SPI_CR1_BR_2;
//  SPI2->CR1 |= SPI_CR1_BR;
  SPI2->CR1 &= ~SPI_CR1_CPOL;
  SPI2->CR1 &= ~SPI_CR1_CPHA;
  SPI2->CR1 &= ~SPI_CR1_DFF;
  SPI2->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI2->CR1 |= SPI_CR1_MSTR;
  SPI2->CR1 |= SPI_CR1_SPE; // SPI XPT2046
}
