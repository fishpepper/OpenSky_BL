#ifndef __FLASH_H__
#define __FLASH_H__
#include <stdint.h>

void flash_init(void);
void flash_read(uint16_t address, __xdata uint8_t *buf, uint8_t len);
uint8_t flash_write_data(uint16_t address, uint8_t *buf, uint8_t len);
uint8_t flash_erase_page(uint8_t page);

#endif
