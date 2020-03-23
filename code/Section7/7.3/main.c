#include "osKernel.h"
#include "timer.h"

#define QUANTA	1

uint32_t count0,count1,count2, pcount1,pcount2;
uint32_t ipcount1;

void Task0(void)
{
	while(1)
	{
		count0++;
		//osThreadYield();
	}
	
}


void Task1(void)
{
	while(1)
	{
		count1++;
	}
	
}


void Task2(void)
{
	while(1)
	{
		count2++;
	}
	
}

void periodicTask1(void){
	pcount1++;
}

void periodicTask2(void){
	pcount2++;
}


int main(void)
{
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);
	osKernelAddPeriodThreads(&periodicTask1,10,&periodicTask2,100);
	
	osKernelLaunch(QUANTA);
}

void TIM3_IRQHandler(void){
	TIM3->SR =0;
	ipcount1++;
}

