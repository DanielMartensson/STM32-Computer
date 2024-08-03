// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020-2021, STMicroelectronics - All Rights Reserved
 */

#include <assert.h>
#include <compiler.h>
#include <drivers/regulator.h>
#include <initcall.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <libfdt.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

static SLIST_HEAD(, rdev) regulator_device_list = SLIST_HEAD_INITIALIZER(rdev);

static void lock_driver(const struct rdev *rdev)
{
	if (rdev->desc->ops->lock)
		rdev->desc->ops->lock(rdev->desc);
}

static void unlock_driver(const struct rdev *rdev)
{
	if (rdev->desc->ops->unlock)
		rdev->desc->ops->unlock(rdev->desc);
}

static struct rdev *regulator_get_by_phandle(int32_t phandle)
{
	struct rdev *rdev = NULL;

	SLIST_FOREACH(rdev, &regulator_device_list, link)
		if (rdev->phandle == phandle)
			return rdev;

	DMSG("phandle %"PRId32" not found", phandle);
	return NULL;
}

/*
 * Get a regulator from its node name
 *
 * @node_name - name of the node "ldo1"
 * Return pointer to rdev if succeed, NULL else.
 */
struct rdev *regulator_get_by_node_name(const char *node_name)
{
	struct rdev *rdev = NULL;

	assert(node_name);

	SLIST_FOREACH(rdev, &regulator_device_list, link)
		if (!strcmp(rdev->desc->node_name, node_name))
			return rdev;

	EMSG("%s not found", node_name);
	return NULL;
}

/*
 * Get a regulator from its regulator name property value
 *
 * @reg_name - target value of regulator-name property
 * Return pointer to rdev if succeed, NULL else.
 */
struct rdev *regulator_get_by_regulator_name(const char *reg_name)
{
	struct rdev *rdev = NULL;

	assert(reg_name);

	SLIST_FOREACH(rdev, &regulator_device_list, link)
		if (rdev->reg_name && !strcmp(rdev->reg_name, reg_name))
			return rdev;

	EMSG("%s not found", reg_name);
	return NULL;
}

#ifdef CFG_EMBED_DTB
#define MAX_PROPERTY_LEN	64

static int32_t get_supply_phandle(const void *fdt, int node, const char *name)
{
	const fdt32_t *cuint = NULL;
	int __maybe_unused len = 0;
	int supply_phandle = -FDT_ERR_NOTFOUND;
	char prop_name[MAX_PROPERTY_LEN] = { 0 };

	len = snprintf(prop_name, sizeof(prop_name) - 1, "%s-supply", name);
	assert((len >= 0) && ((size_t)len < sizeof(prop_name) - 1));

	cuint = fdt_getprop(fdt, node, prop_name, NULL);
	if (cuint) {
		supply_phandle = fdt32_to_cpu(*cuint);
		FMSG("%s: supplied by %d", name, supply_phandle);
	}

	return supply_phandle;
}

/*
 * Get a regulator from a supply name
 *
 * @fdt - pointer to device tree memory
 * @node - offset of the node that contains the supply description
 * @name - name of the supply, i.e. "vdd" for "vdd-supply"
 * Return pointer to rdev if succeed, NULL else.
 */
struct rdev *regulator_get_by_supply_name(const void *fdt, int node,
					  const char *name)
{
	const int p = get_supply_phandle(fdt, node, name);

	if (p < 0)
		return NULL;

	return regulator_get_by_phandle(p);
}
#endif /* CFG_EMBED_DTB */

static TEE_Result _regulator_set_state(struct rdev *rdev, bool on_not_off)
{
	if (!rdev->desc->ops->set_state)
		return TEE_ERROR_GENERIC;

	return rdev->desc->ops->set_state(rdev->desc, on_not_off);
}

static TEE_Result _regulator_enable(struct rdev *rdev)
{
	FMSG("%s", rdev->desc->node_name);

	if (!rdev->desc->ops->set_state)
		return TEE_ERROR_GENERIC;

	if (rdev->supply_dev) {
		TEE_Result res = regulator_enable(rdev->supply_dev);

		if (res)
			return res;
	}

	lock_driver(rdev);

	if (!rdev->use_count) {
		TEE_Result res = _regulator_set_state(rdev, true);

		if (res) {
			EMSG("regul %s set state failed with %#"PRIx32,
			     rdev->desc->node_name, res);

			unlock_driver(rdev);

			if (rdev->supply_dev &&
			    regulator_disable(rdev->supply_dev))
				panic();

			return res;
		}

		udelay(rdev->enable_ramp_delay_us);
	}

	rdev->use_count++;
	assert(rdev->use_count != UINT8_MAX);

	unlock_driver(rdev);

	FMSG("%s refcount: %"PRIu8, rdev->desc->node_name, rdev->use_count);

	return TEE_SUCCESS;
}

