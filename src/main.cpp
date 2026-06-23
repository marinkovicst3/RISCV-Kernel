#include "../h/consoleController.h"
#include "../h/tcb.h"
#include "../test/printing.hpp"
#include "../h/riscvHardware.h"
#include "../h/handleInterrupts.h"
#include "../h/syscall_c.h"

extern void userMain();

void userMainWrapper(void* arg){
    userMain();
}

int main() {
    MemoryAllocator::getInstance().init();
    ConsoleController::init();

    RiscvHardware::writeStvec((uint64)&Interrupts::supervisorTrap);

    TCB* mainThread = TCB::createThread(nullptr, nullptr, nullptr, true);
    TCB::running = mainThread;

    RiscvHardware::setSstatusBit(RiscvHardware::SSTATUS_SIE);
    RiscvHardware::setSieBit(RiscvHardware::SIE_SEIE | RiscvHardware::SIE_SSIE);

    TCB* usermain;
    thread_create(&usermain,userMainWrapper, nullptr);
    while(!usermain->isFinished()){
        thread_dispatch();
    }

    delete usermain;
    printString("Kernel Finished\n");
    ConsoleController::shutDown();
    RiscvHardware::stopEmulator();
    return 0;
}