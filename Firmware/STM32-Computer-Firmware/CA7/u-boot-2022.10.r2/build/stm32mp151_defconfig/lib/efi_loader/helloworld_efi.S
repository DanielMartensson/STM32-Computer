.section .rodata.helloworld.init,"a"
.balign 16
.global __efi_helloworld_begin
__efi_helloworld_begin:
.incbin "lib/efi_loader/helloworld.efi" 
__efi_helloworld_end:
.global __efi_helloworld_end
.balign 16
