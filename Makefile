# BolgenOS-Core minimal build system

# Имя ядра и ISO
TARGET      := bolgenos-core
ISO         := $(TARGET).iso

# Директории
BUILD_DIR   := build
ISO_DIR     := iso
BOOT_DIR    := $(ISO_DIR)/boot
GRUB_DIR    := $(BOOT_DIR)/grub

# Компилятор и линкер по умолчанию.
# При желании можно переопределить: make CC=i386-elf-gcc LD=i386-elf-ld
CC ?= gcc
LD ?= ld

# Флаги для компиляции и линковки
CFLAGS  := -m32 -ffreestanding -O2 -Wall -Wextra -I./src
LDFLAGS := -m elf_i386 -T linker.ld -nostdlib

# Источники и объектные файлы
C_SOURCES   := $(wildcard src/*.c)
C_OBJECTS   := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(BUILD_DIR)/boot.o
OBJECTS     := $(ASM_OBJECTS) $(C_OBJECTS)

.PHONY: all clean iso

# Сборка ядра по умолчанию
all: $(BUILD_DIR)/kernel.elf

# Убедимся, что каталог сборки существует
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# boot.s -> boot.o
$(BUILD_DIR)/boot.o: src/boot.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# *.c -> *.o
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка ядра
$(BUILD_DIR)/kernel.elf: $(OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

# Цель для ISO
iso: $(ISO)

$(ISO): $(BUILD_DIR)/kernel.elf grub/grub.cfg
	rm -rf $(ISO_DIR)
	mkdir -p $(GRUB_DIR)
	cp $(BUILD_DIR)/kernel.elf $(BOOT_DIR)/kernel.elf
	cp grub/grub.cfg $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Очистка
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO)
