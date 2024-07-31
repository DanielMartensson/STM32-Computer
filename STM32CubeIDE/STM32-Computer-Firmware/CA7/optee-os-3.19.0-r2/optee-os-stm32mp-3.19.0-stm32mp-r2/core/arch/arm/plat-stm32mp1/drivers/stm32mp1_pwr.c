// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2018-2021, STMicroelectronics
 */

#include <config.h>
#include <drivers/stm32mp1_pwr.h>
#include <drivers/regulator.h>
#include <initcall.h>
#include <io.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/dt_driver.h>
#include <kernel/panic.h>
#include <kernel/thread.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <platform_config.h>

#define TIMEOUT_US_10MS		(10U * 1000U)

#define PWR_CR3_VBE		BIT(8)
#define PWR_CR3_VBRS		BIT(9)

#define PWR_CR3_USB33_EN	BIT(24)
#define PWR_CR3_USB33_RDY	BIT(26)
#define PWR_CR3_REG18_EN	BIT(28)
#define PWR_CR3_REG18_RDY	BIT(29)
#define PWR_CR3_REG11_EN	BIT(30)
#define PWR_CR3_REG11_RDY	BIT(31)

/* Mutex for protecting PMIC accesses */
static struct mutex pwr_regul_mu = MUTEX_INITIALIZER;

struct pwr_regu_desc {
	uint16_t level_mv;
	uint32_t cr3_enable_mask;
	uint32_t cr3_ready_mask;
};

static const struct pwr_regu_desc pwr_regulators[PWR_REGU_COUNT] = {
	 [PWR_REG11] = {
		 .level_mv = 1100,
		 .cr3_enable_mask = PWR_CR3_REG11_EN,
		 .cr3_ready_mask = PWR_CR3_REG11_RDY,
	 },
	 [PWR_REG18] = {
		 .level_mv = 1800,
		 .cr3_enable_mask = PWR_CR3_REG18_EN,
		 .cr3_ready_mask = PWR_CR3_REG18_RDY,
	 },
	 [PWR_USB33] = {
		 .level_mv = 3300,
		 .cr3_enable_mask = PWR_CR3_USB33_EN,
		 .cr3_ready_mask = PWR_CR3_USB33_RDY,
	 },
};

vaddr_t stm32_pwr_base(void)
{
	static struct io_pa_va base = { .pa = PWR_BASE };

	return io_pa_or_va_secure(&base, 1);
}

static TEE_Result pwr_set_state(const struct regul_desc *desc, bool enable)
{
	struct pwr_regu_desc *p = (struct pwr_regu_desc *)desc->driver_data;
	uintptr_t pwr_cr3 = stm32_pwr_base() + PWR_CR3_OFF;
	uint32_t enable_mask = p->cr3_enable_mask;

	if (enable) {
		uint64_t to = 0;
		uint32_t ready_mask = p->cr3_ready_mask;

		io_setbits32(pwr_cr3, enable_mask);

		to = timeout_init_us(TIMEOUT_US_10MS);
		while (!timeout_elapsed(to))
			if (io_read32(pwr_cr3) & ready_mask)
				break;

		if (!(io_read32(pwr_cr3) & ready_mask))
			return TEE_ERROR_GENERIC;
	} else {
		/* Make sure the previous operations are visible */
		dsb();
		io_clrbits32(pwr_cr3, enable_mask);
	}

	return TEE_SUCCESS;
}

static TEE_Result pwr_get_state(const struct regul_desc *desc, bool *enabled)
{
	struct pwr_regu_desc *p = (struct pwr_regu_desc *)desc->driver_data;
	uintptr_t pwr_cr3 = stm32_pwr_base() + PWR_CR3_OFF;

	*enabled = io_read32(pwr_cr3) & p->cr3_enable_mask;

	return TEE_SUCCESS;
}

static TEE_Result pwr_get_voltage(const struct regul_desc *desc, uint16_t *mv)
{
	struct pwr_regu_desc *p = (struct pwr_regu_desc *)desc->driver_data;

	*mv = p->level_mv;

	return TEE_SUCCESS;
}

