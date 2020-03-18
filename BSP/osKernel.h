#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include "stm32f4xx.h"                  // Device header
#include <stdint.h>

void osKernelLaunch(uint32_t quanta);

void osKernelInit(void);

uint8_t osKernelAddThreads(void(*task0)(void),
													 void(*task1)(void),
													 void(*task2)(void));

void osSemaphoreInit(int32_t *semaphore, int32_t value);

void osThreadYield(void);
void osSignalSet(int32_t *semaphore);
void osSignalWait(volatile int32_t *semaphore);													 
//#define PERIOD		100
//void Task3(void);

#endif
													 
													 