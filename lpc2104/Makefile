export LPC2XXX_MODEL := lpc2104
export F_CPU         := 60000000L
export HSE_Val       := 12000000L

include ./lib/LPC2xxx.mk

PHONY += all clean

all: hello.bin

hello.elf: libLPC2xxx.a reloc.o

libLPC2xxx.a:
	$(MAKE) -C $(LPC2XXXLIB_DIR) $@ O="$(PWD)"

clean:
	rm -f *.o *.a *.elf *.bin

