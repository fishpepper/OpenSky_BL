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
#include "dma.h"
#include "device.h"
#include "cc25xx.h"

__xdata dma_desc_t flash_dma_config;

void flash_init(void) {
    // cancel _ALL_ ongoing DMA transfers:
    DMAARM = DMA_ARM_ABORT | 0x1F;

    flash_dma_config.PRIORITY       = DMA_PRI_HIGH;         // high prio
    flash_dma_config.M8             = DMA_M8_USE_8_BITS;    // irrelevant since we use LEN
    flash_dma_config.IRQMASK        = DMA_IRQMASK_DISABLE;  // disable ints from this ch
    flash_dma_config.TRIG           = DMA_TRIG_FLASH;       // use dma data write complete trigger
    flash_dma_config.TMODE          = DMA_TMODE_SINGLE;     // single mode, see datasheet
    flash_dma_config.WORDSIZE       = DMA_WORDSIZE_BYTE;    // one byte
    flash_dma_config.VLEN           = DMA_VLEN_USE_LEN;     // use LEN
    flash_dma_config.SRCINC         = DMA_SRCINC_1;         // set srcinc to 1 byte
    flash_dma_config.DESTINC        = DMA_DESTINC_0;        // fixed, always write to FWDATA

    // set length of transfer in bytes
    SET_WORD(flash_dma_config.LENH, flash_dma_config.LENL, 0);
    // set src: address of data to be written
    SET_WORD(flash_dma_config.SRCADDRH,  flash_dma_config.SRCADDRL,  0);
    // destination is flash controller data reg
    SET_WORD(flash_dma_config.DESTADDRH, flash_dma_config.DESTADDRL, &X_FWDATA);

    // save pointer to the DMA configuration struct into DMA-channel 0
    // configuration registers
    SET_WORD(DMA0CFGH, DMA0CFGL, flash_dma_config);
}


// NOTE: this will read len+1 bytes to buffer buf
void flash_read(uint16_t address, __xdata uint8_t *buf, uint16_t len) {
    __xdata uint8_t * flash_ptr = (__xdata uint8_t *)address;

    // copy len bytes to buf
    while (len--) {
        *buf++ = *flash_ptr++;
    }
}

uint8_t flash_write_data(uint16_t address, __xdata uint8_t *buf, uint16_t len) {
    // make sure not to overwrite bootloader:
    if (address < BOOTLOADER_SIZE) {
        return 0;
    }

    // make sure not to overwrite beyound valid flash region:
    if ((address + len) >= FLASH_SIZE) {
        return 0;
    }

    if (address & 1) {
        // this should not happen, flash write needs an even start address
        return 0;
    }

    if (len & 1) {
        // this should also not happen, flash write has to write even number of bytes
        return 0;
    }

    // cancel _ALL_ ongoing DMA transfers:
    DMAARM = DMA_ARM_ABORT | 0x1F;

    // set length of transfer in bytes
    SET_WORD(flash_dma_config.LENH, flash_dma_config.LENL, len);
    // set src: address of data to be written
    SET_WORD(flash_dma_config.SRCADDRH,  flash_dma_config.SRCADDRL,  buf);
    // destination is flash controller data reg
    SET_WORD(flash_dma_config.DESTADDRH, flash_dma_config.DESTADDRL, &X_FWDATA);

    // set up address:
    SET_WORD(FADDRH, FADDRL, ((uint16_t)address)>>1);

    // waiting for the flash controller to be ready
    while (FCTL & FCTL_BUSY) {}

    // configure flash controller for 26mhz clock
    FWT = 0x2A;  // (21 * 26) / (16);

    // arm the DMA channel, so that a DMA trigger will initiate DMA writing
    DMAARM = DMA_ARM_CH0;
    NOP();

    // trigger flash write. this generates a DMA trigger.
    flash_trigger_write();

    // wait for dma finish
    while (!(DMAIRQ & DMAIRQ_DMAIF0)) {}

    // wait until flash controller not busy
    while (FCTL & (FCTL_BUSY | FCTL_SWBUSY)) {}

    // by now, the transfer is completed, so the transfer count is reached.
    // the DMA channel 0 interrupt flag is then set, so we clear it here.
    DMAIRQ &= ~DMAIRQ_DMAIF0;

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

static void flash_trigger_write(void) {
    // trigger flash write. this generates a DMA trigger
    // this was moved to a separate function as sdcc does not
    // optimize functions that include asm code (!)
    __asm
    .even              // IMPORTANT: PLACE THIS ON A 2BYTE BOUNDARY!
    ORL _FCTL, #0x02;  // FCTL |=  FCTL_WRITE
    NOP
    __endasm;
}

