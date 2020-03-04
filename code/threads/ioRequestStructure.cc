//Name: Omkar Buchade
#include "ioRequestStructure.h"
#include <cstdlib>
#include <string> 
ioRequest::ioRequest(IntType type, Thread *th, std::string buff, int ticks)
{
    InterruptType=type;
    callThread= th;
    buffer=buff;
    if (type==WriteInt)
        execStart_time=100+ 1+ rand()%10;
    else
    {
        execStart_time=200+ 11+rand()%10;
        std::string s = "This is the data read";
        //s.append(std::to_string(execStart_time));  
        buffer=s;
    }
    std::cout<<"\nCurrent ticks= "<<ticks;

    tAt= ticks+execStart_time;
    std::cout<<"\nTotal wait time= "<<tAt<<"\n";
    
}
int ioRequest::getexecStart_time()
{
    return execStart_time;
}

int ioRequest::getTat_time()
{
    return tAt;
}


IntType ioRequest::getReqType()
{
    return InterruptType;
}

std::string ioRequest::getBuffer()
{
    return buffer;
}

Thread * ioRequest::getCallingThread()
{
    return callThread;
}