#ifndef _XPT2046_H
#define _XPT2046_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Define --------------------------------------------------------------------*/
#define XPT2046_CS_LOW      GPIOB->BRR = GPIO_BRR_BR12
#define XPT2046_CS_HIGHT    GPIOB->BSRR = GPIO_BSRR_BS12

void Xpt2046Init(void);


#endif /* _XPT2046_H */
