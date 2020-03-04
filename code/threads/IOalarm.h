//Name: Omkar Buchade
//This file defines a callback method that is called when an interrupt is generated
#ifndef IOalarm_H
#define IOalarm_H

//#include "main.h"
#include "callback.h"
class IOalarm: public CallBackObj {

  private:
    void CallBack();		// called when the hardware timer generates an interrupt
};

#endif