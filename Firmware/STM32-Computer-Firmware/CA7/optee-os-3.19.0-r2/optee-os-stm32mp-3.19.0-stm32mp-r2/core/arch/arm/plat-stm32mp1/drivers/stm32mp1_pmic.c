// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2017-2023, STMicroelectronics
 */

#include <config.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/regulator.h>
#include <drivers/stm32_firewall.h>
#include <drivers/stm32_i2c.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp1_pmic.h>
#include <drivers/stm32mp1_pwr.h>
#include <drivers/stpmic1.h>
#include <drivers/stpmic1_regulator.h>
#include <dt-bindings/mfd/st,stpmic1.h>
#include <io.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/notif.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/thread.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <platform_config.h>
#include <stdbool.h>
#include <stm32_util.h>
#include <trace.h>
#include <util.h>

#include "pm/power.h"

#define MODE_STANDBY                    8

#define PMIC_I2C_TRIALS			1
#define PMIC_I2C_TIMEOUT_BUSY_MS	5

#define PMIC_REGU_SUPPLY_NAME_LEN	12

#define PMIC_REGU_COUNT			14

/*
 * st,mask-reset: set mask reset for the regulator, meaning that the regulator
 * setting is maintained during pmic reset.
 */
#define STPMIC1_REGUL_MASK_RESET	BIT(15)
/* st,regulator-sink-source: set the regulator in sink source mode */
#define STPMIC1_REGUL_SINK_SOURCE	BIT(14)
/* st,regulator-bypass: set the regulator in bypass mode */
#define STPMIC1_REGUL_ENABLE_BYPASS	BIT(13)

/* suspend() arguments */
#define STPMIC1_LP_STATE_OFF		BIT(0)
#define STPMIC1_LP_STATE_ON		BIT(1)
#define STPMIC1_LP_STATE_UNCHANGED	BIT(2)
#define STPMIC1_LP_STATE_SET_VOLT	BIT(3)

/* Expect a single PMIC instance */
static struct i2c_handle_s *i2c_pmic_handle;
static uint32_t pmic_i2c_addr;

struct pmic_it_handle_s {
	uint8_t pmic_reg;
	uint8_t pmic_bit;
	uint8_t notif_id;

	SLIST_ENTRY(pmic_it_handle_s) link;
};

static SLIST_HEAD(pmic_it_handle_head, pmic_it_handle_s) pmic_it_handle_list =
	SLIST_HEAD_INITIALIZER(pmic_it_handle_list);

/* CPU voltage supplier if found */
static char cpu_supply_name[PMIC_REGU_SUPPLY_NAME_LEN];

/* Mutex for protecting PMIC accesses */
static struct mutex pmic_mu = MUTEX_INITIALIZER;

static TEE_Result register_pmic_regulator(const void *fdt,
					  const char *regu_name, int node);

static int dt_get_pmic_node(const void *fdt)
{
	static int node = -FDT_ERR_BADOFFSET;

	if (node == -FDT_ERR_BADOFFSET)
		node = fdt_node_offset_by_compatible(fdt, -1, "st,stpmic1");

	return node;
}

static int pmic_status = -1;

static bool pmic_is_secure(void)
{
	assert(pmic_status != -1);

	return pmic_status == DT_STATUS_OK_SEC;
}

bool stm32mp_with_pmic(void)
{
	return pmic_status != -1;
}

/* Return a libfdt compliant status value */
static int save_cpu_supply_name(void)
{
	void *fdt = NULL;
	int node = 0;
	const fdt32_t *cuint = NULL;
	const char *name = NULL;

	fdt = get_embedded_dt();
	if (!fdt)
		panic();

	node = fdt_path_offset(fdt, "/cpus/cpu@0");
	if (node < 0)
		return -FDT_ERR_NOTFOUND;

	cuint = fdt_getprop(fdt, node, "cpu-supply", NULL);
	if (!cuint)
		return -FDT_ERR_NOTFOUND;

	node = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(*cuint));
	if (node < 0)
		return -FDT_ERR_NOTFOUND;

	name = fdt_get_name(fdt, node, NULL);
	assert(strnlen(name, sizeof(cpu_supply_name)) <
	       sizeof(cpu_supply_name));

	strncpy(cpu_supply_name, name, sizeof(cpu_supply_name));

	return 0;
}

const char *stm32mp_pmic_get_cpu_supply_name(void)
{
	return cpu_supply_name;
}

