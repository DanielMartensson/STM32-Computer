// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2018-2022, STMicroelectronics
 */

#include <drivers/rstctrl.h>
#include <drivers/stm32_rstctrl.h>
#include <kernel/dt.h>

static struct rstctrl_ops *stm32_reset_mp13_find_ops(unsigned int id __unused)
{
	return &stm32_rstctrl_clr_offset_ops;
}

const struct stm32_reset_data stm32mp13_reset = {
	.reset_get_ops = stm32_reset_mp13_find_ops
};

static const struct dt_device_match stm32_rstctrl_match_table[] = {
	{ .compatible = "st,stm32mp13-rcc", .compat_data = &stm32mp13_reset },
	{ }
};

DEFINE_DT_DRIVER(stm32_rstctrl_dt_driver) = {
	.name = "stm32_rstctrl",
	.type = DT_DRIVER_RSTCTRL,
	.match_table = stm32_rstctrl_match_table,
	.probe = stm32_rstctrl_provider_probe,
};
