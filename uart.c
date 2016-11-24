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

#include "uart.h"
#include "config.h"
#include "led.h"
#include "cc25xx.h"


#if (BOOTLOADER_UART_NUM == 0)
    #define UxBAUD U0BAUD
    #define UxGCR  U0GCR
    #define UxGCR_ORDER U0GCR_ORDER
    #define UxCSR  U0CSR
    #define UxCSR_TX_BYTE U0CSR_TX_BYTE
    #define UxUCR  U0UCR
    #define UxDBUF U0DBUF
    #define URXxIF URX0IF
    #define UTXxIF UTX0IF
#else
    #define UxBAUD U1BAUD
    #define UxGCR  U1GCR
    #define UxGCR_ORDER U1GCR_ORDER
    #define UxCSR  U1CSR
    #define UxCSR_TX_BYTE U1CSR_TX_BYTE
    #define UxUCR  U1UCR
    #define UxDBUF U1DBUF
    #define URXxIF URX1IF
    #define UTXxIF UTX1IF
#endif  // BOOTLOADER_UART

void uart_init(void) {
    __xdata union uart_config_t uart_config;

#if ((BOOTLOADER_UART_NUM) == 0) && ((BOOTLOADER_UART_PORT) == 0)
    // -> USART0_P0
    //    use ALT1 -> Clear flag -> P0_3 = TX / P0_2 = RX
    PERCFG &= ~(PERCFG_U0CFG);

    // configure pins as peripheral:
    P0SEL |= (1<<3) | (1<<2);

    // make sure all P1 pins switch to normal GPIO
    P1SEL &= ~(0x3C);

    // make tx pin output:
    P0DIR |= (1<<3);
#elif((BOOTLOADER_UART_NUM) == 0) && ((BOOTLOADER_UART_PORT) == 1)
    // -> USART0_P1
    //    use ALT2 -> Set flag -> P1_5 = TX / P1_4 = RX
    PERCFG |= (PERCFG_U0CFG);

    // configure pins as peripheral:
    P1SEL |= (1<<5) | (1<<4);

    // make sure all P0 pins switch to normal GPIO
    P0SEL &= ~(0x3C);

    // make tx pin output:
    P1DIR |= (1<<5);
#elif((BOOTLOADER_UART_NUM) == 1) && ((BOOTLOADER_UART_PORT) == 0)
    // -> USART1_P0
    //    use ALT1 -> Clear flag -> P0_4 = TX / P0_5 = RX
    PERCFG &= ~(PERCFG_U1CFG);
    // USART1 has priority when USART0 is also enabled
    P2DIR = (P2DIR & 0x3F) | 0b01000000;

    // configure pins as peripheral:
    P0SEL |= (1<<4) | (1<<5);

    // make sure all P1 pins switch to normal GPIO
    P1SEL &= ~(0xF0);

    // make tx pin output:
    P0DIR |= (1<<4);
#elif((BOOTLOADER_UART_NUM) == 1) && ((BOOTLOADER_UART_PORT) == 1)
    // -> USART1_P1
    //    use ALT2 -> set flag -> P1_6 = TX / P1_7 = RX
    PERCFG  |= (PERCFG_U1CFG);
    // USART1 has priority when USART0 is also enabled
    P2DIR = (P2DIR & 0x3F) | 0b01000000;

    // configure pins as peripheral:
    P1SEL |= (1<<6) | (1<<7);

    // make sure all P0 pins switch to normal GPIO
    P0SEL &= ~(0x3C);

    // make tx pin output:
    P1DIR |= (1<<6);
#else
  #error "ERROR: UNSUPPORTED DEBUG UART"
#endif  // BOOTLOADER_UART_*

    // this assumes cpu runs from XOSC (26mhz) !
    // set baudrate
    UxBAUD = UART_BAUD_M;
    UxGCR = (UxGCR & ~0x1F) | (UART_BAUD_E);

    // set up config
#if BOOTLOADER_UART_INVERTED
    //this is a really nice feature of the cc2510:
    //we can invert the idle level of the usart
    //by setting STOP to zero. by inverting
    //the parity, the startbit, and the data
    //we can effectively invert the usart in software :)
    uart_config.bit.START  = 1; //startbit level = low
    uart_config.bit.STOP   = 0; //stopbit level = high
    uart_config.bit.D9     = 1; //UNEven parity
#else
    //standard usart, non-inverted mode
    uart_config.bit.START  = 0; //startbit level = low
    uart_config.bit.STOP   = 1; //stopbit level = high
    uart_config.bit.D9     = 0; //Even parity
#endif

#if BOOTLOADER_UART_USE_2STOPBITS
    uart_config.bit.SPB    = 1;  // 2 stopbit
#else
    uart_config.bit.SPB    = 0;  // 1 stopbit
#endif

#if BOOTLOADER_UART_USE_PARITY
    // use parity
    uart_config.bit.PARITY = 1;  // 1 = parity enabled, D9=0 -> even parity
    uart_config.bit.BIT9   = 1;  // 8+1 parity bit
#else
    // no parity
    uart_config.bit.PARITY = 0;  // no parity
    uart_config.bit.BIT9   = 0;  // 8+0 parity bit
#endif
    uart_config.bit.FLOW   = 0;  // no hw flow control
    uart_config.bit.ORDER  = 0;  // lsb first

    // enable uart mode
    UxCSR |= 0x80;

    // store config to UxUCR register
    UxUCR = uart_config.byte & (0x7F);

    // store config to UxGCR: (msb/lsb)
    if (uart_config.bit.ORDER) {
        UxGCR |= UxGCR_ORDER;
    } else {
        UxGCR &= ~UxGCR_ORDER;
    }

    // enable receiver
    UxCSR |= 0x40;
}

uint8_t uart_getc(void) {
    uint8_t res = 0;

    led_green_toggle();

    // wait for RX
    while (!URXxIF) {}

    // fetch data
#if BOOTLOADER_UART_INVERTED
    res = 0xFF ^ UxDBUF;
#else
    res = UxDBUF;
#endif

    // clear flag
    URXxIF = 0;

    return res;
}

void uart_putc(uint8_t c) {
    UTXxIF = 0;

#if BOOTLOADER_UART_INVERTED
    UxDBUF = 0xFF ^ c;
#else
    UxDBUF = c;
#endif

    while (!UTXxIF) {}
    UTXxIF = 0;
}

