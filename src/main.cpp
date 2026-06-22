#include "../h/tcb.h"
#include "../h/print.h"
#include "../h/riscvHardware.h"
#include "../h/handleInterrupts.h"
#include "../h/semaphore.h"
#include "../h/syscall_c.h"
#include "../test/Threads_C_API_test.hpp"
#include "../test/Threads_CPP_API_test.hpp"
#include "../test/ConsumerProducer_C_API_test.hpp"
#include "../test/System_Mode_test.hpp"
#include "../test/ThreadSleep_C_API_test.hpp"

static volatile bool testSystemFinished = false;
static volatile bool testCppFinished = false;
static volatile bool testCFinished = false;
static volatile bool testSleepFinished = false;

void runSystemModeTest(void* arg) {
    System_Mode_test();
    testSystemFinished = true;
    thread_dispatch();
}

void runCppApiTest(void* arg) {
    Threads_CPP_API_test();
    testCppFinished = true;
    thread_dispatch();
}

void runCApiTest(void* arg) {
    Threads_C_API_test();
    testCFinished = true;
    thread_dispatch();
}

void runSleepTest(void* arg) {
    testSleeping();
    testSleepFinished = true;
    thread_dispatch();
}

int main() {
    MemoryAllocator::getInstance().init();
    RiscvHardware::writeStvec((uint64)&Interrupts::supervisorTrap);
    TCB* mainThread = TCB::createThread(nullptr, nullptr, nullptr);
    TCB::running = mainThread;
    RiscvHardware::setSstatusBit(RiscvHardware::SSTATUS_SIE);
    thread_t t1, t2, t3,t4;
    thread_create(&t1, runCppApiTest, nullptr);

    printString("Nit za C API Test kreirana.\n");
    thread_create(&t2, runCApiTest, nullptr);
    printString("Nit za C API Test kreirana.\n");

    thread_create(&t3, runSystemModeTest, nullptr);
    printString("Nit za System Mode Test kreirana.\n");

    thread_create(&t4, runSleepTest, nullptr);
    printString("Nit za System Mode Test kreirana.\n");
    while (!(testSystemFinished && testCppFinished && testCFinished && testSleepFinished)) {
        thread_dispatch();
    }

    // System_Mode_test();
    // Threads_CPP_API_test();
    // Threads_C_API_test();
    //testSleeping();
    // producerConsumer_C_API();
    printString("Finished\n");
    return 0;
}
