// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#include <assert.h>
#include <compiler.h>
#include <drivers/regulator.h>
#include <drivers/stm32mp25_pwr.h>
#include <drivers/stm32_rif.h>
#include <initcall.h>
#include <io.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/mutex.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/thread.h>
#include <libfdt.h>
#include <stm32_sysconf.h>
#include <trace.h>

/* Mutex for protecting PWR accesses */
static struct mutex pwr_regu_mu = MUTEX_INITIALIZER;

#define PWR_CR1			U(0x00)
#define PWR_CR7			U(0x18)
#define PWR_CR8			U(0x1c)
#define PWR_CR9			U(0x20)
#define PWR_CR12		U(0x2c)
#define PWR_UCPDR		U(0x30)

#define PWR_CR1_VDDIO3VMEN	BIT(0)
#define PWR_CR1_VDDIO4VMEN	BIT(1)
#define PWR_CR1_USB33VMEN	BIT(2)
#define PWR_CR1_UCPDVMEN	BIT(3)
#define PWR_CR1_AVMEN		BIT(4)
#define PWR_CR1_VDDIO3SV	BIT(8)
#define PWR_CR1_VDDIO4SV	BIT(9)
#define PWR_CR1_USB33SV		BIT(10)
#define PWR_CR1_UCPDSV		BIT(11)
#define PWR_CR1_ASV		BIT(12)
#define PWR_CR1_VDDIO3RDY	BIT(16)
#define PWR_CR1_VDDIO4RDY	BIT(17)
#define PWR_CR1_USB33RDY	BIT(18)
#define PWR_CR1_UCPDRDY		BIT(19)
#define PWR_CR1_ARDY		BIT(20)
#define PWR_CR1_VDDIOVRSEL	BIT(24)
#define PWR_CR1_VDDIO3VRSEL	BIT(25)
#define PWR_CR1_VDDIO4VRSEL	BIT(26)
#define PWR_CR1_GPVMO		BIT(31)

#define PWR_CR7_VDDIO2VMEN	BIT(0)
#define PWR_CR7_VDDIO2SV	BIT(8)
#define PWR_CR7_VDDIO2RDY	BIT(16)
#define PWR_CR7_VDDIO2VRSEL	BIT(24)
#define PWR_CR7_VDDIO2VRSTBY	BIT(25)

#define PWR_CR8_VDDIO1VMEN	BIT(0)
#define PWR_CR8_VDDIO1SV	BIT(8)
#define PWR_CR8_VDDIO1RDY	BIT(16)
#define PWR_CR8_VDDIO1VRSEL	BIT(24)
#define PWR_CR8_VDDIO1VRSTBY	BIT(25)

#define PWR_CR9_BKPRBSEN	BIT(0)
#define PWR_CR9_LPR1BSEN	BIT(4)

#define PWR_CR12_GPUVMEN	BIT(0)
#define PWR_CR12_GPULVTEN	BIT(1)
#define PWR_CR12_GPUSV		BIT(8)
#define PWR_CR12_VDDGPURDY	BIT(16)

#define PWR_UCPDR_UCPD_DBDIS	BIT(0)
#define PWR_UCPDR_UCPD_STBY	BIT(1)

#define TIMEOUT_US_10MS		U(10000)
#define DELAY_100US		U(100)

#define IO_VOLTAGE_THRESHOLD_MV U(2700)

struct pwr_regu {
	uint32_t enable_reg;
	uint32_t enable_mask;
	uint32_t ready_mask;
	uint32_t valid_mask;
	uint32_t vrsel_mask;
	struct rdev *supply;
	enum syscfg_io_ids comp_idx;
	uint16_t suspend_mv;
	bool suspend_state;
	/* An IO domain is a power switch, meaning that
	 * its output voltage follows its power supply voltage
	 * and the IOs have IO compensation cell
	 */
	bool is_an_iod;
	bool need_supply;
	bool keep_monitor_on;
	/* rifsc_filtering_id is used to disable filtering when
	 * accessing to the register
	 */
	uint8_t rifsc_filtering_id;
};

