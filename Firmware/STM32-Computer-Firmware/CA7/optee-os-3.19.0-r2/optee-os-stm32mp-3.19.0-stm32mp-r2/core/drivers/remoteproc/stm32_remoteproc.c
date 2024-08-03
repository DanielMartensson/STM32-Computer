// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2023, STMicroelectronics
 */

#include <assert.h>
#include <config.h>
#include <drivers/rstctrl.h>
#include <drivers/stm32_remoteproc.h>
#include <keep.h>
#include <kernel/cache_helpers.h>
#include <kernel/dt_driver.h>
#include <kernel/pm.h>
#include <kernel/tee_misc.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#ifdef CFG_STM32MP25
#include <stm32_sysconf.h>
#endif /* CFG_STM32MP25 */
#include <stm32_util.h>

#define TIMEOUT_US_1MS	U(1000)

#define INITVTOR_MASK	GENMASK_32(31, 7)

/**
 * struct stm32_rproc_mem - Memory regions used by the remote processor
 *
 * @addr:	physical base address from the CPU space perspective
 * @da:		device address corresponding to the physical base address
 *		from remote processor space perspective
 * @size:	size of the region
 * @phandle:	phandle of the memory region
 * @config:	pointer the stored firewall config
 */
struct stm32_rproc_mem {
	paddr_t addr;
	paddr_t da;
	size_t size;
	uint32_t phandle;
#ifdef CFG_STM32MP25
	void *config;
#else
	const void *config;
#endif
};

/**
 * struct stm32_rproc_instance - rproc instance context
 *
 * @cdata:	pointer to the device compatible data
 * @fdt:	device tree file to work on
 * @link:	the node in the rproc_list
 * @n_regions:	number of memory regions
 * @regions:	memory regions used
 * @mcu_rst:	remote processor reset control
 * @hold_boot:	remote processor hold boot control
 * @boot_addr:	boot address
 * @tzen:	indicate if the remote processor should enable the TrustZone
 * @m_get_cnt:	counter used for the memory region get/release balancing
 */
struct stm32_rproc_instance {
	const struct stm32_rproc_compat_data *cdata;
	const void *fdt;
	SLIST_ENTRY(stm32_rproc_instance) link;
	size_t n_regions;
	struct stm32_rproc_mem *regions;
	struct rstctrl *mcu_rst;
	struct rstctrl *hold_boot;
	paddr_t boot_addr;
	bool tzen;
	uint32_t m33_cr_right;
	uint32_t m_get_cnt;
};

/**
 * struct stm32_rproc_compat_data - rproc associated data for compatible list
 *
 * @rproc_id:	Unique Id of the processor
 * @start:	remote processor start routine
 * @ns_loading:	specify if the firmware is loaded by the OP-TEE or by the
 *		non secure context
 */
struct stm32_rproc_compat_data {
	uint32_t rproc_id;
	TEE_Result (*start)(struct stm32_rproc_instance *rproc);
	TEE_Result (*pm)(enum pm_op op, unsigned int pm_hint,
			 const struct pm_callback_handle *pm_handle);
	bool ns_loading;
};

static SLIST_HEAD(, stm32_rproc_instance) rproc_list =
		SLIST_HEAD_INITIALIZER(rproc_list);

void *stm32_rproc_get(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = NULL;

	SLIST_FOREACH(rproc, &rproc_list, link)
		if (rproc->cdata->rproc_id == rproc_id)
			break;

	return rproc;
}

/* Re-apply default access right on the memory regions */
static TEE_Result
stm32_rproc_release_mems_access(struct stm32_rproc_instance *rproc)
{
	struct stm32_rproc_mem *mems = rproc->regions;
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int i = 0;

	rproc->m_get_cnt--;
	if (rproc->m_get_cnt)
		return TEE_SUCCESS;

	for (i = 0; i < rproc->n_regions; i++) {
		if (!mems[i].config)
			continue;
		DMSG("Release access of the memory region %#"PRIxPA" size %#zx",
		     mems[i].addr, mems[i].size);
#ifdef CFG_STM32MP25
		res = stm32_rif_reconfigure_mem_region(rproc->fdt,
						       mems[i].phandle,
						       &mems[i].config);
#else
		res = stm32_reconfigure_region(rproc->fdt, mems[i].phandle,
					       &mems[i].config);
#endif /* CFG_STM32MP25 */
		if (res)
			EMSG("Failed to apply access rights on region %#"
			     PRIxPA" size %#zx", mems[i].addr, mems[i].size);

		mems[i].config = NULL;
	}

	return res;
}

