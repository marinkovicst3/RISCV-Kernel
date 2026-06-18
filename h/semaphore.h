#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "deque.h"

class TCB;

class MySemaphore {
public:
    MySemaphore(unsigned val);
    int wait();
    int signal();

    int wait_n(unsigned n);
    int signal_n(unsigned n);

    int close();

private:
    void block(unsigned val);
    void unblock();

    bool closed;
    Deque<TCB*> blocked;
    int val;
};

#endif
