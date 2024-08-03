// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021-2022, STMicroelectronics
 */

#include <assert.h>
#include <compiler.h>
#include <drivers/regulator.h>
#include <drivers/stm32mp1_pwr.h>
#include <drivers/stm32mp1_syscfg.h>
#include <initcall.h>
#include <io.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <libfdt.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32_util.h>
#include <trace.h>

#define TIMEOUT_US_10MS		U(10000)

#define IO_VOLTAGE_THRESHOLD	U(2700)

/*
 * struct iod_regul - IO domain regulator instance
 * @enable_reg: PWR register offset for the IO domain
 * @enable_mask: Domain enable register bit mask in PWR register
 * @ready_mask: Domain ready bit mask in PWR register
 * @valid_mask: Domain valid bit mask in PWR register
 * @hslv_index: Index of HSLV register for the IO domain
 * @io_comp_index: Index of IO compensation for the IO domain
 * @suspend_state: True if regulator is enabled before suspend, false otherwise
 * @suspend_mv: Voltage level before suspend in millivolts
 * @supply: Regulator supplier for the IO domain
 */
struct iod_regul {
	uint32_t enable_reg;
	uint32_t enable_mask;
	uint32_t ready_mask;
	uint32_t valid_mask;

	uint32_t hslv_index;
	uint32_t io_comp_index;

	bool suspend_state;
	uint16_t suspend_mv;

	struct rdev *supply;
};

static TEE_Result iod_set_speed(const struct regul_desc *desc, bool state)
{
	struct iod_regul *iod = (struct iod_regul *)desc->driver_data;

	FMSG("%s: set speed=%u", desc->node_name, state);

	stm32mp_set_hslv_by_index(iod->hslv_index, state);

	return TEE_SUCCESS;
}

static TEE_Result iod_set_state(const struct regul_desc *desc, bool enable)
{
	struct iod_regul *iod = (struct iod_regul *)desc->driver_data;
	uintptr_t pwr_reg = stm32_pwr_base() + iod->enable_reg;

	FMSG("%s: set state %u", desc->node_name, enable);

	if (enable) {
		uint64_t to = 0;

		io_setbits32(pwr_reg, iod->enable_mask);

		to = timeout_init_us(TIMEOUT_US_10MS);
		while (!timeout_elapsed(to))
			if (io_read32(pwr_reg) & iod->ready_mask)
				break;

		if (!(io_read32(pwr_reg) & iod->ready_mask))
			return TEE_ERROR_GENERIC;

		io_setbits32(pwr_reg, iod->valid_mask);
		io_clrbits32(pwr_reg, iod->enable_mask);

		stm32mp_set_io_comp_by_index(iod->io_comp_index, true);
	} else {
		stm32mp_set_io_comp_by_index(iod->io_comp_index, false);

		io_clrbits32(pwr_reg, iod->enable_mask | iod->valid_mask);
	}

	return TEE_SUCCESS;
}

static TEE_Result iod_get_state(const struct regul_desc *desc, bool *enabled)
{
	struct iod_regul *iod = (struct iod_regul *)desc->driver_data;
	uintptr_t pwr_reg = stm32_pwr_base() + iod->enable_reg;

	FMSG("%s: get state", desc->node_name);

	*enabled = io_read32(pwr_reg) & (iod->enable_mask | iod->valid_mask);

	return TEE_SUCCESS;
}

static TEE_Result iod_get_voltage(const struct regul_desc *desc, uint16_t *mv)
{
	struct iod_regul *iod = (struct iod_regul *)desc->driver_data;

	FMSG("%s: get volt", desc->node_name);

	return regulator_get_voltage(iod->supply, mv);
}

static TEE_Result iod_set_voltage(const struct regul_desc *desc, uint16_t mv)
{
	struct iod_regul *iod = (struct iod_regul *)desc->driver_data;
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Result result = TEE_ERROR_GENERIC;
	bool en = false;

	FMSG("%s: set volt to %"PRIu16" mv", desc->node_name, mv);

	res = iod_get_state(desc, &en);
	if (res)
		return res;

	/* Isolate IOs and disable IOs compensation when changing voltage */
	if (en) {
		res = iod_set_state(desc, false);
		if (res)
			return res;
	}

	/*
	 * Set IO to low speed
	 * Setting high voltage with IOs in high speed mode may damage the IOs
	 */
	res = iod_set_speed(desc, false);
	if (res)
		return res;

	/* Forward set voltage request to the power supply */
	result = regulator_set_voltage(iod->supply, mv);
	if (result) {
		EMSG("regulator %s set voltage failed (%d)",
		     desc->node_name, result);
		res = regulator_get_voltage(iod->supply, &mv);
		if (res)
			return res;
	}

	if (mv < IO_VOLTAGE_THRESHOLD)
		res = iod_set_speed(desc, true);

	if (en) {
		res = iod_set_state(desc, true);
		if (res)
			return res;
	}

	return result;
}

static TEE_Result iod_list_voltages(const struct regul_desc *desc,
				    uint16_t **levels, size_t *count)
{
	struct iod_regul *iod = (struct iod_regul *)desc->driver_data;

	/* Returns parent voltage list */
	return regulator_list_voltages(iod->supply, levels, count);
}

/*
 * Power management suspend/resume operations
 * To protect the IOs, we switch to low speed before entering suspend state
 * and restore the configuration when resuming.
 */
