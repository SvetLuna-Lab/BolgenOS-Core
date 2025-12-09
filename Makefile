# BolgenOS-Core build system (gcc-based, no cross toolchain required)

# Имя ядра и ISO
TARGET      := bolgenos-core
ISO         := $(TARGET).iso

# Директории
BUILD_DIR   := build
ISO_DIR     := iso
BOOT_DIR    := $(ISO_DIR)/boot
GRUB_DIR    := $(BOOT_DIR)/grub

# Компилятор по умолчанию.
# При желании можно переопределить: make CC=i386-elf-gcc
CC ?= gcc

# Флаги для компиляции и линковки
CFLAGS  := -m32 -ffreestanding -O2 -Wall -Wextra -I./src
LDFLAGS := -m32 -ffreestanding -nostdlib -T linker.ld

# Источники
C_SOURCES   := $(wildcard src/*.c)
ASM_SOURCES := $(wildcard src/*.s)

# Объектные файлы
C_OBJECTS   := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst src/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
OBJECTS     := $(ASM_OBJECTS) $(C_OBJECTS)

.PHONY: all clean iso

# Сборка ядра по умолчанию
all: $(BUILD_DIR)/kernel.elf

# Убедимся, что каталог сборки существует
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Правило для *.s -> *.o
$(BUILD_DIR)/%.o: src/%.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для *.c -> *.o
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка ядра (через gcc, чтобы -m32 и -lgcc работали корректно)
$(BUILD_DIR)/kernel.elf: $(OBJECTS) linker.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) -lgcc

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
