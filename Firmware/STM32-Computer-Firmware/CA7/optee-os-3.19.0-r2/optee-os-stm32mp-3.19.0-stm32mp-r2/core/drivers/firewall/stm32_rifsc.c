// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2021-2023, STMicroelectronics
 */

#include <drivers/stm32_firewall.h>
#include <drivers/stm32_rif.h>
#include <drivers/stm32_rifsc.h>
#include <dt-bindings/soc/stm32mp25-rif.h>
#include <dt-bindings/soc/stm32mp25-rifsc.h>
#include <io.h>
#include <kernel/dt.h>
#include <kernel/boot.h>
#include <kernel/panic.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <tee_api_defines.h>
#include <trace.h>
#include <util.h>

/* RIFSC offset register */
#define _RIFSC_RISC_SECCFGR0		U(0x10)
#define _RIFSC_RISC_PRIVCFGR0		U(0x30)
#define _RIFSC_RISC_RCFGLOCKR0		U(0x50)
#define _RIFSC_RISC_PER0_CIDCFGR	U(0x100)
#define _RIFSC_RISC_PER0_SEMCR		U(0x104)
#define _RIFSC_RIMC_CR			U(0xC00)
#define _RIFSC_RIMC_ATTR0		U(0xC10)

#define _RIFSC_HWCFGR3			U(0xFE8)
#define _RIFSC_HWCFGR2			U(0xFEC)
#define _RIFSC_HWCFGR1			U(0xFF0)
#define _RIFSC_VERR			U(0xFF4)

/* RIFSC_HWCFGR2 register fields */
#define _RIFSC_HWCFGR2_CFG1_MASK	GENMASK_32(15, 0)
#define _RIFSC_HWCFGR2_CFG1_SHIFT	U(0)
#define _RIFSC_HWCFGR2_CFG2_MASK	GENMASK_32(23, 16)
#define _RIFSC_HWCFGR2_CFG2_SHIFT	U(16)
#define _RIFSC_HWCFGR2_CFG3_MASK	GENMASK_32(31, 24)
#define _RIFSC_HWCFGR2_CFG3_SHIFT	U(24)

/* RIFSC_HWCFGR1 register fields */
#define _RIFSC_HWCFGR1_CFG1_MASK	GENMASK_32(3, 0)
#define _RIFSC_HWCFGR1_CFG1_SHIFT	U(0)
#define _RIFSC_HWCFGR1_CFG2_MASK	GENMASK_32(7, 4)
#define _RIFSC_HWCFGR1_CFG2_SHIFT	U(4)
#define _RIFSC_HWCFGR1_CFG3_MASK	GENMASK_32(11, 8)
#define _RIFSC_HWCFGR1_CFG3_SHIFT	U(8)
#define _RIFSC_HWCFGR1_CFG4_MASK	GENMASK_32(15, 12)
#define _RIFSC_HWCFGR1_CFG4_SHIFT	U(12)
#define _RIFSC_HWCFGR1_CFG5_MASK	GENMASK_32(19, 16)
#define _RIFSC_HWCFGR1_CFG5_SHIFT	U(16)
#define _RIFSC_HWCFGR1_CFG6_MASK	GENMASK_32(23, 20)
#define _RIFSC_HWCFGR1_CFG6_SHIFT	U(20)

/*
 * RISC_CR register fields
 */
#define _RIFSC_RISC_CR_GLOCK		BIT(0)

/*
 * RIMC_CR register fields
 */
#define _RIFSC_RIMC_CR_GLOCK		BIT(0)
#define _RIFSC_RIMC_CR_TDCID_MASK	GENMASK_32(6, 4)

/* RIFSC_VERR register fields */
#define _RIFSC_VERR_MINREV_MASK		GENMASK_32(3, 0)
#define _RIFSC_VERR_MINREV_SHIFT	U(0)
#define _RIFSC_VERR_MAJREV_MASK		GENMASK_32(7, 4)
#define _RIFSC_VERR_MAJREV_SHIFT	U(4)

/* Periph id per register */
#define _PERIPH_IDS_PER_REG		U(32)
#define _OFST_PERX_CIDCFGR		U(0x8)

#define RIFSC_RISC_CFEN_MASK		BIT(0)
#define RIFSC_RISC_SEM_EN_MASK		BIT(1)
#define RIFSC_RISC_SCID_MASK		GENMASK_32(6, 4)
#define RIFSC_RISC_SEC_MASK		BIT(8)
#define RIFSC_RISC_PRIV_MASK		BIT(9)
#define RIFSC_RISC_LOCK_MASK		BIT(10)
#define RIFSC_RISC_SEML_MASK		GENMASK_32(23, 16)
#define RIFSC_RISC_PER_ID_MASK		GENMASK_32(31, 24)

#define RIFSC_RISC_PERx_CID_MASK	(RIFSC_RISC_CFEN_MASK | \
					 RIFSC_RISC_SEM_EN_MASK | \
					 RIFSC_RISC_SCID_MASK | \
					 RIFSC_RISC_SEML_MASK)

#define RIFSC_RIMC_MODE_MASK		BIT(2)
#define RIFSC_RIMC_MCID_MASK		GENMASK_32(6, 4)
#define RIFSC_RIMC_MSEC_MASK		BIT(8)
#define RIFSC_RIMC_MPRIV_MASK		BIT(9)
#define RIFSC_RIMC_M_ID_MASK		GENMASK_32(23, 16)

