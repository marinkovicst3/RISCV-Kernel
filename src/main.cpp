#include "../h/tcb.h"
#include "../h/memoryAllocator.h"
#include "../h/print.h"
#include "../h/riscvHardware.h"
#include "../h/handleInterrupts.h"
#include "../h/global_allocators.h"
#include "../h/syscall_cpp.h"
#include "../test/Threads_C_API_test.hpp"

int main() {
    MemoryAllocator::getInstance().init();

    uint64 funk = (uint64)&Interrupts::supervisorTrap;
    RiscvHardware::writeStvec(funk);
    TCB *threads[5];
    threads[0] = TCB::createThread(nullptr, nullptr, nullptr);
    TCB::running = threads[0];
    Threads_C_API_test();
    printString("Finished\n");
    return 0;
}
