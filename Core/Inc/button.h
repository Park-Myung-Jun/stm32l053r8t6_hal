/*
 * file name : button.h
 * maker : mj park
 * date : 2022.01.23
 */

#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdbool.h>

typedef enum {
  BUTTON_STATE_NONE,
  BUTTON_STATE_PUSH,
  BUTTON_STATE_PULL,
} te_button_state_t;

typedef struct {
  uint32_t time_start;
  uint16_t short_count;
  uint8_t timer;
  te_button_state_t state;
  bool isStart;
  bool isTimeOut;
} ts_button_t;

void button_init(void);
void button_interrupt_callback(void);

#endif /* button.h */
