cmd_tools/lib/sha256.o := cc -Wp,-MD,tools/lib/.sha256.o.d -Itools -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11 -DCONFIG_FIT_SIGNATURE -DCONFIG_FIT_SIGNATURE_MAX_SIZE=0xffffffff -DCONFIG_FIT_CIPHER -include /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h -idirafterinclude -idirafter/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include -idirafter/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/scripts/dtc/libfdt  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools -DUSE_HOSTCC -D__KERNEL_STRICT_NAMES -D_GNU_SOURCE -pedantic -c -o tools/lib/sha256.o tools/lib/sha256.c

source_tools/lib/sha256.o := tools/lib/sha256.c

deps_tools/lib/sha256.o := \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/stdc-predef.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/compiler.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stddef.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdbool.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdint.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/stdint.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/libc-header-start.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/features.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/features-time64.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/wordsize.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/timesize.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/cdefs.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/long-double.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/gnu/stubs.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/gnu/stubs-64.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/typesizes.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/time64.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/wchar.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdint-intn.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdint-uintn.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/errno.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/errno.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/errno.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/asm/errno.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/asm-generic/errno.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/asm-generic/errno-base.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/error_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/stdlib.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/waitflags.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/waitstatus.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/floatn.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/floatn-common.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/locale_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__locale_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/types.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/clock_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/clockid_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/time_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/timer_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/endian.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/endian.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/endianness.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/byteswap.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/uintn-identity.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/select.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/select.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/sigset_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__sigset_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_timeval.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_timespec.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/pthreadtypes.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/thread-shared-types.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/pthreadtypes-arch.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/atomic_wide_counter.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_mutex.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_rwlock.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/alloca.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdlib-bsearch.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdlib-float.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/stdio.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/lib/gcc/x86_64-ostl_sdk-linux/12.3.0/include/stdarg.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__fpos_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__mbstate_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__fpos64_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/__FILE.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/FILE.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_FILE.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/cookie_io_functions_t.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdio_lim.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stdio.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/string.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/strings.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/sys/mman.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-map-flags-generic.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-linux.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman-shared.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/mman_ext.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/fcntl.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/fcntl.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/fcntl-linux.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_iovec.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/linux/falloc.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/stat.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/struct_stat.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/byteswap.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/time.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/time.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/timex.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_tm.h \
  /opt/st/stm32mp1/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/include/bits/types/struct_itimerspec.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/tools/../lib/sha256.c \
    $(wildcard include/config/hw/watchdog.h) \
    $(wildcard include/config/watchdog.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/watchdog.h \
    $(wildcard include/config/spl/build.h) \
    $(wildcard include/config/spl/watchdog.h) \
    $(wildcard include/config/mpc85xx.h) \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/u-boot/sha256.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/compiler_attributes.h \

tools/lib/sha256.o: $(deps_tools/lib/sha256.o)

$(deps_tools/lib/sha256.o):
