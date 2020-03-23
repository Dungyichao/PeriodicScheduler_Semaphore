#include "osKernel.h"

#define QUANTA	1

uint32_t count0,count1,count2;
uint32_t pcount1,pcount2, pcount3;


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

void periodicTask3(void){
	
	pcount3++;
}



int main(void)
{
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);
	
	osKernelAddPeriod_Thread(&periodicTask1,10);
	osKernelAddPeriod_Thread(&periodicTask2,100);
	osKernelAddPeriod_Thread(&periodicTask3,200);
	
	osKernelLaunch(QUANTA);
}

