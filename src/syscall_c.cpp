#include  "../h/syscall_c.h"

void* abi_syscall(size_t opcode, size_t args1=0, size_t args2 = 0, size_t args3 = 0,size_t args4 = 0) {
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

int thread_create(thread_t* handle, void(*start_routine)(void*), void *arg) {
    if (!handle) return -1;
    void* stack_adr = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_adr == nullptr) return -1;
    uint64* stack_top = (uint64*)((char*)stack_adr + DEFAULT_STACK_SIZE);
    void* res = abi_syscall(0x11, (size_t)handle, (size_t)start_routine, (size_t)arg, (size_t)stack_top);
    return (int)(size_t)res;
}

int thread_exit() {
    return (int) (uint64) abi_syscall(0x12);
}

void thread_dispatch() {
    abi_syscall(0x13);
}

int sem_open(sem_t*handle, unsigned init) {
    return (int)(size_t)abi_syscall(0x21, (size_t)handle, (size_t)init);
}

int sem_close(sem_t handle) {
    return (int)(size_t)abi_syscall(0x22, (size_t)handle);
}

int sem_wait(sem_t id) {
    return (int)(size_t)abi_syscall(0x23, (size_t)id);
}

int sem_signal(sem_t id) {
    return (int)(size_t)abi_syscall(0x24, (size_t)id);
}

int sem_wait_n(sem_t id, unsigned n) {
    return (int)(size_t)abi_syscall(0x25, (size_t)id,(size_t) n);
}

int sem_signal_n(sem_t id, unsigned n) {
    return (int)(size_t)abi_syscall(0x26, (size_t)id,(size_t) n);
}

