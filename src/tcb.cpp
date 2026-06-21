#include "../h/tcb.h"
#include "../h/scheduler.h"

TCB *TCB::running = nullptr;
//uint64 TCB::timeSliceCounter = 0;

TCB::TCB(Body body, void* arg, uint64* stackAdr) :
body(body),
arg(arg),
stack(stackAdr),
context({(uint64) &threadWrapper,stack != nullptr ? (uint64)stackAdr + DEFAULT_STACK_SIZE : 0 }),
finished(false),
semUnits(0),
semErrorStatus(0),
blocked(false){
    if (body != nullptr) {
        Scheduler::getInstance().put(this);
    }
}

TCB* TCB::createThread(Body body, void* arg, uint64* stackAdr) {
    return new TCB(body, arg, stackAdr);
}

void TCB::yield() {
    uint64 val = 0x13;
    __asm__ volatile ("mv a0, %0" : : "r"(val));
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) {
        Scheduler::getInstance().put(old);
    }
    running = Scheduler::getInstance().get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    //RiscvHardware::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}