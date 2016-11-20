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

#ifndef LED_H_
#define LED_H_
#include <cc2510fx.h>
#include "portmacros.h"
#include "config.h"

#define BOOTLOADER_LED_GREEN_DIR PORT2DIR(BOOTLOADER_LED_GREEN_PORT)
#define BOOTLOADER_LED_RED_DIR   PORT2DIR(BOOTLOADER_LED_RED_PORT)
#define BOOTLOADER_LED_RED_BIT   PORT2BIT(BOOTLOADER_LED_RED_PORT, BOOTLOADER_LED_RED_PIN)
#define BOOTLOADER_LED_GREEN_BIT PORT2BIT(BOOTLOADER_LED_GREEN_PORT, BOOTLOADER_LED_GREEN_PIN)

#define led_green_init() { BOOTLOADER_LED_GREEN_DIR |= (1 << BOOTLOADER_LED_GREEN_PIN); \
led_green_off(); }
#define led_green_on()  { BOOTLOADER_LED_GREEN_BIT = 1; }
#define led_green_off() { BOOTLOADER_LED_GREEN_BIT = 0; }
#define led_green_toggle()  { BOOTLOADER_LED_GREEN_BIT = !BOOTLOADER_LED_GREEN_BIT; }

#define led_red_init()  { BOOTLOADER_LED_RED_DIR |= (1 << BOOTLOADER_LED_RED_PIN); led_red_off(); }
#define led_red_on()    { BOOTLOADER_LED_RED_BIT = 1; }
#define led_red_off()   { BOOTLOADER_LED_RED_BIT = 0; }
#define led_red_toggle()   { BOOTLOADER_LED_RED_BIT = !BOOTLOADER_LED_RED_BIT; }

// led init routines
#define led_init() { led_red_init(); led_green_init(); }

#endif  // LED_H_
