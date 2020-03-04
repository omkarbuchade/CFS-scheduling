//Name: Omkar Buchade

#include "ioRequest.h"
#include <iostream>
#include "main.h"
#include "IOalarm.h"

void Request::writeRequest(std::string writeStr)
{
    std::cout<<"\nCreating a write request";
    int ticks= kernel->stats->totalTicks;
    ioRequest *requestObj=new ioRequest(WriteInt, kernel->currentThread, writeStr, ticks);
    kernel->reqList->Insert(requestObj);

    IOalarm *IOalarmObj = new IOalarm();
    kernel->interrupt->Schedule(IOalarmObj,requestObj->getexecStart_time(), WriteInt);

    kernel->interrupt->SetLevel(IntOff);
    kernel->currentThread->Sleep(false);
    kernel->interrupt->SetLevel(IntOn);
}

void Request::readRequest()
{
    std::cout<<"\nCreating a read request";
    int ticks= kernel->stats->totalTicks;
    ioRequest *requestObj=new ioRequest(ReadInt, kernel->currentThread, "", ticks);
    kernel->reqList->Insert(requestObj);

    IOalarm *IOalarmObj = new IOalarm();
    kernel->interrupt->Schedule(IOalarmObj,requestObj->getexecStart_time(), ReadInt);

        kernel->interrupt->SetLevel(IntOff);
        kernel->currentThread->Sleep(false);
        kernel->interrupt->SetLevel(IntOn);
}