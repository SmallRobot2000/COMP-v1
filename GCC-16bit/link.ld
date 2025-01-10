ENTRY(start)
SECTIONS
{
    . = 0x0000;  /* Set the entry point for the bootloader */

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}
