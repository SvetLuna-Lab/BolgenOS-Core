.intel_syntax noprefix
.code32

/* Simple ISR/IRQ stubs for BolgenOS-Core
 * GAS (GNU as) syntax, Intel mode.
 * Each stub just returns from interrupt with IRET,
 * so that all IDT entries have valid targets.
 */

/* CPU exceptions: isr0..isr31 */

.global isr0
isr0:
    iret

.global isr1
isr1:
    iret

.global isr2
isr2:
    iret

.global isr3
isr3:
    iret

.global isr4
isr4:
    iret

.global isr5
isr5:
    iret

.global isr6
isr6:
    iret

.global isr7
isr7:
    iret

.global isr8
isr8:
    iret

.global isr9
isr9:
    iret

.global isr10
isr10:
    iret

.global isr11
isr11:
    iret

.global isr12
isr12:
    iret

.global isr13
isr13:
    iret

.global isr14
isr14:
    iret

.global isr15
isr15:
    iret

.global isr16
isr16:
    iret

.global isr17
isr17:
    iret

.global isr18
isr18:
    iret

.global isr19
isr19:
    iret

.global isr20
isr20:
    iret

.global isr21
isr21:
    iret

.global isr22
isr22:
    iret

.global isr23
isr23:
    iret

.global isr24
isr24:
    iret

.global isr25
isr25:
    iret

.global isr26
isr26:
    iret

.global isr27
isr27:
    iret

.global isr28
isr28:
    iret

.global isr29
isr29:
    iret

.global isr30
isr30:
    iret

.global isr31
isr31:
    iret

/* Hardware IRQs (remapped PIC): irq0..irq15 */

.global irq0
irq0:
    iret

.global irq1
irq1:
    iret

.global irq2
irq2:
    iret

.global irq3
irq3:
    iret

.global irq4
irq4:
    iret

.global irq5
irq5:
    iret

.global irq6
irq6:
    iret

.global irq7
irq7:
    iret

.global irq8
irq8:
    iret

.global irq9
irq9:
    iret

.global irq10
irq10:
    iret

.global irq11
irq11:
    iret

.global irq12
irq12:
    iret

.global irq13
irq13:
    iret

.global irq14
irq14:
    iret

.global irq15
irq15:
    iret