/*
 * Enable regulator and its supplies
 *
 * @rdev - regulator device
 */
TEE_Result regulator_enable(struct rdev *rdev)
{
	assert(rdev);

	if (rdev->flags & REGUL_ALWAYS_ON)
		return TEE_SUCCESS;

	return _regulator_enable(rdev);
}

static TEE_Result _regulator_disable(struct rdev *rdev)
{
	FMSG("%s", rdev->desc->node_name);

	if (!rdev->desc->ops->set_state)
		return TEE_ERROR_GENERIC;

	lock_driver(rdev);

	if (rdev->use_count == 1) {
		TEE_Result res = _regulator_set_state(rdev, false);

		if (res) {
			EMSG("regul %s set state failed with %#"PRIx32,
			     rdev->desc->node_name, res);
			unlock_driver(rdev);
			return res;
		}
	}

	if (!rdev->use_count) {
		EMSG("Unbalanced %s", rdev->desc->node_name);
		panic();
	}

	rdev->use_count--;

	FMSG("%s refcount:%"PRIu8, rdev->desc->node_name, rdev->use_count);

	unlock_driver(rdev);

	if (rdev->supply_dev && regulator_disable(rdev->supply_dev)) {
		/* We can't leave this unbalanced */
		panic("can't disable supply");
	}

	return TEE_SUCCESS;
}

/*
 * Disable regulator and its supplies
 *
 * @rdev - regulator device
 */
TEE_Result regulator_disable(struct rdev *rdev)
{
	assert(rdev);

	if (rdev->flags & REGUL_ALWAYS_ON)
		return TEE_SUCCESS;

	return _regulator_disable(rdev);
}

/*
 * Return whether the regulator is enabled or not
 *
 * @rdev - regulator device
 */
bool regulator_is_enabled(const struct rdev *rdev)
{
	TEE_Result res = TEE_SUCCESS;
	bool enabled = false;

	if (!rdev->desc->ops->get_state)
		return TEE_ERROR_GENERIC;

	lock_driver(rdev);
	res = rdev->desc->ops->get_state(rdev->desc, &enabled);
	unlock_driver(rdev);

	if (res)
		EMSG("regul %s get state failed with %#"PRIx32,
		     rdev->desc->node_name, res);

	return !res && enabled;
}

/*
 * Set regulator voltage
 *
 * @rdev - regulator device
 * @mvolt - Target voltage level in millivolt
 * Return 0 if succeed, non 0 else.
 */
TEE_Result regulator_set_voltage(struct rdev *rdev, uint16_t mvolt)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint16_t cur_mv = 0;

	assert(rdev);

	FMSG("%s %"PRIu16"mV", rdev->desc->node_name, mvolt);

	if (mvolt < rdev->min_mv || mvolt > rdev->max_mv)
		return TEE_ERROR_BAD_PARAMETERS;

	res = regulator_get_voltage(rdev, &cur_mv);
	if (res)
		return res;
	if (cur_mv == mvolt)
		return TEE_SUCCESS;

	if (!rdev->desc->ops->set_voltage)
		return TEE_ERROR_NOT_IMPLEMENTED;

	lock_driver(rdev);
	res = rdev->desc->ops->set_voltage(rdev->desc, mvolt);
	unlock_driver(rdev);

	if (res) {
		EMSG("regul %s set volt failed with %#"PRIx32,
		     rdev->desc->node_name, res);
		return res;
	}

	if (rdev->ramp_delay_uv_per_us > 0) {
		unsigned int d = 0;

		if (cur_mv > mvolt)
			d = cur_mv - mvolt;
		else
			d = mvolt - cur_mv;

		d = (d * 1000) / rdev->ramp_delay_uv_per_us;

		FMSG("%s %"PRIu32"uS", rdev->desc->node_name, d);
		udelay(d);
	}

	return res;
}

/* Set regulator to its min voltage level */
TEE_Result regulator_set_min_voltage(struct rdev *rdev)
{
	return regulator_set_voltage(rdev, rdev->min_mv);
}

