#ifndef TCB_H
#define TCB_H

#include "../lib/hw.h"

enum Status{ READY, RUNNING, BLOCKED, FINISHED};

using Routine = void(*)(void*);

class TCB {
public:
    TCB(Routine routine,void* arg);
    ~TCB();
    Status getStatus();
    void setUserStack(uint64* stck);
    static TCB* running;
private:
    uint64* userStack;
    uint64* kernelStack;
    uint64 savedSP;
    Status status;
};

#endif