#define RIFSC_RIMC_ATTRx_MASK		(RIFSC_RIMC_MODE_MASK | \
					 RIFSC_RIMC_MCID_MASK | \
					 RIFSC_RIMC_MSEC_MASK | \
					 RIFSC_RIMC_MPRIV_MASK)

/* max entries */
#define MAX_RIMU			U(16)
#define MAX_RISUP			U(128)

#define _RIF_FLD_PREP(field, value)	(((uint32_t)(value) << (field ## _SHIFT)) & (field ## _MASK))
#define _RIF_FLD_GET(field, value)	(((uint32_t)(value) & (field ## _MASK)) >> (field ## _SHIFT))

struct rifsc_driver_data {
	uint32_t version;
	uint8_t nb_rimu;
	uint8_t nb_risup;
	uint8_t nb_risal;
	bool rif_en;
	bool sec_en;
	bool priv_en;
};

struct rifsc_platdata {
	uintptr_t base;
	struct rifsc_driver_data *drv_data;
	struct risup_cfg *risup;
	int nrisup;
	struct rimu_cfg *rimu;
	int nrimu;
};

struct dt_id_attr {
	/* The effective size of the array is meaningless here */
	fdt32_t id_attr[1];
};

static struct stm32_firewall_device *fdev;
static struct rifsc_driver_data rifsc_drvdata;
static struct rifsc_platdata rifsc_pdata;

static void stm32_rifsc_get_driverdata(struct rifsc_platdata *pdata)
{
	uint32_t regval = 0;

	regval = io_read32(pdata->base + _RIFSC_HWCFGR1);
	rifsc_drvdata.rif_en = _RIF_FLD_GET(_RIFSC_HWCFGR1_CFG1, regval) != 0;
	rifsc_drvdata.sec_en = _RIF_FLD_GET(_RIFSC_HWCFGR1_CFG2, regval) != 0;
	rifsc_drvdata.priv_en = _RIF_FLD_GET(_RIFSC_HWCFGR1_CFG3, regval) != 0;

	regval = io_read32(pdata->base + _RIFSC_HWCFGR2);
	rifsc_drvdata.nb_risup = _RIF_FLD_GET(_RIFSC_HWCFGR2_CFG1, regval);
	rifsc_drvdata.nb_rimu = _RIF_FLD_GET(_RIFSC_HWCFGR2_CFG2, regval);
	rifsc_drvdata.nb_risal = _RIF_FLD_GET(_RIFSC_HWCFGR2_CFG3, regval);

	pdata->drv_data = &rifsc_drvdata;

	regval = io_read8(pdata->base + _RIFSC_VERR);

	DMSG("RIFSC version %"PRIu32".%"PRIu32,
	     _RIF_FLD_GET(_RIFSC_VERR_MAJREV, regval),
	     _RIF_FLD_GET(_RIFSC_VERR_MINREV, regval));

	DMSG("HW cap: enabled[rif:sec:priv]:[%s:%s:%s] nb[risup|rimu|risal]:[%"PRIu8",%"PRIu8",%"PRIu8"]",
	     rifsc_drvdata.rif_en ? "true" : "false",
	     rifsc_drvdata.sec_en ? "true" : "false",
	     rifsc_drvdata.priv_en ? "true" : "false",
	     rifsc_drvdata.nb_risup,
	     rifsc_drvdata.nb_rimu,
	     rifsc_drvdata.nb_risal);
}

static TEE_Result stm32_rifsc_glock_config(const void *fdt, int node,
					   struct rifsc_platdata *pdata)
{
	const fdt32_t *cuint = NULL;
	int len = 0;
	uint32_t glock_conf = 0;

	cuint = fdt_getprop(fdt, node, "st,glocked", &len);
	if (!cuint) {
		DMSG("No global lock on RIF configuration");
		return TEE_SUCCESS;
	}

	assert(len == sizeof(uint32_t));

	glock_conf = fdt32_to_cpu(*cuint);

	if (glock_conf & RIFSC_RIMU_GLOCK) {
		DMSG("Setting global lock on RIMU configuration");

		io_setbits32(pdata->base + _RIFSC_RIMC_CR,
			     _RIFSC_RIMC_CR_GLOCK);

		if (!(io_read32(pdata->base + _RIFSC_RIMC_CR) &
		      _RIFSC_RIMC_CR_GLOCK))
			return TEE_ERROR_ACCESS_DENIED;
	}

