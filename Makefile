# Makefile

TARGET   := i386-elf
CC       := $(TARGET)-gcc
LD       := $(TARGET)-ld

CFLAGS   := -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS  := -m32 -T src/linker.ld -nostdlib

BUILD    := build

all: $(BUILD)/kernel.bin

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: src/boot.s | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.o: src/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.bin: $(BUILD)/boot.o $(BUILD)/kernel.o
	$(LD) $(LDFLAGS) -o $@ $^

iso: all
	mkdir -p iso/boot/grub
	cp $(BUILD)/kernel.bin iso/boot/kernel.bin
	cp iso/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o bolgenos-core.iso iso

clean:
	rm -rf $(BUILD) iso bolgenos-core.iso

.PHONY: all iso clean
