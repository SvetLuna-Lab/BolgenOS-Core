// src/console.h
#ifndef BOLGENOS_CONSOLE_H
#define BOLGENOS_CONSOLE_H

#include <stddef.h>
#include <stdint.h>

/**
 * Инициализация экранной консоли:
 * - очистка экрана;
 * - установка курсора в (0, 0);
 * - установка атрибута по умолчанию (белый на чёрном).
 */
void console_init(void);

/**
 * Установка цветового атрибута по умолчанию.
 *
 * attr: старший байт VGA-слова (0x0F = белый на чёрном, 0x0A = светло-зелёный и т.п.)
 */
void console_set_attr(uint8_t attr);

/**
 * Вывод одного символа на текущую позицию с обработкой:
 * - '\n' → перенос строки;
 * - прокрутка экрана при достижении нижней границы.
 */
void console_putc(char c);

/**
 * Вывод C-строки (до '\0') на экран.
 */
void console_write(const char* s);

#endif // BOLGENOS_CONSOLE_H
