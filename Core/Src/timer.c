/*
 * file name : timer.c
 * maker : mj park
 * date : 2022.02.02
 */

#include <stdbool.h>

#include "stm32l0xx_hal.h"
#include "timer.h"

#define TIMER_SIZE 10

typedef struct _tsTimer
{
  void (*callback)();
  uint32_t time_interval;
  uint32_t time_start;  
  bool isOccupied;
  bool isStarted;
  teTimerType type;
} tsTimer;

tsTimer timer[TIMER_SIZE];
uint8_t timer_size = 0U;

void timer_init(void)
{
  for(uint8_t i=0;i<TIMER_SIZE;i++)
  {
    timer[i].callback = NULL;
    timer[i].time_start = 0U;
    timer[i].time_interval = 0U;
    timer[i].isStarted = false;
    timer[i].isOccupied = false;
    timer[i].type = TIMER_TYPE_NONE;
  }
}

void timer_create(uint8_t* id, teTimerType type, uint32_t time, void (*callback)())
{
  if(timer_size == TIMER_SIZE - 1)
  {
    return;
  }

  for(uint8_t i = 0; i < TIMER_SIZE; i++)
  {
    if(!timer[i].isOccupied)
    {
      timer[i].isOccupied = true;
      timer[i].time_start = HAL_GetTick();
      timer[i].time_interval = time;
      timer[i].type = type;
      timer[i].callback = callback;

      *id = i;
      timer_size++;

      return;
    }
  }  
}

void timer_delete(uint8_t* id)
{
  if(timer_size == 0)
  {
    return;
  }

  timer[*id].isOccupied = false;
  timer[*id].isStarted = false;
  timer[*id].time_start = 0U;
  timer[*id].time_interval = 0U;
  timer[*id].type = TIMER_TYPE_NONE;
  timer[*id].callback = NULL;

  timer_size--;
}

void timer_start(uint8_t* id)
{
  timer[*id].isStarted = true;
  timer[*id].time_start = HAL_GetTick();
}

void timer_stop(uint8_t* id)
{
  timer[*id].isStarted = false;
  timer[*id].time_start = 0U;
}

void timer_callback(void)
{
  for(uint8_t i=0;i<TIMER_SIZE;i++)
  {
    if(timer[i].isStarted && (timer[i].time_start + timer[i].time_interval == HAL_GetTick()))
    {
      timer[i].callback();

      if(timer[i].type == TIMER_TYPE_ONCE)
      {
        timer[i].isStarted = false;
      }
      else if(timer[i].type == TIMER_TYPE_REPEAT)
      {
        timer[i].time_start = timer[i].time_start + timer[i].time_interval;
      }
    }
  }
}
