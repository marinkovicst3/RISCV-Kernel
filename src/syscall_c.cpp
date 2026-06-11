#include  "../h/syscall_c.h"

void* abi_syscall(size_t opcode, size_t args1=0, size_t args2 = 0, size_t args3 = 0) {
    void* ret;
    __asm__ __volatile__(
        "ecall\n\t"
        "mv %[re],a0"
        : [re] "=r" (ret)
    );
    return ret;
}

void* mem_alloc(size_t size) {
    size_t blocks = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    return abi_syscall(0x01,blocks);
}

int mem_free(void* adr) {
    void* res = abi_syscall(0x02, (size_t)adr);
    return (int)(size_t)res;
}

