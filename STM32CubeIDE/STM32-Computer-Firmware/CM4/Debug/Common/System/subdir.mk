################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/Common/System/system_stm32mp1xx.c 

OBJS += \
./Common/System/system_stm32mp1xx.o 

C_DEPS += \
./Common/System/system_stm32mp1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Common/System/system_stm32mp1xx.o: /home/lubuntu/Dokument/GitHub/STM32-Computer/STM32CubeIDE/STM32-Computer-Firmware/Common/System/system_stm32mp1xx.c Common/System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32MP151Axx -c -I../Core/Inc -I../../Drivers/STM32MP1xx_HAL_Driver/Inc -I../../Drivers/STM32MP1xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32MP1xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-System

clean-Common-2f-System:
	-$(RM) ./Common/System/system_stm32mp1xx.cyclo ./Common/System/system_stm32mp1xx.d ./Common/System/system_stm32mp1xx.o ./Common/System/system_stm32mp1xx.su

.PHONY: clean-Common-2f-System

