/*
 * file name : shell.c
 * maker : mj park
 * date : 2022.01.24
 */

#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "main.h"

#define BUFFER_MAX 64

typedef struct _tsShellList {
  char cmd[8];
  void (*func)(uint8_t, void*);
} tsShellList;

void cmd_reset(uint8_t argc, void* argv);
void cmd_version(uint8_t argc, void* argv);
void cmd_parse(void);

uint8_t buffer_idx = 0;
char shell_buffer[BUFFER_MAX];
tsShellList cmd_list[] = {
  {"reset", cmd_reset},
  {"version", cmd_version},
};

void shell_operation(uint8_t rx)
{
  if(0x20 <= rx && rx <= 0x7E)
  {
    if(buffer_idx == BUFFER_MAX - 1)
    {
      return;
    }

    shell_buffer[buffer_idx] = rx;
    buffer_idx++;
  }
  else if(rx == '\r' || rx == '\n')
  {
    cmd_parse();

    memset(shell_buffer, 0, BUFFER_MAX);
    buffer_idx = 0;
  }
  else if(rx == '\b')
  {
    printf("\b ");
    buffer_idx--;
  }

  printf("%c", rx);
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

  for(uint8_t i=0; cmd_list[i].func != NULL; i++)
  {
    if(strcmp(cmd_list[i].cmd, argv[0]) == 0)
    {
      cmd_list[i].func(argc, argv);
      return;
    }
  }
}

void cmd_reset(uint8_t argc, void* argv)
{
  NVIC_SystemReset();
}

void cmd_version(uint8_t argc, void* argv)
{
  print("sw_version : %s\r\n", SW_VERSION);
}
