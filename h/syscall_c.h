#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include  "../lib/hw.h"
    void* abi_syscall(size_t opcode, size_t args1,size_t args2,size_t args3);
    void* mem_alloc(size_t size);
    int mem_free(void*);

#endif
