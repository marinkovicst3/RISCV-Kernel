#ifndef CONSOLECONTROLLER_H
#define CONSOLECONTROLLER_H

#include "boundedBuffer.h"
#include "../lib/hw.h"

class ConsoleController {
private:
    friend class Interrupts;

    static BoundedBuffer<char>* rxBuffer;
    static BoundedBuffer<char>* txBuffer;

    static void consoleTxWorker();
    static void txWorkerWrapper(void* arg);

    static uint8 readConsoleStatus();
    static bool isTxReady();
    static bool isRxReady();
    static void writeConsoleTxReg(char c);
    static char readConsoleRxReg();

public:
    static void flush();
    static void shutDown();
    static void handleInterrupt();
    static void init();
    static void putChar(char c);
    static char getChar();
};

#endif
