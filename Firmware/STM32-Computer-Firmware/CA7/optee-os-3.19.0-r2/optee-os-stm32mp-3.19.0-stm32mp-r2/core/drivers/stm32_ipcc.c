// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#include <arm.h>
#include <config.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/stm32_rif.h>
#include <drivers/mailbox.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/dt_driver.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stm32_util.h>
#include <trace.h>

#define IPCC_C1SECCFGR			U(0x80)
#define IPCC_C1PRIVCFGR			U(0x84)
#define IPCC_C1CIDCFGR			U(0x88)
#define IPCC_C2SECCFGR			U(0x90)
#define IPCC_C2PRIVCFGR			U(0x94)
#define IPCC_C2CIDCFGR			U(0x98)
#define IPCC_HWCFGR			U(0x3F0)

#define IPCC_C1CR			U(0x00)
#define IPCC_C1MR			U(0x04)
#define IPCC_C1SCR			U(0x08)
#define IPPC_C1TOC2SR			U(0x0C)

#define IPCC_C2CR			U(0x10)
#define IPCC_C2MR			U(0x14)
#define IPCC_C2SCR			U(0x18)
#define IPPC_C2TOC2SR			U(0x1C)

/* Offset within a core instance */
#define IPCC_CR				U(0x0)
#define IPCC_MR				U(0x4)
#define IPCC_SCR			U(0x8)
#define IPCC_TOSR			U(0xC)
#define IPCC_SECCFGR			U(0x80)
#define IPCC_PRIVCFGR			U(0x84)
#define IPCC_CIDCFGR			U(0x98)

/* Mask for  channel rxo and txf */
#define IPCC_ALL_MR_TXF_CH_MASK		GENMASK_32(31, 16)
#define IPCC_ALL_MR_RXO_CH_MASK		GENMASK_32(15, 0)
#define IPCC_ALL_SR_CH_MASK		GENMASK_32(15, 0)

/*  Define for core instance register */
/*!< PROCESSOR  RECEIVE CHANNEL OCCUPIED NON-SECURE INTERRUPT ENABLE */
#define IPCC_CR_RXOIE			BIT(0)
 /*!< PROCESSOR  RECEIVE CHANNEL OCCUPIED SECURE INTERRUPT ENABLE */
#define IPCC_CR_SECRXOIE		BIT(1)
/*!< PROCESSOR TRANSMIT CHANNEL FREE NON-SECURE INTERRUPT ENABLE */
#define IPCC_CR_TXFIE			BIT(16)
 /*!< PROCESSOR TRANSMIT CHANNEL FREE SECURE INTERRUPT ENABLE */
#define IPCC_CR_SECTXFIE		BIT(17)

#define IPCC_MR_CH1FM_POS		U(16)
#define IPCC_SCR_CH1S_POS		U(16)

/*
 * CIDCFGR register bitfields
 */
#define IPCC_CIDCFGR_SCID_MASK		GENMASK_32(6, 4)
#define IPCC_CIDCFGR_CONF_MASK		(_CIDCFGR_CFEN |	 \
					 IPCC_CIDCFGR_SCID_MASK)

/*
 * PRIVCFGR register bitfields
 */
#define IPCC_PRIVCFGR_MASK		GENMASK_32(15, 0)

/*
 * SECCFGR register bitfields
 */
#define IPCC_SECCFGR_MASK		GENMASK_32(15, 0)

/*
 * IPCC_HWCFGR register bitfields
 */
#define IPCC_HWCFGR_CHAN_MASK		GENMASK_32(7, 0)

/*
 * Miscellaneous
 */
#define IPCC_NB_MAX_RIF_CHAN		U(16)

#define IPCC_NB_MAX_CID_SUPPORTED	U(7)

enum {
	IPCC_ITR_RXO,
	IPCC_ITR_TXF,
	IPCC_ITR_NUM,
};

/* IPCC mbox data  */
struct stm32_ipcc_mbx_data {
	/* Lock to protect concurrent access */
	unsigned int lock;
	uint32_t channel_sec_mask;
	uint32_t channel_enable_mask;
	unsigned int num_ch;
	mbox_rx_callback_t rx_cb;
	mbox_tx_callback_t tx_cb;
	struct mbox_dev *cb_ctx;
};

