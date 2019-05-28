
SDCC ?= sdcc-sdcc
STM8FLASH ?= stm8flash

PART ?= stm8s103f3
PROGRAMMER ?= stlinkv2

INCLUDES := -Istm8s-sdcc/inc -IExample\ project -Iws2812b
CFLAGS := --std-c11 -DSTM8S103 -DF_CPU=16000000 -lstm8 -mstm8 $(INCLUDES)
LDFLAGS := 

MAIN := proj/main.c
SOURCES := proj/stm8s_it.c\
	ws2812b/ws2812b_fx.c\
	ws2812b/ws2812b_LLD.c

OUTDIR ?= build

all: main.ihx

main.ihx: $(MAIN) $(addprefix $(OUTDIR)/,$(SOURCES:.c=.rel))
	$(SDCC) $(CFLAGS) $(LDFLAGS) $^

$(OUTDIR)/%.rel: %.c
	@mkdir -p $(dir $@)
	$(SDCC) -c $(CFLAGS) -o $(dir $@) $^

.PHONY:flash
flash: all
	$(STM8FLASH) -c$(PROGRAMMER) -p$(PART) -w $(notdir $(MAIN:.c=.ihx))

.PHONY: clean
clean:
	rm -f main.asm main.cdb main.ihx main.lk main.lst main.map main.rel main.rst main.sym

