/*
 * file name : shell.h
 * maker : mj park
 * date : 2022.01.24
 */

#ifndef __SHELL_H_
#define __SHELL_H_

#define print(fmt, ...) printf(fmt, ##__VA_ARGS__)

void shell_operation(uint8_t rx);

#endif /* shell.h */
