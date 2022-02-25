/*
 * file name : flash.h
 * maker : mj park
 * date : 2022.02.25
 */

#ifndef __FLASH_H
#define __FLASH_H

#include "main.h"

typedef struct _ts_flash_page1
{
  uint32_t update_version;
  uint32_t first[2];
  uint16_t second[4];
  uint8_t third[8];
  int32_t fourth[2];
  int16_t fifth[4];
  int8_t sixth[8];
  char string[76];
} ts_flash_page_0; // page size 128

HAL_StatusTypeDef flash_init(void);
HAL_StatusTypeDef flash_page_write(uint8_t page, void * target_addr);
HAL_StatusTypeDef flash_page_read(uint8_t page, uint32_t target_addr);
HAL_StatusTypeDef flash_page_erase(uint8_t page);

#endif /* flash.h */