/* Get the exclusive access on the memory regions */
static TEE_Result
stm32_rproc_get_mems_access(struct stm32_rproc_instance *rproc)
{
	struct stm32_rproc_mem *mems = rproc->regions;
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int i = 0;

	rproc->m_get_cnt++;
	if (rproc->m_get_cnt > 1)
		return TEE_SUCCESS;

	for (i = 0; i < rproc->n_regions; i++) {
		DMSG("get access of the memory region %#"PRIxPA" size %#zx",
		     mems[i].addr, mems[i].size);
		mems[i].config = NULL;
#ifdef CFG_STM32MP25
		res = stm32_rif_reconfigure_mem_region(rproc->fdt,
						       mems[i].phandle,
						       &mems[i].config);
#else
		res = stm32_reconfigure_region(rproc->fdt, mems[i].phandle,
					       &mems[i].config);
#endif /* CFG_STM32MP25 */
		if (res)
			goto err;
	}

	return TEE_SUCCESS;

err:
	stm32_rproc_release_mems_access(rproc);

	return res;
}

static TEE_Result stm32mp2_rproc_start(struct stm32_rproc_instance *rproc)
{
	struct stm32_rproc_mem *mems = NULL;
	unsigned int i = 0;
	bool boot_addr_valid = false;

	if (!rproc->boot_addr)
		return TEE_ERROR_GENERIC;

	mems = rproc->regions;

	/* Check that the boot address is in declared regions */
	for (i = 0; i < rproc->n_regions; i++) {
		if (!core_is_buffer_inside(rproc->boot_addr, 1, mems[i].addr,
					   mems[i].size))
			continue;

#ifdef CFG_STM32MP25
		if (rproc->tzen) {
			stm32mp_syscfg_write(A35SSC_M33_INITSVTOR_CR,
					     rproc->boot_addr, INITVTOR_MASK);

			stm32mp_syscfg_write(A35SSC_M33_TZEN_CR,
					     A35SSC_M33_TZEN_CR_CFG_SECEXT,
					     A35SSC_M33_TZEN_CR_CFG_SECEXT);
		} else {
			stm32mp_syscfg_write(A35SSC_M33_INITNSVTOR_CR,
					     rproc->boot_addr, INITVTOR_MASK);
		}
		boot_addr_valid = true;
		break;
#endif /* CFG_STM32MP25 */
	}

	if (!boot_addr_valid) {
		EMSG("Invalid boot address");
		return TEE_ERROR_GENERIC;
	}

	return TEE_SUCCESS;
}

TEE_Result stm32_rproc_start(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!rproc || !rproc->hold_boot)
		return TEE_ERROR_GENERIC;

	if (rproc->cdata->start) {
		res = rproc->cdata->start(rproc);
		if (res)
			return res;
	}

	/*
	 * The firmware is started by de-asserting the hold boot and
	 * asserting it back to avoid auto restart on a crash.
	 * No need to release the MCU reset as it is automatically released by
	 * the hardware.
	 */
	res = rstctrl_deassert_to(rproc->hold_boot, TIMEOUT_US_1MS);
	if (!res)
		res = rstctrl_assert_to(rproc->hold_boot, TIMEOUT_US_1MS);

	return res;
}

static TEE_Result rproc_stop(struct stm32_rproc_instance *rproc)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!rproc->hold_boot || !rproc->mcu_rst)
		return TEE_ERROR_GENERIC;

	res = rstctrl_assert_to(rproc->hold_boot, TIMEOUT_US_1MS);
	if (res)
		return res;
	res = rstctrl_assert_to(rproc->mcu_rst, TIMEOUT_US_1MS);
	if (res)
		return res;

#ifdef CFG_STM32MP25
	/* Disable the TrustZone */
	stm32mp_syscfg_write(A35SSC_M33_TZEN_CR, 0,
			     A35SSC_M33_TZEN_CR_CFG_SECEXT);
#endif /* CFG_STM32MP25 */

	rproc->boot_addr = 0;
	rproc->tzen = false;

	return TEE_SUCCESS;
}

TEE_Result stm32_rproc_stop(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);

	if (!rproc)
		return TEE_ERROR_BAD_PARAMETERS;

	return rproc_stop(rproc);
}

