// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020-2021, STMicroelectronics
 */

#include <assert.h>
#include <compiler.h>
#include <drivers/regulator.h>
#include <initcall.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/dt.h>
#include <kernel/panic.h>
#include <libfdt.h>
#include <stdint.h>
#include <stdio.h>
#include <trace.h>

#define FIXED_REGULATOR_NAME_LEN	22

struct fixed_regul {
	char name[FIXED_REGULATOR_NAME_LEN];
	struct regul_desc desc;
};

static TEE_Result fixed_set_state(const struct regul_desc *desc __unused,
				  bool enabled __unused)
{
	return TEE_SUCCESS;
}

static TEE_Result fixed_get_state(const struct regul_desc *desc __unused,
				  bool *enabled)
{
	*enabled = true;

	return TEE_SUCCESS;
}

static struct regul_ops fixed_regul_ops = {
	.set_state = fixed_set_state,
	.get_state = fixed_get_state,
};
DECLARE_KEEP_PAGER(fixed_regul_ops);

static TEE_Result fixed_regulator_probe(const void *fdt, int node,
					const void *compat_data __unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	size_t __maybe_unused len = 0;
	struct fixed_regul *fr = NULL;
	const char *reg_name = NULL;

	fr = calloc(1, sizeof(*fr));
	if (!fr)
		panic();

	reg_name = fdt_get_name(fdt, node, NULL);
	len = snprintf(fr->name, sizeof(fr->name) - 1, "%s", reg_name);
	assert(len > 0 && len < (sizeof(fr->name) - 1));

	fr->desc.node_name = fr->name;
	fr->desc.driver_data = fr;
	fr->desc.ops = &fixed_regul_ops;

	res = regulator_register(&fr->desc, node);
	if (res) {
		EMSG("regulator_register(%s) failed with %#"PRIx32,
		     reg_name, res);
		panic();
	}

	return TEE_SUCCESS;
}

static const struct dt_device_match regulator_match_table[] = {
	{ .compatible = "regulator-fixed" },
	{ }
};

DEFINE_DT_DRIVER(fixed_regulator_dt_driver) = {
	.name = "fixed-regulator",
	.match_table = regulator_match_table,
	.probe = fixed_regulator_probe,
};
