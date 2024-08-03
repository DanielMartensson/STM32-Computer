OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__BL2_IMAGE_START__)
MEMORY {
 HEADER (rw) : ORIGIN = 0x00000000, LENGTH = (0x3000)
 RAM (rwx) : ORIGIN = ((0x2FFC0000) + (0x00002400) + (0x00000100)), LENGTH = (((0x00040000) - ((1) << (12))) - ((0x00002400) + (0x00000100)))
}
SECTIONS
{
    .header : {
        __HEADER_START__ = .;
        KEEP(*(.header))
        . = ALIGN(4);
        __HEADER_END__ = .;
    } >HEADER
    . = ((0x2FFC0000) + (0x00002400) + (0x00000100));
    .data . : {
        . = ALIGN(((1) << (12)));
        __DATA_START__ = .;
        *(.data*)
        . = ( (((0x2FFC0000) + ((0x00040000) - ((1) << (12))) - (0x00016000)) - (0x00007000)) - ((0x2FFC0000) + (0x00002400) + (0x00000100)) );
        __DTB_IMAGE_START__ = .;
        *(.dtb_image*)
        __DTB_IMAGE_END__ = .;
        . = ( ((0x2FFC0000) + ((0x00040000) - ((1) << (12))) - (0x00016000)) - ((0x2FFC0000) + (0x00002400) + (0x00000100)) );
        __BL2_IMAGE_START__ = .;
        *(.bl2_image*)
        __BL2_IMAGE_END__ = .;
        __DATA_END__ = .;
    } >RAM
    __TF_END__ = .;
}
