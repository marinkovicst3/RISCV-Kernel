#include "../h/print.h"
#include "../h/riscvHardware.h"
#include "../lib/console.h"

void printString(char const *string)
{
    uint64 sstatus = RiscvHardware::readSstatus();
    RiscvHardware::clearSstatusBit(RiscvHardware::SSTATUS_SIE);
    while (*string != '\0')
    {
        __putc(*string);
        string++;
    }
    RiscvHardware::setSstatusBit(sstatus & RiscvHardware::SSTATUS_SIE ? RiscvHardware::SSTATUS_SIE : 0);
}

void printInteger(uint64 integer)
{
    uint64 sstatus = RiscvHardware::readSstatus();
    RiscvHardware::clearSstatusBit(RiscvHardware::SSTATUS_SIE);
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (integer < 0)
    {
        neg = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { __putc(buf[i]); }
    RiscvHardware::setSstatusBit(sstatus & RiscvHardware::SSTATUS_SIE ? RiscvHardware::SSTATUS_SIE : 0);
}