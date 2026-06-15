#include "../h/handleInterrupts.h"
#include "../h/memoryAllocator.h"
#include "../h/riscvHardware.h"
#include "../h/scheduler.h"
#include "../lib/console.h"
#include "../h/syscall_c.h"

int main() {
    MemoryAllocator::getInstance().init();
    uint64 funk = (uint64)&Interrupts::supervisorTrap;
    RiscvHardware::writeStvec(funk);
    TCB* n = Scheduler::getInstance().get();
    (void)n;
    void* adr = mem_alloc(500);
    __putc('a');
    int y = mem_free(adr);
    return y;
}