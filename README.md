# Real-Time Operating Systems (RTOS)
Task scheduling is an important concept in Real-Time Operating Systems (Task, thread, process are interchangeable). Take your daily life for example, when trying to listen to music on your cellphone while surfing news websites through the internet (receiving data from wi-fi router in the meantime). How does it work under only one core of CPU (CPU can only execute one command at a time)? Each task such as open the browser may require hundred or thousand lines of codes, then how can we listen to music and open websites at the same time? It comes to task scheduling. We will use <b>STM32F411RE Nucleo64</b> and <b>ST7735S TFT LCD</b> to demonstrate the concept. If any reference link in this tutorial cannot be accessed, please refer to the Reference folder above.

1. [What is the Goal](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#1-what-is-the-goal-)
2. [Basic Knowledge](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#2-basic-knowledge-)
3. [Code Concept of Preemptive Scheduling with Round Robin](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#3-code-concept-of-preemptive-scheduling-with-round-robin-)
    * 3.1 [The Essential Elements of the Code](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#31-the-essential-elements-of-the-code)
    * 3.2 [Implement Task, Stack, Thread Control Block and Context Switch](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#32-implement-task-stack-thread-control-block-and-context-switch-)
       * 3.2.1 [Task](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#321-task-)
       * 3.2.2 [Stack](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#322-stack-)
       * 3.2.3 [Thread Control Block](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#323-thread-control-block-)
       * 3.2.4 [Context Switch (Part I)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#324-context-switch-part-i-)
       * 3.2.5 [Context Switch (Part II)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#325-context-switch-part-ii-)
       * 3.2.6 [Configure SysTick](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#326-configure-systick-)
    * 3.3 [Result](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#33-result-)
    * 3.4 [Non-preemptive method (Optional)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#34-non-preemptive-method-optional)
4. [Process Synchronization](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#4-process-synchronization)
     * 4.1 [Spin-lock semaphore](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#41-spin-lock-semaphore-)
     * 4.2 [Cooperative spin-lock semaphore](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#42-cooperative-spin-lock-semaphore-)
     * 4.3 [Rendezvous (Optional)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#43-rendezvous-optional)
5. [Implement on LCD](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#5-implement-on-lcd-)
     * 5.1 [Configure CubeMX](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#51-configure-cubemx-)
     * 5.2 [Download code and Copy to our project](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#52-download-code-and-copy-to-our-project-)
     * 5.3 [Fix the duplicate SysTick_Handler problem in the code](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#53-fix-the-duplicate-systick_handler-problem-in-the-code-)
     * 5.4 [Include header file and clear out comment in main.c](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#54-include-header-file-and-clear-out-comment-in-mainc-)
     * 5.5 [Compile and Download to the board](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#55-compile-and-download-to-the-board)
     

------------------------------------Advance (Optional)------------------------------------------- <br />

6. [Using PendSV for Context Switch](https://github.com/Dungyichao/PeriodicScheduler_Semaphore/blob/master/README.md#6-using-pendsv-thread-switcher)
7. [Period Scheduling](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#7-period-scheduling)
	* 7.1 [Method 1 - Multiple Periodic Threads](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#71-method-1---multiple-periodic-threads)
	* 7.2 [Method 2 - Timer Interrupts](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#72-method-2---timer-interrupts)
	* 7.3 [Method 3 - Thread Control Blocks (TCB)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#73-method-3---thread-control-blocks-tcb)
	* 7.4 [TCB + Timer Interrupts](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#74-tcb--timer-interrupts)
	* 7.5 [Method 4 - Thread Sleep](https://github.com/Dungyichao/PeriodicScheduler_Semaphore/blob/master/README.md#75-method-4---thread-sleep)
8. [Inter-Thread Data Transfer](https://github.com/Dungyichao/PeriodicScheduler_Semaphore/blob/master/README.md#8-inter-thread-data-transfer)
9. [Reference and conclusion](https://github.com/Dungyichao/PeriodicScheduler_Semaphore/blob/master/README.md#7-reference-and-conclusion)


# 1. What is the Goal <br />
Let's first take a look at what are we going to achieve after this tutorial.
<p align="center">
<img src="/img/semaphore.gif" height="80%" width="80%"> 
</p>  
That is really simple!!! You can see 3 lines of string Task0, Task1, Task2 with a different colors. Totally we have three tasks, and each task keeps displaying their own string. Please refer to the following link to read the tutorial of displaying color and character on LCD display:

[https://github.com/Dungyichao/STM32F4-LCD_ST7735s](https://github.com/Dungyichao/STM32F4-LCD_ST7735s)


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
The idea is basically the same as the first animation. Now, we add more detail (<b>registers</b> and <b>stack</b>) in the animation. We all know CPU use <b>registers</b> to store computing data or fetch computing data for temporary. When you are calculating some complicated math problem, you write down the value on a sticky note (registers), when you read the equation and calculate the equation in your mind (CPU) and then write down the result back to the sticky note. While you are solving a math problem, your Mom calls you, then you quickly write down the lastest calculation result (in registers) onto a paper (<b>stack</b>). You can now write down what your Mom tells you on the sticky note. After finishing the call, you can go back to solve your math problem by restoring what you left on the paper (stack) to the sticky note (registers) and keep going on. 

We explain the concept and some parts of the code in this section. To make the code run successfully, please see section 5. (Some code will be modified and put into some functions for better code management) 

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
</pre>This is simply a function with no return value and no input parameter. Inside the function is a while loop which will never stop. We will have three tasks, Task0(keep counting count0), Task1(keep counting count1), Task2(keep counting count2). In normal conditions, whenever you enter a while(true) loop, you will never go out this loop until you close the program because the condition is always true. You can add more instructions inside the while loop. </td>
        </tr>
        <tr>
            <td align="center">Context Switch</td>
            <td align="Left">The operation of switching from one task to another is known as a context switch. A Real Time Operating System (RTOS) will typically provide this functionality. Reference link: (https://interrupt.memfault.com/blog/cortex-m-rtos-context-switching) <br /> We have three tasks and each task keep doing their own job without breaking the loop, then how can we do task0, task1, task2? Context switch comes to the place and will switch the task to another and then after visiting all the tasks, it will come back loop through all the tasks again and again.</td>
        </tr>
        <tr>
            <td align="center">Exception</td>
            <td align="Left">Exception is anything that breaks the normal program flow, and invokes a handler from the vector table, and Interrupts are a subset of Exceptions, coming from the peripherals outside the ARM core. Exceptions have an Exception Number, starting from 0. Interrupts (IRQ) have an IRQ Number, starting from 0. Because all Interrupts are Exceptions, they all get an Exception Number, which is 16 higher than the IRQ Number. Exceptions that are not Interrupts have IRQ Numbers too, which by extension fall into the range from -16 to -1. <p align="center">
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

Let's use function to better manage the code. The following codes and function do the exactly same thing in above code.
```c++
struct tcb{
  int32_t *stackPt;       
  struct tcb *nextPt;  
};

typedef struct tcb tcbType;     //name tcb as tcbType
tcbType tcbs[NUM_OF_THREADS];   //tcbs[0][0] stores task0 stack pointer, tcbs[0][1] stores task1 stack pointer
tcbType *currentPt;             //point to the current running task's tcb

currentPt = &tcbs[0];      //We will first do task 0, so store the stack0 pointer

void osKernelStackInit(int i){
  tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE-16]; //-16
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
```
The visualization of the Thread Control Block and the stack is in the following
<p align="center">
<img src="/img/Stack_TCB.JPG" height="70%" width="70%">
</p>

#### 3.2.4 Context Switch (Part I) <br />
The assembly code in the following will only be executed once. This function brings the Task0 information stored in stack0 to the register. Most important of all is that it stored the task0 address into Link Register (R14) so that when the processor exit this function, the processor will be led to task0 and do the task in task0. 

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
The assembly code in the following will be executed when the SysTick_Handler is called by the Systick Exception occurred. When the Systick Exception occured, the processor will first store the data on the registers (R0~R3, R12, LR, PC, xPSR) into the stack (pointed by the current active SP which should be the current task's stack) and then execute the following code. Make sure the <b>Floating Point Hardware</b> is not used (In the Configure Flash Tools, Target tab). 
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

void osKernelInit(void)
{
	 __disable_irq();
	MILLIS_PRESCALER=(BUS_FREQ/1000);
}

void osKernelLaunch(uint32_t quanta)
{
  SysTick->CTRL =0;   //Disable the SysTick timer; Offset: 0x000 (R/W)  SysTick Control and Status Register
  SysTick->VAL=0;     //Clear current value to 0; Offset: 0x008 (R/W)  SysTick Current Value Register
  NVIC_SetPriority(SysTick_IRQn, 0x0);   // This is not necessary because we don't implement other interrupt.
  SysTick->LOAD = (QUANTA * MILLIS_PRESCALER)-1;   //Offset: 0x004 (R/W)  SysTick Reload Value Register
  SysTick->CTRL =0x00000007;
  osSchedulerLaunch();  //in osKernel.s assembly code

  /*
  Since the SysTick timer counts down to 0, if you want to set the SysTick interval to 1000, 
  you should set the reload value (SysTick->LOAD) to 999
  */
}
```
For more SysTick configure, please refer to the Cortex-M4 Devices Generic Use Guide (page 4-33) or the following link: https://www.sciencedirect.com/topics/engineering/systick-interrupt

<p align="center">
<img src="/img/Systick_ctrl.png" height="80%" width="80%">
</p>

** Note. In the Udemy RTOS lecture, the instructor use the following code to define the SysTick priority. When I use ```NVIC_GetPriority(SysTick_IRQn)``` to check the SysTick priority, it doesn't show 7. 
```c++
#define SYSPRI3 (*((volatile uint32_t *)0xE000ED20))
SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; // priority 7      
```
The instructor gives some explanation, but I think that is not correct, and here is my research: https://github.com/Dungyichao/PeriodicScheduler_Semaphore/blob/master/Reference/Problem%20of%20setting%20the%20systick%20priority%20SYSPRI3.pdf


### 3.3 Result <br />
In the main.c, we can call the above function to initialize the stack and thread
```c++
#define QUANTA	1

int main(void)
{
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);	
	osKernelLaunch(QUANTA);
}
```

Let's execute the code (provided in the folder BSP) and enter the debug view to monitor count0, count1, count2. You will see those 3 values are counting at the same time.
<p align="center">
<img src="/img/simple_result.gif" height="60%" width="60%">
</p>

### 3.4 Non-preemptive method (Optional) 
Non-preemptive means the context switching mechanism not trigger by the exception. Only when the task reach any specified point that the context switch can be executed. If we add the following function in every task right after countX = countX + 1, the result will act a little bit differently. Each countX will add up only once and then the counting resource will be handed to the next task. What osThreadYeild does is to make the SysTick timer current value to 0 and set the SysTick exception state to pending. Therefore, the SysTick_Handler will be called and performs the context switching to the next task. 
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

The task will look like the following (from section 3.2.1)
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
  ...... Task0/1/2, tcbs, TCB_STACK initialize 
  ......
}
```
So what does it mean? In the main function, we first initialize the semaphore (token) value. Each task has its own token. In the main function, only one token can be initialized with value larger than 0 (to make sure only one task can access the CPU until it completes the task). We first set the Task0's semaphore0 (token) larger than 0. When first enter the task0 function, the osSinalWait function will examine the semaphore0, if the token is larger than 0, skip the while loop and set the token to 0, and then do the job count0 = count0 + 1. After completing the job, it will pass the token to the next task (Task1) by using the function osSignalSet to set the semaphore1 to 1. Thus the Task1 can be processed. 

I was stuck in the while loop of function osSinalWait when I first learning it, the following link is my question and answer on Stack Overflow: 
[https://stackoverflow.com/questions/60724667/stm32-same-while-loop-code-but-compiled-to-different-assembly-code](https://stackoverflow.com/questions/60724667/stm32-same-while-loop-code-but-compiled-to-different-assembly-code)
.

### 4.2 Cooperative spin-lock semaphore <br />
One major disadvantage of spin-lock semaphore is that resources are being held and doing nothing if the current task's semaphore's value is 0 (stuck in the while loop in the waiting function until the SysTick_Handler exception occurred). To solve this problem, we introduce the Cooperative spin-lock semaphore. Actually, it requires only one line of code added to the waiting function. 

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

### 4.3 Rendezvous (Optional) 

This is a bilateral synchronisation so we only use 2 task to do the example. Both the tasks need to wait another task to get the token (semaphore > 0) so that they can do the counting job.

```c++
uint32_t count0,count1;

void Task0(void)
{
	while(1)
	{
		osSignalSet(&semaphore0);
		osSignalWait(&semaphore1);
		count0++;
	}	
}

void Task1(void)
{
	while(1)
	{
		osSignalSet(&semaphore1);
		osSignalWait(&semaphore0);
		count1++;
	}	
}



int main(void)
{
  osSemaphoreInit(&semaphore0,0);
  osSemaphoreInit(&semaphore1,0);
  ...... Task0/1, tcbs, TCB_STACK initialize 
  ......
}

```
Notice that both the semaphore are initialized with 0. Look at the following result, you can see both count number are adding number at almost the same time. The difference between these two count number is almost 1. 

<p align="center">
<img src="/img/Rendezvous .gif" height="70%" width="70%"> 
</p>


# 5. Implement on LCD <br />
We are approaching the goal. However, we cannot directly apply the same code from the previous LCD tutorial ( [link](https://github.com/Dungyichao/STM32F4-LCD_ST7735s) ) to the code here. The LCD tutorial uses SysTick_Handler() to trigger the countdown of the HAL_Delay(). In this Task Scheduler, we are using SysTick_Handler() to do the context switch, thus, we need to use another timer to trigger the countdown for the HAL_Delay(). Why the HAL_Delay is important to us, that is for the LCD initialization. We will show you how to achieve it now. 


### 5.1 Configure CubeMX <br />

You can follow the following steps to configure the CubeMX. If you want to jump to the code directly, then you can simply download the CubeMX project in the above folder ```CubeMX``` and click Generate Code. This will give you exactly the same configuration of the later tutorial so that you can skip Section 5.1 and goining on with Section 5.2. 

<p align="center">
<table>
    <thead>
        <tr>
            <th align="center">Configure</th>
            <th align="center">Detail</th>
	    <th align="center">Purpose</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">Timers: TIM2</td>
	    <td align="Left"><b>Clock Source:</b> Internal Clock <br /> <b>Prescaler:</b> 16000 <br /> <b>Counter Period:</b> 1 <br /> <b>Trigger Event Selection:</b> Update Event <br /><b>NVIC Settings:</b> click enabled <br /> </td>
	    <td align="Left">For HAL_Delay function. Important for LCD initialization</td>
        </tr>
        <tr>
            <td align="center">Connectivity: SPI1</td>
            <td align="Left"><b>Mode:</b> Transmit Only Master <br /> <b>Prescaler:</b> 32 <br /> <b>SPI1_MOSI: </b>PB5 <br /> <b>SPI1_SCK: </b>PB3 </td>
	    <td align="Left">For STM32F411 Nucleo64 board sending data to LCD</td>
        </tr>
	<tr>
            <td align="center">GPIO_Output</td>
            <td align="Left"><b>PB15: </b> Name it to LCD_CS <br /> <b>PB14: </b> Name it to LCD_RS <br /> <b>PB12: </b>Name it to LCD_RST<br /></td>
	    <td align="Left">For assisting STM32F411 Nucleo64 board sending data to LCD</td>
        </tr>
	 <tr>
            <td align="center">Clock Configure</td>
            <td align="Left"><b>HCLK (MHz):</b> 16 <br /></td>
            <td align="center">Slow down the clock</td>
        </tr>
    </tbody>
</table>
</p>

<br />

Why do we set the prescaler of TIM2 to 16000? First you see we set the HCLK (MHz) to 16MHz. This means the clock will now tick 16000000 times in 1 second. We want TIM2 to tick every 1 milisecond (ms), but the clock source is 16MHz, how do we do that? We use the prescaler, so now 16MHz / prescaler = 1000. It means the TIM2 now only ticks once for every 1 ms. 
<br />

<p align="center">
<img src="/img/cubemx_setup.gif" height="100%" width="100%"> 
</p>  


### 5.2 Download code and Copy to our project <br />
Download the code from the following link. 
[https://github.com/Dungyichao/PeriodicScheduler_Semaphore/tree/master/BSP](https://github.com/Dungyichao/PeriodicScheduler_Semaphore/tree/master/BSP)

<p align="center">
<img src="/img/download_code.JPG" height="80%" width="80%">
</p>
<br />

Copy these files (except main.c) into the MDK-ARM folder under your Project folder. Configure the <b>Include Paths</b>. Add the existing file to the project. 

<p align="center">
<img src="/img/download_copy.gif" height="100%" width="100%"> 
</p>  

You will find 1 error after compiling it. That is caused by the duplicate SysTick_Handler function defined in both stm32f4xx_it.c and our new copied osKernel.c (for context switching). 

### 5.3 Fix the duplicate SysTick_Handler problem in the code <br />
We than move the HAL_IncTick function (for HAL_Delay function usage) into TIM2_IRQHandler in the stm32f4xx_it.c file. Comment out the SysTick_Handler in stm32f4xx_it.c file and stm32f4xx_it.h file. Compile again and the error will disappear.
<p align="center">
<img src="/img/modify_code.gif" height="100%" width="100%"> 
</p>  

### 5.4 Include header file and clear out comment in main.c <br />
We first clear some comments (automatically generated from CubeMX). Add the header file (let the main.c use function in osKernel.c, st7735.c, and fonts.c) in the main.c file. We then add some code in main.c, please follow the following process.

<p align="center">
<img src="/img/process_add_code2.png" height="100%" width="100%">
</p>
<br />

The sequence is important especially for <b>E,F,G, H</b>. Before Initialize LCD (G step), we need to first disable the SysTick Exception (E step, make sure the HAL_Delay can function properly) and start the TIM2 (F step). H step requires the SysTick exception for context switching, so it is placed at the last position.

<p align="center">
<img src="/img/include_header_main.gif" height="100%" width="100%"> 
</p>  

### 5.5 Compile and Download to the board
Lastly, compile the code and download the code to the board. Make sure all the wire connection of LCD with your board are correct (see 
[tutorial](https://github.com/Dungyichao/STM32F4-LCD_ST7735s)
). You can modify the variable QUANTA (in the top of main.c) to 30 or even larger to make your LCD more stable.

You can play around the Initialization of semaphore by changing the 1 to the other semaphore and inspect what happens on the LCD.

# 6. Using PendSV for Context Switch
In order to free up the SysTick timer (so that we can do more important thing in SysTick_Handler), we will use SysTick_Handler to trigger PendSV to perform the context switching. FreeRTOS and Micrium also use PendSV to perform context switching.

It requires only a little modification. Please disable all semaphore elements. In osKernel.s, we change the SysTick_Handler to PendSV_Handler.

```c++
		AREA |.text|, CODE, READONLY, ALIGN=2
                THUMB
		EXTERN  currentPt
		EXPORT	PendSV_Handler

PendSV_Handler             ;save r0,r1,r2,r3,r12,lr,pc,psr      
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

If you encounter multiply defined of PendSV_Handler, please disable the PendSV_Handler defined in ```stm32f4xx_it.h``` and ```stm32f4xx_it.c``` file.

In the osKernel.c, we add

```c++
//(ICSR: Interrupt control and state register)
void SysTick_Handler(void)
{ 
   //Trigger PendSV
   ICSR = 0x10000000; //  Bit28. Change PendSV exception state to pending. trigger PendSV  
}
```
The SysTick_Handler will then trigger the PendSV.
For more information of ICSR, please check document: Cortex - M4 Devices Generic User Guide (page 4-14) provided in the folder ```Reference```. Look for ```PENDSVCLR``` and ```PENDSVSET```

# 7. Period Scheduling
In this section, we will introduce 4 method to achieve period scheduling. Please disable all the semaphore and LCD related elements. Why do we need period scheduling? Some tasks require being executed every SysTick exception while some tasks don't need to be executed so frequently. Thus, for those tasks only need longer period, we can adobt the following methods.

We will follow the code and function in section 3.2 
[(Implement Task, Stack, Thread Control Block and Context Switch)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#32-implement-task-stack-thread-control-block-and-context-switch-)
 and section 6
 [(Using PendSV for Context Switch)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#6-using-pendsv-for-context-switch)
.

### 7.1 Method 1 - Multiple Periodic Threads

In the osKernel.c, we add the following code. periodicTask1 and periodicTask2 only get executed when certain condition matches.

```c++
#define		PERIOD		100
uint32_t period_tick;

void osSchedulerRoundRobin(void)
{
	 period_tick++;
	
	if((period_tick%100)== 1){
	  (*periodicTask1)();   //periodicTask1 is defined in main.c
	}
	if((period_tick%200)== 1){
	  (*periodicTask2)();   //periodicTask2 is defined in main.c
	}
	currentPt =  currentPt->nextPt;
}
```

In osKernel.s, we modify the PendSV_Handler so it will branch to the function in above. 
```c++
			AREA |.text|,CODE,READONLY,ALIGN=2
			THUMB
			PRESERVE8
			EXTERN currentPt
		    	EXPORT PendSV_Handler
			IMPORT osSchedulerRoundRobin

PendSV_Handler   ;save r0,r1,r2,r3,r12,lr,pc,psr  
	CPSID	  I
	PUSH 	  {R4-R11}
	LDR 	  R0,=currentPt
	LDR	  R1,[R0]
	
	STR 	  SP,[R1]

    	PUSH	  {R0,LR}
	BL	  osSchedulerRoundRobin
	POP	  {R0,LR}
	LDR 	  R1,[R0]     ;R1 = currentPt i.e New thread. currentPt was changed in osSchedulerRoundRobin()
	LDR 	  SP,[R1]
	
	POP	  {R4-R11}
	CPSIE	  I
	BX	  LR
```
* PRESERVE8 info: 
[http://www.keil.com/support/man/docs/armasm/armasm_dom1361290021571.htm](http://www.keil.com/support/man/docs/armasm/armasm_dom1361290021571.htm)

In main.c, we add tasks (in section 3.2.1). Don't forget to put the following 2 task in the osKernel.h header file.
```c++
uint32_t pcount1,pcount2;

void periodicTask1(void){	
	pcount1++;
}

void periodicTask2(void){	
	pcount2++;
}
```
The result will be like the following

<p align="center">
<img src="/img/periodic1.gif" height="50%" width="50%"> 
</p>  

### 7.2 Method 2 - Timer Interrupts
We will enable another timer TIM3 to trigger the task. Let's configure TIM3 manually and see how to trigger it. (In the previous section, we configure TIM2 using CubeMX and it generates code for us)

Create timer.c and timer.h (header file for timer.c) file for TIM3 configuration.

in <b>timer.c</b>
```c++
#include "stm32f4xx.h"                  // Device header

void TIM3_Init(void){
	
   RCC->APB1ENR |= 2;
   TIM3->PSC  = 16000-1;    /*divide system clock by 16000*/
   TIM3->ARR  = 1000-1;     /*divide by 1000*/  //be executed every 1000 ms = 1 s
   TIM3->CR1	=  1;
  
   TIM3->DIER |=1;					/*Enable UIE */	
   NVIC_EnableIRQ(TIM3_IRQn);
	
}
```

In <b>main.c</b>, add the following interrupt handler, variable, and the initialization command.
```c++
uint32_t ipcount1;

int main(void)
{
	TIM3_Init();
	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);
	osKernelLaunch(QUANTA);
}

void TIM3_IRQHandler(void){
	TIM3->SR =0;
	ipcount1++;
}
```

For the above TIM3 related register information, please refer to STM32F411xC/E advanced Arm®-based 32-bit MCUs Manual and the following table.
[https://www.st.com/resource/en/reference_manual/dm00119316-stm32f411xc-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf](https://www.st.com/resource/en/reference_manual/dm00119316-stm32f411xc-e-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
<p align="center">
<table>
    <thead>
        <tr>
            <th align="center">Configure</th>
            <th align="center">Detail</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">RCC Register</td>
	    <td align="Left">To enable the clock of TIM3, we need to set the bit1 of RCC_APB1ENR to 1 <br /><p align="center">
<img src="/img/TIM3_1.JPG" height="90%" width="90%"></p>page 118  </td>
        </tr>
        <tr>
            <td align="center">TIMx->PSC</td>
            <td align="Left">TIMx prescaler<br /><p align="center">
<img src="/img/TIM3_2.JPG" height="90%" width="90%"></p>page 367 </td>
        </tr>
	<tr>
            <td align="center">TIMx->ARR</td>
            <td align="Left">TIMx auto-reload register<br /><p align="center">
<img src="/img/TIM3_3.JPG" height="90%" width="90%"></p>page 367</td>
        </tr>
	 <tr>
            <td align="center">TIMx->CR1</td>
            <td align="Left">TIMx control register 1<br /><p align="center">
<img src="/img/TIM3_4.JPG" height="90%" width="90%"></p>page 352</td>
        </tr>
	<tr>
            <td align="center">TIMx->DIER</td>
            <td align="Left">TIMx DMA/Interrupt enable register<br /><p align="center">
<img src="/img/TIM3_5.JPG" height="90%" width="90%"></p>page 357</td>
        </tr>
	<tr>
            <td align="center">NVIC_EnableIRQ</td>
            <td align="Left">Functions to access the Nested Vector Interrupt Controller (NVIC). To enable certain interrupt according it's IRQ number.
https://www.keil.com/pack/doc/CMSIS/Core/html/group__NVIC__gr.html
		    <br /><p align="center">
<img src="/img/irq_n.JPG" height="100%" width="100%"></p></td>
        </tr>
	<tr>
            <td align="center">TIMx->SR</td>
            <td align="Left">TIMx status register<br /><p align="center">
<img src="/img/TIM3_6.JPG" height="90%" width="90%"></p>page 358</td>
        </tr>
    </tbody>
</table>
</p>

The result will be the following
<p align="center">
<img src="/img/periodic2.gif" height="50%" width="50%">
</p>

### 7.3 Method 3 - Thread Control Blocks (TCB)
Like the section 3.2.3 
[(link)](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#323-thread-control-block-)
. We create another thread control block (TCB) for periodic tasks, however, what being stored in the periodic tasks TCB is a little bit different. Please refer to the following image.
<p align="center">
<img src="/img/periodic_tasks_tcb.JPG" height="70%" width="70%">
</p>

In <b>osKernel.c</b>, we add the following codes to manage periodic tasks thread control blocks while initialization.
```c++
#define NUM_PERIODIC_TASKS 2

typedef void(*taskT)(void);
#define NULL (void*)0

//Following is the TCBs for periodic tasks
typedef struct{
  taskT task;
  uint32_t period;
}periodicTaskT;

static periodicTaskT	PeriodicTasks[NUM_PERIODIC_TASKS];
uint32_t MaxPeriod;
uint32_t TimeMsec;

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

```

In <b>osKernel.c</b>, we add the following function to execute periodic tasks when the PendSV handler branch to here.
```c++
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
	 for(i=0;i<NUM_PERIODIC_TASKS;i++)
	 {
	      if((TimeMsec % PeriodicTasks[i].period)==0 && PeriodicTasks[i].task !=NULL)
	        {
			PeriodicTasks[i].task();
		}
	  } 
	  currentPt  =  currentPt->nextPt;
}
```

In <b>main.c</b>, we define 2 periodic tasks: periodicTask1 and periodicTask2.
```c++
uint32_t pcount1,pcount2;

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
```
The <b>osKernel.s</b> is in the following
```c++
		AREA |.text|, CODE, READONLY, ALIGN=2
                THUMB
		PRESERVE8
		EXTERN  currentPt
		EXPORT	PendSV_Handler 
		EXPORT  osSchedulerLaunch
		IMPORT osSchedulerPeriodicRR


PendSV_Handler           ;save r0,r1,r2,r3,r12,lr,pc,psr      
    CPSID   I                  
    PUSH    {R4-R11}        ;save r4,r5,r6,r7,r8,r9,r10,r11   
    LDR     R0, =currentPt  ; r0 points to currentPt       
    LDR     R1, [R0]        ; r1= currentPt   
    STR     SP, [R1]   

    PUSH    {R0,LR}
    BL	    osSchedulerPeriodicRR    ;;do the tasks  and set currentPt = currentPt->next
    POP	    {R0,LR}
   
    LDR     R1, [R0]        ;r1= currentPt    (which is the next task)
    LDR     SP, [R1]        ;SP= currentPt->stackPt   
    POP     {R4-R11}           
    CPSIE   I                  
    BX      LR
	
	

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

The result is the in following
<p align="center">
<img src="/img/periodic3.gif" height="50%" width="50%">
</p>

### 7.4 TCB + Timer Interrupts
This will be a little complicated. Let's first take a look at the following image so that we can explain the detail more easily.
<p align="center">
<img src="/img/Periodic_timer_tcbs1.png" height="100%" width="100%">
</p>

We make both TIM2 and SysTick exception handler do tasks for us. SysTick handler will trigger the context switch 
(see section
[3.2.4](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#324-context-switch-part-i-)
and
[3.2.5](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#325-context-switch-part-ii-)
 ). TIM2 IRQ handler will trigger the execution of perodic tasks. Please see the following code.

In osKernel.c file, we add the following (following the code in section
[3.2.3](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#323-thread-control-block-)
).

```c++
#define NUM_PERIODIC_TASKS 5

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

```
In the osKernelInit() function, we add osPeriodicTask_Init(periodic_events_execute,1000,6);
```c++
void osKernelInit(void)
{
     __disable_irq();
     MILLIS_PRESCALER=(BUS_FREQ/1000);
     osPeriodicTask_Init(periodic_events_execute,1000,6);

}
```

In the main.c file, we add the following
```c++
uint32_t pcount1,pcount2, pcount3;

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
```

The osKernel.s file is same as section 7.3.

The result will be the following.
<p align="center">
<img src="/img/periodic4.gif" height="50%" width="50%">
</p>

### 7.5 Method 4 - Thread Sleep
We can also make one of task sleep for specified time period. During the sleeping time, the task scheduler would skip the sleeping task and move on to executing the non-sleeping task. 

We will continue the code from section [7.4](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#74-tcb--timer-interrupts) with a little modification. Start a new project in Kuvision, select device STM32F411RETx, configure the Run-Time Environment (check CMSIS --> CORE, Device --> Startup). Copy the code from ```code/7.4``` into project folder, include paths of this project folder. Add existing file of the copied code.

We don't need to put periodic tasks or event triggered tasks into sleep because they are often short, so we now look into the ```tcb``` part and add sleep function into it. Remember our main tasks which is in ```tcb``` runs infinit while (1) loop, only when the time quanta is up, or OS thread yield function is called, then the task in the tcb will stop temporally which we can see in [section 3](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#3-code-concept-of-preemptive-scheduling-with-round-robin-) . Sleep will be the third method to put the task in tcb to stop temporally. Now our tcb will be like the following

```c++
struct tcb{
	
	int32_t *stackPt;
	struct tcb *nextPt;
	uint32_t sleepTime;
	
};
```

# 8. Inter-Thread Data Transfer
If one thread (thread 0) is keep reading data from a sensor, we want another thread (thread 1) to keep processing data from thread 0, then we can implement the following method to achieve the function mentioned. Here is our project, we got one Potentiometer and LCD device. The Potentiometer will act as a sensor, Task0 in our code will keep reading the sensor value, while Task1 will keep showing the sensor value onto the LCD.  

### 8.1 Configure CubeMX
We will follow on the section [5.1](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#51-configure-cubemx-) ~ [5.4](https://github.com/Dungyichao/PeriodicScheduler_Semaphore#54-include-header-file-and-clear-out-comment-in-mainc-). However, we add ADC like we did in another [tutorial](https://github.com/Dungyichao/STM32F4-LCD_ST7735s#31--configure-pins-on-cubemx) during CubeMX configuration in section 5.1. On the left panel, click Analog --> ADC1 --> Select Mode: IN4. On the left panel, click SYS --> Timebase Source --> Select TIM2. Lastly, we generate the code.

<p align="center">
<img src="/img/8_cubemx_config.png" height="70%" width="70%">
</p>

### 8.2 Inter-Thread Communication Code

In ```osKernel.c``` we add following code. In ```main.c```, one task (reading sensor value) will keep calling ```osFifoPut(value)``` while another task (display read value on LCD) will keep calling ```osFifoGet()```. The value will be stored in and read from array ```OS_Fifo```.
```c++
#define FIFO_SIZE 15
uint32_t PutI;                // Put index
uint32_t GetI;                // Get index
uint32_t OS_Fifo[FIFO_SIZE];  // Array to store value
int32_t  current_fifo_size;   // Grow as more data being stored into the Array OS_Fifo. This variable would not exceed FIFO_SIZE
uint32_t lost_data;           // amount of data which has not been put into the array and get discarded.


void osFifoInit(void){
  PutI =0;
  GetI =0;
  osSemaphoreInit(&current_fifo_size,0);
  lost_data =0;	
}

int8_t osFifoPut(uint32_t data){
	
  if(current_fifo_size ==  FIFO_SIZE){
      lost_data++;
      return -1;
   }
    OS_Fifo[PutI] = data;
    PutI = (PutI+1)% FIFO_SIZE;
    osSignalSet(&current_fifo_size);     //current_fifo_size will add one
	
    return 1;
}

uint32_t osFifoGet(void){
    uint32_t data;
    osSignalWait(&current_fifo_size);
    __disable_irq();
    data  = OS_Fifo[GetI];
    GetI =  (GetI+1)%FIFO_SIZE;
    __enable_irq();	
	
    return data;	
}
 
```

We also add one function to convert integer to string so that we can display on LCD ([reference](https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c))

```c++
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );
    
    /*
    The bracket operator is acting as an array index operator. The C string decays to a char *, 
    which points to the first letter in the string 'z'. The bracket operator accesses the character 
    at position [35 + (tmp_value - value * base)] in the C string.
    */

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

```
The following code should be added to ```osKernel.h```
```c++
void osFifoInit(void);
int8_t osFifoPut(uint32_t data);
uint32_t osFifoGet(void);		
char* itoa(int value, char* result, int base);													 
#define PERIOD		100
```

In the ```main.c``` we add the following code

```c++
#include "st7735.h"
#include "fonts.h"
#include "osKernel.h"

#define QUANTA	50
int32_t semaphore1, semaphore2, semaphore0;
volatile int sensorValue;
char snum[5];                    // to store string converted from integer

void Task0(void)                 // Reading sensor value. Our sensor is Potentiometer
{
	while(1)
	{
		osSignalWait(&semaphore0);
		
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,1);
		sensorValue = HAL_ADC_GetValue(&hadc1);
		
		osFifoPut(sensorValue);		
		
		osSignalSet(&semaphore1);
	}
}

void Task1(void)
{
	while(1)
	{
		osSignalWait(&semaphore1);
		
		sensorValue  = osFifoGet();
		
		itoa(sensorValue, snum, 10);
		ST7735_DrawString(3, 5, snum, ST7735_RED);	
		
		osSignalSet(&semaphore2);
		
	}
}

void Task2(void)
{
	while(1)
	{
		osSignalWait(&semaphore2);
		//ST7735_DrawString(3, 7, "Task2", ST7735_YELLOW);
		osSignalSet(&semaphore0);	
	}
}

int main(void)
{
	......
	
	ST7735_Init();
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawString(1, 0, "CPU=", ST7735_ORANGE);
	
	osSemaphoreInit(&semaphore0, 1);
	osSemaphoreInit(&semaphore1, 0);
	osSemaphoreInit(&semaphore2, 0);

	osKernelInit();
	osKernelAddThreads(&Task0,&Task1,&Task2);
	osKernelLaunch(QUANTA);
	
	while (1)
	  {



	  }
	
	.....
}
```
Compile the project and load to the board, you may find the LCD is turning full bright. Hard reset the board and the LCD should be turning black. Try to roate the Potentiometer to see the value chaning on the LCD board. The following is the result. 

<p align="center">
<img src="/img/inter_thread_comm3.gif" height="80%" width="80%"> 
</p> 

# 9. Reference and conclusion
* Udemy course: Build Your Own RealTime OS (RTOS) From Ground Up on ARM 1. Instructor: Israel Gbati

The reason of making this tutorial is that the code provided from instructor of the Udemy course is not working at all and the instructor didn't reply to any student at all. This tutorial is to guide those people who want to get some knowledge of multitasking.


