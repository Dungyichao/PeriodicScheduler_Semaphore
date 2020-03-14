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

# 3. Implement Preemptive Scheduling with Round Robin <br />
The idea is basically the same as the first animation. Now, we add more detail (<b>registers</b> and <b>stack</b>) in the animation. We all know CPU use <b>registers</b> to store computing data or fetch computing data for temporary. When you are calculating some complicated math problem, you write down the value on sticky note (registers), when you read the equation and calculate the equation in your mind (CPU) and then write down the result back to the sticky note. While you are solving math problem, your Mom calls you, then you quickly write down the lastest calculation result (in registers) onto a paper (<b>stack</b>). You can now write down what your Mom tells you on the sticky note. After finishing the call, you can go back to solve your math problem by restoring what you left on the paper (stack) to the sticky note (registers) and keep going on. 

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
<img src="/img/elements1.JPG" height="80%" width="80%">
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
		    </p><br />In this section, we only use <b>Systick</b> as a trigger to let us perform the context switch. A SysTick exception is an exception the system timer generates when it reaches zero. Software can also generate a SysTick exception. In an OS environment, the processor can use this exception as system tick.</td>
        </tr>
    </tbody>
</table>
</p>
