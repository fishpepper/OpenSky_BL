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
#include "device.h"

DMADesc_t dma_channel_0;

void bootloader_txc(void) __interrupt UTX0_VECTOR {
}

void bootloader_rxc(void) __interrupt URX0_VECTOR {
}

void bootloader_init_clocks(void) {
    //power up osc (?)
    SLEEP &= ~CLOCKSOURCE_OSC_PD_BIT;

    //wait for XOSC stable
    while(!CLOCKSOURCE_XOSC_STABLE()){}
    NOP();

    //start crystal osc as HS clocksource, OSC32 is int rc osc
    CLKCON = 0x80;

    //wait for selection to be active
    while(!CLOCKSOURCE_XOSC_STABLE()){}
    NOP();

    //power down the unused oscillator
    SLEEP |= CLOCKSOURCE_OSC_PD_BIT;

}

void bootloader_init(void) {
    //show bootloader activity:
    led_red_on();
    led_green_on();

    //set up clocks
    bootloader_init_clocks();

    //turn on cache pre-fetch mode
    FCTL = 0x08;

    //wait for vcc to stabilize
    delay_ms(50);

    //set up dma only for ch0
    DMA_SET_ADDR_DESC0(&dma_channel_0)
}


void main(void) {
    uint8_t state = 0;
    uint8_t command = 0;
    uint8_t rx = 0;
    uint8_t address[4];
    __xdata uint8_t data[256];
    uint8_t *data_ptr = 0;
    uint8_t checksum;
    uint8_t len = 0;

    led_init();

    bootloader_init();
    uart_init();

    led_green_on();
    led_red_on();

    /*P0DIR |= (1<<4);
    P0_4 = 0;
    while(1){
    P0_4 = 0;
    delay_ms(500);
    P0_4 = 1;
    delay_ms(500);
     }*/

    while(1){
        led_green_off();
        led_red_off();
        rx = uart_getc();
        led_green_on();
        delay_ms(100);
        led_green_off();
        uart_putc('X');
        delay_ms(50);
    }

    while(1) {
        //do main statemachine
        switch(state){
            default:
            case(0):
                //fetched for command byte
                command = uart_getc();
                state   = 1;
                break;

            case(1):
                //check nCMD
                rx = uart_getc();
                if (rx == ~command){
                    //fine, valid command -> decode
                    switch(command){
                        //unknown/unsupported command
                        default:
                            //invalid command, abort
                            state = 0xFF;
                            break;

                        //all known commands
                        case(BOOTLOADER_COMMAND_GET):
                        case(BOOTLOADER_COMMAND_GET_VERSION):
                        case(BOOTLOADER_COMMAND_GET_ID):
                        case(BOOTLOADER_COMMAND_READ_MEMORY):
                        case(BOOTLOADER_COMMAND_GO):
                        case(BOOTLOADER_COMMAND_WRITE_MEMORY):
                        case(BOOTLOADER_COMMAND_ERASE):
                            //send ACK and continue with command handler
                            uart_putc(BOOTLOADER_RESPONSE_ACK);
                            state = 10 + command;
                            break;
                    }
                }else{
                    //mismatch - this was either a comm error or we are
                    //in the middle of a command, retry with the current byte as cmd byte:
                    command = rx;
                }
                break;

            //send GET response
            case(10 + BOOTLOADER_COMMAND_GET):
                //number of command bytes that will follow
                uart_putc(7);
                //version
                uart_putc(BOOTLOADER_VERSION);
                //send supported commands
                uart_putc(BOOTLOADER_COMMAND_GET);
                uart_putc(BOOTLOADER_COMMAND_GET_VERSION);
                uart_putc(BOOTLOADER_COMMAND_GET_ID);
                uart_putc(BOOTLOADER_COMMAND_READ_MEMORY);
                uart_putc(BOOTLOADER_COMMAND_GO);
                uart_putc(BOOTLOADER_COMMAND_WRITE_MEMORY);
                uart_putc(BOOTLOADER_COMMAND_ERASE);
                //send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);
                //wait for next command
                state = 0;
                break;

            //send GET_ID response
            case(10 + BOOTLOADER_COMMAND_GET_ID):
                //number of response bytes to follow
                uart_putc(2);
                //send product id of an F1 chip with the same pagesize (1024)
                uart_putc(0x04);
                uart_putc(0x10);
                //send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);
                //wait for next command
                state = 0;
                break;

            //send GET_VERSION response
            case(10 + BOOTLOADER_COMMAND_GET_VERSION):
                //bootloader version
                uart_putc(BOOTLOADER_VERSION);
                //send option bytes
                uart_putc(0x00);
                uart_putc(0x00);
                //send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);
                //wait for next command
                state = 0;
                break;

            //send READ_MEMORY response
            case(10 + BOOTLOADER_COMMAND_READ_MEMORY):
                //read address
                address[3] = uart_getc();
                address[2] = uart_getc();
                address[1] = uart_getc();
                address[0] = uart_getc();
                checksum   = uart_getc();

                //verify checksum
                if (checksum != (address[3] ^ address[2] ^ address[1] ^ address[0])) {
                    //checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                //checksum test passed, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                //fetch data
                len      = uart_getc();
                checksum = uart_getc();

                //verify checksum
                if (len != (~checksum)) {
                    //checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                //checksum test passed, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                //send flash content, send N+1 bytes!
                uart_putc(len); //DUMMY DATA FOR NOW!
                while(len--) {
                    uart_putc(len); //DUMMY DATA FOR NOW!
                }

                //wait for next command
                state = 0;
                break;

            //send GO response
            case(10 + BOOTLOADER_COMMAND_GO):
                //read address
                address[3] = uart_getc();
                address[2] = uart_getc();
                address[1] = uart_getc();
                address[0] = uart_getc();
                checksum   = uart_getc();

                //verify checksum
                if (checksum != (address[3] ^ address[2] ^ address[1] ^ address[0])) {
                    //checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                //send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                //now jump to user application given by address
                //TODO: JUMP TO ADDRESS!

                //wait for next command
                state = 0;
                break;

            //send WRITE_MEMORY response
            case(10 + BOOTLOADER_COMMAND_WRITE_MEMORY):
                //read address
                address[3] = uart_getc();
                address[2] = uart_getc();
                address[1] = uart_getc();
                address[0] = uart_getc();
                checksum   = uart_getc();

                //verify checksum
                if (checksum != (address[3] ^ address[2] ^ address[1] ^ address[0])) {
                    //checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                //checksum test passed, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                //fetch len
                len      = uart_getc();
                checksum = len;

                //fetch data
                data_ptr = &data[0];

                //retrieve N+1 data bytes
                rx = uart_getc();
                checksum   ^= rx;
                *data_ptr++ = rx;

                while(len--) {
                    rx = uart_getc();
                    checksum   ^= rx;
                    *data_ptr++ = rx;
                }

                //verify checksum
                rx = uart_getc();
                if (checksum != rx) {
                    //checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                //checksum ok  - store data
                //TODO: ADD FLASH WRITE
                //MAKE SURE TO FILTER OUT BL REGION!

                //wait for next command
                state = 0;
                break;

            //send ERASE response
            case(10 + BOOTLOADER_COMMAND_ERASE):
                //get len
                len = uart_getc();
/*
                //verify checksum
                if (checksum != (address[3] ^ address[2] ^ address[1] ^ address[0])) {
                    //checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                //checksum test passed, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                //fetch len
                len      = uart_getc();
                checksum = len;

                //fetch data
                data_ptr = &data[0];

                //retrieve N+1 data bytes
                rx = uart_getc();
                checksum   ^= rx;
                *data_ptr++ = rx;

                while(len--) {
                    rx = uart_getc();
                    checksum   ^= rx;
                    *data_ptr++ = rx;
                }

                //verify checksum
                rx = uart_getc();
                if (checksum != rx) {
                    //checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                //checksum ok  - store data
                //TODO: ADD FLASH WRITE
                //MAKE SURE TO FILTER OUT BL REGION!
*/
                //wait for next command
                state = 0;
                break;

            //ABORT STATE - send nack and goto idle
            case(0xFF):
                uart_putc(BOOTLOADER_RESPONSE_NACK);
                state = 0;
                break;
        }
    }
}
