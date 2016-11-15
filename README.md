# OpenSky_BL

Bootloader code for the cc2510 used for for the OpenSky project.
The bootloader protocol used is the same as used for stm32f0/1/3, see [application note](www.st.com/resource/en/application_note/cd00264342.pdf).

tested:
stm32flash /dev/ttyUSB0 -w blink_test/blinky.bin -S 0x08000C00 -v
stm32flash /dev/ttyUSB0 -g 0x08000C00

WARNING: do not call stm32flash to write a hex file! this will allocate a lot of memory and crash your pc!


misc docs:
www.st.com/resource/en/application_note/cd00264342.pdf
http://www.ti.com/lit/an/swra222b/swra222b.pdf
http://www.ti.com/lit/gpn/cc2510
https://github.com/betaflight/betaflight-configurator/blob/master/js/protocols/stm32.js
https://sourceforge.net/p/stm32flash/code/ci/master/tree/stm32.c
http://www.ti.com/general/docs/litabsmultiplefilelist.tsp?literatureNumber=swra371a
