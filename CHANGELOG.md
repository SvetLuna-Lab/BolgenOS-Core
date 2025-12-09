# Changelog

All notable changes to this project are documented here.
Format follows a simplified semantic versioning pattern.

---

## [0.3.0] – 2025-12-09

### Added
- **task.c/.h:** first cooperative task scheduler driven by IRQ0 ticks.
- **io.h:** inline port I/O helpers (`inb`, `outb`).
- **pic.c/.h:** remapping legacy PIC, unmasking IRQ0, sending EOI.
- **timer.c/.h:** configuring PIT 8253 at 100 Hz and calling `scheduler_on_tick()`.
- **isr_stubs.s:** new ASM stub `irq0` saving registers and returning via `iretd`.
- **kernel.c:** initializes IDT → PIC → PIT → tasks → enables interrupts.
- **Makefile:** added `task.o` to build sequence.

### Changed
- IDT initialization now installs vector 0x20 for IRQ0 handler.
- Timer IRQ handler delegates control to the scheduler each tick.
- Console messages now color-coded for task activity (green/blue/gray).

### Result
Kernel now produces real hardware interrupts, periodic ticks and two
independent kernel tasks executing in parallel under a minimal scheduler.

---

## [0.2.0] – 2025-12-09
*(IDT + PIC + Timer introduction)*

### Added
- VGA text console module (`console.c/.h`)
- Minimal `kprintf` with `%s`, `%c`, `%d`, `%x`
- Kernel panic screen
- IDT initialization and exception #0 handler
- PIC remapping and IRQ0 timer support

### Changed
- Makefile and README updated with build/run workflow

---

## [0.1.0] – 2025-12-08
Initial public version — minimal Multiboot-compatible x86 kernel
printing “Hello world” through VGA text memory.

