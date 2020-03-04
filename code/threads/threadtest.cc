//Name: Omkar Buchade
//Aim: Implementing completely fair scheduler for NachosOS

#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "ioRequest.h"

//This method is used to generate the ioRequests.
//ioRequest is a read request and write request which is generated in this method
void ioRequestGenerator()
{
   // std::cout<<"Current ticks: "<<kernel->stats->totalTicks<<"\n";
    std::cout<<"IO bound thread with id: "<<kernel->currentThread->getTid()<<" created.";

        Request *RequestObj=new Request();
        RequestObj->writeRequest("Omkar");
        RequestObj->readRequest();
}

//This method is used to generate the CPU requests 
void cpuRequestGenerator()
{
    //perform some computation for CPU bound threads
    std::cout<<"\nCPU bound thread with id: "<<kernel->currentThread->getTid()<<" created at ticks: "<<kernel->stats->totalTicks<<".\n \n";
    int sum=0;  
    for(int i=0 ; i<30; i++)
    {
        sum=i;
        kernel->interrupt->SetLevel(IntOff);    
        kernel->interrupt->SetLevel(IntOn);
    }
    std::cout<<"CPU bound thread with id: "<<kernel->currentThread->getTid()<<" finished execution at ticks: "<<kernel->stats->totalTicks<<".\n \n \n";
}

//This method is used to generate mixed requests (CPU requests+ IO request) 
void mixedRequestGenerator()
{
    std::cout<<"\nHybrid thread with id: "<<kernel->currentThread->getTid()<<" created at ticks: "<<kernel->stats->totalTicks<<".\n";    
    
    std::cout<<"Hybrid thread id: "<< kernel->currentThread->getTid() <<" | IO bound request created at ticks: "<<kernel->stats->totalTicks<<"\n";
        //generating IO request
        Request *RequestObj=new Request();
        RequestObj->writeRequest("Omkar");
        RequestObj->readRequest();    

    std::cout<<"Hybrid thread id: "<< kernel->currentThread->getTid() <<" | IO bound request completed. \n";// execution at ticks: "<<kernel->stats->totalTicks<<"\n";
    

    std::cout<<"\nHybrid thread id: "<< kernel->currentThread->getTid() <<" | CPU bound request created at ticks: "<<kernel->stats->totalTicks<<"\n";
    //performing CPU bound operation
    int sum=0;  
    for(int i=0 ; i<30; i++)
    {
        sum=i;
    }
    std::cout<<"Hybrid thread id: "<< kernel->currentThread->getTid() <<" | CPU bound request completed. \n"; //execution at ticks: "<<kernel->stats->totalTicks<<"\n";

}

//method to generate CPU, IO and Hybrid threads
void
ThreadGen()
{
    int tot_thrd;
    tot_thrd = 7;
    //tot_thrd = 1;
    for(int i=1;i<=tot_thrd;i++)  //7*3 = 21 threads in total
    {

        Thread *t1 = new Thread("IO-bound thread",i);
        t1->Fork((VoidFunctionPtr) ioRequestGenerator,NULL);

        Thread *t2 = new Thread("CPU-bound thread",i);
        t2->Fork((VoidFunctionPtr) cpuRequestGenerator,NULL);

        Thread *t3 = new Thread("Hybrid thread",i);
        t3->Fork((VoidFunctionPtr) mixedRequestGenerator,NULL);

    }
        kernel->currentThread->Yield();
    
    for(int i=0; i<5000; i++)
    {
        kernel->interrupt->SetLevel(IntOff);    
        kernel->interrupt->SetLevel(IntOn);
    }

}

void
ThreadTest()
{
    Thread *t = new Thread("Admin Thread");
    t->Fork((VoidFunctionPtr) ThreadGen ,NULL);

}
