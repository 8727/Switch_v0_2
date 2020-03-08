#ifndef _FLOOR_H
#define _FLOOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"


/* Define --------------------------------------------------------------------*/
#define RELAY_POWER_ON          GPIOA->BSRR = GPIO_BSRR_BR0
#define RELAY_POWER_OFF         GPIOA->BSRR = GPIO_BSRR_BS0

#define SIMISTOR_POWER_ON       GPIOA->BSRR = GPIO_BSRR_BR1
#define SIMISTOR_POWER_OFF      GPIOA->BSRR = GPIO_BSRR_BS1

extern uint8_t flor;
extern uint8_t florJob;
extern uint8_t florTemperature;

void Adjustment(void);

#endif /* _FLOOR_H */
