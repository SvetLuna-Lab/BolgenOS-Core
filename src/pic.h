// src/pic.h
#ifndef BOLGENOS_PIC_H
#define BOLGENOS_PIC_H

#include <stdint.h>

/* Ремап ПК-контроллеров прерываний PIC на векторы 0x20–0x2F. */
void pic_remap(void);

/* Отправить EOI (End Of Interrupt) для IRQ [0..15]. */
void pic_send_eoi(uint8_t irq);

#endif // BOLGENOS_PIC_H
