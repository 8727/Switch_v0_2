#include "setting.h"

struct settingsInitTypeDef settings;
static __IO uint32_t msTicks;

void SysTick_Handler(void){
  msTicks++;
}

uint32_t GetTick(void){
  return msTicks;
}

void DelayMs(uint32_t ms){
  uint32_t tickstart = GetTick();
  while((GetTick() - tickstart) < ms){}
}

void Start(void){
  SysTick_Config(SystemCoreClock / 1000);   //1ms
  
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  
  AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE + AFIO_MAPR_TIM2_REMAP_FULLREMAP + AFIO_MAPR_I2C1_REMAP;
  
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
  RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
}

void ReadConfig(void){
  uint8_t tempReadBuff[0x30];
  uint8_t tempWriteBuff[0x08];
  Ee24cxxRead(0x00, tempReadBuff, 0x30);
  if(0x00 != tempReadBuff[EEPROM_STATUS]){
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

void Setting(void){
  Ee24cxxInit();
  ReadConfig();
  RtcInit();
//  LcdInitt();
//  TIM2->CCR1 = 0x20;
//  Xpt2046Init();
//  Dht22Init();
//  Ds18b20Init();
  
  W25QxxInit();
  W25QxxReadImgTable();
}
