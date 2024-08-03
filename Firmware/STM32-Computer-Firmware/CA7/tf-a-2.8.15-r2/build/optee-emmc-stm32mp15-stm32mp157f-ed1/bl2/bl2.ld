OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(bl2_entrypoint)
MEMORY {
    RAM (rwx): ORIGIN = ((0x2FFC0000) + ((0x00040000) - ((1) << (12))) - (0x00016000)), LENGTH = (((0x2FFC0000) + ((0x00040000) - ((1) << (12))) - (0x00016000)) + (0x00016000)) - ((0x2FFC0000) + ((0x00040000) - ((1) << (12))) - (0x00016000))
}
SECTIONS
{
    . = ((0x2FFC0000) + ((0x00040000) - ((1) << (12))) - (0x00016000));
    ASSERT(. == ALIGN(((1) << (12))),
           "BL2_BASE address is not aligned on a page boundary.")
    ro . : {
        __RO_START__ = .;
 __TEXT_RESIDENT_START__ = .;
 *bl2_el3_entrypoint.o(.text*)
 *(.text.asm.*)
 __TEXT_RESIDENT_END__ = .;
        *(SORT_BY_ALIGNMENT(.text*))
        *(SORT_BY_ALIGNMENT(.rodata*))
 . = ALIGN(4); __RT_SVC_DESCS_START__ = .; KEEP(*(rt_svc_descs)) __RT_SVC_DESCS_END__ = .; . = ALIGN(4); __FCONF_POPULATOR_START__ = .; KEEP(*(.fconf_populator)) __FCONF_POPULATOR_END__ = .; . = ALIGN(4); __PMF_SVC_DESCS_START__ = .; KEEP(*(pmf_svc_descs)) __PMF_SVC_DESCS_END__ = .; . = ALIGN(4); __PARSER_LIB_DESCS_START__ = .; KEEP(*(.img_parser_lib_descs)) __PARSER_LIB_DESCS_END__ = .; . = ALIGN(4); __CPU_OPS_START__ = .; KEEP(*(cpu_ops)) __CPU_OPS_END__ = .; . = ALIGN(4); __GOT_START__ = .; *(.got) __GOT_END__ = .;
        *(.vectors)
        __RO_END_UNALIGNED__ = .;
        . = ALIGN(((1) << (12)));
        __RO_END__ = .;
    } >RAM
    ASSERT(__CPU_OPS_END__ > __CPU_OPS_START__,
          "cpu_ops not defined for this platform.")
    __RW_START__ = . ;
    .data . : ALIGN(1) { __DATA_START__ = .; *(SORT_BY_ALIGNMENT(.data*)) . = ALIGN(2); __DATA_END__ = .; } >RAM AT>RAM
    __DATA_RAM_START__ = __DATA_START__;
    __DATA_RAM_END__ = __DATA_END__;
    .rel.dyn : ALIGN(4) { __RELA_START__ = .; *(.rel*) __RELA_END__ = .; } >RAM
    stacks (NOLOAD) : { __STACKS_START__ = .; *(tzfw_normal_stacks) __STACKS_END__ = .; } >RAM
    .bss (NOLOAD) : ALIGN(8) { __BSS_START__ = .; *(SORT_BY_ALIGNMENT(.bss*)) *(COMMON) . = ALIGN(((1) << 6)); __BAKERY_LOCK_START__ = .; __PERCPU_BAKERY_LOCK_START__ = .; *(bakery_lock) . = ALIGN(((1) << 6)); __PERCPU_BAKERY_LOCK_END__ = .; __PERCPU_BAKERY_LOCK_SIZE__ = ABSOLUTE(__PERCPU_BAKERY_LOCK_END__ - __PERCPU_BAKERY_LOCK_START__); . = . + (__PERCPU_BAKERY_LOCK_SIZE__ * (((0) + (2)) - 1)); __BAKERY_LOCK_END__ = .; . = ALIGN(((1) << 6)); __PMF_TIMESTAMP_START__ = .; KEEP(*(pmf_timestamp_array)) . = ALIGN(((1) << 6)); __PMF_PERCPU_TIMESTAMP_END__ = .; __PERCPU_TIMESTAMP_SIZE__ = ABSOLUTE(. - __PMF_TIMESTAMP_START__); . = . + (__PERCPU_TIMESTAMP_SIZE__ * (((0) + (2)) - 1)); __PMF_TIMESTAMP_END__ = .; . = ALIGN(16); __BASE_XLAT_TABLE_START__ = .; *(base_xlat_table) __BASE_XLAT_TABLE_END__ = .; __BSS_END__ = .; } >RAM
    xlat_table (NOLOAD) : { __XLAT_TABLE_START__ = .; *(xlat_table) __XLAT_TABLE_END__ = .; } >RAM
    __RW_END__ = .;
    __BL2_END__ = .;
    /DISCARD/ : {
        *(.dynsym .dynstr .hash .gnu.hash)
    }
    __BSS_SIZE__ = SIZEOF(.bss);
    ASSERT(. <= (((0x2FFC0000) + ((0x00040000) - ((1) << (12))) - (0x00016000)) + (0x00016000)), "BL2 image has exceeded its limit.")
}
