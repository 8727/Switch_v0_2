#include "rtc.h"

void RTC_IRQHandler (void)
{
  if(RTC->CRL & RTC_CRL_SECF)
  {
    RTC->CRL &= ~RTC_CRL_SECF;
//    gui_mode.time_wait++;
//    if(0xC8 < gui_mode.time_wait) gui_mode.time_wait = 0xC8; // 200 sec
//    if(gui_mode.wait < gui_mode.time_wait) Gui_Modes();
  }
//  if(RTC->CRL & RTC_CRL_ALRF)
//  {
//    RTC->CRL &= ~RTC_CRL_ALRF;
//    
//    
//  }
//  if(RTC->CRL & RTC_CRL_OWF)
//  {
//    RTC->CRL &= ~RTC_CRL_OWF;
//    
//    
//  }
}
 
void RtcInit(void){
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
  RTC->CRH |= RTC_CRH_SECIE;
  RTC->CRH |= RTC_CRH_ALRIE;
  RTC->CRH |= RTC_CRH_OWIE;
  NVIC_EnableIRQ (RTC_IRQn);
}
 
uint32_t RtcGetCounter(void){
  return (uint32_t)((RTC->CNTH << 16) | RTC->CNTL);
}
 
void RtcSetCounter(uint32_t count){
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

