#ifndef HANDLEINTERRUPTS_H
#define HANDLEINTERRUPTS_H

class Interrupts {
public:
    static void supervisorTrap();
    static void handleSupervisorTrap();

private:
    static void* handleMemAlloc(void* arg1);
    static int handleMemFree(void* arg1);
    static int handleThreadCreate(void* arg1,void* arg2,void* arg3,void* arg4);
    static int handleSemOpen(void* arg1,void* arg2);
    static int handleSemClose(void* arg1);
    static int handleSemWait(void* arg1);
    static int handleSemWaitN(void* arg1,void* arg2);
    static int handleSemSignal(void* arg1);
    static int handleSemSignalN(void* arg1,void* arg2);

};

#endif
