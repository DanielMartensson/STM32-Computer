// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2017, Linaro Limited
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

#include <arm.h>
#include <assert.h>
#include <config.h>
#include <drivers/gic.h>
#include <io.h>
#include <keep.h>
#include <kernel/dt.h>
#include <kernel/interrupt.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <libfdt.h>
#include <malloc.h>
#include <util.h>
#include <trace.h>
#include <string.h>

/* Offsets from gic.gicc_base */
#define GICC_CTLR		(0x000)
#define GICC_PMR		(0x004)
#define GICC_IAR		(0x00C)
#define GICC_EOIR		(0x010)

#define GICC_CTLR_ENABLEGRP0	(1 << 0)
#define GICC_CTLR_ENABLEGRP1	(1 << 1)
#define GICD_CTLR_ENABLEGRP1S	(1 << 2)
#define GICC_CTLR_FIQEN		(1 << 3)

/* Offsets from gic.gicd_base */
#define GICD_CTLR		(0x000)
#define GICD_TYPER		(0x004)
#define GICD_IGROUPR(n)		(0x080 + (n) * 4)
#define GICD_ISENABLER(n)	(0x100 + (n) * 4)
#define GICD_ICENABLER(n)	(0x180 + (n) * 4)
#define GICD_ISPENDR(n)		(0x200 + (n) * 4)
#define GICD_ICPENDR(n)		(0x280 + (n) * 4)
#define GICD_ISACTIVER(n)	(0x300 + (n) * 4)
#define GICD_IPRIORITYR(n)	(0x400 + (n) * 4)
#define GICD_ITARGETSR(n)	(0x800 + (n) * 4)
#define GICD_IGROUPMODR(n)	(0xd00 + (n) * 4)
#define GICD_ICFGR(n)		(0xc00 + (n) * 4)
#define GICD_SGIR		(0xF00)

#define GICD_CTLR_ENABLEGRP0	(1 << 0)
#define GICD_CTLR_ENABLEGRP1	(1 << 1)

/* Number of Private Peripheral Interrupt */
#define NUM_PPI	32

/* Number of Software Generated Interrupt */
#define NUM_SGI			16

/* Number of Non-secure Software Generated Interrupt */
#define NUM_NS_SGI		8

/* Number of interrupts in one register */
#define NUM_INTS_PER_REG	32

/* Number of targets in one register */
#define NUM_TARGETS_PER_REG	4

/* Accessors to access ITARGETSRn */
#define ITARGETSR_FIELD_BITS	8
#define ITARGETSR_FIELD_MASK	0xff

/* Maximum number of interrups a GIC can support */
#define GIC_MAX_INTS		1020

#define GICC_IAR_IT_ID_MASK	0x3ff
#define GICC_IAR_CPU_ID_MASK	0x7
#define GICC_IAR_CPU_ID_SHIFT	10

static void gic_op_add(struct itr_chip *chip, size_t it, uint32_t type,
		       uint32_t prio);
static void gic_op_enable(struct itr_chip *chip, size_t it);
static void gic_op_disable(struct itr_chip *chip, size_t it);
static void gic_op_raise_pi(struct itr_chip *chip, size_t it);
static void gic_op_raise_sgi(struct itr_chip *chip, size_t it,
			uint8_t cpu_mask);
static void gic_op_set_affinity(struct itr_chip *chip, size_t it,
			uint8_t cpu_mask);

#if defined(CFG_ARM_GIC_PM)
static void gic_pm_add_it(struct gic_data *gd, unsigned int it);
static void gic_pm_register(struct gic_data *gd);
#else
static void gic_pm_add_it(struct gic_data *gd __unused,
			  unsigned int it __unused)
{
}
static void gic_pm_register(struct gic_data *gd __unused)
{
}
#endif

#if !defined(CFG_ARM_GICV3)
static uint8_t gic_op_set_pmr(struct itr_chip *chip, uint8_t mask);
static uint8_t gic_op_set_ipriority(struct itr_chip *chip, size_t it,
			uint8_t mask);
