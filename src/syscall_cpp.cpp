#include "../h/syscall_cpp.h"

void* operator new(size_t sz) {
    return mem_alloc(sz);
}

void operator delete(void* ptr) {
    mem_free(ptr);
}

void* operator new[](size_t sz) {
    return mem_alloc(sz);
}

void operator delete[](void* ptr) {
    mem_free(ptr);
}

Thread::Thread(void(*body)(void *), void *arg) : myHandle(nullptr),body(body),arg(arg){}

Thread::Thread() :Thread(nullptr,nullptr){}

Thread::~Thread() {}

int Thread::start() {
    if (body) {
        return thread_create(&myHandle,body,arg);
    }
    return thread_create(&myHandle,&Thread::wrapper,this);
}

void Thread::wrapper(void* obj) {
    if (obj != nullptr) {
        ((Thread*)obj)->run();
    }
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t period) {
    return time_sleep(period);
}

void Thread::run() {}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    if (myHandle!=nullptr) {
        sem_close(this->myHandle);
    }
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}

PeriodicThread::PeriodicThread(time_t period): running(false),period(period) {}

void PeriodicThread::periodicActivation() {}

void PeriodicThread::terminate() {running = false;}

void PeriodicThread::run() {
    running = true;
    while (running) {
        periodicActivation();
        if (!running) { break; }
        time_sleep(period);
    }
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}