static void parse_regulator_fdt_nodes(void)
{
	int pmic_node = 0;
	int regulators_node = 0;
	int regu_node = 0;
	void *fdt = NULL;

	fdt = get_embedded_dt();
	if (!fdt)
		panic();

	pmic_node = dt_get_pmic_node(fdt);
	if (pmic_node < 0)
		panic();

	regulators_node = fdt_subnode_offset(fdt, pmic_node, "regulators");
	if (regulators_node < 0)
		panic();

	fdt_for_each_subnode(regu_node, fdt, regulators_node) {
		int status = _fdt_get_status(fdt, regu_node);
		const char *regu_name = NULL;
		size_t __maybe_unused n = 0;

		assert(status >= 0);
		if (status == DT_STATUS_DISABLED)
			continue;

		regu_name = fdt_get_name(fdt, regu_node, NULL);

		assert(stpmic1_regulator_is_valid(regu_name));

		if (register_pmic_regulator(fdt, regu_name, regu_node))
			panic();
	}

	if (save_cpu_supply_name())
		DMSG("No CPU supply provided");
}

/*
 * PMIC and resource initialization
 */

/* Return true if PMIC is available, false if not found, panics on errors */
static void initialize_pmic_i2c(const void *fdt, int pmic_node)
{

	const fdt32_t *cuint = NULL;

	cuint = fdt_getprop(fdt, pmic_node, "reg", NULL);
	if (!cuint) {
		EMSG("PMIC configuration failed on reg property");
		panic();
	}

	pmic_i2c_addr = fdt32_to_cpu(*cuint) << 1;
	if (pmic_i2c_addr > UINT16_MAX) {
		EMSG("PMIC configuration failed on i2c address translation");
		panic();
	}

	stm32mp_get_pmic();

	if (!stm32_i2c_is_device_ready(i2c_pmic_handle, pmic_i2c_addr,
				       PMIC_I2C_TRIALS,
				       PMIC_I2C_TIMEOUT_BUSY_MS))
		panic();

	stpmic1_bind_i2c(i2c_pmic_handle, pmic_i2c_addr);

	stm32mp_put_pmic();
}

#ifdef CFG_REGULATOR_DRIVERS
static TEE_Result pmic_set_state(const struct regul_desc *desc, bool enable)
{
	int ret = 0;
	FMSG("%s: set state to %u", desc->node_name, enable);

	if (enable)
		ret = stpmic1_regulator_enable(desc->node_name);
	else
		ret = stpmic1_regulator_disable(desc->node_name);

	if (ret)
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}

static TEE_Result pmic_get_state(const struct regul_desc *desc, bool *enabled)
{
	FMSG("%s: get state", desc->node_name);

	*enabled = stpmic1_is_regulator_enabled(desc->node_name);

	return TEE_SUCCESS;
}

static TEE_Result pmic_get_voltage(const struct regul_desc *desc, uint16_t *mv)
{
	int ret = 0;

	FMSG("%s: get volt", desc->node_name);

	ret = stpmic1_regulator_voltage_get(desc->node_name);
	if (ret < 0)
		return TEE_ERROR_GENERIC;

	*mv = ret;

	return TEE_SUCCESS;
}

static TEE_Result pmic_set_voltage(const struct regul_desc *desc, uint16_t mv)
{
	FMSG("%s: set volt", desc->node_name);

	if (stpmic1_regulator_voltage_set(desc->node_name, mv))
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}

static TEE_Result pmic_list_voltages(const struct regul_desc *desc,
				     uint16_t **levels, size_t *count)
{
	FMSG("%s: list volt", desc->node_name);

	if (stpmic1_regulator_levels_mv(desc->node_name,
					(const uint16_t **)levels, count))
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}

static TEE_Result pmic_set_flag(const struct regul_desc *desc, uint16_t flag)
{
	int ret = 0;

	FMSG("%s: set_flag %#"PRIx16, desc->node_name, flag);

	switch (flag) {
	case REGUL_OCP:
		ret = stpmic1_regulator_icc_set(desc->node_name);
		break;
	case REGUL_ACTIVE_DISCHARGE:
		ret = stpmic1_active_discharge_mode_set(desc->node_name);
		break;
	case REGUL_PULL_DOWN:
		ret = stpmic1_regulator_pull_down_set(desc->node_name);
		break;
	case STPMIC1_REGUL_MASK_RESET:
		ret = stpmic1_regulator_mask_reset_set(desc->node_name);
		break;
	case STPMIC1_REGUL_SINK_SOURCE:
		ret = stpmic1_regulator_sink_mode_set(desc->node_name);
		break;
	case STPMIC1_REGUL_ENABLE_BYPASS:
		ret = stpmic1_regulator_bypass_mode_set(desc->node_name);
		break;
	default:
		DMSG("Invalid flag %#"PRIx16, flag);
		panic();
	}

	if (ret)
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}

