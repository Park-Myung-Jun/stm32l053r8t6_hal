/*
 * file name : shell.h
 * maker : mj park
 * date : 2022.01.24
 */

#ifndef __SHELL_H
#define __SHELL_H

#include <stdio.h>

#define PROMPT "PROMPT > "
#define CRLF "\r\n"

#define SHELL_RED "\x1b[31m"
#define SHELL_YELLOW "\x1b[33m"
#define SHELL_GREEN "\x1b[32m"
#define SHELL_RESET "\x1b[0m"

#if 0
#define print(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

void shell_operation(uint8_t rx);

// TO DO : log division - INFO, TEST etc...

#endif /* shell.h */
