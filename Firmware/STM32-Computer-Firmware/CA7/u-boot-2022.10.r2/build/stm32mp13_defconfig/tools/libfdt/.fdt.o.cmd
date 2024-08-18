cmd_tools/libfdt/fdt.o := cc -Wp,-MD,tools/libfdt/.fdt.o.d -Itools -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11 -DCONFIG_FIT_SIGNATURE -DCONFIG_FIT_SIGNATURE_MAX_SIZE=0xffffffff -DCONFIG_FIT_CIPHER -include /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h -idirafterinclude -idirafter/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include -idirafter/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/scripts/dtc/libfdt  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools -DUSE_HOSTCC -D__KERNEL_STRICT_NAMES -D_GNU_SOURCE -c -o tools/libfdt/fdt.o /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/libfdt/fdt.c

source_tools/libfdt/fdt.o := /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/libfdt/fdt.c

deps_tools/libfdt/fdt.o := \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/stdc-predef.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stddef.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdbool.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdint.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/stdint.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/libc-header-start.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/features.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/features-time64.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/wordsize.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/timesize.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/cdefs.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/long-double.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/gnu/stubs.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/gnu/stubs-64.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/typesizes.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/time64.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/wchar.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdint-intn.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdint-uintn.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/errno.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/errno.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/errno.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/asm/errno.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/asm-generic/errno.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/asm-generic/errno-base.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/error_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/stdlib.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/waitflags.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/waitstatus.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/floatn.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/floatn-common.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/locale_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__locale_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/types.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/clock_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/clockid_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/time_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/timer_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/endian.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/endian.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/endianness.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/byteswap.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/uintn-identity.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/select.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/select.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/sigset_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__sigset_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_timeval.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_timespec.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/pthreadtypes.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/thread-shared-types.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/pthreadtypes-arch.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/atomic_wide_counter.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_mutex.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_rwlock.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/alloca.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdlib-bsearch.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdlib-float.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/stdio.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdarg.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__fpos_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__mbstate_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__fpos64_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__FILE.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/FILE.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_FILE.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/cookie_io_functions_t.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdio_lim.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdio.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/string.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/strings.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/mman.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-map-flags-generic.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-linux.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-shared.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman_ext.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/fcntl.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/fcntl.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/fcntl-linux.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_iovec.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/falloc.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stat.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_stat.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/byteswap.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/time.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/time.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/timex.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_tm.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_itimerspec.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/fdt_host.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../include/linux/libfdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../include/linux/../../scripts/dtc/libfdt/libfdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../include/linux/../../scripts/dtc/libfdt/libfdt_env.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/limits.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include-fixed/limits.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/posix1_lim.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/local_lim.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/limits.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/pthread_stack_min-dynamic.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/posix2_lim.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/xopen_lim.h \
  /home/lubuntu/Program/STM32/STM32MP/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/uio_lim.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../include/linux/../../scripts/dtc/libfdt/fdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../include/fdt_support.h \
    $(wildcard include/config/of/libfdt.h) \
    $(wildcard include/config/arch/fixup/fdt/memory.h) \
    $(wildcard include/config/usb/ehci/fsl.h) \
    $(wildcard include/config/usb/xhci/fsl.h) \
    $(wildcard include/config/sys/fsl/sec/compat.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/sys/fdt/pad.h) \
    $(wildcard include/config/of/board/setup.h) \
    $(wildcard include/config/board/rng/seed.h) \
    $(wildcard include/config/of/system/setup.h) \
    $(wildcard include/config/fdt/fixup/partitions.h) \
    $(wildcard include/config/fman/enet.h) \
    $(wildcard include/config/fsl/mc/enet.h) \
    $(wildcard include/config/cmd/pstore.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../scripts/dtc/libfdt/fdt.c \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../scripts/dtc/libfdt/libfdt_env.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/scripts/dtc/libfdt/fdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/scripts/dtc/libfdt/libfdt.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../scripts/dtc/libfdt/libfdt_internal.h \

tools/libfdt/fdt.o: $(deps_tools/libfdt/fdt.o)

$(deps_tools/libfdt/fdt.o):
