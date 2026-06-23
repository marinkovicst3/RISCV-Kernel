#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H

#include "../h/semaphore.h"

template <typename T, int CAPACITY = 256>
class BoundedBuffer {
public:

    void* operator new(size_t size) {
        uint64 blocks = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
        return MemoryAllocator::getInstance().mem_alloc(blocks);
    }

    void operator delete(void* ptr){
        MemoryAllocator::getInstance().mem_free(ptr);
    }

    BoundedBuffer() : head(0), tail(0) {
        spaceAvailable = new MySemaphore(CAPACITY);
        itemAvailable = new MySemaphore(0);
        mutex = new MySemaphore(1);
    }

    ~BoundedBuffer() {
        spaceAvailable->close();
        itemAvailable->close();
        mutex->close();
        delete spaceAvailable;
        delete itemAvailable;
        delete mutex;
    }

    void put(const T& item) {
        spaceAvailable->wait();
        mutex->wait();

        buffer[tail] = item;
        tail = (tail + 1) % CAPACITY;

        mutex->signal();
        itemAvailable->signal();
    }

    T get() {
        itemAvailable->wait();
        mutex->wait();

        T item = buffer[head];
        head = (head + 1) % CAPACITY;

        mutex->signal();
        spaceAvailable->signal();

        return item;
    }

    bool isFull() {
        mutex->wait();
        int count = (tail >= head) ? (tail - head) : (CAPACITY - (head - tail));
        mutex->signal();
        return count >= CAPACITY;
    }

    bool isEmpty() {
        mutex->wait();
        bool empty = (head == tail);
        mutex->signal();
        return empty;
    }

private:
    T buffer[CAPACITY];
    int head;
    int tail;

    MySemaphore* spaceAvailable;
    MySemaphore* itemAvailable;
    MySemaphore* mutex;
};
#endif
