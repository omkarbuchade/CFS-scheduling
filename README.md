# CFS-scheduling
Implementation of the CFS scheduling algorithm using red black trees for NACHOS Operating System

Implementation:
Task 1: Designed own I/O simulating input(read)/output(write) operations.
Task 2: CFS Scheduling using RBTree in Nachos.

Files with directory name that you modified or created for this assignment:
 nachos/code/build.linux/Makefile
 nachos/code/machine/interrupt.h
 nachos/code/machine/stats.h
 nachos/code/machine/timer.h
 nachos/code/machine/timer.cc
 nachos/code/threads/IOalarm.h
 nachos/code/threads/IOalarm.cc
 nachos/code/threads/ioHandler.h
 nachos/code/threads/ioHandler.cc
 nachos/code/threads/ioRequest.h
 nachos/code/threads/ioRequest.cc
 nachos/code/threads/ioRequestStructure.h
 nachos/code/threads/ioRequestStructure.cc
 nachos/code/threads/kernel.h
 nachos/code/threads/kernel.cc
 nachos/code/threads/rbt.h
 nachos/code/threads/rbt.cc
 nachos/code/threads/scheduler.h
 nachos/code/threads/scheduler.cc
 nachos/code/threads/thread.h
 nachos/code/threads/thread.cc
 nachos/code/threads/threadtest.cc

Design/solution for each task:
Task 1:
o To simulate IO read and write operation I have designed a class ioHandler in
ioHandler.h file where the read write operations are simulated (a string is read from a
buffer for read operation and for write operation a string is written into a buffer).
o Inorder to generate IO requests the class ioRequest in the file ioRequest.h file is used.
It responsible for generating IO requests (read/write). The structure of this request is
defined in the ioRequestStructure.h file where the type of interrupt is set, and a
random waiting time for the request is generated. Once a request is generated it is
added to a list (reqList) present in the kernel.h file and an alarm is set for the randomly
generated waiting time which simulates the request by invoking the callback method
in the IOalarm.h class thereby triggering the simulation methods in ioHandler class
which we discussed in the previous point.
o Since this tasks tells us to create our own IO simulated interrupts, I have defined the
two new IO interrupts in the file interrupt.h at line 51 in the enum as ‘ReadInt’ and
‘WriteInt’ for read and write interrupt respectively.

Task 2:
o For implementing CFS scheduler I have implemented the Red Black Trees. 
o The time quantum for each thread is the ratio of kernel->stats->totalTicks divided by
the total number of threads available for execution in the RB tree. The value of
totalTicks is reset from 100 to 1000. So now 1000 will be equally divided into n
number of threads available for execution in RB tree. After the time slice for a
process is calculated an interrupt is set.
o This interrupt is used to preempt the thread after its time slice is elapsed thus
bringing the next thread from the RB tree for execution. 
o After a thread has completed executing for its time slice the VRuntime for that
thread is updated and the wait count is reset to 1.
o After each iteration/removal of thread/node from the RB tree we increment the wait
time for each thread in the RBtree that is not executed (which means it is waiting to
be executed. The wait time/decay is simply a count which is used to keep a track of
how long has the process been sitting the tree waiting to get executed).
o There is no need to update Vruntime of every node in the tree after removal of one
node. Only incrementing the wait time for every node will work since the
order/structure of the tree would remain the same even after recalculating the
VRuntime after updating the wait. Thus there is no need of the extra computation of
VrunTime of all nodes at every iteration.
o The virtual rum time is calculated as: execution_time*weight
Where,
weight=1/wait time (thus larger the wait, time lower will be the
weight thus making the Vruntime smaller and bringing it to the left
side of the RB tree causing it to be invoked as soon as possible) 