static TEE_Result pwr_enable_reg(const struct regul_desc *desc)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;
	uintptr_t pwr_reg = stm32_pwr_base() + regu->enable_reg;
	uint64_t to = U(0);
	bool cid_filtering_enabled = false;

	FMSG("%s: enable", desc->node_name);

	if (!regu->enable_mask)
		return TEE_SUCCESS;

	if (regu->rifsc_filtering_id)
		cid_filtering_enabled =
			stm32_rifsc_cid_is_enabled(regu->rifsc_filtering_id);

	if (cid_filtering_enabled)
		stm32_rifsc_cid_disable(regu->rifsc_filtering_id);

	io_setbits32(pwr_reg, regu->enable_mask);

	/* Wait for vddgpu to enable as stated in the reference manual */
	if (regu->keep_monitor_on)
		udelay(DELAY_100US);

	to = timeout_init_us(TIMEOUT_US_10MS);
	while (!timeout_elapsed(to))
		if (io_read32(pwr_reg) & regu->ready_mask)
			break;

	if (!(io_read32(pwr_reg) & regu->ready_mask)) {
		if (cid_filtering_enabled)
			stm32_rifsc_cid_enable(regu->rifsc_filtering_id);
		return TEE_ERROR_GENERIC;
	}

	io_setbits32(pwr_reg, regu->valid_mask);

	/* Do not keep the voltage monitor enabled except for GPU */
	if (!regu->keep_monitor_on)
		io_clrbits32(pwr_reg, regu->enable_mask);

	if (cid_filtering_enabled)
		stm32_rifsc_cid_enable(regu->rifsc_filtering_id);

	return TEE_SUCCESS;
}

static void pwr_disable_reg(const struct regul_desc *desc)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;
	uintptr_t pwr_reg = stm32_pwr_base() + regu->enable_reg;
	bool cid_filtering_enabled = false;

	FMSG("%s: disable", desc->node_name);

	if (regu->rifsc_filtering_id)
		cid_filtering_enabled =
			stm32_rifsc_cid_is_enabled(regu->rifsc_filtering_id);

	if (cid_filtering_enabled)
		stm32_rifsc_cid_disable(regu->rifsc_filtering_id);

	if (regu->enable_mask) {
		/* Make sure the previous operations are visible */
		dsb();
		io_clrbits32(pwr_reg, regu->enable_mask | regu->valid_mask);
	}

	if (cid_filtering_enabled)
		stm32_rifsc_cid_enable(regu->rifsc_filtering_id);

}

static TEE_Result pwr_set_state(const struct regul_desc *desc, bool enable)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;
	TEE_Result res = TEE_ERROR_GENERIC;

	FMSG("%s: set state %u", desc->node_name, enable);

	if (enable) {
		res = pwr_enable_reg(desc);
		if (res)
			return res;

		if (regu->is_an_iod) {
			res = stm32mp25_syscfg_enable_io_compensation(regu->comp_idx);
			if (res) {
				pwr_disable_reg(desc);
				return res;
			}
		}
	} else {
		if (regu->is_an_iod) {
			res = stm32mp25_syscfg_disable_io_compensation(regu->comp_idx);
			if (res)
				return res;
		}

		pwr_disable_reg(desc);
	}

	return TEE_SUCCESS;
}

static TEE_Result pwr_get_state(const struct regul_desc *desc, bool *enabled)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;
	uintptr_t pwr_reg = stm32_pwr_base() + regu->enable_reg;

	FMSG("%s: get state", desc->node_name);

	if (!regu->enable_mask)
		*enabled = true;
	else
		*enabled = io_read32(pwr_reg) & (regu->enable_mask |
						 regu->valid_mask);

	return TEE_SUCCESS;
}

