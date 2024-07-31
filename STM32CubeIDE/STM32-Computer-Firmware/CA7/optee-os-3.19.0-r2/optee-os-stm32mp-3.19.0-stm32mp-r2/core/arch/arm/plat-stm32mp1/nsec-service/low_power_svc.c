// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2017-2018, STMicroelectronics - All Rights Reserved
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 */

#include <drivers/stm32mp_dt_bindings.h>
#include <sm/pm.h>
#include <sm/psci.h>
#include <stm32mp_pm.h>
#include <stm32_util.h>
#include <trace.h>

#include "stm32mp1_smc.h"
#include "low_power_svc.h"
#include "../pm/power.h"
#include "../pm/context.h"

#ifndef CFG_PM
uint32_t pm_domain_scv_handler(uint32_t id __unused, uint32_t enable __unused)
{
	return STM32_SIP_SVC_FAILED;
}
#else
uint32_t pm_domain_scv_handler(uint32_t id, uint32_t enable)
{
	unsigned int pd = id;

	DMSG("%sable PD %u", enable != 0 ? "En" : "Dis", pd);

	switch (pd) {
	case STM32MP1_PD_VSW:
	case STM32MP1_PD_CORE_RET:
	case STM32MP1_PD_CORE:
		break;
	default:
		return STM32_SIP_SVC_INVALID_PARAMS;
	}

	stm32mp1_set_pm_domain_state(pd, enable);

	return STM32_SIP_SVC_OK;
}
#endif /*CFG_PM*/
