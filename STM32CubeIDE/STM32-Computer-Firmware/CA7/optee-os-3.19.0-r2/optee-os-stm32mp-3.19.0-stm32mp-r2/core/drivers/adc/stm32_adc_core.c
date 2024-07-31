// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/regulator.h>
#include <drivers/stm32_adc_core.h>
#include <io.h>
#include <kernel/dt.h>
#include <kernel/pm.h>
#include <libfdt.h>
#include <mm/core_memprot.h>

#define STM32MP13_ADC_MAX_CLK_RATE	U(75000000)

struct stm32_adc_core_data {
	struct clk *aclk;
	struct clk *bclk;
	struct rdev *vref;
	struct rdev *vdda;
	struct stm32_adc_common common;
};

struct stm32_adc_core_device {
	struct stm32_adc_core_data data;
};

/**
 * struct stm32mp13_adc_ck_spec - specification for stm32mp13 ADC clock
 * @ckmode: ADC clock mode, Async (0) or sync (!=0) with prescaler.
 * @presc: prescaler bitfield for async clock mode
 * @div: prescaler division ratio
 */
struct stm32mp13_adc_ck_spec {
	uint32_t ckmode;
	uint32_t presc;
	int div;
};

static const struct stm32mp13_adc_ck_spec stm32mp13_adc_ckmodes_spec[] = {
	/* 00: CK_ADC[1..3]: Asynchronous clock modes */
	{ .ckmode = 0, .presc = 0, .div = 1 },
	{ .ckmode = 0, .presc = 1, .div = 2 },
	{ .ckmode = 0, .presc = 2, .div = 4 },
	{ .ckmode = 0, .presc = 3, .div = 6 },
	{ .ckmode = 0, .presc = 4, .div = 8 },
	{ .ckmode = 0, .presc = 5, .div = 10 },
	{ .ckmode = 0, .presc = 6, .div = 12 },
	{ .ckmode = 0, .presc = 7, .div = 16 },
	{ .ckmode = 0, .presc = 8, .div = 32 },
	{ .ckmode = 0, .presc = 9, .div = 64 },
	{ .ckmode = 0, .presc = 10, .div = 128 },
	{ .ckmode = 0, .presc = 11, .div = 256 },
	/* HCLK used: Synchronous clock modes (1, 2 or 4 prescaler) */
	{ .ckmode = 1, .presc = 0, .div = 1 },
	{ .ckmode = 2, .presc = 0, .div = 2 },
	{ .ckmode = 3, .presc = 0, .div = 4 },
};

static TEE_Result stm32_adc_core_hw_start(struct stm32_adc_core_device *adc_dev)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct stm32_adc_core_data *priv = &adc_dev->data;

	res = regulator_enable(priv->vdda);
	if (res) {
		EMSG("VDDA enable failed  %#"PRIx32, res);
		return res;
	}

	res = regulator_enable(priv->vref);
	if (res) {
		EMSG("VREF enable failed %#"PRIx32, res);
		goto err_vdda;
	}

	res = clk_enable(priv->bclk);
	if (res) {
		EMSG("Failed to enable bclk %#"PRIx32, res);
		goto err_vref;
	}

	if (priv->aclk) {
		res = clk_enable(priv->aclk);
		if (res) {
			EMSG("Failed to enable aclk %#"PRIx32, res);
			goto err_bclk;
		}
	}

	return TEE_SUCCESS;

err_bclk:
	clk_disable(priv->bclk);

err_vref:
	regulator_disable(priv->vref);

err_vdda:
	regulator_disable(priv->vdda);

	return res;
}

