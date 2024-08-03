// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2023, STMicroelectronics - All Rights Reserved
 */

#include <assert.h>
#include <drivers/nvmem.h>
#include <dt-bindings/soc/stm32mp25-rif.h>
#include <initcall.h>
#include <io.h>
#include <kernel/dt.h>
#include <kernel/interrupt.h>
#include <libfdt.h>
#include <malloc.h>
#include <mm/core_mmu.h>
#include <mm/core_memprot.h>
#include <string.h>
#include <tee_api_defines.h>
#include <tee_api_types.h>
#include <trace.h>
#include <types_ext.h>
#include <util.h>

#define CURRENT_CID RIF_CID1
#define NB_ZONES_STM32MP1 U(3)
#define NB_ZONES_STM32MP2 U(7)

#define _TAMP_SECCFGR U(0x20)
#define _TAMP_BKPRIFR(x) (U(0x70) + U(0x4) * ((x) - (U(0x1))))
#define _TAMP_RXCIDCFGR(x) (U(0x80) + U(0x4) * (x))

#define BKPREG_ZONE_1 U(0)
#define BKPREG_ZONE_2 U(1)
#define BKPREG_ZONE_3 U(2)

#define BKPREG_ZONE_1_RIF1 U(0)
#define BKPREG_ZONE_1_RIF2 U(1)
#define BKPREG_ZONE_2_RIF1 U(2)
#define BKPREG_ZONE_2_RIF2 U(3)
#define BKPREG_ZONE_3_RIF1 U(4)
#define BKPREG_ZONE_3_RIF0 U(5)
#define BKPREG_ZONE_3_RIF2 U(6)
#define NB_CMPT_MP2 U(3)

struct reg_field {
	uint16_t reg_offset;
	uint8_t lowest_bit;
	uint8_t highest_bit;
};

#define REG_FIELD(offset, l, h)                                         \
	{                                                               \
		.reg_offset = offset, .lowest_bit = l, .highest_bit = h \
	}

enum stm32_tamp_bkpreg_access {
	RONS,
	RNSWNS,
	RNSWS,
	RSWS,
	NORNOW,
};

struct stm32_tamp_nvram_drv_data {
	vaddr_t base;
	vaddr_t tamp_base;
	size_t size;
	const void *fdt;
	int node;
	int nb_total_regs;
	int *bkpreg_zones_end;
	enum stm32_tamp_bkpreg_access *bkpreg_access;
	bool *compartment_owner;
	const struct stm32_tamp_nvram_cdata *cdata;
};

struct stm32_tamp_nvram_cdata {
	const int nb_zones;
	const struct reg_field *zone_cfg;
};

static const struct reg_field mp1_zone_cfg_fields[NB_ZONES_STM32MP1 - 1] = {
	[BKPREG_ZONE_1] = REG_FIELD(_TAMP_SECCFGR, 0, 7),
	[BKPREG_ZONE_2] = REG_FIELD(_TAMP_SECCFGR, 16, 23),
};

static const struct reg_field mp25_zone_cfg_fields[NB_ZONES_STM32MP2 - 1] = {
	[BKPREG_ZONE_1_RIF1] = REG_FIELD(_TAMP_BKPRIFR(1), 0, 7),
	[BKPREG_ZONE_1_RIF2] = REG_FIELD(_TAMP_SECCFGR, 0, 7),
	[BKPREG_ZONE_2_RIF1] = REG_FIELD(_TAMP_BKPRIFR(2), 0, 7),
	[BKPREG_ZONE_2_RIF2] = REG_FIELD(_TAMP_SECCFGR, 16, 23),
	[BKPREG_ZONE_3_RIF1] = REG_FIELD(_TAMP_BKPRIFR(3), 0, 7),
	[BKPREG_ZONE_3_RIF0] = REG_FIELD(_TAMP_BKPRIFR(3), 16, 23),
};

static const struct reg_field mp25_rxcidcfg_cfen_fields[NB_CMPT_MP2] = {
	REG_FIELD(_TAMP_RXCIDCFGR(0), 0, 0),
	REG_FIELD(_TAMP_RXCIDCFGR(1), 0, 0),
	REG_FIELD(_TAMP_RXCIDCFGR(2), 0, 0),
};

