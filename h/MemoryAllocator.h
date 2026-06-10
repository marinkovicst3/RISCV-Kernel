#ifndef PROJECT_BASE_MEMORYALLOCATOR_H
#define PROJECT_BASE_MEMORYALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator{
public:

    static MemoryAllocator& getInstance();

    void init();
    void* mem_alloc(size_t size);
    int mem_free(void* addr);

private:
    MemoryAllocator() : head(nullptr){}

    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;

    struct Block{
        bool isFree;
        size_t size;
        Block* next;
        Block* prev;
    };

    Block* findFirstFit(size_t size);
    size_t roundBytesToBlock(size_t size);
    void* getAdr(Block* frag);
    void split(Block* main,size_t blocksNeeded);
    void join(Block* first, Block* second);

    static MemoryAllocator* instance;
    Block* head;
};

#endif
