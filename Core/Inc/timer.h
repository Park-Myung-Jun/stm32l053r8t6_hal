/*
 * file name : timer.h
 * maker : mj park
 * date : 2022.02.02
 */

#include <stdbool.h>

#define TIMER_SIZE 6

typedef enum _teTimerType
{
  TIMER_TYPE_NONE,
	TIMER_TYPE_ONCE,
  TIMER_TYPE_REPEAT,
} teTimerType;

typedef struct _tsTimer
{
  void (*callback)();
  uint32_t time_interval;
  uint32_t time_start;
  bool isOccupied;
  bool isStarted;
  teTimerType type;
} tsTimer;

void timer_init(void);
void timer_create(uint8_t* id, teTimerType type, uint32_t time, void (*callback)());
void timer_delete(uint8_t* id);
void timer_start(uint8_t* id);
void timer_stop(uint8_t* id);

tsTimer* timer_get_info(void);

void timer_callback(void);
