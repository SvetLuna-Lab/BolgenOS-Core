# Makefile for BolgenOS-Core
# Minimal x86 Multiboot kernel (C + assembly)

# Cross-compiler toolchain (can be overridden from environment)
TARGET  ?= i386-elf
CC      := $(TARGET)-gcc
LD      := $(TARGET)-ld

CFLAGS  := -m32 -ffreestanding -O2 -Wall -Wextra -I./src
LDFLAGS := -m32 -T src/linker.ld -nostdlib

BUILD      := build
ISO_DIR    := iso
KERNEL_BIN := $(BUILD)/kernel.bin

OBJS := \
	$(BUILD)/boot.o \
	$(BUILD)/kernel.o \
	$(BUILD)/console.o \
	$(BUILD)/kprintf.o \
	$(BUILD)/panic.o \
	$(BUILD)/idt.o \
	$(BUILD)/isr_stubs.o \
	$(BUILD)/exceptions.o

all: $(KERNEL_BIN)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: src/boot.s | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.o: src/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/console.o: src/console.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kprintf.o: src/kprintf.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/panic.o: src/panic.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: src/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/isr_stubs.o: src/isr_stubs.s | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/exceptions.o: src/exceptions.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Build bootable ISO with GRUB
iso: $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	cp iso/grub/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o bolgenos-core.iso $(ISO_DIR)

# Run in QEMU (if installed)
run: iso
	qemu-system-i386 -cdrom bolgenos-core.iso

clean:
	rm -rf $(BUILD) $(ISO_DIR)/boot bolgenos-core.iso

.PHONY: all iso run clean
