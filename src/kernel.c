// src/kernel.c

#include "console.h"
#include "kprintf.h"
#include "panic.h"
#include "idt.h"

void kernel_main(void) {
    console_init();

    console_write("BolgenOS-Core booted.\n");
    console_write("VGA text console initialized.\n\n");

    kprintf("Setting up IDT...\n");
    idt_init();
    kprintf("IDT loaded. Enabling interrupts (sti).\n");

    __asm__ __volatile__("sti");

    console_set_attr(0x0A); // светло-зелёный
    console_write("\n[OK] IDT is active, interrupts enabled.\n");

    // На этом шаге мы ИДЕАЛЬНО можем проверить исключение #0:
    // раскомментировать блок ниже и пересобрать,
    // но компилятор может ругаться на division by zero.
    //
    // volatile int zero = 0;
    // volatile int one  = 1;
    // volatile int x    = one / zero;
    // (void)x;

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
