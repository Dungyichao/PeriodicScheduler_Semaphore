# Task Scheduler & Semaphore
Task scheduling is an important concept in Real-Time Operating Systems. Take your daily life for example, when trying to listen music on your cellphone while surfing news website through the internet. How does it work under only one core of CPU (CPU can only execute one command at a time)? Each task such as open the browser may requires hundred or thousand line of codes, then how can we listen music and open website at the same time? It comes to task scheduling. Let's take a look at the following animation of the task scheduling concept.
<br />
<p align="center">
<img src="/img/scheduler_concept_1.gif" height="40%" width="40%"> 
</p>  
<br />
You can think of Task 0 as music player, Task 1 as Chrome web browser, and Task 2 as processing Wi-Fi data. Each task is being processed in a fixed amount of time. However, the amount of time can be configured based on different task.  