/*
 * Get regulator voltage level in millivolt
 *
 * @rdev - regulator device
 * @level_mv - Output voltage level
 */
TEE_Result regulator_get_voltage(const struct rdev *rdev, uint16_t *level_mv)
{
	TEE_Result res = TEE_SUCCESS;

	assert(rdev && level_mv);

	if (!rdev->desc->ops->get_voltage) {
		*level_mv = rdev->min_mv;
		return TEE_SUCCESS;
	}

	lock_driver(rdev);
	res = rdev->desc->ops->get_voltage(rdev->desc, level_mv);
	unlock_driver(rdev);

	if (res)
		EMSG("regul %s get voltage failed with %#"PRIx32,
		     rdev->desc->node_name, res);

	return res;
}

/*
 * List regulator voltages
 *
 * @rdev - regulator device
 * @levels - optional output ref to supported millitvolt levels from min to max
 * @count - output number of supported millivolt levels
 */
TEE_Result regulator_list_voltages(struct rdev *rdev, uint16_t **levels,
				   size_t *count)
{
	TEE_Result res = TEE_SUCCESS;
	uint16_t *ref = NULL;
	size_t ref_cnt = 0;
	size_t n = 0;

	assert(rdev && count);

	if (!rdev->desc->ops->list_voltages) {
		if (levels)
			*levels = &rdev->min_mv;
		*count = 1;
		return TEE_SUCCESS;
	}

	lock_driver(rdev);
	res = rdev->desc->ops->list_voltages(rdev->desc, &ref, &ref_cnt);
	unlock_driver(rdev);

	if (res) {
		/* Caller may be query the level table size */
		if (res != TEE_ERROR_SHORT_BUFFER)
			DMSG("regul %s list_voltages failed: res = %#"PRIx32,
			     rdev->desc->node_name, res);

		return res;
	}

	/* Reduce the possible values depending on min and max from DT */
	n = ref_cnt;
	while ((ref[n - 1U] > rdev->max_mv) && (n > 1))
		n--;

	/* Verify that max val is a valid value */
	if (rdev->max_mv != ref[n - 1]) {
		EMSG("regul %s: max value %"PRIu16" is invalid",
		     rdev->desc->node_name, rdev->max_mv);
		return TEE_ERROR_GENERIC;
	}

	while (*ref < rdev->min_mv && n > 1) {
		ref++;
		n--;
	}

	if (!n) {
		EMSG("regul %s set min voltage is too high",
		     rdev->desc->node_name);
		return TEE_ERROR_GENERIC;
	}

	if (rdev->min_mv != *ref) {
		EMSG("regul %s: min value %u is invalid",
		     rdev->desc->node_name, rdev->min_mv);
		return TEE_ERROR_GENERIC;
	}

	*count = n;
	if (levels)
		*levels = ref;

	FMSG("min_mv=%"PRIu16" max_mv=%"PRIu16, rdev->min_mv, rdev->max_mv);

	return TEE_SUCCESS;
}

/*
 * Get regulator voltages range
 *
 * @rdev - regulator device
 * @min_mv - out: min possible millivolt value
 * @max_mv - out: max possible millivolt value
 * Return 0 if succeed, non 0 else.
 */
void regulator_get_range(const struct rdev *rdev, uint16_t *min_mv,
			 uint16_t *max_mv)
{
	if (min_mv)
		*min_mv = rdev->min_mv;

	if (max_mv)
		*max_mv = rdev->max_mv;
}

/*
 * Set regulator flag
 *
 * @rdev - regulator device
 * @flag - flag value to set (eg: REGUL_OCP)
 */
TEE_Result regulator_set_flag(struct rdev *rdev, uint16_t flag)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	/* check that only one bit is set on flag */
	if (!IS_POWER_OF_TWO(flag))
		return TEE_ERROR_GENERIC;

	/* REGUL_ALWAYS_ON and REGUL_BOOT_ON are internal properties of the core */
	if ((flag == REGUL_ALWAYS_ON) || (flag == REGUL_BOOT_ON)) {
		rdev->flags |= flag;
		return TEE_SUCCESS;
	}

	if (!rdev->desc->ops->set_flag) {
		EMSG("%s lacks set_flag handler", rdev->desc->node_name);
		return TEE_ERROR_GENERIC;
	}

	lock_driver(rdev);
	res = rdev->desc->ops->set_flag(rdev->desc, flag);
	unlock_driver(rdev);

	if (res) {
		EMSG("%s: set flag %#"PRIx16" failed with %#"PRIx32,
		     rdev->desc->node_name, flag, res);
		return res;
	}

	rdev->flags |= flag;

	return TEE_SUCCESS;
}

