// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#include <assert.h>
#include <stdint.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/stm32_rif.h>
#include <drivers/stm32_risab.h>
#include <dt-bindings/soc/stm32mp25-risab.h>
#include <io.h>
#include <kernel/boot.h>
#include <kernel/dt.h>
#include <kernel/pm.h>
#include <kernel/tee_misc.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <platform_config.h>
#include <stm32_sysconf.h>

#define _RISAB_CR				U(0x0)
#define _RISAB_IASR				U(0x8)
#define _RISAB_IACR				U(0xC)
#define _RISAB_RCFGLOCKR			U(0x10)
#define _RISAB_IAESR				U(0x20)
#define _RISAB_IADDR				U(0x24)
#define _RISAB_PGy_SECCFGR(y)			(U(0x100) + (0x4 * (y)))
#define _RISAB_PGy_PRIVCFGR(y)			(U(0x200) + (0x4 * (y)))
#define _RISAB_RISAB_PGy_C2PRIVCFGR(y)		(U(0x600) + (0x4 * (y)))
#define _RISAB_CIDxPRIVCFGR(x)			(U(0x800) + (0x20 * (x)))
#define _RISAB_CIDxRDCFGR(x)			(U(0x808) + (0x20 * (x)))
#define _RISAB_CIDxWRCFGR(x)			(U(0x810) + (0x20 * (x)))
#define _RISAB_PGy_CIDCFGR(y)			(U(0xA00) + (0x4 * (y)))
#define _RISAB_HWCFGR3				U(0xFE8)
#define _RISAB_HWCFGR2				U(0xFEC)
#define _RISAB_HWCFGR1				U(0xFF0)
#define _RISAB_VERR				U(0xFF4)
#define _RISAB_IPIDR				U(0xFF8)
#define _RISAB_SIDR				U(0xFFC)

/* RISAB_CR bitfields */
#define _RISAB_CR_SRWIAD			BIT(31)

/* RISAB_IACR bitfields */
#define _RISAB_IACR_CAEF			BIT(0)
#define _RISAB_IACR_IAEF			BIT(1)

/* Define RISAB_PG_SECCFGR bitfields */
#define _RISAB_PG_SECCFGR_MASK			GENMASK_32(7, 0)

/* Define RISAB_PG_PRIVCFGR bitfields */
#define _RISAB_PG_PRIVCFGR_MASK			GENMASK_32(7, 0)

/* CIDCFGR bitfields */
#define _RISAB_PG_CIDCFGR_CFEN			BIT(0)
#define _RISAB_PG_CIDCFGR_DCEN			BIT(2)
#define _RISAB_PG_CIDCFGR_DDCID_SHIFT		U(4)
#define _RISAB_PG_CIDCFGR_DDCID_MASK		GENMASK_32(6, 4)
#define _RISAB_PG_CIDCFGR_CONF_MASK		(_RISAB_PG_CIDCFGR_CFEN | \
						 _RISAB_PG_CIDCFGR_DCEN | \
						 _RISAB_PG_CIDCFGR_DDCID_MASK)

/* Miscellaneous */
#define _RISAB_NB_PAGES_MAX			U(32)

#define _RISAB_PAGE_SIZE			U(0x1000)

struct stm32_risab_rif_conf {
	unsigned int first_page;
	unsigned int nb_pages_cfged;
	uint32_t plist[RISAB_NB_MAX_CID_SUPPORTED];
	uint32_t rlist[RISAB_NB_MAX_CID_SUPPORTED];
	uint32_t wlist[RISAB_NB_MAX_CID_SUPPORTED];
	uint32_t cidcfgr;
	uint32_t dprivcfgr;
	uint32_t seccfgr;
};

struct stm32_risab_pdata {
	unsigned int nb_regions_cfged;
	struct clk *clock;
	struct mem_region region_cfged;
	struct stm32_risab_rif_conf *subr_cfg;
#if TRACE_LEVEL >= TRACE_INFO
	char risab_name[20];
#endif
	uintptr_t base;
	uint32_t pages_configured;
	bool srwiad;

	SLIST_ENTRY(stm32_risab_pdata) link;
};

static SLIST_HEAD(, stm32_risab_pdata) risab_list =
		SLIST_HEAD_INITIALIZER(risab_list);

static bool is_tdcid;

