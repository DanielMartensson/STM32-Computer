// SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0+)
/*
 * Copyright (c) 2021-2023, STMicroelectronics
 */

#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/regulator.h>
#include <initcall.h>
#include <io.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/pm.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <stdio.h>

/* STM32 VREFBUF registers */
#define STM32_VREFBUF_CSR		U(0x00)

/* STM32 VREFBUF CSR bitfields */

/* VRS bit 3 is unused because the voltage is not specified */
#define STM32_VRS			GENMASK_32(5, 4)
#define STM32_VRS_SHIFT			U(4)

#define STM32_VRR			BIT(3)
#define STM32_HIZ			BIT(1)
#define STM32_ENVR			BIT(0)

#define VREFBUF_REGULATOR_NAME_LEN	U(24)

#define TIMEOUT_US_10MS			U(10 * 1000)
#define TIMEOUT_US_1MS			U(1 * 1000)

struct stm32_vrefbuf_compat_data {
	const uint16_t *voltages;
	uint8_t nb_voltages;
	bool invert_voltages;
};

static const uint16_t stm32_vrefbuf_mp25_voltages[2] = {
	U(1210), U(1500)
};

static const struct stm32_vrefbuf_compat_data stm32_vrefbuf_mp25 = {
	.voltages = stm32_vrefbuf_mp25_voltages,
	.nb_voltages = ARRAY_SIZE(stm32_vrefbuf_mp25_voltages),
	.invert_voltages = false,
};

static const uint16_t stm32_vrefbuf_mp13_voltages[4] = {
	U(1650), U(1800), U(2048), U(2500)
};

static const struct stm32_vrefbuf_compat_data stm32_vrefbuf_mp13 = {
	.voltages = stm32_vrefbuf_mp13_voltages,
	.nb_voltages = ARRAY_SIZE(stm32_vrefbuf_mp13_voltages),
	.invert_voltages = true,
};

struct vrefbuf_regul {
	char name[VREFBUF_REGULATOR_NAME_LEN];
	struct io_pa_va base;
	struct regul_desc desc;
	struct clk *clock;
	uint32_t pm_val;
	uint64_t disable_timeout;
	struct stm32_vrefbuf_compat_data *comp_data;
};

static vaddr_t stm32mp_vrefbuf_base(struct vrefbuf_regul *vr)
{
	return io_pa_or_va_secure(&vr->base, 1);
}

static struct mutex vrefbuf_mu = MUTEX_INITIALIZER;

static TEE_Result vrefbuf_wait_ready(struct vrefbuf_regul *vr)
{
	uint64_t to = timeout_init_us(TIMEOUT_US_10MS);
	uintptr_t reg = stm32mp_vrefbuf_base(vr) + STM32_VREFBUF_CSR;

	while (!timeout_elapsed(to))
		if ((io_read32(reg) & STM32_VRR))
			break;

	if ((io_read32(reg) & STM32_VRR))
		return TEE_SUCCESS;

	return TEE_ERROR_GENERIC;
}

static TEE_Result vrefbuf_set_state(const struct regul_desc *desc, bool enable)
{
	struct vrefbuf_regul *vr = (struct vrefbuf_regul *)desc->driver_data;
	uintptr_t reg = stm32mp_vrefbuf_base(vr) + STM32_VREFBUF_CSR;

	clk_enable(vr->clock);

	if (enable) {
		io_clrbits32(reg, STM32_HIZ);

		/*
		 * If first enable after boot,
		 * or if it was disabled since less than 1ms,
		 * then wait for 1ms in pull down mode to avoid an overshoot.
		 */
		if (vr->disable_timeout == 0 ||
		    !timeout_elapsed(vr->disable_timeout))
			udelay(1000);

		io_setbits32(reg, STM32_ENVR);

		if (vrefbuf_wait_ready(vr) != TEE_SUCCESS) {
			clk_disable(vr->clock);

			return TEE_ERROR_GENERIC;
		}
	} else {
		io_clrbits32(reg, STM32_ENVR);

		vr->disable_timeout = timeout_init_us(TIMEOUT_US_1MS);
	}

	clk_disable(vr->clock);

	return TEE_SUCCESS;
}

static TEE_Result vrefbuf_get_state(const struct regul_desc *desc,
				    bool *enabled)
{
	struct vrefbuf_regul *vr = (struct vrefbuf_regul *)desc->driver_data;
	uintptr_t reg = stm32mp_vrefbuf_base(vr) + STM32_VREFBUF_CSR;

	clk_enable(vr->clock);

	*enabled = io_read32(reg) & STM32_VRR;

	clk_disable(vr->clock);

	return TEE_SUCCESS;
}

static TEE_Result vrefbuf_get_voltage(const struct regul_desc *desc,
				      uint16_t *mv)
{
	struct vrefbuf_regul *vr = (struct vrefbuf_regul *)desc->driver_data;
	uintptr_t reg = stm32mp_vrefbuf_base(vr) + STM32_VREFBUF_CSR;
	uint32_t index = 0;

	clk_enable(vr->clock);

	if (vr->comp_data->invert_voltages)
		index = ((~io_read32(reg)) & STM32_VRS) >> STM32_VRS_SHIFT;
	else
		index = (io_read32(reg) & STM32_VRS) >> STM32_VRS_SHIFT;

	clk_disable(vr->clock);

	*mv = vr->comp_data->voltages[index];

	return TEE_SUCCESS;
}

