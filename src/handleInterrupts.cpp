#include "../h/handleInterrupts.h"

#include "../h/memoryAllocator.h"
#include "../h/riscvHardware.h"
#include "../h/tcb.h"
#include "../lib/hw.h"
#include "../h/global_allocators.h"
#include "../h/semaphore.h"
#include "../h/print.h"

void* Interrupts::handleMemAlloc(void* arg1) {
    size_t blocks = (size_t) arg1;
    void* allocatedPointer = MemoryAllocator::getInstance().mem_alloc(blocks);
    return allocatedPointer;
}

int Interrupts::handleMemFree(void* arg1) {
    void* adr = arg1;
    int x = MemoryAllocator::getInstance().mem_free(adr);
    return x;
}

int Interrupts::handleThreadCreate(void* arg1,void* arg2,void* arg3,void* arg4) {
    TCB** handle = (TCB**) arg1;
    Body routine = (Body) arg2;
    void* arg = (void*) arg3;
    char* stackSpace = (char*) arg4;
    TCB* newThread = new TCB(routine, arg,(uint64*) stackSpace);
    if (newThread == nullptr) return -1;
    if (handle != nullptr) {
        *handle = newThread;
    }
    return 0;
}

int Interrupts::handleSemOpen(void* arg1,void* arg2) {
    MySemaphore** handle = (MySemaphore**) arg1;
    if (handle == nullptr) return -1;
    unsigned init = (unsigned) (size_t)arg2;
    MySemaphore* newSem = new MySemaphore(init);
    if (newSem==nullptr) return -2;
    *handle = newSem;
    return 0;
}

int Interrupts::handleSemClose(void* arg1) {
    MySemaphore* handle = (MySemaphore*) arg1;
    if (handle == nullptr) return -1;
    int res = handle->close();
    operator delete(handle);
    return res;
}

int Interrupts::handleSemWait(void* arg1) {
    MySemaphore* handle = (MySemaphore*) arg1;
    if (handle == nullptr) return -1;
    return handle->wait();
}

int Interrupts::handleSemSignal(void* arg1) {
    MySemaphore* handle = (MySemaphore*) arg1;
    if (handle == nullptr) return -1;
    return handle->signal();
}

int Interrupts::handleSemWaitN(void* arg1, void* arg2) {
    MySemaphore* handle = (MySemaphore*) arg1;
    unsigned n = (unsigned) (size_t) arg2;
    if (handle == nullptr) return -1;
    return handle->wait_n(n);
}

int Interrupts::handleSemSignalN(void* arg1, void* arg2) {
    MySemaphore* handle = (MySemaphore*) arg1;
    unsigned n = (unsigned) (size_t) arg2;
    if (handle == nullptr) return -1;
    return handle->signal_n(n);
}

void Interrupts::handleSupervisorTrap() {
    uint64 args[5];
    asm  volatile("mv %0, a0" : "=r"(args[0]));
    asm  volatile("mv %0, a1" : "=r"(args[1]));
    asm  volatile("mv %0, a2" : "=r"(args[2]));
    asm  volatile("mv %0, a3" : "=r"(args[3]));
    asm  volatile("mv %0, a4" : "=r"(args[4]));

    uint64 scause = RiscvHardware::readScause();
    if (scause == 0x08UL || scause == 0x09UL) {
        uint64 currentSepc = RiscvHardware::readSepc();
        RiscvHardware::writeSepc(currentSepc + 4);

        switch (args[0]) {
            case 0x01: {
                void* adr = handleMemAlloc((void*)args[1]);
                RiscvHardware::writeA0OnStack((uint64) adr);
                break;
            }
            case 0x02:{
                int x = handleMemFree((void*)args[1]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x11: {
                int x = handleThreadCreate((void*)args[1],(void*)args[2],(void*)args[3],(void*)args[4]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x12: {
                TCB::running->setFinished(true);
                TCB::dispatch();
                break;
            }
            case 0x13: {
                TCB::dispatch();
                break;
            }
            case 0x21: {
                int x = handleSemOpen((void*)args[1],(void*)args[2]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x22: {
                int x = handleSemClose((void*)args[1]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x23: {
                int x = handleSemWait((void*)args[1]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x24: {
                int x = handleSemSignal((void*)args[1]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x25: {
                int x = handleSemWaitN((void*)args[1],(void*)args[2]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x26: {
                int x = handleSemSignalN((void*)args[1],(void*)args[2]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            default:
                break;
        }
    }
    else {
        uint64 scause = RiscvHardware::readScause();
        uint64 stval = RiscvHardware::readStval();
        uint64 stvec = RiscvHardware::readStvec();
        uint64 sepc = RiscvHardware::readSepc();

        printString("scause: ");
        printInt(scause);
        printString("\n");

        printString("stval: ");
        printInt(stval);
        printString("\n");

        printString("stvec: ");
        printInt(stvec);
        printString("\n");

        printString("sepc: ");
        printInt(sepc);
        printString("\n");
    }
}
