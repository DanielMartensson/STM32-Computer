scpfw-incdirs-y += $(scpfw-path)/product/optee-stm32mp1/include

srcs-y += $(scpfw-path)/product/optee-stm32mp1/fw/config_all.c

$(eval $(call scpfw-embed-product-module,stm32_regu_consumer))
ifeq ($(CFG_STM32MP13),y)
$(eval $(call scpfw-embed-product-module,psu_optee_regulator))
endif

scpfw-cmake-flags-y += -DCFG_STM32MP13=$(CFG_STM32MP13)
