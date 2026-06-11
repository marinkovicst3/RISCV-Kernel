#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include  "../lib/hw.h"

    void* mem_alloc(size_t size);
    int mem_free(void*);

#endif
