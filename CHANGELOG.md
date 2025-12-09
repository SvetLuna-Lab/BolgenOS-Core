# Changelog

All notable changes to this project will be documented in this file.

The format is inspired by Keep a Changelog and semantic versioning,
but kept intentionally simple for an educational hobby OS project.

---

## [0.2.0] – 2025-12-09

### Added
- VGA text console module (`console.c/.h`) with clear, scroll and attributes
- Minimal `kprintf` implementation (`kprintf.c/.h`) supporting %s, %c, %d, %x
- Kernel panic facility (`panic.c/.h`) with red-screen halt
- Interrupt Descriptor Table setup (`idt.c/.h`) and loading via `lidt`
- ASM stub for CPU exception #0 (division by zero) in `isr_stubs.s`
- C-level exception handler (`exceptions.c`) calling `panic()`
- Updated kernel entry (`kernel.c`) to use console + kprintf + IDT

### Changed
- Makefile updated to build and link all new modules
- README rewritten with detailed structure and build/run instructions

---

## [0.1.0] – 2025-12-08

### Added
- Initial Multiboot-compatible x86 kernel skeleton
- `boot.s` with Multiboot header and `_start` entry point
- `linker.ld` to place the kernel at 1 MiB
- Simple `kernel.c` writing a string to VGA text memory
- GRUB configuration (`iso/grub/grub.cfg`)
- Makefile to build `kernel.bin` and bootable ISO image
