OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
 . = ((0xc0000000 + 0x20000000 - 0x02000000) + ((0xc0000000 + 0x20000000 - 0x02000000) - (0xc0000000 + 0x20000000 - 0x02000000)));
 ASSERT(!(((0xc0000000 + 0x20000000 - 0x02000000) + ((0xc0000000 + 0x20000000 - 0x02000000) - (0xc0000000 + 0x20000000 - 0x02000000))) & 31), "text start should align to 32bytes")
 __text_start = .;
 __flatmap_unpg_rx_start = ((__text_start) - ((__text_start) % ((1 << (12)))));
 .text : {
  KEEP(*(.text._start))
  __identity_map_init_start = .;
  __text_data_start = .;
  *(.identity_map.data)
  __text_data_end = .;
  *(.identity_map .identity_map.*
    .text.get_core_pos_mpidr)
  __identity_map_init_end = .;
  KEEP(*(.text.init .text.plat_cpu_reset_early .text.reset .text.reset_primary .text.unhandled_cpu .text.__assert_flat_mapped_range))
  *(.text .text.*)
  *(.sram.text.glue_7* .gnu.linkonce.t.*)
  . = ALIGN(8);
 }
 __text_end = .;
 __flatmap_unpg_rx_size = . - __flatmap_unpg_rx_start;
 __flatmap_unpg_ro_start = .;
 .rodata : ALIGN(8) {
  __rodata_start = .;
  *(.gnu.linkonce.r.*)
  *(.rodata .rodata.*)
  . = ALIGN(8);
  KEEP(*(SORT(.scattered_array*)));
  . = ALIGN(8);
  __rodata_end = .;
 }
 .got : { *(.got.plt) *(.got) }
 .note.gnu.property : { *(.note.gnu.property) }
 .plt : { *(.plt) }
 .ctors : ALIGN(8) {
  __ctor_list = .;
  KEEP(*(.ctors .ctors.* .init_array .init_array.*))
  __ctor_end = .;
 }
 .dtors : ALIGN(8) {
  __dtor_list = .;
  KEEP(*(.dtors .dtors.* .fini_array .fini_array.*))
  __dtor_end = .;
 }
 .ARM.exidx : {
  __exidx_start = .;
  *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  __exidx_end = .;
 }
 .ARM.extab : {
  __extab_start = .;
  *(.ARM.extab*)
  __extab_end = .;
 }
 . = ALIGN((1 << (12)));
 __flatmap_unpg_ro_size = . - __flatmap_unpg_ro_start;
 __flatmap_unpg_rw_start = .;
 .data : ALIGN(8) {
  __data_start_rom = .;
  __data_start = .;
  *(.data .data.* .gnu.linkonce.d.*)
  . = ALIGN(8);
 }
 .bss : {
  __data_end = .;
  __bss_start = .;
  *(.bss .bss.*)
  *(.gnu.linkonce.b.*)
  *(COMMON)
  . = ALIGN(8);
  __bss_end = .;
 }
 .heap1 (NOLOAD) : {
  __heap1_start = .;
  . += 65536;
  . = ALIGN(4 * 1024);
  __heap1_end = .;
 }
 .nozi (NOLOAD) : {
  __nozi_start = .;
  KEEP(*(.nozi .nozi.*))
  . = ALIGN(16);
  __nozi_end = .;
  __nozi_stack_start = .;
  KEEP(*(.nozi_stack .nozi_stack.*))
  . = ALIGN(8);
  __nozi_stack_end = .;
 }
 __end = .;
 __init_size = __data_end - ((0xc0000000 + 0x20000000 - 0x02000000) + ((0xc0000000 + 0x20000000 - 0x02000000) - (0xc0000000 + 0x20000000 - 0x02000000)));
 ASSERT(. <= ((0xc0000000 + 0x20000000 - 0x02000000) + (1 << (21))),
  "TEE_RAM_VA_SIZE is too small")
 . = (0xc0000000 + 0x20000000 - 0x02000000) + (1 << (21));
 _end_of_ram = .;
 __flatmap_unpg_rw_size = _end_of_ram - __flatmap_unpg_rw_start;
 __get_tee_init_end = .;
 .dynamic : { *(.dynamic) }
 .hash : { *(.hash) }
 .dynsym : { *(.dynsym) }
 .dynstr : { *(.dynstr) }
 .rel : {
  *(.rel.*)
 }
 .rela : {
  *(.rela.*)
 }
 ASSERT(SIZEOF(.rel) == 0, "Relocation entries not expected")
 ASSERT(SIZEOF(.rela) == 0, "Relocation entries not expected")
 /DISCARD/ : {
  *(.comment .note .eh_frame .interp)
  *(__keep_meta_vars*)
 }
}
__vcore_unpg_rx_start = __flatmap_unpg_rx_start;
__vcore_unpg_ro_start = __flatmap_unpg_ro_start;
__vcore_unpg_rx_size = __flatmap_unpg_rx_size + __flatmap_unpg_ro_size;
__vcore_unpg_ro_size = 0;
__vcore_unpg_rx_end = __vcore_unpg_rx_start + __vcore_unpg_rx_size;
__vcore_unpg_ro_end = __vcore_unpg_ro_start + __vcore_unpg_ro_size;
__vcore_unpg_rw_start = __flatmap_unpg_rw_start;
__vcore_unpg_rw_size = __flatmap_unpg_rw_size;
__vcore_unpg_rw_end = __vcore_unpg_rw_start + __vcore_unpg_rw_size;
