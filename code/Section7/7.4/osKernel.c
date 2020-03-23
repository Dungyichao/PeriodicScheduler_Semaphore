#include "osKernel.h"

#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define ICSR         (*((volatile uint32_t *)0xE000ED04))   //(ICSR: Interrupt control and state register)

void  osSchedulerLaunch(void);

#define NUM_OF_THREADS  3        
#define STACKSIZE   100   
#define NUM_PERIODIC_TASKS 5



#define BUS_FREQ 				16000000
uint32_t MILLIS_PRESCALER;
uint32_t irq_temp, irq_temp1;
uint32_t temp;

typedef void(*taskT)(void);
#define NULL (void*)0

typedef struct{
  	taskT task;
	uint32_t period;
	uint32_t  timeLeft;

}periodicTaskT;

periodicTaskT	PeriodicTasks[NUM_PERIODIC_TASKS];
uint32_t TimeMsec;
uint32_t MaxPeriod;
int32_t NumPeriodicThreads =0;

struct tcb{
  int32_t *stackPt;       
  struct tcb *nextPt;  
};

typedef struct tcb tcbType;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(int i){
  tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE-16]; //-16
  TCB_STACK[i][STACKSIZE-1] = 0x01000000;  
	for(int j = 0; j < 92; j++)
	{
		TCB_STACK[i][j] = (int32_t)&TCB_STACK[i][j];
	}
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
		osPeriodicTask_Init(periodic_events_execute,1000,6);

}
void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL =0;
	SysTick->VAL=0;
	SysTick->LOAD = (quanta* MILLIS_PRESCALER)-1;
  //SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0x70000000; // priority 7   |0xE0000000;
//	irq_temp = NVIC_GetPriority(SysTick_IRQn);
	//NVIC_SetPriority(SysTick_IRQn, 0x7);
//	irq_temp1 = NVIC_GetPriority(SysTick_IRQn);
//	temp = 0;
	
	

	SysTick->CTRL =0x00000007;
	osSchedulerLaunch();
}

#define ICSR         (*((volatile uint32_t *)0xE000ED04))
	
void osThreadYield(void)
{ 
	 SysTick->VAL=0;
   ICSR = 0x04000000; // trigger SysTick

}

void SysTick_Handler(void)
{ 
   //Trigger PendSV
   ICSR = 0x10000000; //  Bit28. Change PendSV exception state to pending. trigger PendSV  
}

uint8_t osKernelAddPeriod_Thread(void (*task)(void), uint32_t period)
{
  if(NumPeriodicThreads ==  NUM_PERIODIC_TASKS || period == 0){
	  return 0;
	}
	
	PeriodicTasks[NumPeriodicThreads].task = task;
	PeriodicTasks[NumPeriodicThreads].period =  period;
	PeriodicTasks[NumPeriodicThreads].timeLeft =  period -1;
	
	NumPeriodicThreads++;
	return 1;
	
}


void periodic_events_execute(void){
 
	int i;
	 for(i=0;i<NumPeriodicThreads;i++){
	   if(PeriodicTasks[i].timeLeft ==0){
			 PeriodicTasks[i].task();
			 PeriodicTasks[i].timeLeft = PeriodicTasks[i].period -1;
	    }
	    else{
		  PeriodicTasks[i].timeLeft--;
	     }
	 }	 
}

void osSchedulerPeriodicRR(void)
{		 
		 currentPt  =  currentPt->nextPt;
}

void (*PeriodicTask)(void);

void osPeriodicTask_Init(void(*task)(void), uint32_t freq, uint8_t priority)
{

	 //__disable_irq();
	 PeriodicTask = task;

   	 RCC->APB1ENR |=1;
   	 TIM2->PSC =  16-1;    /* 16 000 000 / 16 = 1 000 000*/
	 TIM2->ARR =  (1000000/freq)-1;
  	 TIM2->CR1 =1;
	 
	 TIM2->DIER |=1;
	 NVIC_SetPriority(TIM2_IRQn,priority);
	 NVIC_EnableIRQ(TIM2_IRQn);
}


void TIM2_IRQHandler(void){
  TIM2->SR  =0;
	(*PeriodicTask)();
}



