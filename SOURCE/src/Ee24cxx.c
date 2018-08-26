#include "Ee24cxx.h"

 void Ee24cxxReadConfig(void){
  uint8_t tempReadBuff[0x30];
  uint8_t tempWriteBuff[0x08];
  Ee24cxxRead(0x00, tempReadBuff, 0x30);
  if(0xFF != tempReadBuff[EEPROM_STATUS]){
    rtc.year  = BUILD_YEAR;
    rtc.month = BUILD_MONTH;
    rtc.day   = BUILD_DAY;
    rtc.hour  = BUILD_TIME_H;
    rtc.min   = BUILD_TIME_M;
    rtc.sec   = BUILD_TIME_S;
    CounterToBuffer(RtcTimeToCounter(), tempWriteBuff);
    Ee24cxxWritePage(EEPROM_BUILD_DATE, tempWriteBuff, 0x04);
    Ee24cxxWriteByte(EEPROM_STATUS, 0x00);
    Ee24cxxWriteByte(EEPROM_DEVICE_N, DEVICE_NUMBER);
    Ee24cxxWriteByte(EEPROM_BUILD_TYPE,'S');
    Ee24cxxWriteByte(EEPROM_CALIBRATION, RTC_CALIBRATION);
    Ee24cxxWritePage(EEPROM_NAME_BUILD, (uint8_t*)NAME_BUILD, 0x05);
    Ee24cxxWriteByte(EEPROM_CALIB_POWER_V, CALIB_POWER_V);
    CounterToBuffer(CAN_SPEED, tempWriteBuff);
    Ee24cxxWritePage(EEPROM_CAN_SPEED, tempWriteBuff, 0x08);
    Ee24cxxWriteByte(EEPROM_RS485_SPEED, ((uint8_t)RS485_SPEED >> 0x08));
    Ee24cxxWriteByte(EEPROM_RS485_SPEED + 0x01, (uint8_t)RS485_SPEED);
    
    
    
    Ee24cxxRead(0x00, tempReadBuff, 0x30);
  }
  settings.type = tempReadBuff[EEPROM_BUILD_TYPE];
  settings.number = tempReadBuff[EEPROM_DEVICE_N];
  settings.date = BufferToCounter(tempReadBuff);
  settings.canDevice = 0x0000 + (settings.number << 0x04);
  settings.calibration = tempReadBuff[EEPROM_CALIBRATION];
  settings.calibPowerV = tempReadBuff[EEPROM_CALIB_POWER_V];
  settings.canSpeed = (tempReadBuff[EEPROM_CAN_SPEED] << 0x18)| (tempReadBuff[EEPROM_CAN_SPEED + 0x01] << 0x10) |
                      (tempReadBuff[EEPROM_CAN_SPEED + 0x02] << 0x08)| (tempReadBuff[EEPROM_CAN_SPEED + 0x03]);
  settings.rs485Speed = (tempReadBuff[EEPROM_RS485_SPEED] << 0x08)| tempReadBuff[EEPROM_RS485_SPEED + 0x01];
  
  
  
  
  dht22.humidity = 0xFFFF;
  dht22.temperature = 0xFFFF;
}

void CounterToBuffer(uint32_t counter, uint8_t* buff){
  buff[0x00] = (counter >> 0x18);
  buff[0x01] = (counter >> 0x10);
  buff[0x02] = (counter >> 0x08);
  buff[0x03] = counter; 
}

uint32_t BufferToCounter(uint8_t* buff){
  uint32_t data = buff[0x00] << 0x18;
  data |= buff[0x01] << 0x10;
  data |= buff[0x02] << 0x08;
  data |= buff[0x03]; 
  return data;
}

void Ee24cxxRead(uint8_t addr, uint8_t* data, uint16_t len){
  uint8_t i = 0x00;
//  I2C1->CR1 |= I2C_CR1_ACK;
  I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB)){}
  (void) I2C1->SR1;
  I2C1->DR = 0xA0;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
  (void) I2C1->SR1;
  (void) I2C1->SR2;
  I2C1->DR = addr;
  while (!(I2C1->SR1 & I2C_SR1_TXE)){}
  I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB)){}
  (void) I2C1->SR1;
  I2C1->DR = 0xA1;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
  (void) I2C1->SR1;
  (void) I2C1->SR2;
  if (0x01 < len){
    len--;
    while(len--){
      while (!(I2C1->SR1 & I2C_SR1_RXNE)){}
      data[i++] = I2C1->DR;
    }
  }
  while (!(I2C1->SR1 & I2C_SR1_BTF)){}
  I2C1->CR1 &= ~I2C_CR1_ACK;
  data[i++] = I2C1->DR;
  I2C1->CR1 |= I2C_CR1_STOP;
}

void Ee24cxxWritePage(uint8_t page, uint8_t *data, uint8_t len){
  uint8_t i = 0x00;
  if(0x08 < len) len = 0x08;
  //I2C1->CR1 |= I2C_CR1_ACK;
  I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB)){}
  (void) I2C1->SR1;
  I2C1->DR = 0xA0;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
  (void) I2C1->SR1;
  (void) I2C1->SR2;
  I2C1->DR = page << 0x03;
  while (len--){
    while (!(I2C1->SR1 & I2C_SR1_TXE)){}
    I2C1->DR = data[i++];
  }
  while (!(I2C1->SR1 & I2C_SR1_BTF)){}
  I2C1->CR1 |= I2C_CR1_STOP;
  DelayMs(0x05);
}

void Ee24cxxWriteByte(uint8_t addr, uint8_t data){
  //I2C1->CR1 |= I2C_CR1_ACK;
  I2C1->CR1 |= I2C_CR1_START;
  while (!(I2C1->SR1 & I2C_SR1_SB)){}
  (void) I2C1->SR1;
  I2C1->DR = 0xA0;
  while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
  (void) I2C1->SR1;
  (void) I2C1->SR2;
  I2C1->DR = addr;
  while (!(I2C1->SR1 & I2C_SR1_TXE)){}
  I2C1->DR = data;
  while (!(I2C1->SR1 & I2C_SR1_BTF)){}
  I2C1->CR1 |= I2C_CR1_STOP;
  DelayMs(0x05);
}

void Ee24cxxInitt(void){
  GPIOB->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9);
  GPIOB->CRH |= (GPIO_CRH_CNF8 | GPIO_CRH_CNF9);
  
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
  I2C1->CR1 = 0x00;
  I2C1->CR2 = 0x00;
  I2C1->CR2 |= 0x28;
//  I2C1->CR2 |= I2C_CR2_ITEVTEN;
  I2C1->TRISE = 0x0D;
  I2C1->OAR1 |= I2C_OAR1_ADD4;
  I2C1->CCR |= I2C_CCR_FS;
  I2C1->CCR |= 0x0022;
  I2C1->CR1 |= I2C_CR1_PE;
}
