/*
 * file name : shell.c
 * maker : mj park
 * date : 2022.01.24
 */

#include <string.h>
#include <stdlib.h>

#include "shell.h"
#include "main.h"
#include "usart.h"
#include "timer.h"
#include "led.h"

#define BUFFER_MAX 64

typedef struct _tsShellList {
  char cmd[8];
  void (*func)(uint8_t, void*);
} tsShellList;

void cmd_led(uint8_t argc, void* argv);
void cmd_timer(uint8_t argc, void* argv);
void cmd_clear(uint8_t argc, void* argv);
void cmd_version(uint8_t argc, void* argv);
void cmd_reset(uint8_t argc, void* argv);
void cmd_parse(void);

uint8_t buffer_idx = 0;
char shell_buffer[BUFFER_MAX];
tsShellList cmd_list[] = {
  {"reset", cmd_reset},
  {"version", cmd_version},
  {"cls", cmd_clear},
  {"timer", cmd_timer},
  {"led", cmd_led},
};

void cmd_led(uint8_t argc, void* argv)
{
  char** list = argv;

  ts_led_t* tmp_led = led_get_info();

  if(argc == 1)
  {
    printf("state : %d, repeat : %d, time on : %ld, time off : %ld\r\n",
      tmp_led->state, tmp_led->repeat, tmp_led->time_on, tmp_led->time_off);
  }

  if(argc == 2)
  {
    if(!strcmp("off", list[1]))
    {
      led_off(); //printf("led off\r\n");
    }
    else if(!strcmp("on", list[1]))
    {
      led_on(); //printf("led on\r\n");
    }
    else if(!strcmp("blk", list[1]))
    {
      led_blink(); //printf("led blink\r\n");
    }
    else if(!strcmp("dim", list[1]))
    {
      led_dimming(); //printf("led dimming\r\n");
    }
  }

  if(argc == 3)
    {
      if(!strcmp("ton", list[1]))
      {
        tmp_led->time_on = atoi(list[2]);
      }
      else if(!strcmp("toff", list[1]))
      {
        tmp_led->time_off = atoi(list[2]);
      }
      else if(!strcmp("repeat", list[1]))
      {
        tmp_led->repeat = atoi(list[2]);
      }
    }
}

void cmd_timer(uint8_t argc, void* argv)
{
  char** list = argv; //printf("%d %s %s\r\n", argc, ((char**)argv)[0], ((char**)argv)[1]);

  if(argc == 1)
  {
    tsTimer* tmp_timer = timer_get_info();

    for(uint8_t i = 0; i < TIMER_SIZE; i++)
    {
      printf("%d : %d %d %d %ld\r\n", i, tmp_timer[i].isOccupied,
        tmp_timer[i].isStarted, tmp_timer[i].type, tmp_timer[i].time_interval);
    }
  }

  if(argc == 3)
  {
    if(!strcmp("start", list[1]))
    {
      uint8_t num = atoi(list[2]);

      switch(num)
      {
        case 0:
          main_timer_start();
          break;
        case 1:
          main_timer_start2();
          break;
        case 2:
          main_timer_start3();
          break;
        default:
          break;
      }
    }
    else if(!strcmp("stop", list[1]))
    {
      uint8_t num = atoi(list[2]);

      switch(num)
      {
        case 0:
          main_timer_stop();
          break;
        case 1:
          main_timer_stop2();
          break;
        case 2:
          main_timer_stop3();
          break;
        default:
          break;
      }
    }
  }
}

void cmd_reset(uint8_t argc, void* argv)
{
  NVIC_SystemReset();
}

void cmd_version(uint8_t argc, void* argv)
{
  printf("sw_version : %s\r\n", SW_VERSION);
}

void cmd_clear(uint8_t argc, void* argv)
{
  printf("\e[1;1H\e[2J");
}

void shell_operation(uint8_t rx)
{
  if(0x20 <= rx && rx <= 0x7E)
  {
    if(buffer_idx == BUFFER_MAX - 1)
    {
      return;
    }

    printf("%c", rx);
    shell_buffer[buffer_idx] = rx;
    buffer_idx++;
  }
  else if(rx == '\r' || rx == '\n') // rx : 13(\r)
  {
    printf(CRLF);

    cmd_parse();

    memset(shell_buffer, 0, BUFFER_MAX);
    buffer_idx = 0;

    printf(PROMPT);
  }
  else if(rx == '\b')
  {
    if(buffer_idx == 0)
    {
      return;
    }

    printf("\b \b");
    buffer_idx--;
    shell_buffer[buffer_idx] = 0;
  }
}

void cmd_parse(void)
{
  uint8_t argc = 0;
  void *argv[128];
  uint8_t argv_idx = 0;

  if(buffer_idx != 0)
  {
    argv_idx = 1;
    argv[0] = &shell_buffer[0];
  }
  else
  {
    return;
  }

  for(int i=0; i<buffer_idx; i++)
  {
    if(shell_buffer[i] == ' ')
    {
      argv[argv_idx++] = &shell_buffer[i+1];
      shell_buffer[i] = '\0';
      argc++;
    }
  }

  argc++;

  for(uint8_t i=0; cmd_list[i].func != NULL; i++)
  {
    if(strcmp(cmd_list[i].cmd, argv[0]) == 0)
    {
      cmd_list[i].func(argc, argv);
      return;
    }
  }
}

// https://sinoroo.tistory.com/entry/STM32-HAL-UART-Receive
int _write(int file, char *ptr, int len)
{
  for(int i=0; i<len; i++)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ptr[i], 1, 0xFFFF); //HAL_UART_Transmit_IT(&huart2, (uint8_t*)ptr, len); -> error
  }

  return len;
}
