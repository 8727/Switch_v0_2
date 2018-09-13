#ifndef _W25QXX_H
#define _W25QXX_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "ili9488.h"

/* Define --------------------------------------------------------------------*/
#define W25Qxx_CS_LOW      GPIOA->BRR = GPIO_BRR_BR4
#define W25Qxx_CS_HIGHT    GPIOA->BSRR = GPIO_BSRR_BS4

#define CMD_W_STATUS_1  0x01
#define CMD_W_STATUS_2  0x31
#define CMD_W_STATUS_3  0x11

#define CMD_R_STATUS_1  0x05
#define CMD_R_STATUS_2  0x35
#define CMD_R_STATUS_3  0x15

#define CMD_WRITE       0x02
#define CMD_READ        0x03
#define CMD_FAST_READ   0x0B

#define CMD_WRITE_ON    0x06
#define CMD_WRITE_OFF   0x04

#define CMD_ON_RESET    0x66
#define CMD_RESET       0x99

#define CMD_DEV_ID      0x90
#define CMD_JEDEC_ID    0x9F

#define CMD_ERASE_CHIP  0x60 //0xC7 //0x60
#define CMD_ERASE_4K    0x20
#define CMD_ERASE_64K   0xD8

struct w25qxxInitTypeDef{
  uint8_t  id;
  char     *name;
  uint8_t  imgtable[0x100];
};

extern struct w25qxxInitTypeDef w25qxx;


void W25QxxErase(void);
void W25QxxReadPage(uint16_t page, uint8_t *buff);
void W25QxxWritePage(uint16_t page, uint8_t *buff);
void W25QxxReadImgTable(void);
void W25QxxInit(void);

#endif /* _W25QXX_H */
