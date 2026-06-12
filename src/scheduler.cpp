#include "../h/scheduler.h"

Scheduler* Scheduler::instance = nullptr;

Scheduler & Scheduler::getInstance() {
    if (!instance) {
        static Scheduler sch;
        instance = &sch;
    }
    return *instance;
}

TCB* Scheduler::get() {
    if (readyThreads.empty()) return nullptr;
    TCB* tmp = readyThreads.front();
    readyThreads.pop_front();
    return tmp;
}

void Scheduler::put(TCB* thread) {
    readyThreads.push_back(thread);
}
