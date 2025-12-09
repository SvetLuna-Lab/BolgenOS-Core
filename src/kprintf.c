// src/kprintf.c

#include "kprintf.h"
#include "console.h"

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

static void kputc(char c) {
    console_putc(c);
}

static void kputs(const char* s) {
    if (!s) {
        kputs("(null)");
        return;
    }
    while (*s) {
        kputc(*s++);
    }
}

static void kputu_dec(unsigned int value) {
    char buf[16];
    int i = 0;

    if (value == 0) {
        kputc('0');
        return;
    }

    while (value > 0 && i < (int)sizeof(buf)) {
        unsigned int digit = value % 10;
        buf[i++] = (char)('0' + digit);
        value /= 10;
    }

    // выводим в обратном порядке
    while (i > 0) {
        kputc(buf[--i]);
    }
}

static void kput_int(int value) {
    if (value < 0) {
        kputc('-');
        // приведение к unsigned, чтобы избежать проблем с INT_MIN
        kputu_dec((unsigned int)(- (long)value));
    } else {
        kputu_dec((unsigned int)value);
    }
}

static void kputu_hex(unsigned int value) {
    char buf[16];
    int i = 0;

    if (value == 0) {
        kputc('0');
        return;
    }

    while (value > 0 && i < (int)sizeof(buf)) {
        unsigned int digit = value & 0xF;
        if (digit < 10) {
            buf[i++] = (char)('0' + digit);
        } else {
            buf[i++] = (char)('a' + (digit - 10));
        }
        value >>= 4;
    }

    // выводим в обратном порядке
    while (i > 0) {
        kputc(buf[--i]);
    }
}

void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            kputc(*fmt++);
            continue;
        }

        fmt++;  // пропускаем '%'
        char spec = *fmt++;

        switch (spec) {
        case 's': {
            const char* s = va_arg(args, const char*);
            kputs(s);
            break;
        }
        case 'c': {
            int c = va_arg(args, int); // char промотируется до int
            kputc((char)c);
            break;
        }
        case 'd': {
            int v = va_arg(args, int);
            kput_int(v);
            break;
        }
        case 'x': {
            unsigned int v = va_arg(args, unsigned int);
            kputu_hex(v);
            break;
        }
        case '%':
            kputc('%');
            break;
        default:
            // неизвестный спецификатор: выводим как есть
            kputc('%');
            kputc(spec);
            break;
        }
    }

    va_end(args);
}
