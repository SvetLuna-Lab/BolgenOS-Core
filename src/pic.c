// src/pic.c

#include "pic.h"
#include "io.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

#define PIC_EOI   0x20

void pic_remap(void) {
    // Сохраняем старые маски (формально можно и не сохранять, но это аккуратнее)
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    // Инициализация обоих PIC (ICW1)
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);

    // Векторы (ICW2): master → 0x20, slave → 0x28
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    // Связь мастер–слейв (ICW3)
    outb(PIC1_DATA, 0x04); // slave на IRQ2
    outb(PIC2_DATA, 0x02); // номер линии master'а

    // Режим (ICW4)
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Маски: включаем только IRQ0, остальные глушим
    // 1111 1110b = 0xFE → разрешён только бит 0
    outb(PIC1_DATA, 0xFE);
    outb(PIC2_DATA, 0xFF);

    // (по желанию можно восстановить a1/a2 и потом вручную настраивать)
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_CMD, PIC_EOI);
    }
    outb(PIC1_CMD, PIC_EOI);
}
