.global contextSwitch
contextSwitch:
    sd ra, 0(a0)
    sd sp, 8(a0)
    ld ra, 0(a1)
    ld sp, 8(a1)
    ret