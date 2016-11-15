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
#pragma noiv

#include <stdint.h>
#include "cc25xx.h"
#include "main.h"
#include "uart.h"
#include "led.h"
#include "delay.h"
#include "device.h"
#include "flash.h"

void bootloader_init_clocks(void) {
    // power up osc (?)
    SLEEP &= ~CLOCKSOURCE_OSC_PD_BIT;

    // wait for XOSC stable
    while (!CLOCKSOURCE_XOSC_STABLE()) {}
    NOP();

    // start crystal osc as HS clocksource, OSC32 is int rc osc
    CLKCON = 0x80;

    // wait for selection to be active
    while (!CLOCKSOURCE_XOSC_STABLE()) {}
    NOP();

    // power down the unused oscillator
    SLEEP |= CLOCKSOURCE_OSC_PD_BIT;
}

void bootloader_init(void) {
    // show bootloader activity:
    led_red_on();
    led_green_on();

    // set up clocks
    bootloader_init_clocks();

    // turn on cache pre-fetch mode
    FCTL = 0x08;

    // wait for vcc to stabilize
    delay_ms(50);
}


uint8_t bootloader_decode_address(uint16_t *address) {
    uint8_t rx;
    uint8_t checksum;

    // as we only accept 16bit addresses
    // stm32 uses 0x0800xxyy as address for flash
    // ignore the first two bytes
    if (uart_getc() != 0x08) { return 0; }
    if (uart_getc() != 0x00) { return 0; }

    // prepare checksum
    checksum = 0x08;

    // high byte of 16bit
    rx        = uart_getc();
    *address  = rx;
    checksum ^= rx;

    // low byte of 16bit
    rx        = uart_getc();
    *address  = ((*address) << 8) | rx;
    checksum ^= rx;

    // read address checksum
    rx = uart_getc();

    // verify checksum
    if (rx != checksum) {
        // checksum invalid -> abort here
        return 0;
    }

    // verify if this is within memory bounds:
    if ((*address) > (DEVICE_FLASH_SIZE)) {
        return 0;
    }

    // everything is fine
    return 1;
}


