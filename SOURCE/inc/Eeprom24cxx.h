#ifndef _EEPROM24CXX_H
#define _EEPROM24CXX_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Define --------------------------------------------------------------------*/
#define BUILD_YEAR (__DATE__[7] == '?' ? 1900 : (((__DATE__[7] - '0') * 1000 ) + (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + __DATE__[10] - '0'))
#define BUILD_MONTH (__DATE__ [2] == '?' ? 1 : __DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) : __DATE__ [2] == 'b' ? 2 : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
        : __DATE__ [2] == 'y' ? 5 : __DATE__ [2] == 'l' ? 7 : __DATE__ [2] == 'g' ? 8 : __DATE__ [2] == 'p' ? 9 : __DATE__ [2] == 't' ? 10 : __DATE__ [2] == 'v' ? 11 : 12)
#define BUILD_DAY (__DATE__[4] == '?' ? 1 : ((__DATE__[4] == ' ' ? 0 : ((__DATE__[4] - '0') * 10)) + __DATE__[5] - '0'))
#define BUILD_TIME_H (__TIME__[0] == '?' ? 1 : ((__TIME__[0] == ' ' ? 0 : ((__TIME__[0] - '0') * 10)) + __TIME__[1] - '0'))
#define BUILD_TIME_M (__TIME__[3] == '?' ? 1 : ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0'))
#define BUILD_TIME_S (__TIME__[6] == '?' ? 1 : ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0'))

#define EEPROM_BUILD_DATE       0x00  // 0x01, 0x02, 0x03  BUILD_DATE
#define EEPROM_NAME_BUILD       0x04  // 0x05, 0x06, 0x07, 0x08  // v0.02
#define EEPROM_BUILD_TYPE       0x09  // Device type, S = switch, W = water
#define EEPROM_DEVICE_N         0x0A  // Device number


//#define ROTATION          0x27 // Rotation 0x00-0; 0x09-90; 0x18-180; 0x27-270;
//#define GUI_WAIT          0x1D // 30-1 start gui mode
//#define GUI_WAIT_S        0x13 // 20-1 time selection gui mode
//#define GUI_SELECT        0x01 // 
////#define CALIBRATION       0x00 // RTC CalibrationPpm
//#define DEFAULT_CONFIG    0x0B // D-Default, С-Сorrected
//#define ROTATION_SAVE     0x0C // Rotation 0x00-0; 0x09-90; 0x18-180; 0x27-270;
//#define CALIBRATION_SAVE  0x0D // RTC CalibrationPpm
//#define GUI_WAIT_SAVE     0x0E // GUI_WAIT
//#define GUI_WAIT_S_SAVE   0x0F // GUI_WAIT_S
//#define CALIBRATION_TS    0x10 //  0x014, 0x18, 0x1C, 0x20, 0x24 Calibration_TS
//#define XXXXXXX           0x28 //









struct eeprom24cxxInitTypeDef{
  uint8_t   type;
  uint8_t   number;
  uint32_t  date;
  
  
};

extern struct eeprom24cxxInitTypeDef eeprom24cxx;

void EepromInitt(void);

#endif /* _EEPROM24CXX_H */
