#ifndef _RS485_H
#define _RS485_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"


/* Define --------------------------------------------------------------------*/
#define SIZE_BUFF_RS485                     0x0100

#define RX485_TX                            GPIOA->BSRR = GPIO_BSRR_BS8
#define RX485_RX                            GPIOA->BSRR = GPIO_BSRR_BR8

struct Rs485InitTypeDef{
  uint8_t rxBuff[0x010D];
  uint8_t txBuff[SIZE_BUFF_RS485];
  uint16_t rxStart;
  uint16_t rxStop;
  uint8_t txStart;
  uint8_t txStop;
};

extern struct Rs485InitTypeDef rs485;

void Rs485Sends(uint8_t *str);
void Rs485Init(void);

#endif /* _RS485_H */
