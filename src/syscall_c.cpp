#include  "../h/syscall_c.h"

void *mem_alloc(size_t size) {
    size_t blocks = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    size_t opcode = 0x01;
    __asm__ __volatile__(
        "mv a0,%[code]\n\t"
        "mv a1,%[bl]\n\t"
        "ecall"
        : : [code] "r" (opcode),
            [bl] "r" (blocks)
        : "a0","a1"
    );

    void* ret;
    asm volatile ("mv %[re], a0" : [re] "=r" (ret));
    return ret;
}

int mem_free(void* adr) {
    
}
