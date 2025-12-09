ENTRY(start)

SECTIONS
{
    /* Загружаем ядро по адресу 1 Мбайт */
    . = 1M;

    /* Секция загрузчика / multiboot (если есть) */
    .multiboot :
    {
        KEEP(*(.multiboot))
    }

    /* Код */
    .text :
    {
        *(.text*)
    }

    /* Константные данные */
    .rodata :
    {
        *(.rodata*)
    }

    /* Инициализированные данные */
    .data :
    {
        *(.data*)
    }

    /* Неинициализированные данные (BSS) */
    .bss :
    {
        *(.bss*)
        *(COMMON)
    }
}
