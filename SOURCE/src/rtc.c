#include "rtc.h"

void rtcInit(void){
  if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN){
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
    PWR->CR |= PWR_CR_DBP;
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;
    RCC->BDCR |= RCC_BDCR_RTCEN;
    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
    RCC->BDCR |= RCC_BDCR_LSEON;
    while ((RCC->BDCR & RCC_BDCR_LSEON) != RCC_BDCR_LSEON){}
    BKP->RTCCR |= 0x00;
    while (!(RTC->CRL & RTC_CRL_RTOFF));
    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLL = 0x7FFF;
    RTC->CRL &= ~RTC_CRL_CNF;
    while (!(RTC->CRL & RTC_CRL_RTOFF));
    RTC->CRL &= (uint16_t)~RTC_CRL_RSF;
    while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF){}
    PWR->CR &= ~PWR_CR_DBP;
  }
}
 
uint32_t rtcGetCounter(void){
  return (uint32_t)((RTC->CNTH << 16) | RTC->CNTL);
}
 
void rtcSetCounter(uint32_t count){
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
  PWR->CR |= PWR_CR_DBP;
  while (!(RTC->CRL & RTC_CRL_RTOFF));
  RTC->CRL |= RTC_CRL_CNF;
  RTC->CNTH = count>>16;
  RTC->CNTL = count;
  RTC->CRL &= ~RTC_CRL_CNF;
  while (!(RTC->CRL & RTC_CRL_RTOFF));
  PWR->CR &= ~PWR_CR_DBP;
}

