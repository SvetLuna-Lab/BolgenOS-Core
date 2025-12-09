# BolgenOS-Core Architecture Overview

This document gives a high-level view of how BolgenOS-Core is structured
and how control flows from the bootloader into the kernel and its modules.

---

## 1. Boot flow

```text
BIOS / UEFI
   ↓
GRUB (Multiboot loader)
   ↓
multiboot header in boot.s
   ↓
_start (boot.s, 32-bit mode)
   ↓
kernel_main (kernel.c)
```

1. BIOS/UEFI transfers control to GRUB.

2. GRUB detects the Multiboot header in boot.s and loads kernel.bin
at the requested address (1 MiB).

3. GRUB jumps to the entry point _start.

4. _start (in boot.s) calls kernel_main (in kernel.c).

## 2. Kernel entry and core modules
```text
                 +-----------------+
                 |   boot.s (_start)|
                 +---------+-------+
                           |
                           v
                    +------+------+
                    | kernel_main |
                    +------+------+
                           |
         +-----------------+-------------------+
         |                 |                   |
         v                 v                   v
   console_init()     idt_init()          (future) timer_init()
   kprintf()          lidt()              task setup, etc.
   panic()            sti
```

- kernel.c is responsible for:

- initializing the VGA console,

- printing early boot messages,

- setting up the IDT,

- enabling interrupts (via sti),

- entering an idle loop (hlt).

- console.c/.h implement a simple VGA text console.

- kprintf.c/.h provide formatted text output on top of the console.

- panic.c/.h provide a red-screen “kernel panic” and halt.


## 3. Interrupts and exceptions
```text
CPU exception #0 (division by zero)
             ↓
          IDT[0]
             ↓
          isr0 (ASM stub, isr_stubs.s)
             ↓
      isr0_handler (exceptions.c)
             ↓
          panic() → red screen + halt

```

- The IDT (256 entries) is defined in idt.c.

- On initialization:

- entry 0 (exception #0) is set to point to isr0.

- idtr is loaded via lidt.

- isr0 (in isr_stubs.s) is a small ASM stub:

- disables interrupts,

- calls isr0_handler (C),

- then halts forever.

- isr0_handler (in exceptions.c):

- calls panic("CPU exception #0: division by zero");

Later, the same pattern can be extended for other exceptions (page fault, GPF)
and for hardware IRQs (via PIC and a timer).



## 4. Future extensions (planned modules)

Potential future modules and their roles:

- pic.c/.h – remap the legacy PIC, unmask IRQs, send EOI.

- timer.c/.h – configure the PIT (8253/8254), keep a global tick counter.

- task.c/.h – define task structures and implement context switching.

- mm.c/.h – early physical memory manager, later paging and virtual memory.

These modules will be plugged into kernel_main() after the current steps,
using the console and kprintf for logging and panic for fatal errors.

