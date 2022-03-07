/*
 * file name : shell.c
 * maker : mj park
 * date : 2022.02.28
 */

#include "low_power.h"
#include "main.h"
#include "rtc.h"
#include "stm32l0xx_it.h"
#include "wwdg.h"
#include "iwdg.h"
#include "shell.h"
#include "timer.h"
#include "usart.h"

te_low_power_type low_power;

void low_power_set(te_low_power_type type)
{
  low_power = type;
}

void low_power_operation(void)
{
  if(low_power == LOW_POWER_STOP)
  {
    HAL_IWDG_Refresh(&hiwdg);
    HAL_UART_DeInit(&huart2);

    HAL_SuspendTick();

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_DisableLowPowerRunMode();

    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 4624, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
    {
      Error_Handler();
    }

    while(interrupt_get_state() == INTERRUPT_NONE)
    {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

      HAL_IWDG_Refresh(&hiwdg); //HAL_WWDG_Refresh(&hwwdg);
    }

    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

    main_system_init();

    usart_init();

    HAL_IWDG_Refresh(&hiwdg);

    printf("Wake up!\r\n" PROMPT);

    low_power = LOW_POWER_NONE;
  }
  else
  {
    interrupt_set_state(INTERRUPT_NONE);
  }
}
