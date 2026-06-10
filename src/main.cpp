#include "../h/MemoryAllocator.h"
#include "../lib/console.h"
void main() {
    MemoryAllocator::getInstance().init();

    void* p1 = MemoryAllocator::getInstance().mem_alloc(100);
    if (p1 != nullptr) {
        __putc('A');
        void* p2 = MemoryAllocator::getInstance().mem_alloc(200);
        MemoryAllocator::getInstance().mem_free(p1);
        MemoryAllocator::getInstance().mem_free(p2);
    }

}
