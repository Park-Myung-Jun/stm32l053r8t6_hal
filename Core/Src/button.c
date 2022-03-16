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

#define BUTTON_SHORT_TIME 500U
#define BUTTON_LONG_TIME 2000U
#define BUTTON_VERY_LONG_TIME 5000U

ts_button_t button;

void button_timeout_oepration(void);
void button_callback(void);

void button_init(void)
{
  button.time_start = 0U;
  button.short_count = 0U;
  button.long_count = 0U;
  button.very_long_count = 0U;
  button.type = BUTTON_TYPE_NONE;
  button.state = BUTTON_STATE_NONE;
  button.isStart = false;
  button.isTimeOut = false;

  timer_create(&button.timer, TIMER_TYPE_REPEAT, 10, button_callback);
}

void button_interrupt_callback(void)
{
  GPIO_PinState button_pin = HAL_GPIO_ReadPin(BUTTON_BLUE_GPIO_Port, BUTTON_BLUE_Pin);

  if (button_pin == GPIO_PIN_RESET)
  {
    led_on();

    button.state = BUTTON_STATE_PUSH;
    button.isStart = true;

    button.time_start = HAL_GetTick();

    timer_start(&button.timer);
  }
  else if (button_pin == GPIO_PIN_SET)
  {
    led_off();

    if (button.isStart)
    {
      switch (button.type)
      {
      case BUTTON_TYPE_SHORT:
      {
        printf("short key pressed\r\n");
        button.short_count++;
        break;
      }

      case BUTTON_TYPE_LONG:
      {
        printf("long key pressed\r\n");
        button.long_count++;
        break;
      }

      case BUTTON_TYPE_VERY_LONG:
      {
        printf("very long key pressed\r\n");
        button.very_long_count++;
        break;
      }

      default:
        break;
      }
    }

    button.type = BUTTON_TYPE_NONE;
    button.state = BUTTON_STATE_PULL;
    button.isStart = true;

    button.time_start = HAL_GetTick();

    if (!button.isTimeOut)
    {
      timer_start(&button.timer);
    }
    else
    {
      button.isTimeOut = false;
    }
  }
}

void button_timeout_oepration(void)
{
  printf("press time end, short : %d, long : %d, very long : %d\r\n",
         button.short_count, button.long_count, button.very_long_count);
  timer_stop(&button.timer);

  // TO DO : working by setted button counts

  button.type = BUTTON_TYPE_NONE;
  button.short_count = 0U;
  button.long_count = 0U;
  button.very_long_count = 0U;
  button.isStart = false;
}

void button_callback(void)
{
  if (button.state == BUTTON_STATE_PUSH)
  {
    if (button.type == BUTTON_TYPE_NONE)
    {
      button.type = BUTTON_TYPE_SHORT;
      printf("short in\r\n");
    }
    else if ((HAL_GetTick() - button.time_start > BUTTON_SHORT_TIME) && (button.type == BUTTON_TYPE_SHORT))
    {
      button.type = BUTTON_TYPE_LONG;
      printf("long in\r\n");
    }
    else if ((HAL_GetTick() - button.time_start > BUTTON_LONG_TIME) && (button.type == BUTTON_TYPE_LONG))
    {
      button.type = BUTTON_TYPE_VERY_LONG;
      printf("very long in\r\n");
    }
    else if (HAL_GetTick() - button.time_start > BUTTON_VERY_LONG_TIME)
    {
      button_timeout_oepration();
      button.isTimeOut = true;
    }
  }
  else if (button.state == BUTTON_STATE_PULL)
  {
    if (HAL_GetTick() - button.time_start > BUTTON_SHORT_TIME)
    {
      button_timeout_oepration();
    }
  }
}
