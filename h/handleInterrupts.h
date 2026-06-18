#ifndef HANDLEINTERRUPTS_H
#define HANDLEINTERRUPTS_H

class Interrupts {
public:
    static void supervisorTrap();
    static void handleSupervisorTrap();

private:
    static void* handleMemAlloc();
    static int handleMemFree();
    static int handleThreadCreate();
    static int handleSemOpen();
    static int handleSemClose();
    static int handleSemWait();
    static int handleSemWaitN();
    static int handleSemSignal();
    static int handleSemSignalN();

};

#endif
