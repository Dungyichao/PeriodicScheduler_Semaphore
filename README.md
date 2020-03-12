# Task Scheduler & Semaphore
Task scheduling is an important concept in Real-Time Operating Systems (Task, thread, process are interchangeable). Take your daily life for example, when trying to listen music on your cellphone while surfing news website through the internet (reaceiving data from wi-fi router in the mean time). How does it work under only one core of CPU (CPU can only execute one command at a time)? Each task such as open the browser may requires hundred or thousand line of codes, then how can we listen music and open website at the same time? It comes to task scheduling. Let's take a look at the following animation of the task scheduling concept.
<br />
<p align="center">
<img src="/img/scheduler_concept_1.gif" height="40%" width="40%"> 
</p>  
<br />
You can think of Task 0 as music player, Task 1 as Chrome web browser, and Task 2 as processing Wi-Fi data. There are two way of task scheduling: <b>Preemptive Scheduling</b> and <b>Non-Preemptive Scheduling</b>. Reading resource: 

Reading resource: [link](https://www.geeksforgeeks.org/preemptive-and-non-preemptive-scheduling/)


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
            <td align="center">SPI, I2C, UART, Ethernet</td>
        </tr>
        <tr>
            <td align="center">Non-Preemptive Scheduling</td>
            <td align="center">Wi-Fi, Bluetooth, Cellular</td>
        </tr>
    </tbody>
</table>
</p>

Each task is being processed in a fixed amount of time. However, the amount of time can be configured based on different task.  

