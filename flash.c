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

#include "flash.h"
#include "config.h"
#include "main.h"
#include "device.h"
#include "cc25xx.h"

void flash_init(void) {
}

// NOTE: this will read len+1 bytes to buffer buf
void flash_read(uint16_t address, __xdata uint8_t *buf, uint8_t len) {
    __xdata uint8_t * flash_ptr = (__xdata uint8_t *)address;

    // copy len+1 bytes to buf
    *buf++ = *flash_ptr++;
    while (len--) {
        *buf++ = *flash_ptr++;
    }
}

uint8_t flash_write_data(uint16_t address, uint8_t *buf, uint8_t len) {
    uint8_t loop_counter;
    uint8_t *data_ptr;
    __xdata uint8_t * flash_ptr;

    // make sure not to overwrite bootloader:
    if (address < BOOTLOADER_SIZE) {
        return 0;
    }

    // make sure not to overwrite beyound valid flash region:
    if ((address + len + 1) >= DEVICE_FLASH_SIZE) {
        return 0;
    }

    // wait until flash controller is ready
    while (FCTL & FCTL_BUSY) {}

    // set up flash write start address
    FADDRH = (address >> 8) & 0xFF;
    FADDRL = address & 0xFF;

    // configure flash controller for 26mhz clock
    FWT = 0x2A;

    // we have to write len+1 bytes and we have to make sure to alwys write two bytes
    // calculate number of iterations:
    loop_counter = (len / 2) + 1;

    // prepare read pointer
    data_ptr = &buf[0];

    // enable write
    FCTL |= FCTL_WRITE;
    NOP();

    // do write loop
    while (loop_counter--) {
        FWDATA = *data_ptr++;
        FWDATA = *data_ptr++;

        while (FCTL & FCTL_SWBUSY) {}
    }

    // verify write
    flash_ptr = (__xdata uint8_t *)address;
    data_ptr  = &buf[0];

    if ((*flash_ptr) != (*data_ptr)) {
        return 0;
    }
    flash_ptr++;
    data_ptr++;

    while (len--) {
        if ((*flash_ptr) != (*data_ptr)) {
            return 0;
        }
        flash_ptr++;
        data_ptr++;
    }

    return 1;
}

uint8_t flash_erase_page(uint8_t page) {
    if (page >= PAGECOUNT_FLASH) {
        // maximum page exceeded, abort
        return 0;
    }

    // deny deletion of bootloader
    if (page < PAGECOUNT_BOOTLOADER) {
        return 0;
    }

    // wait until flash controller is ready
    while (FCTL & FCTL_BUSY) {}

    // set up flash address FADDRH[5:1] set flash page
    FADDRH = page << 1;

    // configure flash controller for 26mhz clock
    FWT = 0x2A;

    // execute erase
    FCTL |= FCTL_ERASE;
    NOP();

    return 1;
}

