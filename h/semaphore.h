#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "deque.h"

class TCB;

class MySemaphore {
public:
    void* operator new(size_t size) {
        uint64 blocks = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
        return MemoryAllocator::getInstance().mem_alloc(blocks);
    }

    void operator delete(void* ptr){
        MemoryAllocator::getInstance().mem_free(ptr);
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
