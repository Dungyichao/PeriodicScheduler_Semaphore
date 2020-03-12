# Task Scheduler
Task scheduling is an important concept in Real-Time Operating Systems (Task, thread, process are interchangeable). Take your daily life for example, when trying to listen music on your cellphone while surfing news website through the internet (reaceiving data from wi-fi router in the mean time). How does it work under only one core of CPU (CPU can only execute one command at a time)? Each task such as open the browser may requires hundred or thousand line of codes, then how can we listen music and open website at the same time? It comes to task scheduling. 

# 1. What is the Goal <br />
Let's first take a look what are we going to achieve after this tutorial.
<p align="center">
<img src="/img/semaphore.gif" height="80%" width="80%"> 
</p>  
That is really simple!!! You can see 3 line of string Task0, Task1, Task2 with different color. Totally we have three tasks, and each task displays their own string. Please refer to the following link to read the tutorial of displaying color and character on LCD display:

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