static const struct reg_field mp25_rxcidcfg_fields[NB_CMPT_MP2] = {
	REG_FIELD(_TAMP_RXCIDCFGR(0), 4, 6),
	REG_FIELD(_TAMP_RXCIDCFGR(1), 4, 6),
	REG_FIELD(_TAMP_RXCIDCFGR(2), 4, 6),
};

static enum stm32_tamp_bkpreg_access mp1_bkpreg_access[NB_ZONES_STM32MP1] = {
	[BKPREG_ZONE_1] = RSWS,
	[BKPREG_ZONE_2] = RNSWS,
	[BKPREG_ZONE_3] = RNSWNS,
};

static const struct stm32_tamp_nvram_cdata stm32mp1_tamp_nvram = {
	.nb_zones = NB_ZONES_STM32MP1,
	.zone_cfg = mp1_zone_cfg_fields,
};

static const struct stm32_tamp_nvram_cdata stm32mp25_tamp_nvram = {
	.nb_zones = NB_ZONES_STM32MP2,
	.zone_cfg = mp25_zone_cfg_fields,
};

static TEE_Result
stm32_tamp_nvram_reg_get_zone(struct stm32_tamp_nvram_drv_data *drv_data,
			      int reg, int *zone)
{
	int *bkpreg_zones_end = drv_data->bkpreg_zones_end;
	int nb_zones = drv_data->cdata->nb_zones;
	int protection_zone_idx = 0;

	if (reg < 0)
		return TEE_ERROR_BAD_PARAMETERS;

	/*
	 * Zones are ordered in bkpreg_zones_end
	 * So the register is part of the first zone that frontier
	 * is greater or equal than the reg index.
	 */
	for (protection_zone_idx = 0; protection_zone_idx < nb_zones;
	     protection_zone_idx++) {
		if (reg <= bkpreg_zones_end[protection_zone_idx]) {
			*zone = protection_zone_idx;
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_ITEM_NOT_FOUND;
}

static TEE_Result
stm32_tamp_nvram_read_bkp(struct stm32_tamp_nvram_drv_data *drv_data,
			  size_t offset, uint32_t *val)
{
	unsigned int reg = offset / sizeof(uint32_t);
	int protection_zone_idx = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;

	ret = stm32_tamp_nvram_reg_get_zone(drv_data, reg,
					    &protection_zone_idx);
	if (ret != TEE_SUCCESS)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (drv_data->bkpreg_access[protection_zone_idx]) {
	case RNSWNS:
	case RNSWS:
	case RONS:
	case RSWS:
		*val = io_read32(drv_data->base + offset);
		break;
	case NORNOW:
		/* Intentionally output 0 and return with success */
		*val = 0x0;
		break;
	default:
		EMSG("Can't get access rights for the zone");
		panic();
	}
	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_nvram_write_bkp(struct stm32_tamp_nvram_drv_data *drv_data,
			   size_t offset, uint32_t val)
{
	unsigned int reg = offset / sizeof(uint32_t);
	int protection_zone_idx = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;

	ret = stm32_tamp_nvram_reg_get_zone(drv_data, reg,
					    &protection_zone_idx);

	if (ret != TEE_SUCCESS)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (drv_data->bkpreg_access[protection_zone_idx]) {
	case RNSWNS:
	case RNSWS:
	case RSWS:
		io_write32(drv_data->base + offset, val);
		break;
	case RONS:
	case NORNOW:
		/* Intentionally discard request and return with success */
		break;
	default:
		EMSG("Can't get access rights for the zone");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_SUCCESS;
}

static TEE_Result reg_field_read(vaddr_t base, struct reg_field field,
				 uint32_t *val)
{
	uint32_t reg_val = 0;

	reg_val = io_read32(base + field.reg_offset);
	reg_val &= GENMASK_32(field.highest_bit, field.lowest_bit);
	reg_val >>= field.lowest_bit;
	*val = reg_val;
	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_nvram_is_isolation_en_mp2(struct stm32_tamp_nvram_drv_data *drv_data,
				     bool *isolation_enabled)
{
	unsigned int i = 0;
	int nb_compartment_enabled = 0;
	uint32_t cfen = 0;
	const struct reg_field *cfen_field = NULL;
	TEE_Result ret = TEE_ERROR_GENERIC;

	for (i = 0; i < NB_CMPT_MP2; i++) {
		cfen_field = &mp25_rxcidcfg_cfen_fields[i];
		ret = reg_field_read(drv_data->tamp_base, *cfen_field, &cfen);

		if (ret != TEE_SUCCESS) {
			EMSG("Can't read field for registers zones");
			return ret;
		}
		nb_compartment_enabled += cfen;
	}

	if (!nb_compartment_enabled)
		*isolation_enabled = false;
	else if (nb_compartment_enabled == NB_CMPT_MP2)
		*isolation_enabled = true;
	else
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_get_cmpt_owner_mp2(struct stm32_tamp_nvram_drv_data *drv_data,
			      bool **cmpt_owner)
{
	struct reg_field cid_field = { 0 };
	uint32_t cid_per_zone = 0;
	bool isolation_enabled = false;
	TEE_Result ret = TEE_ERROR_GENERIC;
	unsigned int i = 0;

	ret = stm32_tamp_nvram_is_isolation_en_mp2(drv_data,
						   &isolation_enabled);
	if (ret != TEE_SUCCESS)
		return ret;

	*cmpt_owner = calloc(NB_CMPT_MP2, sizeof(**cmpt_owner));
	if (!*cmpt_owner)
		return TEE_ERROR_OUT_OF_MEMORY;

	for (i = 0; i < NB_CMPT_MP2; i++) {
		if (isolation_enabled) {
			cid_field = mp25_rxcidcfg_fields[i];
			ret = reg_field_read(drv_data->tamp_base, cid_field,
					     &cid_per_zone);

			if (ret != TEE_SUCCESS) {
				EMSG("Can't read field for registers zones");
				free(*cmpt_owner);
				return ret;
			}

			(*cmpt_owner)[i] = (cid_per_zone == CURRENT_CID);
		} else {
			(*cmpt_owner)[i] = true;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_get_access_rights(struct stm32_tamp_nvram_drv_data *drv_data,
			     enum stm32_tamp_bkpreg_access **bkpreg_access)
{
	int index = 0;
	int nb_zones = drv_data->cdata->nb_zones;
	bool *compartment_owner = NULL;
	TEE_Result ret = TEE_ERROR_GENERIC;

	ret = stm32_tamp_get_cmpt_owner_mp2(drv_data, &compartment_owner);
	if (ret != TEE_SUCCESS)
		return ret;

	*bkpreg_access = calloc(NB_ZONES_STM32MP2, sizeof(**bkpreg_access));
	if (!*bkpreg_access) {
		free(compartment_owner);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	for (index = 0; index < nb_zones; index++) {
		switch (index) {
		case BKPREG_ZONE_1_RIF1:
			if (compartment_owner[1])
				(*bkpreg_access)[index] = RSWS;
			else
				(*bkpreg_access)[index] = NORNOW;
			break;
		case BKPREG_ZONE_1_RIF2:
			if (compartment_owner[2])
				(*bkpreg_access)[index] = RSWS;
			else
				(*bkpreg_access)[index] = NORNOW;
			break;
		case BKPREG_ZONE_2_RIF1:
			if (compartment_owner[1])
				(*bkpreg_access)[index] = RNSWS;
			else if (compartment_owner[2])
				(*bkpreg_access)[index] = RONS;
			else
				(*bkpreg_access)[index] = NORNOW;
			break;
		case BKPREG_ZONE_2_RIF2:
			if (compartment_owner[2])
				(*bkpreg_access)[index] = RNSWS;
			else if (compartment_owner[1])
				(*bkpreg_access)[index] = RONS;
			else
				(*bkpreg_access)[index] = NORNOW;
			break;
		case BKPREG_ZONE_3_RIF1:
			if (compartment_owner[1])
				(*bkpreg_access)[index] = RNSWNS;
			else if (compartment_owner[0] || compartment_owner[2])
				(*bkpreg_access)[index] = RONS;
			else
				(*bkpreg_access)[index] = NORNOW;
			break;
		case BKPREG_ZONE_3_RIF0:
			if (compartment_owner[0])
				(*bkpreg_access)[index] = RNSWNS;
			else if (compartment_owner[1] || compartment_owner[2])
				(*bkpreg_access)[index] = RONS;
			else
				(*bkpreg_access)[index] = NORNOW;
			break;
		case BKPREG_ZONE_3_RIF2:
			if (compartment_owner[2])
				(*bkpreg_access)[index] = RNSWNS;
			else if (compartment_owner[0] || compartment_owner[1])
				(*bkpreg_access)[index] = RONS;
			else
				(*bkpreg_access)[index] = NORNOW;
			break;
		default:
			free(compartment_owner);
			free(*bkpreg_access);
			return TEE_ERROR_GENERIC;
		}
	}

	drv_data->compartment_owner = compartment_owner;
	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_nvram_write_byte(struct stm32_tamp_nvram_drv_data *drv_data,
			    uint32_t offset, uint8_t byte)
{
	int offset_aligned = ROUNDDOWN(offset, sizeof(uint32_t));
	int byte_in_word = offset - offset_aligned;
	uint32_t read_value = 0;
	uint32_t to_be_written_value = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;

	if (!byte)
		return TEE_ERROR_NO_DATA;

	ret = stm32_tamp_nvram_read_bkp(drv_data, offset_aligned, &read_value);
	if (ret != TEE_SUCCESS)
		return ret;

	to_be_written_value = read_value & ~(0xFFUL << (byte_in_word * 8));
	to_be_written_value |= (uint32_t)byte << (byte_in_word * 8);

	return stm32_tamp_nvram_write_bkp(drv_data, offset_aligned,
					  to_be_written_value);
}

static TEE_Result
stm32_tamp_nvram_read_byte(struct stm32_tamp_nvram_drv_data *drv_data,
			   unsigned int offset, uint8_t *byte)
{
	int offset_aligned = ROUNDDOWN(offset, sizeof(uint32_t));
	int byte_in_word = offset - offset_aligned;
	uint32_t read_value = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;

	if (!byte)
		return TEE_ERROR_NO_DATA;

	ret = stm32_tamp_nvram_read_bkp(drv_data, offset_aligned, &read_value);

	if (ret != TEE_SUCCESS)
		return ret;

	*byte = (read_value >> (byte_in_word * 8)) & 0xFF;

	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_nvram_read(struct stm32_tamp_nvram_drv_data *drv_data, int offset,
		      uint8_t *buf_u8, int size)
{
	uint8_t byte = 0;
	uint32_t temp_reg32 = 0;
	size_t i = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;
	unsigned int total = 0;

	if (offset < 0 || !buf_u8)
		return TEE_ERROR_BAD_PARAMETERS;

	total = offset + size;
	i = offset;
	while (i < total) {
		if ((i + (int)sizeof(uint32_t)) <= total &&
		    IS_ALIGNED_WITH_TYPE(i, uint32_t)) {
			ret = stm32_tamp_nvram_read_bkp(drv_data, i,
							&temp_reg32);
			if (ret != TEE_SUCCESS) {
				DMSG("BKPreg %zu is not allowed to be read",
				     (i / sizeof(uint32_t)));
				temp_reg32 = 0;
			}

			memcpy(buf_u8, &temp_reg32, sizeof(uint32_t));
			buf_u8 += sizeof(uint32_t);
			i += sizeof(uint32_t);
		} else {
			ret = stm32_tamp_nvram_read_byte(drv_data, i, &byte);
			if (ret != TEE_SUCCESS) {
				DMSG("BKPreg %zu is not allowed to be read",
				     (i / sizeof(uint32_t)));
				byte = 0;
			}
			*buf_u8 = byte;
			i++;
			buf_u8++;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_nvram_write(struct stm32_tamp_nvram_drv_data *drv_data, int offset,
		       const uint8_t *buf_u8, int size)
{
	uint32_t temp_reg32 = 0;
	size_t total = offset + size;
	size_t i = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;
	uint32_t reg_idx __maybe_unused = 0;

	if (offset < 0)
		return TEE_ERROR_BAD_PARAMETERS;

	i = offset;
	while (i < total) {
		if (i + sizeof(uint32_t) <= total &&
		    IS_ALIGNED(i, sizeof(uint32_t))) {
			memcpy(&temp_reg32, buf_u8, sizeof(uint32_t));
			ret = stm32_tamp_nvram_write_bkp(drv_data, i,
							 temp_reg32);
			if (ret != TEE_SUCCESS) {
				DMSG("BKPreg %zu is not allowed to be written",
				     (i / sizeof(uint32_t)));
			}

			buf_u8 += sizeof(uint32_t);
			i += sizeof(uint32_t);
		} else {
			ret = stm32_tamp_nvram_write_byte(drv_data, i, *buf_u8);
			if (ret != TEE_SUCCESS) {
				DMSG("BKPreg %zu is not allowed to be written",
				     (i / sizeof(uint32_t)));
			}
			i++;
			buf_u8++;
		}
	}

	return TEE_SUCCESS;
}

static TEE_Result
stm32_tamp_nvram_get_backup_zones(struct stm32_tamp_nvram_drv_data *drv_data,
				  int **bkpreg_zones_end)
{
	int nb_zones = drv_data->cdata->nb_zones;
	const struct stm32_tamp_nvram_cdata *cdata = drv_data->cdata;
	int zone_idx = 0;
	uint32_t offset_field = 0;
	const struct reg_field *zone_cfg_field = NULL;
	TEE_Result ret = TEE_ERROR_GENERIC;

	*bkpreg_zones_end = calloc(nb_zones, sizeof(**bkpreg_zones_end));
	if (!*bkpreg_zones_end)
		return TEE_ERROR_OUT_OF_MEMORY;

	/* Set end frontiers of the n-1 first zones */
	for (zone_idx = 0; zone_idx < nb_zones - 1; zone_idx++) {
		zone_cfg_field = &cdata->zone_cfg[zone_idx];
		ret = reg_field_read(drv_data->tamp_base, *zone_cfg_field,
				     &offset_field);

		if (ret != TEE_SUCCESS) {
			EMSG("Can't read field for registers zones");
			free(*bkpreg_zones_end);
			return ret;
		}

		(*bkpreg_zones_end)[zone_idx] = offset_field - 1;
	}

	/* The last zone end is defined by the number of registers in TAMP */
	(*bkpreg_zones_end)[zone_idx] = drv_data->nb_total_regs - 1;

	return TEE_SUCCESS;
}

static TEE_Result stm32_tamp_nvram_read_cell(struct nvmem_cell *cell,
					     uint8_t *data, size_t buf_len,
					     size_t *read_len)
{
	struct stm32_tamp_nvram_drv_data *drv_data = cell->drv_data;
	TEE_Result ret;

	if (buf_len < cell->len)
		return TEE_ERROR_BAD_PARAMETERS;

	ret = stm32_tamp_nvram_read(drv_data, cell->offset, data, cell->len);

	if (ret == TEE_SUCCESS && read_len)
		*read_len = cell->len;

	return ret;
}

static TEE_Result stm32_tamp_nvram_write_cell(struct nvmem_cell *cell,
					      uint8_t *data, size_t len)
{
	struct stm32_tamp_nvram_drv_data *drv_data = cell->drv_data;

	if (len > cell->len)
		return TEE_ERROR_BAD_PARAMETERS;

	return stm32_tamp_nvram_write(drv_data, cell->offset, data, cell->len);
}

static TEE_Result stm32_tamp_nvram_free_cell(struct nvmem_cell *cell)
{
	free(cell);

	return TEE_SUCCESS;
}

static const struct nvmem_ops stm32_tamp_nvram_nvmem_ops = {
	.read_cell = stm32_tamp_nvram_read_cell,
	.write_cell = stm32_tamp_nvram_write_cell,
	.free_cell = stm32_tamp_nvram_free_cell,
};

static struct nvmem_cell *stm32_tamp_nvram_dt_get(struct dt_pargs *a,
						  void *data, TEE_Result *res)
{
	struct nvmem_cell *cell = NULL;

	cell = calloc(1, sizeof(*cell));
	if (!cell) {
		*res = TEE_ERROR_OUT_OF_MEMORY;
		return NULL;
	}

	*res = nvmem_cell_parse_dt(a->fdt, a->phandle_node, cell);
	if (*res)
		goto out_free;

	cell->ops = &stm32_tamp_nvram_nvmem_ops;
	cell->drv_data = data;
	*res = TEE_SUCCESS;

	return cell;

out_free:
	free(cell);

	return NULL;
}

static TEE_Result stm32_tamp_nvram_probe(const void *fdt, int node,
					 const void *compat_data)
{
	int parent_node = 0;
	struct stm32_tamp_nvram_drv_data *drv_data = NULL;
	paddr_t pbase = 0;
	size_t tamp_size = 0;
	size_t size = 0;
	TEE_Result ret = TEE_ERROR_GENERIC;
	struct io_pa_va p = { };

	if (!(_fdt_get_status(fdt, node) & DT_STATUS_OK_SEC))
		return TEE_SUCCESS;

	drv_data = calloc(1, sizeof(*drv_data));
	if (!drv_data)
		return TEE_ERROR_OUT_OF_MEMORY;

	/* Get the parent base address and size */
	pbase = _fdt_reg_base_address(fdt, node);
	if (pbase == DT_INFO_INVALID_REG) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto free_nvram;
	}
	size = _fdt_reg_size(fdt, node);
	if (size == DT_INFO_INVALID_REG_SIZE) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto free_nvram;
	}

	p.pa = pbase;
	p.va = (vaddr_t)NULL;
	drv_data->base = io_pa_or_va_secure(&p, 1);

	drv_data->nb_total_regs = size / sizeof(uint32_t);

	parent_node = fdt_parent_offset(fdt, node);
	if (parent_node < 0) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto free_nvram;
	}

	/* Get the TAMP Backup registers base address and size */
	pbase = _fdt_reg_base_address(fdt, parent_node);
	if (pbase == DT_INFO_INVALID_REG) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto free_nvram;
	}

	tamp_size = _fdt_reg_size(fdt, parent_node);
	if (tamp_size == DT_INFO_INVALID_REG_SIZE) {
		ret = TEE_ERROR_BAD_PARAMETERS;
		goto free_nvram;
	}

	p.pa = pbase;
	p.va = (vaddr_t)NULL;
	drv_data->tamp_base = io_pa_or_va_secure(&p, 1);

	drv_data->cdata = compat_data;

	ret = stm32_tamp_nvram_get_backup_zones(drv_data,
						&drv_data->bkpreg_zones_end);
	if (ret != TEE_SUCCESS) {
		EMSG("Can't get the backup zone from tamp regs");
		ret = TEE_ERROR_OUT_OF_MEMORY;
		goto free_nvram;
	}

	if (drv_data->cdata == &stm32mp25_tamp_nvram) {
		ret = stm32_tamp_get_access_rights(drv_data,
						   &drv_data->bkpreg_access);
		if (ret != TEE_SUCCESS)
			goto free_bkpreg_zones_end;
	} else if (drv_data->cdata == &stm32mp1_tamp_nvram) {
		drv_data->bkpreg_access = mp1_bkpreg_access;
	} else {
		ret = TEE_ERROR_NOT_SUPPORTED;
		goto free_bkpreg_zones_end;
	}

	drv_data->node = node;
	drv_data->fdt = fdt;

	return nvmem_register_provider(fdt, node, stm32_tamp_nvram_dt_get,
				       drv_data);

free_bkpreg_zones_end:
	free(drv_data->bkpreg_zones_end);
free_nvram:
	free(drv_data);
	return ret;
}

static const struct dt_device_match stm32_tamp_nvram_match_table[] = {
	{ .compatible = "st,stm32mp15-tamp-nvram",
	  .compat_data = &stm32mp1_tamp_nvram },
	{ .compatible = "st,stm32mp25-tamp-nvram",
	  .compat_data = &stm32mp25_tamp_nvram },
	{},
};

DEFINE_DT_DRIVER(stm32_tamp_nvram_dt_driver) = {
	.name = "stm32_tamp_nvram",
	.type = DT_DRIVER_NVMEM,
	.match_table = stm32_tamp_nvram_match_table,
	.probe = stm32_tamp_nvram_probe,
};
