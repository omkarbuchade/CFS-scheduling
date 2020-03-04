//Name: Omkar Buchade

#include "IOalarm.h"
#include "ioHandler.h"
#include "main.h"

void IOalarm::CallBack()
{
    while(kernel->reqList->NumInList()>0)
    {
        if(kernel->stats->totalTicks >= kernel->reqList->Front()->getTat_time())
        {
            std::cout<<"\nHit! Interrupt triggred at ticks: "<<kernel->stats->totalTicks<<"\n";
            ioHandler *ioHandlerObj = new ioHandler();
            if(kernel->reqList->Front()->getReqType()==WriteInt)
            {
                ioHandlerObj->Write(kernel->reqList->Front()); //call write fun
            }
            else
            {
                ioHandlerObj->Read(kernel->reqList->Front()); //call read fn
            }
            kernel->reqList->RemoveFront();
        }
        else
        {
            break;
        }
    }
}