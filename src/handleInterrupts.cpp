#include "../h/handleInterrupts.h"

#include "../h/memoryAllocator.h"
#include "../h/riscvHardware.h"
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
            default:
                break;
        }
    }
    else {
    }
}
