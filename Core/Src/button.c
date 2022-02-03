/*
 * file name : button.c
 * maker : mj park
 * date : 2022.01.23
 */

#include "main.h"
#include "button.h"
#include "led.h"

ts_button_t button;

void button_init(void)
{
  button.state = BUTTON_PULL;
}

void button_interrupt_callback(void)
{
  GPIO_PinState button_pin = HAL_GPIO_ReadPin(BUTTON_BLUE_GPIO_Port, BUTTON_BLUE_Pin);

  if(button_pin == GPIO_PIN_RESET)
  {
    led_on();
  }
  else if(button_pin == GPIO_PIN_SET)
  {
    led_off();
  }
}
