#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include  "../lib/hw.h"
    void* abi_syscall(size_t opcode, size_t args1,size_t args2,size_t args3);

    void* mem_alloc(size_t size);
    int mem_free(void*);

    class TCB;
    typedef TCB* thread_t;
    int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
    int thread_exit();
    void thread_dispatch();

    class MySemaphore;
    typedef MySemaphore* sem_t;
    int sem_open(sem_t* handle, unsigned init);
    int sem_close(sem_t handle);
    int sem_wait(sem_t id);
    int sem_signal(sem_t id);
    int sem_wait_n(sem_t id, unsigned n);
    int sem_signal_n(sem_t id, unsigned n);

    typedef unsigned long time_t;
    // int time_sleep(time_t);
    //
    // const int EOF = -1;
    // char getc();
    // void putc(char);

#endif
