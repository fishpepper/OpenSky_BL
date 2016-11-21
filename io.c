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

#include "io.h"
#include "config.h"
#include <cc2510fx.h>
#include "portmacros.h"

void io_init(void) {
    // enable pullup on bootloader input
    PORT2INP(BOOTLOADER_ENABLE_PORT) &= ~(1 << BOOTLOADER_ENABLE_PIN);

    // set bootloader activation pin to input
    PORT2DIR(BOOTLOADER_ENABLE_PORT) &= ~(1 << BOOTLOADER_ENABLE_PIN);
}

uint8_t io_bootloader_enabled(void) {
    if (BOOTLOADER_ENABLE_PORT & (1 << BOOTLOADER_ENABLE_PIN)) {
        // pin is high
        return 0;
    } else {
        // pin is low
        return 1;
    }
}
