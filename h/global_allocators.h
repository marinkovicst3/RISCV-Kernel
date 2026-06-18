#ifndef GLOBAL_ALLOCATORS_H
#define GLOBAL_ALLOCATORS_H

#include "../lib/hw.h"

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;


#endif