#endif

static const struct itr_ops gic_ops = {
	.add = gic_op_add,
	.enable = gic_op_enable,
	.disable = gic_op_disable,
	.raise_pi = gic_op_raise_pi,
	.raise_sgi = gic_op_raise_sgi,
	.set_affinity = gic_op_set_affinity,
#if !defined(CFG_ARM_GICV3)
	.set_pmr = gic_op_set_pmr,
	.set_ipriority = gic_op_set_ipriority,
#endif
};
DECLARE_KEEP_PAGER(gic_ops);

static size_t probe_max_it(vaddr_t gicc_base __maybe_unused, vaddr_t gicd_base)
{
	int i;
	uint32_t old_ctlr;
	size_t ret = 0;
	const size_t max_regs = ((GIC_MAX_INTS + NUM_INTS_PER_REG - 1) /
					NUM_INTS_PER_REG) - 1;

	/*
	 * Probe which interrupt number is the largest.
	 */
#if defined(CFG_ARM_GICV3)
	old_ctlr = read_icc_ctlr();
	write_icc_ctlr(0);
#else
	old_ctlr = io_read32(gicc_base + GICC_CTLR);
	io_write32(gicc_base + GICC_CTLR, 0);
#endif
	for (i = max_regs; i >= 0; i--) {
		uint32_t old_reg;
		uint32_t reg;
		int b;

		old_reg = io_read32(gicd_base + GICD_ISENABLER(i));
		io_write32(gicd_base + GICD_ISENABLER(i), 0xffffffff);
		reg = io_read32(gicd_base + GICD_ISENABLER(i));
		io_write32(gicd_base + GICD_ICENABLER(i), ~old_reg);
		for (b = NUM_INTS_PER_REG - 1; b >= 0; b--) {
			if (BIT32(b) & reg) {
				ret = i * NUM_INTS_PER_REG + b;
				goto out;
			}
		}
	}
out:
#if defined(CFG_ARM_GICV3)
	write_icc_ctlr(old_ctlr);
#else
	io_write32(gicc_base + GICC_CTLR, old_ctlr);
#endif
	return ret;
}

void gic_cpu_init(struct gic_data *gd)
{
#if defined(CFG_ARM_GICV3)
	assert(gd->gicd_base);
#else
	assert(gd->gicd_base && gd->gicc_base);
#endif

	/* per-CPU interrupts config:
	 * ID0-ID7(SGI)   for Non-secure interrupts
	 * ID8-ID15(SGI)  for Secure interrupts.
	 * All PPI config as Non-secure interrupts.
	 */
	io_write32(gd->gicd_base + GICD_IGROUPR(0), 0xffff00ff);

	/* Set the priority mask to permit Non-secure interrupts, and to
	 * allow the Non-secure world to adjust the priority mask itself
	 */
#if defined(CFG_ARM_GICV3)
	write_icc_pmr(GIC_HIGHEST_NS_PRIORITY);
	write_icc_igrpen1(1);
#else
	io_write32(gd->gicc_base + GICC_PMR, GIC_HIGHEST_NS_PRIORITY);

	/* Enable GIC */
	io_write32(gd->gicc_base + GICC_CTLR,
		   GICC_CTLR_ENABLEGRP0 | GICC_CTLR_ENABLEGRP1 |
		   GICC_CTLR_FIQEN);
#endif
}

static void gic_setup_clear_it(struct gic_data *gd)
{
	size_t n = 0;

	for (n = 0; n <= gd->max_it / NUM_INTS_PER_REG; n++) {
		/* Disable interrupts */
		io_write32(gd->gicd_base + GICD_ICENABLER(n), 0xffffffff);

		/* Make interrupts non-pending */
		io_write32(gd->gicd_base + GICD_ICPENDR(n), 0xffffffff);
	};
}

