// src/task.c

#include "task.h"
#include "kprintf.h"
#include "console.h"

static uint32_t task0_counter = 0;
static uint32_t task1_counter = 0;

/*
 * Простейшие "задачи" ядра: не настоящие потоки,
 * а периодические шаги, которые вызываются по тикам таймера.
 */

static void task0_step(uint32_t tick) {
    task0_counter++;

    // Раз в 50 тиков помечаем "task0" как живую
    if (tick % 50u == 0u) {
        console_set_attr(0x0A); // светло-зелёный
        kprintf("[task0] heartbeat, tick = %u (counter = %u)\n",
                (unsigned)tick, (unsigned)task0_counter);
    }
}

static void task1_step(uint32_t tick) {
    task1_counter++;

    // Раз в 120 тиков — другая задача
    if (tick % 120u == 0u) {
        console_set_attr(0x0B); // светло-голубой
        kprintf("[task1] stats: tick = %u, loops = %u\n",
                (unsigned)tick, (unsigned)task1_counter);
    }
}

void tasks_init(void) {
    task0_counter = 0;
    task1_counter = 0;

    console_set_attr(0x0F); // обратно к белому на чёрном
    kprintf("[tasks] initialized (two periodic kernel tasks).\n");
}

/*
 * Простенький диспетчер, вызываемый на каждый тик таймера.
 */
void scheduler_on_tick(uint32_t tick) {
    task0_step(tick);
    task1_step(tick);
}
