/*
 * file name : flash.c
 * maker : mj park
 * date : 2022.02.25
 */

#include <string.h>

#include "flash.h"

#define FLASH_UPDATE_VERSION 1
#define FLASH_PAGE_NUMBER 2

volatile uint32_t flash_page_addr[FLASH_PAGE_NUMBER] =
{
  0x0800FF80,
  0x0800FF00,
};

ts_flash_page_0 flash_page_0 = {0};

HAL_StatusTypeDef flash_page_write_init_0(void);

HAL_StatusTypeDef flash_init(void)
{
  if((*(uint32_t*)flash_page_addr[0]) != FLASH_UPDATE_VERSION)
  {
    flash_page_write_init_0();
  }
  else
  {
    flash_page_read(0, flash_page_addr[0]);
  }

  return HAL_OK;
}

HAL_StatusTypeDef flash_page_write_init_0(void)
{
  uint8_t i;

  flash_page_0.update_version = FLASH_UPDATE_VERSION;

  for(i = 0; i < 2; i++)
  {
    flash_page_0.first[i] = 0x12345678 + i;
    flash_page_0.fourth[i] = 0x12345678 - i;
  }

  for(i = 0; i < 4; i++)
  {
    flash_page_0.second[i] = 0x1234 + i;
    flash_page_0.fifth[i] = 0x1234 - i;
  }

  for(i = 0; i < 8; i++)
  {
    flash_page_0.third[i] = 0x37 + i;
    flash_page_0.sixth[i] = 0x37 - i;
  }

  strcpy(flash_page_0.string, "Hello, this is flash page0 string\r\n");

  flash_page_write(0, &flash_page_0);

  return HAL_OK;
}

HAL_StatusTypeDef flash_page_write(uint8_t page, void * target_addr)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  uint8_t verify[128] = {0};

  if(flash_page_erase(page) == HAL_OK)
  {
    if( HAL_FLASH_Unlock() == HAL_OK )
    {
      assert_param(IS_FLASH_PROGRAM_ADDRESS(flash_page[page]));

      for(uint8_t i = 0; i < FLASH_PAGE_SIZE / 4; i++)
      {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_page_addr[page] + (i * 4), ((uint32_t*)target_addr)[i]);
      }

      if( HAL_FLASH_Lock() == HAL_OK )
      {
        flash_page_read(page, (uint32_t)verify);

        if( strncmp((char*)&flash_page_addr[page], (char*)verify, FLASH_PAGE_SIZE) == 0 )
        {
          return HAL_OK;
        }
        else
        {
          return HAL_ERROR;
        }
      }
    }
    else
    {
      return HAL_ERROR;
    }
  }

  return status;
}

HAL_StatusTypeDef flash_page_read(uint8_t page, uint32_t target_addr)
{
  HAL_StatusTypeDef status = HAL_ERROR;

  assert_param(IS_FLASH_PROGRAM_ADDRESS(flash_page[page]));

  memcpy((void*)target_addr, (void*)flash_page_addr[page], FLASH_PAGE_SIZE);

  return status;
}

HAL_StatusTypeDef flash_page_erase(uint8_t page)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  FLASH_EraseInitTypeDef  tsEraseInit;
  uint32_t uPageError = 0;

  if( HAL_FLASH_Unlock() == HAL_OK )
  {
    tsEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    tsEraseInit.PageAddress = flash_page_addr[page];
    tsEraseInit.NbPages = 1;

    status = HAL_FLASHEx_Erase(&tsEraseInit, &uPageError);

    if( HAL_FLASH_Lock() == HAL_OK )
    {
      return status;
    }
  }

  return status;
}
