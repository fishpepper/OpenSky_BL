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

#ifndef FLASH_H_
#define FLASH_H_
#include <stdint.h>

void flash_init(void);
void flash_read(uint16_t address, __xdata uint8_t *buf, uint8_t len);
uint8_t flash_write_data(uint16_t address, uint8_t *buf, uint8_t len);
uint8_t flash_erase_page(uint8_t page);

<<<<<<< HEAD
=======
//HAL_DMA CONFIG
//see https://e2e.ti.com/support/wireless_connectivity/f/156/t/16922
typedef struct {
    uint8_t SRCADDRH;
    uint8_t SRCADDRL;
    uint8_t DESTADDRH;
    uint8_t DESTADDRL;
    uint8_t LENH      : 5;
    uint8_t VLEN      : 3;
    uint8_t LENL      : 8;
    uint8_t TRIG      : 5;
    uint8_t TMODE     : 2;
    uint8_t WORDSIZE  : 1;
    uint8_t PRIORITY  : 2;
    uint8_t M8        : 1;
    uint8_t IRQMASK   : 1;
    uint8_t DESTINC   : 2;
    uint8_t SRCINC    : 2;
} dma_desc_t;


>>>>>>> 130cb01cb16afaa9909d41ce7e03d9f84e154ef0

#if ((DEVICE_FLASH_SIZE) > 65535)
    #error "ERROR: maximum supported flash size is 64k!"
#endif  // ((DEVICE_FLASH_SIZE) > 65535)

#define PAGECOUNT_BOOTLOADER ((BOOTLOADER_SIZE) / (DEVICE_FLASH_PAGESIZE))
#define PAGECOUNT_FLASH      ((DEVICE_FLASH_SIZE) / (DEVICE_FLASH_PAGESIZE))


#endif  // FLASH_H_
