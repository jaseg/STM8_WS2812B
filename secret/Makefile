
SDCC ?= sdcc-sdcc
STM8FLASH ?= stm8flash

PART ?= stm8s103f3
PROGRAMMER ?= stlinkv2

INCLUDES := -Istm8s-sdcc/inc
CFLAGS := --std-c11 -DSTM8S103 -DF_CPU=16000000 -lstm8 -mstm8 $(INCLUDES)
LDFLAGS := 

OUT_IMAGE := main.ihx
SOURCES := src/stm8s_it.c src/main.c

OUTDIR ?= build

all: $(OUT_IMAGE)

$(OUT_IMAGE): $(OUTDIR)/$(OUT_IMAGE)
	cp $^ $@

$(OUTDIR)/$(OUT_IMAGE): $(addprefix $(OUTDIR)/,$(SOURCES:.c=.rel))
	$(SDCC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OUTDIR)/%.rel: %.c
	@mkdir -p $(dir $@)
	$(SDCC) -c $(CFLAGS) -o $(dir $@) $^

.PHONY: flash
flash: all
	$(STM8FLASH) -c$(PROGRAMMER) -p$(PART) -w $(OUT_IMAGE)

.PHONY: clean
clean:
	rm -f $(OUT_IMAGE)
	rm -rf $(OUTDIR)

