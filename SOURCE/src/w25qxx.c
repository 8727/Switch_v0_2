#include "w25qxx.h"

void W25QxxInit(void){
  
  GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
  GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;
  GPIOA->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7; // GPIO SPI Flash
  
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  SPI1->CR1 &= ~SPI_CR1_BR;
  SPI1->CR1 &= ~SPI_CR1_CPOL;
  SPI1->CR1 &= ~SPI_CR1_CPHA;
  SPI1->CR1 &= ~SPI_CR1_DFF;
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  SPI1->CR1 |= SPI_CR1_MSTR;
  SPI1->CR1 |= SPI_CR1_SPE; // SPI Flash
  
//  w25qxx.id = 0x00;
//  w25qxx.conf_img = 0x0000;
//  w25qxx.conf_sistem = 0x0000;
//  w25qxx.name = "XXXXXXXXX";
//  
//  W25Qxx_CS_LOW;
//  W25Qxx_W_R(CMD_ON_RESET);
//  W25Qxx_CS_HIGHT;
// 
//  W25Qxx_CS_LOW;
//  W25Qxx_W_R(CMD_RESET);
//  W25Qxx_CS_HIGHT;

//  W25Qxx_CS_LOW;
//  W25Qxx_W_R(CMD_JEDEC_ID);
//  W25Qxx_W_R(0x00);
//  W25Qxx_W_R(0x00);
//  w25qxx.id = W25Qxx_W_R(0x00);
//  W25Qxx_CS_HIGHT;
//  
//  for(uint8_t i = 0x00; i < 0x06; i++)
//  {
//    if (W25Qxx_device[i].id == w25qxx.id)
//    {
//      w25qxx.conf_img = W25Qxx_device[i].conf_img;
//      w25qxx.conf_sistem = W25Qxx_device[i].conf_sistem;
//      w25qxx.name = W25Qxx_device[i].name;
//    }
//  }
}
