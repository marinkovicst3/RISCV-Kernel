#include "../h/handleInterrupts.h"
#include "../h/memoryAllocator.h"
#include "../h/riscvHardware.h"
#include "../lib/console.h"
#include "../h/syscall_c.h"

int main() {
    MemoryAllocator::getInstance().init();
    uint64 funk = (uint64)&Interrupts::supervisorTrap;
    RiscvHardware::writeStvec(funk);
    void* adr = mem_alloc(500);
    size_t x = (size_t)adr;
    x=5;
    return x;
}