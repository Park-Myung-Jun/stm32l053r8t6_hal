/*
 * file name : eeprom.h
 * maker : mj park
 * date : 2022.02.25
 */

#ifndef __EEPROM_H
#define __EEPROM_H

#include "main.h"

typedef struct _ts_eeprom_usage
{
  uint32_t update_version;
  uint32_t first[5];
  uint16_t second[10];
  uint8_t third[20];
  int32_t fourth[5];
  int16_t fifth[10];
  int8_t sixth[20];
} ts_eeprom_usage; // size 124

HAL_StatusTypeDef eeprom_init(void);
HAL_StatusTypeDef eeprom_write_bit_32(uint16_t addr_idx, uint32_t* value);
HAL_StatusTypeDef eeprom_read_bit_32(uint16_t addr_idx, uint32_t* value);

#if 0
typedef enum
{
  EEPROM_TYPE_UI,
  EEPROM_TYPE_US,
  EEPROM_TYPE_UC,
  EEPROM_TYPE_I,
  EEPROM_TYPE_S,
  EEPROM_TYPE_C,
} bit_type_32_t;

typedef union
{
  uint32_t ui;
  uint16_t us[2];
  uint8_t uc[4];
  int32_t i;
  int16_t s[2];
  int8_t c[4];
} bit_value_32_t;

HAL_StatusTypeDef eeprom_read_bit_32_type(void *value, uint16_t idx, bit_type_32_t type);
HAL_StatusTypeDef eeprom_write_bit_32_type(void *value, uint16_t idx, bit_type_32_t type);
#endif

#endif /* eeprom.h */
