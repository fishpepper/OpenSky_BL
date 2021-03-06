/*
    Copyright 2016 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    author: fishpepper <AT> gmail.com
*/

#ifndef DEVICE_H_
#define DEVICE_H_
#include <cc2510fx.h>

#define CLOCKSOURCE_OSC_PD_BIT        0x04
#define CLOCKSOURCE_XOSC_STABLE_BIT   0x40
#define CLOCKSOURCE_XOSC_STABLE()     (SLEEP & (CLOCKSOURCE_XOSC_STABLE_BIT))

#ifndef NOP
    #define NOP() { __asm nop __endasm; }
#endif  // NOP

#endif  // DEVICE_H_