TEE_Result stm32_rproc_da_to_pa(uint32_t rproc_id, paddr_t da, size_t size,
				paddr_t *pa)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);
	struct stm32_rproc_mem *mems = NULL;
	unsigned int i = 0;

	if (!rproc)
		return TEE_ERROR_BAD_PARAMETERS;

	mems = rproc->regions;

	for (i = 0; i < rproc->n_regions; i++) {
		if (core_is_buffer_inside(da, size, mems[i].da, mems[i].size)) {
			/*
			 * A match between the requested DA memory area and the
			 * registered regions has been found.
			 * The PA is the reserved-memory PA address plus the
			 * delta between the requested DA and the
			 * reserved-memory DA address.
			 */
			*pa = mems[i].addr + da - mems[i].da;
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_ACCESS_DENIED;
}

static TEE_Result stm32_rproc_map_mem(paddr_t pa, size_t size, void **va)
{
	*va = core_mmu_add_mapping(MEM_AREA_RAM_SEC, pa, size);
	if (!*va) {
		EMSG("Can't map region %#"PRIxPA" size %zu", pa, size);
		return TEE_ERROR_GENERIC;
	}

	return TEE_SUCCESS;
}

TEE_Result stm32_rproc_map(uint32_t rproc_id, paddr_t pa, size_t size,
			   void **va)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);
	struct stm32_rproc_mem *mems = NULL;
	unsigned int i = 0;

	if (!rproc)
		return TEE_ERROR_BAD_PARAMETERS;

	mems = rproc->regions;

	for (i = 0; i < rproc->n_regions; i++) {
		if (!core_is_buffer_inside(pa, size, mems[i].addr,
					   mems[i].size))
			continue;

		return stm32_rproc_map_mem(pa, size, va);
	}

	return TEE_ERROR_ACCESS_DENIED;
}

static TEE_Result stm32_rproc_unmap_mem(void *va, size_t size)
{
	/* Flush the cache before unmapping the memory */
	dcache_clean_range(va, size);

	if (core_mmu_remove_mapping(MEM_AREA_RAM_SEC, va, size)) {
		EMSG("Can't unmap region %p size %zu", va, size);
		return TEE_ERROR_GENERIC;
	}

	return TEE_SUCCESS;
}

TEE_Result stm32_rproc_unmap(uint32_t rproc_id, void *va, size_t size)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);
	struct stm32_rproc_mem *mems = NULL;
	paddr_t pa = virt_to_phys(va);
	unsigned int i = 0;

	if (!rproc || !pa)
		return TEE_ERROR_BAD_PARAMETERS;

	mems = rproc->regions;

	for (i = 0; i < rproc->n_regions; i++) {
		if (!core_is_buffer_inside(pa, size, mems[i].addr,
					   mems[i].size))
			continue;

		return stm32_rproc_unmap_mem(va, size);
	}

	return TEE_ERROR_ACCESS_DENIED;
}

static TEE_Result stm32_rproc_get_dma_range(struct stm32_rproc_mem *region,
					    const void *fdt, int node)
{
	const fdt32_t *list = NULL;
	int ahb_node = 0;
	int len = 0;
	int nranges = 0;
	int i = 0;

	/*
	 * The match between local and remote processor memory mapping is
	 * described in the dma-ranges defined by the bus parent node.
	 */
	ahb_node = fdt_parent_offset(fdt, node);

	list = fdt_getprop(fdt, ahb_node, "dma-ranges", &len);
	if (!list) {
		if (len != -FDT_ERR_NOTFOUND)
			return TEE_ERROR_GENERIC;
		/* Same memory mapping */
		DMSG("No dma-ranges found in DT");
		region->da = region->addr;
		return TEE_SUCCESS;
	}

	if ((len % (sizeof(uint32_t) * 3)))
		return TEE_ERROR_GENERIC;

	nranges = len / sizeof(uint32_t);

	for (i = 0; i < nranges; i += 3) {
		uint32_t da = fdt32_to_cpu(list[i]);
		uint32_t pa = fdt32_to_cpu(list[i + 1]);
		uint32_t size = fdt32_to_cpu(list[i + 2]);

		if (core_is_buffer_inside(region->addr, region->size,
					  pa, size)) {
			region->da = da + (region->addr - pa);
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_BAD_PARAMETERS;
}

TEE_Result stm32_rproc_clean(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);
	struct stm32_rproc_mem *mems = NULL;
	unsigned int i = 0;
	paddr_t pa = 0;
	void *va = NULL;
	size_t size = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!rproc)
		return TEE_ERROR_GENERIC;

	res = stm32_rproc_get_mems_access(rproc);
	if (res)
		return res;

	mems = rproc->regions;
	for (i = 0; i < rproc->n_regions; i++) {
		pa = mems[i].addr;
		size = mems[i].size;
		res = stm32_rproc_map_mem(pa, size, &va);
		if (res)
			break;
		memset(va, 0, size);
		res = stm32_rproc_unmap_mem(va, size);
		if (res)
			break;
	}

	stm32_rproc_release_mems_access(rproc);

	return res;
}

