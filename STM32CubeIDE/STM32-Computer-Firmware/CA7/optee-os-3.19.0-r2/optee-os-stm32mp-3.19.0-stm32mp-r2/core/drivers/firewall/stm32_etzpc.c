// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017-2021, STMicroelectronics
 */

/*
 * STM32 ETPZC acts as a firewall on stm32mp SoC peripheral interfaces and
 * internal memories. The driver expects a single instance of the controller
 * in the platform.
 *
 * The driver API is defined in header file stm32_etzpc.h.
 *
 * Driver registers a PM callback for restoration of the access permissions
 * when it resumes.
 */

#include <assert.h>
#include <drivers/clk_dt.h>
#include <drivers/stm32_etzpc.h>
#include <drivers/stm32_firewall.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <initcall.h>
#include <io.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/dt.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <stm32_util.h>
#include <util.h>

/* Devicetree compatibility */
#define ETZPC_LOCK_MASK			BIT(0)
#define ETZPC_MODE_SHIFT		8
#define ETZPC_MODE_MASK			GENMASK_32(1, 0)
#define ETZPC_ID_SHIFT			16
#define ETZPC_ID_MASK			GENMASK_32(7, 0)

/* ID Registers */
#define ETZPC_TZMA0_SIZE		0x000U
#define ETZPC_DECPROT0			0x010U
#define ETZPC_DECPROT_LOCK0		0x030U
#define ETZPC_HWCFGR			0x3F0U
#define ETZPC_VERR			0x3F4U

/* ID Registers fields */
#define ETZPC_TZMA0_SIZE_LOCK		BIT(31)
#define ETZPC_DECPROT0_MASK		GENMASK_32(1, 0)
#define ETZPC_HWCFGR_NUM_TZMA_MASK	GENMASK_32(7, 0)
#define ETZPC_HWCFGR_NUM_TZMA_SHIFT	0
#define ETZPC_HWCFGR_NUM_PER_SEC_MASK	GENMASK_32(15, 8)
#define ETZPC_HWCFGR_NUM_PER_SEC_SHIFT	8
#define ETZPC_HWCFGR_NUM_AHB_SEC_MASK	GENMASK_32(23, 16)
#define ETZPC_HWCFGR_NUM_AHB_SEC_SHIFT	16
#define ETZPC_HWCFGR_CHUNCKS1N4_MASK	GENMASK_32(31, 24)
#define ETZPC_HWCFGR_CHUNCKS1N4_SHIFT	24

#define DECPROT_SHIFT			1
#define IDS_PER_DECPROT_REGS		16U
#define IDS_PER_DECPROT_LOCK_REGS	32U

#define PAGE_SIZE			BIT(12) /* 4KB page size */

/*
 * Implementation uses uint8_t to store each securable DECPROT configuration
 * and uint16_t to store each securable TZMA configuration. When resuming
 * from deep suspend, the DECPROT configurations are restored.
 */
#define PERIPH_PM_LOCK_BIT		BIT(7)
#define PERIPH_PM_ATTR_MASK		GENMASK_32(2, 0)
#define TZMA_PM_LOCK_BIT		BIT(15)
#define TZMA_PM_VALUE_MASK		GENMASK_32(9, 0)

/*
 * @num_tzma - number of TZMA zones, read from the hardware
 * @num_per_sec - number of securable AHB & APB periphs, read from the hardware
 * @num_ahb_sec - number of securable AHB master zones, read from the hardware
 */
struct stm32_etzpc_driver_data {
	unsigned int num_tzma;
	unsigned int num_per_sec;
	unsigned int num_ahb_sec;
};

struct etzpc_device {
	struct stm32_etzpc_platdata pdata;
	struct stm32_etzpc_driver_data *ddata;
	struct stm32_firewall_device *fdev;
	unsigned int lock;
};

static uint32_t etzpc_lock(struct etzpc_device *dev)
{
	return may_spin_lock(&dev->lock);
}

static void etzpc_unlock(struct etzpc_device *dev, uint32_t exceptions)
{
	may_spin_unlock(&dev->lock, exceptions);
}

static bool __maybe_unused valid_decprot_id(struct etzpc_device *etzpc_dev,
					    unsigned int id)
{
	return id < etzpc_dev->ddata->num_per_sec;
}

static bool __maybe_unused valid_tzma_id(struct etzpc_device *etzpc_dev,
					 unsigned int id)
{
	return id < etzpc_dev->ddata->num_tzma;
}

static enum etzpc_decprot_attributes etzpc_binding2decprot(uint32_t mode)
{
	switch (mode) {
	case DECPROT_S_RW:
		return ETZPC_DECPROT_S_RW;
	case DECPROT_NS_R_S_W:
		return ETZPC_DECPROT_NS_R_S_W;
#ifdef CFG_STM32MP15
	case DECPROT_MCU_ISOLATION:
		return ETZPC_DECPROT_MCU_ISOLATION;
#endif
	case DECPROT_NS_RW:
		return ETZPC_DECPROT_NS_RW;
	default:
		panic();
	}
}

