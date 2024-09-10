cmd_arch/arm/boot/dts/stm32f469-disco.dtb := gcc -E -Wp,-MMD,arch/arm/boot/dts/.stm32f469-disco.dtb.d.pre.tmp -nostdinc -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.stm32f469-disco.dtb.dts.tmp /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32f469-disco.dts ; ./scripts/dtc/dtc -o arch/arm/boot/dts/stm32f469-disco.dtb -b 0 -i/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/ -i/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes -Wno-interrupt_provider -Wno-unique_unit_address -Wno-unit_address_vs_reg -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg   -@ -d arch/arm/boot/dts/.stm32f469-disco.dtb.d.dtc.tmp arch/arm/boot/dts/.stm32f469-disco.dtb.dts.tmp ; cat arch/arm/boot/dts/.stm32f469-disco.dtb.d.pre.tmp arch/arm/boot/dts/.stm32f469-disco.dtb.d.dtc.tmp > arch/arm/boot/dts/.stm32f469-disco.dtb.d

source_arch/arm/boot/dts/stm32f469-disco.dtb := /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32f469-disco.dts

deps_arch/arm/boot/dts/stm32f469-disco.dtb := \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32f469.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32f429.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/armv7-m.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/clock/stm32fx-clock.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/mfd/stm32f4-rcc.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32f469-pinctrl.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32f4-pinctrl.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/pinctrl/stm32-pinfunc.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \

arch/arm/boot/dts/stm32f469-disco.dtb: $(deps_arch/arm/boot/dts/stm32f469-disco.dtb)

$(deps_arch/arm/boot/dts/stm32f469-disco.dtb):
