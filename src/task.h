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
 * tick — глобальный счётчик тиков.
 */
void scheduler_on_tick(uint64_t tick);

#endif // BOLGENOS_TASK_H
