/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2017-2018, STMicroelectronics
 */
#ifndef __STM32MP_PM_H__
#define __STM32MP_PM_H__

#define PSCI_MODE_SYSTEM_SUSPEND	0
#define PSCI_MODE_SYSTEM_OFF		1

enum stm32mp1_pm_domain {
	STM32MP1_PD_VSW,
	STM32MP1_PD_CORE_RET,
	STM32MP1_PD_CORE,
	STM32MP1_PD_MAX_PM_DOMAIN
};

void __noreturn stm32_cores_reset(void);

/* Convert low power mode carried by pm_hint into pm mode name */
const char *stm32mp_pm_hint2mode_name(uint32_t pm_hint);

#endif /*__STM32MP_PM_H__*/

