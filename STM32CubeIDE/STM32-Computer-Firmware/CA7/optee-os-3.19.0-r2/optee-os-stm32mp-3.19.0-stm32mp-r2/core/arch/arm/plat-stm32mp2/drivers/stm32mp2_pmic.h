/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022-2023, STMicroelectronics
 */

#ifndef __DRIVERS_STM32MP2_PMIC_H__
#define __DRIVERS_STM32MP2_PMIC_H__

#include <drivers/regulator.h>
#include <drivers/stpmic2.h>
#include <stdint.h>
#include <tee_api_types.h>

TEE_Result stpmic2_set_prop(const struct regul_desc *desc,
			    enum stpmic2_prop_id prop, uint32_t value);

#endif /*__DRIVERS_STM32MP2_PMIC_H__*/
