BOOTLOADER_SIZE = 0x0C00 #use 3 pages for bootloader (1 page = 0x400)

FLASH_SIZE     ?= 0x4000     #default to 16k flash
FLASH_PAGESIZE ?= 0x400 #1k page size for all targets

CC = sdcc

#set some flags depending on the target
CFLAGS     = -DBOOTLOADER_SIZE=$(BOOTLOADER_SIZE)
CFLAGS    += -DFLASH_SIZE=$(FLASH_SIZE)
CFLAGS    += -DFLASH_PAGESIZE=$(FLASH_PAGESIZE)

#allow a custom config file to be passed
CONFIG_INCLUDE_DIR ?= default
CFLAGS += -I $(CONFIG_INCLUDE_DIR)

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

SRC = main.c uart.c delay.c flash.c io.c

ADB=$(SRC:.c=.adb)
ASM=$(SRC:.c=.asm)
LNK=$(SRC:.c=.lnk)
LST=$(SRC:.c=.lst)
REL=$(SRC:.c=.rel)
RST=$(SRC:.c=.rst)
SYM=$(SRC:.c=.sym)
BL_TARGET=bootloader

PCDB=$(PROGS:.hex=.cdb)
PLNK=$(PROGS:.hex=.lnk)
PMAP=$(PROGS:.hex=.map)
PMEM=$(PROGS:.hex=.mem)
PAOM=$(PROGS:.hex=)
HEADER=$(wildcard *.h)

all: stylecheck $(BL_TARGET).hex

stylecheck: $(HEADER) $(SRC)
	./stylecheck/cpplint.py --root=$(STYLECHECKROOT) \
		--filter=-build/include,-build/storage_class,-readability/casting,-runtime/arrays \
		--extensions="h,c" --linelength=100 $(HEADER) $(SRC) || true

startup.rel : startup.s
	@cpp -P  $(CFLAGS) $< > $<_preprocessed
	$(AS) $(ASFLAGS) $<_preprocessed

%.rel : %.c
	$(CC) -c $(SDCC_FLAGS) -o$*.rel $<

$(BL_TARGET).hex: startup.rel $(REL) Makefile
	$(CC) $(LDFLAGS_FLASH) $(SDCC_FLAGS) -o $(BL_TARGET).hex  $(REL) startup.rel

$(BL_TARGET).bin: $(BL_TARGET).hex
	@objcopy -Iihex -Obinary $(BL_TARGET).hex $(BL_TARGET).bin

clean:
	@rm -f startup.rel
	@rm -f $(ADB) $(ASM) $(LNK) $(LST) $(REL) $(RST) $(SYM)
	@rm -f $(BL_TARGET) $(PCDB) $(PLNK) $(PMAP) $(PMEM) $(PAOM)
	@rm -f *_preprocessed

flash: $(BL_TARGET).hex
	$(CC_TOOL) -f -e -w $(BL_TARGET).hex

.PHONY: stylecheck clean flash
