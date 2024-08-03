srcs-$(CFG_DDR_LOWPOWER) += stm32mp1_ddrc.c
srcs-$(CFG_STM32_CLKCALIB) += stm32mp1_calib.c
srcs-$(CFG_STPMIC1) += stm32mp1_pmic.c
srcs-y 	+= stm32mp1_pwr.c
srcs-y 	+= stm32mp1_pwr_irq.c
srcs-$(CFG_SYSCFG) += stm32mp1_syscfg.c
