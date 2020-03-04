//Name: Omkar Buchade
//This file is used for demonstrating the read and write operations for our simulated interrupt

#include "ioHandler.h"
#include "main.h"

//This method simulates the read operation
void ioHandler::Read(ioRequest * obj)
{
    std::cout<<"Read operation started \n";
    std::cout<<"Calling thread type: "<<obj->getCallingThread()->getName() <<" with id: "<< obj->getCallingThread()->getTid()<<"\n" ;
    std::cout<<"Read content is: "<<obj->getBuffer()<<"\n";
    std::cout<<"Read operation finished \n";

    IntStatus oldStatus= kernel->interrupt->SetLevel(IntOff);
    kernel->scheduler->ReadyToRun(obj->getCallingThread());
    (void)kernel->interrupt->SetLevel(oldStatus);

}
//This method simulates the write operation
void ioHandler::Write(ioRequest * obj)
{
    std::cout<<"Write operation started \n";
    std::cout<<"Calling thread type: "<<obj->getCallingThread()->getName() <<" with id: "<< obj->getCallingThread()->getTid()<<"\n" ;
    std::cout<<"Write content is: "<<obj->getBuffer()<<"\n";
    std::cout<<"Write operation finished \n";

    IntStatus oldStatus= kernel->interrupt->SetLevel(IntOff);
    kernel->scheduler->ReadyToRun(obj->getCallingThread());
    (void)kernel->interrupt->SetLevel(oldStatus);
}