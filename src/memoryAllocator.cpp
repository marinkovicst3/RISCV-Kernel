
#include "../h/memoryAllocator.h"

MemoryAllocator* MemoryAllocator::instance = nullptr;

MemoryAllocator& MemoryAllocator::getInstance() {
    if(instance == nullptr){
        static MemoryAllocator glAlloc;
        instance = &glAlloc;
    }
    return *instance;
}


void MemoryAllocator::init() {
    size_t alignedStart = roundBytesToBlock((size_t) HEAP_START_ADDR);
    size_t alignedEnd   = ((size_t)HEAP_END_ADDR) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;

    head = (Block*) alignedStart;
    head->next = nullptr;
    head->prev = nullptr;
    head->isFree = true;
    head->size = alignedEnd - alignedStart - sizeof(Block);
}


MemoryAllocator::Block* MemoryAllocator::findFirstFit(size_t size) {
    Block* tmpHead = head;
    while(tmpHead){
        if(tmpHead->isFree && tmpHead->size >= size) break;
        tmpHead = tmpHead->next;
    }
    return tmpHead;
}


size_t MemoryAllocator::roundBytesToBlock(size_t size) {
    size_t round = ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE) * MEM_BLOCK_SIZE;
    return round;
}

void* MemoryAllocator::getAdr(Block *frag) {
    return (void*)((char*) frag + sizeof(Block));
}

void MemoryAllocator::split(Block* main, size_t bytesNeeded) {
    size_t remaining = main->size - bytesNeeded - sizeof(Block);
    Block* newBlock = (Block*)((char*) getAdr(main) + bytesNeeded);

    newBlock->isFree = true;
    newBlock->size = remaining;
    newBlock->prev = main;
    newBlock->next = main->next;

    if (main->next) {
        main->next->prev = newBlock;
    }
    main->next = newBlock;
    main->size = bytesNeeded;
}

void MemoryAllocator::join(Block *first, Block *second) {
    if(!first || !second) return;
    if(!first->isFree || !second->isFree) return;
    first->size += second->size + sizeof(Block);
    first->next = second->next;
    if(second->next) second->next->prev = first;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if(size == 0) return nullptr;

    size_t bytesNeeded = size*MEM_BLOCK_SIZE;
    Block* frag = findFirstFit(bytesNeeded);
    if(!frag) return nullptr;

    void* adr = getAdr(frag);

    if(frag->size >= bytesNeeded + sizeof(Block) + MEM_BLOCK_SIZE){
        split(frag,bytesNeeded);
    }
    frag->isFree = false;
    return adr;
}


int MemoryAllocator::mem_free(void* addr) {
    if(addr == nullptr)return -1;
    Block* blk = (Block*)((char*) addr - sizeof(Block));
    blk->isFree = true;
    join(blk,blk->next);
    join(blk->prev,blk);
    return 0;
}





