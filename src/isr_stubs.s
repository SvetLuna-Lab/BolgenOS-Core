# src/isr_stubs.s
# Простейший обработчик исключения #0 (деление на 0)
# Передаёт управление в C-функцию isr0_handler и затем навсегда зависает.

    .section .text
    .globl isr0
    .extern isr0_handler

isr0:
    cli                 # на всякий случай отключаем прерывания
    call isr0_handler   # C-обработчик (не возвращается из panic)
.hang:
    hlt
    jmp .hang
