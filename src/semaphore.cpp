#include "../h/semaphore.h"

#include "../h/scheduler.h"
#include "../h/tcb.h"

MySemaphore::MySemaphore(unsigned val):closed(false) ,val((int)val){}

int MySemaphore::wait() {
    return wait_n(1);
}

int MySemaphore::signal() {
    return signal_n(1);
}

int MySemaphore::wait_n(unsigned n) {
    if (closed) return -1;
    if (val >= (int)n) {
        val -= (int)n;
        return 0;
    }
    block(n);
    return TCB::running->getSemErrorStatus();
}

int MySemaphore::signal_n(unsigned n) {
    if (closed) return -1;
    val+=(int)n;
    unblock();
    return 0;
}

int MySemaphore::close() {
    if (closed) return -1;
    closed = true;
    while (!blocked.empty()) {
        TCB* tcb = blocked.front();
        blocked.pop_front();
        tcb->setSemErrorStatus(-1);
        tcb->setBlocked(false);
        Scheduler::getInstance().put(tcb);
    }
    return 0;
}

void MySemaphore::block(unsigned n) {
    TCB::running->setSemUnits((int)n);
    TCB::running->setSemErrorStatus(0);
    TCB::running->setBlocked(true);
    blocked.push_back(TCB::running);
    TCB::yield();
}

void MySemaphore::unblock() {
    Deque<TCB*>::Iterator it = blocked.getIterator();
    while (it.hasNext()) {
        TCB* tcb = it.getData();
        int units = (int) tcb->getSemUnits();
        if (units<=val) {
            val-=units;
            blocked.erase(it);
            tcb->setBlocked(false);
            tcb->setSemErrorStatus(0);
            Scheduler::getInstance().put(tcb);
        }else {
            it.next();
        }
    }
}
