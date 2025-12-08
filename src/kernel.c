// src/kernel.c

#include <stddef.h>
#include <stdint.h>

static volatile uint16_t* const VIDEO_MEMORY = (uint16_t*)0xB8000;
static const size_t VIDEO_COLS = 80;
static const size_t VIDEO_ROWS = 25;

/* Записываем символ с атрибутом (цветом) в позицию (row, col) */
static void put_char_at(char c, uint8_t attr, size_t row, size_t col) {
    size_t index = row * VIDEO_COLS + col;
    VIDEO_MEMORY[index] = ((uint16_t)attr << 8) | (uint8_t)c;
}

/* Пишем строку в первую строку экрана */
static void write_string(const char* s) {
    uint8_t attr = 0x0F; // белый на чёрном
    size_t col = 0;
    while (*s && col < VIDEO_COLS) {
        put_char_at(*s++, attr, 0, col++);
    }
}

void kernel_main(void) {
    const char* msg = "Hello from BolgenOS-Core!";
    write_string(msg);

    // Бесконечный цикл, чтобы ядро не упало
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