void main(void) {
    uint8_t state = 0;
    uint8_t command = 0;
    uint8_t rx = 0;
    uint16_t address;
    __xdata uint8_t buf[256];
    uint8_t *data_ptr = 0;
    uint8_t checksum;
    uint8_t len = 0;
    uint8_t i;

    led_init();

    bootloader_init();
    uart_init();
    flash_init();

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
/*
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
*/

    while (1) {
        // uart_putc_d(state);
        // do main statemachine
        switch (state) {
            default:
            case(0):
                // fetch command byte
                command = uart_getc();
                if (command == BOOTLOADER_COMMAND_INIT) {
                    // init sequence, send ack
                    uart_putc(BOOTLOADER_RESPONSE_ACK);
                } else {
                    // real command
                    state   = 1;
                }
                break;

            case(1):
                // check command checksum (inverted)
                rx = uart_getc();
                // NOTE: ~x seems to be calculated in uint16_t !
                if (rx == (command ^ 0xFF)) {
                    // fine, valid command -> decode
                    switch (command) {
                        // unknown or unsupported command
                        default:
                            // invalid command, abort
                            state = 0xFF;
                            break;

                        // all known commands
                        case(BOOTLOADER_COMMAND_GET):
                        case(BOOTLOADER_COMMAND_GET_VERSION):
                        case(BOOTLOADER_COMMAND_GET_ID):
                        case(BOOTLOADER_COMMAND_READ_MEMORY):
                        case(BOOTLOADER_COMMAND_GO):
                        case(BOOTLOADER_COMMAND_WRITE_MEMORY):
                        case(BOOTLOADER_COMMAND_ERASE):
                            // send ACK and continue with command handler
                            uart_putc(BOOTLOADER_RESPONSE_ACK);
                            state = 10 + command;
                            break;
                    }
                } else {
                    // mismatch - this was either a comm error or we are
                    // in the middle of a command, retry with the current byte as cmd byte:
                    if (rx == BOOTLOADER_COMMAND_INIT) {
                        // init sequence, send ack
                        uart_putc(BOOTLOADER_RESPONSE_ACK);
                        state   = 0;
                    } else {
                        // real command
                        command = rx;
                    }
                }
                break;

            // send GET response
            case(10 + BOOTLOADER_COMMAND_GET):
                // number of command bytes that will follow
                uart_putc(7);
                // version
                uart_putc(BOOTLOADER_VERSION);
                // send supported commands
                uart_putc(BOOTLOADER_COMMAND_GET);
                uart_putc(BOOTLOADER_COMMAND_GET_VERSION);
                uart_putc(BOOTLOADER_COMMAND_GET_ID);
                uart_putc(BOOTLOADER_COMMAND_READ_MEMORY);
                uart_putc(BOOTLOADER_COMMAND_GO);
                uart_putc(BOOTLOADER_COMMAND_WRITE_MEMORY);
                uart_putc(BOOTLOADER_COMMAND_ERASE);
                // send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);
                // wait for next command
                state = 0;
                break;

            // send GET_ID response
            case(10 + BOOTLOADER_COMMAND_GET_ID):
                // number of response bytes to follow
                uart_putc(1);
                // send product id of an F1 chip with the same pagesize (1024)
                uart_putc(0x04);
                uart_putc(0x12);
                // send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);
                // wait for next command
                state = 0;
                break;

            // send GET_VERSION response
            case (10 + BOOTLOADER_COMMAND_GET_VERSION):
                // bootloader version
                uart_putc(BOOTLOADER_VERSION);
                // send option bytes
                uart_putc(0x00);
                uart_putc(0x00);
                // send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);
                // wait for next command
                state = 0;
                break;

            // send READ_MEMORY response
            case(10 + BOOTLOADER_COMMAND_READ_MEMORY):
                if (!bootloader_decode_address(&address)) {
                    // abort now
                    state = 0xFF;
                    break;
                }

                // addresss is valid, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                // fetch data
                len      = uart_getc();
                checksum = uart_getc();

                // verify checksum
                if (len != (checksum ^ 0xFF)) {
                    // checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                // checksum test passed, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                // send flash content, send N+1 bytes!
                flash_read(address, buf, len);

                // send len+1 bytes
                data_ptr = &buf[0];
                uart_putc(*data_ptr++);
                while (len--) {
                    uart_putc(*data_ptr++);
                }

                // wait for next command
                state = 0;
                break;

            // send GO response
            case(10 + BOOTLOADER_COMMAND_GO):
                if (!bootloader_decode_address(&address)) {
                    // abort now
                    state = 0xFF;
                    break;
                }

                // addresss is valid, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                // now jump to user application given by address
                // NOTE: once we use ISRs we need to unconfigure the interrupt bits here!
                __asm
                ljmp BOOTLOADER_SIZE
                __endasm;

                // wait for next command
                state = 0;
                break;

            // send WRITE_MEMORY response
            case(10 + BOOTLOADER_COMMAND_WRITE_MEMORY):
                if (!bootloader_decode_address(&address)) {
                    // abort now
                    state = 0xFF;
                    break;
                }

                // addresss is valid, send ack
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                // fetch len
                len      = uart_getc();
                checksum = len;

                // fetch data
                data_ptr = &buf[0];

                // retrieve N+1 data bytes
                rx          = uart_getc();
                *data_ptr++ = rx;
                checksum   ^= rx;

                while (len--) {
                    rx          = uart_getc();
                    *data_ptr++ = rx;
                    checksum   ^= rx;
                }

                // verify checksum
                rx = uart_getc();
                if (checksum != rx) {
                    // checksum invalid -> abort here
                    state = 0xFF;
                    break;
                }

                // checksum ok  - store data
                if (!flash_write_data(address, buf, len)) {
                    // write failed
                    state = 0xFF;
                    break;
                }

                // done
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                // wait for next command
                state = 0;
                break;

            // send ERASE response
            case(10 + BOOTLOADER_COMMAND_ERASE):
                // get number of pages to be erased
                len      = uart_getc();
                checksum = len;

                if (len == 0xFF) {
                    // special case, full flash erase
                    if (uart_getc() == 0x00) {
                        // valid command, mark all pages to be erased
                        len = 0;
                        data_ptr = &buf[0];
                        for (i = PAGECOUNT_BOOTLOADER; i < PAGECOUNT_FLASH; i++) {
                            buf[len] = i;
                            len++;
                        }
                    } else {
                        // checksum error, abort
                        state = 0xFF;
                        break;
                    }
                } else {
                    // fetch len+1 pages to be erased
                    data_ptr = &buf[0];
                    rx          = uart_getc();
                    *data_ptr++ = rx;
                    checksum   ^= rx;

                    for (i = 0; i < len; i++) {
                        rx          = uart_getc();
                        *data_ptr++ = rx;
                        checksum   ^= rx;
                    }

                    // fetch checksum
                    rx = uart_getc();

                    if (rx != checksum) {
                        // checksum mismatch, abort
                        state = 0xFF;
                        break;
                    }
                }

                // fine, the len+1 pages to be erased are now in buf[]
                // execute the erase of len+1 pages
                data_ptr = &buf[0];
                if (!flash_erase_page(*data_ptr++)) { state = 0xFF; break;}
                while (len--) {
                    if (!flash_erase_page(*data_ptr++)) { state = 0xFF; break;}
                }

                // execute suceeded!
                uart_putc(BOOTLOADER_RESPONSE_ACK);

                // wait for next command
                state = 0;
                break;

            // ABORT STATE - send nack and goto idle
            case(0xFF):
                uart_putc(BOOTLOADER_RESPONSE_NACK);
                state = 0;
                break;
        }
    }
}
