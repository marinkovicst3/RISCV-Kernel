#  Kernel — RISC-V Multithreaded OS

A small, library-style operating system kernel for the **RISC-V (RV64IMA)** architecture, implemented in C++.

The kernel provides threads, semaphores, time-sharing, and asynchronous preemption on timer and keyboard interrupts — running statically linked with the user application inside the `xv6`-based host environment, emulated with QEMU.

---

## Overview

This project implements a minimal but fully functional multithreaded kernel with time sharing, following an embedded-systems style design: the kernel and the user application are compiled and statically linked into a single executable that shares one address space. Concurrent "processes" created by the application are lightweight threads managed entirely by the kernel — memory allocation, thread management, synchronization, and I/O are all implemented from scratch, without relying on any host OS or standard library services.

The kernel exposes its functionality through three layered interfaces:

```
  User's program
        │
   C++ OO API   ← object-oriented wrapper
        │
      C API     ← procedural syscall wrappers
        │
       ABI      ← binary syscall interface (software interrupt)
        │
      Kernel
        │
  HW access module
```

---

## Features

- **Memory allocation** — `mem_alloc` / `mem_free`, block-based continuous allocation (first-fit)
- **Threads** — `thread_create`, `thread_exit`, `thread_dispatch`, with kernel-managed stacks and context switching
- **Semaphores** — `sem_open`, `sem_close`, `sem_wait` / `sem_signal`, plus batched `sem_wait_n` / `sem_signal_n`
- **Time sharing & preemption** — synchronous and asynchronous context switching on timer interrupts, configurable time slices
- **Sleep / wake** — `time_sleep`, with an efficient sorted wake-time list
- **Console I/O** — interrupt-driven, buffered `getc` / `putc` over a UART-style serial console
- **C++ object-oriented API** — `Thread`, `Semaphore`, `PeriodicThread`, and `Console` wrapper classes, plus `new` / `delete` operators routed through the kernel allocator
- **Single-processor, monolithic kernel design** — all kernel code runs in the same address space in privileged mode

---

## Tech Stack

- **C++** for the kernel, C++ / RISC-V assembly (`.S` files) for context switching and low-level routines
- Target: **RISC-V RV64IMA**
- Toolchain: `gcc` for RISC-V, built and run via `make`
- Emulated with **QEMU**, running under a modified **xv6** host system
- Developed in **CLion**, inside the VM
