sudo /home/lubuntu/Program/STM32/CubeProgrammer/bin/STM32_Programmer_CLI -l usb
sudo /home/lubuntu/Program/STM32/CubeProgrammer/bin/STM32_Programmer_CLI -c port=usb1 -d arm-trusted-firmware/tf-a-stm32mp151a-stm32-computer-firmware-mx-usb.stm32 0x1 -s 0x1 -d fip/fip-stm32mp151a-stm32-computer-firmware-mx-optee-emmc.bin 0x3 -s 0x3