struct ipcc_pdata {
	/*
	 * An IPCC has nb_channels_cfg channel configuration for its
	 * (nb_channels_cfg / 2) bi-directionnal channels
	 */
	unsigned int nb_channels_cfg;
	struct clk *ipcc_clock;
	vaddr_t base;
	struct rif_conf_data conf_data;

	/* Remote proc base address */
	vaddr_t rbase;
	/* Local  proc base address */
	vaddr_t lbase;
	size_t itr_num[IPCC_ITR_NUM];
	struct itr_handler *itr[IPCC_ITR_NUM];

	/* Single mailbox user (mailbox framework) */
	struct stm32_ipcc_mbx_data data;
	STAILQ_ENTRY(ipcc_pdata) link;
};

static STAILQ_HEAD(, ipcc_pdata) ipcc_list =
		STAILQ_HEAD_INITIALIZER(ipcc_list);

static void apply_rif_config(struct ipcc_pdata *ipcc_d, bool is_tdcid)
{
	uint32_t priv_proc_1 = 0;
	uint32_t priv_proc_2 = 0;
	uint32_t sec_proc_1 = 0;
	uint32_t sec_proc_2 = 0;
	unsigned int i = 0;
	bool is_cid_configured = false;

	/*
	 * Check that the number of channel supported by hardware
	 * is coherent with the config
	 */
	assert((io_read32(ipcc_d->base + IPCC_HWCFGR) &
			  IPCC_HWCFGR_CHAN_MASK) >=
	       ipcc_d->nb_channels_cfg / 2);

	/*
	 * When TDCID, OP-TEE should be the one to set the CID filtering
	 * configuration. Clearing previous configuration prevents
	 * undesired events during the only legitimate configuration.
	 */
	if (is_tdcid) {
		/* IPCC Processor 1 */
		io_clrbits32(ipcc_d->base + IPCC_C1CIDCFGR,
			     IPCC_CIDCFGR_CONF_MASK);

		/* IPCC Processor 2 */
		io_clrbits32(ipcc_d->base + IPCC_C2CIDCFGR,
			     IPCC_CIDCFGR_CONF_MASK);
	}

	/* Split the sec and priv configuration for IPCC processor 1 and 2 */
	sec_proc_1 = ipcc_d->conf_data.sec_conf[0] &
		     GENMASK_32(IPCC_NB_MAX_RIF_CHAN - 1, 0);
	priv_proc_1 = ipcc_d->conf_data.priv_conf[0] &
		     GENMASK_32(IPCC_NB_MAX_RIF_CHAN - 1, 0);

	sec_proc_2 = (ipcc_d->conf_data.sec_conf[0] &
		      GENMASK_32((IPCC_NB_MAX_RIF_CHAN * 2) - 1,
				 IPCC_NB_MAX_RIF_CHAN)) >>
		     IPCC_NB_MAX_RIF_CHAN;
	priv_proc_2 = (ipcc_d->conf_data.priv_conf[0] &
		       GENMASK_32((IPCC_NB_MAX_RIF_CHAN * 2) - 1,
				  IPCC_NB_MAX_RIF_CHAN)) >>
		      IPCC_NB_MAX_RIF_CHAN;

	/* Security and privilege RIF configuration */
	io_clrsetbits32(ipcc_d->base + IPCC_C1PRIVCFGR, IPCC_PRIVCFGR_MASK,
			priv_proc_1);
	io_clrsetbits32(ipcc_d->base + IPCC_C2PRIVCFGR, IPCC_PRIVCFGR_MASK,
			priv_proc_2);
	io_clrsetbits32(ipcc_d->base + IPCC_C1SECCFGR, IPCC_SECCFGR_MASK,
			sec_proc_1);
	io_clrsetbits32(ipcc_d->base + IPCC_C2SECCFGR, IPCC_SECCFGR_MASK,
			sec_proc_2);

	/*
	 * Evaluate RIF CID filtering configuration before setting it.
	 * Parsed configuration must have consistency. If CID filtering
	 * is enabled for an IPCC channel, then it must be the case for all
	 * channels of this processor. This is a configuration check.
	 */
	for (i = 0; i < IPCC_NB_MAX_RIF_CHAN; i++) {
		if (!(BIT(i) & ipcc_d->conf_data.access_mask[0]))
			continue;

		if (!is_cid_configured &&
		    (BIT(0) & ipcc_d->conf_data.cid_confs[i])) {
			is_cid_configured = true;
			if (i == IPCC_NB_MAX_RIF_CHAN - 1)
				panic("Inconsistent IPCC CID filtering RIF configuration");
		}

		if (is_cid_configured &&
		    !(BIT(0) & ipcc_d->conf_data.cid_confs[i]))
			panic("Inconsistent IPCC CID filtering RIF configuration");
	}

	/* IPCC processor 1 CID filtering configuration */
	if (!is_tdcid)
		return;

	io_clrsetbits32(ipcc_d->base + IPCC_C1CIDCFGR,
			IPCC_CIDCFGR_CONF_MASK,
			ipcc_d->conf_data.cid_confs[0]);

	/*
	 * Reset this field to evaluate CID filtering configuration
	 * for processor 2
	 */
	is_cid_configured = false;

	for (i = IPCC_NB_MAX_RIF_CHAN; i < IPCC_NB_MAX_RIF_CHAN * 2; i++) {
		if (!(BIT(i) & ipcc_d->conf_data.access_mask[0]))
			continue;

		if (!is_cid_configured &&
		    (BIT(0) & ipcc_d->conf_data.cid_confs[i])) {
			is_cid_configured = true;
			if (i == (IPCC_NB_MAX_RIF_CHAN * 2) - 1)
				panic("Inconsistent IPCC CID filtering RIF configuration");
		}

		if (is_cid_configured &&
		    !(BIT(0) & ipcc_d->conf_data.cid_confs[i]))
			panic("Inconsistent IPCC CID filtering RIF configuration");
	}

	/* IPCC Processor 2 CID filtering configuration */
	io_clrsetbits32(ipcc_d->base + IPCC_C2CIDCFGR,
			IPCC_CIDCFGR_CONF_MASK,
			ipcc_d->conf_data.cid_confs[IPCC_NB_MAX_RIF_CHAN]);
}

