/*
 * file name : low_power.h
 * maker : mj park
 * date : 2022.02.28
 */

#ifndef __LOW_POWER_H
#define __LOW_POWER_H

typedef enum _te_low_power_type
{
  LOW_POWER_NONE,
  LOW_POWER_SLEEP,
  LOW_POWER_STOP,
  LOW_POWER_STAND_BY,
} te_low_power_type;

void low_power_set(te_low_power_type type);
te_low_power_type low_power_get(void);

#endif /* low_power.h */
