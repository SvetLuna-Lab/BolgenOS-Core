// src/timer.c

#include "timer.h"
#include "io.h"
#include "pic.h"
#include "kprintf.h"
#include "task.h"
#include "console.h"

#define PIT_FREQUENCY 1193180u

volatile uint32_t timer_ticks = 0;

void timer_init(uint32_t frequency) {
    if (frequency == 0u) {
        frequency = 100u; // защита от деления на ноль
    }

    uint32_t divisor = PIT_FREQUENCY / frequency;

    // Режим: канал 0, lobyte/hibyte, square wave generator
    outb(0x43, 0x36);

    // Делитель: младший байт, потом старший
    outb(0x40, (uint8_t)(divisor & 0xFFu));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFFu));
}

/* C-обработчик IRQ0 — вызывается из ASM-стаба irq0. */
void irq0_handler(void) {
    timer_ticks++;

    // Даем слово планировщику — он сам решит, какие задачи дергать
    scheduler_on_tick(timer_ticks);

    // Для общей диагностики — редкий лог от таймера
    if (timer_ticks % 1000u == 0u) {
        console_set_attr(0x07); // серый
        kprintf("[timer] tick = %u\n", (unsigned)timer_ticks);
    }

    // Сообщаем PIC, что прерывание обработано
    pic_send_eoi(0);
}
