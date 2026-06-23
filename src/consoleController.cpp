#include "../h/consoleController.h"
#include "../h/tcb.h"
#include "../h/syscall_c.h"

BoundedBuffer<char>* ConsoleController::rxBuffer = nullptr;
BoundedBuffer<char>* ConsoleController::txBuffer = nullptr;

void ConsoleController::init() {
    rxBuffer = new BoundedBuffer<char>();
    txBuffer = new BoundedBuffer<char>();

    uint64 blocks = (DEFAULT_STACK_SIZE+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    void* stackAdr =  MemoryAllocator::getInstance().mem_alloc(blocks);
    TCB::createThread(txWorkerWrapper, nullptr, (uint64*) stackAdr, true);
}

uint8 ConsoleController::readConsoleStatus() {
    return *(uint8*)CONSOLE_STATUS;
}

bool ConsoleController::isTxReady() {
    return (readConsoleStatus() & (uint8)CONSOLE_TX_STATUS_BIT) != 0;
}

bool ConsoleController::isRxReady() {
    return (readConsoleStatus() & (uint8)CONSOLE_RX_STATUS_BIT) != 0;
}

void ConsoleController::writeConsoleTxReg(char c) {
    *(char*)CONSOLE_TX_DATA = c;
}

char ConsoleController::readConsoleRxReg() {
    return *(char*)CONSOLE_RX_DATA;
}

void ConsoleController::putChar(char c) {
    txBuffer->put(c);
}

char ConsoleController::getChar() {
    if (rxBuffer == nullptr) {
        return EOF;
    }
    char c = rxBuffer->get();
    if (TCB::running->getSemErrorStatus() == -1) {
        return EOF;
    }
    if (c == 4) {
        return EOF;
    }
    return c;
}


void ConsoleController::consoleTxWorker() {
    while (true) {
        char c = txBuffer->get();
        while (!isTxReady()) {}
        writeConsoleTxReg(c);
    }
}

void ConsoleController::txWorkerWrapper(void* arg) {
    consoleTxWorker();
}

void ConsoleController::flush() {
    while (!txBuffer->isEmpty()) {
        thread_dispatch();
    }
    while (!isTxReady()) {
        thread_dispatch();
    }
}

void ConsoleController::shutDown() {
    flush();
    delete txBuffer;
    delete rxBuffer;
}


void ConsoleController::handleInterrupt() {
    bool rxReady = isRxReady();
    while (rxReady) {
        char c = readConsoleRxReg();
        if (!rxBuffer->isFull()) {
            rxBuffer->put(c);
        }
        rxReady = isRxReady();
    }
}
