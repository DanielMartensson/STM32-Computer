cmd_cmd/fastboot.o := arm-ostl-linux-gnueabi-gcc -Wp,-MD,cmd/.fastboot.o.d -nostdinc -isystem /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include -Iinclude  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include    -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include -include /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/kconfig.h  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/cmd -Icmd -D__KERNEL__ -D__UBOOT__ -Wall -Wstrict-prototypes -Wno-format-security -fno-builtin -ffreestanding -std=gnu11 -fshort-wchar -fno-strict-aliasing -fno-PIE -Os -fno-stack-protector -fno-delete-null-pointer-checks -Wno-pointer-sign -Wno-stringop-truncation -Wno-zero-length-bounds -Wno-array-bounds -Wno-stringop-overflow -Wno-maybe-uninitialized -fmacro-prefix-map=/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/= -g -fstack-usage -Wno-format-nonliteral -Wno-address-of-packed-member -Wno-unused-but-set-variable -Werror=date-time -Wno-packed-not-aligned --sysroot=/home/lubuntu/Program/STM32/SDK/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi -D__ARM__ -Wa,-mimplicit-it=always -mthumb -mthumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -mgeneral-regs-only -pipe -march=armv7-a -D__LINUX_ARM_ARCH__=7 -mtune=generic-armv7-a  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/mach-stm32mp/include    -DKBUILD_BASENAME='"fastboot"'  -DKBUILD_MODNAME='"fastboot"' -c -o cmd/fastboot.o /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/cmd/fastboot.c

source_cmd/fastboot.o := /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/cmd/fastboot.c

