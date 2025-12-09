// src/idt.h
#ifndef BOLGENOS_IDT_H
#define BOLGENOS_IDT_H

#include <stdint.h>

/*
 * Одна запись IDT (Interrupt Descriptor Table) для 32-битного x86.
 */
typedef struct {
    uint16_t base_low;   // младшие 16 бит адреса обработчика
    uint16_t sel;        // селектор сегмента кода
    uint8_t  always0;    // всегда 0
    uint8_t  flags;      // тип + флаги
    uint16_t base_high;  // старшие 16 бит адреса обработчика
} __attribute__((packed)) idt_entry_t;

/*
 * Псевдо-регистр IDTR для инструкции lidt.
 */
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

/*
 * Инициализация IDT: заполняет таблицу и вызывает lidt.
 * На этом шаге регистрируем только обработчик исключения #0 (деление на 0).
 */
void idt_init(void);

#endif // BOLGENOS_IDT_H
