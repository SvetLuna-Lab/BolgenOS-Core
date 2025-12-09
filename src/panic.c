// src/panic.c

#include "panic.h"
#include "console.h"
#include "kprintf.h"

void panic(const char* msg) {
    // Красный фон, светло-серый текст: 0x4F
    console_set_attr(0x4F);
    console_write("\n\nKERNEL PANIC: ");
    if (msg) {
        kprintf("%s\n", msg);
    } else {
        console_write("(no message)\n");
    }

    console_write("System halted.\n");

    // Отключаем прерывания и навсегда зависаем
    __asm__ __volatile__("cli");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