void stm32_risab_clear_illegal_access_flags(void)
{
	struct stm32_risab_pdata *risab = NULL;

	SLIST_FOREACH(risab, &risab_list, link) {
		if (clk_enable(risab->clock))
			panic("Can't enable RISAB clock");

		if (!io_read32(risab->base + _RISAB_IASR)) {
			clk_disable(risab->clock);
			continue;
		}

		io_write32(risab->base + _RISAB_IACR, _RISAB_IACR_CAEF |
			   _RISAB_IACR_IAEF);

		clk_disable(risab->clock);
	}
}

#if TRACE_LEVEL >= TRACE_INFO
void stm32_risab_dump_erroneous_data(void)
{
	struct stm32_risab_pdata *risab = NULL;

	SLIST_FOREACH(risab, &risab_list, link) {
		if (clk_enable(risab->clock))
			panic("Can't enable RISAB clock");

		/* Check if faulty address on this RISAB */
		if (!io_read32(risab->base + _RISAB_IASR)) {
			clk_disable(risab->clock);
			continue;
		}

		EMSG("\n\nDUMPING DATA FOR %s\n\n", risab->risab_name);
		EMSG("=====================================================");
		EMSG("Status register (IAESR): %#x",
		     io_read32(risab->base + _RISAB_IAESR));
		EMSG("-----------------------------------------------------");
		EMSG("Faulty address (IADDR): %#x",
		     io_read32(risab->base + _RISAB_IADDR));

		EMSG("=====================================================\n");

		clk_disable(risab->clock);
	};
}
#endif /* TRACE_LEVEL >= TRACE_INFO */

static bool regs_access_granted(struct stm32_risab_pdata *risab_d,
				unsigned int reg_idx)
{
	unsigned int first_page = risab_d->subr_cfg[reg_idx].first_page;
	uint32_t cidcfgr = io_read32(risab_d->base +
				     _RISAB_PGy_CIDCFGR(first_page));

	/* Trusted CID access */
	if (is_tdcid &&
	    ((cidcfgr & _RISAB_PG_CIDCFGR_CFEN &&
	      !(cidcfgr & _RISAB_PG_CIDCFGR_DCEN)) ||
	     !(cidcfgr & _RISAB_PG_CIDCFGR_CFEN)))
		return true;

	/* Delegated CID access check */
	if (cidcfgr & _RISAB_PG_CIDCFGR_CFEN &&
	    cidcfgr & _RISAB_PG_CIDCFGR_DCEN &&
	    ((cidcfgr & _RISAB_PG_CIDCFGR_DDCID_MASK) >>
	     _RISAB_PG_CIDCFGR_DDCID_SHIFT) == RIF_CID1)
		return true;

	return false;
}

static void set_block_seccfgr(struct stm32_risab_pdata *risab_d,
			      unsigned int reg_idx)
{
	unsigned int i = 0;
	unsigned int first_page = risab_d->subr_cfg[reg_idx].first_page;
	unsigned int last_page = first_page +
				 risab_d->subr_cfg[reg_idx].nb_pages_cfged - 1;

	for (i = first_page; i <= last_page; i++)
		io_clrsetbits32(risab_d->base + _RISAB_PGy_SECCFGR(i),
				_RISAB_PG_SECCFGR_MASK,
				risab_d->subr_cfg[reg_idx].seccfgr);
}

static void set_block_dprivcfgr(struct stm32_risab_pdata *risab_d,
				unsigned int reg_idx)
{
	unsigned int i = 0;
	unsigned int first_page = risab_d->subr_cfg[reg_idx].first_page;
	unsigned int last_page = first_page +
				 risab_d->subr_cfg[reg_idx].nb_pages_cfged - 1;

	for (i = first_page; i <= last_page; i++)
		io_clrsetbits32(risab_d->base + _RISAB_PGy_PRIVCFGR(i),
				_RISAB_PG_PRIVCFGR_MASK,
				risab_d->subr_cfg[reg_idx].dprivcfgr);
}

static void set_cidcfgr(struct stm32_risab_pdata *risab_d,
			unsigned int reg_idx)
{
	unsigned int i = 0;
	unsigned int first_page = risab_d->subr_cfg[reg_idx].first_page;
	unsigned int last_page = first_page +
				 risab_d->subr_cfg[reg_idx].nb_pages_cfged - 1;

	for (i = first_page; i <= last_page; i++) {
		/*
		 * When TDCID, OP-TEE should be the one to set the CID filtering
		 * configuration. Clearing previous configuration prevents
		 * undesired events during the only legitimate configuration.
		 */
		io_clrsetbits32(risab_d->base + _RISAB_PGy_CIDCFGR(i),
				_RISAB_PG_CIDCFGR_CONF_MASK,
				risab_d->subr_cfg[reg_idx].cidcfgr);
	}
}

