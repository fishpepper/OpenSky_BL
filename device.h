#ifndef __DEVICE_H__
#define __DEVICE_H__

#define CLOCKSOURCE_OSC_PD_BIT 0x04
#define CLOCKSOURCE_XOSC_STABLE_BIT   0x40
#define CLOCKSOURCE_XOSC_STABLE()     (SLEEP & CLOCKSOURCE_XOSC_STABLE_BIT)

#ifndef NOP
    #define NOP() { __asm nop __endasm; }
#endif

#endif