static TEE_Result pwr_set_low_volt(const struct regul_desc *desc, bool state)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;
	uintptr_t pwr_reg = stm32_pwr_base() + regu->enable_reg;

	if (regu->vrsel_mask) {
		FMSG("%s: set speed=%u", desc->node_name, state);

		if (state) {
			io_setbits32(pwr_reg, regu->vrsel_mask);

			if (!(io_read32(pwr_reg) & regu->vrsel_mask)) {
				EMSG("%s: set VRSEL failed", desc->node_name);
				panic();
			}
		} else {
			io_clrbits32(pwr_reg, regu->vrsel_mask);
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result pwr_get_voltage(const struct regul_desc *desc, uint16_t *mv)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;

	FMSG("%s: get volt", desc->node_name);

	return regulator_get_voltage(regu->supply, mv);
}

static TEE_Result pwr_set_voltage(const struct regul_desc *desc, uint16_t mv)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_Result result = TEE_ERROR_GENERIC;
	bool en = false;

	FMSG("%s: set volt to %"PRIu16" mv", desc->node_name, mv);

	res = pwr_get_state(desc, &en);
	if (res)
		return res;

	/* Isolate IOs and disable IOs compensation */
	if (en) {
		res = pwr_set_state(desc, false);
		if (res)
			return res;
	}

	/* Set IO to high voltage */
	if (mv >= IO_VOLTAGE_THRESHOLD_MV) {
		res = pwr_set_low_volt(desc, false);
		if (res)
			return res;
	}

	/* Forward set voltage request to the power supply */
	result = regulator_set_voltage(regu->supply, mv);
	if (result) {
		EMSG("regulator %s set voltage failed (%d)",
		     desc->node_name, result);
		/* continue to restore IOs setting for current voltage */
		res = regulator_get_voltage(regu->supply, &mv);
		if (res)
			return res;
	}

	if (mv < IO_VOLTAGE_THRESHOLD_MV)
		res = pwr_set_low_volt(desc, true);

	/* De-isolate IOs and enable IOs compensation */
	if (en) {
		res = pwr_set_state(desc, true);
		if (res)
			return res;
	}

	/* Make sure the regulator is updated before returning to caller */
	dsb();

	return result;
}

static TEE_Result pwr_list_voltages(const struct regul_desc *desc,
				    uint16_t **levels, size_t *count)
{
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;

	/* Return parent voltage list */
	return regulator_list_voltages(regu->supply, levels, count);
}

static void pwr_regu_lock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_lock(&pwr_regu_mu);
}

static void pwr_regu_unlock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_unlock(&pwr_regu_mu);
}

/*
 * suspend - resume operations:
 * To protect the IOs, disable low voltage mode before entering suspend
 * resume restore the previous configuration.
 */