void gic_init_setup(struct gic_data *gd)
{
	size_t n = 0;

	for (n = 0; n <= gd->max_it / NUM_INTS_PER_REG; n++) {
		/* Mark interrupts non-secure */
		if (n == 0) {
			/* per-CPU inerrupts config:
			 * ID0-ID7(SGI)	  for Non-secure interrupts
			 * ID8-ID15(SGI)  for Secure interrupts.
			 * All PPI config as Non-secure interrupts.
			 */
			io_write32(gd->gicd_base + GICD_IGROUPR(n), 0xffff00ff);
		} else {
			io_write32(gd->gicd_base + GICD_IGROUPR(n), 0xffffffff);
		}
	}

	/* Set the priority mask to permit Non-secure interrupts, and to
	 * allow the Non-secure world to adjust the priority mask itself
	 */
#if defined(CFG_ARM_GICV3)
	write_icc_pmr(GIC_HIGHEST_NS_PRIORITY);
	write_icc_igrpen1(1);
	io_setbits32(gd->gicd_base + GICD_CTLR, GICD_CTLR_ENABLEGRP1S);
#else
	io_write32(gd->gicc_base + GICC_PMR, GIC_HIGHEST_NS_PRIORITY);

	/* Enable GIC */
	io_write32(gd->gicc_base + GICC_CTLR, GICC_CTLR_FIQEN |
		   GICC_CTLR_ENABLEGRP0 | GICC_CTLR_ENABLEGRP1);
	io_setbits32(gd->gicd_base + GICD_CTLR,
		     GICD_CTLR_ENABLEGRP0 | GICD_CTLR_ENABLEGRP1);
#endif
}

static int gic_dt_get_irq(const uint32_t *properties, int count, uint32_t *type,
			  uint32_t *prio)
{
	int it_num = DT_INFO_INVALID_INTERRUPT;

	if (type)
		*type = IRQ_TYPE_NONE;

	if (prio)
		*prio = 0;

	if (!properties || count < 2)
		return DT_INFO_INVALID_INTERRUPT;

	it_num = fdt32_to_cpu(properties[1]);

	switch (fdt32_to_cpu(properties[0])) {
	case 1:
		it_num += 16;
		break;
	case 0:
		it_num += 32;
		break;
	default:
		it_num = DT_INFO_INVALID_INTERRUPT;
	}

	return it_num;
}

void gic_init_base_addr(struct gic_data *gd,
			paddr_t gicc_base_pa __maybe_unused,
			paddr_t gicd_base_pa)
{
	vaddr_t gicc_base = 0;
	vaddr_t gicd_base = 0;

	assert(cpu_mmu_enabled());

	gicd_base = core_mmu_get_va(gicd_base_pa, MEM_AREA_IO_SEC,
				    GIC_DIST_REG_SIZE);
	if (!gicd_base)
		panic();

	if (!IS_ENABLED(CFG_ARM_GICV3)) {
		gicc_base = core_mmu_get_va(gicc_base_pa, MEM_AREA_IO_SEC,
					    GIC_CPU_REG_SIZE);
		if (!gicc_base)
			panic();
	}

	gd->gicc_base = gicc_base;
	gd->gicd_base = gicd_base;
	gd->max_it = probe_max_it(gicc_base, gicd_base);
	gd->chip.ops = &gic_ops;

	if (IS_ENABLED(CFG_DT))
		gd->chip.dt_get_irq = gic_dt_get_irq;

	gic_pm_register(gd);
}

void gic_init(struct gic_data *gd, paddr_t gicc_base_pa, paddr_t gicd_base_pa)
{
	gic_init_base_addr(gd, gicc_base_pa, gicd_base_pa);
	gic_setup_clear_it(gd);
	gic_init_setup(gd);
}

