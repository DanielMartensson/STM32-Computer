cmd_arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dtb := gcc -E -Wp,-MMD,arch/arm/boot/dts/.stm32mp151a-stm32-computer-firmware-mx.dtb.d.pre.tmp -nostdinc -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.stm32mp151a-stm32-computer-firmware-mx.dtb.dts.tmp /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dts ; ./scripts/dtc/dtc -o arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dtb -b 0 -i/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/ -i/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes -Wno-interrupt_provider -Wno-unique_unit_address -Wno-unit_address_vs_reg -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg   -@ -d arch/arm/boot/dts/.stm32mp151a-stm32-computer-firmware-mx.dtb.d.dtc.tmp arch/arm/boot/dts/.stm32mp151a-stm32-computer-firmware-mx.dtb.dts.tmp ; cat arch/arm/boot/dts/.stm32mp151a-stm32-computer-firmware-mx.dtb.d.pre.tmp arch/arm/boot/dts/.stm32mp151a-stm32-computer-firmware-mx.dtb.d.dtc.tmp > arch/arm/boot/dts/.stm32mp151a-stm32-computer-firmware-mx.dtb.d

source_arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dtb := /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dts

deps_arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dtb := \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/pinctrl/stm32-pinfunc.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp151.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/bus/stm32mp15_sys_bus.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/arm-gic.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/clock/stm32mp1-clks.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/reset/stm32mp1-resets.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp15xa.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp15xxac-pinctrl.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp15-m4-srm.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp15-scmi.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/mfd/st,stpmic1.h \

arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dtb: $(deps_arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dtb)

$(deps_arch/arm/boot/dts/stm32mp151a-stm32-computer-firmware-mx.dtb):
