#include "portmacros.h"
#include "uart.h"

#define LED_GREEN_PORT P2
#define LED_GREEN_PIN  4
#define LED_RED_PORT P2
#define LED_RED_PIN  3

#define DEVICE_FLASH_SIZE       32*1024L
#define DEVICE_FLASH_PAGESIZE   1024L

#define BOOTLOADER_UART USART1_P0
