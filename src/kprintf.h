// src/kprintf.h
#ifndef BOLGENOS_KPRINTF_H
#define BOLGENOS_KPRINTF_H

#include <stdarg.h>

/**
 * kprintf(fmt, ...) — минимальный printf ядра.
 *
 * Поддерживаемые спецификаторы:
 *   %s  - строка (const char*)
 *   %c  - символ (char)
 *   %d  - десятичное целое (int)
 *   %x  - шестнадцатеричное беззнаковое (unsigned int)
 *
 * Все выводит через console_putc().
 */

void kprintf(const char* fmt, ...);

#endif // BOLGENOS_KPRINTF_H
