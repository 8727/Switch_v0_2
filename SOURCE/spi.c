void SPI_init(void)
{

// SPI2 init
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // такты на SPI2

        GPIOB->CRH &= (~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF15_0)); //настройка выводов для SPI
        GPIOB->CRH |= (GPIO_CRH_MODE13 | GPIO_CRH_MODE15                
         | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1);                // AF Push-Pull out

        SPI2->CR1 |= SPI_CR1_BR_1    //настройка делителя 
//       | SPI_CR1_BR_0    //комментируя или раскомментируя эти строки можно изменять значение делителя 
//       | SPI_CR1_BR_2
         | SPI_CR1_SSM   // программное управление слэйвом
         | SPI_CR1_SSI   // при программном управлении слэйвом данный бит должен быть 1
         | SPI_CR1_MSTR  // Режим мастера
         | SPI_CR1_SPE;   // разрешить SPI2 

        SPI2->CR2 |= SPI_CR2_RXDMAEN // разрешить передачу принятых данных через DMA
         | SPI_CR2_TXDMAEN; // Разрешить принимать данные для передачи через DMA

        return;
}

RCC->AHBENR |= RCC_AHBENR_DMA1EN; //такты

        // 4 канал DMA1  RX SPI2 -> память
        DMA1_Channel4->CPAR = SPI2_BASE+0x0C; // адрес DR
        DMA1_Channel4->CMAR = pSPI2_readbuf_addr->Buf_Addr; //адрес буфера приема
        DMA1_Channel4->CNDTR = 12; // размер транзакции
        DMA1_Channel4->CCR |= DMA_CCR4_MINC     //инкремент адреса буфера
        | DMA_CCR4_PSIZE_0;     //размерность данных SPI 16 бит (происходит автоматическое преобразование 16->8 бит)

        // 5 канал DMA1  память -> TX SPI2
        DMA1_Channel5->CPAR = SPI2_BASE+0x0C; //адрес DR
        DMA1_Channel5->CMAR = pSPI2_writebuf_addr->Buf_Addr; //адрес буфера передачи
        DMA1_Channel5->CNDTR = 12; // размер транзакции
        DMA1_Channel5->CCR |= DMA_CCR5_MINC     //инкремент адреса буфера
        | DMA_CCR5_DIR; // направление передачи из памяти в периферию


void GuiColorImg (uint16_t x, uint16_t y, uint8_t img){
  SPI1->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
        DMA1_Channel4->CPAR = SPI2_BASE+0x0C; //DR Base 
        DMA1_Channel4->CMAR = pSPI2_readbuf_addr->Buf_Addr;
        DMA1_Channel4->CNDTR = 12;
        DMA1_Channel5->CPAR = SPI2_BASE+0x0C; //DR Base
        DMA1_Channel5->CMAR = pSPI2_writebuf_addr->Buf_Addr;
        DMA1_Channel5->CNDTR = 12;
        DMA1_Channel4->CCR |= DMA_CCR4_TCIE | DMA_CCR4_EN;
        DMA1_Channel5->CCR |= DMA_CCR5_EN;
        return;
}

void DMAChannel4_IRQHandler     (void)
{
        DMA1_Channel4->CCR &= (~(DMA_CCR4_TCIE | DMA_CCR4_EN)); // выключаем канал приемника и его прерывание
        DMA1->IFCR |= DMA_IFCR_CTCIF4 | DMA_IFCR_CGIF4; //очищаем флаги
        DMA1_Channel5->CCR &= (~(DMA_CCR5_EN)); // выключаем канал передатчика
        pCurrent_mode->stat.led_data_send = 0; // выставляем нужные флаги для дальнейшей работы с данными 
        HC595_OUT;
        pCurrent_mode->stat.read_key_data_ready=1;
        return;
}
