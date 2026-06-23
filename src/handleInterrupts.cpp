#include "../h/handleInterrupts.h"
#include "../h/riscvHardware.h"
#include "../h/tcb.h"
#include "../h/semaphore.h"
#include "../test/printing.hpp"
#include "../h/consoleController.h"

const uint64 Timer_Interrupt = 0x8000000000000001UL;
const uint64 Hardware_Interrupt = 0x8000000000000009UL;
const uint64 Usermod_Ecall = 0x08UL;
const uint64 Kernelmod_Ecall = 0x09UL;

void RiscvHardware::popSppSpie() {
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}


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
    void* arg = arg3;
    char* stackSpace = (char*) arg4;
    TCB* newThread = TCB::createThread(routine,arg,(uint64*)stackSpace);
    if (newThread == nullptr) return -1;
    if (handle != nullptr) {
        *handle = newThread;
    }
    return 0;
}

int Interrupts::handleSemOpen(void* arg1,void* arg2) {
    MySemaphore** handle = (MySemaphore**) arg1;
    unsigned int init = (unsigned int) (uint64)arg2;
    MySemaphore* newSem = new MySemaphore(init);
    if (newSem==nullptr) return -2;
    *handle = newSem;
    return 0;
}

int Interrupts::handleSemClose(void* arg1) {
    MySemaphore* handle = (MySemaphore*) arg1;
    int res = handle->close();
    return res;
}

int Interrupts::handleSemWait(void* arg1) {
    MySemaphore* handle = (MySemaphore*) arg1;
    return handle->wait();
}

int Interrupts::handleSemSignal(void* arg1) {
    MySemaphore* handle = (MySemaphore*) arg1;
    return handle->signal();
}

int Interrupts::handleSemWaitN(void* arg1, void* arg2) {
    MySemaphore* handle = (MySemaphore*) arg1;
    unsigned int n = (unsigned int) (uint64) arg2;
    return handle->wait_n(n);
}

int Interrupts::handleSemSignalN(void* arg1, void* arg2) {
    MySemaphore* handle = (MySemaphore*) arg1;
    unsigned int n = (unsigned int) (uint64) arg2;
    return handle->signal_n(n);
}

int Interrupts::handleTimeSleep(void* arg1) {
    time_t period = (time_t)arg1;
    TCB::insertSleepingThread(TCB::running,period);
    TCB::dispatch();
    return 0;
}

char Interrupts::handleGetC() {
    return ConsoleController::getChar();
}

void Interrupts::handlePutC(void* arg1) {
    char c = (char)(uint64)arg1;
    ConsoleController::putChar(c);
}

void Interrupts::handleSupervisorTrap() {
    uint64 args[5];
    asm  volatile("mv %0, a0" : "=r"(args[0]));
    asm  volatile("mv %0, a1" : "=r"(args[1]));
    asm  volatile("mv %0, a2" : "=r"(args[2]));
    asm  volatile("mv %0, a3" : "=r"(args[3]));
    asm  volatile("mv %0, a4" : "=r"(args[4]));

    uint64 scause = RiscvHardware::readScause();
    uint64 currentSepc = RiscvHardware::readSepc();
    uint64 currentSstatus = RiscvHardware::readSstatus();
    if (scause == Usermod_Ecall || scause == Kernelmod_Ecall) {
        currentSepc+=4;
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
            case 0x31: {
                int x = handleTimeSleep((void*)args[1]);
                RiscvHardware::writeA0OnStack((uint64) x);
                break;
            }
            case 0x41: {
                char c = handleGetC();
                RiscvHardware::writeA0OnStack((uint64) c);
                break;
            }
            case 0x42: {
                handlePutC((void*)args[1]);
                break;
            }
            default:
                break;
        }
        RiscvHardware::writeSepc(currentSepc);
        RiscvHardware::writeSstatus(currentSstatus);
    }
    else if (scause == Timer_Interrupt) {
        handleTimerInterrupt();
    }
    else if (scause == Hardware_Interrupt) {
        handleHardwareInterrupt();
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

void Interrupts::handleTimerInterrupt() {
    RiscvHardware::clearSipBit(RiscvHardware::SIP_SSIP);
    TCB* running = TCB::running;
    TCB::updateSleepList();
    TCB::timeSliceCounter++;
    if (running->getTimeSlice() <= TCB::timeSliceCounter) {
        uint64 currentSepc = RiscvHardware::readSepc();
        uint64 currentSstatus = RiscvHardware::readSstatus();
        TCB::dispatch();
        RiscvHardware::writeSstatus(currentSstatus);
        RiscvHardware::writeSepc(currentSepc);
    }

}

void Interrupts::handleHardwareInterrupt() {
    RiscvHardware::clearSipBit(RiscvHardware::SIP_SEIP);
    uint64 currentSstatus = RiscvHardware::readSstatus();
    int irq = plic_claim();
    if (irq == CONSOLE_IRQ) {
        ConsoleController::handleInterrupt();
    }
    plic_complete(irq);
    RiscvHardware::writeSstatus(currentSstatus);
}
