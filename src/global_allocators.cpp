#include "../h/memoryAllocator.h"
#include "../lib/hw.h"

void* operator new(size_t size) {
    return MemoryAllocator::getInstance().mem_alloc(size);
}

void* operator new[](size_t size) {
    return MemoryAllocator::getInstance().mem_alloc(size);
}

void operator delete(void* ptr) noexcept {
    MemoryAllocator::getInstance().mem_free(ptr);
}

void operator delete[](void* ptr) noexcept {
    MemoryAllocator::getInstance().mem_free(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    MemoryAllocator::getInstance().mem_free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    MemoryAllocator::getInstance().mem_free(ptr);
}