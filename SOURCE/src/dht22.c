#include "dht22.h"

struct Dht22InitTypeDef dht22;

void Dht22Init(void){
  dht22.timDelayBuff[0x00] = 0xC288; //49800;
  dht22.timDelayBuff[0x01] = 0xffff;
  dht22.timDelayBuff[0x02] = 0xffff;
  dht22.timDelayBuff[0x03] = 0xffff;
  
  GPIOA->CRH |= GPIO_CRH_MODE15;
  GPIOA->CRH |= GPIO_CRH_CNF15;
  
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel2->CCR = 0x00;
  DMA1_Channel2->CMAR = (uint32_t) &dht22.timDelayBuff[0x00];
  DMA1_Channel2->CPAR = (uint32_t) &TIM2->CCR1;
  DMA1_Channel2->CNDTR = sizeof(dht22.timDelayBuff)/sizeof(*dht22.timDelayBuff);
  DMA1_Channel2->CCR = DMA_CCR2_PL | DMA_CCR2_MSIZE_0 | DMA_CCR2_PSIZE_1 | DMA_CCR2_MINC | DMA_CCR2_CIRC | DMA_CCR2_DIR;
  DMA1->IFCR = DMA_IFCR_CGIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CTCIF2 | DMA_IFCR_CTEIF2;
  DMA1_Channel2->CCR |= DMA_CCR5_EN;
  
  DMA1_Channel7->CCR = 0x00;
  DMA1_Channel7->CMAR = (uint32_t) &dht22.dataBuff[0x00];
  DMA1_Channel7->CPAR = (uint32_t) &TIM2->CCR2;
  DMA1_Channel7->CNDTR = sizeof(dht22.dataBuff)/sizeof(*dht22.dataBuff);
  DMA1_Channel7->CCR = DMA_CCR7_PL | DMA_CCR7_MSIZE_0 | DMA_CCR7_PSIZE_1 | DMA_CCR7_MINC | DMA_CCR7_TCIE;
  DMA1->IFCR = DMA_IFCR_CGIF7 | DMA_IFCR_CHTIF7 | DMA_IFCR_CTCIF7 | DMA_IFCR_CTEIF7;
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 0x031F; //799  настраиваем на период 10 мкс и переполнение на 100мс
  TIM2->ARR = 0xC34F; //49999
  TIM2->SR = 0x00;
  TIM2->DIER = TIM_DIER_UDE | TIM_DIER_CC1IE | TIM_DIER_CC2DE;
  TIM2->CCMR1 = TIM_CCMR1_OC1M | TIM_CCMR1_CC2S_1;
  TIM2->CR1 = TIM_CR1_ARPE;
  TIM2->EGR = TIM_EGR_UG;
  TIM2->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC2P;
  TIM2->CR1 |= TIM_CR1_CEN;
  
  NVIC_SetPriority(TIM2_IRQn, PRIORITY_DHT22_TIM);
  NVIC_EnableIRQ(TIM2_IRQn);
  
  NVIC_SetPriority(DMA1_Channel7_IRQn, PRIORITY_DHT22_DMA);
  NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void TIM2_IRQHandler(void){
  if(TIM2->SR & TIM_SR_CC1IF){
   DMA1_Channel7->CCR &= ~DMA_CCR7_EN;
   DMA1_Channel7->CMAR = (uint32_t) &dht22.dataBuff[0x00];
   DMA1_Channel7->CNDTR = sizeof(dht22.dataBuff)/sizeof(*dht22.dataBuff);
   DMA1_Channel7->CCR |= DMA_CCR7_EN;
   TIM2->CCER |= TIM_CCER_CC2E;
   TIM2->SR &= ~TIM_SR_CC1IF;
  }
}

void DMA1_Channel7_IRQHandler(void){
  uint64_t tData = 0x00;
  uint8_t *tDataBuff = (uint8_t*)&tData;
  uint8_t a;
  uint32_t *b;
  uint8_t tCRC = 0x00;
  
  if(DMA1->ISR & DMA_ISR_TCIF7){ // приняли данные от DHT22
    TIM2->CCER &= ~TIM_CCER_CC2E;
    b = BITBAND_RAMADR((uint32_t)&tData, 0x00);
    for(a = 0x00; a < 0x28; a++) if((dht22.dataBuff[a + 0x02] - dht22.dataBuff[a + 0x01]) > 0x0A) b[0x27 - a] = 0x01;
      else b[0x27 - a] = 0x00;
    tCRC = 0x00;
    for(a = 0x04; a > 0x00; a--) tCRC = tCRC + tDataBuff[a]; // подсчет CRC
    if(tCRC == tDataBuff[0x00]){
      dht22.humidity = (uint16_t)(tData >> 0x18);
      dht22.temperature = (uint16_t)(tData >> 0x08);
    }else{
    }
    DMA1->IFCR = DMA_IFCR_CTCIF7;
  }
}
