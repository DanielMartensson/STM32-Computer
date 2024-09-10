cmd_arch/arm/lib/crt0_arm_efi.o := arm-ostl-linux-gnueabi-gcc -Wp,-MD,arch/arm/lib/.crt0_arm_efi.o.d -nostdinc -isystem /opt/STMicroelectronics/STM32MP1/SDK/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include -Iinclude  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include    -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include -include /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/kconfig.h -D__KERNEL__ -D__UBOOT__ -D__ASSEMBLY__ -fno-PIE -g -DCONFIG_THUMB2_KERNEL -D__ARM__ -Wa,-mimplicit-it=always -mthumb -mthumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -mgeneral-regs-only -pipe -march=armv7-a -D__LINUX_ARM_ARCH__=7 -mtune=generic-armv7-a  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/mach-stm32mp/include   -c -o arch/arm/lib/crt0_arm_efi.o /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/lib/crt0_arm_efi.S

source_arch/arm/lib/crt0_arm_efi.o := /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/lib/crt0_arm_efi.S

deps_arch/arm/lib/crt0_arm_efi.o := \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/kconfig.h \
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
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/asm-generic/pe.h \

arch/arm/lib/crt0_arm_efi.o: $(deps_arch/arm/lib/crt0_arm_efi.o)

$(deps_arch/arm/lib/crt0_arm_efi.o):
