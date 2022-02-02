/*
 * file name : timer.h
 * maker : mj park
 * date : 2022.02.02
 */

// timer create, delete, start, stop, callback

typedef enum _teTimerType
{
  TIMER_TYPE_NONE,
	TIMER_TYPE_ONCE,
  TIMER_TYPE_REPEAT,
} teTimerType;

void timer_init(void);
void timer_create(uint8_t* id, teTimerType type, uint32_t time, void (*callback)());
void timer_delete(uint8_t* id);
void timer_start(uint8_t* id);
void timer_stop(uint8_t* id);
void timer_callback(void);