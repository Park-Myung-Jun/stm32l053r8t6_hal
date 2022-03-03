/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "wwdg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "shell.h"
#include "led.h"
#include "button.h"
#include "usart.h"
#include "timer.h"
#include "low_power.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum reset_cause
{
    RESET_CAUSE_UNKNOWN = 0,
    RESET_CAUSE_LOW_POWER_RESET,
    RESET_CAUSE_WINDOW_WATCHDOG_RESET,
    RESET_CAUSE_INDEPENDENT_WATCHDOG_RESET,
    RESET_CAUSE_SOFTWARE_RESET,
    RESET_CAUSE_POWER_ON_POWER_DOWN_RESET,
    RESET_CAUSE_OPTION_BYTE_LOADER_RESET,
    RESET_CAUSE_EXTERNAL_RESET_PIN_RESET,
} reset_cause_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void main_callback(void);
void main_callback2(void);
void main_callback3(void);

char* main_reset_cause(void); // const char * main_reset_cause_name(reset_cause_t reset_cause);

uint8_t main_timer, main_timer2, main_timer3;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_IWDG_Init();
  MX_RTC_Init();
  MX_WWDG_Init();
  /* USER CODE BEGIN 2 */
  timer_init();
  iwdg_init();
  wwdg_init();
  led_init();
  button_init();
  usart_init();

  printf("System reset cause: %s", main_reset_cause());
  __HAL_RCC_CLEAR_RESET_FLAGS();

  printf(CRLF SHELL_GREEN "[system] " SHELL_RESET "Module init end");
  printf(CRLF SHELL_GREEN "[version] " SHELL_RESET "SW_VERSION : %s\r\n" PROMPT, SW_VERSION);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  timer_create(&main_timer, TIMER_TYPE_REPEAT, 500, main_callback);
  timer_create(&main_timer2, TIMER_TYPE_REPEAT, 333, main_callback2);
  timer_create(&main_timer3, TIMER_TYPE_REPEAT, 777, main_callback3);

  while (1)
  {
    low_power_operation();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void main_timer_start(void)
{
  timer_start(&main_timer);
}

void main_timer_stop(void)
{
  timer_stop(&main_timer);
}

void main_timer_start2(void)
{
  timer_start(&main_timer2);
}

void main_timer_stop2(void)
{
  timer_stop(&main_timer2);
}

void main_timer_start3(void)
{
  timer_start(&main_timer3);
}

void main_timer_stop3(void)
{
  timer_stop(&main_timer3);
}

void main_callback(void)
{
  printf("main callback\r\n");
}

void main_callback2(void)
{
  printf("main callback2\r\n");
}

void main_callback3(void)
{
  printf("main callback3\r\n");
}

void main_system_init(void)
{
  SystemClock_Config();
  HAL_ResumeTick();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_IWDG_Init();
  MX_RTC_Init();
}

char* main_reset_cause(void)
{
  //reset_cause_t reset_cause;

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
  {
    return "LOW_POWER_RESET"; // reset_cause = RESET_CAUSE_LOW_POWER_RESET;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
  {
    return "WINDOW_WATCHDOG_RESET"; // reset_cause = RESET_CAUSE_WINDOW_WATCHDOG_RESET;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
  {
    return "INDEPENDENT_WATCHDOG_RESET"; // reset_cause = RESET_CAUSE_INDEPENDENT_WATCHDOG_RESET;
  }
  /* This reset is induced by calling the ARM CMSIS `NVIC_SystemReset()` function! */
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
  {
    return "SOFTWARE_RESET"; // reset_cause = RESET_CAUSE_SOFTWARE_RESET;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST))
  {
    return "OPTION_BYTE_LOADER_RESET"; // reset_cause = RESET_CAUSE_OPTION_BYTE_LOADER_RESET;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
  {
    return "EXTERNAL_RESET_PIN_RESET"; // reset_cause = RESET_CAUSE_EXTERNAL_RESET_PIN_RESET;
  }
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
  {
    return "POWER-ON_RESET (POR) / POWER-DOWN_RESET (PDR)"; // reset_cause = "POWER-ON_RESET (POR) / POWER-DOWN_RESET (PDR)";
  }
  else
  {
    return "UNKNOWN"; // reset_cause = RESET_CAUSE_UNKNOWN;
  }
}

#if 0
const char * main_reset_cause_name(reset_cause_t reset_cause)
{
  const char * reset_cause_name = "DEFAULT";

  switch (reset_cause)
  {
    case RESET_CAUSE_UNKNOWN:
    {
      reset_cause_name = "UNKNOWN";
    }
      break;
    case RESET_CAUSE_LOW_POWER_RESET:
    {
      reset_cause_name = "LOW_POWER_RESET";
    }
      break;
    case RESET_CAUSE_WINDOW_WATCHDOG_RESET:
    {
      reset_cause_name = "WINDOW_WATCHDOG_RESET";
    }
      break;
    case RESET_CAUSE_INDEPENDENT_WATCHDOG_RESET:
    {
      reset_cause_name = "INDEPENDENT_WATCHDOG_RESET";
    }
      break;
    case RESET_CAUSE_SOFTWARE_RESET:
    {
      reset_cause_name = "SOFTWARE_RESET";
    }
      break;
    case RESET_CAUSE_POWER_ON_POWER_DOWN_RESET:
    {
      reset_cause_name = "POWER-ON_RESET (POR) / POWER-DOWN_RESET (PDR)";
    }
      break;
    case RESET_CAUSE_OPTION_BYTE_LOADER_RESET:
    {
      reset_cause_name = "OPTION_BYTE_LOADER_RESET";
    }
      break;
    case RESET_CAUSE_EXTERNAL_RESET_PIN_RESET:
    {
      reset_cause_name = "EXTERNAL_RESET_PIN_RESET";
    }
      break;
  }

  return reset_cause_name;
}
#endif
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
    printf("Error Handler\r\n");
    NVIC_SystemReset();
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
