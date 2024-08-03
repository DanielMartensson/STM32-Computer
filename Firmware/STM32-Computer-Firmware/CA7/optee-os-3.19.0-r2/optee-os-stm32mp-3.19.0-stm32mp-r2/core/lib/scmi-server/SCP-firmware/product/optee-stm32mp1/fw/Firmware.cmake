#
# Arm SCP/MCP Software
# Copyright (c) 2022, Linaro Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#
# Configure the build system.
#

set(SCP_FIRMWARE "scmi-fw")

set(SCP_FIRMWARE_TARGET "scmi-fw")

set(SCP_TOOLCHAIN_INIT "GNU")

set(SCP_ARCHITECTURE "optee")

set(CMAKE_BUILD_TYPE "Release")

set(SCP_ENABLE_NOTIFICATIONS_INIT FALSE)

set(SCP_ENABLE_SCMI_NOTIFICATIONS_INIT FALSE)

set(SCP_ENABLE_SCMI_SENSOR_EVENTS_INIT FALSE)

set(SCP_ENABLE_FAST_CHANNELS_INIT FALSE)

set(SCP_ENABLE_SCMI_RESET_INIT TRUE)

set(SCP_ENABLE_IPO_INIT FALSE)

list(PREPEND SCP_MODULE_PATHS "${CMAKE_CURRENT_LIST_DIR}/../module/stm32_regu_consumer")
list(PREPEND SCP_MODULE_PATHS "${CMAKE_CURRENT_LIST_DIR}/../module/psu_optee_regulator")

# The order of the modules in the following list is the order in which the
# modules are initialized, bound, started during the pre-runtime phase.
# any change in the order will cause firmware initialization errors.

list(APPEND SCP_MODULES "optee-mbx")
list(APPEND SCP_MODULES "msg-smt")
list(APPEND SCP_MODULES "scmi")
list(APPEND SCP_MODULES "optee-clock")
list(APPEND SCP_MODULES "clock")
list(APPEND SCP_MODULES "scmi-clock")
list(APPEND SCP_MODULES "optee-reset")
list(APPEND SCP_MODULES "reset-domain")
list(APPEND SCP_MODULES "scmi-reset-domain")
list(APPEND SCP_MODULES "stm32-regu-consumer")
list(APPEND SCP_MODULES "voltage-domain")
list(APPEND SCP_MODULES "scmi-voltage-domain")
if (CFG_STM32MP13)
    list(APPEND SCP_MODULES "psu-optee-regulator")
    list(APPEND SCP_MODULES "psu")
    list(APPEND SCP_MODULES "dvfs")
    list(APPEND SCP_MODULES "scmi-perf")
endif (CFG_STM32MP13)
list(APPEND SCP_MODULES "optee-console")