deps_cmd/fastboot.o := \
    $(wildcard include/config/udp/function/fastboot.h) \
    $(wildcard include/config/usb/function/fastboot.h) \
    $(wildcard include/config/sys/longhelp.h) \
    $(wildcard include/config/fastboot/buf/addr.h) \
    $(wildcard include/config/fastboot/buf/size.h) \
    $(wildcard include/config/sys/maxargs.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/kconfig.h \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
    $(wildcard include/config/spl/.h) \
    $(wildcard include/config/tpl/.h) \
    $(wildcard include/config/tools/.h) \
    $(wildcard include/config/tpl/build.h) \
    $(wildcard include/config/vpl/build.h) \
    $(wildcard include/config/spl/build.h) \
    $(wildcard include/config/tools/foo.h) \
    $(wildcard include/config/spl/foo.h) \
    $(wildcard include/config/tpl/foo.h) \
    $(wildcard include/config/vpl/foo.h) \
    $(wildcard include/config/option.h) \
    $(wildcard include/config/acme.h) \
    $(wildcard include/config/spl/acme.h) \
    $(wildcard include/config/tpl/acme.h) \
    $(wildcard include/config/if/enabled/int.h) \
    $(wildcard include/config/int/option.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/common.h \
  include/config.h \
    $(wildcard include/config/boarddir.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/config_uncmd_spl.h \
    $(wildcard include/config/spl/dm.h) \
    $(wildcard include/config/dm/serial.h) \
    $(wildcard include/config/dm/i2c.h) \
    $(wildcard include/config/dm/stdio.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/configs/stm32mp15_st_common.h \
    $(wildcard include/config/sys/baudrate/table.h) \
    $(wildcard include/config/extra/env/settings.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/configs/stm32mp15_common.h \
    $(wildcard include/config/sys/sdram/base.h) \
    $(wildcard include/config/sys/bootmapsz.h) \
    $(wildcard include/config/sys/mmc/max/device.h) \
    $(wildcard include/config/sys/max/nand/device.h) \
    $(wildcard include/config/dwc/eth/qos.h) \
    $(wildcard include/config/serverip.h) \
    $(wildcard include/config/distro/defaults.h) \
    $(wildcard include/config/cmd/mmc.h) \
    $(wildcard include/config/net.h) \
    $(wildcard include/config/cmd/ubifs.h) \
    $(wildcard include/config/cmd/usb.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/sizes.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/const.h \
  include/asm/arch/stm32.h \
    $(wildcard include/config/stm32mp15x.h) \
    $(wildcard include/config/stm32mp13x.h) \
    $(wildcard include/config/stm32mp25x.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/bitops.h \
    $(wildcard include/config/sandbox.h) \
    $(wildcard include/config/sandbox/bits/per/long.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/types.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/dma/addr/t/64bit.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/int-ll64.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/bitsperlong.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/kasan.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/cc/has/asm/inline.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/compiler_attributes.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/compiler-gcc.h \
    $(wildcard include/config/retpoline.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/posix_types.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/stddef.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/posix_types.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stdbool.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/kernel.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/printk.h \
    $(wildcard include/config/loglevel.h) \
    $(wildcard include/config/log.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/log.h \
    $(wildcard include/config/log/max/level.h) \
    $(wildcard include/config/panic/hang.h) \
    $(wildcard include/config/log/error/return.h) \
    $(wildcard include/config/logf/file.h) \
    $(wildcard include/config/logf/line.h) \
    $(wildcard include/config/logf/func.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/stdio.h \
    $(wildcard include/config/tpl/serial.h) \
    $(wildcard include/config/spl/serial.h) \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stdarg.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linker_lists.h \
    $(wildcard include/config/linker/list/align.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dm/uclass-id.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/list.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/poison.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/bitops.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/bitops/__ffs.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/bitops/__fls.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/bitops/fls.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/bitops/fls64.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/proc-armv/system.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/config_distro_bootcmd.h \
    $(wildcard include/config/efi/loader.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/x86/run/32bit.h) \
    $(wildcard include/config/x86/run/64bit.h) \
    $(wildcard include/config/arch/rv32i.h) \
    $(wildcard include/config/arch/rv64i.h) \
    $(wildcard include/config/cmd/bootefi/bootmgr.h) \
    $(wildcard include/config/sata.h) \
    $(wildcard include/config/nvme.h) \
    $(wildcard include/config/scsi.h) \
    $(wildcard include/config/ide.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/cmd/virtio.h) \
    $(wildcard include/config/cmd/dhcp.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/cmd/pxe.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/config.h \
    $(wildcard include/config/arch/ls1021a.h) \
    $(wildcard include/config/fsl/layerscape.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/kconfig.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/config_fallbacks.h \
    $(wildcard include/config/spl/pad/to.h) \
    $(wildcard include/config/spl/max/size.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/errno.h \
    $(wildcard include/config/errno/str.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/errno.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/time.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/typecheck.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/string.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/string.h \
    $(wildcard include/config/use/arch/memcpy.h) \
    $(wildcard include/config/use/arch/memmove.h) \
    $(wildcard include/config/use/arch/memset.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/linux_string.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/u-boot.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/u-boot.h \
    $(wildcard include/config/mpc8xx.h) \
    $(wildcard include/config/e500.h) \
    $(wildcard include/config/mpc86xx.h) \
    $(wildcard include/config/m68k.h) \
    $(wildcard include/config/mpc83xx.h) \
    $(wildcard include/config/extra/clock.h) \
    $(wildcard include/config/nr/dram/banks.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/u-boot-arm.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/vsprintf.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/command.h \
    $(wildcard include/config/sys/help/cmd/width.h) \
    $(wildcard include/config/auto/complete.h) \
    $(wildcard include/config/cmd/run.h) \
    $(wildcard include/config/cmd/memory.h) \
    $(wildcard include/config/cmd/i2c.h) \
    $(wildcard include/config/cmd/itest.h) \
    $(wildcard include/config/cmd/pci.h) \
    $(wildcard include/config/cmd/setexpr.h) \
    $(wildcard include/config/cmd/bootd.h) \
    $(wildcard include/config/cmd/bootm.h) \
    $(wildcard include/config/cmd/nvedit/efi.h) \
    $(wildcard include/config/cmdline.h) \
    $(wildcard include/config/needs/manual/reloc.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/env.h \
    $(wildcard include/config/env/import/fdt.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stddef.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/byteorder.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/byteorder/little_endian.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/byteorder/swab.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/byteorder/generic.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/console.h \
    $(wildcard include/config/console/record.h) \
    $(wildcard include/config/console/mux.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/stdio_dev.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/iomux.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/g_dnl.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/usb/ch9.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/unaligned.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/unaligned/le_byteshift.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/unaligned/be_byteshift.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/unaligned/generic.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/usb/gadget.h \
    $(wildcard include/config/usb/gadget/dualspeed.h) \
    $(wildcard include/config/usb/otg.h) \
    $(wildcard include/config/dm/usb/gadget.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/usb.h \
    $(wildcard include/config/dm/usb.h) \
    $(wildcard include/config/usb/musb/host.h) \
    $(wildcard include/config/usb/ehci/hcd.h) \
    $(wildcard include/config/usb/storage.h) \
    $(wildcard include/config/usb/host/ether.h) \
    $(wildcard include/config/usb/keyboard.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/fdtdec.h \
    $(wildcard include/config/of/embed.h) \
    $(wildcard include/config/of/board.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/libfdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/libfdt_env.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/../../scripts/dtc/libfdt/libfdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/../../scripts/dtc/libfdt/libfdt_env.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/../../scripts/dtc/libfdt/fdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/pci.h \
    $(wildcard include/config/sys/pci/64bit.h) \
    $(wildcard include/config/dm/pci/compat.h) \
    $(wildcard include/config/mpc85xx.h) \
    $(wildcard include/config/pci/sriov.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/pci_ids.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dm/pci.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/usb_defs.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/cache.h \
    $(wildcard include/config/sys/thumb/build.h) \
    $(wildcard include/config/sys/cacheline/size.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/system.h \
    $(wildcard include/config/armv8/psci.h) \
    $(wildcard include/config/armv7/lpae.h) \
    $(wildcard include/config/cpu/v7a.h) \
    $(wildcard include/config/armv7/psci.h) \
    $(wildcard include/config/sys/arm/cache/writethrough.h) \
    $(wildcard include/config/sys/arm/cache/writealloc.h) \
    $(wildcard include/config/sys/arm/cache/writeback.h) \
    $(wildcard include/config/sys/noncached/memory.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/barriers.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/part.h \
    $(wildcard include/config/partition/uuids.h) \
    $(wildcard include/config/partition/type/guid.h) \
    $(wildcard include/config/dos/partition.h) \
    $(wildcard include/config/partitions.h) \
    $(wildcard include/config/spl/fs/ext4.h) \
    $(wildcard include/config/spl/fs/fat.h) \
    $(wildcard include/config/sys/mmcsd/raw/mode/u/boot/partition.h) \
    $(wildcard include/config/efi/partition.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/blk.h \
    $(wildcard include/config/sys/64bit/lba.h) \
    $(wildcard include/config/blk.h) \
    $(wildcard include/config/lba48.h) \
    $(wildcard include/config/block/cache.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/efi.h \
    $(wildcard include/config/efi/stub/64bit.h) \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/efi/ram/size.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/linkage.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/linkage.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/ide.h \
    $(wildcard include/config/sys/ide/maxdevice.h) \
    $(wildcard include/config/sys/ide/maxbus.h) \
    $(wildcard include/config/sys/ata/base/addr.h) \
    $(wildcard include/config/of/ide/fixup.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/uuid.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/part_efi.h \
    $(wildcard include/config/efi/partition/entries/numbers.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/compat.h \
    $(wildcard include/config/xen.h) \
    $(wildcard include/config/lbdaf.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/malloc.h \
    $(wildcard include/config/sys/malloc/simple.h) \
    $(wildcard include/config/valgrind.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/processor.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/smp.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/atomic.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/ptrace.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/proc-armv/ptrace.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/proc-armv/processor.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include/asm/proc-armv/domain.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/err.h \
    $(wildcard include/config/err/ptr/offset.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/usb/composite.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/bitmap.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/fastboot.h \
    $(wildcard include/config/fastboot/flash.h) \
    $(wildcard include/config/fastboot/cmd/oem/format.h) \
    $(wildcard include/config/fastboot/cmd/oem/partconf.h) \
    $(wildcard include/config/fastboot/cmd/oem/bootbus.h) \
    $(wildcard include/config/fastboot/uuu/support.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/net.h \
    $(wildcard include/config/sys/rx/eth/buffer.h) \
    $(wildcard include/config/dm/eth.h) \
    $(wildcard include/config/api.h) \
    $(wildcard include/config/dm/dsa.h) \
    $(wildcard include/config/bootp/dns2.h) \
    $(wildcard include/config/bootp/max/root/path/len.h) \
    $(wildcard include/config/cmd/dns.h) \
    $(wildcard include/config/cmd/ping.h) \
    $(wildcard include/config/cmd/cdp.h) \
    $(wildcard include/config/cmd/sntp.h) \
    $(wildcard include/config/netconsole.h) \
    $(wildcard include/config/reset/phy/r.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/if_ether.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/rand.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/watchdog.h \
    $(wildcard include/config/watchdog.h) \
    $(wildcard include/config/hw/watchdog.h) \
    $(wildcard include/config/spl/watchdog.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/stringify.h \

cmd/fastboot.o: $(deps_cmd/fastboot.o)

$(deps_cmd/fastboot.o):