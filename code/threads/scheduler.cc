// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "debug.h"
#include "scheduler.h"
#include "main.h"
//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------

Scheduler::Scheduler()
{ 
    rbTree = new RBTree();
    toBeDestroyed = NULL;
} 

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
    delete rbTree;
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------


void
Scheduler::ReadyToRun (Thread *thread)
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());

    thread->setStatus(READY);
    //insert thread into RB tree
    rbTree->insertValue(thread);

    rbTree->setTreeSize(true);
    std::cout<<"\n \n";
    std::cout<<"Thread to insert: "<<thread->getName()<<" | id: "<<thread->getTid()<<"\n";
    std::cout<<"RB tree content after creation of new thread (inorder traversal): \n";
    rbTree->inorder();
    std::cout<<"-----------------------------------------------------------------------------------------------\n";

    std::cout<<" \n";

}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

//Method to perform division of two numbers. 
//This is used for computing the time slice for each thread
//Nachos does not perform division correctly thus this method 
float Divide (int x,int y) 
{  
    int c1=0;  
    int c2=0;  
    if(x>y)     
    {     
        while (x-y > 0)     
        { 
            x=x-y;         
            c1++;     
        }     
        x=x*100;     
        while(x-y >= 0)     
        {       
            c2++;
            x=x-y;     
        }     
        return (c1+ 0.01*c2);  
        }  
        else if (x==y)     
            return 1; 
        else 
        {     
            x=x*100;     
            while(x-y >= 0)     
            {                  
                c2++;         
                x=x-y;     
            }     
            return (0.01*c2); 
        } 
    }


Thread *
Scheduler::FindNextToRun ()
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (rbTree->getMinNode()==NULL) {
	    return NULL;
    } 
    else 
    {
        //calculate time slice 
        int slice = Divide(kernel->stats->Timerticks,rbTree->getTreeSize());

        //--std::cout<<"-----tot ticks is: "<<kernel->stats->Timerticks<<" tree size is: "<<rbTree->getTreeSize()<<" slice is: " << slice<<"\n";
        //schedule an interrupt for the calculated time slice
        kernel->interrupt->Schedule(this, slice, TimerInt);


        Thread *th= rbTree->getMinNode()->th;
        //--std::cout<<"Node to schedule next: "<<th->getName()<<" thread id: "<<th->getTid()<<"\n";
        th->setWait(rbTree->getMinNode()->wait);
        rbTree->deleteMinValue();
        rbTree->setTreeSize(false);
            
        std::cout <<"Next to Run: "<<th->getName()<<" | thread id: "<<th->getTid()<< "\n";

        //method to update wait (to calulate the decay)
        rbTree->updateWait();
        std::cout<<"RB tree after deletion of min node(lowest vRunTime) and updating decay/wait (inorder traversal): \n";
        rbTree->inorder();
        std::cout<<"-----------------------------------------------------------------------------------------------\n";

        th->setStart(kernel->stats->totalTicks);

        return th;
    }
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

void
Scheduler::Run (Thread *nextThread, bool finishing)
{
    Thread *oldThread = kernel->currentThread;
    
    //ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {	// mark that we need to delete current thread
         ASSERT(toBeDestroyed == NULL);
	 toBeDestroyed = oldThread;
    }
    
    if (oldThread->space != NULL) {	// if this thread is a user program,
        oldThread->SaveUserState(); 	// save the user's CPU registers
	oldThread->space->SaveState();
    }
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow


    kernel->currentThread = nextThread;  // switch to the next thread
    nextThread->setStatus(RUNNING);      // nextThread is now running
    std::cout<<"Context switched from: <Thread: "<< oldThread->getName() <<", id: "<<oldThread->getTid()<<"> to <Thread: "<<nextThread->getName()<<", id: "<<nextThread->getTid()<<">\n\n\n";
    DEBUG(dbgThread, "Switching from: " << oldThread->getName() << " to: " << nextThread->getName());
    
    // This is a machine-dependent assembly language routine defined 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".
    //--std::cout<<"Executing thread: "<<oldThread->getName()<<" thread_id: "<<oldThread->getTid()<<" start time: "<<oldThread->getStart()<<" end time: "<<oldThread->getFinish()<<" execution time: "<<oldThread->getExecution_time()<<"\n";

    SWITCH(oldThread, nextThread);

    // we're back, running oldThread
      
    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();		// check if thread we were running
					// before this one has finished
					// and needs to be cleaned up
    
    if (oldThread->space != NULL) {	    // if there is an address space
        oldThread->RestoreUserState();     // to restore, do it.
	oldThread->space->RestoreState();
    }
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void
Scheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
	toBeDestroyed = NULL;
    }
}

//Callback method to achieve context switching when an interrupt is hit (time slice finishes)
//This is the only callback method now in Nachos that will prempt a thread when it is done 
//executing in its time slice
void Scheduler::CallBack()
{
    kernel->interrupt->SetLevel(IntOff);
    kernel->interrupt->YieldOnReturn();
}
 
