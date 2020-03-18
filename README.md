# Task Scheduler
Task scheduling is an important concept in Real-Time Operating Systems (Task, thread, process are interchangeable). Take your daily life for example, when trying to listen music on your cellphone while surfing news website through the internet (reaceiving data from wi-fi router in the mean time). How does it work under only one core of CPU (CPU can only execute one command at a time)? Each task such as open the browser may requires hundred or thousand line of codes, then how can we listen music and open website at the same time? It comes to task scheduling. We will use <b>STM32F411RE Nucleo64</b> and <b>ST7735S TFT LCD</b> to demonstrate the concept. 

# 1. What is the Goal <br />
Let's first take a look what are we going to achieve after this tutorial.
<p align="center">
<img src="/img/semaphore.gif" height="80%" width="80%"> 
</p>  
That is really simple!!! You can see 3 line of string Task0, Task1, Task2 with different color. Totally we have three tasks, and each task keep displaying their own string. Please refer to the following link to read the tutorial of displaying color and character on LCD display:

[link](https://github.com/Dungyichao/STM32F4-LCD_ST7735s)


# 2. Basic Knowledge <br />
Let's take a look at the following animation of the task scheduling concept (Preemptive Scheduling).
<br />
<p align="center">
<img src="/img/scheduler_concept_1.gif" height="40%" width="40%"> 
</p>  
<br />
You can think of Task 0 as music player, Task 1 as Chrome web browser, and Task 2 as processing Wi-Fi data. There are two way of task scheduling: <b>Preemptive Scheduling</b> and <b>Non-Preemptive Scheduling</b>. 

Reference: [link](https://www.geeksforgeeks.org/preemptive-and-non-preemptive-scheduling/)
<p align="center">
<table>
    <thead>
        <tr>
            <th align="center">Task scheduling</th>
            <th align="center">Detail</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">Preemptive Scheduling</td>
            <td align="Left">The resources (mainly CPU cycles) are allocated to the process for the limited amount of time and then is taken away, and the process is again placed back in the ready queue if that process still has CPU burst time remaining. That process stays in ready queue till it gets next chance to execute. <br /> <br /> Algorithms: Round Robin (RR),Shortest Remaining Time First (SRTF), Priority (preemptive version), etc.</td>
        </tr>
        <tr>
            <td align="center">Non-Preemptive Scheduling</td>
            <td align="Left">Once the resources (CPU cycles) is allocated to a process, the process holds the CPU till it gets terminated or it reaches a waiting state. In case of non-preemptive scheduling does not interrupt a process running CPU in middle of the execution. Instead, it waits till the process complete its CPU burst time and then it can allocate the CPU to another process. <br /> <br /> Algorithms: Shortest Job First (SJF basically non preemptive) and Priority (non preemptive version), etc.</td>
        </tr>
    </tbody>
</table>
</p>

# 3. Code Concept of Preemptive Scheduling with Round Robin <br />
The idea is basically the same as the first animation. Now, we add more detail (<b>registers</b> and <b>stack</b>) in the animation. We all know CPU use <b>registers</b> to store computing data or fetch computing data for temporary. When you are calculating some complicated math problem, you write down the value on sticky note (registers), when you read the equation and calculate the equation in your mind (CPU) and then write down the result back to the sticky note. While you are solving math problem, your Mom calls you, then you quickly write down the lastest calculation result (in registers) onto a paper (<b>stack</b>). You can now write down what your Mom tells you on the sticky note. After finishing the call, you can go back to solve your math problem by restoring what you left on the paper (stack) to the sticky note (registers) and keep going on. 

We explain the concept and some part of the code in this section. To make the code run successfully, please see the section 4. (Some code will be modified and put into some functions for better code management) 

<p align="center">
<img src="/img/RoundRobbin_Stack.png" height="70%" width="70%">
</p>
<br />

<p align="center">
<table>
    <thead>
        <tr>
            <th align="center">Item</th>
            <th align="center">Example</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">Registers</td>
            <td align="Left">Registers store value for CPU fetching or storing temporary data. The data accessing speed of registers is way more faster than RAM.</td>
        </tr>
        <tr>
            <td align="center">Stack</td>
            <td align="Left">We assign a place in RAM as stack. When exception happens, CPU would store some of the registers value into the stack. After the CPU finish handling the exception, CPU can restore those data (stored in the stack) back to registers and keep doing on what the CPU has left. </td>
        </tr>
    </tbody>
</table>
</p>

### 3.1 The Essential Elements of the Code
There are some elements and concept (based on Cortex M4) require more elaboration before we start introducing the code.
<p align="center">
<img src="/img/elements2.JPG" height="80%" width="80%">
</p>
<br />

<p align="center">
<table>
    <thead>
        <tr>
            <th align="center">Element</th>
            <th align="center">Concept</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">Registers</td>
            <td align="Left"><b>R0~R12</b>: General-purpose registers. Maybe store the function parameter, store the function return value<br /><br /><b>R13(SP)</b>: Stack Pointer. There are 2 kinds, Main Stack Pointer(MSP) and Process Stack Pointer(PSP). When an exception happens, a stack frame (R0~R3, R12, R14, R15, xPSR) gets pushed to the currently active stack pointer.<br /><br /><b>R14(LR)</b>:Link Register. LR is used to store the return program counter (PC) when a subroutine or function is called. On reset, the processor loads the LR value 0xFFFFFFFF.<br /><br /><b>R15(PC)</b>:Program Counter. It store the address of the next command or instruction.<br /><br /><b>xPSR</b>:Special-purpose Program Status Registers. The reset value is 0x01000000. <br /> * ASPR:  contains the current state of the condition flags from previous instruction executions.  <br /> * IPSR: contains the exception type number of the current Interrupt Service Routine(ISR).  <br /> * EPSR: contains the Thumb state bit, and the execution state bits. </td>
        </tr>        
        <tr>
            <td align="center">Stack</td>
            <td align="Left">Think of the stack as a container which implement Last-In-First-Out(LIFO) rule. Take a look at the following animation to understand the stack operation rule.<br /><br /><p align="center">
<img src="/img/stack.gif" height="50%" width="50%"></p><br />However, Cortex-M4 implements the stack in a different way. It implements a <b>full-descending stack</b>(http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/BABIAJHJ.html) .<br /><br /><p><code>PUSH</code> stores registers on the stack, with the lowest numbered register using the lowest memory address and the highest numbered register using the highest memory address.<br /><p><code>POP</code> loads registers from the stack, with the lowest numbered register using the lowest memory address and the highest numbered register using the highest memory address.</p><br /><p align="center">
<img src="/img/full-descend-stack2.gif" height="70%" width="70%"></p><br /></p></td>
        </tr>
        <tr>
            <td align="center">Task</td>
            <td align="Left">Keep doing some jobs in a task. Let's take a look at a simple task we are going to implement in our tutorial.<pre>
 void TaskX(void)
    {
	while(true)
	{
       		countX = countX + 1;	
	}
    }
</pre>This is simply a function with no return value and no input parameter. Inside the function is a while loop which will never stop. We will have three tasks, Task0(keep counting count0), Task1(keep counting count1), Task2(keep counting count2). In normal condition, whenever you enter a while(true) loop, you will never go out this loop untill you close the program because the condition is always true. You can add more instructions inside the while loop. </td>
        </tr>
        <tr>
            <td align="center">Context Switch</td>
            <td align="Left">The operation of switching from one task to another is known as a context switch. A Real Time Operating System (RTOS) will typically provide this functionality.Reference link: (https://interrupt.memfault.com/blog/cortex-m-rtos-context-switching) <br /> We have three tasks and each task keep doing their own job without breaking the loop, then how can we do task0, task1, task2? Context switch comes to the place and will switch task to another and then after visiting all the tasks, it will come back loop through all the tasks again and again.</td>
        </tr>
        <tr>
            <td align="center">Exception</td>
            <td align="Left">Exception is anything that breaks the normal program flow, and invokes a handler from the vector table, and Interrupts are a subset of Exceptions, coming from the peripherals outside the ARM core. Exceptions have an Exception Number, starting from 0. Interrupt(IRQ) have an IRQ Number, starting from 0. Because all Interrupts are Exceptions, they all get an Exception Number, which is 16 higher than the IRQ Number. Exceptions that are not Interrupts have IRQ Numbers too, which by extension fall into the range from -16 to -1. <p align="center">
<img src="/img/exception_vector.JPG" height="60%" width="60%">
		    </p><br />In this section, we only use <b>Systick</b> as a trigger to let us perform the context switch. A SysTick exception is an exception the system timer generates when it reaches zero. Software can also generate a SysTick exception. In an OS environment, the processor can use this exception as system tick.<br />Reading resource: https://interrupt.memfault.com/arm-cortex-m-exceptions-and-nvic#built-in-exceptions</td>
        </tr>
	<tr>
            <td align="center">Thread Control Block</th>
            <td align="Left">Think of this as a summary of all the tasks. It should contain the information of where the current task's information and where is the next task information.</th>
        </tr>
    </tbody>
</table>
</p>

### 3.2 Implement Task, Stack, Thread Control Block and Context Switch <br />
#### 3.2.1 Task <br />
We initialize 3 task functions. This is simply a function with no return value and no input parameter. Inside the function is a while loop which will never stop. 
```c++
uint32_t count0,count1,count2;

void Task0(void)
{
	while(1)
	{
		count0++;
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

```

#### 3.2.2 Stack <br />
We initialize a 2D array names TCB_STACK, and each element datatype is int32_t. 
```c++
#define NUM_OF_THREADS  3        
#define STACKSIZE   100
int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];
```
We also put some information into the stack in advance.
```c++
TCB_STACK[0][99] = 0x01000000;   //xPSR will be reset
TCB_STACK[1][99] = 0x01000000;   //xPSR will be reset
TCB_STACK[2][99] = 0x01000000;   //xPSR will be reset

TCB_STACK[0][98] = (int32_t)(task0); // The address where store the instruction of task0
TCB_STACK[1][98] = (int32_t)(task1); // The address where store the instruction of task1
TCB_STACK[2][98] = (int32_t)(task2); // The address where store the instruction of task2
```
The visualization of the stack is in the following
<p align="center">
<img src="/img/stack_init.png" height="70%" width="70%">
</p>

#### 3.2.3 Thread Control Block <br />

```c++
struct tcb{
  int32_t *stackPt;       
  struct tcb *nextPt;  
};

typedef struct tcb tcbType;     //name tcb as tcbType
tcbType tcbs[NUM_OF_THREADS];   //tcbs[0][0] stores task0 stack pointer, tcbs[0][1] stores task1 stack pointer
tcbType *currentPt;             //point to the current running task's tcb

currentPt = &tcbs[0];      //We will first do task 0, so store the stack0 pointer

tcbs[0].stackPt = &TCB_STACK[0][84]; //mimic R0~R12, R14, R15, xPSR   (we do this for context switch)
tcbs[1].stackPt = &TCB_STACK[1][84]; //mimic R0~R12, R14, R15, xPSR   (we do this for context switch)
tcbs[2].stackPt = &TCB_STACK[2][84]; //mimic R0~R12, R14, R15, xPSR   (we do this for context switch)

tcbs[0].nextPt = &tcbs[1];      // after doing task0, next should do task1, so store stack1 pointer
tcbs[1].nextPt = &tcbs[2];      // after doing task1, next should do task2, so store stack2 pointer
tcbs[2].nextPt = &tcbs[0];      // after doing task2, next should do task0, so store stack0 pointer
```
The visualization of the Thread Control Block and the stack is in the following
<p align="center">
<img src="/img/Stack_TCB.JPG" height="70%" width="70%">
</p>

#### 3.2.4 Context Switch (Part I) <br />
The assembly code in the following will only be executed once. This function brings the Task0 information stored in stack0 to the register. Most important of all is that it stored the task0 address into Link Register (R14) so that when processor exit this function, the processor will be led to task0 and do the task in task0. 

```c++
		AREA |.text|, CODE, READONLY, ALIGN=2
                THUMB
		EXPORT  osSchedulerLaunch
		
osSchedulerLaunch
    LDR     R0, =currentPt         
    LDR     R2, [R0]       ; R2 =currentPt       
    LDR     SP, [R2]       ;SP = currentPt->stackPt    
    POP     {R4-R11}          
    POP     {R0-R3}            
    POP     {R12}
    ADD     SP,SP,#4           
    POP     {LR}               
    ADD     SP,SP,#4         
    CPSIE   I                 
    BX      LR                 

    ALIGN
    END
```
The visualization of the osSchedulerLaunch and the stack is in the following 
<p align="center">
<img src="/img/Process2.png" height="100%" width="100%">
</p>

#### 3.2.5 Context Switch (Part II) <br />
The assembly code in the following will be executed when the SysTick_Handler be called by the Systick Exception occured. When the Systick Exception occured, the processor will first store the data on the registers (R0~R3, R12, LR, PC, xPSR) into the stack (pointed by the current active SP which should be the current task's stack) and then executes the following code.
```c++
		AREA |.text|, CODE, READONLY, ALIGN=2
                THUMB
		EXTERN  currentPt
		EXPORT	SysTick_Handler

SysTick_Handler             ;save r0,r1,r2,r3,r12,lr,pc,psr      
    CPSID   I                  
    PUSH    {R4-R11}        ;save r4,r5,r6,r7,r8,r9,r10,r11   
    LDR     R0, =currentPt  ; r0 points to currentPt       
    LDR     R1, [R0]        ; r1= currentPt   
    STR     SP, [R1]           
    LDR     R1, [R1,#4]     ; r1 =currentPt->next   
    STR     R1, [R0]        ;currentPt =r1   
    LDR     SP, [R1]        ;SP= currentPt->stackPt   
    POP     {R4-R11}           
    CPSIE   I                  
    BX      LR 
```
The visualization of the SysTick_Handler and the stack is in the following 
<p align="center">
<img src="/img/Process3.png" height="90%" width="90%">
</p>

#### 3.2.6 Configure SysTick <br />
```c++
#define BUS_FREQ   16000000
#define QUANTA	1
uint32_t MILLIS_PRESCALER;

MILLIS_PRESCALER=(BUS_FREQ/1000);
SysTick->CTRL =0;   //Disable the SysTick timer; Offset: 0x000 (R/W)  SysTick Control and Status Register
SysTick->VAL=0;     //Clear current value to 0; Offset: 0x008 (R/W)  SysTick Current Value Register
NVIC_SetPriority(SysTick_IRQn, 0x0);   // This is not necessary because we don't implement other interrupt.
SysTick->LOAD = (QUANTA * MILLIS_PRESCALER)-1;   //Offset: 0x004 (R/W)  SysTick Reload Value Register
SysTick->CTRL =0x00000007;

/*
Since the SysTick timer counts down to 0, if you want to set the SysTick interval to 1000, 
you should set the reload value (SysTick->LOAD) to 999
*/
```
For more SysTick configure, please refer to the Cortex-M4 Devices Generic Use Guide (page 4-33) or the following link: https://www.sciencedirect.com/topics/engineering/systick-interrupt

<p align="center">
<img src="/img/Systick_ctrl.png" height="80%" width="80%">
</p>

### 3.3 Result <br />
Let's execute the code (provided in the folder Simple_code) and enter the debug view to monitor count0, count1, count2. You will see those 3 values are counting at the same time.
<p align="center">
<img src="/img/simple_result.gif" height="60%" width="60%">
</p>

#### 3.4 Non-preemptive method (Optional) 
If we add the following function in every task right after countX = countX + 1, the result will act a little bit different. Each countX will be add up only once and then the counting resource will be handed to the next task. What osThreadYeild does is to make the SysTick timer current value to 0 and set the SysTick exception state to pending. Therefore, the SysTick_Handler will be called and performs the context switching to the next task. 
```c++
#define ICSR         (*((volatile uint32_t *)0xE000ED04))   //(ICSR: Interrupt control and state register)
void osThreadYield(void)
{ 
   SysTick->VAL=0;
   ICSR = 0x04000000; //  Bit26. Change SysTick exception state to pending. trigger SysTick  

}
```
<p align="center">
<img src="/img/simple_yield_result.gif" height="60%" width="60%">
</p>
The above result is the slow down motion in the debug view. 

For more information of the ICSR pleas refer to the Cortex-M4 Generic User Guide pdf file or the following image.
<p align="center">
<img src="/img/ICSR.png" height="100%" width="100%">
</p>

# 4. Process Synchronization
We are using <b>Semaphore</b> to achieve process synchronization in the multiprocessing environment. It is like passing one token around these tasks. Only the task with the token can be executed. Please refer to the following link for more information of the Semaphore.
[Semaphores](https://www.geeksforgeeks.org/semaphores-in-process-synchronization/),
[Process Synchronization](https://www.geeksforgeeks.org/introduction-of-process-synchronization/)

### 4.1 Spin-lock semaphore <br />
In the following, we will implement spin-lock semaphore.
[reference link](http://users.ece.utexas.edu/~valvano/EE345M/view06_semaphores.pdf)
(please see page 7). The semaphore just like the token. There are three critical elements: Initial function, semaphore value setting, and waiting function.

```c++
//Initial function
void osSemaphoreInit(int32_t *semaphore, int32_t value)
{
	*semaphore = value;
}

//Semaphore value setting
void osSignalSet(int32_t *semaphore)
{
	__disable_irq();
	*semaphore += 0x01;
	__enable_irq();
}

//waiting function
void osSignalWait(volatile int32_t *semaphore)
{
	while(*semaphore <=0)
	{		
		__disable_irq();		
		__enable_irq();
	}
	*semaphore -= 0x01;
	__enable_irq();
}
```

The task will look like the following (from section 4.3.1)
```c++
int32_t semaphore0,semaphore1,semaphore2;
void Task0(void)
{
	while(1)
	{
    		osSignalWait(&semaphore0);
		count0 = count0 + 1;
    		osSignalSet(&semaphore1);
	
	}
}

void Task1(void)
{
	while(1)
	{
		osSignalWait(&semaphore1);
		count1 = count1 + 1;
   		osSignalSet(&semaphore2);
	}
}

void Task2(void)
{
	while(1)
	{
		osSignalWait(&semaphore2);
		count2 = count2 + 1;
    		osSignalSet(&semaphore0);
	}
}

int main(void)
{
  osSemaphoreInit(&semaphore1,1);
  osSemaphoreInit(&semaphore2,0);
  osSemaphoreInit(&semaphore3,0);
}
```
So what does it mean? In the main function, we first initialize the semaphore (token) value. Each task has its own token. In the main function, only one token can be initialized with value larger than 0 (to make sure only one task can access the cpu untill it complete the task). We first set the Task0's semaphore0 (token) larger than 0. When first enter the task0 function, the osSinalWait function will examine the semaphore0, if the token is larger than 0, skip the while loop and set the token to 0, and then do the job count0 = count0 + 1. After completing the job, it will pass the token to the next task (Task1) by using the function osSignalSet to set the semaphore1 to 1. Thus the Task1 can be processed. 

I was stuck in the while loop of function osSinalWait when I first learning it, the following link is my question and answer on Stack Overflow: 
[link](https://stackoverflow.com/questions/60724667/stm32-same-while-loop-code-but-compiled-to-different-assembly-code)
.

### 4.2 Cooperative spin-lock semaphore <br />
One major disadventage of spin-lock semaphore is that resources are being held and doing nothing if the current task's semaphore's value is 0 (stuck in the while loop in the waiting function until the SysTick_Handler exception occured). To solve this problem, we introduce Cooperative spin-lock semaphore. Actually, it require only one line of code added to the waiting function. 

```c++
void osSignalWait(volatile int32_t *semaphore)
{
	while(*semaphore <=0)
	{		
		__disable_irq();
		osThreadYield();  // see section 3.4 in this tutorial
		__enable_irq();
	}
	*semaphore -= 0x01;
	__enable_irq();
}
```

The osThreadYield() function is inserted in the while loop. That's great, we can hand the resources to the next task right away while the current task (semaphore < 0) is waiting. 

# 5. Implement on LCD
We are approaching the goal. However, we cannot not directly apply the same code from the previous LCD tutorial ( [link](https://github.com/Dungyichao/STM32F4-LCD_ST7735s) ) to the code here. The LCD tutorial use SysTick_Handler() to trigger the countdown of the HAL_Delay(). In this Task Scheduler, we are using SysTick_Handler() to do the context switch, thus, we need to use other timer to trigger the countdown for the HAL_Delay(). Why the HAL_Delay is important to us, that is for the LCD initialization. We will show you how to achieve it now. 


### 4.1 Configure CubeMX