TEE_Result stm32_rproc_get_mem(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);

	if (!rproc)
		return TEE_ERROR_GENERIC;

	return stm32_rproc_get_mems_access(rproc);
}

TEE_Result stm32_rproc_release_mem(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);

	if (!rproc)
		return TEE_ERROR_GENERIC;

	return stm32_rproc_release_mems_access(rproc);
}

TEE_Result stm32_rproc_set_boot_address(uint32_t rproc_id, paddr_t address)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);

	if (!rproc)
		return TEE_ERROR_GENERIC;

	if (rproc->boot_addr) {
		DMSG("Firmware boot address already set");
		return TEE_ERROR_GENERIC;
	}

	rproc->boot_addr = address;

	return TEE_SUCCESS;
}

TEE_Result stm32_rproc_enable_sec_boot(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);

	if (!rproc)
		return TEE_ERROR_GENERIC;

	if (rproc->tzen) {
		DMSG("Firmware TrustZone already enabled");
		return TEE_ERROR_GENERIC;
	}

	rproc->tzen = true;

	return TEE_SUCCESS;
}

/* Get device tree memory regions reserved for the Cortex-M and the IPC */
static TEE_Result stm32_rproc_parse_mems(struct stm32_rproc_instance *rproc,
					 const void *fdt, int node)
{
	const fdt32_t *list = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;
	struct stm32_rproc_mem *regions = NULL;
	int len = 0;
	int n_regions = 0;
	int i = 0;

	/*
	 * In case of firmware loading by the non secure context no need to
	 * register memory regions, so we ignore them.
	 */
	if (rproc->cdata->ns_loading)
		return TEE_SUCCESS;

	list = fdt_getprop(fdt, node, "memory-region", &len);
	if (!list) {
		EMSG("No memory regions found in DT");
		return TEE_ERROR_GENERIC;
	}

	n_regions = len / sizeof(uint32_t);

	regions = calloc(n_regions, sizeof(*regions));
	if (!regions)
		return TEE_ERROR_OUT_OF_MEMORY;

	for (i = 0; i < n_regions; i++) {
		int pnode = 0;
		uint32_t sec_mem = 0;

		pnode = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(list[i]));
		if (pnode < 0) {
			res = TEE_ERROR_GENERIC;
			goto err;
		}

		regions[i].addr = _fdt_reg_base_address(fdt, pnode);
		regions[i].size = _fdt_reg_size(fdt, pnode);
		regions[i].phandle = fdt32_to_cpu(list[i]);

		if (regions[i].addr <= 0 || regions[i].size <= 0) {
			res = TEE_ERROR_GENERIC;
			goto err;
		}

		res = stm32_rproc_get_dma_range(&regions[i], fdt, node);
		if (res)
			goto err;

		/* TODO remove temporary property and use firewall */
		res = _fdt_read_uint32_index(fdt, node, "st,s-memory-region",
					     i, &sec_mem);
		if (res)
			sec_mem = 0;

		if (!regions[i].addr || !regions[i].size) {
			res = TEE_ERROR_BAD_PARAMETERS;
			goto err;
		}

		DMSG("register %s region %#"PRIxPA" size %#zx",
		     sec_mem ? "sec" : " nsec",
		     regions[i].addr, regions[i].size);
	}

	rproc->n_regions = n_regions;
	rproc->regions = regions;

	return TEE_SUCCESS;

err:
	free(regions);

	return res;
}

static void stm32_rproc_cleanup(struct stm32_rproc_instance *rproc)
{
	free(rproc->regions);
	free(rproc);
}

static void stm32_rproc_a35ss_cfg(struct stm32_rproc_instance *rproc __unused)
{
#ifdef CFG_STM32MP25
	stm32mp_syscfg_write(A35SSC_M33CFG_ACCESS_CR, rproc->m33_cr_right,
			     A35SSC_M33_TZEN_CR_M33CFG_SEC |
			     A35SSC_M33_TZEN_CR_M33CFG_PRIV);
	/* Disable the TrustZone that is enabled by default */
	stm32mp_syscfg_write(A35SSC_M33_TZEN_CR, 0,
			     A35SSC_M33_TZEN_CR_CFG_SECEXT);

#endif /* CFG_STM32MP25 */
}

