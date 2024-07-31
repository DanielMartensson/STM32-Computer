cmd_tools/envcrc.o := cc -Wp,-MD,tools/.envcrc.o.d -Itools -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11 -DCONFIG_FIT_SIGNATURE -DCONFIG_FIT_SIGNATURE_MAX_SIZE=0xffffffff -DCONFIG_FIT_CIPHER -include /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h -idirafterinclude -idirafter/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include -idirafter/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/scripts/dtc/libfdt  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools -DUSE_HOSTCC -D__KERNEL_STRICT_NAMES -D_GNU_SOURCE -c -o tools/envcrc.o /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/envcrc.c

source_tools/envcrc.o := /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/envcrc.c

deps_tools/envcrc.o := \
    $(wildcard include/config/env/is/in/flash.h) \
    $(wildcard include/config/env/addr.h) \
    $(wildcard include/config/sys/flash/base.h) \
    $(wildcard include/config/env/offset.h) \
    $(wildcard include/config/env/addr/redund.h) \
    $(wildcard include/config/env/offset/redund.h) \
    $(wildcard include/config/env/size.h) \
    $(wildcard include/config/env/sect/size.h) \
    $(wildcard include/config/sys/monitor/base.h) \
    $(wildcard include/config/sys/monitor/len.h) \
    $(wildcard include/config/build/envcrc.h) \
    $(wildcard include/config/sys/redundand/environment.h) \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/stdc-predef.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stddef.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdbool.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdint.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/stdint.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/libc-header-start.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/features.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/features-time64.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/wordsize.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/timesize.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/cdefs.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/long-double.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/gnu/stubs.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/gnu/stubs-64.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/typesizes.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/time64.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/wchar.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdint-intn.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdint-uintn.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/errno.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/errno.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/errno.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/asm/errno.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/asm-generic/errno.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/asm-generic/errno-base.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/error_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/stdlib.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/waitflags.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/waitstatus.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/floatn.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/floatn-common.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/locale_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__locale_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/types.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/clock_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/clockid_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/time_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/timer_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/endian.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/endian.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/endianness.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/byteswap.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/uintn-identity.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/select.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/select.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/sigset_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__sigset_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_timeval.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_timespec.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/pthreadtypes.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/thread-shared-types.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/pthreadtypes-arch.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/atomic_wide_counter.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_mutex.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_rwlock.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/alloca.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdlib-bsearch.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdlib-float.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/stdio.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdarg.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__fpos_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__mbstate_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__fpos64_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__FILE.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/FILE.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_FILE.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/cookie_io_functions_t.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdio_lim.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdio.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/string.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/strings.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/mman.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-map-flags-generic.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-linux.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-shared.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman_ext.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/fcntl.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/fcntl.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/fcntl-linux.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_iovec.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/falloc.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stat.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_stat.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/byteswap.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/time.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/time.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/timex.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_tm.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_itimerspec.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/u-boot/crc.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/unistd.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/posix_opt.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/environments.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/confname.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/getopt_posix.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/getopt_core.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/unistd_ext.h \
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/close_range.h \
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
  /home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/const.h \
  include/asm/arch/stm32.h \
    $(wildcard include/config/stm32mp15x.h) \
    $(wildcard include/config/stm32mp13x.h) \
    $(wildcard include/config/stm32mp25x.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/config_distro_bootcmd.h \
    $(wildcard include/config/sandbox.h) \
    $(wildcard include/config/efi/loader.h) \
    $(wildcard include/config/arm64.h) \
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
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/config_fallbacks.h \
    $(wildcard include/config/spl/pad/to.h) \
    $(wildcard include/config/spl/max/size.h) \

tools/envcrc.o: $(deps_tools/envcrc.o)

$(deps_tools/envcrc.o):
