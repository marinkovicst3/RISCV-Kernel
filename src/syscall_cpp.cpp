#include "../h/syscall_cpp.h"


Thread::Thread(void(*body)(void *), void *arg) : myHandle(nullptr),body(body),arg(arg){}

Thread::Thread() :Thread(nullptr,nullptr){}


Thread::~Thread() {}

int Thread::start() {
    return thread_create(&myHandle,body,arg);
}
void Thread::run() {}

void Thread::dispatch() {
    thread_dispatch();
}

// int Thread::sleep(time_t) {
// }


