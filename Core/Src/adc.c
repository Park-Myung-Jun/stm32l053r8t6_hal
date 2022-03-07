/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include <string.h>

#define ADC_SIZE 2
#define ADC_COUNT_SIZE 10

void adc_bubble_sort(uint16_t* data_arr);
uint16_t adc_sort_data(uint16_t* adc_arr);

uint16_t adc_cnt = 0;
uint16_t adc_save_data[ADC_SIZE][ADC_COUNT_SIZE] = {0};
uint16_t adc_get_data[ADC_SIZE] = {0};
uint16_t adc_avg_data[ADC_SIZE] = {0};
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc;

/* ADC init function */
void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerFrequencyMode = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* ADC1 DMA Init */
    /* ADC Init */
    hdma_adc.Instance = DMA1_Channel1;
    hdma_adc.Init.Request = DMA_REQUEST_0;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void adc_bubble_sort(uint16_t* data_arr)
{
  uint8_t i, j;
  uint16_t tmp;

  for (i = 0; i < ADC_COUNT_SIZE - 1; i++)
  {
    for (j = 0; j < ADC_COUNT_SIZE - i - 1; j++)
    {
      if (data_arr[j] > data_arr[j + 1])
      {
        tmp = data_arr[j];
        data_arr[j] = data_arr[j + 1];
        data_arr[j + 1] = tmp;
      }
    }
  }
}

uint16_t adc_sort_data(uint16_t* adc_arr)
{
  uint8_t i;
  uint16_t adc_avg = 0;

  adc_bubble_sort(adc_arr);

  for (i = ADC_COUNT_SIZE/2-2; i < ADC_COUNT_SIZE/2+2; ++i)
  {
    adc_avg += adc_arr[i];
  }

  adc_avg /= 4;

  return adc_avg;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if(hadc->Instance == hadc1.Instance)
  {
    if(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS))
    {
      adc_save_data[0][adc_cnt] = adc_get_data[0];
      adc_save_data[1][adc_cnt] = adc_get_data[1];

      adc_cnt++;

      if(adc_cnt >= ADC_COUNT_SIZE)
      {
        adc_cnt = 0;

        adc_avg_data[0] = adc_sort_data(adc_save_data[0]);
        adc_avg_data[1] = adc_sort_data(adc_save_data[1]);

        memset(adc_save_data, 0x00, sizeof(adc_save_data));
      }
    }
  }
}

void ADC_Initialize(void)
{
  /* Initialize ADC group regular data buffer values */
  for (uint8_t i = 0; i < ADC_SIZE; i++)
  {
    adc_get_data[i] = 0;
  }

  if (HAL_ADC_Stop(&hadc1) != HAL_OK)
  {
    //dlog(ADCMOD, WARN, "ADC Stop FAIL\r\n");
    /* Error: ADC conversion start could not be performed */
    Error_Handler();
  }

  /* Perform ADC calibration */
  if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
  {
    //dlog(ADCMOD, WARN, "ADC Calibration FAIL\r\n");
    /* Calibration Error */
    Error_Handler();
  }

  /* Start ADC group regular conversion */
  /* Note: First start with DMA transfer initialization, following ones with basic ADC start. */
  if (HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_get_data, ADC_SIZE) != HAL_OK)
  {
    //dlog(ADCMOD, WARN, "ADC DMA Start FAIL\r\n");
    /* Error: ADC conversion start could not be performed */
    Error_Handler();
  }
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