static void set_read_conf(struct stm32_risab_pdata *risab_d,
			  unsigned int reg_idx)
{
	unsigned int i = 0;
	unsigned int first_page = risab_d->subr_cfg[reg_idx].first_page;
	unsigned int last_page = first_page +
				 risab_d->subr_cfg[reg_idx].nb_pages_cfged - 1;
	uint32_t mask = GENMASK_32(last_page, first_page);

	for (i = 0; i < RISAB_NB_MAX_CID_SUPPORTED; i++) {
		io_clrsetbits32(risab_d->base + _RISAB_CIDxRDCFGR(i), mask,
				risab_d->subr_cfg[reg_idx].rlist[i]);
	}
}

static void set_write_conf(struct stm32_risab_pdata *risab_d,
			   unsigned int reg_idx)
{
	unsigned int i = 0;
	unsigned int first_page = risab_d->subr_cfg[reg_idx].first_page;
	unsigned int last_page = first_page +
				 risab_d->subr_cfg[reg_idx].nb_pages_cfged - 1;
	uint32_t mask = GENMASK_32(last_page, first_page);

	for (i = 0; i < RISAB_NB_MAX_CID_SUPPORTED; i++) {
		io_clrsetbits32(risab_d->base + _RISAB_CIDxWRCFGR(i), mask,
				risab_d->subr_cfg[reg_idx].wlist[i]);
	}
}

static void set_cid_priv_conf(struct stm32_risab_pdata *risab_d,
			      unsigned int reg_idx)
{
	unsigned int i = 0;
	unsigned int first_page = risab_d->subr_cfg[reg_idx].first_page;
	unsigned int last_page = first_page +
				 risab_d->subr_cfg[reg_idx].nb_pages_cfged - 1;
	uint32_t mask = GENMASK_32(last_page, first_page);

	for (i = 0; i < RISAB_NB_MAX_CID_SUPPORTED; i++) {
		io_clrsetbits32(risab_d->base + _RISAB_CIDxPRIVCFGR(i), mask,
				risab_d->subr_cfg[reg_idx].plist[i]);
	}
}

static void apply_rif_config(struct stm32_risab_pdata *risab_d)
{
	unsigned int i = 0;

	if (clk_enable(risab_d->clock))
		panic("Can't enable RISAB clock");

	/* Clear security/default-privilege conf, this can't generate an IAC */
	for (i = 0; i < _RISAB_NB_PAGES_MAX; i++) {
		io_clrbits32(risab_d->base + _RISAB_PGy_SECCFGR(i),
			     _RISAB_PG_SECCFGR_MASK);
		io_clrbits32(risab_d->base + _RISAB_PGy_PRIVCFGR(i),
			     _RISAB_PG_PRIVCFGR_MASK);
	}

	for (i = 0; i < risab_d->nb_regions_cfged; i++) {
		set_block_dprivcfgr(risab_d, i);

		if (!regs_access_granted(risab_d, i)) {
			DMSG("Access not granted to RISAB %s region %u",
			     risab_d->risab_name, i);
			continue;
		}

		set_block_seccfgr(risab_d, i);

		/* Grant page access to some CIDs, in read and/or write */
		set_read_conf(risab_d, i);
		set_write_conf(risab_d, i);

		/* For each granted CID define privilege access per page */
		set_cid_priv_conf(risab_d, i);

		/* Delegate RIF configuration or not */
		if (is_tdcid)
			set_cidcfgr(risab_d, i);
	}

	clk_disable(risab_d->clock);
}

