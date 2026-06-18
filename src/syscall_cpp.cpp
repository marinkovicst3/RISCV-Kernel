#include "../h/syscall_cpp.h"


Thread::Thread(void(*body)(void *), void *arg) : myHandle(nullptr),body(body),arg(arg){}

Thread::Thread() :Thread(nullptr,nullptr){}


Thread::~Thread() {}

int Thread::start() {
    return thread_create(&myHandle,body,arg);
}
void Thread::run() {}

Semaphore::Semaphore(unsigned init) {
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

// int Thread::sleep(time_t) {
// }


