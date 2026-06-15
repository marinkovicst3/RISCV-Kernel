#include "../h/tcb.h"

Status TCB::getStatus() {return status;}

void TCB::setUserStack(uint64 *stck) { this->userStack = stck; }
