# BolgenOS-Core – Minimal x86 Kernel Prototype

**BolgenOS-Core** is a small, educational OS kernel prototype for the x86 architecture.

It is **not** a full operating system.  
Instead, it is a minimal, Multiboot-compatible kernel that:

- boots via GRUB in 32-bit mode,
- jumps from a tiny assembly stub into C code,
- writes text directly into VGA text memory (`0xB8000`),
- installs its own Interrupt Descriptor Table (IDT),
- handles at least one CPU exception (#0 – division by zero) via a kernel panic,
- then halts in an infinite loop.

The goal is to provide a **clean starting point** for learning OS development:
C + assembly, linker scripts, boot process, interrupts.

---

## 1. Project structure

```text
bolgenos-core/
├─ Makefile                # build kernel and ISO image, run in QEMU
├─ README.md               # this file
├─ src/
│  ├─ boot.s               # Multiboot header + entry point (_start)
│  ├─ linker.ld            # linker script (load the kernel at 1 MiB)
│  ├─ kernel.c             # kernel_main(): high-level initialization
│  │
│  ├─ console.h            # VGA text console interface
│  ├─ console.c            # VGA text console implementation
│  │
│  ├─ kprintf.h            # minimal printf-like API for the kernel
│  ├─ kprintf.c            # implementation of kprintf (%s, %c, %d, %x)
│  │
│  ├─ panic.h              # kernel panic API
│  ├─ panic.c              # panic(): red screen + system halt
│  │
│  ├─ idt.h                # IDT structures and idt_init()
│  ├─ idt.c                # IDT setup and lidt
│  │
│  ├─ isr_stubs.s          # ASM stub for CPU exception #0 (division by zero)
│  └─ exceptions.c         # C-level handler for exception #0 → panic()
│
└─ iso/
   └─ grub/
      └─ grub.cfg          # GRUB menu entry for the kernel
```

Later additions (not yet implemented here) may include:

- pic.c, timer.c for IRQ handling and PIT timer,

- simple task switching,

- basic memory management.


## 2. Components
2.1. Boot stub and linker script
src/boot.s

- Contains a Multiboot header recognized by GRUB.

- Defines the _start entry point.

- Disables interrupts (cli) for safety.

- Calls kernel_main in C.

- After kernel_main returns (normally it does not), loops forever with hlt.

This file uses GNU assembler (GAS) syntax and is compiled with i386-elf-gcc.

src/linker.ld

- Sets the entry point to _start.

- Places the kernel at physical address 1 MiB (. = 1M;).

- Orders sections as:

- .multiboot (Multiboot header),

- .text* (code),

- .rodata* (read-only data),

- .data* (initialized data),

- .bss* + COMMON (zero-initialized data).

This layout follows common OSDev practice for a simple 32-bit kernel.


2.2. VGA text console
src/console.h / src/console.c

Provides a minimal VGA text-mode console:

- Hardcoded address 0xB8000 (80×25 text mode).

- Functions:

void console_init(void);
void console_set_attr(uint8_t attr);
void console_putc(char c);
void console_write(const char* s);


Features:

- screen clear at initialization,

- tracking of cursor_row and cursor_col,

- newline handling ('\n'),

- automatic scroll when reaching the bottom of the screen.

All higher-level output (kprintf, panic messages) goes through this console.


2.3. kprintf – kernel-side formatted output
src/kprintf.h / src/kprintf.c

A tiny, freestanding printf-like implementation for the kernel:

- uses only the console API (console_putc),

- no standard library, no stdio.h,

- supports:

- %s – string (const char*),

- %c – character,

- %d – signed decimal integer,

- %x – unsigned hexadecimal integer (lowercase),

- %% – literal %.

Example usage:

kprintf("Kernel greeting from %s!\n", "BolgenOS-Core");
kprintf("Example: decimal = %d, hex = 0x%x\n", 42, 42);

This is the main logging mechanism for early kernel code.


2.4. Kernel panic
src/panic.h / src/panic.c

Implements a simple panic facility:

void panic(const char* msg);


Behavior:

- switches console attribute to red background, light gray text (0x4F),

- prints KERNEL PANIC: <message> and System halted.,

- disables interrupts (cli),

- enters an infinite hlt loop.

Any fatal error or unhandled exception can call panic().

Example:

panic("Demo panic: nothing to do yet.");


2.5. IDT (Interrupt Descriptor Table) and exception #0
src/idt.h / src/idt.c

Defines:

- idt_entry_t – a 32-bit interrupt gate entry,

- idt_ptr_t – the structure loaded into IDTR by lidt,

- idt_init() – initialization routine.

On idt_init():

- the IDT (256 entries) is zeroed,

- idtr (limit + base) is set,

- entry 0 (CPU exception #0 – division by zero) is configured:

- handler: isr0 (ASM stub),

- code segment selector: 0x08 (first code segment from GRUB),

- flags: 0x8E (present, ring 0, 32-bit interrupt gate),

- lidt is executed to load the new IDT.

At this stage only exception #0 is hooked, but the structure is ready for more (exceptions, IRQs).

src/isr_stubs.s

Defines a minimal ASM stub for exception #0:

.globl isr0
.extern isr0_handler

isr0:
    cli
    call isr0_handler
.hang:
    hlt
    jmp .hang


- The stub disables interrupts,

- calls the C-level handler isr0_handler,

- then halts forever (panic handler itself also halts the system).

src/exceptions.c

Implements a C-level handler for exception #0:

void isr0_handler(void) {
    panic("CPU exception #0: division by zero");
}


2.6. Kernel entry – src/kernel.c

kernel_main() ties everything together at a high level:

- initializes the console,

- prints boot messages,

- sets up the IDT,

- enables interrupts (sti),

- then enters an infinite hlt loop.

Example flow:


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

    console_set_attr(0x0A); // light green
    console_write("\n[OK] IDT is active, interrupts enabled.\n");

    kprintf("Kernel greeting from %s!\n", "BolgenOS-Core");
    kprintf("Example: decimal = %d, hex = 0x%x\n", 42, 42);

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}


At this stage, we do not yet configure hardware IRQs or the PIT timer,
but the kernel already owns its IDT and can handle at least one CPU exception.


## 3. Building and running
3.1. Prerequisites

You need a 32-bit cross-compiler toolchain and GRUB tools, for example:

- i386-elf-gcc, i386-elf-ld

- grub-mkrescue

- qemu-system-i386 (or another virtual machine to boot the ISO)

The Makefile assumes:

TARGET = i386-elf
CC     = i386-elf-gcc
LD     = i386-elf-ld


If your toolchain has different names, you can override TARGET:

make TARGET=i386-elf

(or adjust the Makefile accordingly).


3.2. Build the kernel binary

From the repository root:

make


This will:

- assemble src/boot.s,

- compile all C sources:

- kernel.c, console.c, kprintf.c, panic.c,

- idt.c, exceptions.c,

- compile ASM stubs isr_stubs.s,

- link them into build/kernel.bin using src/linker.ld.

If it succeeds, you should see build/kernel.bin in the build/ directory.


3.3. Build a bootable ISO image

make iso

This will:

- create iso/boot/grub if needed,

- copy build/kernel.bin to iso/boot/kernel.bin,

- copy iso/grub/grub.cfg to iso/boot/grub/grub.cfg,

- run grub-mkrescue to produce:

bolgenos-core.iso

in the repository root.


3.4. Run in QEMU

The simplest way to boot the kernel:

make run

This is equivalent to:

qemu-system-i386 -cdrom bolgenos-core.iso


On successful boot you should see something like:
```text
BolgenOS-Core booted.
VGA text console initialized.

Setting up IDT...
IDT loaded. Enabling interrupts (sti).

[OK] IDT is active, interrupts enabled.
Kernel greeting from BolgenOS-Core!
Example: decimal = 42, hex = 0x2a



```
The CPU then stays in an infinite hlt loop,
ready for future work (timer, scheduler, etc.).


## 4. Educational purpose and next steps

BolgenOS-Core is intentionally minimal.

It is designed to be a clean, readable starting point for:

- understanding the Multiboot boot process via GRUB,

- writing freestanding C code for a kernel (no standard library),

- working with VGA text mode,

- setting up an IDT and handling a basic CPU exception,

- structuring kernel code into modules (console, logging, panic, IDT).

Possible next steps (not implemented yet in this repository):

1. PIC remapping and IRQ handling

- remap the legacy PIC to avoid conflicts with CPU exceptions,

- introduce irq stubs and handlers,

- start handling hardware interrupts.

2. PIT timer and kernel ticks

- configure the PIT (8253/8254) to a chosen frequency (e.g. 100 Hz),

- maintain a global tick counter,

- implement basic delay/sleep in terms of ticks.

3. Simple task switching

- define a basic task structure,

- save/restore CPU context,

- switch between tasks periodically using the timer interrupt.

4. Memory management

- early physical memory map (from Multiboot),

- simple frame allocator,

- later – paging and virtual memory.

5. Higher-level kernel services

- more robust logging,

- richer console (scroll, colors, simple UI),

- eventually a userland and a tiny shell.

This project is primarily for learning and for use in personal portfolios
to demonstrate interest and skills in low-level / systems programming.

## 5. License

This project is released under the MIT License.
See the LICENSE file for details.

