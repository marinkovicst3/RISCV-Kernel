#ifndef TCB_H
#define TCB_H

#include "../lib/hw.h"

enum Status { READY, RUNNING, BLOCKED, FINISHED };

using Routine = void(*)(void*);

class TCB {
public:
    TCB(Routine routine, void* arg, void* stack_space = nullptr);
    ~TCB();

    Status getStatus() { return status; }
    bool isFinished() { return status == FINISHED; }
    bool isBlocked() { return status == BLOCKED; }

    void setStatus(Status s) { status = s; }

    static void dispatch();

    static TCB* running;

    struct Context {
        uint64 ra;
        uint64 sp;
    };
    //static void contextSwitch(Context* oldContext, Context* newContext);

private:
    static void threadWrapper();

    Routine routine;
    void* arg;
    Status status;

    Context context;
    void* stackSpace;
};

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);

#endif