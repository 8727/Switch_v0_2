#include "rtc.h"

struct RtcInitTypeDef rtc;

void RTC_IRQHandler(void){
  if(RTC->CRL & RTC_CRL_SECF)
  {
    RTC->CRL &= ~RTC_CRL_SECF;
//    RtcCounterToTime(RtcGetCounter());
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



void RtcCounterToTime(uint32_t counter){
  uint32_t a, t;
  char b, c, d;
  t = counter % SEC_A_DAY;
  a = ((counter + 43200) / (SEC_A_DAY >> 1)) + (2440587 << 1) + 1;
  a >>= 1;
  rtc.wday = a % 7;
  a += 32044;
  b = (4 * a + 3) / 146097;
  a = a - (146097 * b) / 4;
  c = (4 * a + 3) / 1461;
  a = a - (1461 * c) / 4;
  d = (5 * a + 2) / 153;
  rtc.day = a - (153 * d + 2) / 5 + 1;
  rtc.month = d + 3 - 12 * (d / 10);
  rtc.year = 100 * b + c - 4800 + (d / 10);
  rtc.hour = t / 3600;
  rtc.min = (t % 3600) / 60;
  rtc.sec = (t % 3600) % 60;
}

uint32_t RtcTimeToCounter(void){
  uint8_t a, m;
  uint16_t y;
  a = ((14 - rtc.month) / 12);
  y = rtc.year + 4800 - a;
  m = rtc.month + (12 * a) - 3;
  return (((rtc.day + ((153 * m + 2) / 5) + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045) - 2440588) * 
          SEC_A_DAY) + rtc.sec + rtc.min * 60 + rtc.hour * 3600;
}

uint32_t RtcGetCounter(void){
  return (uint32_t)((RTC->CNTH << 0x10) | RTC->CNTL);
}

void RtcSetCounter(uint32_t counter){
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
  PWR->CR |= PWR_CR_DBP;
  while (!(RTC->CRL & RTC_CRL_RTOFF));
  RTC->CRL |= RTC_CRL_CNF;
  RTC->CNTH = counter >> 16;
  RTC->CNTL = counter;
  RTC->CRL &= ~RTC_CRL_CNF;
  while (!(RTC->CRL & RTC_CRL_RTOFF));
  PWR->CR &= ~PWR_CR_DBP;
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
    BKP->RTCCR |= settings.calibration;
    while (!(RTC->CRL & RTC_CRL_RTOFF));
    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLL = 0x7FFF;
//    BKP->RTCCR |= BKP_RTCCR_CCO;
    RTC->CNTH = settings.date >> 0x10;
    RTC->CNTL = settings.date;
    RTC->CRL &= ~RTC_CRL_CNF;
    while (!(RTC->CRL & RTC_CRL_RTOFF));
    RTC->CRL &= (uint16_t) ~RTC_CRL_RSF;
    while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF){}
    PWR->CR &= ~PWR_CR_DBP;
  }
  RTC->CRH |= RTC_CRH_SECIE;
//  RTC->CRH |= RTC_CRH_ALRIE;
//  RTC->CRH |= RTC_CRH_OWIE;
  NVIC_SetPriority(RTC_IRQn, PRIORITY_RTC);
  NVIC_EnableIRQ (RTC_IRQn);
}
