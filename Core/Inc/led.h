/*
 * file name : led.h
 * maker : mj park
 * date : 2022.01.22
 */

#ifndef __LED_H
#define __LED_H

typedef enum {
  LED_OFF,
  LED_ON,
  LED_BLINK,
  LED_DIMMING,
} te_led_state_t;

typedef struct {
  uint32_t time_start;
  uint32_t time_on;
  uint32_t time_off;
  uint8_t count;
  uint8_t repeat;
  uint8_t dim_diff;
  uint8_t duty;
  uint8_t timer;
  te_led_state_t state;
} ts_led_t;

void led_init(void);
void led_on(void);
void led_off(void);
void led_blink(void);
void led_dimming(void);

ts_led_t* led_get_info(void);

#endif /* led.h */