static void driver_lock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_lock(&pmic_mu);

	stm32mp_get_pmic();
}

static void driver_unlock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_unlock(&pmic_mu);

	stm32mp_put_pmic();
}

static TEE_Result driver_suspend(const struct regul_desc *desc, uint8_t state,
				 uint16_t mv)
{
	int ret = 0;

	FMSG("%s: suspend state:%"PRIu8", %"PRIu16" mV", desc->node_name,
	     state, mv);

	if (!stpmic1_regu_has_lp_cfg(desc->node_name))
		return TEE_SUCCESS;

	ret = stpmic1_lp_copy_reg(desc->node_name);
	if (ret)
		return TEE_ERROR_GENERIC;

	if ((state & STPMIC1_LP_STATE_OFF) != 0U) {
		ret = stpmic1_lp_reg_on_off(desc->node_name, 0);
		if (ret)
			return TEE_ERROR_GENERIC;
	}

	if ((state & STPMIC1_LP_STATE_ON) != 0U) {
		ret = stpmic1_lp_reg_on_off(desc->node_name, 1);
		if (ret)
			return TEE_ERROR_GENERIC;
	}

	if ((state & STPMIC1_LP_STATE_SET_VOLT) != 0U) {
		ret = stpmic1_lp_set_voltage(desc->node_name, mv);
		if (ret)
			return TEE_ERROR_GENERIC;
	}

	return 0;
}

struct regu_handle_s {
	struct regul_desc *desc;
	uint8_t lp_state[STM32_PM_MAX_SOC_MODE];
	uint16_t lp_mv[STM32_PM_MAX_SOC_MODE];
};

const struct regul_ops pmic_ops = {
	.set_state = pmic_set_state,
	.get_state = pmic_get_state,
	.set_voltage = pmic_set_voltage,
	.get_voltage = pmic_get_voltage,
	.list_voltages = pmic_list_voltages,
	.set_flag = pmic_set_flag,
	.lock = driver_lock,
	.unlock = driver_unlock,
};

#define DEFINE_REGU(name) { \
	.node_name = name, \
	.ops = &pmic_ops, \
	.ramp_delay_uv_per_us = 2200, \
	.enable_ramp_delay_us = 1000, \
}

static const struct regul_ops pmic_sw_ops = {
	.set_state = pmic_set_state,
	.get_state = pmic_get_state,
	.set_flag = pmic_set_flag,
	.lock = driver_lock,
	.unlock = driver_unlock,
};

#define DEFINE_SWITCH(name) { \
	.node_name = name, \
	.ops = &pmic_sw_ops, \
	.enable_ramp_delay_us = 1000, \
}

static struct regul_desc pmic_reguls[] = {
	DEFINE_REGU("buck1"),
	DEFINE_REGU("buck2"),
	DEFINE_REGU("buck3"),
	DEFINE_REGU("buck4"),
	DEFINE_REGU("ldo1"),
	DEFINE_REGU("ldo2"),
	DEFINE_REGU("ldo3"),
	DEFINE_REGU("ldo4"),
	DEFINE_REGU("ldo5"),
	DEFINE_REGU("ldo6"),
	DEFINE_REGU("vref_ddr"),
	DEFINE_REGU("boost"),
	DEFINE_SWITCH("pwr_sw1"),
	DEFINE_SWITCH("pwr_sw2"),
};
DECLARE_KEEP_PAGER(pmic_reguls);

struct regul_property {
	const char *name;
	uint16_t flag;
};

static struct regul_property flag_prop[] = {
	{
		.name = "st,mask-reset",
		.flag = STPMIC1_REGUL_MASK_RESET,
	},
	{
		.name = "st,regulator-sink-source",
		.flag = STPMIC1_REGUL_SINK_SOURCE,
	},
	{
		.name = "st,regulator-bypass",
		.flag = STPMIC1_REGUL_ENABLE_BYPASS,
	},
};

