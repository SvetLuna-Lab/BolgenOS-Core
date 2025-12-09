// src/kernel.c

#include "console.h"
#include "kprintf.h"
#include "panic.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"

void kernel_main(void) {
    console_init();

    console_write("BolgenOS-Core booted.\n");
    console_write("VGA text console initialized.\n\n");

    kprintf("Setting up IDT...\n");
    idt_init();
    kprintf("IDT loaded.\n");

    kprintf("Remapping PIC and configuring timer...\n");
    pic_remap();
    timer_init(100);  // 100 Гц → ~1 тик каждые 10 мс

    kprintf("Enabling interrupts (sti).\n");
    __asm__ __volatile__("sti");

    console_set_attr(0x0A); // светло-зелёный
    console_write("\n[OK] IDT + PIC + timer are active.\n");
    console_write("[OK] Waiting for timer IRQs (tick messages)...\n\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
