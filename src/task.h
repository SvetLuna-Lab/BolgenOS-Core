// src/task.h
#ifndef BOLGENOS_TASK_H
#define BOLGENOS_TASK_H

#include <stdint.h>

/*
 * Инициализация подсистемы задач.
 * Вызывается один раз из kernel_main().
 */
void tasks_init(void);

/*
 * Вызывается по каждому тиканью таймера (IRQ0).
 * tick — глобальный 32-битный счётчик тиков.
 */
void scheduler_on_tick(uint32_t tick);

#endif // BOLGENOS_TASK_H