static TEE_Result stm32_adc_core_hw_stop(struct stm32_adc_core_device *adc_dev)
{
	struct stm32_adc_core_data *priv = &adc_dev->data;
	TEE_Result res1 = TEE_ERROR_GENERIC;
	TEE_Result res2 = TEE_ERROR_GENERIC;

	if (priv->aclk)
		clk_disable(priv->aclk);
	clk_disable(priv->bclk);

	res1 = regulator_disable(priv->vref);
	res2 = regulator_disable(priv->vdda);

	if (res1) {
		EMSG("Failed to disable VREF regulator");
		return res1;
	}

	if (res2) {
		EMSG("Failed to disable VDDA regulator");
		return res2;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_adc_core_clk_sel(struct stm32_adc_core_device *adc_dev)
{
	struct stm32_adc_core_data *priv = &adc_dev->data;
	uint32_t ckmode = 0;
	uint32_t presc = 0;
	unsigned long rate = 0;
	unsigned int i = 0;
	int div = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	/*
	 * The ADC can use either 'bus' or 'adc' clock for analog circuitry.
	 * So, choice is to have bus clock mandatory and ADC clock optional.
	 * If optional 'adc' clock has been found, then try to use it first.
	 */
	if (priv->aclk) {
		/*
		 * Asynchronous clock modes (e.g. ckmode == 0)
		 * From spec: PLL output musn't exceed max rate
		 */
		rate = clk_get_rate(priv->aclk);
		if (!rate) {
			EMSG("Invalid %s clock rate", clk_get_name(priv->aclk));
			return TEE_ERROR_GENERIC;
		}

		for (i = 0; i < ARRAY_SIZE(stm32mp13_adc_ckmodes_spec); i++) {
			ckmode = stm32mp13_adc_ckmodes_spec[i].ckmode;
			presc = stm32mp13_adc_ckmodes_spec[i].presc;
			div = stm32mp13_adc_ckmodes_spec[i].div;

			if (ckmode)
				continue;

			if ((rate / div) <= STM32MP13_ADC_MAX_CLK_RATE)
				goto out;
		}
	}

	/* Synchronous clock modes (e.g. ckmode is 1, 2 or 3) */
	rate = clk_get_rate(priv->bclk);
	if (!rate) {
		EMSG("Invalid bus clock rate: 0");
		return TEE_ERROR_GENERIC;
	}

	for (i = 0; i < ARRAY_SIZE(stm32mp13_adc_ckmodes_spec); i++) {
		ckmode = stm32mp13_adc_ckmodes_spec[i].ckmode;
		presc = stm32mp13_adc_ckmodes_spec[i].presc;
		div = stm32mp13_adc_ckmodes_spec[i].div;

		if (!ckmode)
			continue;

		if ((rate / div) <= STM32MP13_ADC_MAX_CLK_RATE)
			goto out;
	}

	EMSG("Clock selection failed");

	return TEE_ERROR_GENERIC;

out:
	/* rate used later by each ADC instance to control BOOST mode */
	priv->common.rate = rate / div;

	res = clk_enable(priv->bclk);
	if (res) {
		EMSG("Failed to enable bclk");
		return res;
	}

	/* Set common clock mode and prescaler */
	io_clrsetbits32(priv->common.regs + STM32MP13_ADC_CCR,
			STM32MP13_CKMODE_MASK | STM32MP13_PRESC_MASK,
			ckmode << STM32MP13_CKMODE_SHIFT |
			presc << STM32MP13_PRESC_SHIFT);

	clk_disable(priv->bclk);

	DMSG("Using %s clock/%d source at %ld kHz",
	     ckmode ? "bus" : "adc", div, priv->common.rate / 1000);

	priv->common.rate = presc;

	return TEE_SUCCESS;
}

static struct stm32_adc_common *
stm32_adc_core_get_common_data(struct dt_pargs *pargs __unused, void *data,
			       TEE_Result *res)
{
	if (data)
		*res = TEE_SUCCESS;
	else
		*res = TEE_ERROR_GENERIC;

	return (struct stm32_adc_common *)data;
}

static TEE_Result
stm32_adc_core_pm_resume(struct stm32_adc_core_device *adc_dev)
{
	return stm32_adc_core_hw_start(adc_dev);
}

static TEE_Result
stm32_adc_core_pm_suspend(struct stm32_adc_core_device *adc_dev)
{
	return stm32_adc_core_hw_stop(adc_dev);
}

static TEE_Result
stm32_adc_core_pm(enum pm_op op, unsigned int pm_hint __unused,
		  const struct pm_callback_handle *pm_handle)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct stm32_adc_core_device *dev =
	(struct stm32_adc_core_device *)PM_CALLBACK_GET_HANDLE(pm_handle);

	if (op == PM_OP_RESUME)
		res = stm32_adc_core_pm_resume(dev);
	else
		res = stm32_adc_core_pm_suspend(dev);

	return res;
}

static TEE_Result stm32_adc_core_probe(const void *fdt, int node,
				       const void *compat_data __unused)
{
	struct dt_node_info dt_info = { };
	struct stm32_adc_core_device *adc_dev = NULL;
	struct stm32_adc_core_data *priv = NULL;
	struct io_pa_va base = { };
	int subnode = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	adc_dev = calloc(1, sizeof(*adc_dev));
	if (!adc_dev)
		return TEE_ERROR_OUT_OF_MEMORY;

	priv = &adc_dev->data;
	priv->common.dev = adc_dev;

	_fdt_fill_device_info(fdt, &dt_info, node);

	if (dt_info.reg == DT_INFO_INVALID_REG ||
	    dt_info.interrupt == DT_INFO_INVALID_INTERRUPT)
		goto err;

	base.pa = dt_info.reg;
	priv->common.regs = io_pa_or_va_secure(&base, dt_info.reg_size);

	priv->vref = regulator_get_by_supply_name(fdt, node, "vref");
	if (!priv->vref) {
		res = TEE_ERROR_DEFER_DRIVER_INIT;
		goto err;
	}

	priv->vdda = regulator_get_by_supply_name(fdt, node, "vdda");
	if (!priv->vdda) {
		res = TEE_ERROR_DEFER_DRIVER_INIT;
		goto err;
	}

	res = regulator_set_min_voltage(priv->vref);
	if (res)
		goto err;

	res = regulator_get_voltage(priv->vref, &priv->common.vref_mv);
	if (res)
		goto err;

	res = clk_dt_get_by_name(fdt, node, "bus", &priv->bclk);
	if (res) {
		if (res != TEE_ERROR_DEFER_DRIVER_INIT)
			EMSG("Error %#"PRIx32" getting 'bus' clock", res);
		goto err;
	}

	/* The 'adc' clock is optional. Only check defer status. */
	res = clk_dt_get_by_name(fdt, node, "adc", &priv->aclk);
	if (res == TEE_ERROR_DEFER_DRIVER_INIT)
		goto err;

	res = stm32_adc_core_hw_start(adc_dev);
	if (res)
		goto err;

	res = stm32_adc_core_clk_sel(adc_dev);
	if (res) {
		EMSG("Error %#"PRIx32" selecting clock", res);
		goto err_stop;
	}

	register_pm_core_service_cb(stm32_adc_core_pm, adc_dev,
				    "stm32-adc-core");

	res = dt_driver_register_provider(fdt, node,
					  (get_of_device_func)
					  stm32_adc_core_get_common_data,
					  (void *)&priv->common,
					  DT_DRIVER_NOTYPE);
	if (res) {
		EMSG("Couldn't register ADC core provider");
		goto err_stop;
	}

	fdt_for_each_subnode(subnode, fdt, node) {
		res = dt_driver_maybe_add_probe_node(fdt, subnode);
		if (res) {
			EMSG("Failed on node %s with %#"PRIx32,
			     fdt_get_name(fdt, subnode, NULL), res);
			return res;
		}
	}

	DMSG("ADC core %s probed", fdt_get_name(fdt, node, NULL));

	return TEE_SUCCESS;

err_stop:
	stm32_adc_core_hw_stop(adc_dev);

err:
	free(adc_dev);

	return res;
}

static const struct dt_device_match stm32_adc_core_match_table[] = {
	{ .compatible = "st,stm32mp13-adc-core" },
	{ }
};

DEFINE_DT_DRIVER(stm32_adc_core_dt_driver) = {
	.name = "stm32-adc-core",
	.match_table = stm32_adc_core_match_table,
	.probe = stm32_adc_core_probe,
};