#ifdef CFG_EMBED_DTB
struct regul_property {
	const char *name;
	uint16_t flag;
};

static struct regul_property flag_prop[] = {
	{
		.name = "regulator-always-on",
		.flag = REGUL_ALWAYS_ON,
	},
	{
		.name = "regulator-boot-on",
		.flag = REGUL_BOOT_ON,
	},
	{
		.name = "regulator-active-discharge",
		.flag = REGUL_ACTIVE_DISCHARGE,
	},
	{
		.name = "regulator-over-current-protection",
		.flag = REGUL_OCP,
	},
	{
		.name = "regulator-pull-down",
		.flag = REGUL_PULL_DOWN,
	},
};

static TEE_Result parse_properties(const void *fdt, struct rdev *rdev, int node)
{
	TEE_Result res = TEE_SUCCESS;
	const fdt32_t *cuint = NULL;
	struct regul_property *p = NULL;

	for (p = flag_prop; p < (flag_prop + ARRAY_SIZE(flag_prop)); p++) {
		if (fdt_getprop(fdt, node, p->name, NULL)) {
			FMSG("%s: %#"PRIx32, rdev->desc->node_name, p->flag);

			res = regulator_set_flag(rdev, p->flag);
			if (res)
				return res;
		}
	}

	cuint = fdt_getprop(fdt, node, "regulator-ramp-delay", NULL);
	if (cuint) {
		rdev->ramp_delay_uv_per_us = (uint32_t)(fdt32_to_cpu(*cuint));
		FMSG("%s: ramp delay = %"PRIu32" (uV/us)",
		     rdev->desc->node_name, rdev->ramp_delay_uv_per_us);
	}

	cuint = fdt_getprop(fdt, node, "regulator-enable-ramp-delay", NULL);
	if (cuint) {
		rdev->enable_ramp_delay_us = (uint32_t)(fdt32_to_cpu(*cuint));
		FMSG("%s: enable ramp delay = %u (us)", rdev->desc->node_name,
		     rdev->enable_ramp_delay_us);
	}

	rdev->reg_name = fdt_getprop(fdt, node, "regulator-name", NULL);
	if (rdev->reg_name) {
		rdev->reg_name = strdup(rdev->reg_name);
		if (!rdev->reg_name)
			panic();
	}

	return TEE_SUCCESS;
}

static void parse_supply(const void *fdt, struct rdev *rdev, int node)
{
	const char *name = rdev->desc->supply_name;

	if (!name)
		name = rdev->desc->node_name;

	rdev->supply_phandle = get_supply_phandle(fdt, node, name);
	if (rdev->supply_phandle < 0) {
		node = fdt_parent_offset(fdt, node);
		rdev->supply_phandle = get_supply_phandle(fdt, node, name);
	}
}

/*
 * Parse the device-tree for a regulator
 *
 * Read min/max voltage from dt and check its validity
 * Read the properties, and call the driver to set flags
 * Read power supply phandle
 * Read and store low power mode states
 *
 * @rdev - regulator device
 * @node - device-tree node offset of the regulator
 */
