;main application will be moved to the following location:
APP_OFFSET=BOOTLOADER_SIZE

;define custom interrupt vector:
    .module ivect
    .area IVECT (CODE)
    ljmp __sdcc_gsinit_startup

    ;0x0003: RF TX done / RX ready
    ljmp 0x0003 + APP_OFFSET
    .ds     5

    ;0x000B: ADC end of conversion
    ljmp 0x000B + APP_OFFSET
    .ds     5

    ;0x0013: USART0 RX complete
    ljmp 0x0013 + APP_OFFSET
    ;ljmp _bootloader_rxc
    .ds     5

    ;0x001B: USART1 RX complete
    ljmp 0x001B + APP_OFFSET
    .ds     5

    ;0x0023: AES enc/dec complete
    ljmp 0x0023 + APP_OFFSET
    .ds     5

    ;0x002B: Sleep Timer compare
    ljmp 0x002B + APP_OFFSET
    .ds     5

    ;0x0033; Port 2 inputs
    ljmp 0x0033 + APP_OFFSET
    .ds     5

    ;0x003B: USART0 TXC
    ljmp 0x003B + APP_OFFSET
    ;ljmp _bootloader_txc
    .ds     5

    ;0x0043: DMA transfer complete
    ljmp 0x0043 + APP_OFFSET
    .ds     5

    ;0x004B: Timer 1 (16-bit) capture/Compare/overflow
    ljmp 0x004B + APP_OFFSET
    .ds     5

    ;0x0053: Timer 2 (MAC Timer) overflow
    ljmp 0x0053 + APP_OFFSET
    .ds     7

    ;0x005B: Timer 3 (8-bit) capture/compare/overflow
    ljmp 0x005B + APP_OFFSET
    .ds     5

    ;0x0063: Timer 4 (8-bit) capture/compare/overflow
    ljmp 0x0063 + APP_OFFSET
    .ds     5

    ;0x006B: Port 0 inputs
    ljmp 0x006B + APP_OFFSET
    .ds     5

    ;0x0073: USART1 TXC
    ljmp 0x0073 + APP_OFFSET
    .ds     5

    ;0x007B: Port 1 inputs
    ljmp 0x007B + APP_OFFSET
    .ds     5

    ;0x0083: RF general interrupts
    ljmp 0x0083 + APP_OFFSET
    .ds     5

    ;0x008B: Watchdog overflow in timer mode
    ljmp 0x008B + APP_OFFSET

