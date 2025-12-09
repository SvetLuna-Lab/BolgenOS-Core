// src/timer.h
#ifndef BOLGENOS_TIMER_H
#define BOLGENOS_TIMER_H

#include <stdint.h>

/* Глобальный счётчик тиков (увеличивается на каждый IRQ0). */
extern volatile uint64_t timer_ticks;

/* Инициализация PIT: задаём частоту IRQ0 (в Гц). */
void timer_init(uint32_t frequency);

/* Обработчик IRQ0 (вызывается из ASM-стаба irq0). */
void irq0_handler(void);

#endif // BOLGENOS_TIMER_H
