#include "osKernel.h"

#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))


void  osSchedulerLaunch(void);

#define NUM_OF_THREADS  3        
#define STACKSIZE   100      


#define BUS_FREQ 				16000000
uint32_t MILLIS_PRESCALER;

struct tcb{
  int32_t *stackPt;       
  struct tcb *nextPt;  
};

typedef struct tcb tcbType;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(int i){
  tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE-16]; 
  TCB_STACK[i][STACKSIZE-1] = 0x01000000;  
}



uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void))
{ 
	__disable_irq();
  tcbs[0].nextPt = &tcbs[1]; 
  tcbs[1].nextPt = &tcbs[2]; 
  tcbs[2].nextPt = &tcbs[0]; 
  osKernelStackInit(0);
  TCB_STACK[0][STACKSIZE-2] = (int32_t)(task0); 
  
  osKernelStackInit(1);
  TCB_STACK[1][STACKSIZE-2] = (int32_t)(task1); 
  
  osKernelStackInit(2);
  TCB_STACK[2][STACKSIZE-2] = (int32_t)(task2); 
  currentPt = &tcbs[0];
  __enable_irq();
	
  return 1;              
}
void osKernelInit(void)
{
     __disable_irq();
     MILLIS_PRESCALER=(BUS_FREQ/1000);

}
void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL =0;
	SysTick->VAL=0;
	SysTick->LOAD = (quanta* MILLIS_PRESCALER)-1;
        //SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0x70000000; // priority 7
	NVIC_SetPriority(SysTick_IRQn, 0x00000000);

	SysTick->CTRL =0x00000007;
	osSchedulerLaunch();
}

#define ICSR         (*((volatile uint32_t *)0xE000ED04))
	
void osThreadYield(void)
{ 
	SysTick->VAL = 0;
        ICSR = 0x04000000; // trigger SysTick
}

uint32_t period_tick;

//void osSchedulerRoundRobin(void)
//{
//	if((++period_tick)==PERIOD){
//	(*Task3)();
//		period_tick=0;
//	}
//	currentPt =  currentPt->nextPt;
//}

void osSemaphoreInit(int32_t *semaphore, int32_t value)
{
	*semaphore = value;
}

void osSignalSet(int32_t *semaphore)
{
	__disable_irq();
	*semaphore += 0x01;
	__enable_irq();
}

void osSignalWait(volatile int32_t *semaphore)
{
	__disable_irq();
	while(*semaphore <=0)
	{	
		__enable_irq();
		//osThreadYield();  // Non-preemptive method
		__disable_irq();			
	}
	*semaphore -= 0x01;
	__enable_irq();
}



