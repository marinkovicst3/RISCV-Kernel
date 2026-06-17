#ifndef TCB_H
#define TCB_H

#include "../lib/hw.h"
using Body = void (*)(void*);

class TCB {
public:
    ~TCB() { delete[] stack; }
    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }
    uint64 getTimeSlice() const { return timeSlice; }

    static TCB *createThread(Body body, void* arg, uint64* stackAdr);
    static void yield();
    static TCB *running;
private:
    TCB(Body body,void* arg, uint64* stackAdr, uint64 timeSlice);

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    friend class RiscvHardware;
    friend class Interrupts;

    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *runningContext);
    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif
