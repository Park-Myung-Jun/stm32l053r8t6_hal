/*
 * file name : button.h
 * maker : mj park
 * date : 2022.01.23
 */

#ifndef __BUTTON_H
#define __BUTTON_H

typedef enum {
  BUTTON_PUSH,
  BUTTON_PULL,
} te_button_state_t;

typedef struct {
  te_button_state_t state;
} ts_button_t;

void button_init(void);
void button_interrupt_callback(void);

#endif /* button.h */