static void stm32_ipcc_pm_resume(struct ipcc_pdata *ipcc)
{
	apply_rif_config(ipcc, true);
}

static void stm32_ipcc_pm_suspend(struct ipcc_pdata *ipcc __unused)
{
	/*
	 * Do nothing because IPCC forbids RIF configuration read if CID
	 * filtering is enabled. We'll simply restore the device tree RIF
	 * configuration.
	 */
}

static TEE_Result
stm32_ipcc_pm(enum pm_op op, unsigned int pm_hint,
	      const struct pm_callback_handle *pm_handle)
{
	struct ipcc_pdata *ipcc = pm_handle->handle;
	TEE_Result res = TEE_ERROR_GENERIC;
	bool is_tdcid = false;

	if (stm32_rifsc_check_tdcid(&is_tdcid))
		panic();

	if (!PM_HINT_IS_STATE(pm_hint, CONTEXT) || !is_tdcid)
		return TEE_SUCCESS;

	res = clk_enable(ipcc->ipcc_clock);
	if (res)
		return res;

	if (op == PM_OP_RESUME)
		stm32_ipcc_pm_resume(ipcc);
	else
		stm32_ipcc_pm_suspend(ipcc);

	clk_disable(ipcc->ipcc_clock);

	return TEE_SUCCESS;
}

static bool ipcc_channel_is_active(vaddr_t lbase, unsigned int chn)
{
	return io_read32(lbase + IPCC_TOSR) & BIT(chn);
}

static void ipcc_channel_transmit(vaddr_t lbase, unsigned int chn, bool enable)
{
	if (enable)
		io_clrbits32(lbase + IPCC_MR, BIT(chn + IPCC_MR_CH1FM_POS));
	else
		io_setbits32(lbase + IPCC_MR, BIT(chn + IPCC_MR_CH1FM_POS));
}

