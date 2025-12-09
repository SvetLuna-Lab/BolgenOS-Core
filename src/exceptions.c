// src/exceptions.c

#include "panic.h"
#include "kprintf.h"

/*
 * Обработчик исключения #0: деление на ноль.
 * Здесь можно вывести дополнительную информацию, но пока достаточно паники.
 */
void isr0_handler(void) {
    panic("CPU exception #0: division by zero");
}