static TEE_Result vrefbuf_set_voltage(const struct regul_desc *desc,
				      uint16_t mv)
{
	struct vrefbuf_regul *vr = (struct vrefbuf_regul *)desc->driver_data;
	uintptr_t reg = stm32mp_vrefbuf_base(vr) + STM32_VREFBUF_CSR;
	uint8_t i = 0;

	for (i = 0 ; i < vr->comp_data->nb_voltages ; i++) {
		if (vr->comp_data->voltages[i] == mv) {
			uint32_t val = i << STM32_VRS_SHIFT;

			if (vr->comp_data->invert_voltages)
				val = (~val) & STM32_VRS;

			clk_enable(vr->clock);

			io_clrsetbits32(reg,  STM32_VRS, val);

			clk_disable(vr->clock);

			return TEE_SUCCESS;
		}
	}

	EMSG("Failed to set voltage on vrefbuf");

	return TEE_ERROR_GENERIC;
}

static TEE_Result vrefbuf_list_voltages(const struct regul_desc *desc __unused,
					uint16_t **levels, size_t *count)
{
	struct vrefbuf_regul *vr = (struct vrefbuf_regul *)desc->driver_data;
	const uint16_t **vrefbuf = (const uint16_t **)levels;

	*count = vr->comp_data->nb_voltages;
	*vrefbuf = vr->comp_data->voltages;

	return TEE_SUCCESS;
}

static void vrefbuf_lock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_lock(&vrefbuf_mu);
}

static void vrefbuf_unlock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_unlock(&vrefbuf_mu);
}

struct regul_ops vrefbuf_ops = {
	.set_state = vrefbuf_set_state,
	.get_state = vrefbuf_get_state,
	.set_voltage = vrefbuf_set_voltage,
	.get_voltage = vrefbuf_get_voltage,
	.list_voltages = vrefbuf_list_voltages,
	.lock = vrefbuf_lock,
	.unlock = vrefbuf_unlock,
};

static TEE_Result vrefbuf_pm(enum pm_op op, unsigned int pm_hint __unused,
			     const struct pm_callback_handle *hdl)
{
	struct vrefbuf_regul *vr = (struct vrefbuf_regul *)hdl->handle;
	uintptr_t reg = stm32mp_vrefbuf_base(vr) + STM32_VREFBUF_CSR;

	assert(op == PM_OP_SUSPEND || op == PM_OP_RESUME);

	clk_enable(vr->clock);

	if (op == PM_OP_SUSPEND) {
		FMSG("Vrefbuf suspend");
		vr->pm_val = io_read32(reg);

		if ((vr->pm_val & STM32_ENVR) &&
		    (vrefbuf_wait_ready(vr) != TEE_SUCCESS)) {
			clk_disable(vr->clock);

			return TEE_ERROR_GENERIC;
		}
	} else {
		FMSG("Vrefbuf resume");

		io_clrsetbits32(reg,  STM32_VRS, vr->pm_val);

		if (vr->pm_val & STM32_ENVR) {
			vr->disable_timeout = 0;
			vrefbuf_set_state(&vr->desc, true);
		}
	}

	clk_disable(vr->clock);

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(vrefbuf_pm);

static TEE_Result stm32_vrefbuf_regulator_probe(const void *fdt, int node,
						const void *comp_data)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct dt_node_info info = { };
	size_t __maybe_unused len = 0;
	const char *reg_name = NULL;
	struct vrefbuf_regul *vr = NULL;
	struct io_pa_va base = { };
	struct clk *clk = NULL;

	FMSG("vrefbuf init");

	res = clk_dt_get_by_index(fdt, node, 0, &clk);
	if (res)
		return res;

	_fdt_fill_device_info(fdt, &info, node);

	vr = calloc(1, sizeof(*vr));
	if (!vr)
		return TEE_ERROR_OUT_OF_MEMORY;

	reg_name = fdt_get_name(fdt, node, NULL);
	len = snprintf(vr->name, sizeof(vr->name) - 1, "%s", reg_name);
	assert(len > 0 && len < (sizeof(vr->name) - 1));

	base.pa = info.reg;
	io_pa_or_va_secure(&base, info.reg_size);
	if (!base.va)
		panic();

	vr->base = base;
	vr->desc.node_name = vr->name;
	vr->desc.driver_data = vr;
	vr->desc.ops = &vrefbuf_ops;
	vr->desc.supply_name = "vdda";
	vr->clock = clk;
	vr->comp_data = (struct stm32_vrefbuf_compat_data *)comp_data;

	res = regulator_register(&vr->desc, node);
	if (res) {
		EMSG("Failed to register vrefbuf");
		free(vr);
		return res;
	}

	register_pm_driver_cb(vrefbuf_pm, (void *)vr, "vrefbuf-driver");

	return TEE_SUCCESS;
}

static const struct dt_device_match vrefbuf_match_table[] = {
	{
		.compatible = "st,stm32mp25-vrefbuf",
		.compat_data = (void *)&stm32_vrefbuf_mp25,
	},
	{
		.compatible = "st,stm32mp13-vrefbuf",
		.compat_data = (void *)&stm32_vrefbuf_mp13,
	},
	{ }
};

DEFINE_DT_DRIVER(vrefbuf_regulator_dt_driver) = {
	.name = "stm32-vrefbuf-regulator",
	.match_table = vrefbuf_match_table,
	.probe = &stm32_vrefbuf_regulator_probe,
};