static TEE_Result stm32mp25_rproc_pm(enum pm_op op, unsigned int pm_hint,
				     const struct pm_callback_handle *pm_handle)
{
	struct stm32_rproc_instance *rproc = pm_handle->handle;

	if (PM_HINT_IS_STATE(pm_hint, CONTEXT) && op == PM_OP_RESUME)
		stm32_rproc_a35ss_cfg(rproc);

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(stm32mp25_rproc_pm);

static TEE_Result stm32_rproc_probe(const void *fdt, int node,
				    const void *comp_data)
{
	struct stm32_rproc_instance *rproc = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	rproc = calloc(1, sizeof(*rproc));
	if (!rproc)
		return TEE_ERROR_OUT_OF_MEMORY;

#ifdef CFG_STM32MP25
	rproc->m33_cr_right = A35SSC_M33_TZEN_CR_M33CFG_SEC |
			      A35SSC_M33_TZEN_CR_M33CFG_PRIV;
#endif /* CFG_STM32MP25 */

	rproc->cdata = comp_data;
	rproc->fdt = fdt;

	if (!rproc->cdata->ns_loading) {
		res = stm32_rproc_parse_mems(rproc, fdt, node);
		if (res)
			goto err;
	}

	res = rstctrl_dt_get_by_name(fdt, node, "mcu_rst", &rproc->mcu_rst);
	if (res)
		goto err;

	res = rstctrl_dt_get_by_name(fdt, node, "hold_boot", &rproc->hold_boot);
	if (res)
		goto err;

	/* Ensure that the remote processor is in expected stop state */
	res = rproc_stop(rproc);
	if (res)
		goto err;

#ifdef CFG_STM32MP25
	if (rproc->cdata->ns_loading) {
		/*
		 * The remote firmware will be loaded by the non secure
		 * Provide access rights to A35SSC_M33 registers
		 * to the non secure context
		 */
		rproc->m33_cr_right = A35SSC_M33_TZEN_CR_M33CFG_PRIV;
	}
	stm32_rproc_a35ss_cfg(rproc);
#endif /* CFG_STM32MP25 */

	if (!rproc->cdata->ns_loading)
		SLIST_INSERT_HEAD(&rproc_list, rproc, link);

	if (rproc->cdata->pm)
		register_pm_driver_cb(rproc->cdata->pm, rproc, "stm32-rproc");

	return TEE_SUCCESS;

err:
	stm32_rproc_cleanup(rproc);
	return res;
}

TEE_Result stm32_rproc_reset_grant_access(uint32_t rproc_id)
{
	struct stm32_rproc_instance *rproc = stm32_rproc_get(rproc_id);

	if (rproc && !rproc->cdata->ns_loading)
		return TEE_ERROR_ACCESS_DENIED;

	return TEE_SUCCESS;
}

static const struct stm32_rproc_compat_data stm32_rproc_m4_tee_compat = {
	.rproc_id = STM32MP1_M4_RPROC_ID,
	.ns_loading = false,
};

static const struct stm32_rproc_compat_data stm32_rproc_m33_compat = {
	.rproc_id = STM32MP2_M33_RPROC_ID,
	.start = stm32mp2_rproc_start,
	.pm = stm32mp25_rproc_pm,
	.ns_loading = true,
};

static const struct stm32_rproc_compat_data stm32_rproc_m33_tee_compat = {
	.rproc_id = STM32MP2_M33_RPROC_ID,
	.start = stm32mp2_rproc_start,
	.pm = stm32mp25_rproc_pm,
	.ns_loading = false,
};

static const struct dt_device_match stm32_rproc_match_table[] = {
	{
		.compatible = "st,stm32mp1-m4-tee",
		.compat_data = &stm32_rproc_m4_tee_compat,
	},
	{
		.compatible = "st,stm32mp2-m33",
		.compat_data = &stm32_rproc_m33_compat,
	},
	{
		.compatible = "st,stm32mp2-m33-tee",
		.compat_data = &stm32_rproc_m33_tee_compat,
	},
	{ }
};

DEFINE_DT_DRIVER(stm32_rproc_dt_driver) = {
	.name = "stm32-rproc",
	.match_table = stm32_rproc_match_table,
	.probe = &stm32_rproc_probe,
};
