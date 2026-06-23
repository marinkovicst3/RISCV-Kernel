#ifndef TCB_H
#define TCB_H

#include "../lib/hw.h"
#include "memoryAllocator.h"

using Body = void (*)(void*);

class TCB {
public:
    TCB();
    ~TCB();

    void* operator new(size_t size) {
        uint64 blocks = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
        return MemoryAllocator::getInstance().mem_alloc(blocks);
    }

    void operator delete(void* ptr){
        MemoryAllocator::getInstance().mem_free(ptr);
    }

    bool isFinished() const { return finished; }
    bool isBlocked() const { return blocked; }
    bool isAsleep() const {return asleep;}

    void setFinished(bool value) { finished = value; }
    void setSemErrorStatus(int value) { semErrorStatus = value; }
    void setSemUnits(int value){ semUnits = value; }
    void setBlocked(bool value) { blocked = value; }

    uint64 getTimeSlice() const { return timeSlice; }
    int getSemErrorStatus() const { return semErrorStatus; }
    unsigned getSemUnits() const { return semUnits; }

    static TCB *createThread(Body body, void* arg, uint64* stackAdr,bool isKernelThread = false);
    static void yield();
    static TCB *running;
private:
    TCB(Body body,void* arg, uint64* stackAdr,uint64 timeslice,bool isKernelThread);

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
    bool asleep;
    bool isKernelThread;

    friend class RiscvHardware;
    friend class Interrupts;

    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *runningContext);
    static void dispatch();
    static void insertSleepingThread(TCB* thread, time_t period);
    static void updateSleepList();

    static uint64 timeSliceCounter;
    static TCB* sleepHead;
    static TCB* sleepTail;
    TCB* next;
    TCB* prev;

    time_t sleepTime;
};

#endif