static TEE_Result pwr_list_voltages(const struct regul_desc *desc,
				    uint16_t **levels, size_t *count)
{
	struct pwr_regu_desc *p = (struct pwr_regu_desc *)desc->driver_data;

	*count = 1;
	*levels = &p->level_mv;

	return TEE_SUCCESS;
}

void stm32mp1_pwr_regul_lock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_lock(&pwr_regul_mu);
}

void stm32mp1_pwr_regul_unlock(const struct regul_desc *desc __unused)
{
	if (thread_get_id_may_fail() != THREAD_ID_INVALID)
		mutex_unlock(&pwr_regul_mu);
}

struct regul_ops pwr_ops = {
	.set_state = pwr_set_state,
	.get_state = pwr_get_state,
	.get_voltage = pwr_get_voltage,
	.list_voltages = pwr_list_voltages,
	.lock = stm32mp1_pwr_regul_lock,
	.unlock = stm32mp1_pwr_regul_unlock,
};

#define DEFINE_REG(id, name, supply) { \
	.node_name = name, \
	.ops = &pwr_ops, \
	.driver_data = &pwr_regulators[id], \
	.supply_name = supply, \
}

static struct regul_desc stm32mp1_pwr_regs[] = {
	[PWR_REG11] = DEFINE_REG(PWR_REG11, "reg11", "vdd"),
	[PWR_REG18] = DEFINE_REG(PWR_REG18, "reg18", "vdd"),
	[PWR_USB33] = DEFINE_REG(PWR_USB33, "usb33", "vdd_3v3_usbfs"),
};
DECLARE_KEEP_PAGER(stm32mp1_pwr_regs);

#ifdef CFG_EMBED_DTB
static TEE_Result stm32mp1_pwr_regu_probe(const void *fdt, int node,
					  const void *compat_data __unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int subnode = 0;

	fdt_for_each_subnode(subnode, fdt, node) {
		const char *reg_name = fdt_get_name(fdt, subnode, NULL);
		const struct regul_desc *desc = NULL;
		unsigned int i = 0;

		for (i = 0; i < ARRAY_SIZE(stm32mp1_pwr_regs); i++) {
			desc = &stm32mp1_pwr_regs[i];
			if (!strcmp(stm32mp1_pwr_regs[i].node_name, reg_name)) {
				res = regulator_register(desc, subnode);
				if (res) {
					EMSG("Can't register %s: %#"PRIx32,
					     reg_name, res);
					panic();
				}

				break;
			}
		}
	}

	if (fdt_getprop(fdt, node, "st,enable-vbat-charge", NULL)) {
		uintptr_t cr3 = stm32_pwr_base() + PWR_CR3_OFF;

		io_setbits32(cr3, PWR_CR3_VBE);

		if (fdt_getprop(fdt, node, "st,vbat-charge-1K5", NULL))
			io_setbits32(cr3, PWR_CR3_VBRS);
	}

	fdt_for_each_subnode(subnode, fdt, node) {
		res = dt_driver_maybe_add_probe_node(fdt, subnode);
		if (res) {
			EMSG("Failed on node %s with %#"PRIx32,
			     fdt_get_name(fdt, subnode, NULL), res);
			panic();
		}
	}

	return TEE_SUCCESS;
}

static const struct dt_device_match pwr_regu_match_table[] = {
	{ .compatible = "st,stm32mp1,pwr-reg"},
	{ }
};

DEFINE_DT_DRIVER(stm32mp1_pwr_regu_dt_driver) = {
	.name = "stm32mp1-pwr-regu",
	.match_table = pwr_regu_match_table,
	.probe = stm32mp1_pwr_regu_probe,
};
#endif /* CFG_EMBED_DTB */

void stm32mp_pwr_monitoring_enable(void)
{
	vaddr_t pwr_base = stm32_pwr_base();

	io_setbits32(pwr_base + PWR_CR2_OFF, PWR_CR2_MONEN);
}
