#include "../h/tcb.h"
#include "../h/scheduler.h"
#include "../h/riscvHardware.h"
#include "../h/global_allocators.h"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB::TCB(Body body, void* arg, uint64* stackAdr, uint64 timeSlice) : body(body),
arg(arg),
stack(stackAdr),
context({(uint64) &threadWrapper,stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0 }),
timeSlice(timeSlice),
finished(false) {
    if (body != nullptr) {
        Scheduler::getInstance().put(this);
    }
}

TCB* TCB::createThread(Body body, void* arg, uint64* stackAdr) {
    return new TCB(body, arg, stackAdr, TIME_SLICE);
}

void TCB::yield() {
    RiscvHardware::writeA0(0x13);
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::getInstance().put(old); }
    running = Scheduler::getInstance().get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    //RiscvHardware::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}