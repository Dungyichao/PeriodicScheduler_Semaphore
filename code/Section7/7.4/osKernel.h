#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include "stm32f4xx.h"                  // Device header
#include <stdint.h>
void osKernelLaunch(uint32_t quanta);

void osKernelInit(void);
void osThreadYield(void);
uint8_t osKernelAddThreads(void(*task0)(void),
													 void(*task1)(void),
													 void(*task2)(void));
void osPeriodicTask_Init(void(*task)(void), uint32_t freq, uint8_t priority);
void periodic_events_execute(void);
uint8_t osKernelAddPeriod_Thread(void (*task)(void), uint32_t period);													 
#endif
													 
													 													 