static void gic_it_add(struct gic_data *gd, size_t it)
{
	size_t idx = it / NUM_INTS_PER_REG;
	uint32_t mask = 1 << (it % NUM_INTS_PER_REG);

	/* Disable the interrupt */
	io_write32(gd->gicd_base + GICD_ICENABLER(idx), mask);
	/* Make it non-pending */
	io_write32(gd->gicd_base + GICD_ICPENDR(idx), mask);
	/* Assign it to group0 */
	io_clrbits32(gd->gicd_base + GICD_IGROUPR(idx), mask);
#if defined(CFG_ARM_GICV3)
	/* Assign it to group1S */
	io_setbits32(gd->gicd_base + GICD_IGROUPMODR(idx), mask);
#endif
}

static void gic_it_set_cpu_mask(struct gic_data *gd, size_t it,
				uint8_t cpu_mask)
{
	size_t idx __maybe_unused = it / NUM_INTS_PER_REG;
	uint32_t mask __maybe_unused = 1 << (it % NUM_INTS_PER_REG);
	uint32_t target = 0;
	uint32_t target_shift = 0;
	vaddr_t itargetsr = gd->gicd_base +
			    GICD_ITARGETSR(it / NUM_TARGETS_PER_REG);

	/* Assigned to group0 */
	assert(!(io_read32(gd->gicd_base + GICD_IGROUPR(idx)) & mask));

	/* Route it to selected CPUs */
	target = io_read32(itargetsr);
	target_shift = (it % NUM_TARGETS_PER_REG) * ITARGETSR_FIELD_BITS;
	target &= ~(ITARGETSR_FIELD_MASK << target_shift);
	target |= cpu_mask << target_shift;
	DMSG("cpu_mask: writing 0x%x to 0x%" PRIxVA, target, itargetsr);
	io_write32(itargetsr, target);
	DMSG("cpu_mask: 0x%x", io_read32(itargetsr));
}

static void gic_it_set_prio(struct gic_data *gd, size_t it, uint8_t prio)
{
	size_t idx __maybe_unused = it / NUM_INTS_PER_REG;
	uint32_t mask __maybe_unused = 1 << (it % NUM_INTS_PER_REG);

	/* Set prio it to selected CPUs */
	DMSG("prio: writing 0x%x to 0x%" PRIxVA,
		prio, gd->gicd_base + GICD_IPRIORITYR(0) + it);
	io_write8(gd->gicd_base + GICD_IPRIORITYR(0) + it, prio);
}

static uint8_t gic_op_set_pmr(struct itr_chip *chip, uint8_t mask)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);
	uint32_t pmr = io_read32(gd->gicc_base + GICC_PMR);

	/*
	 * Order memory updates w.r.t. PMR write, and ensure they're visible
	 * before potential out of band interrupt trigger because of PMR update.
	 */
	dsb_ishst();
	io_write32(gd->gicc_base + GICC_PMR, mask);
	dsb_ishst();

	return (uint8_t)pmr;
}

static uint8_t gic_op_set_ipriority(struct itr_chip *chip, size_t it,
			uint8_t mask)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);
	uint8_t prio = io_read8(gd->gicd_base + GICD_IPRIORITYR(0) + it);

	gic_it_set_prio(gd, it, mask);

	return prio;
}

static void gic_it_enable(struct gic_data *gd, size_t it)
{
	size_t idx = it / NUM_INTS_PER_REG;
	uint32_t mask = 1 << (it % NUM_INTS_PER_REG);
	vaddr_t base = gd->gicd_base;

	/* Assigned to group0 */
	assert(!(io_read32(base + GICD_IGROUPR(idx)) & mask));

	/* Enable the interrupt */
	io_write32(base + GICD_ISENABLER(idx), mask);
}

static void gic_it_disable(struct gic_data *gd, size_t it)
{
	size_t idx = it / NUM_INTS_PER_REG;
	uint32_t mask = 1 << (it % NUM_INTS_PER_REG);

	/* Assigned to group0 */
	assert(!(io_read32(gd->gicd_base + GICD_IGROUPR(idx)) & mask));

	/* Disable the interrupt */
	io_write32(gd->gicd_base + GICD_ICENABLER(idx), mask);
}

