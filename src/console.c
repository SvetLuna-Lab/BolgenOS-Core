// src/console.c

#include "console.h"

static volatile uint16_t* const VIDEO_MEMORY = (uint16_t*)0xB8000;
static const size_t VIDEO_COLS = 80;
static const size_t VIDEO_ROWS = 25;

static size_t cursor_row = 0;
static size_t cursor_col = 0;
static uint8_t current_attr = 0x0F; // белый на чёрном по умолчанию

static void put_entry_at(char c, uint8_t attr, size_t row, size_t col) {
    size_t index = row * VIDEO_COLS + col;
    VIDEO_MEMORY[index] = ((uint16_t)attr << 8) | (uint8_t)c;
}

static void clear_screen_internal(void) {
    for (size_t row = 0; row < VIDEO_ROWS; ++row) {
        for (size_t col = 0; col < VIDEO_COLS; ++col) {
            put_entry_at(' ', current_attr, row, col);
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

static void scroll_if_needed(void) {
    if (cursor_row < VIDEO_ROWS) {
        return;
    }

    // Сдвигаем все строки вверх на одну
    for (size_t row = 1; row < VIDEO_ROWS; ++row) {
        for (size_t col = 0; col < VIDEO_COLS; ++col) {
            size_t from = row * VIDEO_COLS + col;
            size_t to   = (row - 1) * VIDEO_COLS + col;
            VIDEO_MEMORY[to] = VIDEO_MEMORY[from];
        }
    }

    // Очищаем последнюю строку
    for (size_t col = 0; col < VIDEO_COLS; ++col) {
        put_entry_at(' ', current_attr, VIDEO_ROWS - 1, col);
    }

    cursor_row = VIDEO_ROWS - 1;
}

void console_init(void) {
    current_attr = 0x0F; // можно потом сделать параметром
    clear_screen_internal();
}

void console_set_attr(uint8_t attr) {
    current_attr = attr;
}

void console_putc(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
        return;
    }

    if (cursor_col >= VIDEO_COLS) {
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
    }

    put_entry_at(c, current_attr, cursor_row, cursor_col);
    cursor_col++;
}

void console_write(const char* s) {
    while (*s) {
        console_putc(*s++);
    }
}
