/*
 * file name : button.c
 * maker : mj park
 * date : 2022.01.23
 */

#include <stdio.h>

#include "main.h"
#include "button.h"
#include "led.h"
#include "timer.h"

#define BUTTON_TIME 250U

ts_button_t button;

void button_callback(void);

void button_init(void)
{
  button.time_start = 0U;
  button.short_count = 0U;
  button.state = BUTTON_STATE_NONE;
  button.isStart = false;
  button.isTimeOut = false;

  timer_create(&button.timer, TIMER_TYPE_REPEAT, 10, button_callback);
}

void button_interrupt_callback(void)
{
  GPIO_PinState button_pin = HAL_GPIO_ReadPin(BUTTON_BLUE_GPIO_Port, BUTTON_BLUE_Pin);

  if(button_pin == GPIO_PIN_RESET)
  {
    led_on();

    button.state = BUTTON_STATE_PUSH;
    button.isStart = true;

    button.time_start = HAL_GetTick();

    timer_start(&button.timer);
  }
  else if(button_pin == GPIO_PIN_SET)
  {
    led_off();

    if(button.isStart)
    {
      button.short_count++;
    }

    button.state = BUTTON_STATE_PULL;
    button.isStart = true;

    button.time_start = HAL_GetTick();

    if(!button.isTimeOut)
    {
      timer_start(&button.timer);
    }    
    else
    {
      button.isTimeOut = false;
    }
  }
}

void button_callback(void)
{
  if(button.state == BUTTON_STATE_PUSH)
  {
    if(HAL_GetTick() - button.time_start > BUTTON_TIME)
    {
      // short key end

      printf("short key pressed : %d\r\n", button.short_count);
      timer_stop(&button.timer);
      button.short_count = 0U;
      button.isStart = false;
      button.isTimeOut = true;
    }
  }
  else if(button.state == BUTTON_STATE_PULL)
  {
    if(HAL_GetTick() - button.time_start > BUTTON_TIME)
    {
      // short key end

      printf("short key pressed : %d\r\n", button.short_count);
      timer_stop(&button.timer);
      button.short_count = 0U;
      button.isStart = false;
    }
  }
}