static void etzpc_configure_decprot(struct etzpc_device *etzpc_dev,
				    uint32_t decprot_id,
				    enum etzpc_decprot_attributes decprot_attr)
{
	size_t offset = 4U * (decprot_id / IDS_PER_DECPROT_REGS);
	uint32_t shift = (decprot_id % IDS_PER_DECPROT_REGS) << DECPROT_SHIFT;
	uint32_t masked_decprot = (uint32_t)decprot_attr & ETZPC_DECPROT0_MASK;
	vaddr_t base = etzpc_dev->pdata.base;
	unsigned int exceptions = 0;

	assert(valid_decprot_id(etzpc_dev, decprot_id));

	FMSG("ID : %"PRIu32", CONF %d", decprot_id, decprot_attr);

	exceptions = etzpc_lock(etzpc_dev);

	io_clrsetbits32(base + ETZPC_DECPROT0 + offset,
			ETZPC_DECPROT0_MASK << shift,
			masked_decprot << shift);

	etzpc_unlock(etzpc_dev, exceptions);
}

static enum etzpc_decprot_attributes
etzpc_get_decprot(struct etzpc_device *etzpc_dev, uint32_t decprot_id)
{
	size_t offset = 4U * (decprot_id / IDS_PER_DECPROT_REGS);
	uint32_t shift = (decprot_id % IDS_PER_DECPROT_REGS) << DECPROT_SHIFT;
	vaddr_t base = etzpc_dev->pdata.base;
	uint32_t value = 0;

	assert(valid_decprot_id(etzpc_dev, decprot_id));

	value = (io_read32(base + ETZPC_DECPROT0 + offset) >> shift) &
		ETZPC_DECPROT0_MASK;

	return (enum etzpc_decprot_attributes)value;
}

static void etzpc_lock_decprot(struct etzpc_device *etzpc_dev,
			       uint32_t decprot_id)
{
	size_t offset = 4U * (decprot_id / IDS_PER_DECPROT_LOCK_REGS);
	uint32_t mask = BIT(decprot_id % IDS_PER_DECPROT_LOCK_REGS);
	vaddr_t base = etzpc_dev->pdata.base;
	uint32_t exceptions = 0;

	assert(valid_decprot_id(etzpc_dev, decprot_id));

	exceptions = etzpc_lock(etzpc_dev);

	io_write32(base + offset + ETZPC_DECPROT_LOCK0, mask);

	etzpc_unlock(etzpc_dev, exceptions);
}

static bool is_decprot_locked(struct etzpc_device *etzpc_dev,
			      uint32_t decprot_id)
{
	size_t offset = 4U * (decprot_id / IDS_PER_DECPROT_LOCK_REGS);
	uint32_t mask = BIT(decprot_id % IDS_PER_DECPROT_LOCK_REGS);
	vaddr_t base = etzpc_dev->pdata.base;

	assert(valid_decprot_id(etzpc_dev, decprot_id));

	return io_read32(base + offset + ETZPC_DECPROT_LOCK0) & mask;
}

static void etzpc_configure_tzma(struct etzpc_device *etzpc_dev,
				 uint32_t tzma_id, uint16_t tzma_value)
{
	size_t offset = sizeof(uint32_t) * tzma_id;
	vaddr_t base = etzpc_dev->pdata.base;
	uint32_t exceptions = 0;

	assert(valid_tzma_id(etzpc_dev, tzma_id));

	exceptions = etzpc_lock(etzpc_dev);

	io_write32(base + ETZPC_TZMA0_SIZE + offset, tzma_value);

	etzpc_unlock(etzpc_dev, exceptions);
}

static uint16_t etzpc_get_tzma(struct etzpc_device *etzpc_dev,
			       uint32_t tzma_id)
{
	size_t offset = sizeof(uint32_t) * tzma_id;
	vaddr_t base = etzpc_dev->pdata.base;

	assert(valid_tzma_id(etzpc_dev, tzma_id));

	return io_read32(base + ETZPC_TZMA0_SIZE + offset);
}

static void etzpc_lock_tzma(struct etzpc_device *etzpc_dev, uint32_t tzma_id)
{
	size_t offset = sizeof(uint32_t) * tzma_id;
	vaddr_t base = etzpc_dev->pdata.base;
	uint32_t exceptions = 0;

	assert(valid_tzma_id(etzpc_dev, tzma_id));

	exceptions = etzpc_lock(etzpc_dev);

	io_setbits32(base + ETZPC_TZMA0_SIZE + offset, ETZPC_TZMA0_SIZE_LOCK);

	etzpc_unlock(etzpc_dev, exceptions);
}