static void ipcc_channel_receive(vaddr_t lbase, unsigned int chn, bool enable)
{
	if (enable)
		io_clrbits32(lbase + IPCC_MR, BIT(chn));
	else
		io_setbits32(lbase + IPCC_MR, BIT(chn));
}

static enum itr_return stm32_ipcc_mbox_rxo_isr(struct itr_handler *h)
{
	struct ipcc_pdata *pdata = (struct ipcc_pdata *)h->data;
	struct stm32_ipcc_mbx_data *data = &pdata->data;
	unsigned int value = 0;
	uint32_t mask = 0;
	unsigned int i = 0;

	mask = ~io_read32(pdata->lbase + IPCC_MR) & IPCC_ALL_MR_RXO_CH_MASK;
	mask &= io_read32(pdata->rbase + IPCC_TOSR) & IPCC_ALL_SR_CH_MASK;

	/* Get mask for enabled channels only */
	mask &= data->channel_enable_mask;

	for (i = 0; i < data->num_ch; i++) {
		if (!(BIT(i) & mask))
			continue;
		FMSG("rx channel = %"PRIu32, i);
		if (data->rx_cb)
			data->rx_cb(data->cb_ctx, i, &value);
		/* Clear rxo flag */
		io_write32(pdata->lbase + IPCC_SCR, BIT(i));
	}
	return ITRR_HANDLED;
}

static enum itr_return stm32_ipcc_mbox_txf_isr(struct itr_handler *h)
{
	struct ipcc_pdata *pdata = (struct ipcc_pdata *)h->data;
	struct stm32_ipcc_mbx_data *data = &pdata->data;
	uint32_t mask = 0;
	uint32_t i = 0;

	mask = ~io_read32(pdata->lbase + IPCC_MR) & IPCC_ALL_MR_TXF_CH_MASK;
	mask = mask >> IPCC_MR_CH1FM_POS;

	mask &= ~io_read32(pdata->lbase + IPCC_TOSR) & IPCC_ALL_SR_CH_MASK;

	/* Get mask for enabled channels only */
	mask &= data->channel_enable_mask;

	for (i = 0; i < data->num_ch; i++) {
		if (BIT(i) & mask) {
			if (data->tx_cb)
				data->tx_cb(data->cb_ctx, i);
			/* Disable txf interrupt */
			ipcc_channel_transmit(pdata->lbase, i, false);
		}
	}
	return ITRR_HANDLED;
}

