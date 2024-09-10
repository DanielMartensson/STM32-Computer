cmd_arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dtb := gcc -E -Wp,-MMD,arch/arm/boot/dts/.stm32mp135f-dk-a7-examples.dtb.d.pre.tmp -nostdinc -I/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.stm32mp135f-dk-a7-examples.dtb.dts.tmp /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dts ; ./scripts/dtc/dtc -o arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dtb -b 0 -i/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/ -i/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes -Wno-interrupt_provider -Wno-unique_unit_address -Wno-unit_address_vs_reg -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg   -@ -d arch/arm/boot/dts/.stm32mp135f-dk-a7-examples.dtb.d.dtc.tmp arch/arm/boot/dts/.stm32mp135f-dk-a7-examples.dtb.dts.tmp ; cat arch/arm/boot/dts/.stm32mp135f-dk-a7-examples.dtb.d.pre.tmp arch/arm/boot/dts/.stm32mp135f-dk-a7-examples.dtb.d.dtc.tmp > arch/arm/boot/dts/.stm32mp135f-dk-a7-examples.dtb.d

source_arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dtb := /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dts

deps_arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dtb := \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp135f-dk.dts \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/leds/common.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/regulator/st,stm32mp13-regulator.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/rtc/rtc-stm32.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp135.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp133.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp131.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/bus/stm32mp13_sys_bus.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/arm-gic.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/clock/stm32mp13-clks.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/reset/stm32mp13-resets.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/thermal/thermal.h \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp13xf.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/arch/arm/boot/dts/stm32mp13-pinctrl.dtsi \
  /home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/linux-6.1.82/linux-6.1.82/scripts/dtc/include-prefixes/dt-bindings/pinctrl/stm32-pinfunc.h \

arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dtb: $(deps_arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dtb)

$(deps_arch/arm/boot/dts/stm32mp135f-dk-a7-examples.dtb):