static void parse_risab_rif_conf(struct stm32_risab_pdata *risab_d,
				 uint32_t rif_conf, unsigned int idx,
				 unsigned int subr_offset)
{
	unsigned int first_page = subr_offset / _RISAB_PAGE_SIZE;
	unsigned int last_page = first_page +
				 risab_d->subr_cfg[idx].nb_pages_cfged - 1;
	uint32_t reg_pages_cfged = GENMASK_32(last_page, first_page);
	unsigned int i = 0;

	assert(last_page <= _RISAB_NB_PAGES_MAX);

	risab_d->subr_cfg[idx].first_page = first_page;

	DMSG("Configuring pages %u to %u", first_page, last_page);

	/* Parse secure configuration */
	if (rif_conf & BIT(RISAB_SEC_SHIFT)) {
		risab_d->subr_cfg[idx].seccfgr = _RISAB_PG_SECCFGR_MASK;
		if (reg_pages_cfged & risab_d->pages_configured)
			panic("Memory region overlap detected");
	} else {
		risab_d->subr_cfg[idx].seccfgr = 0;
	}

	/* Parse default privilege configuration */
	if (rif_conf & BIT(RISAB_DPRIV_SHIFT)) {
		risab_d->subr_cfg[idx].dprivcfgr = _RISAB_PG_PRIVCFGR_MASK;
		if (reg_pages_cfged & risab_d->pages_configured)
			panic("Memory region overlap detected");
	} else {
		risab_d->subr_cfg[idx].dprivcfgr = 0;
	}

	risab_d->pages_configured |= reg_pages_cfged;

	for (i = 0; i < RISAB_NB_MAX_CID_SUPPORTED; i++) {
		/* RISAB compartment priv configuration */
		if (rif_conf & BIT(i)) {
			risab_d->subr_cfg[idx].plist[i] |=
			GENMASK_32(first_page +
				   risab_d->subr_cfg[idx].nb_pages_cfged - 1,
				   first_page);
		}

		/* RISAB compartment read configuration */
		if (rif_conf & BIT(i) << RISAB_READ_LIST_SHIFT) {
			risab_d->subr_cfg[idx].rlist[i] |=
			GENMASK_32(first_page +
				   risab_d->subr_cfg[idx].nb_pages_cfged - 1,
				   first_page);
		}

		/* RISAB compartment write configuration */
		if (rif_conf & BIT(i) << RISAB_WRITE_LIST_SHIFT) {
			risab_d->subr_cfg[idx].wlist[i] |=
			GENMASK_32(first_page +
				   risab_d->subr_cfg[idx].nb_pages_cfged - 1,
				   first_page);
		}
	}

	/* CID filtering configuration */
	if (rif_conf & BIT(RISAB_CFEN_SHIFT))
		risab_d->subr_cfg[idx].cidcfgr |= _RISAB_PG_CIDCFGR_CFEN;

	if (rif_conf & BIT(RISAB_DCEN_SHIFT))
		risab_d->subr_cfg[idx].cidcfgr |= _RISAB_PG_CIDCFGR_DCEN;

	if (rif_conf & RISAB_DCCID_MASK) {
		uint32_t ddcid = (((rif_conf & RISAB_DCCID_MASK) >>
				   RISAB_DCCID_SHIFT) <<
				  _RISAB_PG_CIDCFGR_DDCID_SHIFT);

		assert(((rif_conf & RISAB_DCCID_MASK) >> RISAB_DCCID_SHIFT) <
		       RISAB_NB_MAX_CID_SUPPORTED);

		risab_d->subr_cfg[idx].cidcfgr |= ddcid;
	}
}

static TEE_Result parse_dt(const void *fdt, int node,
			   struct stm32_risab_pdata *risab_d)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int mem_reg_node = 0;
	int lenp = 0;
	unsigned int i = 0;
	const fdt32_t *cuint = NULL;
	const fdt32_t *mem_regions = NULL;
	struct dt_node_info info = {};
	struct io_pa_va addr = {};

	_fdt_fill_device_info(fdt, &info, node);
	assert(info.reg != DT_INFO_INVALID_REG &&
	       info.reg_size != DT_INFO_INVALID_REG_SIZE);

	addr.pa = info.reg;
	risab_d->base = io_pa_or_va(&addr, info.reg_size);

	/* Gate the IP */
	res = clk_dt_get_by_index(fdt, node, 0, &risab_d->clock);
	if (res)
		return res;

#if TRACE_LEVEL >= TRACE_INFO
	strncpy(risab_d->risab_name, fdt_get_name(fdt, node, NULL), 19);
