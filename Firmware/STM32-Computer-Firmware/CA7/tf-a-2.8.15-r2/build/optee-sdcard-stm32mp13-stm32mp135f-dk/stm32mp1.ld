OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__BL2_IMAGE_START__)
MEMORY {
 HEADER (rw) : ORIGIN = 0x00000000, LENGTH = (0x200)
 RAM (rwx) : ORIGIN = (0x2FFE0000), LENGTH = ((0x00020000) - ((1) << (12)))
}
SECTIONS
{
    .header : {
        __HEADER_START__ = .;
        KEEP(*(.header))
        . = ALIGN(4);
        __HEADER_END__ = .;
    } >HEADER
    . = (0x2FFE0000);
    .data . : {
        . = ALIGN(((1) << (12)));
        __DATA_START__ = .;
        *(.data*)
        . = ( (0x2FFE0000) - (0x2FFE0000) );
        __DTB_IMAGE_START__ = .;
        *(.dtb_image*)
        __DTB_IMAGE_END__ = .;
        . = ( ((0x2FFE0000) + (0x00004000)) - (0x2FFE0000) );
        __BL2_IMAGE_START__ = .;
        *(.bl2_image*)
        __BL2_IMAGE_END__ = .;
        __DATA_END__ = .;
    } >RAM
    __TF_END__ = .;
}
