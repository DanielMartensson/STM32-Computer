$(call force,CFG_SCPFW_MOD_CLOCK,y)
$(call force,CFG_SCPFW_MOD_MSG_SMT,y)
$(call force,CFG_SCPFW_MOD_OPTEE_CLOCK,y)
$(call force,CFG_SCPFW_MOD_OPTEE_CONSOLE,y)
$(call force,CFG_SCPFW_MOD_OPTEE_MBX,y)
$(call force,CFG_SCPFW_MOD_OPTEE_RESET,y)
$(call force,CFG_SCPFW_MOD_RESET_DOMAIN,y)
$(call force,CFG_SCPFW_MOD_SCMI,y)
$(call force,CFG_SCPFW_MOD_SCMI_CLOCK,y)
$(call force,CFG_SCPFW_MOD_SCMI_RESET_DOMAIN,y)
$(call force,CFG_SCPFW_MOD_SCMI_VOLTAGE_DOMAIN,y)
$(call force,CFG_SCPFW_MOD_STM32_REGU_CONSUMER,y)
$(call force,CFG_SCPFW_MOD_VOLTAGE_DOMAIN,y)

ifeq ($(CFG_STM32MP13),y)
$(call force,CFG_SCPFW_MOD_DVFS,y)
$(call force,CFG_SCPFW_MOD_PSU,y)
$(call force,CFG_SCPFW_MOD_PSU_OPTEE_REGULATOR,y)
$(call force,CFG_SCPFW_MOD_SCMI_PERF,y)
endif

# Info level is sufficient for scp-firmware
CFG_SCPFW_LOG_LEVEL ?= 1

$(call force,CFG_SCPFW_NOTIFICATION,n)
$(call force,CFG_SCPFW_FAST_CHANNEL,n)