static TEE_Result parse_properties(const void *fdt,
				   const struct regul_desc *desc, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct regul_property *p = NULL;

	stm32mp_get_pmic();

	for (p = flag_prop; p < (flag_prop + ARRAY_SIZE(flag_prop)); p++) {
		if (fdt_getprop(fdt, node, p->name, NULL)) {
			FMSG("%s: %#"PRIx32, desc->node_name, p->flag);

			res = pmic_set_flag(desc, p->flag);
			if (res)
				return res;
		}
	}

	stm32mp_put_pmic();

	return TEE_SUCCESS;
}

static void parse_low_power_mode(const void *fdt,
				 const struct regul_desc *desc,
				 int node, int mode)
{
	struct regu_handle_s *regu = (struct regu_handle_s *)desc->driver_data;
	const fdt32_t *cuint = NULL;

	regu->lp_state[mode] = 0;

	if (fdt_getprop(fdt, node, "regulator-off-in-suspend", NULL)) {
		FMSG("%s: mode:%d OFF", desc->node_name, mode);
		regu->lp_state[mode] |= STPMIC1_LP_STATE_OFF;
	} else if (fdt_getprop(fdt, node, "regulator-on-in-suspend", NULL)) {
		FMSG("%s: mode:%d ON", desc->node_name, mode);
		regu->lp_state[mode] |= STPMIC1_LP_STATE_ON;
	} else {
		regu->lp_state[mode] |= STPMIC1_LP_STATE_UNCHANGED;
	}

	cuint = fdt_getprop(fdt, node, "regulator-suspend-microvolt", NULL);
	if (cuint) {
		uint16_t mv = (uint16_t)(fdt32_to_cpu(*cuint) / 1000U);

		FMSG("%s: mode:%d suspend mv=%"PRIu16, desc->node_name,
		     mode, mv);

		regu->lp_state[mode] |= STPMIC1_LP_STATE_SET_VOLT;
		regu->lp_mv[mode] = mv;
	}
}

static void parse_low_power_modes(const void *fdt,
				  const struct regul_desc *desc, int node)
{
	int mode_count = STM32_PM_MAX_SOC_MODE;
	int mode = 0;

	for (mode = 0; mode < mode_count; mode++) {
		const char *lp_mode_name = plat_get_lp_mode_name(mode);

		if (lp_mode_name) {
			int n = 0;

			/* Get the configs from regulator_state_node subnode */
			n = fdt_subnode_offset(fdt, node, lp_mode_name);
			if (n >= 0)
				parse_low_power_mode(fdt, desc, n, mode);
		}
	}
}

#ifdef CFG_PM
static TEE_Result stpmic1_regu_pm(enum pm_op op, uint32_t pm_hint,
				  const struct pm_callback_handle *pm_handle)
{
	TEE_Result res = TEE_SUCCESS;
	struct regul_desc *desc = (struct regul_desc *)pm_handle->handle;
	struct regu_handle_s *regu = (struct regu_handle_s *)desc->driver_data;

	if (op == PM_OP_SUSPEND) {
		stm32mp_get_pmic();

		res = driver_suspend(desc, regu->lp_state[pm_hint],
				     regu->lp_mv[pm_hint]);

		stm32mp_put_pmic();
	}

	return res;
}
DECLARE_KEEP_PAGER(stpmic1_regu_pm);
#endif

static TEE_Result register_pmic_regulator(const void *fdt,
					  const char *regu_name, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t i = 0;
	struct regul_desc *desc = NULL;
	struct regu_handle_s *regu = NULL;

	for (i = 0; i < ARRAY_SIZE(pmic_reguls); i++)
		if (!strcmp(pmic_reguls[i].node_name, regu_name))
			break;

	assert(i < ARRAY_SIZE(pmic_reguls));

	regu = calloc(1, sizeof(*regu));
	if (!regu)
		panic();

	desc = &pmic_reguls[i];
	desc->driver_data = regu;
	regu->desc = desc;

	res = parse_properties(fdt, desc, node);
	if (res)
		EMSG("Failed to parse properties for %s", regu_name);

	parse_low_power_modes(fdt, desc, node);

	res = regulator_register(desc, node);
	if (res) {
		EMSG("Failed to register %s, error: %#"PRIx32, regu_name, res);
		free(regu);
		return res;
	}

#ifdef CFG_PM
	register_pm_core_service_cb(stpmic1_regu_pm, desc, desc->node_name);
#endif

	return res;
}
#endif /* CFG_REGULATOR_DRIVERS */