static TEE_Result stm32_ipcc_mbox_send(const struct mbox_desc *desc,
				       unsigned int id,
				       const void *buff __unused, size_t size)
{
	struct ipcc_pdata *pdata = (struct ipcc_pdata *)desc->priv;
	struct stm32_ipcc_mbx_data *data = &pdata->data;

	/* No data transmission, only doorbell */
	if (size)
		return TEE_ERROR_NOT_SUPPORTED;

	if (id >= data->num_ch || !(BIT(id) & data->channel_sec_mask)) {
		EMSG("invalid id %u", id);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	FMSG("Send msg on channel %u", id);

	/* Check that the channel is free */
	if (ipcc_channel_is_active(pdata->lbase, id)) {
		FMSG("Waiting for channel to be freed");
		return TEE_ERROR_BUSY;
	}
	/* Set channel txs Flag */
	io_write32(pdata->lbase + IPCC_SCR, BIT(id + IPCC_SCR_CH1S_POS));
	/* Enable channel txf interrupt */
	ipcc_channel_transmit(pdata->lbase, id, true);

	return TEE_SUCCESS;
}

static size_t stm32_ipcc_mbox_get_mtu(const struct mbox_desc *desc __unused)
{
	/* No data transfer capability */
	return 0;
}

static size_t stm32_ipcc_mbox_channel_count(const struct mbox_desc *desc)
{
	struct ipcc_pdata *pdata = (struct ipcc_pdata *)desc->priv;
	struct stm32_ipcc_mbx_data *data = &pdata->data;

	assert(data->num_ch);

	return data->num_ch;
}

static TEE_Result stm32_ipcc_mbox_register_callback(const
						    struct mbox_desc *desc,
						    mbox_rx_callback_t rx_cb,
						    mbox_tx_callback_t tx_cb,
						    struct mbox_dev *cb_ctx)
{
	struct ipcc_pdata *pdata = (struct ipcc_pdata *)desc->priv;
	struct stm32_ipcc_mbx_data *data = &pdata->data;
	uint32_t except = 0;
	TEE_Result res = TEE_SUCCESS;

	except = cpu_spin_lock_xsave(&data->lock);
	if (!data->rx_cb && !data->tx_cb && !data->cb_ctx) {
		data->rx_cb = rx_cb;
		data->tx_cb = tx_cb;
		data->cb_ctx = cb_ctx;
		res = TEE_SUCCESS;
	} else {
		res = TEE_ERROR_BAD_STATE;
	}
	cpu_spin_unlock_xrestore(&data->lock, except);

	return res;
}

static TEE_Result stm32_ipcc_mbox_enable(const struct mbox_desc *desc,
					 bool enable, unsigned int id)
{
	struct ipcc_pdata *pdata = (struct ipcc_pdata *)desc->priv;
	struct stm32_ipcc_mbx_data *data = &pdata->data;
	uint32_t channel_enable_mask = 0;
	uint32_t except = 0;

	if (!(BIT(id) & data->channel_sec_mask))
		return TEE_ERROR_BAD_PARAMETERS;

	if (enable && clk_enable(pdata->ipcc_clock))
		panic("Cannot access ipcc clock");

	except = cpu_spin_lock_xsave(&data->lock);

	if (enable)
		channel_enable_mask = (BIT(id) & data->channel_sec_mask) |
			data->channel_enable_mask;
	else
		channel_enable_mask = ~(BIT(id) & data->channel_sec_mask) &
			data->channel_enable_mask;

	if (channel_enable_mask && !data->channel_enable_mask) {
		/* Enable secure txf and rxo interrupt */
		io_setbits32(pdata->lbase + IPCC_CR,
			     IPCC_CR_SECTXFIE | IPCC_CR_SECRXOIE);
	}

	if (!channel_enable_mask && data->channel_enable_mask) {
		/* Disable secure txf and rxo interrupt */
		io_clrbits32(pdata->lbase + IPCC_CR,
			     IPCC_CR_SECTXFIE | IPCC_CR_SECRXOIE);
	}

	/*
	 * Update mask and then enable rxo interrupt
	 * since mask is used within rxo interrupt handler and
	 * do the opposite for disable
	 */
	if (enable)
		data->channel_enable_mask = channel_enable_mask;
	ipcc_channel_receive(pdata->lbase, id, enable);
	if (!enable)
		data->channel_enable_mask = channel_enable_mask;

	cpu_spin_unlock_xrestore(&data->lock, except);
	if (!enable)
		clk_disable(pdata->ipcc_clock);

	return TEE_SUCCESS;
}

static TEE_Result stm32_ipcc_mbox_complete(const struct mbox_desc *desc,
					   unsigned int id)
{
	struct ipcc_pdata *pdata = (struct ipcc_pdata *)desc->priv;
	struct stm32_ipcc_mbx_data *data = &pdata->data;

	if (!(BIT(id) & data->channel_enable_mask))
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}

static const struct mbox_ops stm32_ipcc_mbox_ops = {
	.send = stm32_ipcc_mbox_send,
	.register_callback = stm32_ipcc_mbox_register_callback,
	.max_data_size = stm32_ipcc_mbox_get_mtu,
	.max_channel_count = stm32_ipcc_mbox_channel_count,
	.channel_interrupt = stm32_ipcc_mbox_enable,
	.complete = stm32_ipcc_mbox_complete,
};

static void stm32_ipcc_mbox_init(struct ipcc_pdata *pdata, const void *fdt,
				 int node)
{
	const fdt32_t *cuint = NULL;
	int lenp = 0;
	uint32_t i = 0;
	struct stm32_ipcc_mbx_data *data = &pdata->data;
	struct mbox_desc *desc = NULL;

	/* Null phandle means no client of this node, no mailbox support */
	if (!fdt_get_phandle(fdt, node))
		return;

	cuint = fdt_getprop(fdt, node, "interrupts", &lenp);
	if (!cuint) {
		if (lenp != -FDT_ERR_NOTFOUND)
			panic();
		/* No interrupt means that ipcc cannot be used for mailbox */
		return;
	}
	pdata->itr_num[IPCC_ITR_RXO] = fdt32_to_cpu(*(cuint + 1)) + 32;
	pdata->itr_num[IPCC_ITR_TXF] = fdt32_to_cpu(*(cuint + 4)) + 32;

	desc = calloc(1, sizeof(*desc));
	if (!desc)
		panic();

	desc->ops = &stm32_ipcc_mbox_ops;
	desc->priv = pdata;

	if (clk_enable(pdata->ipcc_clock))
		panic("Cannot access ipcc clock");

	/* Disable secure rxo and txf interrupts */
	io_clrbits32(pdata->lbase + IPCC_CR, IPCC_CR_SECTXFIE |
		     IPCC_CR_SECRXOIE);

	/* Fill max Channel */
	data->num_ch = io_read32(pdata->base + IPCC_HWCFGR) &
		IPCC_HWCFGR_CHAN_MASK;

	/* Set channel_sec_mask according to rif protection */
	data->channel_sec_mask = io_read32(pdata->lbase + IPCC_SECCFGR);
	data->channel_enable_mask = 0;

	/* Fix Me : Possibly Add Check on cid filtering and privileged */

	for (i = 0; i < data->num_ch; i++) {
		/* Clear rxo status */
		io_write32(pdata->lbase + IPCC_SCR, BIT(i));
		/* Mask channel rxo and txf interrupts */
		ipcc_channel_receive(pdata->lbase, i, false);
		ipcc_channel_transmit(pdata->lbase, i, false);
	}
	/* Register irq handler */
	pdata->itr[IPCC_ITR_RXO] = itr_alloc_add(pdata->itr_num[IPCC_ITR_RXO],
						 stm32_ipcc_mbox_rxo_isr,
						 ITRF_TRIGGER_LEVEL, pdata);
	pdata->itr[IPCC_ITR_TXF] = itr_alloc_add(pdata->itr_num[IPCC_ITR_TXF],
						 stm32_ipcc_mbox_txf_isr,
						 ITRF_TRIGGER_LEVEL, pdata);

	if (!pdata->itr[IPCC_ITR_RXO])
		panic("Cannot register ipcc rx handler");

	if (!pdata->itr[IPCC_ITR_TXF])
		panic("Cannot register ipcc rx handler");

	itr_enable(pdata->itr[IPCC_ITR_RXO]->it);
	itr_enable(pdata->itr[IPCC_ITR_TXF]->it);

	clk_disable(pdata->ipcc_clock);

	/* Register device to mailbox framework */
	if (IS_ENABLED(CFG_DRIVERS_MAILBOX) &&
	    mbox_dt_register(fdt, node, desc))
		panic("Failed to register ipcc mbox");
}

static TEE_Result parse_dt(const void *fdt, int node, struct ipcc_pdata *ipcc_d)
{
	int lenp = 0;
	unsigned int i = 0;
	uint32_t rif_conf = 0;
	TEE_Result res = TEE_ERROR_GENERIC;
	const fdt32_t *cuint = NULL;
	struct dt_node_info info = { };
	struct io_pa_va addr = { };
	uint32_t proc_id = 0;

	_fdt_fill_device_info(fdt, &info, node);
	assert(info.reg != DT_INFO_INVALID_REG &&
	       info.reg_size != DT_INFO_INVALID_REG_SIZE);

	addr.pa = info.reg;
	ipcc_d->base = io_pa_or_va_secure(&addr, info.reg_size);
	assert(ipcc_d->base);
	/*
	 * Field st-proc-id, fixes CPU core on which driver is instantiated
	 * according to proc ID mailbox lbase. rbase mailbox are swapped.
	 */
	(void)_fdt_read_uint32(fdt, node, "st,proc-id", &proc_id);
	if (proc_id == 2) {
		ipcc_d->lbase = ipcc_d->base + IPCC_C2CR;
		ipcc_d->rbase = ipcc_d->base + IPCC_C1CR;
	} else {
		ipcc_d->lbase = ipcc_d->base + IPCC_C1CR;
		ipcc_d->rbase = ipcc_d->base + IPCC_C2CR;
	}

	/* Gate the IP */
	res = clk_dt_get_by_index(fdt, node, 0, &ipcc_d->ipcc_clock);
	if (res)
		return res;

	cuint = fdt_getprop(fdt, node, "st,protreg", &lenp);
	if (!cuint)
		panic("No RIF configuration available");

	ipcc_d->nb_channels_cfg = (unsigned int)(lenp / sizeof(uint32_t));
	assert(ipcc_d->nb_channels_cfg <= (IPCC_NB_MAX_RIF_CHAN * 2));

	ipcc_d->conf_data.cid_confs = calloc(IPCC_NB_MAX_RIF_CHAN * 2,
					     sizeof(uint32_t));
	ipcc_d->conf_data.sec_conf = calloc(1, sizeof(uint32_t));
	ipcc_d->conf_data.priv_conf = calloc(1, sizeof(uint32_t));
	ipcc_d->conf_data.access_mask = calloc(1, sizeof(uint32_t));
	if (!ipcc_d->conf_data.cid_confs || !ipcc_d->conf_data.sec_conf ||
	    !ipcc_d->conf_data.priv_conf || !ipcc_d->conf_data.access_mask)
		panic("Missing memory capacity for ipcc RIF configuration");

	for (i = 0; i < ipcc_d->nb_channels_cfg; i++) {
		rif_conf = fdt32_to_cpu(cuint[i]);

		stm32_rif_parse_cfg(rif_conf, &ipcc_d->conf_data,
				    IPCC_NB_MAX_CID_SUPPORTED,
				    IPCC_NB_MAX_RIF_CHAN * 2);
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_ipcc_probe(const void *fdt, int node,
				   const void *compat_data __unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct ipcc_pdata *ipcc_d = NULL;
	bool is_tdcid = false;

	res = stm32_rifsc_check_tdcid(&is_tdcid);
	if (res)
		return res;

	ipcc_d = calloc(1, sizeof(*ipcc_d));
	if (!ipcc_d)
		return TEE_ERROR_OUT_OF_MEMORY;

	res = parse_dt(fdt, node, ipcc_d);
	if (res)
		goto err;

	res = clk_enable(ipcc_d->ipcc_clock);
	if (res)
		panic("Cannot access IPCC clock");

	apply_rif_config(ipcc_d, is_tdcid);

	clk_disable(ipcc_d->ipcc_clock);

	STAILQ_INSERT_TAIL(&ipcc_list, ipcc_d, link);

	if (IS_ENABLED(CFG_PM))
		register_pm_core_service_cb(stm32_ipcc_pm, ipcc_d,
					    "stm32-ipcc");

	if (IS_ENABLED(CFG_DRIVERS_MAILBOX))
		stm32_ipcc_mbox_init(ipcc_d, fdt, node);

	return TEE_SUCCESS;

err:
	/* Free all allocated resources */
	free(ipcc_d->conf_data.access_mask);
	free(ipcc_d->conf_data.cid_confs);
	free(ipcc_d->conf_data.priv_conf);
	free(ipcc_d->conf_data.sec_conf);
	free(ipcc_d);

	return res;
}

static const struct dt_device_match stm32_ipcc_match_table[] = {
	{ .compatible = "st,stm32mp25-ipcc" },
	{ }
};

DEFINE_DT_DRIVER(stm32_ipcc_dt_driver) = {
	.name = "st,stm32mp-ipcc",
	.match_table = stm32_ipcc_match_table,
	.probe = stm32_ipcc_probe,
};
