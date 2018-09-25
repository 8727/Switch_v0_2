#include "rs485.h"

struct Rs485InitTypeDef rs485;
static const uint8_t preamble[] = {0x55, 0xAA, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void ImgUpdate(void){
  if(!strncmp((char*)rs485.rxBuff, (char*)preamble, 0x0A)){
    uint8_t c;
    uint16_t adrr;
    uint8_t send[1] = {0x01};
    uint8_t buff[0x0100];
    uint8_t write_check = 0x00;
    adrr = rs485.rxBuff[0x0A] << 0x08;
    adrr |= rs485.rxBuff[0x0B];
    for(uint16_t i = 0; i < 0x100; i++){
      c = rs485.rxBuff[0x0C + i];
      buff[i] = c;
      write_check ^= c;
    }
    c = rs485.rxBuff[0x010C];
    if(c != write_check){
      send[0] = 0x00;
      Rs485Sends((uint8_t*)send);
    }else{
      W25QxxWritePage(adrr, buff);
      Rs485Sends((uint8_t*)send);
    }
  }
  rs485.rxStop = 0x00;
}

void Rs485Sends(uint8_t *str){
  uint8_t i = 0x00;
  while(str[i]){
    rs485.txBuff[rs485.txStop++] = str[i++];
  }
  RX485_TX;
  USART1->CR1 |= USART_CR1_TXEIE;
}

void Rs485Init(void){
  GPIOA->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10);
  GPIOA->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9 | GPIO_CRH_CNF10_0);
  RX485_RX;
  
  RCC->APB2ENR|= RCC_APB2ENR_USART1EN;
  
  USART1->BRR = settings.rs485Speed;
  
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
  USART1->CR1 |= USART_CR1_RXNEIE;
//  USART1->CR1 |= USART_CR1_IDLEIE;
  USART1->CR1 |= USART_CR1_TCIE;
  USART1->CR1 |= USART_CR1_UE;
  
  NVIC_SetPriority(USART1_IRQn, PRIORITY_RS485);
  NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void){
  if(USART1->SR & USART_SR_RXNE){
    USART1->SR &= ~USART_SR_RXNE;
    rs485.rxBuff[rs485.rxStop++] = USART1->DR;
    if(0x010C < rs485.rxStop){
      ImgUpdate();
    }
  }
  if(USART1->SR & USART_SR_TXE){
    if (rs485.txStart != rs485.txStop){
      USART1->DR = rs485.txBuff[rs485.txStart++];
    }else{
      USART1->CR1 &= ~USART_CR1_TXEIE;
    }
  }
  if(USART1->SR & USART_SR_TC){
    USART1->SR &= ~USART_SR_TC;
    RX485_RX;
  }
}
