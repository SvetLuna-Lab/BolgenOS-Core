// src/idt.c

#include "idt.h"

#include <stddef.h>
#include <stdint.h>

extern void isr0(void);  // ASM-стаб для исключения #0
extern void irq0(void);  // ASM-стаб для IRQ0 (таймер)

static idt_entry_t idt[256];
static idt_ptr_t   idt_reg;

/* Простая memset без стандартной библиотеки. */
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
    // Обнуляем таблицу
    idt_memset(idt, 0, sizeof(idt));

    // Настраиваем IDTR
    idt_reg.limit = (uint16_t)(sizeof(idt_entry_t) * 256 - 1);
    idt_reg.base  = (uint32_t)&idt[0];

    // Исключение #0 (division by zero)
    // селектор кода: 0x08, флаги: 0x8E (present, ring0, 32-bit interrupt gate)
    idt_set_entry(0, (uint32_t)isr0, 0x08, 0x8E);

    // IRQ0 (таймер) → вектор 0x20
    idt_set_entry(0x20, (uint32_t)irq0, 0x08, 0x8E);

    // Загружаем IDT
    __asm__ __volatile__("lidtl (%0)" :: "r" (&idt_reg));
}
