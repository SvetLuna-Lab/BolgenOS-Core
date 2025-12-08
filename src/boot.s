/* src/boot.s */

/*
 * Multiboot-заголовок для GRUB
 * (упрощённо, 32-битный режим).
 */

.set ALIGN,    1 << 0
.set MEMINFO,  1 << 1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

    .section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

    .section .text
    .globl _start
    .extern kernel_main

_start:
    cli                 /* отключаем прерывания на всякий случай */

    call kernel_main    /* прыгаем в C-код */

.hang:
    hlt                 /* останавливаем CPU до следующего прерывания */
    jmp .hang
