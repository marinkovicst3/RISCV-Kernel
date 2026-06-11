.extern _ZN10Interrupts20handleSupervisorTrapEv

.align 4
.global _ZN10Interrupts14supervisorTrapEv
.type _ZN10Interrupts14supervisorTrapEv, @function

.macro PUSH_ALL_REGS
    addi sp,sp,-256
    .irp idx, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\idx,\idx*8(sp)
    .endr
.endm

.macro POP_ALL_REGS
    .irp idx, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\idx,\idx*8(sp)
    .endr
    addi sp,sp,256
.endm

_ZN10Interrupts14supervisorTrapEv:
    PUSH_ALL_REGS
    call _ZN10Interrupts20handleSupervisorTrapEv
    POP_ALL_REGS
    sret