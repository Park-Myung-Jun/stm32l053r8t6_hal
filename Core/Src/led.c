/*
 * file name : led.h
 * maker : mj park
 * date : 2022.01.22
 */

#include "main.h"
#include "led.h"
#include "tim.h"
#include "timer.h"

ts_led_t led;

void led_callback(void);

void led_init(void)
{
  led.state = LED_OFF;
  led.time_start = 0U;
  led.time_on = 500U;
  led.time_off = 500U;
  led.count = 0;
  led.repeat = 0;
  led.dim_diff = 1;
  led.duty = 0;

  timer_create(&led.timer, TIMER_TYPE_REPEAT, 10, led_callback);
}

void led_off(void)
{
  led.state = LED_OFF;
  led.time_start = 0U;
  led.count = 0;
  led.dim_diff = 1;

  TIM2->CCR1 = 0;

  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);//HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  timer_stop(&led.timer);
}

void led_on(void)
{
  led.state = LED_ON;
  led.time_start = 0U;
  led.count = 0;
  led.dim_diff = 1;

  TIM2->CCR1 = 100;

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);

  timer_stop(&led.timer);
}

void led_blink(void)
{
  led.state = LED_BLINK;
  led.time_start = HAL_GetTick();

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  timer_start(&led.timer);
}

void led_dimming(void)
{
  led.state = LED_DIMMING;

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  timer_start(&led.timer);
}

void led_callback(void)
{
  uint32_t time_now = HAL_GetTick();

  if(led.state == LED_BLINK)
  {
    if(time_now - led.time_start == led.time_on)
    {
      TIM2->CCR1 = 0;
    }
    else if(HAL_GetTick() - led.time_start == led.time_on + led.time_off)
    {
      TIM2->CCR1 = 100;

      led.time_start = time_now;

      if(led.repeat != 0)
      {
        if(led.repeat == led.count)
        {
          led_off();
        }

        led.count++;
      }
    }
  }
  else if(led.state == LED_DIMMING)
  {
    led.duty += led.dim_diff;

    if(led.duty == 0)
    {
      led.dim_diff = 1;

      if(led.repeat != 0)
      {
        if(led.repeat == led.count)
        {
          led_off();
        }

        led.count++;
      }
    }
    else if(led.duty == 100)
    {
      led.dim_diff = -1;
    }

    TIM2->CCR1 = led.duty;
  }
}

ts_led_t* led_get_info(void)
{
  return &led;
}