static bool is_tzma_locked(struct etzpc_device *etzpc_dev, uint32_t tzma_id)
{
	size_t offset = sizeof(uint32_t) * tzma_id;
	vaddr_t base = etzpc_dev->pdata.base;

	assert(valid_tzma_id(etzpc_dev, tzma_id));

	return io_read32(base + ETZPC_TZMA0_SIZE + offset) &
	       ETZPC_TZMA0_SIZE_LOCK;
}

static TEE_Result etzpc_pm(enum pm_op op, unsigned int pm_hint __unused,
			   const struct pm_callback_handle *pm_handle)
{
	struct stm32_firewall_device *fdev =
		(struct stm32_firewall_device *)pm_handle->handle;
	struct etzpc_device *etzpc_dev = stm32_firewall_priv(fdev);
	struct stm32_etzpc_driver_data *ddata = etzpc_dev->ddata;
	struct stm32_etzpc_platdata *pdata = &etzpc_dev->pdata;
	unsigned int n = 0;

	if (op == PM_OP_SUSPEND) {
		for (n = 0; n < ddata->num_per_sec; n++) {
			pdata->periph_cfg[n] =
				(uint8_t)etzpc_get_decprot(etzpc_dev, n);
			if (is_decprot_locked(etzpc_dev, n))
				pdata->periph_cfg[n] |= PERIPH_PM_LOCK_BIT;
		}

		for (n = 0; n < ddata->num_tzma; n++) {
			pdata->tzma_cfg[n] =
				(uint8_t)etzpc_get_tzma(etzpc_dev, n);
			if (is_tzma_locked(etzpc_dev, n))
				pdata->tzma_cfg[n] |= TZMA_PM_LOCK_BIT;
		}

		return TEE_SUCCESS;
	}

	/* PM_OP_RESUME */
	for (n = 0; n < ddata->num_per_sec; n++) {
		unsigned int attr = pdata->periph_cfg[n] & PERIPH_PM_ATTR_MASK;

		etzpc_configure_decprot(etzpc_dev, n,
					(enum etzpc_decprot_attributes)attr);

		if (pdata->periph_cfg[n] & PERIPH_PM_LOCK_BIT)
			etzpc_lock_decprot(etzpc_dev, n);
	}

	for (n = 0; n < ddata->num_tzma; n++) {
		uint16_t value = pdata->tzma_cfg[n] & TZMA_PM_VALUE_MASK;

		etzpc_configure_tzma(etzpc_dev, n, value);

		if (pdata->tzma_cfg[n] & TZMA_PM_LOCK_BIT)
			etzpc_lock_tzma(etzpc_dev, n);
	}

	return TEE_SUCCESS;
}

DECLARE_KEEP_PAGER(etzpc_pm);

