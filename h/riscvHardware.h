#ifndef RISCV_HARDWARE_H
#define RISCV_HARDWARE_H

#include "../lib/hw.h"

class RiscvHardware{
public:
    static void stopEmulator();

    static void popSppSpie();

    static uint64 readScause();
    static void writeScause(uint64 scause);

    static uint64 readSepc();
    static void writeSepc(uint64 sepc);

    static uint64 readStvec();
    static void writeStvec(uint64 stvec);

    static uint64 readStval();
    static void writeStval(uint64 stval);

    enum SipMask
    {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9),
    };

    static void setSipBit(uint64 mask);
    static void clearSipBit(uint64 mask);
    static uint64 readSip();
    static void writeSip(uint64 sip);

    enum SstatusMask
    {
        SSTATUS_SIE  = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP  = (1 << 8),
    };

    static void setSstatusBit(uint64 mask);
    static void clearSstatusBit(uint64 mask);
    static uint64 readSstatus();
    static void writeSstatus(uint64 sstatus);

    static void writeA0OnStack(uint64 value);
};

inline void RiscvHardware::stopEmulator() {
    asm volatile("li t0, 0x5555");
    asm volatile("li t1, 0x100000");
    asm volatile("sw t0, 0(t1)");
}

inline uint64 RiscvHardware::readScause() {
    uint64 volatile val;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(val));
    return val;
}

inline void RiscvHardware::writeScause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscvHardware::readSepc() {
    uint64 volatile val;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(val));
    return val;
}

inline void RiscvHardware::writeSepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscvHardware::readStvec() {
    uint64 volatile val;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(val));
    return val;
}

inline void RiscvHardware::writeStvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscvHardware::readStval() {
    uint64 volatile val;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(val));
    return val;
}

inline void RiscvHardware::writeStval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void RiscvHardware::setSipBit(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscvHardware::clearSipBit(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscvHardware::readSip() {
    uint64 volatile val;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(val));
    return val;
}

inline void RiscvHardware::writeSip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscvHardware::setSstatusBit(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscvHardware::clearSstatusBit(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscvHardware::readSstatus() {
    uint64 volatile val;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(val));
    return val;
}

inline void RiscvHardware::writeSstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline void RiscvHardware::writeA0OnStack(uint64 value) {
    uint64 current_fp;
    __asm__ __volatile__("mv %0, s0" : "=r"(current_fp));
    uint64* a0_address = (uint64*)(current_fp + 80);
    *a0_address = value;
}
#endif