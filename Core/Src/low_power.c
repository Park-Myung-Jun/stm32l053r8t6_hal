/*
 * file name : shell.c
 * maker : mj park
 * date : 2022.02.28
 */

#include "low_power.h"

te_low_power_type low_power;

void low_power_set(te_low_power_type type)
{
  low_power = type;
}

te_low_power_type low_power_get(void)
{
  return low_power;
}
