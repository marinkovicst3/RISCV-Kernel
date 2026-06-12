#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "deque.h"

class TCB;

class Scheduler {
public:
    static Scheduler& getInstance();
    TCB* get();
    void put(TCB* thread);
private:
    Scheduler():readyThreads(){}

    Deque<TCB*> readyThreads;
    static Scheduler* instance;
};

#endif
