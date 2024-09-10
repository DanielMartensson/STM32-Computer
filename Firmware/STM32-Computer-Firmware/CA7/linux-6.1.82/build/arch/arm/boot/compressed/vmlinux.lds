OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
  /DISCARD/ : {
    *(.discard) *(.discard.*) *(.modinfo) *(.gnu.version*)
    *(.ARM.exidx*)
    *(.ARM.extab*)
    *(.note.*)
    *(.rel.*)
    *(.printk_index)
    *(.data)
  }
  . = 0;
  _text = .;
  .text : {
    _start = .;
    *(.start)
    *(.text)
    *(.text.*)
    *(.gnu.warning) *(.glue_7) *(.glue_7t) *(.vfp11_veneer) *(.v4_bx)
  }
  .table : ALIGN(4) {
    _table_start = .;
    LONG((6))
    LONG((0x5a534c4b))
    LONG((__piggy_size_addr - _start))
    LONG((_kernel_bss_size))
    LONG((0x00008000))
    LONG((65536))
    LONG(0)
    _table_end = .;
  }
  .rodata : {
    *(.rodata)
    *(.rodata.*)
    *(.data.rel.ro)
    *(.data.rel.ro.*)
  }
  .piggydata : {
    *(.piggydata)
    __piggy_size_addr = . - 4;
  }
  . = ALIGN(4);
  _etext = .;
  .got.plt : { *(.got.plt) }
  .pad : { BYTE(0); . = ALIGN(8); }
  .data : ALIGN(4096) {
    __pecoff_data_start = .;
    _got_start = .;
    *(.got)
    _got_end = .;
    *(.data.efistub .bss.efistub)
    __pecoff_data_end = .;
    . = ALIGN(512);
  }
  __pecoff_data_rawsize = . - ADDR(.data);
  _edata = .;
  .image_end (NOLOAD) : {
    _edata_real = .;
  }
  _magic_sig = (0x016f2818);
  _magic_start = (_start);
  _magic_end = (_edata);
  _magic_table = (_table_start - _start);
  . = ALIGN(8);
  __bss_start = .;
  .bss : { *(.bss) }
  _end = .;
  . = ALIGN(8);
  .stack : { *(.stack) }
  PROVIDE(__pecoff_data_size = ALIGN(512) - ADDR(.data));
  PROVIDE(__pecoff_end = ALIGN(512));
  .stab 0 : { *(.stab) } .stabstr 0 : { *(.stabstr) } .stab.excl 0 : { *(.stab.excl) } .stab.exclstr 0 : { *(.stab.exclstr) } .stab.index 0 : { *(.stab.index) } .stab.indexstr 0 : { *(.stab.indexstr) }
  .debug 0 : { *(.debug) } .line 0 : { *(.line) } .debug_srcinfo 0 : { *(.debug_srcinfo) } .debug_sfnames 0 : { *(.debug_sfnames) } .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .debug_pubtypes 0 : { *(.debug_pubtypes) } .debug_ranges 0 : { *(.debug_ranges) } .debug_weaknames 0 : { *(.debug_weaknames) } .debug_funcnames 0 : { *(.debug_funcnames) } .debug_typenames 0 : { *(.debug_typenames) } .debug_varnames 0 : { *(.debug_varnames) } .debug_gnu_pubnames 0 : { *(.debug_gnu_pubnames) } .debug_gnu_pubtypes 0 : { *(.debug_gnu_pubtypes) } .debug_types 0 : { *(.debug_types) } .debug_addr 0 : { *(.debug_addr) } .debug_line_str 0 : { *(.debug_line_str) } .debug_loclists 0 : { *(.debug_loclists) } .debug_macro 0 : { *(.debug_macro) } .debug_names 0 : { *(.debug_names) } .debug_rnglists 0 : { *(.debug_rnglists) } .debug_str_offsets 0 : { *(.debug_str_offsets) }
  .comment 0 : { *(.comment) } .symtab 0 : { *(.symtab) } .strtab 0 : { *(.strtab) } .shstrtab 0 : { *(.shstrtab) } .ARM.attributes 0 : { *(.ARM.attributes) }
  .plt : { *(.iplt) *(.rel.iplt) *(.iplt) *(.igot.plt) } ASSERT(SIZEOF(.plt) == 0, "Unexpected run-time procedure linkages detected!")
}
ASSERT(_edata_real == _edata, "error: zImage file size is incorrect");
