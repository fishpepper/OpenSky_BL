# OpenSky_BL

Bootloader code for the cc2510 to be used by the [OpenSky project](https://github.com/fishpepper/OpenSky).
The bootloader protocol used is the same as used for stm32f0/1/3, see [application note](www.st.com/resource/en/application_note/cd00264342.pdf).

The idea is to have a bootloader that can be used from within betaflight without the need for any further software. As betaflight supports the stm32 bootloader protocol, i choosed to implement it for the cc2510.
This allows to update/flash the receiver firmware without the need of a cc-tool isp programmer. 

#Status
Bootloader upload/erase/go is working. Tested with betaflight configurator and stm32flash.
Note that Opensky needs to be compiled to with --code-loc 0x0c00 in order to be stored behind the bootloader. The bootloader code redirects all interrupts to the interrupt vector located at 0x0C00.

#TODO
Use a gpio hi/lo signal to decide wether to enter the bootloader or to jump to the app.

misc docs:
www.st.com/resource/en/application_note/cd00264342.pdf
http://www.ti.com/lit/an/swra222b/swra222b.pdf
http://www.ti.com/lit/gpn/cc2510
https://github.com/betaflight/betaflight-configurator/blob/master/js/protocols/stm32.js
https://sourceforge.net/p/stm32flash/code/ci/master/tree/stm32.c
http://www.ti.com/general/docs/litabsmultiplefilelist.tsp?literatureNumber=swra371a
