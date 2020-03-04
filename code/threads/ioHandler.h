//Name: Omkar Buchade
// This file simulates the read and write operation simulation after an interrupt is hit
#include "ioRequestStructure.h"
class ioHandler
{
    public:
        void Write(ioRequest *);  //simulation of read operation
        void Read(ioRequest *); //simulation of the write operation
};