#endif

	cuint = fdt_getprop(fdt, node, "st,srwiad", NULL);
	if (cuint)
		risab_d->srwiad = true;

	/* Get the memory region being configured */
	cuint = fdt_getprop(fdt, node, "st,mem-map", &lenp);
	if (!cuint)
		panic("Missing st,mem-map property in configure memory region");

	assert((unsigned int)(lenp / sizeof(uint32_t)) == 2);

	risab_d->region_cfged.base = fdt32_to_cpu(cuint[0]);
	risab_d->region_cfged.size = fdt32_to_cpu(cuint[1]);

	/* Get the memory regions to configure */
	mem_regions = fdt_getprop(fdt, node, "memory-region", &lenp);
	if (!mem_regions)
		panic("No memory region to configure");

	risab_d->nb_regions_cfged = (unsigned int)(lenp / sizeof(uint32_t));
	assert(risab_d->nb_regions_cfged < _RISAB_NB_PAGES_MAX);

	risab_d->subr_cfg = calloc(risab_d->nb_regions_cfged,
				   sizeof(*risab_d->subr_cfg));
	if (!risab_d->subr_cfg)
		return TEE_ERROR_OUT_OF_MEMORY;

	for (i = 0; i < risab_d->nb_regions_cfged; i++) {
		uintptr_t sub_region_offset = 0;
		uintptr_t address = 0;
		size_t length = 0;

		mem_reg_node =
		fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(mem_regions[i]));
		if (mem_reg_node < 0)
			return TEE_ERROR_ITEM_NOT_FOUND;

		/*
		 * Get the reg property to determine the number of pages
		 * to configure
		 */
		address = (uintptr_t)_fdt_reg_base_address(fdt, mem_reg_node);
		length = _fdt_reg_size(fdt, mem_reg_node);

		/*
		 * Get the sub region offset and check if it is not out
		 * of bonds
		 */
		sub_region_offset = address - risab_d->region_cfged.base;

		assert(sub_region_offset < (risab_d->region_cfged.base +
					    risab_d->region_cfged.size));

		risab_d->subr_cfg[i].nb_pages_cfged = length /
						      _RISAB_PAGE_SIZE;
		if (!risab_d->subr_cfg[i].nb_pages_cfged)
			panic("Range to configure is < to the size of a page");

		/* Get the RIF configuration for this region */
		cuint = fdt_getprop(fdt, mem_reg_node, "st,protreg", &lenp);
		if (!cuint)
			panic("No RIF configuration available");

		/* There should be only one configuration for this region */
		assert((unsigned int)(lenp / sizeof(uint32_t)) == 1);

		parse_risab_rif_conf(risab_d, fdt32_to_cpu(cuint[0]), i,
				     sub_region_offset);
	}

	return TEE_SUCCESS;
}

static void set_srswiad_conf(struct stm32_risab_pdata *risab_d)
{
	if (risab_d->srwiad)
		io_setbits32(risab_d->base, _RISAB_CR_SRWIAD);
};

static void clean_iac_regs(struct stm32_risab_pdata *risab_d)
{
	io_setbits32(risab_d->base + _RISAB_IACR, GENMASK_32(1, 0));
}

static void set_vderam_syscfg(struct stm32_risab_pdata *risab_d)
{
	/*
	 * Set the VDERAMCR_VDERAM_EN bit if the VDERAM should be accessed by
	 * the system. Else, clear it so that VDEC/VENC can access it.
	 */
	if (risab_d->nb_regions_cfged)
		stm32mp_syscfg_write(SYSCFG_VDERAMCR, VDERAMCR_VDERAM_EN,
				     VDERAMCR_MASK);
	else
		stm32mp_syscfg_write(SYSCFG_VDERAMCR, 0, VDERAMCR_MASK);
}