/*
 * Automated suspend/resume at system suspend/resume is expected
 * only when the PMIC is secure. If it is non secure, only atomic
 * execution context can get/put the PMIC resources.
 */
static TEE_Result pmic_pm(enum pm_op op, uint32_t pm_hint __unused,
			  const struct pm_callback_handle *pm_handle __unused)
{
	if (op == PM_OP_SUSPEND)
		stm32_i2c_suspend(i2c_pmic_handle);
	else
		stm32_i2c_resume(i2c_pmic_handle);

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(pmic_pm);

/* stm32mp_get/put_pmic allows secure atomic sequences to use non secure PMIC */
void stm32mp_get_pmic(void)
{
	if (!pmic_is_secure())
		stm32_i2c_resume(i2c_pmic_handle);
}

void stm32mp_put_pmic(void)
{
	if (!pmic_is_secure())
		stm32_i2c_suspend(i2c_pmic_handle);
}

/* stm32mp_pm_get/put_pmic enforces get/put PMIC accesses */
void stm32mp_pm_get_pmic(void)
{
	stm32_i2c_resume(i2c_pmic_handle);
}

void stm32mp_pm_put_pmic(void)
{
	stm32_i2c_suspend(i2c_pmic_handle);
}

static void register_non_secure_pmic(void)
{
	unsigned int __maybe_unused clock_id = 0;

	/* Allow this function to be called when STPMIC1 not used */
	if (!i2c_pmic_handle->base.pa)
		return;

	if (stm32_pinctrl_set_secure_cfg(i2c_pmic_handle->pinctrl_list, false))
		panic();

	if (IS_ENABLED(CFG_STM32MP15)) {
		stm32mp_register_non_secure_periph_iomem(i2c_pmic_handle->base.pa);
		/*
		 * Non secure PMIC can be used by secure world during power
		 * state transition when non-secure world is suspended.
		 * Therefore secure the I2C clock parents, if not specifically
		 * the I2C clock itself.
		 */
		clock_id = stm32mp_rcc_clk_to_clock_id(i2c_pmic_handle->clock);
		stm32mp_register_clock_parents_secure(clock_id);
	}
}

static enum itr_return stpmic1_irq_handler(struct itr_handler *handler __unused)
{
	uint8_t read_val = 0U;
	unsigned int i = 0U;

	FMSG("Stpmic1 irq handler");

	stm32mp_get_pmic();

	for (i = 0U; i < 4U; i++) {
		if (stpmic1_register_read(ITLATCH1_REG + i, &read_val))
			panic();

		if (read_val) {
			struct pmic_it_handle_s *prv = NULL;

			FMSG("Stpmic1 irq pending %u: %#"PRIx8, i, read_val);

			if (stpmic1_register_write(ITCLEARLATCH1_REG + i,
						   read_val))
				panic();

			SLIST_FOREACH(prv, &pmic_it_handle_list, link)
				if ((prv->pmic_reg == ITCLEARMASK1_REG + i) &&
				    (read_val & BIT(prv->pmic_bit))) {
					FMSG("STPMIC1 send notif %u",
					     prv->notif_id);

					notif_send_it(prv->notif_id);
				}
		}
	}

	stm32mp_put_pmic();

	return ITRR_HANDLED;
}

static TEE_Result init_pmic_secure_state(void)
{
	TEE_Result res = TEE_SUCCESS;
	const struct stm32_firewall_cfg sec_cfg[] = {
		{ FWLL_SEC_RW | FWLL_MASTER(0) },
		{ }, /* Null terminated */
	};

	res = stm32_firewall_check_access(i2c_pmic_handle->base.pa,
					  0, sec_cfg);

	if (!res)
		pmic_status = DT_STATUS_OK_SEC;
	else
		pmic_status = DT_STATUS_OK_NSEC;

	return res;
}

static void initialize_pmic(const void *fdt, int pmic_node)
{
	unsigned long pmic_version = 0;

	if (init_pmic_secure_state())
		register_non_secure_pmic();

	initialize_pmic_i2c(fdt, pmic_node);

	stm32mp_get_pmic();

	if (stpmic1_get_version(&pmic_version))
		panic("Failed to access PMIC");

	DMSG("PMIC version = 0x%02lx", pmic_version);
	stpmic1_dump_regulators();

	if (stpmic1_powerctrl_on())
		panic("Failed to enable PMIC pwr control");

	register_pm_core_service_cb(pmic_pm, NULL, "stm32mp1-pmic");

	parse_regulator_fdt_nodes();

	stm32mp_put_pmic();
}

static TEE_Result stm32_pmic_init_it(const void *fdt, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	const uint32_t *notif_ids = NULL;
	int nb_notif = 0;
	size_t pwr_it = 0;
	struct itr_handler *hdl = NULL;
	const fdt32_t *cuint = NULL;

	cuint = fdt_getprop(fdt, node, "st,wakeup-pin-number", NULL);
	if (!cuint) {
		IMSG("Missing wake-up pin description");
		return TEE_SUCCESS;
	}

	pwr_it = fdt32_to_cpu(*cuint) - 1U;

	notif_ids = fdt_getprop(fdt, node, "st,notif-it-id", &nb_notif);
	if (!notif_ids)
		return TEE_ERROR_ITEM_NOT_FOUND;

	if (nb_notif > 0) {
		struct pmic_it_handle_s *prv = NULL;
		unsigned int i = 0;
		const uint32_t *pmic_its = NULL;
		int nb_it = 0;

		pmic_its = fdt_getprop(fdt, node, "st,pmic-it-id", &nb_it);
		if (!pmic_its)
			return TEE_ERROR_ITEM_NOT_FOUND;

		if (nb_it != nb_notif)
			panic("st,notif-it-id incorrect description");

		for (i = 0; i < (nb_notif / sizeof(uint32_t)); i++) {
			uint8_t val = 0;
			uint8_t pmic_it = 0;

			prv = calloc(1, sizeof(*prv));
			if (!prv)
				panic("pmic: Could not allocate pmic it");

			pmic_it = fdt32_to_cpu(pmic_its[i]);

			assert(pmic_it <= IT_SWIN_R);

			prv->pmic_reg = ITCLEARMASK1_REG + pmic_it / U(8);
			prv->pmic_bit = pmic_it % U(8);
			prv->notif_id = fdt32_to_cpu(notif_ids[i]);

			SLIST_INSERT_HEAD(&pmic_it_handle_list, prv, link);

			stm32mp_get_pmic();

			/* Enable requested interrupt */
			if (stpmic1_register_read(prv->pmic_reg, &val))
				panic();

			val |= BIT(prv->pmic_bit);

			if (stpmic1_register_write(prv->pmic_reg, val))
				panic();

			stm32mp_put_pmic();

			FMSG("STPMIC1 forwards irq reg:%u bit:%u as notif:%u",
			     prv->pmic_reg, prv->pmic_bit, prv->notif_id);
		}
	}

	res = stm32mp1_pwr_itr_alloc_add(pwr_it, stpmic1_irq_handler,
					 PWR_WKUP_FLAG_FALLING |
					 PWR_WKUP_FLAG_THREADED,
					 NULL, &hdl);
	if (res)
		panic("pmic: Could not allocate itr");

	stm32mp1_pwr_itr_enable(hdl->it);

	return res;
}

static TEE_Result stm32_pmic_probe(const void *fdt, int node,
				   const void *compat_data __unused)
{
	TEE_Result res = TEE_SUCCESS;
	const fdt32_t *cuint = NULL;
	int len = 0;
	uint32_t phandle = 0;

	if (IS_ENABLED(CFG_STM32MP13)) {
		cuint = fdt_getprop(fdt, node, "wakeup-parent", &len);
		if (!cuint || len != sizeof(uint32_t))
			panic("Missing wakeup-parent");

		phandle = fdt32_to_cpu(*cuint);
		if (!dt_driver_get_provider_by_phandle(phandle,
						       DT_DRIVER_NOTYPE))
			return TEE_ERROR_DEFER_DRIVER_INIT;
	}

	res = i2c_dt_get_by_subnode(fdt, node, &i2c_pmic_handle);
	if (res)
		return res;

	initialize_pmic(fdt, node);

	if (IS_ENABLED(CFG_STM32MP13))
		res = stm32_pmic_init_it(fdt, node);

	return res;
}

static const struct dt_device_match stm32_pmic_match_table[] = {
	{ .compatible = "st,stpmic1" },
	{ }
};

DEFINE_DT_DRIVER(stm32_pmic_dt_driver) = {
	.name = "stm32_pmic",
	.match_table = stm32_pmic_match_table,
	.probe = stm32_pmic_probe,
};