static TEE_Result iod_pm(enum pm_op op, unsigned int pm_hint __unused,
			 const struct pm_callback_handle *hdl)
{
	struct regul_desc *desc = (struct regul_desc *)hdl->handle;
	struct iod_regul *iod = (struct iod_regul *)desc->driver_data;
	TEE_Result res = TEE_ERROR_GENERIC;

	assert(op == PM_OP_SUSPEND || op == PM_OP_RESUME);

	if (op == PM_OP_SUSPEND) {
		FMSG("%s: suspend", desc->node_name);

		res = iod_get_state(desc, &iod->suspend_state);
		if (res)
			return res;

		res = iod_get_voltage(desc, &iod->suspend_mv);
		if (res)
			return res;

		/* Force to low speed */
		res = iod_set_speed(desc, false);
		if (res)
			return res;
	} else {
		FMSG("%s: resume", desc->node_name);

		res = iod_set_voltage(desc, iod->suspend_mv);
		if (res)
			return res;

		res = iod_set_state(desc, iod->suspend_state);
		if (res)
			return res;
	}

	return TEE_SUCCESS;
}

static const struct regul_ops iod_ops = {
	.set_state = iod_set_state,
	.get_state = iod_get_state,
	.set_voltage = iod_set_voltage,
	.get_voltage = iod_get_voltage,
	.list_voltages = iod_list_voltages,
	.lock = stm32mp1_pwr_regul_lock,
	.unlock = stm32mp1_pwr_regul_unlock,
};

static const struct regul_ops iod_fixed_ops = {
	.set_state = iod_set_state,
	.get_state = iod_get_state,
	.get_voltage = iod_get_voltage,
	.lock = stm32mp1_pwr_regul_lock,
	.unlock = stm32mp1_pwr_regul_unlock,
};

#define DEFINE_REG(id, name, supply) { \
	.node_name = name, \
	.ops = &iod_ops, \
	.driver_data = &iod_regulators[id], \
	.supply_name = supply, \
}

enum iod_regulator {
	IOD_SDMMC1,
	IOD_SDMMC2,
	IOD_REGU_COUNT
};

static struct iod_regul iod_regulators[] = {
	 [IOD_SDMMC1] = {
		.enable_reg = PWR_CR3_OFF,
		.enable_mask = PWR_CR3_VDDSD1EN,
		.ready_mask = PWR_CR3_VDDSD1RDY,
		.valid_mask = PWR_CR3_VDDSD1VALID,
		.hslv_index = SYSCFG_HSLV_IDX_SDMMC1,
		.io_comp_index = SYSCFG_IO_COMP_IDX_SD1,
	 },
	 [IOD_SDMMC2] = {
		.enable_reg = PWR_CR3_OFF,
		.enable_mask = PWR_CR3_VDDSD2EN,
		.ready_mask = PWR_CR3_VDDSD2RDY,
		.valid_mask = PWR_CR3_VDDSD2VALID,
		.hslv_index = SYSCFG_HSLV_IDX_SDMMC2,
		.io_comp_index = SYSCFG_IO_COMP_IDX_SD2,
	 },
};

static struct regul_desc iod_regul_desc[] = {
	[IOD_SDMMC1] = DEFINE_REG(IOD_SDMMC1, "sdmmc1_io", "vddsd1"),
	[IOD_SDMMC2] = DEFINE_REG(IOD_SDMMC2, "sdmmc2_io", "vddsd2"),
};

static TEE_Result iod_regulator_probe(const void *fdt, int node,
				      const void *compat_data __unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct regul_desc *desc = NULL;
	const char *regu_name = NULL;
	struct iod_regul *iod = NULL;
	uint16_t mv = 0;
	size_t i = 0;

	regu_name = fdt_get_name(fdt, node, NULL);

	FMSG("iod probe with regu_name=%s", regu_name);

	/* Look for matching regul */
	for (i = 0; i < IOD_REGU_COUNT; i++)
		if (!strcmp(iod_regul_desc[i].node_name, regu_name))
			break;

	if (i == IOD_REGU_COUNT) {
		EMSG("regu_name=%s not found", regu_name);
		return TEE_ERROR_GENERIC;
	}

	desc = &iod_regul_desc[i];
	iod = (struct iod_regul *)desc->driver_data;

	iod->supply = regulator_get_by_supply_name(fdt, node,
						   desc->supply_name);
	if (!iod->supply) {
		DMSG("desc->supply_name=%s not found", desc->supply_name);
		return TEE_ERROR_DEFER_DRIVER_INIT;
	}

	/* If the supply voltage is fixed, do not handle voltage ops */
	if (iod->supply->min_mv == iod->supply->max_mv)
		desc->ops = &iod_fixed_ops;

	res = regulator_register(desc, node);
	if (res) {
		EMSG("regulator_register(%s) failed with %#"PRIx32,
		     regu_name, res);
		return res;
	}

	res = iod_get_voltage(desc, &mv);
	if (res)
		return res;

	if (mv < IO_VOLTAGE_THRESHOLD) {
		res = iod_set_speed(desc, true);
		if (res)
			return res;
	}

	register_pm_driver_cb(iod_pm, (void *)desc, "iod-regu");

	FMSG("regu_name=%s probed", regu_name);

	return TEE_SUCCESS;
}

static const struct dt_device_match iod_regulator_match_table[] = {
	{ .compatible = "st,stm32mp13-iod" },
	{ }
};

DEFINE_DT_DRIVER(stm32mp1_regulator_iod_dt_driver) = {
	.name = "stm32mp1-iod-regulator",
	.match_table = iod_regulator_match_table,
	.probe = iod_regulator_probe,
};
