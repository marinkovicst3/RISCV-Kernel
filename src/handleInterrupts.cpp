#include "../h/handleInterrupts.h"

#include "../h/memoryAllocator.h"
#include "../h/riscvHardware.h"
#include "../h/scheduler.h"
#include "../h/tcb.h"
#include "../lib/hw.h"

void* Interrupts::handleMemAlloc() {
    size_t blocks = RiscvHardware::readA1();
    void* allocatedPointer = MemoryAllocator::getInstance().mem_alloc(blocks);
    return allocatedPointer;
}

int Interrupts::handleMemFree() {
    void* adr = (void*) RiscvHardware::readA1();
    int x = MemoryAllocator::getInstance().mem_free(adr);
    return x;
}

int Interrupts::handleThreadCreate() {
    TCB** handle = (TCB**) RiscvHardware::readA1();
    Routine routine = (Routine) RiscvHardware::readA2();
    void* arg = (void*) RiscvHardware::readA3();
    char* userStackTop = (char*) RiscvHardware::readA4();
    void* stackSpace = (void*)(userStackTop - DEFAULT_STACK_SIZE);
    TCB* newThread = new TCB(routine, arg, stackSpace);
    if (newThread == nullptr) return -1;
    if (handle != nullptr) {
        *handle = newThread;
    }
    Scheduler::getInstance().put(newThread);
    return 0;
}

void Interrupts::handleSupervisorTrap() {
    uint64 scause = RiscvHardware::readScause();
    if (scause == 0x08UL || scause == 0x09UL) {
        uint64 currentSepc = RiscvHardware::readSepc();
        RiscvHardware::writeSepc(currentSepc + 4);
        uint64 opcode = RiscvHardware::readA0();

        switch (opcode) {
            case 0x01: {
                void* adr = handleMemAlloc();
                RiscvHardware::writeA0OnStack((uint64) adr);
                break;
            }
            case 0x02:{
                int x = handleMemFree();
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x11: {
                int x = handleThreadCreate();
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x13: {
                TCB::dispatch();
                break;
            }
            default:
                break;
        }
    }
    else {
    }
}