static TEE_Result parse_dt(struct rdev *rdev, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	const fdt32_t *cuint = NULL;
	uint16_t *levels = NULL;
	void *fdt = NULL;
	size_t size = 0;

	FMSG("%s: parse embedded DTB", rdev->desc->node_name);

	fdt = get_embedded_dt();
	if (!fdt)
		return TEE_SUCCESS;

	/* Save phandle and DTB reference to identify the regulator */
	rdev->phandle = fdt_get_phandle(fdt, node);

	cuint = fdt_getprop(fdt, node, "regulator-min-microvolt", NULL);
	if (cuint) {
		uint16_t min_mv = (uint16_t)(fdt32_to_cpu(*cuint) / 1000U);

		FMSG("%s: min_mv=%"PRIu16, rdev->desc->node_name, min_mv);
		if (min_mv <= rdev->max_mv) {
			rdev->min_mv = min_mv;
		} else {
			EMSG("%s: min_mv=%"PRIu16" is too high",
			     rdev->desc->node_name, min_mv);
			return TEE_ERROR_GENERIC;
		}
	}

	cuint = fdt_getprop(fdt, node, "regulator-max-microvolt", NULL);
	if (cuint) {
		uint16_t max_mv = (uint16_t)(fdt32_to_cpu(*cuint) / 1000U);

		FMSG("%s: max_mv=%"PRIu16, rdev->desc->node_name, max_mv);
		if (max_mv >= rdev->min_mv) {
			rdev->max_mv = max_mv;
		} else {
			EMSG("%s: max_mv=%"PRIu16" is too low",
			     rdev->desc->node_name, max_mv);
			return TEE_ERROR_GENERIC;
		}
	}

	/* validate that min and max values can be used */
	res = regulator_list_voltages(rdev, &levels, &size);
	if (res)
		return res;

	res = parse_properties(fdt, rdev, node);
	if (res)
		return res;

	parse_supply(fdt, rdev, node);

	return TEE_SUCCESS;
}
#else /* CFG_EMBED_DTB */
static TEE_Result parse_dt(struct rdev *rdev __unused, int node __unused)
{
	return TEE_SUCCESS;
}
#endif /* CFG_EMBED_DTB */

/*
 * Register a regulator driver in regulator framework.
 * Initialize voltage range from driver description
 *
 * @desc - pointer to the regulator description
 * @node - device-tree node offset of the regulator, or 0 if unused
 */
TEE_Result regulator_register(const struct regul_desc *desc, int node)
{
	TEE_Result res = TEE_ERROR_OUT_OF_MEMORY;
	struct rdev *rdev = NULL;
	uint16_t mv = 0;

	assert(desc);

	FMSG("register %s", desc->node_name);

	rdev = calloc(1, sizeof(*rdev));
	if (!rdev)
		return TEE_ERROR_OUT_OF_MEMORY;

	rdev->ramp_delay_uv_per_us = desc->ramp_delay_uv_per_us;
	rdev->enable_ramp_delay_us = desc->enable_ramp_delay_us;
	rdev->desc = desc;

	if (desc->ops->list_voltages) {
		uint16_t *levels = NULL;
		size_t count = 0;
		size_t __maybe_unused i = 0;

		lock_driver(rdev);
		res = desc->ops->list_voltages(desc, &levels, &count);
		unlock_driver(rdev);

		if (res) {
			EMSG("%s list_voltages failed with %#"PRIx32,
			     desc->node_name, res);
			goto out;
		}
		assert(levels && count);
		for (i = 0; i < count - 1; i++)
			assert(levels[i] <= levels[i + 1]);

		rdev->min_mv = levels[0];
		rdev->max_mv = levels[count - 1];
	} else {
		rdev->max_mv = UINT16_MAX;
	}

	res = parse_dt(rdev, node);
	if (res)
		goto out;

	if (regulator_get_voltage(rdev, &mv) == TEE_SUCCESS) {
		if (mv < rdev->min_mv || mv > rdev->max_mv) {
			FMSG("Update regulator %s to %"PRIu16"mV",
			     desc->node_name, rdev->min_mv);

			res = regulator_set_voltage(rdev, rdev->min_mv);
			if (res)
				goto out;
		}
	}

	SLIST_INSERT_HEAD(&regulator_device_list, rdev, link);

out:
	if (res)
		free(rdev);

	return res;
}

/*
 * Suspend a single regulator before low power entry
 * Call regulator suspend call back,
 * Enable the regulator if boot_on flag is set as regulator is needed during
 * boot/resume from suspend sequences.
 *
 * @rdev - regulator device
 * @pm_hint - Power level hint provided by PM framework
 */
static TEE_Result suspend_regulator(struct rdev *rdev,
				    unsigned int pm_hint __unused)
{
	TEE_Result res = TEE_SUCCESS;

	/* Ensure boot-on regulators are enabled when resuming */
	if (rdev->flags & REGUL_BOOT_ON)
		res = regulator_enable(rdev);

	return res;
}

/*
 * Resume a single regulator after low power
 *
 * @rdev - regulator device
 */
static int resume_regulator(struct rdev *rdev)
{
	TEE_Result res = TEE_SUCCESS;

	if (rdev->flags & REGUL_BOOT_ON) {
		/* Balance refcount in suspend sequence */
		res = regulator_disable(rdev);
		if (res)
			EMSG("%s resume failed with %#"PRIx32,
			     rdev->desc->node_name, res);
	}

	return res;
}

