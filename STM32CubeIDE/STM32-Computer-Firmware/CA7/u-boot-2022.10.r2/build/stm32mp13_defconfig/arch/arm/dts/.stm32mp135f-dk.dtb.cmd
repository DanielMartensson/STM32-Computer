cmd_arch/arm/dts/stm32mp135f-dk.dtb := mkdir -p arch/arm/dts/ ; (cat /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp135f-dk.dts; echo '$(pound)include "stm32mp135f-dk-u-boot.dtsi"') > arch/arm/dts/.stm32mp135f-dk.dtb.pre.tmp;  cc -E -Wp,-MD,arch/arm/dts/.stm32mp135f-dk.dtb.d.pre.tmp -nostdinc -Iinclude  -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include    -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/include -include /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/linux/kconfig.h -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/include -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include -I/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/external-dt/u-boot -D__ASSEMBLY__ -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/dts/.stm32mp135f-dk.dtb.dts.tmp arch/arm/dts/.stm32mp135f-dk.dtb.pre.tmp ; ./scripts/dtc/dtc -O dtb -o arch/arm/dts/stm32mp135f-dk.dtb -b 0 -i /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/ -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-graph_port -Wno-unique_unit_address -Wno-simple_bus_reg -Wno-pci_device_reg -Wno-pci_bridge -Wno-pci_device_bus_num  -@ -a 0x8 -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-graph_port -Wno-unique_unit_address -Wno-simple_bus_reg -Wno-pci_device_reg -Wno-pci_bridge -Wno-pci_device_bus_num  -@ -d arch/arm/dts/.stm32mp135f-dk.dtb.d.dtc.tmp arch/arm/dts/.stm32mp135f-dk.dtb.dts.tmp || (echo "Check /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/build/stm32mp13_defconfig/arch/arm/dts/.stm32mp135f-dk.dtb.pre.tmp for errors" && false) ; sed "s:arch/arm/dts/.stm32mp135f-dk.dtb.pre.tmp:/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp135f-dk.dts:" arch/arm/dts/.stm32mp135f-dk.dtb.d.pre.tmp arch/arm/dts/.stm32mp135f-dk.dtb.d.dtc.tmp > arch/arm/dts/.stm32mp135f-dk.dtb.d

source_arch/arm/dts/stm32mp135f-dk.dtb := /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp135f-dk.dts

deps_arch/arm/dts/stm32mp135f-dk.dtb := \
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
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/gpio/gpio.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/input/input.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/input/linux-event-codes.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/leds/common.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/regulator/st,stm32mp13-regulator.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/rtc/rtc-stm32.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp135.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp133.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp131.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/bus/stm32mp13_sys_bus.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/interrupt-controller/arm-gic.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/interrupt-controller/irq.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/clock/stm32mp13-clks.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/reset/stm32mp13-resets.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/thermal/thermal.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp13xf.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp13-pinctrl.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/include/dt-bindings/pinctrl/stm32-pinfunc.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp135f-dk-u-boot.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/CA7/u-boot-2022.10.r2/u-boot-stm32mp-v2022.10-stm32mp-r2/arch/arm/dts/stm32mp13-u-boot.dtsi \

arch/arm/dts/stm32mp135f-dk.dtb: $(deps_arch/arm/dts/stm32mp135f-dk.dtb)

$(deps_arch/arm/dts/stm32mp135f-dk.dtb):
