#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void SysTickDelay_us(__IO u32 nTime);

extern int check_count;

#endif /* __SYSTICK_H */
