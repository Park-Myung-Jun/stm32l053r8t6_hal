/*
 * file name : timer.c
 * maker : mj park
 * date : 2022.02.02
 */

#include <stdio.h>

#include "stm32l0xx_hal.h"
#include "timer.h"

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

void timer_create(uint8_t* id, teTimerType type, uint32_t time, void (*callback)(void))
{
  if(timer_size == TIMER_SIZE)
  {
    printf("timer full\r\n");
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

      //printf("timer occupied : %d\r\n", i);

      return;
    }
  }  
}

void timer_delete(uint8_t* id)
{
  if(timer_size == 0)
  {
    printf("timer empty\r\n");
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

tsTimer* timer_get_info(void)
{
  return timer;
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
