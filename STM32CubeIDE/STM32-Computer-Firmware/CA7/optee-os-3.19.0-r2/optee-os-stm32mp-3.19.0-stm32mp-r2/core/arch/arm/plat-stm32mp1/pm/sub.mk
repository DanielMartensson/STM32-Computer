asm-defines-y += context_asm_defines.c

srcs-$(CFG_PM) += context.c
srcs-$(CFG_PM) += low_power.c
srcs-$(CFG_PM) += pm_helpers.S
srcs-$(CFG_PM) += power_config.c
srcs-$(CFG_PSCI_ARM32) += psci.c
