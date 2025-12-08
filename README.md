# BolgenOS-Core – Minimal x86 Kernel Prototype

**BolgenOS-Core** is a small, educational OS kernel prototype for the x86 architecture.

It is **not** a full operating system.  
Instead, it is a minimal, Multiboot-compatible kernel that:

- boots via GRUB in 32-bit mode,
- switches to C code from a tiny assembly stub,
- writes a message directly into VGA text memory (`0xB8000`),
- then halts in an infinite loop.

This repository is intended as a **first step** towards a hobby OS:
a clean, understandable starting point for learning OS development
(C, assembly, linker scripts, boot process).

---

## 1. Project structure

```text
bolgenos-core/
├─ Makefile                # build kernel and ISO image
├─ README.md               # this file
├─ src/
│  ├─ boot.s               # Multiboot header + entry point (_start)
│  ├─ kernel.c             # kernel_main(): "Hello from BolgenOS-Core!"
│  └─ linker.ld            # linker script (load at 1 MiB)
└─ iso/
   └─ grub/
      └─ grub.cfg          # GRUB menu entry for the kernel
```

2. Components
2.1 boot.s – Multiboot entry and hand-off to C

Contains a Multiboot header that GRUB recognizes.

Defines the _start entry point.

Disables interrupts (cli) and calls kernel_main in C.

After returning, executes hlt in an infinite loop.

This file is written in GAS (GNU assembler) syntax
and is compiled with i386-elf-gcc.

2.2 kernel.c – “Hello from BolgenOS-Core”

The kernel is written in freestanding C:

No standard library, no stdio.h, no dynamic memory.

A hardcoded pointer to VGA text memory at 0xB8000.

A tiny text output routine that writes characters
with a simple attribute (white on black) into the first row.

On boot, kernel_main() writes:

Hello from BolgenOS-Core!

and then halts the CPU inside an infinite loop using hlt.


2.3 linker.ld – linking the kernel image

The linker script:

sets the entry point to _start,

places the kernel at the physical address 1 MiB,

groups sections in the following order:

.multiboot (Multiboot header),

.text* (code),

.rodata* (read-only data),

.data* (initialized data),

.bss* and COMMON (zero-initialized data).

This matches typical OSDev patterns for a simple 32-bit kernel.

2.4 iso/grub/grub.cfg – GRUB configuration

Provides a single menu entry:

menuentry "BolgenOS-Core" {
    multiboot /boot/kernel.bin
    boot
}

GRUB loads /boot/kernel.bin as a Multiboot kernel and jumps to _start.


3. Building

Note: The commands below assume a cross-compiler toolchain
such as i386-elf-gcc and i386-elf-ld, plus grub-mkrescue.

3.1 Build the kernel binary

From the repository root:

make

This will:

assemble src/boot.s,

compile src/kernel.c,

link them into build/kernel.bin using src/linker.ld.

3.2 Build a bootable ISO image

make iso

This will:

create the iso/boot and iso/boot/grub directories if needed,

copy kernel.bin to iso/boot/kernel.bin,

copy iso/grub/grub.cfg into iso/boot/grub/grub.cfg,

run grub-mkrescue to produce bolgenos-core.iso.

4. Running in QEMU

After make iso, you can boot the kernel in QEMU:

qemu-system-i386 -cdrom bolgenos-core.iso

If everything is built correctly, you should see a black screen with:

Hello from BolgenOS-Core!


The CPU then stays in an infinite hlt loop.

You can also boot the ISO in other virtual machines
(VirtualBox, VMware) by attaching bolgenos-core.iso
as a CD/DVD image.


5. Educational purpose and next steps

BolgenOS-Core is intentionally minimal.
It is meant to be a clean starting point for exploring:

the Multiboot standard and how GRUB loads a kernel;

how to write freestanding C code for the kernel;

how to use a linker script to control memory layout.

Possible next steps:

add a basic interrupt descriptor table (IDT) and a timer interrupt;

implement a very simple task switcher (cooperative or timer-based);

add a basic physical/virtual memory abstraction;

introduce a small in-kernel console instead of direct VGA writes.

This repository is a first brick for a hobby OS project
and complements higher-level simulations (e.g. BolgenOS-Sim in Python).


6. License

This project is released under the MIT License.
See the LICENSE file for details.

