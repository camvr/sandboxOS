/* Declare constants for multiboot header */
.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)

/* Declare a multiboot header to mark the programas a kernel */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Define the stack pointer and allocate 16 KiB for it */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* Linker specifies _start as the entry point to the kernel */
.section .text
.global _start
.type _start, @function
_start:
    /* Setup the stack */
    mov $stack_top, %esp

    /* Load in other features here such as GDT, paging, etc. */

    /* Enter in the high-level kernel */
    call kernel_main

    /* Nothing more to do, put computer into an infinite loop */
    cli
1:  hlt
    jmp 1b

/* Set the size of the _start symbol to the current location minus its start */
.size _start, . - _start
