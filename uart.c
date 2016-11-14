/*
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
#include "cc25xx.h"


#if (BOOTLOADER_UART == USART0_P0) || (BOOTLOADER_UART == USART0_P1)
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
#endif

//http://www.ti.com/lit/an/swra222b/swra222b.pdf

void uart_init(void) {
    __xdata union uart_config_t uart_config;

#if BOOTLOADER_UART == USART0_P0
    //USART0 use ALT1 -> Clear flag -> P0_3 = TX / P0_2 = RX
    PERCFG &= ~(PERCFG_U0CFG);

    //configure pins as peripheral:
    P0SEL |= (1<<3) | (1<<2);

    //make sure all P1 pins switch to normal GPIO
    P1SEL &= ~(0x3C);

    //make tx pin output:
    P0DIR |= (1<<3);
#elif BOOTLOADER_UART == USART0_P1
    //USART0 use ALT2 -> Set flag -> P1_5 = TX / P1_4 = RX
    PERCFG |= (PERCFG_U0CFG);

    //configure pins as peripheral:
    P1SEL |= (1<<5) | (1<<4);

    //make sure all P0 pins switch to normal GPIO
    P0SEL &= ~(0x3C);

    //make tx pin output:
    P1DIR |= (1<<5);
#elif BOOTLOADER_UART == USART1_P0
    //USART1 use ALT1 -> Clear flag -> P0_4 = TX / P0_5 = RX
    PERCFG &= ~(PERCFG_U1CFG);

    //configure pins as peripheral:
    P0SEL |= (1<<4) | (1<<5);

    //make sure all P1 pins switch to normal GPIO
    P1SEL &= ~(0xF0);

    //make tx pin output:
    P0DIR |= (1<<4);
#elif BOOTLOADER_UART == USART1_P1
    //USART1 use ALT2 -> set flag -> P1_6 = TX / P1_7 = RX
    PERCFG  |= (PERCFG_U1CFG);

    //configure pins as peripheral:
    P1SEL |= (1<<6) | (1<<7);

    //make sure all P0 pins switch to normal GPIO
    P0SEL &= ~(0x3C);

    //make tx pin output:
    P1DIR |= (1<<6);
#else
  #error "ERROR: UNSUPPORTED DEBUG UART"
#endif

    //this assumes cpu runs from XOSC (26mhz) !
    //set baudrate
    UxBAUD = UART_BAUD_M;
    UxGCR = (UxGCR & ~0x1F) | (UART_BAUD_E);

    //set up config
    uart_config.bit.START  = 0; //startbit level = low
    uart_config.bit.STOP   = 1; //stopbit level = high
    uart_config.bit.SPB    = 0; //1 stopbit
    uart_config.bit.PARITY = 0; //1 = parity enabled, D9=0 -> even parity
    uart_config.bit.BIT9   = 0; //8bit
    uart_config.bit.D9     = 0; //EVEN parity
    uart_config.bit.FLOW   = 0; //no hw flow control
    uart_config.bit.ORDER  = 0; //lsb first

    //enable uart mode
    UxCSR |= 0x80;

    //store config to UxUCR register
    UxUCR = uart_config.byte & (0x7F);

    //store config to UxGCR: (msb/lsb)
    if (uart_config.bit.ORDER){
        UxGCR |= UxGCR_ORDER;
    }else{
        UxGCR &= ~UxGCR_ORDER;
    }

    //interrupt prio to 01 (0..3=highest)
    /*IP0 |=  (1<<2);
    IP1 &= ~(1<<2);*/

    //enable receiver
    UxCSR |= 0x40;
}

uint8_t uart_getc(void) {
    uint8_t res = 0;


    //UxCSR |= 0x40;

    //wait for RX
    while(!URXxIF) {}

    //fetch data
    res = UxDBUF;

    //clear flag
    URXxIF = 0;

    return res;
}

void uart_putc(uint8_t c) {
    UTXxIF = 0;
    UxDBUF = c;
    while (!UTXxIF) {}
    UTXxIF = 0;
}

/*
void hal_uart_start_transmission(uint8_t ch){
    //clear flags
    UTXxIF = 0;
    UxCSR &= ~UxCSR_TX_BYTE;

    //enable TX int:
    IEN2 |= (IEN2_UTX0IE);

    //send this char
    UxDBUF = ch;
}

static void hal_uart_set_mode(__xdata union hal_uart_config_t *cfg){
    //enable uart mode
    UxCSR |= 0x80;

    //store config to UxUCR register
    UxUCR = cfg->byte & (0x7F);

    //store config to UxGCR: (msb/lsb)
    if (cfg->bit.ORDER){
        UxGCR |= UxGCR_ORDER;
    }else{
        UxGCR &= ~UxGCR_ORDER;
    }

    //interrupt prio to 01 (0..3=highest)
    IP0 |=  (1<<2);
    IP1 &= ~(1<<2);
}


*/
