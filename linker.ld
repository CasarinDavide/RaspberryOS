SECTIONS
{
    . = 0x80000;     /* Kernel load address for AArch64 */
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }
    PROVIDE(_data = .);
    .data : { *(.data .data.* .gnu.linkonce.d*) }
     .bss : {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
        __stack_top = 0x80000;
        __bss_size = __bss_end - __bss_start;
    }


    _end = .;

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>5;
