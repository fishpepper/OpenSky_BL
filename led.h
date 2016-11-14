#ifndef __LED_H__
#define __LED_H__
#include "config.h"
#include <cc2510fx.h>

//LEDS
#define LED_GREEN_DIR PORT2DIR(LED_GREEN_PORT)
#define LED_RED_DIR   PORT2DIR(LED_RED_PORT)
#define LED_RED_BIT PORT2BIT(LED_RED_PORT, LED_RED_PIN)
#define LED_GREEN_BIT PORT2BIT(LED_GREEN_PORT, LED_GREEN_PIN)

#define led_green_init() { LED_GREEN_DIR |= (1<<LED_GREEN_PIN); led_green_off(); }
#define led_green_on()  { LED_GREEN_BIT = 1; }
#define led_green_off() { LED_GREEN_BIT = 0; }
#define led_green_toggle()  { LED_GREEN_BIT = !LED_GREEN_BIT; }

#define led_red_init()  { LED_RED_DIR |= (1<<LED_RED_PIN); led_red_off(); }
#define led_red_on()    { LED_RED_BIT = 1; }
#define led_red_off()   { LED_RED_BIT = 0; }
#define led_red_toggle()   { LED_RED_BIT = !LED_RED_BIT; }

//led init routines
#define led_init() { led_red_init(); led_green_init(); }

#endif