	if (glock_conf & RIFSC_RISUP_GLOCK) {
		DMSG("Setting global lock on RISUP configuration");

		io_setbits32(pdata->base, _RIFSC_RISC_CR_GLOCK);

		if (!(io_read32(pdata->base) & _RIFSC_RISC_CR_GLOCK))
			return TEE_ERROR_ACCESS_DENIED;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_rifsc_dt_conf_risup(const void *fdt, int node,
					    int *nrisup,
					    struct risup_cfg **risups)
{
	const struct dt_id_attr *conf_list = NULL;
	int i = 0;
	int len = 0;

	conf_list = (const struct dt_id_attr *)fdt_getprop(fdt, node,
							   "st,protreg", &len);
	if (!conf_list) {
		IMSG("No RISUP configuration in DT");
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	*nrisup = len / sizeof(uint32_t);
	*risups = calloc(*nrisup, sizeof(**risups));
	if (!*risups)
		return TEE_ERROR_OUT_OF_MEMORY;

	for (i = 0; i < *nrisup; i++) {
		uint32_t value = fdt32_to_cpu(conf_list->id_attr[i]);
		struct risup_cfg *risup = *risups + i;

		risup->id = _RIF_FLD_GET(RIFSC_RISC_PER_ID, value);
		risup->sec = (bool)_RIF_FLD_GET(RIFSC_RISC_SEC, value);
		risup->priv = (bool)_RIF_FLD_GET(RIFSC_RISC_PRIV, value);
		risup->lock = (bool)_RIF_FLD_GET(RIFSC_RISC_LOCK, value);
		risup->cid_attr = _RIF_FLD_GET(RIFSC_RISC_PERx_CID, value);
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_rifsc_dt_conf_rimu(const void *fdt, int node,
					   struct rifsc_platdata *pdata)
{
	const struct dt_id_attr *conf_list = NULL;
	int i = 0;
	int len = 0;

	conf_list = (const struct dt_id_attr *)fdt_getprop(fdt, node,
							   "st,rimu", &len);
	if (!conf_list) {
		IMSG("No RIMU configuration in DT");
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	len = len / sizeof(uint32_t);

	pdata->nrimu = len;
	pdata->rimu = calloc(len, sizeof(*pdata->rimu));
	if (!pdata->rimu)
		return TEE_ERROR_OUT_OF_MEMORY;

	for (i = 0; i < len; i++) {
		uint32_t value = fdt32_to_cpu(conf_list->id_attr[i]);
		struct rimu_cfg *rimu = pdata->rimu + i;

		rimu->id = _RIF_FLD_GET(RIFSC_RIMC_M_ID, value);
		rimu->attr = _RIF_FLD_GET(RIFSC_RIMC_ATTRx, value);
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_rifsc_parse_fdt(const void *fdt, int node,
					struct rifsc_platdata *pdata)
{
	static struct io_pa_va base;
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t reg_size = 0;

	base.pa = _fdt_reg_base_address(fdt, node);
	if (base.pa == DT_INFO_INVALID_REG)
		return TEE_ERROR_BAD_PARAMETERS;

	reg_size = _fdt_reg_size(fdt, node);
	if (reg_size == DT_INFO_INVALID_REG_SIZE)
		return TEE_ERROR_BAD_PARAMETERS;

	pdata->base = io_pa_or_va_secure(&base, reg_size);

	res = stm32_rifsc_dt_conf_risup(fdt, node, &pdata->nrisup,
					&pdata->risup);
	if (res)
		return res;

	res = stm32_rifsc_dt_conf_rimu(fdt, node, pdata);
	if (res)
		return res;

	return TEE_SUCCESS;
}

static TEE_Result
stm32_rifsc_has_access(struct stm32_firewall_device *dev __unused,
		       unsigned int id,
		       paddr_t base __unused, size_t sz __unused,
		       const struct stm32_firewall_cfg *cfg)
{
	TEE_Result res = TEE_ERROR_ACCESS_DENIED;
	const struct stm32_firewall_cfg *f_cfg = cfg;
	unsigned int reg_id = id / 32;
	unsigned int periph_offset = id - (reg_id * 32);
	uintptr_t rifsc_base = rifsc_pdata.base;
	uint32_t sec_reg_value = 0;
	uint32_t priv_reg_value = 0;
	uint32_t cid_reg_value = 0;
	uint32_t mode = 0;

	DMSG("Check firewall access for peripheral (ID): %u", id);

	assert(rifsc_base);

	if (!f_cfg)
		return TEE_ERROR_BAD_PARAMETERS;

	for (f_cfg = cfg; f_cfg->access; f_cfg++) {
		/* The CID corresponds to the * FWLL_MASTER(ID) + 1 */
		unsigned int master = ((f_cfg->access & FWLL_MASTER_MASK) >>
				      FWLL_MASTER_SHIFT) + 1;

		if (master > MAX_CID_SUPPORTED) {
			EMSG("This CID does not exist: %u", master);
			panic();
		}

		mode = cfg->access & FWLL_CONF_MASK;

		if (mode == FWLL_NO_ACCESS)
			continue;

		sec_reg_value = io_read32(rifsc_base + _RIFSC_RISC_SECCFGR0 +
					  0x4 * reg_id);
		priv_reg_value = io_read32(rifsc_base + _RIFSC_RISC_PRIVCFGR0 +
					   0x4 * reg_id);
		cid_reg_value = io_read32(rifsc_base +
					  _RIFSC_RISC_PER0_CIDCFGR +
					  0x8 * id);

		if (!(mode & (FWLL_SEC_PRIV | FWLL_SEC_UNPRIV |
			      FWLL_NSEC_PRIV | FWLL_NSEC_UNPRIV)))
			return TEE_ERROR_BAD_PARAMETERS;

		/*
		 * First check conditions for semaphore mode, which doesn't
		 * take into account static CID.
		 */
		if (cid_reg_value & _CIDCFGR_SEMEN) {
			if (cid_reg_value & BIT(master + SEMWL_SHIFT)) {
				/* Static CID is irrelevant if semaphore mode */
				goto skip_cid_check;
			} else {
				return TEE_ERROR_ACCESS_DENIED;
			}
		}

		if (!(cid_reg_value & _CIDCFGR_CFEN) ||
		    ((cid_reg_value & RIFSC_RISC_SCID_MASK) >>
		    RIFSC_RISC_SCID_SHIFT) == RIF_CID0)
			goto skip_cid_check;

		/*
		 * Coherency check with the CID configuration
		 */
		if (((cid_reg_value & RIFSC_RISC_SCID_MASK) >>
		    RIFSC_RISC_SCID_SHIFT) != master)
			return TEE_ERROR_ACCESS_DENIED;

skip_cid_check:
		/* Check non-secure, non-privilege configuration */
		if (mode & (FWLL_NSEC_UNPRIV) &&
		    (sec_reg_value & BIT(periph_offset) ||
		     priv_reg_value & BIT(periph_offset)))
			return TEE_ERROR_ACCESS_DENIED;

		/* Check secure configuration */
		if ((mode & (FWLL_SEC_PRIV) || mode & (FWLL_SEC_UNPRIV)) &&
		    !(sec_reg_value & BIT(periph_offset)))
			return TEE_ERROR_ACCESS_DENIED;

		/* Check unprivilege configuration */
		if ((priv_reg_value & BIT(periph_offset)) &&
		    (!(mode & FWLL_NSEC_PRIV)) && (!(mode & FWLL_SEC_PRIV)))
			return TEE_ERROR_ACCESS_DENIED;

		res = TEE_SUCCESS;
	}

	return res;
}

static TEE_Result stm32_risup_cfg(struct rifsc_platdata *pdata,
				  struct risup_cfg *risup)
{
	struct rifsc_driver_data *drv_data = pdata->drv_data;
	uintptr_t cidcfgr_offset = _OFST_PERX_CIDCFGR * risup->id;
	uintptr_t offset = sizeof(uint32_t) * (risup->id / _PERIPH_IDS_PER_REG);
	uint32_t shift = risup->id % _PERIPH_IDS_PER_REG;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!risup || risup->id >= drv_data->nb_risup)
		return TEE_ERROR_BAD_PARAMETERS;

	if (drv_data->sec_en)
		io_clrsetbits32(pdata->base + _RIFSC_RISC_SECCFGR0 + offset,
				BIT(shift), risup->sec << shift);

	if (drv_data->priv_en)
		io_clrsetbits32(pdata->base + _RIFSC_RISC_PRIVCFGR0 + offset,
				BIT(shift), risup->priv << shift);

	if (drv_data->rif_en)
		io_write32(pdata->base + _RIFSC_RISC_PER0_CIDCFGR +
			   cidcfgr_offset, risup->cid_attr);

	/* Lock configuration for this RISUP */
	if (risup->lock) {
		DMSG("Locking RIF conf for peripheral n°%"PRIu32, risup->id);
		io_setbits32(pdata->base + _RIFSC_RISC_RCFGLOCKR0 + offset,
			     BIT(shift));
	}

	/*
	 * Take semaphore if the resource is in semaphore mode
	 * and secured.
	 */
	if (SEM_MODE_INCORRECT(risup->cid_attr) ||
	    !(io_read32(pdata->base + _RIFSC_RISC_SECCFGR0 + offset) &
	      BIT(shift))) {
		res = stm32_rif_release_semaphore(pdata->base +
						  _RIFSC_RISC_PER0_SEMCR +
						  cidcfgr_offset,
						  MAX_CID_SUPPORTED);
		if (res) {
			EMSG("Couldn't release semaphore for resource %u",
			     risup->id);
			return TEE_ERROR_ACCESS_DENIED;
		}
	} else {
		res = stm32_rif_acquire_semaphore(pdata->base +
						  _RIFSC_RISC_PER0_SEMCR +
						  cidcfgr_offset,
						  MAX_CID_SUPPORTED);
		if (res) {
			EMSG("Couldn't acquire semaphore for resource %u",
			     risup->id);
			return TEE_ERROR_ACCESS_DENIED;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_risup_setup(struct rifsc_platdata *pdata)
{
	struct rifsc_driver_data *drv_data = pdata->drv_data;
	int i = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	for (i = 0; i < pdata->nrisup && i < drv_data->nb_risup; i++) {
		struct risup_cfg *risup = pdata->risup + i;

		res = stm32_risup_cfg(pdata, risup);
		if (res) {
			EMSG("risup cfg(%d/%d) error", i + 1, pdata->nrisup);
			return res;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_rimu_cfg(struct rifsc_platdata *pdata,
				 struct rimu_cfg *rimu)
{
	struct rifsc_driver_data *drv_data = pdata->drv_data;
	uintptr_t offset =  _RIFSC_RIMC_ATTR0 + (sizeof(uint32_t) * rimu->id);

	if (!rimu || rimu->id >= drv_data->nb_rimu)
		return TEE_ERROR_BAD_PARAMETERS;

	if (drv_data->rif_en)
		io_write32(pdata->base + offset, rimu->attr);

	return TEE_SUCCESS;
}

static TEE_Result stm32_rimu_setup(struct rifsc_platdata *pdata)
{
	struct rifsc_driver_data *drv_data = pdata->drv_data;
	int i = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	for (i = 0; i < pdata->nrimu && i < drv_data->nb_rimu; i++) {
		struct rimu_cfg *rimu = pdata->rimu + i;

		res = stm32_rimu_cfg(pdata, rimu);
		if (res) {
			EMSG("rimu cfg(%d/%d) error", i + 1, pdata->nrimu);
			return res;
		}
	}

	return 0;
}

TEE_Result stm32_rifsc_reconfigure_risup(unsigned int risup_id,
					 unsigned int cid,
					 bool sec, bool priv, bool cfen)
{
	unsigned int offset = sizeof(uint32_t) *
			      (risup_id / _PERIPH_IDS_PER_REG);
	TEE_Result res = TEE_ERROR_GENERIC;
	struct risup_cfg *risup = NULL;
	int i = 0;

	if (risup_id > rifsc_pdata.drv_data->nb_risup ||
	    cid > MAX_CID_SUPPORTED)
		return TEE_ERROR_BAD_PARAMETERS;

	for (i = 0; i < rifsc_pdata.nrisup; i++) {
		if (rifsc_pdata.risup[i].id == risup_id)
			risup = &rifsc_pdata.risup[i];
	}
	if (!risup)
		return TEE_ERROR_BAD_PARAMETERS;

	if (io_read32(rifsc_pdata.base + _RIFSC_RISC_RCFGLOCKR0 + offset) &
	    BIT(risup_id % _PERIPH_IDS_PER_REG)) {
		DMSG("RIMU configuration is locked");
		return TEE_ERROR_ACCESS_DENIED;
	}

	risup->cid_attr = cid << RIFSC_RISC_SCID_SHIFT;
	if (cfen)
		risup->cid_attr |= RIFSC_RISC_CFEN_MASK;
	else
		risup->cid_attr &= ~RIFSC_RISC_CFEN_MASK;

	risup->sec = sec;
	risup->priv = priv;

	res = stm32_risup_cfg(&rifsc_pdata, risup);
	if (res) {
		EMSG("RISUP %u reconfiguration error", risup_id);
		return res;
	}

	return TEE_SUCCESS;
}

bool stm32_rifsc_cid_is_enabled(unsigned int  rifc_id)
{
	struct io_pa_va rifsc_addr = { .pa = RIFSC_BASE };
	vaddr_t rifsc_base = io_pa_or_va(&rifsc_addr, 1);
	uint32_t cidcfgr = io_read32(rifsc_base + _RIFSC_RISC_PER0_CIDCFGR +
				     _OFST_PERX_CIDCFGR * rifc_id);

	return (cidcfgr & _CIDCFGR_CFEN) == _CIDCFGR_CFEN;
}

TEE_Result stm32_rifsc_cid_enable(unsigned int  rifc_id)
{
	struct io_pa_va rifsc_addr = { .pa = RIFSC_BASE };
	vaddr_t rifsc_base = io_pa_or_va(&rifsc_addr, 1);

	io_setbits32(rifsc_base + _RIFSC_RISC_PER0_CIDCFGR +
		     _OFST_PERX_CIDCFGR * rifc_id, _CIDCFGR_CFEN);

	return TEE_SUCCESS;
}

TEE_Result stm32_rifsc_cid_disable(unsigned int  rifc_id)
{
	struct io_pa_va rifsc_addr = { .pa = RIFSC_BASE };
	vaddr_t rifsc_base = io_pa_or_va(&rifsc_addr, 1);

	io_clrbits32(rifsc_base + _RIFSC_RISC_PER0_CIDCFGR +
		     _OFST_PERX_CIDCFGR * rifc_id, _CIDCFGR_CFEN);

	return TEE_SUCCESS;
}

TEE_Result stm32_rifsc_reconfigure_rimu(unsigned int id,
					unsigned int master_cid,
					bool cid_sel, bool sec, bool priv)
{
	struct rimu_cfg *rimu = NULL;
	uint32_t rimu_conf = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (id > rifsc_pdata.drv_data->nb_rimu ||
	    master_cid > MAX_CID_SUPPORTED)
		return TEE_ERROR_BAD_PARAMETERS;

	rimu = &rifsc_pdata.rimu[id];

	if (io_read32(rifsc_pdata.base + _RIFSC_RIMC_CR) &
	    _RIFSC_RIMC_CR_GLOCK) {
		DMSG("RIMU configuration is locked");
		return TEE_ERROR_ACCESS_DENIED;
	}

	if (cid_sel)
		rimu_conf |= RIFSC_RIMC_MODE_MASK;
	if (sec)
		rimu_conf |= RIFSC_RIMC_MSEC_MASK;
	if (priv)
		rimu_conf |= RIFSC_RIMC_MPRIV_MASK;

	rimu_conf |= master_cid << RIFSC_RIMC_MCID_SHIFT;

	rimu->attr = rimu_conf;

	res = stm32_rimu_cfg(&rifsc_pdata, rimu);
	if (res) {
		EMSG("RIMU %u reconfiguration error", id);
		return res;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_rifsc_sem_pm_suspend(void)
{
	int i = 0;

	for (i = 0; i < rifsc_pdata.nrisup && i < rifsc_drvdata.nb_risup; i++) {
		uint32_t semcfgr = io_read32(rifsc_pdata.base +
					     _RIFSC_RISC_PER0_SEMCR +
					     _OFST_PERX_CIDCFGR * i);
		struct risup_cfg *risup = rifsc_pdata.risup + i;

		/* Save semaphores that were taken by the CID1 */
		risup->pm_sem = semcfgr & _SEMCR_MUTEX &&
				((semcfgr & _SEMCR_SEMCID_MASK) >>
				 _SEMCR_SEMCID_SHIFT == RIF_CID1) ?
				true : false;

		FMSG("RIF semaphore %s for ID: %u",
		     risup->pm_sem ? "SAVED" : "NOT SAVED", risup->id);
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_rifsc_sem_pm_resume(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int i = 0;

	for (i = 0; i < rifsc_pdata.nrisup && i < rifsc_drvdata.nb_risup; i++) {
		struct risup_cfg *risup = rifsc_pdata.risup + i;
		uintptr_t cidcfgr_offset = _OFST_PERX_CIDCFGR * risup->id;
		uintptr_t offset = sizeof(uint32_t) *
				   (risup->id / _PERIPH_IDS_PER_REG);
		uintptr_t perih_offset = risup->id % _PERIPH_IDS_PER_REG;
		uint32_t seccgfr = io_read32(rifsc_pdata.base +
					     _RIFSC_RISC_SECCFGR0 + offset);
		uint32_t privcgfr = io_read32(rifsc_pdata.base +
					      _RIFSC_RISC_PRIVCFGR0 + offset);
		uint32_t lockcfgr = io_read32(rifsc_pdata.base +
					      _RIFSC_RISC_RCFGLOCKR0 + offset);

		/* Update RISUPs fields */
		risup->cid_attr = io_read32(rifsc_pdata.base +
					    _RIFSC_RISC_PER0_CIDCFGR +
					    cidcfgr_offset);
		risup->sec = (bool)(seccgfr & BIT(perih_offset));
		risup->priv = (bool)(privcgfr & BIT(perih_offset));
		risup->lock = (bool)(lockcfgr & BIT(perih_offset));

		/* Acquire available appropriate semaphores */
		if (SEM_MODE_INCORRECT(risup->cid_attr) || !risup->pm_sem)
			continue;

		res = stm32_rif_acquire_semaphore(rifsc_pdata.base +
						  _RIFSC_RISC_PER0_SEMCR +
						  cidcfgr_offset,
						  MAX_CID_SUPPORTED);
		if (res) {
			EMSG("Could not acquire semaphore for resource %u",
			     risup->id);
			return TEE_ERROR_ACCESS_DENIED;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result
stm32_rifsc_sem_pm(enum pm_op op, unsigned int pm_hint,
		   const struct pm_callback_handle *pm_handle __unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!PM_HINT_IS_STATE(pm_hint, CONTEXT))
		return TEE_SUCCESS;

	if (op == PM_OP_RESUME)
		res = stm32_rifsc_sem_pm_resume();
	else
		res = stm32_rifsc_sem_pm_suspend();

	return res;
}

TEE_Result stm32_rifsc_check_tdcid(bool *tdcid_state)
{
	if (!rifsc_pdata.base)
		return TEE_ERROR_DEFER_DRIVER_INIT;

	*tdcid_state = false;

	if (((io_read32(rifsc_pdata.base + _RIFSC_RIMC_CR) &
	     _RIFSC_RIMC_CR_TDCID_MASK)) == (RIF_CID1 << SCID_SHIFT))
		*tdcid_state = true;

	return TEE_SUCCESS;
}

static TEE_Result
stm32_rifsc_get_data_config(const void *fdt, int node,
			    int *ndata, void **data_cfg)
{
	if (stm32_rifsc_dt_conf_risup(fdt, node, ndata,
				      (struct risup_cfg **)data_cfg))
		return TEE_ERROR_ITEM_NOT_FOUND;

	return TEE_SUCCESS;
}

static TEE_Result
stm32_rifsc_set_data_config(int ndata, void *data_cfg)
{
	struct rifsc_platdata *pdata = &rifsc_pdata;
	struct rifsc_driver_data *drv_data = pdata->drv_data;
	int i = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	for (i = 0; i < ndata && i < drv_data->nb_risup; i++) {
		struct risup_cfg *risup = (struct risup_cfg *)data_cfg + i;

		res = stm32_risup_cfg(pdata, risup);
		if (res) {
			EMSG("risup cfg(%d/%d) error", i + 1, ndata);
			return res;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result
stm32_rifsc_put_data_config(int ndata __unused, void *data_cfg)
{
	free(data_cfg);

	return TEE_SUCCESS;
}

static const struct stm32_firewall_ops stm32_rifsc_fw_ops = {
	.has_access = stm32_rifsc_has_access,
	.get_data_config = stm32_rifsc_get_data_config,
	.set_data_config = stm32_rifsc_set_data_config,
	.put_data_config = stm32_rifsc_put_data_config,
};

static TEE_Result stm32_rifsc_probe(const void *fdt, int node,
				    const void *compat_data)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	res = stm32_rifsc_parse_fdt(fdt, node, &rifsc_pdata);
	if (res)
		return res;

	if (!rifsc_pdata.drv_data)
		stm32_rifsc_get_driverdata(&rifsc_pdata);

	res = stm32_risup_setup(&rifsc_pdata);
	if (res)
		return res;

	res = stm32_rimu_setup(&rifsc_pdata);
	if (res)
		return res;

	stm32_rifsc_glock_config(fdt, node, &rifsc_pdata);

	fdev = stm32_firewall_dev_alloc();
	if (!fdev)
		return TEE_ERROR_OUT_OF_MEMORY;

	fdev->name = "rifsc";
	fdev->ops = &stm32_rifsc_fw_ops;
	fdev->compat = (struct stm32_firewall_compat *)compat_data;
	fdev->priv = (void *)&rifsc_pdata;

	res = stm32_firewall_dev_register(fdev);
	if (res)
		goto out;

	stm32_firewall_bus_probe(fdev, fdt, node);

	if (IS_ENABLED(CFG_PM))
		register_pm_core_service_cb(stm32_rifsc_sem_pm, NULL,
					    "stm32-rifsc-semaphores");

out:
	if (res)
		free(fdev);

	return res;
}

static struct stm32_firewall_reg
rifsc_compat_mp25[STM32MP25_RIFSC_IAC_ID + 1] = {
	[STM32MP25_RIFSC_TIM1_ID] = { 0x40200000 },
	[STM32MP25_RIFSC_TIM2_ID] = { 0x40000000 },
	[STM32MP25_RIFSC_TIM3_ID] = { 0x40010000 },
	[STM32MP25_RIFSC_TIM4_ID] = { 0x40020000 },
	[STM32MP25_RIFSC_TIM5_ID] = { 0x40030000 },
	[STM32MP25_RIFSC_TIM6_ID] = { 0x40040000 },
	[STM32MP25_RIFSC_TIM7_ID] = { 0x40050000 },
	[STM32MP25_RIFSC_TIM8_ID] = { 0x40210000 },
	[STM32MP25_RIFSC_TIM10_ID] = { 0x401C0000 },
	[STM32MP25_RIFSC_TIM11_ID] = { 0x401D0000 },
	[STM32MP25_RIFSC_TIM12_ID] = { 0x40060000 },
	[STM32MP25_RIFSC_TIM13_ID] = { 0x40070000 },
	[STM32MP25_RIFSC_TIM14_ID] = { 0x40080000 },
	[STM32MP25_RIFSC_TIM15_ID] = { 0x40250000 },
	[STM32MP25_RIFSC_TIM16_ID] = { 0x40260000 },
	[STM32MP25_RIFSC_TIM17_ID] = { 0x40270000 },
	[STM32MP25_RIFSC_TIM20_ID] = { 0x40320000 },
	[STM32MP25_RIFSC_LPTIM1_ID] = { 0x40090000 },
	[STM32MP25_RIFSC_LPTIM2_ID] = { 0x400A0000 },
	[STM32MP25_RIFSC_LPTIM3_ID] = { 0x46050000 },
	[STM32MP25_RIFSC_LPTIM4_ID] = { 0x46060000 },
	[STM32MP25_RIFSC_LPTIM5_ID] = { 0x46070000 },
	[STM32MP25_RIFSC_SPI1_ID] = { 0x40230000 },
	[STM32MP25_RIFSC_SPI2_ID] = { 0x400B0000 },
	[STM32MP25_RIFSC_SPI3_ID] = { 0x400C0000 },
	[STM32MP25_RIFSC_SPI4_ID] = { 0x40240000 },
	[STM32MP25_RIFSC_SPI5_ID] = { 0x40280000 },
	[STM32MP25_RIFSC_SPI6_ID] = { 0x40350000 },
	[STM32MP25_RIFSC_SPI7_ID] = { 0x40360000 },
	[STM32MP25_RIFSC_SPI8_ID] = { 0x46020000 },
	[STM32MP25_RIFSC_SPDIFRX_ID] = { 0x400D0000 },
	[STM32MP25_RIFSC_USART1_ID] = { 0x40330000 },
	[STM32MP25_RIFSC_USART2_ID] = { 0x400E0000 },
	[STM32MP25_RIFSC_USART3_ID] = { 0x400F0000 },
	[STM32MP25_RIFSC_UART4_ID] = { 0x40100000 },
	[STM32MP25_RIFSC_UART5_ID] = { 0x40110000 },
	[STM32MP25_RIFSC_USART6_ID] = { 0x40220000 },
	[STM32MP25_RIFSC_UART7_ID] = { 0x40370000 },
	[STM32MP25_RIFSC_UART8_ID] = { 0x40380000 },
	[STM32MP25_RIFSC_UART9_ID] = { 0x402C0000 },
	[STM32MP25_RIFSC_LPUART1_ID] = { 0x46030000 },
	[STM32MP25_RIFSC_I2C1_ID] = { 0x40120000 },
	[STM32MP25_RIFSC_I2C2_ID] = { 0x40130000 },
	[STM32MP25_RIFSC_I2C3_ID] = { 0x40140000 },
	[STM32MP25_RIFSC_I2C4_ID] = { 0x40150000 },
	[STM32MP25_RIFSC_I2C5_ID] = { 0x40160000 },
	[STM32MP25_RIFSC_I2C6_ID] = { 0x40170000 },
	[STM32MP25_RIFSC_I2C7_ID] = { 0x40180000 },
	[STM32MP25_RIFSC_I2C8_ID] = { 0x46040000 },
	[STM32MP25_RIFSC_SAI1_ID] = { 0x40290000 },
	[STM32MP25_RIFSC_SAI2_ID] = { 0x402A0000 },
	[STM32MP25_RIFSC_SAI3_ID] = { 0x402B0000 },
	[STM32MP25_RIFSC_SAI4_ID] = { 0x40340000},
	[STM32MP25_RIFSC_MDF1_ID] = { 0x404D0000 },
	[STM32MP25_RIFSC_ADF1_ID] = { 0x46220000 },
	[STM32MP25_RIFSC_FDCAN_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_HDP_ID] = { 0x44090000 },
	[STM32MP25_RIFSC_ADC12_ID] = { 0x404E0000 },
	[STM32MP25_RIFSC_ADC3_ID] = { 0x404F0000 },
	[STM32MP25_RIFSC_ETH1_ID] = { 0x482C0000 },
	[STM32MP25_RIFSC_ETH2_ID] = { 0x482D0000 },
	[STM32MP25_RIFSC_USBH_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_USB3DR_ID] = { 0x48300000 },
	[STM32MP25_RIFSC_COMBOPHY_ID] = { 0x480C0000 },
	[STM32MP25_RIFSC_PCIE_ID] = { 0x48400000 },
	[STM32MP25_RIFSC_UCPD1_ID] = { 0x480A0000 },
	[STM32MP25_RIFSC_ETHSW_DEIP_ID] = { 0x4C000000 },
	[STM32MP25_RIFSC_ETHSW_ACM_CFG_ID] = { 0x4B080000 },
	[STM32MP25_RIFSC_ETHSW_ACM_MSGBUF_ID] = { 0x4B000000 },
	[STM32MP25_RIFSC_STGEN_ID] = { 0x48080000 },
	[STM32MP25_RIFSC_OCTOSPI1_ID] = { 0x40430000 },
	[STM32MP25_RIFSC_OCTOSPI2_ID] = { 0x40440000 },
	[STM32MP25_RIFSC_SDMMC1_ID] = { 0x48220000 },
	[STM32MP25_RIFSC_SDMMC2_ID] = { 0x48230000 },
	[STM32MP25_RIFSC_SDMMC3_ID] = { 0x48240000 },
	[STM32MP25_RIFSC_GPU_ID] = { 0x48280000 },
	[STM32MP25_RIFSC_LTDC_CMN_ID] = { 0x48010000 },
	[STM32MP25_RIFSC_DSI_CMN_ID] = { 0x48000000 },
	[STM32MP25_RIFSC_LVDS_ID] = { 0x48060000 },
	[STM32MP25_RIFSC_CSI_ID] = { 0x48020000 },
	[STM32MP25_RIFSC_DCMIPP_ID] = { 0x48030000 },
	[STM32MP25_RIFSC_DCMI_PSSI_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_VDEC_ID] = { 0x480D0000 },
	[STM32MP25_RIFSC_VENC_ID] = { 0x480E0000 },
	[STM32MP25_RIFSC_RNG_ID] = { 0x42020000 },
	[STM32MP25_RIFSC_PKA_ID] = { 0x42060000 },
	[STM32MP25_RIFSC_SAES_ID] = { 0x42050000 },
	[STM32MP25_RIFSC_HASH_ID] = { 0x42010000 },
	[STM32MP25_RIFSC_CRYP1_ID] = { 0x42030000 },
	[STM32MP25_RIFSC_CRYP2_ID] = { 0x42040000 },
	[STM32MP25_RIFSC_IWDG1_ID] = { 0x44010000 },
	[STM32MP25_RIFSC_IWDG2_ID] = { 0x44020000 },
	[STM32MP25_RIFSC_IWDG3_ID] = { 0x44030000 },
	[STM32MP25_RIFSC_IWDG4_ID] = { 0x44040000 },
	[STM32MP25_RIFSC_IWDG5_ID] = { 0x46090000 },
	[STM32MP25_RIFSC_WWDG1_ID] = { 0x44050000 },
	[STM32MP25_RIFSC_WWDG2_ID] = { 0x460A0000 },
	[STM32MP25_RIFSC_VREFBUF_ID] = { 0x44060000 },
	[STM32MP25_RIFSC_DTS_ID] = { 0x44070000 },
	[STM32MP25_RIFSC_RAMCFG_ID] = { 0x42070000 },
	[STM32MP25_RIFSC_CRC_ID] = { 0x404C0000 },
	[STM32MP25_RIFSC_SERC_ID] = { 0x44080000 },
	[STM32MP25_RIFSC_OCTOSPIM_ID] = { 0x40500000 },
	[STM32MP25_RIFSC_GICV2M_ID] = { 0x48090000 },
	[STM32MP25_RIFSC_I3C1_ID] = { 0x40190000 },
	[STM32MP25_RIFSC_I3C2_ID] = { 0x401A0000 },
	[STM32MP25_RIFSC_I3C3_ID] = { 0x401B0000 },
	[STM32MP25_RIFSC_I3C4_ID] = { 0x46080000 },
	[STM32MP25_RIFSC_ICACHE_DCACHE_ID] = { 0x40470000 },
	[STM32MP25_RIFSC_LTDC_L0L1_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_LTDC_L2_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_LTDC_ROT_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_DSI_TRIG_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_DSI_RDFIFO_ID] = { 0xFFFFFFFF },
	[STM32MP25_RIFSC_OTFDEC1_ID] = { 0x40450000 },
	[STM32MP25_RIFSC_OTFDEC2_ID] = { 0x40460000 },
	[STM32MP25_RIFSC_IAC_ID] = { 0x42090000 },
};

static const struct stm32_firewall_compat rifsc_compat[] = {
	{
		.reg = rifsc_compat_mp25,
		.compat_size = ARRAY_SIZE(rifsc_compat_mp25)
	},
};

static const struct dt_device_match rifsc_match_table[] = {
	{
		.compatible = "st,stm32mp25-rifsc",
		.compat_data = (void *)rifsc_compat,
	},
	{ }
};

DEFINE_DT_DRIVER(rifsc_dt_driver) = {
	.name = "stm32-rifsc",
	.match_table = rifsc_match_table,
	.probe = stm32_rifsc_probe,
};
