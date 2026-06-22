#include "../h/tcb.h"

#include "../h/riscvHardware.h"
#include "../h/scheduler.h"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
TCB* TCB::sleepHead = nullptr;
TCB* TCB::sleepTail = nullptr;

TCB::TCB(Body body, void* arg, uint64* stackAdr,uint64 timeSlice) :
body(body),
arg(arg),
stack(stackAdr),
context({(uint64) &threadWrapper,stack != nullptr ? (uint64)stackAdr + DEFAULT_STACK_SIZE : 0 }),
timeSlice(timeSlice),
finished(false),
semUnits(0),
semErrorStatus(0),
blocked(false),
asleep(false),
next(nullptr),
prev(nullptr){
    if (body != nullptr) {
        Scheduler::getInstance().put(this);
    }
}

TCB* TCB::createThread(Body body, void* arg, uint64* stackAdr) {
    return new TCB(body, arg, stackAdr,DEFAULT_TIME_SLICE);
}

void TCB::yield() {
    uint64 val = 0x13;
    __asm__ volatile ("mv a0, %0" : : "r"(val));
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked() && !old->isAsleep()) {
        Scheduler::getInstance().put(old);
    }
    running = Scheduler::getInstance().get();
    TCB::contextSwitch(&old->context, &running->context);
    TCB::timeSliceCounter = 0;
}

void TCB::insertSleepingThread(TCB* thread, time_t period) {
    thread->next = nullptr;
    thread->prev = nullptr;
    thread->asleep = true;
    thread->sleepTime = period;

    if (sleepHead == nullptr) {
        sleepHead = thread;
        sleepTail = thread;
        return;
    }
    TCB* curr = sleepHead;
    while (curr != nullptr && thread->sleepTime >= curr->sleepTime) {
        thread->sleepTime -= curr->sleepTime;
        curr = curr->next;
    }
    if (curr == nullptr) {
        sleepTail->next = thread;
        thread->prev = sleepTail;
        sleepTail = thread;
        return;
    }
    if (curr == sleepHead) {
        thread->next = sleepHead;
        sleepHead->prev = thread;
        sleepHead = thread;
    } else {
        thread->next = curr;
        thread->prev = curr->prev;
        curr->prev->next = thread;
        curr->prev = thread;
    }
    curr->sleepTime-=thread->sleepTime;
}

void TCB::updateSleepList() {
    if (sleepHead == nullptr) return;
    sleepHead->sleepTime--;
    while (sleepHead != nullptr && sleepHead->sleepTime == 0) {
        TCB* wakeUpThread = sleepHead;
        sleepHead = sleepHead->next;
        if (sleepHead != nullptr) {
            sleepHead->prev = nullptr;
        } else {
            sleepTail = nullptr;
        }
        wakeUpThread->next = nullptr;
        wakeUpThread->prev = nullptr;
        wakeUpThread->asleep = false;
        Scheduler::getInstance().put(wakeUpThread);
    }
}


void TCB::threadWrapper() {
    if(running->body)
        RiscvHardware::clearSstatusBit(RiscvHardware::SSTATUS_SPP);
    else
        RiscvHardware::setSstatusBit(RiscvHardware::SSTATUS_SPP);
    RiscvHardware::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}
