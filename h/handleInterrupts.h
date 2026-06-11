#ifndef PROJECT_BASE_HANDLETRAPS_H
#define PROJECT_BASE_HANDLETRAPS_H

class Interrupts {
public:
    static void supervisorTrap();
    static void handleSupervisorTrap();
private:
    static void* handleMemAlloc();
    static void handleMemFree(void* adr);
};

#endif
