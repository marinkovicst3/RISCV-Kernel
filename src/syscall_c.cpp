#include  "../h/syscall_c.h"

void* abi_syscall(uint64 opcode, uint64 args1=0, uint64 args2 = 0, uint64 args3 = 0,uint64 args4 = 0) {
    void* ret;
    __asm__ __volatile__(
        "ecall\n\t"
        "mv %[re],a0"
        : [re] "=r" (ret)
    );
    return ret;
}

void* mem_alloc(uint64 size) {
    uint64 blocks = (size+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    return abi_syscall(0x01,blocks);
}

int mem_free(void* adr) {
    void* res = abi_syscall(0x02, (uint64)adr);
    return (int)(uint64)res;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void *arg) {
    if (!handle) return -1;
    void* stack_adr = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_adr == nullptr) return -1;
    void* res = abi_syscall(0x11, (uint64)handle, (uint64)start_routine, (uint64)arg, (uint64)stack_adr);
    return (int)(uint64)res;
}

int thread_exit() {
    return (int) (uint64) abi_syscall(0x12);
}

void thread_dispatch() {
    abi_syscall(0x13);
}

int sem_open(sem_t*handle, unsigned int init) {
    if (!handle)return -1;
    return (int)(uint64)abi_syscall(0x21, (uint64)handle, init);
}

int sem_close(sem_t handle) {
    if (!handle)return -1;
    return (int)(uint64)abi_syscall(0x22, (uint64)handle);
}

int sem_wait(sem_t id) {
    if (!id)return -1;
    return (int)(uint64)abi_syscall(0x23, (uint64)id);
}

int sem_signal(sem_t id) {
    if (!id)return -1;
    return (int)(uint64)abi_syscall(0x24, (uint64)id);
}

int sem_wait_n(sem_t id, unsigned int n) {
    if (!id)return -1;
    return (int)(uint64)abi_syscall(0x25, (uint64)id,n);
}

int sem_signal_n(sem_t id, unsigned int n) {
    if (!id)return -1;
    return (int)(uint64)abi_syscall(0x26, (uint64)id,n);
}

int time_sleep(time_t period) {
    if (period <= 0) return -1;
    return (int)(uint64)abi_syscall(0x31, period);
}

