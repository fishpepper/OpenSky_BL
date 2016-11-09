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

#pragma noiv

#include <cc2510fx.h>
#include <stdint.h>
#include "main.h"
#include "uart.h"
#include "led.h"
#include "delay.h"
#include "dma.h"

DMADesc_t dma_channel_0;

void bootloader_txc(void) __interrupt UTX0_VECTOR {
}

void bootloader_rxc(void) __interrupt URX0_VECTOR {
}

void bootloader_init_clocks(void) {
    //turn on both HS oscs (HS XOSC + HS RCOSC)
    SLEEP &= ~0x04;
    //wait for completion
    while(!(SLEEP & 0x40)) {}

    //select HS XOSC
    CLKCON &= 0xBF;
    //wait for selection to be active
    while(CLKCON & 0x40) {}

    //turn of HS osc that was not selected as sys clock (here RCOSC)
    SLEEP |= 0x04;
}

void bootloader_init(void) {
    //show bootloader activity:
    LED_RED_ON();
    LED_GREEN_ON();

    //set up clocks
    bootloader_init_clocks();

    //turn on cache pre-fetch mode
    FCTL = 0x08;

    //wait for vcc to stabilize
    delay_ms(50);

    //set up dma only for ch0
    DMA_SET_ADDR_DESC0(&dma_channel_0)

    //set up uart
    uart_init();
}

void main(void) {
    led_init();

    bootloader_init();

    while(1) {
    }
}
