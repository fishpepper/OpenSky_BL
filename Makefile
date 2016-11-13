CC = sdcc
CFLAGS = --model-small --opt-code-speed -I /usr/share/sdcc/include -Wl-bIVECT=0 
LDFLAGS_FLASH = \
--out-fmt-ihx \
--code-loc 0x000 --code-size 0x2000 \
--xram-loc 0xf000 --xram-size 0x7FF \
--iram-size 0x100

AS = sdas8051
ASFLAGS = -plosgff


#programmer binary
CC_TOOL ?= cc-tool

ifdef DEBUG
CFLAGS += --debug
endif

SRC = main.c uart.c delay.c

ADB=$(SRC:.c=.adb)
ASM=$(SRC:.c=.asm)
LNK=$(SRC:.c=.lnk)
LST=$(SRC:.c=.lst)
REL=$(SRC:.c=.rel)
RST=$(SRC:.c=.rst)
SYM=$(SRC:.c=.sym)
TARGET=opensky_cc2510_bl.hex
PCDB=$(PROGS:.hex=.cdb)
PLNK=$(PROGS:.hex=.lnk)
PMAP=$(PROGS:.hex=.map)
PMEM=$(PROGS:.hex=.mem)
PAOM=$(PROGS:.hex=)

all: $(TARGET)

ivect.rel : ivect.asm
	$(AS) $(ASFLAGS) $<

%.rel : %.c
	$(CC) -c $(CFLAGS) -o$*.rel $<

$(TARGET): ivect.rel $(REL) Makefile
	$(CC) $(LDFLAGS_FLASH) $(CFLAGS) -o $(TARGET)  $(REL) ivect.rel
clean:
	rm -f ivect.rel
	rm -f $(ADB) $(ASM) $(LNK) $(LST) $(REL) $(RST) $(SYM)
	rm -f $(TARGET) $(PCDB) $(PLNK) $(PMAP) $(PMEM) $(PAOM)

flash: $(TARGET)
	$(CC_TOOL) -f -e -w $(TARGET)
                                              