static TEE_Result pwr_regu_pm(enum pm_op op, unsigned int pm_hint __unused,
			      const struct pm_callback_handle *hdl)
{
	struct regul_desc *desc = (struct regul_desc *)hdl->handle;
	struct pwr_regu *regu = (struct pwr_regu *)desc->driver_data;
	TEE_Result res = TEE_ERROR_GENERIC;

	assert(op == PM_OP_SUSPEND || op == PM_OP_RESUME);

	if (op == PM_OP_SUSPEND) {
		FMSG("%s: suspend", desc->node_name);

		res = pwr_get_state(desc, &regu->suspend_state);
		if (res)
			return res;

		if (regu->is_an_iod) {
			res = pwr_get_voltage(desc, &regu->suspend_mv);
			if (res)
				return res;

			/* Disable low voltage mode to protect IOs*/
			res = pwr_set_low_volt(desc, false);
			if (res)
				return res;

			res = stm32mp25_syscfg_disable_io_compensation(regu->comp_idx);
			if (res)
				return res;
		}
	} else {
		FMSG("%s: resume", desc->node_name);

		if (regu->is_an_iod) {
			res = pwr_set_voltage(desc, regu->suspend_mv);
			if (res)
				return res;
		}

		res = pwr_set_state(desc, regu->suspend_state);
		if (res)
			return res;
	}

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(pwr_regu_pm);

static const struct regul_ops pwr_regu_ops = {
	.set_state = pwr_set_state,
	.get_state = pwr_get_state,
	.set_voltage = pwr_set_voltage,
	.get_voltage = pwr_get_voltage,
	.list_voltages = pwr_list_voltages,
	.lock = pwr_regu_lock,
	.unlock = pwr_regu_unlock,
};
DECLARE_KEEP_PAGER(pwr_regu_ops);

static const struct regul_ops pwr_regu_fixed_ops = {
	.set_state = pwr_set_state,
	.get_state = pwr_get_state,
	.lock = pwr_regu_lock,
	.unlock = pwr_regu_unlock,
};
DECLARE_KEEP_PAGER(pwr_regu_fixed_ops);

enum pwr_regulator {
	IOD_VDDIO1,
	IOD_VDDIO2,
	IOD_VDDIO3,
	IOD_VDDIO4,
	IOD_VDDIO,
	REGU_UCPD,
	REGU_A,
	REGU_GPU,
	PWR_REGU_COUNT
};

static struct pwr_regu pwr_regulators[] = {
	 [IOD_VDDIO1] = {
		.enable_reg = PWR_CR8,
		.enable_mask = PWR_CR8_VDDIO1VMEN,
		.ready_mask = PWR_CR8_VDDIO1RDY,
		.valid_mask = PWR_CR8_VDDIO1SV,
		.vrsel_mask = PWR_CR8_VDDIO1VRSEL,
		.is_an_iod = true,
		.need_supply = true,
		.comp_idx = SYSFG_VDDIO1_ID,
	 },
	 [IOD_VDDIO2] = {
		.enable_reg = PWR_CR7,
		.enable_mask = PWR_CR7_VDDIO2VMEN,
		.ready_mask = PWR_CR7_VDDIO2RDY,
		.valid_mask = PWR_CR7_VDDIO2SV,
		.vrsel_mask = PWR_CR7_VDDIO2VRSEL,
		.is_an_iod = true,
		.need_supply = true,
		.comp_idx = SYSFG_VDDIO2_ID,
	 },
	 [IOD_VDDIO3] = {
		.enable_reg = PWR_CR1,
		.enable_mask = PWR_CR1_VDDIO3VMEN,
		.ready_mask = PWR_CR1_VDDIO3RDY,
		.valid_mask = PWR_CR1_VDDIO3SV,
		.vrsel_mask = PWR_CR1_VDDIO3VRSEL,
		.is_an_iod = true,
		.need_supply = true,
		.comp_idx = SYSFG_VDDIO3_ID,
	 },
	 [IOD_VDDIO4] = {
		.enable_reg = PWR_CR1,
		.enable_mask = PWR_CR1_VDDIO4VMEN,
		.ready_mask = PWR_CR1_VDDIO4RDY,
		.valid_mask = PWR_CR1_VDDIO4SV,
		.vrsel_mask = PWR_CR1_VDDIO4VRSEL,
		.is_an_iod = true,
		.need_supply = true,
		.comp_idx = SYSFG_VDDIO4_ID,
	 },
	 [IOD_VDDIO] = {
		.enable_reg = PWR_CR1,
		.vrsel_mask = PWR_CR1_VDDIOVRSEL,
		.is_an_iod = true,
		.need_supply = true,
		.comp_idx = SYSFG_VDD_IO_ID,
	 },
	 [REGU_UCPD] = {
		.enable_reg = PWR_CR1,
		.enable_mask = PWR_CR1_UCPDVMEN,
		.ready_mask = PWR_CR1_UCPDRDY,
		.valid_mask = PWR_CR1_UCPDSV,
	 },
	 [REGU_A] = {
		.enable_reg = PWR_CR1,
		.enable_mask = PWR_CR1_AVMEN,
		.ready_mask = PWR_CR1_ARDY,
		.valid_mask = PWR_CR1_ASV,
	 },
	 [REGU_GPU] = {
		.enable_reg = PWR_CR12,
		.enable_mask = PWR_CR12_GPUVMEN,
		.ready_mask = PWR_CR12_VDDGPURDY,
		.valid_mask = PWR_CR12_GPUSV,
		.need_supply = true,
		.keep_monitor_on = true,
		.rifsc_filtering_id = STM32MP25_RIFSC_GPU_ID,
	 },
};

#define DEFINE_REGUL(id, name, supply) { \
	.node_name = name, \
	.ops = &pwr_regu_ops, \
	.driver_data = &pwr_regulators[id], \
	.supply_name = supply, \
}