static void gic_it_set_pending(struct gic_data *gd, size_t it)
{
	size_t idx = it / NUM_INTS_PER_REG;
	uint32_t mask = BIT32(it % NUM_INTS_PER_REG);

	/* Should be Peripheral Interrupt */
	assert(it >= NUM_SGI);

	/* Raise the interrupt */
	io_write32(gd->gicd_base + GICD_ISPENDR(idx), mask);
}

static void gic_it_raise_sgi(struct gic_data *gd, size_t it,
		uint8_t cpu_mask, uint8_t group)
{
	uint32_t mask_id = it & 0xf;
	uint32_t mask_group = group & 0x1;
	uint32_t mask_cpu = cpu_mask & 0xff;
	uint32_t mask = (mask_id | SHIFT_U32(mask_group, 15) |
		SHIFT_U32(mask_cpu, 16));

	/* Should be Software Generated Interrupt */
	assert(it < NUM_SGI);

	/* Raise the interrupt */
	io_write32(gd->gicd_base + GICD_SGIR, mask);
}

static uint32_t gic_read_iar(struct gic_data *gd __maybe_unused)
{
#if defined(CFG_ARM_GICV3)
	return read_icc_iar1();
#else
	return io_read32(gd->gicc_base + GICC_IAR);
#endif
}

static void gic_write_eoir(struct gic_data *gd __maybe_unused, uint32_t eoir)
{
#if defined(CFG_ARM_GICV3)
	write_icc_eoir1(eoir);
#else
	io_write32(gd->gicc_base + GICC_EOIR, eoir);
#endif
}

static bool gic_it_is_enabled(struct gic_data *gd, size_t it)
{
	size_t idx = it / NUM_INTS_PER_REG;
	uint32_t mask = 1 << (it % NUM_INTS_PER_REG);
	return !!(io_read32(gd->gicd_base + GICD_ISENABLER(idx)) & mask);
}

static bool __maybe_unused gic_it_get_group(struct gic_data *gd, size_t it)
{
	size_t idx = it / NUM_INTS_PER_REG;
	uint32_t mask = 1 << (it % NUM_INTS_PER_REG);
	return !!(io_read32(gd->gicd_base + GICD_IGROUPR(idx)) & mask);
}

static uint32_t __maybe_unused gic_it_get_target(struct gic_data *gd, size_t it)
{
	size_t reg_idx = it / NUM_TARGETS_PER_REG;
	uint32_t target_shift = (it % NUM_TARGETS_PER_REG) *
				ITARGETSR_FIELD_BITS;
	uint32_t target_mask = ITARGETSR_FIELD_MASK << target_shift;
	uint32_t target = io_read32(gd->gicd_base + GICD_ITARGETSR(reg_idx));

	return (target & target_mask) >> target_shift;
}

void gic_dump_state(struct gic_data *gd)
{
	int i;

#if defined(CFG_ARM_GICV3)
	DMSG("GICC_CTLR: 0x%x", read_icc_ctlr());
#else
	DMSG("GICC_CTLR: 0x%x", io_read32(gd->gicc_base + GICC_CTLR));
#endif
	DMSG("GICD_CTLR: 0x%x", io_read32(gd->gicd_base + GICD_CTLR));

	for (i = 0; i <= (int)gd->max_it; i++) {
		if (gic_it_is_enabled(gd, i)) {
			DMSG("irq%d: enabled, group:%d, target:%x", i,
			     gic_it_get_group(gd, i), gic_it_get_target(gd, i));
		}
	}
}

void gic_it_handle(struct gic_data *gd)
{
	uint32_t iar = 0;
	uint32_t id = 0;

	iar = gic_read_iar(gd);
	id = iar & GICC_IAR_IT_ID_MASK;

	if (id <= gd->max_it)
		itr_handle(id);
	else
		DMSG("ignoring interrupt %" PRIu32, id);

	gic_write_eoir(gd, iar);
}