TEE_Result stm32_risab_reconfigure_region(uintptr_t reg_base, size_t reg_len,
					  void **conf)
{
	struct stm32_risab_rif_conf new_conf = { };
	struct stm32_risab_rif_conf *old_conf = NULL;
	struct stm32_risab_pdata *risab = NULL;
	unsigned int first_page = 0;
	unsigned int last_page = 0;
	uintptr_t region_offset = 0;
	bool keep_old_conf = false;
	unsigned int i = 0;
	unsigned int j = 0;

	assert(conf);

	if (!*conf) {
		keep_old_conf = true;
		old_conf = calloc(1, sizeof(*old_conf));
		if (!old_conf)
			return TEE_ERROR_OUT_OF_MEMORY;

		new_conf.seccfgr = GENMASK_32(7, 0);
		new_conf.dprivcfgr = GENMASK_32(7, 0);
		new_conf.plist[0] = RIF_CID1_BF;
		new_conf.rlist[0] = RIF_CID1_BF;
		new_conf.wlist[0] = RIF_CID1_BF;
		new_conf.cidcfgr = 0x1;
	} else {
		new_conf = **(struct stm32_risab_rif_conf **)conf;
	}

	assert(((new_conf.cidcfgr & _RISAB_PG_CIDCFGR_DDCID_MASK) <<
		_RISAB_PG_CIDCFGR_DDCID_SHIFT) < RISAB_NB_MAX_CID_SUPPORTED);

	SLIST_FOREACH(risab, &risab_list, link) {
		if (!core_is_buffer_inside(reg_base, reg_len,
					   risab->region_cfged.base,
					   risab->region_cfged.size))
			continue;

		region_offset = reg_base - risab->region_cfged.base;
		first_page = region_offset / _RISAB_PAGE_SIZE;
		last_page = (reg_len / _RISAB_PAGE_SIZE) + first_page - 1;

		for (i = 0; i < risab->nb_regions_cfged; i++) {
			/* Make sure the reconfigured zone is identified */
			if (first_page == risab->subr_cfg[i].first_page &&
			    last_page == risab->subr_cfg[i].first_page +
			    risab->subr_cfg[i].nb_pages_cfged - 1)
				goto found;
		};
	};

	free(old_conf);

	return TEE_ERROR_ITEM_NOT_FOUND;

found:
	if (clk_enable(risab->clock))
		panic();

	for (j = risab->subr_cfg[i].first_page;
	     j <= risab->subr_cfg[i].first_page +
	     risab->subr_cfg[i].nb_pages_cfged;
	     j++) {
		/* If TDCID, clear the CID configuration to access registers */
		if (is_tdcid)
			io_clrsetbits32(risab->base + _RISAB_PGy_CIDCFGR(j),
					_RISAB_PG_CIDCFGR_CONF_MASK, 0);

		if (!regs_access_granted(risab, i)) {
			EMSG("Memory %#lx-%#zx cannot be reconfigured",
			     reg_base, reg_len);

			free(old_conf);

			return TEE_ERROR_ACCESS_DENIED;
		}
	}
	clk_disable(risab->clock);

	if (keep_old_conf)
		memcpy(old_conf, &risab->subr_cfg[i], sizeof(*old_conf));

	/* Secure configuration */
	risab->subr_cfg[i].seccfgr = new_conf.seccfgr;

	/* Default privilege configuration */
	risab->subr_cfg[i].dprivcfgr = new_conf.dprivcfgr;

	for (j = 0; j < RISAB_NB_MAX_CID_SUPPORTED; j++) {
		risab->subr_cfg[i].plist[j] = 0;
		risab->subr_cfg[i].rlist[j] = 0;
		risab->subr_cfg[i].wlist[j] = 0;

		/* RISAB compartment priv configuration */
		if (new_conf.plist[0] & BIT(j)) {
			risab->subr_cfg[i].plist[j] |=
			GENMASK_32(first_page +
				   risab->subr_cfg[i].nb_pages_cfged - 1,
				   first_page);
		}

		/* RISAB compartment read configuration */
		if (new_conf.rlist[0] & BIT(j)) {
			risab->subr_cfg[i].rlist[j] |=
			GENMASK_32(first_page +
				   risab->subr_cfg[i].nb_pages_cfged - 1,
				   first_page);
		}

		/* RISAB compartment write configuration */
		if (new_conf.wlist[0] & BIT(j)) {
			risab->subr_cfg[i].wlist[j] |=
			GENMASK_32(first_page +
				   risab->subr_cfg[i].nb_pages_cfged - 1,
				   first_page);
		}
	}

	/* CID filtering configuration */
	risab->subr_cfg[i].cidcfgr = 0;
	if (new_conf.cidcfgr & _RISAB_PG_CIDCFGR_CFEN)
		risab->subr_cfg[i].cidcfgr |= _RISAB_PG_CIDCFGR_CFEN;

	if (new_conf.cidcfgr & _RISAB_PG_CIDCFGR_DCEN)
		risab->subr_cfg[i].cidcfgr |= _RISAB_PG_CIDCFGR_DCEN;

	risab->subr_cfg[i].cidcfgr |= new_conf.cidcfgr &
				      (_RISAB_PG_CIDCFGR_DDCID_MASK |
				       _RISAB_PG_CIDCFGR_CFEN |
				       _RISAB_PG_CIDCFGR_DCEN);

	apply_rif_config(risab);

	/* Save old configuration */
	if (keep_old_conf) {
		*conf = old_conf;
	} else {
		free(*conf);
		*conf = NULL;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_risab_pm_resume(struct stm32_risab_pdata *risab)
{
	size_t i = 0;

	if (is_tdcid) {
		if (virt_to_phys((void *)risab->base) == RISAB6_BASE)
			set_vderam_syscfg(risab);
		set_srswiad_conf(risab);
		clean_iac_regs(risab);
	}

	for (i = 0; i < risab->nb_regions_cfged; i++) {
		/* Restoring RISAB RIF configuration */
		set_block_dprivcfgr(risab, i);

		if (!regs_access_granted(risab, i))
			continue;

		set_cidcfgr(risab, i);

		/*
		 * This sequence will generate an IAC if the CID filtering
		 * configuration is inconsistent with these desired rights
		 * to apply.
		 */
		set_block_seccfgr(risab, i);
		set_read_conf(risab, i);
		set_write_conf(risab, i);
		set_cid_priv_conf(risab, i);
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_risab_pm_suspend(struct stm32_risab_pdata *risab)
{
	size_t i = 0;

	for (i = 0; i < risab->nb_regions_cfged; i++) {
		size_t j = 0;
		uintptr_t base = risab->base;
		unsigned int first_page = risab->subr_cfg[i].first_page;

		/* Save all configuration fields that need to be restored */
		risab->subr_cfg[i].seccfgr =
			io_read32(base + _RISAB_PGy_SECCFGR(first_page));
		risab->subr_cfg[i].dprivcfgr =
			io_read32(base + _RISAB_PGy_PRIVCFGR(first_page));
		risab->subr_cfg[i].cidcfgr =
			io_read32(base + _RISAB_PGy_CIDCFGR(first_page));

		for (j = 0; j < RISAB_NB_MAX_CID_SUPPORTED; j++) {
			risab->subr_cfg[i].rlist[j] =
				io_read32(base + _RISAB_CIDxRDCFGR(j));
			risab->subr_cfg[i].wlist[j] =
				io_read32(base + _RISAB_CIDxWRCFGR(j));
			risab->subr_cfg[i].plist[j] =
				io_read32(base + _RISAB_CIDxPRIVCFGR(j));
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result
stm32_risab_pm(enum pm_op op, unsigned int pm_hint,
	       const struct pm_callback_handle *pm_handle)
{
	struct stm32_risab_pdata *risab = pm_handle->handle;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!PM_HINT_IS_STATE(pm_hint, CONTEXT) || !is_tdcid)
		return TEE_SUCCESS;

	res = clk_enable(risab->clock);
	if (res)
		return res;

	if (op == PM_OP_RESUME)
		res = stm32_risab_pm_resume(risab);
	else
		res = stm32_risab_pm_suspend(risab);

	clk_disable(risab->clock);

	return res;
}

static TEE_Result stm32_risab_probe(const void *fdt, int node,
				    const void *compat_data __maybe_unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct stm32_risab_pdata *risab_d = NULL;

	res = stm32_rifsc_check_tdcid(&is_tdcid);
	if (res)
		return res;

	risab_d = calloc(1, sizeof(*risab_d));
	if (!risab_d)
		return TEE_ERROR_OUT_OF_MEMORY;

	res = parse_dt(fdt, node, risab_d);
	if (res)
		goto err;

	if (clk_enable(risab_d->clock))
		panic("Can't enable RISAB clock");

	if (is_tdcid) {
		if (virt_to_phys((void *)risab_d->base) == RISAB6_BASE)
			set_vderam_syscfg(risab_d);
		clean_iac_regs(risab_d);
		set_srswiad_conf(risab_d);
	}

	apply_rif_config(risab_d);

	clk_disable(risab_d->clock);

	SLIST_INSERT_HEAD(&risab_list, risab_d, link);

	if (IS_ENABLED(CFG_PM))
		register_pm_core_service_cb(stm32_risab_pm, risab_d,
					    "stm32-risab");

	return TEE_SUCCESS;

err:
	free(risab_d->subr_cfg);
	free(risab_d);

	return res;
}

static const struct dt_device_match risab_match_table[] = {
	{ .compatible = "st,stm32mp25-risab" },
	{ }
};

DEFINE_DT_DRIVER(risab_dt_driver) = {
	.name = "stm32-risab",
	.match_table = risab_match_table,
	.probe = stm32_risab_probe,
};
