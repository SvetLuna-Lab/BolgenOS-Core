// src/kernel.c

#include "console.h"
#include "kprintf.h"
#include "panic.h"

void kernel_main(void) {
    console_init();

    console_write("BolgenOS-Core booted.\n");
    console_write("VGA text console initialized.\n\n");

    kprintf("Kernel greeting from %s!\n", "BolgenOS-Core");
    kprintf("Example: decimal = %d, hex = 0x%x\n", 42, 42);

    console_set_attr(0x0A); // светло-зелёный
    console_write("\n[OK] kprintf is working.\n");

    // Пример "условной паники" (закомментировано, чтобы не мешало):
    // panic("Demo panic: nothing to do yet.");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
