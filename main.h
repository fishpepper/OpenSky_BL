#ifndef __MAIN_H__
#define __MAIN_H__

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))


#define BOOTLOADER_COMMAND_GET            0x00
#define BOOTLOADER_COMMAND_GET_VERSION    0x01
#define BOOTLOADER_COMMAND_GET_ID         0x02
#define BOOTLOADER_COMMAND_READ_MEMORY    0x11
#define BOOTLOADER_COMMAND_GO             0x21
#define BOOTLOADER_COMMAND_WRITE_MEMORY   0x31
#define BOOTLOADER_COMMAND_ERASE          0x43

#define BOOTLOADER_RESPONSE_ACK           0x79
#define BOOTLOADER_RESPONSE_NACK          0x1F

//version 1.0
#define BOOTLOADER_VERSION 0x10

#if ((DEVICE_FLASH_SIZE) > 65535)
    #error "ERROR: maximum supported flash size is 64k!"
#endif

#endif
