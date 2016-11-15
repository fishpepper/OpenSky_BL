BOOTLOADER_SIZE = 0x0C00 #use 3kb for bootloader

CC = sdcc
CFLAGS     = -Wl-bHOME=$(BOOTLOADER_SIZE) 
SDCC_FLAGS = --model-small --opt-code-speed -I /usr/share/sdcc/include $(CFLAGS)
LDFLAGS_FLASH = \
--out-fmt-ihx \
--code-loc 0x000 --code-size $(BOOTLOADER_SIZE) \
--xram-loc 0xf000 --xram-size 0x7FF \
--iram-size 0x100

AS = sdas8051
ASFLAGS = -plosgff

#programmer binary
CC_TOOL ?= cc-tool

ifdef DEBUG
CFLAGS += --debug
endif

SRC = main.c ../delay.c 

ADB=$(SRC:.c=.adb)
ASM=$(SRC:.c=.asm)
LNK=$(SRC:.c=.lnk)
LST=$(SRC:.c=.lst)
REL=$(SRC:.c=.rel)
RST=$(SRC:.c=.rst)
SYM=$(SRC:.c=.sym)
TARGET=blinky
PCDB=$(PROGS:.hex=.cdb)
PLNK=$(PROGS:.hex=.lnk)
PMAP=$(PROGS:.hex=.map)
PMEM=$(PROGS:.hex=.mem)
PAOM=$(PROGS:.hex=)
HEADER=$(wildcard *.h)

all: $(TARGET).bin

%.rel : %.c
	$(CC) -c $(SDCC_FLAGS) -o$*.rel $<

$(TARGET).hex: $(REL) Makefile
	$(CC) $(LDFLAGS_FLASH) $(SDCC_FLAGS) -o $(TARGET).hex  $(REL) 

$(TARGET).bin: $(TARGET).hex
	objcopy -Iihex -Obinary $(TARGET).hex $(TARGET).bin


clean:
	rm -f $(ADB) $(ASM) $(LNK) $(LST) $(REL) $(RST) $(SYM)
	rm -f $(TARGET) $(PCDB) $(PLNK) $(PMAP) $(PMEM) $(PAOM)

.PHONY: clean 
