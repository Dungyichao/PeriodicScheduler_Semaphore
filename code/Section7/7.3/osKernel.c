#include "osKernel.h"

#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define ICSR         (*((volatile uint32_t *)0xE000ED04))   //(ICSR: Interrupt control and state register)

void  osSchedulerLaunch(void);

#define NUM_OF_THREADS  3        
#define STACKSIZE   100 
#define NUM_PERIODIC_TASKS 2


#define BUS_FREQ 				16000000
uint32_t MILLIS_PRESCALER;
uint32_t irq_temp, irq_temp1;
uint32_t temp;

typedef void(*taskT)(void);
#define NULL (void*)0

struct tcb{
  int32_t *stackPt;       
  struct tcb *nextPt;  
};

typedef struct{
  taskT task;
	uint32_t period;
}periodicTaskT;

static periodicTaskT	PeriodicTasks[NUM_PERIODIC_TASKS];
uint32_t TimeMsec;
uint32_t MaxPeriod;

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
	
uint8_t osKernelAddPeriodThreads( void(*thread1)(void),
																	uint32_t period1,
																	void(*thread2)(void),
																	uint32_t period2
																 )
{
	MaxPeriod  = period1 > period2 ? period1 : period2;
	uint32_t minPeriod =  period1 < period2 ? period1 :  period2;
	
	if(MaxPeriod %minPeriod !=0) return 0;
	
	PeriodicTasks[0].task  =  thread1;
	PeriodicTasks[0].period =  period1;
	PeriodicTasks[1].task   =  thread2;
  PeriodicTasks[1].period =  period2;

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

uint32_t period_tick;

void osSchedulerPeriodicRR(void)
{
	 if(TimeMsec < MaxPeriod)
	 {
		 TimeMsec++;
	 }
	 else
	 {
			TimeMsec = 1;
	 }
		
		int i;
		 for(i=0;i<NUM_PERIODIC_TASKS;i++){
		   if((TimeMsec % PeriodicTasks[i].period)==0 && PeriodicTasks[i].task !=NULL){
			   PeriodicTasks[i].task();
			 }
		 } 
		 currentPt  =  currentPt->nextPt;
}






