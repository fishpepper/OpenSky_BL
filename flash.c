#include "flash.h"
#include "config.h"
#include <cc2510fx.h>

//DMADesc_t dma_channel_0;

void flash_init(void) {
    //set up dma only for ch0
    //DMA_SET_ADDR_DESC0(&dma_channel_0)
}

// NOTE: this will read len+1 bytes to buffer buf
void flash_read(uint16_t address, __xdata uint8_t *buf, uint8_t len) {
    __xdata uint8_t * flash_ptr = (__xdata uint8_t *)address;

    //copy len+1 bytes to buf
    *buf++ = *flash_ptr++;
    while(len--) {
        *buf++ = *flash_ptr++;
    }
}

uint8_t flash_write_data(uint16_t address, uint8_t *buf, uint8_t len) {
    //make sure not to overwrite bootloader:
    if (address < BOOTLOADER_SIZE) {
        return 0;
    }

    //make sure not to overwrite beyound valid flash region:
    if ((address + len + 1) >= DEVICE_FLASH_SIZE) {
        return 0;
    }

    //TODO: add write to flash code

    return 1;
}

uint8_t flash_erase_page(uint8_t page) {
    if (page >= ((DEVICE_FLASH_SIZE) / (DEVICE_FLASH_PAGESIZE))) {
        //maximum page exceeded, abort
        return 0;
    }

    //deny deletion of bootloader
    if (page <= ((BOOTLOADER_SIZE) / (DEVICE_FLASH_PAGESIZE))) {
        return 0;
    }

    //TODO: add erase page code

    return 1;
}

