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
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "config.h"

// for a 26MHz Crystal:
#define CC2510_BAUD_M_115200 34
#define CC2510_BAUD_E_115200 12
#define CC2510_BAUD_M_57600  34
#define CC2510_BAUD_E_57600  11
//best match for 100kbit/s = 99975.5859375 bit/s
//baudrate = (((256.0 + baud_m)*2.0**baud_e) / (2**28)) * 26000000.0
#define CC2510_BAUD_E_100000 11
#define CC2510_BAUD_M_100000 248


// set baudrate:
#if (BOOTLOADER_UART_BAUDRATE == 57600)
    #define UART_BAUD_M CC2510_BAUD_M_57600
    #define UART_BAUD_E CC2510_BAUD_E_57600
#elif (BOOTLOADER_UART_BAUDRATE == 115200)
    #define UART_BAUD_M CC2510_BAUD_M_115200
    #define UART_BAUD_E CC2510_BAUD_E_115200
#elif (BOOTLOADER_UART_BAUDRATE == 100000)
    #define UART_BAUD_M CC2510_BAUD_M_100000
    #define UART_BAUD_E CC2510_BAUD_E_100000
#else
    #error INVALID BAUDRATE SELECTED! BOOTLOADER_UART_BAUDRATE
#endif


void uart_init(void);

union uart_config_t{
  uint8_t byte;
  struct {
        uint8_t START : 1;  // start bit level
        uint8_t STOP  : 1;  // stop bit level
        uint8_t SPB   : 1;  // stop bits (0=1, 1=2)
        uint8_t PARITY: 1;  // parity (on/off)
        uint8_t BIT9  : 1;  // 9 bit mode
        uint8_t D9    : 1;  // 9th bit level or parity type
        uint8_t FLOW  : 1;  // flow control
        uint8_t ORDER : 1;  // data bit order (LSB or MSB first)
  } bit;
};

uint8_t uart_getc(void);
void uart_putc(uint8_t c);


#endif  // UART_H_
