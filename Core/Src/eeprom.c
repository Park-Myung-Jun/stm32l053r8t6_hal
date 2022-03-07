/*
 * file name : eeprom.c
 * maker : mj park
 * date : 2022.02.25
 */

#include <string.h>

#include "eeprom.h"

#define EEPROM_ADDRESS ((uint32_t)0x08080000)
#define EEPROM_RANGE  ((uint32_t)0x800)
#define EEPROM_UPDATE_VERSION 1

ts_eeprom_usage eeprom_usage;

HAL_StatusTypeDef eeprom_init(void)
{
  if((*(uint32_t*)EEPROM_ADDRESS) != EEPROM_UPDATE_VERSION)
  {
    uint8_t i;

    eeprom_usage.update_version = EEPROM_UPDATE_VERSION;

    for(i = 0; i < 5; i++)
    {
      eeprom_usage.first[i] = 0x12345678 + i;
      eeprom_usage.fourth[i] = 0x12345678 - i;
    }

    for(i = 0; i < 10; i++)
    {
      eeprom_usage.second[i] = 0x1234 + i;
      eeprom_usage.fifth[i] = 0x1234 - i;
    }

    for(i = 0; i < 20; i++)
    {
      eeprom_usage.third[i] = 0x37 + i;
      eeprom_usage.sixth[i] = 0x37 - i;
    }

    for(i = 0; i < sizeof(eeprom_usage) / 4; i++)
    {
      if(eeprom_write_bit_32(i, (uint32_t*)&eeprom_usage + i) != HAL_OK)
      {
        return HAL_ERROR;
      }
    }
  }
  else
  {
    memcpy(&eeprom_usage, (uint32_t*)EEPROM_ADDRESS, sizeof(eeprom_usage));
  }

  return HAL_OK;
}

/* read 32 bit eeprom by address index */
HAL_StatusTypeDef eeprom_read_bit_32(uint16_t addr_idx, uint32_t* value)
{
  *value = *(uint32_t*)(EEPROM_ADDRESS + addr_idx * 4);

  return HAL_OK;
}

/* write 32 bit eeprom by address index */
HAL_StatusTypeDef eeprom_write_bit_32(uint16_t addr_idx, uint32_t* value)
{
  HAL_FLASHEx_DATAEEPROM_Unlock();

  HAL_FLASHEx_DATAEEPROM_Erase(EEPROM_ADDRESS + addr_idx * 4);

  HAL_Delay(4);

  /* TypeProgram : FLASH_TYPEPROGRAMDATA_WORD, FLASH_TYPEPROGRAMDATA_HALFWORD, FLASH_TYPEPROGRAMDATA_BYTE */
  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, EEPROM_ADDRESS + addr_idx * 4, *value);

  HAL_Delay(4);

  HAL_FLASHEx_DATAEEPROM_Lock();

  return HAL_OK;
}

#if 0
/* read value by type, calculating offset and sub index */
HAL_StatusTypeDef eeprom_read_bit_32_type(void *value, uint16_t idx, bit_type_32_t type)
{
  bit_value_32_t tmp;
  uint8_t offset;

  /* divide idx by type and read value by eeprom and get value */
  switch (type)
  {
    case EEPROM_TYPE_UI:
    case EEPROM_TYPE_I:
    {
      if (eeprom_read_bit_32(idx, &tmp) != HAL_OK)
      {
        return HAL_ERROR;
      }

      if(type == EEPROM_TYPE_UI)
      {
        *(uint32_t*)value = tmp.ui;
      }
      else
      {
        *(int32_t*)value = tmp.i;
      }
    }
      break;

    case EEPROM_TYPE_US:
    case EEPROM_TYPE_S:
    {
      offset = idx % 2;
      idx = idx / 2;

      if (eeprom_read_bit_32(idx, &tmp) != HAL_OK)
      {
        return HAL_ERROR;
      }

      if (type == EEPROM_TYPE_US)
      {
        *(uint16_t*)value = tmp.us[offset];
      }
      else
      {
        *(int16_t*)value = tmp.s[offset];
      }
    }
      break;

    case EEPROM_TYPE_UC:
    case EEPROM_TYPE_C:
    {
      offset = idx % 4;
      idx = idx / 4;

      if (eeprom_read_bit_32(idx, &tmp) != HAL_OK)
      {
        return HAL_ERROR;
      }

      if (type == EEPROM_TYPE_UC)
      {
        *(uint8_t*)value = tmp.uc[offset];
      }
      else
      {
        *(int8_t*)value = tmp.c[offset];
      }
    }
      break;

    default:
    {
      return HAL_ERROR;
    }
      break;
  }

  return HAL_OK;
}

/* write value by type, calculating offset and sub index */
HAL_StatusTypeDef eeprom_write_bit_32_type(void *value, uint16_t idx, bit_type_32_t type)
{
  bit_value_32_t tmp;
  uint8_t offset;

  /* divide idx by type and read value by eeprom */
  switch (type)
  {
    case EEPROM_TYPE_UI:
    {
      tmp.ui = *(uint32_t*)value;
    }
      break;

    case EEPROM_TYPE_I:
    {
      tmp.i = *(int32_t*)value;
    }
      break;

    case EEPROM_TYPE_US:
    case EEPROM_TYPE_S:
    {
      offset = idx % 2;
      idx = idx / 2;

      if (eeprom_read_bit_32(idx, &tmp) != HAL_OK)
      {
        return HAL_ERROR;
      }

      if (type == EEPROM_TYPE_US)
      {
        tmp.us[offset] = *(uint16_t*)value;
      }
      else
      {
        tmp.s[offset] = *(int16_t*)value;
      }
    }
      break;

    case EEPROM_TYPE_UC:
    case EEPROM_TYPE_C:
    {
      offset = idx % 4;
      idx = idx / 4;

      if (eeprom_read_bit_32(idx, &tmp) != HAL_OK)
      {
        return HAL_ERROR;
      }

      if (type == EEPROM_TYPE_UC)
      {
        tmp.uc[offset] = *(uint8_t*)value;
      }
      else
      {
        tmp.c[offset] = *(int8_t*)value;
      }
    }
      break;

    default:
    {
      return HAL_ERROR;
    }
      break;
  }

  /* write target index using eeprom */
  if(eeprom_write_bit_32(idx, &tmp) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}
#endif
