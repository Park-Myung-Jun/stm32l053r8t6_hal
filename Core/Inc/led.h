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
} te_led_state_t;

typedef struct {
  te_led_state_t state;
} ts_led_t;

void led_init(void);
void led_on(void);
void led_off(void);

#endif /* led.h */
