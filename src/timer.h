// src/timer.h
#ifndef BOLGENOS_TIMER_H
#define BOLGENOS_TIMER_H

#include <stdint.h>

/*
 * Глобальный счётчик тиков таймера.
 * 32-битный счётчик более чем достаточен для учебного ядра.
 */
extern volatile uint32_t timer_ticks;

/*
 * Инициализация программируемого таймера PIT.
 * frequency — желаемая частота тиков (например, 100 Гц).
 */
void timer_init(uint32_t frequency);

/*
 * C-обработчик IRQ0, вызывается из ASM-стаба irq0.
 */
void irq0_handler(void);

#endif // BOLGENOS_TIMER_H
