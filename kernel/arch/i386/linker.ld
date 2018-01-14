/* Designate the entry point for the bootloader to start execution at */
ENTRY(_start)

/* Define where sections of the kernel will be put in the executable */
SECTIONS
{
    /* Begin putting sections at 1 Megabyte */
    . = 1M;

    /* Define multiboot header and the .text section */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Initialized read-write data */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* Uninitialized read-write data and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}