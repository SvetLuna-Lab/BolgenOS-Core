// src/io.h
#ifndef BOLGENOS_IO_H
#define BOLGENOS_IO_H

#include <stdint.h>

/* Записать байт в порт (outb). */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* Прочитать байт из порта (inb). */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif // BOLGENOS_IO_H
