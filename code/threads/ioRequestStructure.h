//Name: Omkar Buchade
//This class defines the io request structure
#ifndef ioReqStruct
#define ioReqStruct

#include "../machine/interrupt.h"
#include <iostream>
#include "thread.h"
class ioRequest {
    private:
        IntType InterruptType;
        Thread *callThread;
        std::string buffer;
        int execStart_time; //wait time for request. Generated randomly
        int tAt;

    public:
        ioRequest(IntType, Thread *, std::string, int);
        int getexecStart_time(); //returns the randomly generated wait time for a request
        IntType getReqType(); //returns the type of request generated. Read or write
        std::string getBuffer();
        Thread * getCallingThread(); //returns the calling thread
        int getTat_time(); //get the current + waiting time
};
#endif