#include "../h/tcb.h"
#include "../h/scheduler.h"
#include "../h/riscvHardware.h"

TCB* TCB::running = nullptr;

TCB::TCB(Routine routine, void* arg, void* stack_space) {
    this->routine = routine;
    this->arg = arg;
    this->stackSpace = stack_space;

    if (routine != nullptr) {
        this->status = READY;
        uint64* stackTop = (uint64*)((char*)stackSpace + DEFAULT_STACK_SIZE);
        stackTop -= 32;
        stackTop[0] = (uint64)&TCB::threadWrapper;
        this->context.sp = (uint64)stackTop;
        this->context.ra = (uint64)&TCB::threadWrapper;
    }
    else {
        this->status = RUNNING;
        this->context.sp = 0;
        this->context.ra = 0;
    }
}

TCB::~TCB() {
    if (stackSpace != nullptr) {
        MemoryAllocator::getInstance().mem_free(stackSpace);
    }
}
void TCB::dispatch() {
    TCB* oldThread = TCB::running;
    if (!oldThread->isFinished() && !oldThread->isBlocked()) {
        oldThread->setStatus(READY);
        Scheduler::getInstance().put(oldThread);
    }
    TCB* newThread = Scheduler::getInstance().get();
    if (newThread == nullptr) {
        newThread = oldThread;
    }
    TCB::running = newThread;
    newThread->setStatus(RUNNING);
    if (oldThread != newThread) {
        contextSwitch(&(oldThread->context), &(newThread->context));
    }
}

void TCB::threadWrapper() {
    TCB::running->routine(TCB::running->arg);
    TCB::running->setStatus(FINISHED);
    TCB::dispatch();
}