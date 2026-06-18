#include "../h/tcb.h"
#include "../h/memoryAllocator.h"
#include "../h/print.h"
#include "../h/workers.h"
#include "../h/riscvHardware.h"
#include "../h/handleInterrupts.h"
#include "../h/global_allocators.h"
#include "../h/semaphore.h"
#include "../h/syscall_c.h"


int main() {
    MemoryAllocator::getInstance().init();

    uint64 funk = (uint64)&Interrupts::supervisorTrap;
    RiscvHardware::writeStvec(funk);
    TCB *threads[5];

    threads[0] = TCB::createThread(nullptr, nullptr, nullptr);
    TCB::running = threads[0];


    uint64* stackA = new uint64[1024];
    threads[1] = TCB::createThread(workerBodyA, nullptr, stackA);
    printString("ThreadA created\n");

    uint64* stackB = new uint64[1024];
    threads[2] = TCB::createThread(workerBodyB, nullptr, stackB);
    printString("ThreadB created\n");

    uint64* stackC = new uint64[1024];
    threads[3] = TCB::createThread(workerBodyC, nullptr, stackC);
    printString("ThreadC created\n");

    uint64* stackD = new uint64[1024];
    threads[4] = TCB::createThread(workerBodyD, nullptr, stackD);
    printString("ThreadD created\n");

    // RiscvHardware::setSstatusBit(RiscvHardware::SSTATUS_SIE);
    while (!(threads[1]->isFinished() && threads[2]->isFinished() &&
             threads[3]->isFinished() && threads[4]->isFinished())) {
        TCB::yield();
             }

    for (auto &thread: threads) { operator delete (thread); }
    printString("Finished\n");




    return 0;
}
