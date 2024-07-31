// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021-2022, STMicroelectronics
 */

 #include <drivers/stm32_rif.h>
 #include <kernel/panic.h>

TEE_Result stm32_rif_check_access(uint32_t cidcfgr,
				  uint32_t semcr,
				  unsigned int nb_cid_supp,
				  unsigned int cid_to_check)
{
	uint32_t scid_mask = 0;
	uint32_t msb_nb_cid_supp = sizeof(nb_cid_supp) * 8 -
				   __builtin_clz(nb_cid_supp | 1) - 1;

	/* SCID bitfield highend can't be > SCID_SHIFT + MAX_CID_BITFIELD */
	assert(msb_nb_cid_supp < MAX_CID_BITFIELD);

	scid_mask = GENMASK_32(SCID_SHIFT + msb_nb_cid_supp, SCID_SHIFT);

	if (!(cidcfgr & _CIDCFGR_CFEN))
		return TEE_SUCCESS;

	if (SCID_OK(cidcfgr, scid_mask, cid_to_check))
		return TEE_SUCCESS;

	if (SEM_EN_AND_OK(cidcfgr, cid_to_check)) {
		if (!(semcr & _SEMCR_MUTEX) ||
		    ((semcr & scid_mask) >> SCID_SHIFT) == cid_to_check) {
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_ACCESS_DENIED;
}

void stm32_rif_parse_cfg(uint32_t rif_conf,
			 struct rif_conf_data *conf_data,
			 unsigned int nb_cid_supp,
			 unsigned int nb_channel)
{
	uint32_t cidcfdg_conf_mask = 0;
	uint32_t channel_id = 0;
	uint32_t msb_nb_cid_supp = sizeof(nb_cid_supp) * 8 -
				   __builtin_clz(nb_cid_supp | 1) - 1;
	uint32_t scid_mask = 0;
	uint32_t semwl_mask = 0;
	unsigned int conf_index = 0;

	/* SCID bitfield highend can't be > SCID_SHIFT + MAX_CID_BITFIELD */
	assert(msb_nb_cid_supp < MAX_CID_BITFIELD);

	scid_mask = GENMASK_32(SCID_SHIFT + msb_nb_cid_supp, SCID_SHIFT);
	semwl_mask = GENMASK_32(SEMWL_SHIFT + nb_cid_supp, SEMWL_SHIFT);

	cidcfdg_conf_mask = scid_mask | semwl_mask | _CIDCFGR_CFEN |
			    _CIDCFGR_SEMEN;

	/* Shift corresponding to the desired resources */
	channel_id = RIF_CHANNEL_ID(rif_conf);
	if (channel_id >= nb_channel)
		panic("Bad RIF controllers number");

	/*
	 * Make sure we set the bits in the right sec and priv conf register.
	 * This is done to support IPS having more than 32 channels.
	 */
	conf_index = channel_id / 32;

	/* Privilege configuration */
	if (rif_conf & RIFPROT_PRIV)
		conf_data->priv_conf[conf_index] |= BIT(channel_id);

	/* Security RIF configuration */
	if (rif_conf & RIFPROT_SEC)
		conf_data->sec_conf[conf_index] |= BIT(channel_id);

	/* RIF configuration lock */
	if (rif_conf & RIFPROT_LOCK && conf_data->lock_conf)
		conf_data->lock_conf[conf_index] |= BIT(channel_id);

	/* CID configuration */
	conf_data->cid_confs[channel_id] = rif_conf & cidcfdg_conf_mask;

	/* This resource will be configured */
	conf_data->access_mask[conf_index] |= BIT(channel_id);
}

bool stm32_rif_is_semaphore_available(vaddr_t addr)
{
	return !(io_read32(addr) & _SEMCR_MUTEX);
}

TEE_Result stm32_rif_acquire_semaphore(vaddr_t addr,
				       unsigned int nb_cid_supp)
{
	uint32_t msb_nb_cid_supp = sizeof(nb_cid_supp) * 8 -
				   __builtin_clz(nb_cid_supp | 1) - 1;
	uint32_t scid_mask = 0;

	/* SCID bitfield highend can't be > SCID_SHIFT + MAX_CID_BITFIELD */
	assert(msb_nb_cid_supp < MAX_CID_BITFIELD);

	scid_mask = GENMASK_32(SCID_SHIFT + msb_nb_cid_supp, SCID_SHIFT);

	io_setbits32(addr, _SEMCR_MUTEX);

	/* Check that cortex A has the semaphore */
	if (stm32_rif_is_semaphore_available(addr) ||
	    ((io_read32(addr) & scid_mask) >> SCID_SHIFT) != RIF_CID1)
		return TEE_ERROR_ACCESS_DENIED;

	return TEE_SUCCESS;
}

TEE_Result stm32_rif_release_semaphore(vaddr_t addr,
				       unsigned int nb_cid_supp)
{
	unsigned int msb_nb_cid_supp = sizeof(nb_cid_supp) * 8 -
				       __builtin_clz(nb_cid_supp | 1) - 1;
	uint32_t scid_mask = 0;

	/* SCID bitfield highend can't be > SCID_SHIFT + MAX_CID_BITFIELD */
	assert(msb_nb_cid_supp <= MAX_CID_BITFIELD);

	if (stm32_rif_is_semaphore_available(addr))
		return TEE_SUCCESS;

	scid_mask = GENMASK_32(SCID_SHIFT + msb_nb_cid_supp, SCID_SHIFT);

	io_clrbits32(addr, _SEMCR_MUTEX);

	/* Ok if another compartment takes the semaphore before the check */
	if (!stm32_rif_is_semaphore_available(addr) &&
	    ((io_read32(addr) & scid_mask) >> SCID_SHIFT) == RIF_CID1)
		return TEE_ERROR_ACCESS_DENIED;

	return TEE_SUCCESS;
}