static void gic_op_add(struct itr_chip *chip, size_t it,
		       uint32_t type __unused,
		       uint32_t prio __unused)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);

	if (it > gd->max_it)
		panic();

	gic_it_add(gd, it);
	/* Set the CPU mask to deliver interrupts to any online core */
	gic_it_set_cpu_mask(gd, it, 0xff);
	gic_it_set_prio(gd, it, 0x1);

	gic_pm_add_it(gd, it);
}

static void gic_op_enable(struct itr_chip *chip, size_t it)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);

	if (it > gd->max_it)
		panic();

	gic_it_enable(gd, it);
}

static void gic_op_disable(struct itr_chip *chip, size_t it)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);

	if (it > gd->max_it)
		panic();

	gic_it_disable(gd, it);
}

static void gic_op_raise_pi(struct itr_chip *chip, size_t it)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);

	if (it > gd->max_it)
		panic();

	gic_it_set_pending(gd, it);
}

static void gic_op_raise_sgi(struct itr_chip *chip, size_t it,
			uint8_t cpu_mask)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);

	if (it > gd->max_it)
		panic();

	if (it < NUM_NS_SGI)
		gic_it_raise_sgi(gd, it, cpu_mask, 1);
	else
		gic_it_raise_sgi(gd, it, cpu_mask, 0);
}

static void gic_op_set_affinity(struct itr_chip *chip, size_t it,
			uint8_t cpu_mask)
{
	struct gic_data *gd = container_of(chip, struct gic_data, chip);

	if (it > gd->max_it)
		panic();

	gic_it_set_cpu_mask(gd, it, cpu_mask);
}

#if defined(CFG_ARM_GIC_PM)
/*
 * Save/restore interrupts registered from the gic_op_add_it() handler
 */
#define IT_PM_GPOUP1_BIT	BIT(0)
#define IT_PM_ENABLE_BIT	BIT(1)
#define IT_PM_PENDING_BIT	BIT(2)
#define IT_PM_ACTIVE_BIT	BIT(3)
#define IT_PM_CONFIG_MASK	GENMASK_32(1, 0)

/*
 * @it - interrupt ID/number
 * @flags - bitflag IT_PM_*_BIT
 * @iprio - 8bit prio from IPRIORITYR
 * @itarget - 8bit target from ITARGETR
 * @icfg - 2bit configuration from ICFGR and IT_PM_CONFIG_MASK
 */
struct gic_it_pm {
	uint16_t it;
	uint8_t flags;
	uint8_t iprio;
	uint8_t itarget;
	uint8_t icfg;
};

static void gic_pm_add_it(struct gic_data *gd, unsigned int it)
{
	struct gic_pm *pm = &gd->pm;

	pm->count++;
	pm->pm_cfg = realloc(pm->pm_cfg, pm->count * sizeof(*pm->pm_cfg));
	if (!pm->pm_cfg)
		panic();

	pm->pm_cfg[pm->count - 1] = (struct gic_it_pm){ .it = it };
}

