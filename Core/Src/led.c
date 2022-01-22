/*
 * file name : led.h
 * maker : mj park
 * date : 2022.01.22
 */

#include "main.h"
#include "led.h"

ts_led_t led;

void led_init(void)
{
  led.state = LED_OFF;
}

void led_on(void)
{
  led.state = LED_ON;

  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
}

void led_off(void)
{
  led.state = LED_OFF;

  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
}