static TEE_Result regulator_pm(enum pm_op op, unsigned int pm_hint,
			       const struct pm_callback_handle *hdl __unused)
{
	struct rdev *rdev = NULL;

	assert(op == PM_OP_SUSPEND || op == PM_OP_RESUME);

	if (op == PM_OP_SUSPEND) {
		FMSG("Regulator core suspend");

		SLIST_FOREACH(rdev, &regulator_device_list, link)
			if (suspend_regulator(rdev, pm_hint))
				panic();

		regulator_core_dump();
	} else {
		FMSG("Regulator core resume");

		SLIST_FOREACH(rdev, &regulator_device_list, link)
			if (resume_regulator(rdev))
				panic();
	}

        return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(regulator_pm);

static void __maybe_unused sprint_name(char *dest, const char *src, int len)
{
	int l = 0;

	if (!src)
		src = "";

	l = strlen(src);
	if (l > len)
		l = len;

	memset(dest, ' ', len);
	memcpy(dest, src, l);
	dest[len] = '\0';
}

/*
 * Log regulators state
 */
void regulator_core_dump(void)
{
	struct rdev *rdev = NULL;

	if (TRACE_LEVEL < TRACE_FLOW)
		return;

	FMSG("Dump Regulators\n");
	FMSG("reg      name     use\ten\tmV\tmin\tmax\tflags\tsupply\n");

	SLIST_FOREACH(rdev, &regulator_device_list, link) {
		uint16_t mv = 0;
		uint16_t min_mv = 0;
		uint16_t max_mv = 0;
		char reg[9] = "";
		char name[9] = { 0 };
		const char __maybe_unused *supply = "";

		sprint_name(name, rdev->desc->node_name, 8);
		sprint_name(reg, rdev->reg_name, 8);

		if (regulator_get_voltage(rdev, &mv))
			mv = UINT16_MAX;

		regulator_get_range(rdev, &min_mv, &max_mv);
		if (rdev->supply_dev)
			supply = rdev->supply_dev->desc->node_name;

		FMSG("%s %s %"PRIu8"\t%d\t%"PRIu16"\t%"PRIu16"\t%"PRIu16"\t%#"PRIx16"\t%s\n",
		     reg, name, rdev->use_count, regulator_is_enabled(rdev), mv,
		     min_mv, max_mv, rdev->flags, supply);
	}
}

/*
 * Connect each regulator to its supply
 * Apply min voltage if the voltage is outside the authorized range
 * Enable always-on regulators
 */
static TEE_Result regulator_core_config(void)
{
	TEE_Result res = TEE_SUCCESS;
	struct rdev *rdev = NULL;

	FMSG("");

	SLIST_FOREACH(rdev, &regulator_device_list, link) {
		if (rdev->supply_phandle >= 0) {
			struct rdev *s = NULL;

			FMSG("%s: connect supply", rdev->desc->node_name);

			s = regulator_get_by_phandle(rdev->supply_phandle);
			if (!s)
				return TEE_ERROR_GENERIC;

			rdev->supply_dev = s;
		}
	}

	SLIST_FOREACH(rdev, &regulator_device_list, link) {
		/*
		 * Enable always-on regulator and increment its use_count so that
		 * the regulator is not being disabled during clean-up sequence.
		 */
		if (rdev->flags & REGUL_ALWAYS_ON) {
			res = _regulator_enable(rdev);
			if (res)
				break;
		}
	}

	regulator_core_dump();

	return res;
}

static TEE_Result regulator_core_init(void)
{
	if (regulator_core_config())
		panic();

	register_pm_driver_cb(regulator_pm, NULL, "regulator-driver");

	return TEE_SUCCESS;
}
driver_init_late(regulator_core_init);

/*
 * Clean-up regulators that are not used.
 */
static TEE_Result regulator_core_cleanup(void)
{
	struct rdev *rdev = NULL;

	FMSG("");

	SLIST_FOREACH(rdev, &regulator_device_list, link) {
		if (!rdev->use_count) {
			DMSG("disable %s", rdev->desc->node_name);
			lock_driver(rdev);
			_regulator_set_state(rdev, false /* disable */);
			unlock_driver(rdev);
		}
	}

	return TEE_SUCCESS;
}

release_init_resource(regulator_core_cleanup);
