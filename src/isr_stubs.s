# src/isr_stubs.s
# Стабы для:
#  - CPU exception #0 (division by zero) → isr0_handler (C)
#  - IRQ0 (timer) → irq0_handler (C)

    .section .text
    .globl isr0
    .globl irq0

    .extern isr0_handler
    .extern irq0_handler

# Исключение #0: деление на ноль
isr0:
    cli                 # на всякий случай отключаем прерывания
    call isr0_handler   # C-обработчик (panic не возвращает)
.hang_exc0:
    hlt
    jmp .hang_exc0

# IRQ0: таймер
irq0:
    pusha               # сохраняем все общие регистры
    call irq0_handler   # C-обработчик для таймера
    popa                # восстанавливаем регистры
    iretd               # возвращаемся к прерванному коду