static void gic_save_it(struct gic_data *gd, struct gic_it_pm *pm)
{
	unsigned int it = pm->it;
	size_t idx = 0;
	uint32_t bit_mask = BIT(it % NUM_INTS_PER_REG);
	uint32_t shift2 = it % (NUM_INTS_PER_REG / 2) * 2;
	uint32_t shift8 = it % (NUM_INTS_PER_REG / 8) * 8;
	uint32_t data32 = 0;

	pm->flags = 0;
	idx = it / NUM_INTS_PER_REG;

	if (io_read32(gd->gicd_base + GICD_IGROUPR(idx)) & bit_mask)
		pm->flags |= IT_PM_GPOUP1_BIT;
	if (io_read32(gd->gicd_base + GICD_ISENABLER(idx)) & bit_mask)
		pm->flags |= IT_PM_ENABLE_BIT;
	if (io_read32(gd->gicd_base + GICD_ISPENDR(idx)) & bit_mask)
		pm->flags |= IT_PM_PENDING_BIT;
	if (io_read32(gd->gicd_base + GICD_ISACTIVER(idx)) & bit_mask)
		pm->flags |= IT_PM_ACTIVE_BIT;

	idx = (8 * it) / NUM_INTS_PER_REG;

	data32 = io_read32(gd->gicd_base + GICD_IPRIORITYR(idx)) >> shift8;
	pm->iprio = (uint8_t)data32;

	data32 = io_read32(gd->gicd_base + GICD_ITARGETSR(idx)) >> shift8;
	pm->itarget = (uint8_t)data32;

	/* Note: ICFGR is RAO for SPIs and PPIs */
	idx = (2 * it) / NUM_INTS_PER_REG;
	data32 = io_read32(gd->gicd_base + GICD_ICFGR(idx)) >> shift2;
	pm->icfg = (uint8_t)data32 & IT_PM_CONFIG_MASK;
}

static void gic_restore_it(struct gic_data *gd, struct gic_it_pm *pm)
{
	unsigned int it = pm->it;
	size_t idx = it / NUM_INTS_PER_REG;
	uint32_t mask = BIT(it % NUM_INTS_PER_REG);
	uint32_t shift2 = it % (NUM_INTS_PER_REG / 2) * 2;
	uint32_t shift8 = it % (NUM_INTS_PER_REG / 8) * 8;

	io_mask32(gd->gicd_base + GICD_IGROUPR(idx),
		  (pm->flags & IT_PM_GPOUP1_BIT) ? mask : 0, mask);

	io_mask32(gd->gicd_base + GICD_ISENABLER(idx),
		  (pm->flags & IT_PM_ENABLE_BIT) ? mask : 0, mask);

	io_mask32(gd->gicd_base + GICD_ISPENDR(idx),
		  (pm->flags & IT_PM_PENDING_BIT) ? mask : 0, mask);

	io_mask32(gd->gicd_base + GICD_ISACTIVER(idx),
		  (pm->flags & IT_PM_ACTIVE_BIT) ? mask : 0, mask);

	idx = (8 * it) / NUM_INTS_PER_REG;

	io_mask32(gd->gicd_base + GICD_IPRIORITYR(idx),
		  (uint32_t)pm->iprio << shift8, UINT8_MAX << shift8);

	io_mask32(gd->gicd_base + GICD_ITARGETSR(idx),
		  (uint32_t)pm->itarget << shift8, UINT8_MAX << shift8);

	/* Note: ICFGR is WI for SPIs and PPIs */
	idx = (2 * it) / NUM_INTS_PER_REG;
	io_mask32(gd->gicd_base + GICD_ICFGR(idx),
		  (uint32_t)pm->icfg << shift2, IT_PM_CONFIG_MASK << shift2);
}

static TEE_Result gic_pm(enum pm_op op, unsigned int pm_hint __unused,
			 const struct pm_callback_handle *handle)
{
	void (*sequence)(struct gic_data *gd, struct gic_it_pm *pm) = NULL;
	struct gic_it_pm *cfg = NULL;
	unsigned int n = 0;
	struct gic_data *gd = (struct gic_data *)PM_CALLBACK_GET_HANDLE(handle);
	struct gic_pm *pm = &gd->pm;

	if (op == PM_OP_SUSPEND) {
		sequence = gic_save_it;
	} else {
		gic_init_setup(gd);
		sequence = gic_restore_it;
	}
	for (n = 0, cfg = pm->pm_cfg; n < pm->count; n++, cfg++)
		sequence(gd, cfg);

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(gic_pm);

static void gic_pm_register(struct gic_data *gd)
{
	register_pm_core_service_cb(gic_pm, gd, "arm-gic");
}
#endif /*CFG_ARM_GIC_PM*/
