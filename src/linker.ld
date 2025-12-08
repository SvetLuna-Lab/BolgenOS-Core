/* src/linker.ld */

ENTRY(_start)

SECTIONS
{
    /* Загружаем ядро по адресу 1 МБ */
    . = 1M;

    .text :
    {
        *(.multiboot)
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data*)
    }

    .bss :
    {
        *(.bss*)
        *(COMMON)
    }
}