#define DEFINE_FIXED(id, name, supply) { \
	.node_name = name, \
	.ops = &pwr_regu_fixed_ops, \
	.driver_data = &pwr_regulators[id], \
	.supply_name = supply, \
}

/* Not const to allow probe to reassign ops if device is a fixed_iod */
static struct regul_desc pwr_regu_desc[] = {
	[IOD_VDDIO1] = DEFINE_REGUL(IOD_VDDIO1, "vddio1", "vddio1"),
	[IOD_VDDIO2] = DEFINE_REGUL(IOD_VDDIO2, "vddio2", "vddio2"),
	[IOD_VDDIO3] = DEFINE_REGUL(IOD_VDDIO3, "vddio3", "vddio3"),
	[IOD_VDDIO4] = DEFINE_REGUL(IOD_VDDIO4, "vddio4", "vddio4"),
	[IOD_VDDIO]  = DEFINE_REGUL(IOD_VDDIO, "vddio", "vdd"),
	[REGU_UCPD]  = DEFINE_FIXED(REGU_UCPD, "vdd33ucpd", "vdd33ucpd"),
	[REGU_A]     = DEFINE_FIXED(REGU_A, "vdda18adc", "vdda18adc"),
	[REGU_GPU]   = DEFINE_REGUL(REGU_GPU, "vddgpu", "vddgpu"),
};
DECLARE_KEEP_PAGER(pwr_regu_desc);

static TEE_Result pwr_regulator_probe(const void *fdt, int node,
				      const void *compat_data __unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct regul_desc *desc = NULL;
	const char *regu_name = NULL;
	struct pwr_regu *regu = NULL;
	uint16_t mv = 0;
	size_t i = 0;

	regu_name = fdt_get_name(fdt, node, NULL);

	FMSG("PWR probe regu %s", regu_name);

	/* Look for matching regul */
	for (i = 0; i < PWR_REGU_COUNT; i++)
		if (!strcmp(pwr_regu_desc[i].node_name, regu_name))
			break;

	if (i == PWR_REGU_COUNT) {
		EMSG("regu_name=%s not found", regu_name);
		return TEE_ERROR_GENERIC;
	}

	desc = &pwr_regu_desc[i];
	regu = (struct pwr_regu *)desc->driver_data;

	if (regu->need_supply) {
		regu->supply = regulator_get_by_supply_name(fdt, node,
							    desc->supply_name);
		if (!regu->supply) {
			DMSG("%s-supply not found", desc->supply_name);
			return TEE_ERROR_DEFER_DRIVER_INIT;
		}

		/* If the supply voltage is fixed, do not handle voltage ops */
		if (regu->supply->min_mv == regu->supply->max_mv)
			desc->ops = &pwr_regu_fixed_ops;
	}

	res = regulator_register(desc, node);
	if (res) {
		EMSG("regulator_register(%s) failed with %#"PRIx32,
		     regu_name, res);
		return res;
	}

	if (regu->is_an_iod) {
		res = pwr_get_voltage(desc, &mv);
		if (res)
			return res;

		if (mv < IO_VOLTAGE_THRESHOLD_MV) {
			res = pwr_set_low_volt(desc, true);
			if (res)
				return res;
		}
	}

	register_pm_driver_cb(pwr_regu_pm, (void *)desc, "pwr-regu");

	FMSG("regu_name=%s probed", regu_name);

	return TEE_SUCCESS;
}

static const struct dt_device_match pwr_regulator_match_table[] = {
	{ .compatible = "st,stm32mp25-pwr-regu" },
	{ }
};

DEFINE_DT_DRIVER(stm32mp25_pwr_regulator_dt_driver) = {
	.name = "stm32mp25-pwr-regulator",
	.match_table = pwr_regulator_match_table,
	.probe = pwr_regulator_probe,
};
