// src/kernel.c

#include "console.h"

void kernel_main(void) {
    console_init();

    console_write("Hello from BolgenOS-Core!\n");
    console_write("Console driver moved to console.c/.h.\n");
    console_write("Now the kernel can use console_write() from anywhere.\n");

    // Пример смены цвета: светло-зелёный текст
    console_set_attr(0x0A);
    console_write("\n[OK] Kernel console is up.\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
