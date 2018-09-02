#include "xpt2046.h"

struct xpt2046ReadInitTypeDef xpt2046Read;
struct xpt2046InitTypeDef xpt2046;

static uint8_t xpt2046SendADC[0x04] = {XPT2046_T0, XPT2046_T1, XPT2046_BAT, XPT2046_BRG};
static uint8_t xpt2046SendXYZ[0x04] = {XPT2046_Z1, XPT2046_Z2, XPT2046_A, XPT2046_B};

void Xpt2046GetTouch(uint8_t type){
  if(0x00 < type){
    memcpy (xpt2046Read.send, xpt2046SendXYZ, 0x04);
  }else{
    memcpy (xpt2046Read.send, xpt2046SendADC, 0x04);
  }
  XPT2046_CS_LOW;
  xpt2046Read.reads = 0x00;
  SPI2->CR2 |= SPI_CR2_RXNEIE;
  SPI2->DR = xpt2046Read.send[0x00];
}

void SPI2_IRQHandler(void){
  if(SPI2->SR & SPI_SR_RXNE){
    xpt2046Read.read[xpt2046Read.sendStep] = SPI2->DR;
    xpt2046Read.sendStep++;
    if(0x03 > xpt2046Read.sendStep){
      SPI2->DR = 0x00;
    }else{
      xpt2046Read.readSend[xpt2046Read.sendCoun] = (((xpt2046Read.read[0x01] << 0x08) | xpt2046Read.read[0x02]) >> 0x03);
      xpt2046Read.sendStep = 0x00;
      xpt2046Read.sendCoun++;
      if(0x04 > xpt2046Read.sendCoun){
        SPI2->DR = xpt2046Read.send[xpt2046Read.sendCoun];
      }else{
        xpt2046Read.sendCoun = 0x00;
        SPI2->CR2 &= ~SPI_CR2_RXNEIE;
        XPT2046_CS_HIGHT;
        if(XPT2046_T0 == xpt2046Read.send[0x00]){
          xpt2046.t0 = xpt2046Read.readSend[0x00];
          xpt2046.t1 = xpt2046Read.readSend[0x01];
          xpt2046.bat = xpt2046Read.readSend[0x02] * 0.329;
          xpt2046.brg = xpt2046Read.readSend[0x03] * 0.0245;
        }else{
          xpt2046.z1 = xpt2046Read.readSend[0x00];
          xpt2046.z2 = xpt2046Read.readSend[0x01];
          xpt2046.y = xpt2046Read.readSend[0x02];
          xpt2046.x = xpt2046Read.readSend[0x03];
        }
        xpt2046Read.reads = 0x01;
      }
    }
  }
}


void EXTI15_10_IRQHandler(void){
  if(EXTI->PR & EXTI_PR_PR11){
    if(0x01 == xpt2046Read.reads) Xpt2046GetTouch(0x01);
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
  
  NVIC_SetPriority(SPI2_IRQn, PRIORITY_XPT2046);
  NVIC_EnableIRQ(SPI2_IRQn);

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
}
