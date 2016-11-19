BOOTLOADER_SIZE = 0x0C00 #use 3kb for bootloader

FLASH_SIZE     ?= 0x4000     #default to 16k flash
FLASH_PAGESIZE ?= 0x400 #1k page size for all targets

CC = sdcc
CFLAGS     = -DBOOTLOADER_SIZE=$(BOOTLOADER_SIZE) -DFLASH_SIZE=$(FLASH_SIZE) -DFLASH_PAGESIZE=$(FLASH_PAGESIZE)
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

STYLECHECKTOOT ?= 

SRC = main.c uart.c delay.c flash.c

ADB=$(SRC:.c=.adb)
ASM=$(SRC:.c=.asm)
LNK=$(SRC:.c=.lnk)
LST=$(SRC:.c=.lst)
REL=$(SRC:.c=.rel)
RST=$(SRC:.c=.rst)
SYM=$(SRC:.c=.sym)
TARGET=opensky_cc2510_bl
PCDB=$(PROGS:.hex=.cdb)
PLNK=$(PROGS:.hex=.lnk)
PMAP=$(PROGS:.hex=.map)
PMEM=$(PROGS:.hex=.mem)
PAOM=$(PROGS:.hex=)
HEADER=$(wildcard *.h)

all: stylecheck $(TARGET).hex

stylecheck: $(HEADER) $(SRC)
	./stylecheck/cpplint.py --root=$(STYLECHECKROOT) --filter=-build/include,-build/storage_class,-readability/casting,-runtime/arrays --extensions="h,c" --linelength=100 $(HEADER) $(SRC) || true


startup.rel : startup.s
	cpp -P  $(CFLAGS) $< > $<_preprocessed
	$(AS) $(ASFLAGS) $<_preprocessed

%.rel : %.c
	$(CC) -c $(SDCC_FLAGS) -o$*.rel $<

$(TARGET).hex: startup.rel $(REL) Makefile
	$(CC) $(LDFLAGS_FLASH) $(SDCC_FLAGS) -o $(TARGET).hex  $(REL) startup.rel

$(TARGET).bin: $(TARGET).hex
	objcopy -Iihex -Obinary $(TARGET).hex $(TARGET).bin

clean:
	rm -f startup.rel
	rm -f $(ADB) $(ASM) $(LNK) $(LST) $(REL) $(RST) $(SYM)
	rm -f $(TARGET) $(PCDB) $(PLNK) $(PMAP) $(PMEM) $(PAOM)

flash: $(TARGET).hex
	$(CC_TOOL) -f -e -w $(TARGET).hex

.PHONY: stylecheck clean flash
