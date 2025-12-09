// src/idt.c

#include "idt.h"

#include <stddef.h>

extern void isr0(void);  // ASM-стаб для исключения #0

static idt_entry_t idt[256];
static idt_ptr_t   idt_reg;

/* Простая реализация memset, чтобы не тянуть стандартную библиотеку */
static void idt_memset(void* dst, uint8_t value, size_t size) {
    uint8_t* p = (uint8_t*)dst;
    for (size_t i = 0; i < size; ++i) {
        p[i] = value;
    }
}

static void idt_set_entry(
    uint8_t num,
    uint32_t base,
    uint16_t sel,
    uint8_t flags
) {
    idt[num].base_low  = (uint16_t)(base & 0xFFFF);
    idt[num].base_high = (uint16_t)((base >> 16) & 0xFFFF);

    idt[num].sel      = sel;
    idt[num].always0  = 0;
    idt[num].flags    = flags;
}

void idt_init(void) {
    // Обнуляем всю таблицу
    idt_memset(idt, 0, sizeof(idt));

    // Настраиваем IDTR
    idt_reg.limit = (uint16_t)(sizeof(idt_entry_t) * 256 - 1);
    idt_reg.base  = (uint32_t)&idt[0];

    // Регистрируем обработчик исключения #0 (деление на 0)
    // Селектор кода: 0x08 (первый сегмент кода GDT у GRUB)
    // Флаги: 0x8E = 10001110b
    //  - P=1 (present)
    //  - DPL=00 (ring 0)
    //  - Type=1110 (32-bit interrupt gate)
    idt_set_entry(0, (uint32_t)isr0, 0x08, 0x8E);

    // Загружаем IDT
    __asm__ __volatile__("lidtl (%0)" :: "r" (&idt_reg));
}
