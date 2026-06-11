#ifndef RISCV_HARDWARE_H
#define RISCV_HARDWARE_H

#include "../lib/hw.h"

class RiscvHardware{
public:

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

    static uint64 readA0();
    static void writeA0(uint64 value);

    static uint64 readA1();
    static void writeA1(uint64 value);

    static uint64 readA2();
    static void writeA2(uint64 value);
};


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

inline uint64 RiscvHardware::readA0() {
    uint64 volatile reg_a0;
    __asm__ volatile ("mv %[out_a0], a0" : [out_a0] "=r"(reg_a0));
    return reg_a0;
}

inline void RiscvHardware::writeA0(uint64 value) {
    __asm__ volatile ("mv a0, %[in_a0]" : : [in_a0] "r"(value));
}

inline uint64 RiscvHardware::readA1() {
    uint64 volatile reg_a1;
    __asm__ volatile ("mv %[out_a1], a1" : [out_a1] "=r"(reg_a1));
    return reg_a1;
}

inline void RiscvHardware::writeA1(uint64 value) {
    __asm__ volatile ("mv a1, %[in_a1]" : : [in_a1] "r"(value));
}

inline uint64 RiscvHardware::readA2() {
    uint64 volatile reg_a2;
    __asm__ volatile ("mv %[out_a2], a2" : [out_a2] "=r"(reg_a2));
    return reg_a2;
}

inline void RiscvHardware::writeA2(uint64 value) {
    __asm__ volatile ("mv a2, %[in_a2]" : : [in_a2] "r"(value));
}
#endif