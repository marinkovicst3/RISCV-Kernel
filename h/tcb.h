#ifndef TCB_H
#define TCB_H

#include "../lib/hw.h"
#include "memoryAllocator.h"

using Body = void (*)(void*);

class TCB {
public:
    TCB();
    ~TCB() { delete[] stack; }
    bool isFinished() const { return finished; }
    bool isBlocked() const { return blocked; }


    void setFinished(bool value) { finished = value; }
    void setSemErrorStatus(int value) { semErrorStatus = value; }
    void setSemUnits(int value){ semUnits = value; }
    void setBlocked(bool value) { blocked = value; }

    uint64 getTimeSlice() const { return timeSlice; }
    int getSemErrorStatus() const { return semErrorStatus; }
    unsigned getSemUnits() const { return semUnits; }

    static TCB *createThread(Body body, void* arg, uint64* stackAdr);
    static void yield();
    static TCB *running;
private:

    void* operator new(size_t size) {
        return MemoryAllocator::getInstance().mem_alloc(size);
    }

    void operator delete(void* ptr){
        MemoryAllocator::getInstance().mem_free(ptr);
    }

    TCB(Body body,void* arg, uint64* stackAdr,uint64 timeslice);

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

    unsigned semUnits;
    int semErrorStatus;
    bool blocked;

    friend class RiscvHardware;
    friend class Interrupts;

    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *runningContext);
    static void dispatch();

    static uint64 timeSliceCounter;
};

#endif
