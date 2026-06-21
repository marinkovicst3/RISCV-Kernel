#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "deque.h"

class TCB;

class MySemaphore {
public:
    void* operator new(size_t size) {
        return MemoryAllocator::getInstance().mem_alloc(size);
    }

    MySemaphore(unsigned int val);

    int wait();
    int signal();

    int wait_n(unsigned int n);
    int signal_n(unsigned int n);

    int close();

private:
    void block(unsigned int val);
    void unblock();

    bool closed;
    Deque<TCB*> blocked;
    int val;
};

#endif
