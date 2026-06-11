#ifndef HANDLEINTERRUPTS_H
#define HANDLEINTERRUPTS_H

class Interrupts {
public:
    static void supervisorTrap();
    static void handleSupervisorTrap();

private:
    static void* handleMemAlloc();
    static int handleMemFree();
};

#endif
