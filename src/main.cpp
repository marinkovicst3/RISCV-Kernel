#include "../h/tcb.h"
#include "../h/scheduler.h"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../h/riscvHardware.h"
#include "../h/handleInterrupts.h"

void kernel_print(const char* str) {
    while (*str) {
        __putc(*str++);
    }
}

void funkcijaA(void* arg) {
    for (int i = 0; i < 3; i++) {
        kernel_print("[Nit A] Radim krug\n");
        TCB::dispatch();
    }
}

void funkcijaB(void* arg) {
    for (int i = 0; i < 3; i++) {
        kernel_print("[Nit B] Radim krug\n");
        TCB::dispatch();
    }
}

int main() {
    MemoryAllocator::getInstance().init();
    uint64 funk = (uint64)&Interrupts::supervisorTrap;
    RiscvHardware::writeStvec(funk);
    kernel_print("--- START OPERATIVNOG SISTEMA ---\n");
    TCB* mainThread = new TCB(nullptr, nullptr);
    TCB::running = mainThread;
    void* stackA = mem_alloc(4096);
    void* stackB = mem_alloc(4096);

    TCB* nitA = new TCB(funkcijaA, nullptr, stackA);
    TCB* nitB = new TCB(funkcijaB, nullptr, stackB);

    Scheduler::getInstance().put(nitA);
    Scheduler::getInstance().put(nitB);

    kernel_print("[Main] Pokrećem dispatch vrtešku...\n");

    TCB::dispatch();

    kernel_print("--- KRAJ OPERATIVNOG SISTEMA (Main se vratio) ---\n");

    return 0;
}