static TEE_Result etzpc_mode_from_cfg(const struct stm32_firewall_cfg *cfg,
				      enum etzpc_decprot_attributes *attr,
				      bool *lock)
{
	const struct stm32_firewall_cfg *config = cfg;

	assert(cfg && attr);

	*attr = ETZPC_DECPROT_MAX;

	while (config->access) {
		uint32_t mode = 0;

		mode = config->access & FWLL_CONF_MASK;
		if (mode == FWLL_NO_ACCESS) {
			config++;
			continue;
		}

		if (lock && (config->access & FWLL_CONF_LOCK))
			*lock |= true;

		switch (config->access & FWLL_MASTER_MASK) {
		case FWLL_MASTER(0):
			if (*attr != ETZPC_DECPROT_MAX) {
				DMSG("Inconsistent configuration data");
				return TEE_ERROR_BAD_PARAMETERS;
			}

			if (mode & FWLL_SEC_RW)
				*attr = ETZPC_DECPROT_S_RW;

			if (mode & FWLL_NSEC_READ)
				*attr = ETZPC_DECPROT_NS_R_S_W;

			if (mode & FWLL_NSEC_WRITE)
				*attr = ETZPC_DECPROT_NS_RW;

			break;
#ifdef CFG_STM32MP15
		case FWLL_MASTER(1):
			if (*attr != ETZPC_DECPROT_MAX) {
				DMSG("Inconsistent configuration data");
				return TEE_ERROR_BAD_PARAMETERS;
			}

			/* No other configuration set */
			if (mode & FWLL_FULL_ACCESS &&
			    *attr == ETZPC_DECPROT_MAX)
				*attr = ETZPC_DECPROT_MCU_ISOLATION;

			break;
#endif
		default:
			return TEE_ERROR_BAD_PARAMETERS;
		}

		config++;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_etzpc_has_access(struct stm32_firewall_device *fdev,
					 unsigned int id,
					 paddr_t base, size_t sz,
					 const struct stm32_firewall_cfg *cfg)
{
	struct etzpc_device *etzpc_dev = stm32_firewall_priv(fdev);
	enum etzpc_decprot_attributes attr_req = ETZPC_DECPROT_MAX;
	enum etzpc_decprot_attributes attr = ETZPC_DECPROT_MAX;
	uint16_t size = 0;
	uint32_t tzma_id = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	assert(cfg);

	res = etzpc_mode_from_cfg(cfg, &attr_req, NULL);
	if (res || attr_req == ETZPC_DECPROT_MAX)
		return TEE_ERROR_SECURITY;

	if (id < etzpc_dev->ddata->num_per_sec) {
		attr = etzpc_get_decprot(etzpc_dev, id);
		FMSG("Check Access %u - attr %d - requested %d", id,
		     attr, attr_req);
		if (attr == attr_req)
			return TEE_SUCCESS;
		else
			return TEE_ERROR_ACCESS_DENIED;
	}

	/* TZMA */
	tzma_id = (id - etzpc_dev->ddata->num_per_sec);
	size = etzpc_get_tzma(etzpc_dev, tzma_id) * PAGE_SIZE;
	if (base < fdev->compat->reg[id].addr + size &&
	    base + sz - 1 < fdev->compat->reg[id].addr + size &&
	    attr_req == ETZPC_DECPROT_S_RW)
		return TEE_SUCCESS;

	if (base >= fdev->compat->reg[id].addr + size &&
	    base + sz - 1 < fdev->compat->reg[id].addr +
	    fdev->compat->reg[id].size &&
	    attr_req == ETZPC_DECPROT_NS_RW)
		return TEE_SUCCESS;

	return TEE_ERROR_ACCESS_DENIED;
}

static TEE_Result stm32_etzpc_configure(struct stm32_firewall_device *fdev,
					unsigned int id,
					paddr_t base, size_t sz,
					const struct stm32_firewall_cfg *cfg)
{
	struct etzpc_device *etzpc_dev = stm32_firewall_priv(fdev);
	enum etzpc_decprot_attributes attr = ETZPC_DECPROT_MAX;
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int total_sz = 0;
	uint32_t tzma_id = 0;
	bool lock = false;

	assert(cfg);

	res = etzpc_mode_from_cfg(cfg, &attr, &lock);
	if (res || attr == ETZPC_DECPROT_MAX)
		return TEE_ERROR_BAD_PARAMETERS;

	if (id < etzpc_dev->ddata->num_per_sec) {
		if (is_decprot_locked(etzpc_dev, id))
			return TEE_ERROR_ACCESS_DENIED;

		etzpc_configure_decprot(etzpc_dev, id, attr);
		if (lock)
			etzpc_lock_decprot(etzpc_dev, id);

		return TEE_SUCCESS;
	}

	/* TZMA */
	if (base + sz > fdev->compat->reg[id].addr + fdev->compat->reg[id].size)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (attr) {
	case ETZPC_DECPROT_S_RW:
		total_sz = base + sz - fdev->compat->reg[id].addr;
		break;
	case ETZPC_DECPROT_NS_RW:
		total_sz = base - fdev->compat->reg[id].addr;
		break;
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	total_sz = ROUNDUP_DIV(total_sz, PAGE_SIZE);
	tzma_id = id - etzpc_dev->ddata->num_per_sec;
	etzpc_configure_tzma(etzpc_dev, tzma_id, total_sz);
	if (lock)
		etzpc_lock_tzma(etzpc_dev, tzma_id);

	return TEE_SUCCESS;
}

static const struct stm32_firewall_ops stm32_etzpc_fw_ops = {
	.has_access = stm32_etzpc_has_access,
	.configure_access = stm32_etzpc_configure,
};

static struct etzpc_device *stm32_etzpc_alloc(void)
{
	struct etzpc_device *etzpc_dev = calloc(1, sizeof(*etzpc_dev));
	struct stm32_etzpc_driver_data *ddata = calloc(1, sizeof(*ddata));

	if (etzpc_dev && ddata) {
		etzpc_dev->ddata = ddata;
		return etzpc_dev;
	}

	free(ddata);
	free(etzpc_dev);

	return NULL;
}

/* Informative unused function */
static __unused void stm32_etzpc_free(struct etzpc_device *etzpc_dev)
{
	if (etzpc_dev) {
		if (etzpc_dev->fdev)
			stm32_firewall_dev_free(etzpc_dev->fdev);

		free(etzpc_dev->ddata);
		free(etzpc_dev);
	}
}

static void stm32_etzpc_set_driverdata(struct etzpc_device *dev)
{
	struct stm32_etzpc_driver_data *ddata = dev->ddata;
	uintptr_t base = dev->pdata.base;
	uint32_t reg = io_read32(base + ETZPC_HWCFGR);

	ddata->num_tzma = (reg & ETZPC_HWCFGR_NUM_TZMA_MASK) >>
			  ETZPC_HWCFGR_NUM_TZMA_SHIFT;
	ddata->num_per_sec = (reg & ETZPC_HWCFGR_NUM_PER_SEC_MASK) >>
			     ETZPC_HWCFGR_NUM_PER_SEC_SHIFT;
	ddata->num_ahb_sec = (reg & ETZPC_HWCFGR_NUM_AHB_SEC_MASK) >>
			     ETZPC_HWCFGR_NUM_AHB_SEC_SHIFT;

	DMSG("ETZPC revision 0x%02" PRIu8 ", per_sec %u, ahb_sec %u, tzma %u",
	     io_read8(base + ETZPC_VERR),
	     ddata->num_per_sec, ddata->num_ahb_sec, ddata->num_tzma);
}

#ifdef CFG_EMBED_DTB
struct dt_id_attr {
	/* The effective size of the array is meaningless here */
	fdt32_t id_attr[1];
};

static void fdt_etzpc_conf_decprot(struct etzpc_device *dev,
				   const void *fdt, int node)
{
	const struct dt_id_attr *conf_list = NULL;
	size_t i = 0;
	int len = 0;

	conf_list = (const struct dt_id_attr *)fdt_getprop(fdt, node,
							   "st,decprot", &len);
	if (!conf_list) {
		DMSG("No ETZPC DECPROT configuration in DT");
		return;
	}

	clk_enable(dev->pdata.clk);

	for (i = 0; i < len / sizeof(uint32_t); i++) {
		uint32_t value = fdt32_to_cpu(conf_list->id_attr[i]);
		uint32_t id = (value >> ETZPC_ID_SHIFT) & ETZPC_ID_MASK;
		uint32_t mode = (value >> ETZPC_MODE_SHIFT) & ETZPC_MODE_MASK;
		bool lock = value & ETZPC_LOCK_MASK;
		enum etzpc_decprot_attributes attr = ETZPC_DECPROT_MAX;

		if (!valid_decprot_id(dev, id)) {
			DMSG("Invalid DECPROT %"PRIu32, id);
			panic();
		}

		attr = etzpc_binding2decprot(mode);
		etzpc_configure_decprot(dev, id, attr);

		if (lock)
			etzpc_lock_decprot(dev, id);
	}

	clk_disable(dev->pdata.clk);
}

static TEE_Result init_etzpc_from_dt(struct etzpc_device *etzpc_dev,
				     const void *fdt, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct dt_node_info etzpc_info = { };
	struct io_pa_va base = { };
	int len = 0;

	_fdt_fill_device_info(fdt, &etzpc_info, node);
	if (etzpc_info.reg == DT_INFO_INVALID_REG ||
	    etzpc_info.reg_size == DT_INFO_INVALID_REG_SIZE ||
	    etzpc_info.clock == DT_INFO_INVALID_CLOCK) {
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	base.pa = etzpc_info.reg;
	etzpc_dev->pdata.name = fdt_get_name(fdt, node, &len);
	etzpc_dev->pdata.base = io_pa_or_va_secure(&base, etzpc_info.reg_size);
	res = clk_dt_get_by_index(fdt, node, 0, &etzpc_dev->pdata.clk);
	if (res)
		return res;

	stm32_etzpc_set_driverdata(etzpc_dev);

	etzpc_dev->pdata.periph_cfg =
		calloc(etzpc_dev->ddata->num_per_sec,
		       sizeof(*etzpc_dev->pdata.periph_cfg));

	etzpc_dev->pdata.tzma_cfg =
		calloc(etzpc_dev->ddata->num_tzma,
		       sizeof(*etzpc_dev->pdata.tzma_cfg));
	if (!etzpc_dev->pdata.periph_cfg || !etzpc_dev->pdata.tzma_cfg)
		return TEE_ERROR_OUT_OF_MEMORY;

	fdt_etzpc_conf_decprot(etzpc_dev, fdt, node);

	return TEE_SUCCESS;
}
#else
static TEE_Result init_etzpc_from_dt(struct etzpc_device *etzpc_dev __unsued,
				     const void *fdt __unsued,
				     int node __unsued,
				     const struct dt_device_match *dm __unsued)
{
	return TEE_ERROR_NOT_SUPPORTED;
}
#endif

static TEE_Result stm32_etzpc_probe(const void *fdt, int node,
				    const void *compat_data)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct etzpc_device *etzpc_dev = stm32_etzpc_alloc();

	if (!etzpc_dev) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto err;
	}

	res = init_etzpc_from_dt(etzpc_dev, fdt, node);
	if (res)
		goto err;

	etzpc_dev->fdev = stm32_firewall_dev_alloc();
	if (!etzpc_dev->fdev) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto err;
	}

	etzpc_dev->fdev->name = etzpc_dev->pdata.name;
	etzpc_dev->fdev->ops = &stm32_etzpc_fw_ops;
	etzpc_dev->fdev->compat = (struct stm32_firewall_compat *)compat_data;
	etzpc_dev->fdev->priv = etzpc_dev;

	res = stm32_firewall_dev_register(etzpc_dev->fdev);
	if (res)
		goto err;

	stm32_firewall_bus_probe(etzpc_dev->fdev, fdt, node);

	register_pm_core_service_cb(etzpc_pm, etzpc_dev->fdev, "stm32-etzpc");

	return TEE_SUCCESS;

err:
	EMSG("ETZPC probe failed: %#"PRIx32, res);
	panic();
}

#ifdef CFG_EMBED_DTB
#ifdef CFG_STM32MP13
static struct stm32_firewall_reg
etzpc_compat_reg_mp13[STM32MP1_ETZPC_MAX_ID + 2] = {
	[STM32MP1_ETZPC_VREFBUF_ID] =		{ 0x50025000 },
	[STM32MP1_ETZPC_LPTIM2_ID] =		{ 0x50021000 },
	[STM32MP1_ETZPC_LPTIM3_ID] =		{ 0x50022000 },
	[STM32MP1_ETZPC_LTDC_ID] =		{ 0x5A001000 },
	[STM32MP1_ETZPC_DCMIPP_ID] =		{ 0x5A000000 },
	[STM32MP1_ETZPC_USBPHYCTRL_ID] =	{ 0x5A006000 },
	[STM32MP1_ETZPC_DDRCTRLPHY_ID] =	{ 0x5A003000 },
	[STM32MP1_ETZPC_IWDG1_ID] =		{ 0x5C003000 },
	[STM32MP1_ETZPC_STGENC_ID] =		{ 0x5C008000 },
	[STM32MP1_ETZPC_USART1_ID] =		{ 0x4C000000 },
	[STM32MP1_ETZPC_USART2_ID] =		{ 0x4C001000 },
	[STM32MP1_ETZPC_SPI4_ID] =		{ 0x4C002000 },
	[STM32MP1_ETZPC_SPI5_ID] =		{ 0x4C003000 },
	[STM32MP1_ETZPC_I2C3_ID] =		{ 0x4C004000 },
	[STM32MP1_ETZPC_I2C4_ID] =		{ 0x4C005000 },
	[STM32MP1_ETZPC_I2C5_ID] =		{ 0x4C006000 },
	[STM32MP1_ETZPC_TIM12_ID] =		{ 0x4C007000 },
	[STM32MP1_ETZPC_TIM13_ID] =		{ 0x4C008000 },
	[STM32MP1_ETZPC_TIM14_ID] =		{ 0x4C009000 },
	[STM32MP1_ETZPC_TIM15_ID] =		{ 0x4C00A000 },
	[STM32MP1_ETZPC_TIM16_ID] =		{ 0x4C00B000 },
	[STM32MP1_ETZPC_TIM17_ID] =		{ 0x4C00C000 },
	[STM32MP1_ETZPC_ADC1_ID] =		{ 0x48003000 },
	[STM32MP1_ETZPC_ADC2_ID] =		{ 0x48004000 },
	[STM32MP1_ETZPC_OTG_ID] =		{ 0x49000000 },
	[STM32MP1_ETZPC_TSC_ID] =		{ 0x5000B000 },
	[STM32MP1_ETZPC_RNG_ID] =		{ 0x54004000 },
	[STM32MP1_ETZPC_HASH_ID] =		{ 0x54003000 },
	[STM32MP1_ETZPC_CRYP_ID] =		{ 0x54002000 },
	[STM32MP1_ETZPC_SAES_ID] =		{ 0x54005000 },
	[STM32MP1_ETZPC_PKA_ID] =		{ 0x54006000 },
	[STM32MP1_ETZPC_BKPSRAM_ID] =		{ 0x54000000, 0x4000 },
	[STM32MP1_ETZPC_ETH1_ID] =		{ 0x5800A000 },
	[STM32MP1_ETZPC_ETH2_ID] =		{ 0x5800E000 },
	[STM32MP1_ETZPC_SDMMC1_ID] =		{ 0x58005000 },
	[STM32MP1_ETZPC_SDMMC2_ID] =		{ 0x58007000 },
	[STM32MP1_ETZPC_MCE_ID] =		{ 0x58001000 },
	[STM32MP1_ETZPC_FMC_ID] =		{ 0x58002000 },
	[STM32MP1_ETZPC_QSPI_ID] =		{ 0x58003000 },
	[STM32MP1_ETZPC_SRAM1_ID] =		{ 0x30000000, 0x4000 },
	[STM32MP1_ETZPC_SRAM2_ID] =		{ 0x30004000, 0x2000 },
	[STM32MP1_ETZPC_SRAM3_ID] =		{ 0x30006000, 0x2000 },
	/* MAX ID to catch the TZMA address */
	[STM32MP1_ETZPC_MAX_ID] =		{ 0x00000000, 0x20000},
	[STM32MP1_ETZPC_MAX_ID + 1] =		{ 0x2FFE0000, 0x20000},
};

static const struct stm32_firewall_compat etzpc_compat[] = {
	{
		.reg = etzpc_compat_reg_mp13,
		.compat_size = ARRAY_SIZE(etzpc_compat_reg_mp13)
	},
};
#endif

#ifdef CFG_STM32MP15
static struct stm32_firewall_reg
etzpc_compat_reg_mp15[STM32MP1_ETZPC_MAX_ID + 2] = {
	[STM32MP1_ETZPC_STGENC_ID] =		{ 0x5C008000 },
	[STM32MP1_ETZPC_BKPSRAM_ID] =		{ 0x54000000, 0x1000 },
	[STM32MP1_ETZPC_IWDG1_ID] =		{ 0x5C003000 },
	[STM32MP1_ETZPC_USART1_ID] =		{ 0x5C000000 },
	[STM32MP1_ETZPC_SPI6_ID] =		{ 0x5C001000 },
	[STM32MP1_ETZPC_I2C4_ID] =		{ 0x5C002000 },
	[STM32MP1_ETZPC_RNG1_ID] =		{ 0x54003000 },
	[STM32MP1_ETZPC_HASH1_ID] =		{ 0x54002000 },
	[STM32MP1_ETZPC_CRYP1_ID] =		{ 0x54001000 },
	[STM32MP1_ETZPC_DDRCTRL_ID] =		{ 0x5A003000 },
	[STM32MP1_ETZPC_DDRPHYC_ID] =		{ 0x5A004000 },
	[STM32MP1_ETZPC_I2C6_ID] =		{ 0x5C009000 },
	[STM32MP1_ETZPC_TIM2_ID] =		{ 0x40000000 },
	[STM32MP1_ETZPC_TIM3_ID] =		{ 0x40001000 },
	[STM32MP1_ETZPC_TIM4_ID] =		{ 0x40002000 },
	[STM32MP1_ETZPC_TIM5_ID] =		{ 0x40003000 },
	[STM32MP1_ETZPC_TIM6_ID] =		{ 0x40004000 },
	[STM32MP1_ETZPC_TIM7_ID] =		{ 0x40005000 },
	[STM32MP1_ETZPC_TIM12_ID] =		{ 0x40006000 },
	[STM32MP1_ETZPC_TIM13_ID] =		{ 0x40007000 },
	[STM32MP1_ETZPC_TIM14_ID] =		{ 0x40008000 },
	[STM32MP1_ETZPC_LPTIM1_ID] =		{ 0x40009000 },
	[STM32MP1_ETZPC_WWDG1_ID] =		{ 0x4000A000 },
	[STM32MP1_ETZPC_SPI2_ID] =		{ 0x4000B000 },
	[STM32MP1_ETZPC_SPI3_ID] =		{ 0x4000C000 },
	[STM32MP1_ETZPC_SPDIFRX_ID] =		{ 0x4000D000 },
	[STM32MP1_ETZPC_USART2_ID] =		{ 0x4000E000 },
	[STM32MP1_ETZPC_USART3_ID] =		{ 0x4000F000 },
	[STM32MP1_ETZPC_UART4_ID] =		{ 0x40010000 },
	[STM32MP1_ETZPC_UART5_ID] =		{ 0x40011000 },
	[STM32MP1_ETZPC_I2C1_ID] =		{ 0x40012000 },
	[STM32MP1_ETZPC_I2C2_ID] =		{ 0x40013000 },
	[STM32MP1_ETZPC_I2C3_ID] =		{ 0x40014000 },
	[STM32MP1_ETZPC_I2C5_ID] =		{ 0x40015000 },
	[STM32MP1_ETZPC_CEC_ID] =		{ 0x40016000 },
	[STM32MP1_ETZPC_DAC_ID] =		{ 0x40017000 },
	[STM32MP1_ETZPC_UART7_ID] =		{ 0x40018000 },
	[STM32MP1_ETZPC_UART8_ID] =		{ 0x40019000 },
	[STM32MP1_ETZPC_MDIOS_ID] =		{ 0x4001C000 },
	[STM32MP1_ETZPC_TIM1_ID] =		{ 0x44000000 },
	[STM32MP1_ETZPC_TIM8_ID] =		{ 0x44001000 },
	[STM32MP1_ETZPC_USART6_ID] =		{ 0x44003000 },
	[STM32MP1_ETZPC_SPI1_ID] =		{ 0x44004000 },
	[STM32MP1_ETZPC_SPI4_ID] =		{ 0x44005000 },
	[STM32MP1_ETZPC_TIM15_ID] =		{ 0x44006000 },
	[STM32MP1_ETZPC_TIM16_ID] =		{ 0x44007000 },
	[STM32MP1_ETZPC_TIM17_ID] =		{ 0x44008000 },
	[STM32MP1_ETZPC_SPI5_ID] =		{ 0x44009000 },
	[STM32MP1_ETZPC_SAI1_ID] =		{ 0x4400A000 },
	[STM32MP1_ETZPC_SAI2_ID] =		{ 0x4400B000 },
	[STM32MP1_ETZPC_SAI3_ID] =		{ 0x4400C000 },
	[STM32MP1_ETZPC_DFSDM_ID] =		{ 0x4400D000 },
	[STM32MP1_ETZPC_TT_FDCAN_ID] =		{ 0x4400E000 },
	[STM32MP1_ETZPC_LPTIM2_ID] =		{ 0x50021000 },
	[STM32MP1_ETZPC_LPTIM3_ID] =		{ 0x50022000 },
	[STM32MP1_ETZPC_LPTIM4_ID] =		{ 0x50023000 },
	[STM32MP1_ETZPC_LPTIM5_ID] =		{ 0x50024000 },
	[STM32MP1_ETZPC_SAI4_ID] =		{ 0x50027000 },
	[STM32MP1_ETZPC_VREFBUF_ID] =		{ 0x50025000 },
	[STM32MP1_ETZPC_DCMI_ID] =		{ 0x4C006000 },
	[STM32MP1_ETZPC_CRC2_ID] =		{ 0x4C004000 },
	[STM32MP1_ETZPC_ADC_ID] =		{ 0x48003000 },
	[STM32MP1_ETZPC_HASH2_ID] =		{ 0x4C002000 },
	[STM32MP1_ETZPC_RNG2_ID] =		{ 0x4C003000 },
	[STM32MP1_ETZPC_CRYP2_ID] =		{ 0x4C005000 },
	[STM32MP1_ETZPC_SRAM1_ID] =		{ 0x10000000, 0x20000 },
	[STM32MP1_ETZPC_SRAM2_ID] =		{ 0x10020000, 0x20000 },
	[STM32MP1_ETZPC_SRAM3_ID] =		{ 0x10040000, 0x10000 },
	[STM32MP1_ETZPC_SRAM4_ID] =		{ 0x10050000, 0x10000 },
	[STM32MP1_ETZPC_RETRAM_ID] =		{ 0x38000000, 0x10000 },
	[STM32MP1_ETZPC_OTG_ID] =		{ 0x49000000 },
	[STM32MP1_ETZPC_SDMMC3_ID] =		{ 0x48004000 },
	[STM32MP1_ETZPC_DLYBSD3_ID] =		{ 0x48005000 },
	[STM32MP1_ETZPC_DMA1_ID] =		{ 0x48000000 },
	[STM32MP1_ETZPC_DMA2_ID] =		{ 0x48001000 },
	[STM32MP1_ETZPC_DMAMUX_ID] =		{ 0x48002000 },
	[STM32MP1_ETZPC_FMC_ID] =		{ 0x58002000 },
	[STM32MP1_ETZPC_QSPI_ID] =		{ 0x58003000 },
	[STM32MP1_ETZPC_DLYBQ_ID] =		{ 0x58004000 },
	[STM32MP1_ETZPC_ETH_ID] =		{ 0x5800A000 },
	/* MAX ID to catch the TZMA address */
	[STM32MP1_ETZPC_MAX_ID] =		{ 0x00000000, 0x20000},
	[STM32MP1_ETZPC_MAX_ID + 1] =		{ 0x2FFC0000, 0x40000},
};

static const struct stm32_firewall_compat etzpc_compat[] = {
	{
		.reg = etzpc_compat_reg_mp15,
		.compat_size = ARRAY_SIZE(etzpc_compat_reg_mp15)
	},
};
#endif

static const struct dt_device_match etzpc_match_table[] = {
	{ .compatible = "st,stm32-etzpc", .compat_data = etzpc_compat },
	{ }
};

DEFINE_DT_DRIVER(etzpc_dt_driver) = {
	.name = "stm32-etzpc",
	.match_table = etzpc_match_table,
	.probe = stm32_etzpc_probe,
};
#endif /* CFG_EMBED_DTB */
