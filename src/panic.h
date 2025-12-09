// src/panic.h
#ifndef BOLGENOS_PANIC_H
#define BOLGENOS_PANIC_H

/**
 * Неблокируемая ошибка ядра: выводит сообщение и останавливает систему.
 */
void panic(const char* msg);

#endif // BOLGENOS_PANIC_H
