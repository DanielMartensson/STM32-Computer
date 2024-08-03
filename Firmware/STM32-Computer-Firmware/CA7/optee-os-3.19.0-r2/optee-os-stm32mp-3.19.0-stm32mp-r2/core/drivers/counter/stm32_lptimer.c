// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2020-2022, STMicroelectronics
 */
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/counter.h>
#include <drivers/stm32_lptimer.h>
#include <io.h>
#include <kernel/dt.h>
#include <kernel/boot.h>
#include <kernel/interrupt.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <tee_api_types.h>
#include <trace.h>
#include <util.h>

/* LPTIM offset register */
#define _LPTIM_ISR		U(0x000)
#define _LPTIM_ICR		U(0x004)
#define _LPTIM_IER		U(0x008)
#define _LPTIM_CFGR		U(0x00C)
#define _LPTIM_CR		U(0x010)
#define _LPTIM_CMP		U(0x014)
#define _LPTIM_ARR		U(0x018)
#define _LPTIM_CNT		U(0x01C)
#define _LPTIM_CFGR2		U(0x024)
#define _LPTIM_HWCFGR		U(0x3F0)
#define _LPTIM_VERR		U(0x3F4)

/* LPTIM_IXX register fields */
#define _LPTIM_IXX_CMPM		BIT(0)
#define _LPTIM_IXX_ARRM		BIT(1)
#define _LPTIM_IXX_EXTTRIG	BIT(2)
#define _LPTIM_IXX_CMPOK	BIT(3)
#define _LPTIM_IXX_ARROK	BIT(4)
#define _LPTIM_IXX_UP		BIT(5)
#define _LPTIM_IXX_DOWN		BIT(6)
#define _LPTIM_CMPARR_OK	(_LPTIM_IXX_CMPOK | _LPTIM_IXX_ARROK)

/* LPTIM_CFGR register fields */
#define _LPTIM_CFGR_CKSEL		BIT(0)
#define _LPTIM_CFGR_CKPOL_MASK		GENMASK_32(2, 1)
#define _LPTIM_CFGR_CKPOL_SHIFT		1
#define _LPTIM_CFGR_CKFLT_MASK		GENMASK_32(4, 3)
#define _LPTIM_CFGR_CKFLT_SHIFT		3
#define _LPTIM_CFGR_TRGFLT_MASK		GENMASK_32(7, 6)
#define _LPTIM_CFGR_TRGFLT_SHIFT	6
#define _LPTIM_CFGR_PRESC_MASK		GENMASK_32(11, 9)
#define _LPTIM_CFGR_PRESC_SHIFT		9
#define _LPTIM_CFGR_TRIGSEL_MASK	GENMASK_32(15, 13)
#define _LPTIM_CFGR_TRIGSEL_SHIFT	13
#define _LPTIM_CFGR_TRIGEN_MASK		GENMASK_32(18, 17)
#define _LPTIM_CFGR_TRIGEN_SHIFT	17
#define _LPTIM_CFGR_TIMOUT		BIT(19)
#define _LPTIM_CFGR_WAVE		BIT(20)
#define _LPTIM_CFGR_WAVEPOL		BIT(21)
#define _LPTIM_CFGR_PRELOAD		BIT(22)
#define _LPTIM_CFGR_COUNTMODE		BIT(23)
#define _LPTIM_CFGR_ENC			BIT(24)

/* LPTIM_CR register fields */
#define _LPTIM_CR_ENABLE		BIT(0)
#define _LPTIM_CR_SNGSTRT		BIT(1)
#define _LPTIM_CR_CNTSTRT		BIT(2)
#define _LPTIM_CR_COUNTRST		BIT(3)
#define _LPTIM_CR_RSTARE		BIT(4)

/* LPTIM_CMP register fields */
#define _LPTIM_CMP_MASK			GENMASK_32(15, 0)
#define _LPTIM_CMP_SHIFT		0

/* LPTIM_ARR register fields */
#define _LPTIM_ARR_MASK			GENMASK_32(15, 0)
#define _LPTIM_ARR_SHIFT		0

/* LPTIM_CNT register fields */
#define _LPTIM_CNT_MASK			GENMASK_32(15, 0)
#define _LPTIM_CNT_SHIFT		0

/* LPTIM_CFGR2 register fields */
#define _LPTIM_CFGR2_IN1SEL_MASK	GENMASK_32(1, 0)
#define _LPTIM_CFGR2_IN1SEL_SHIFT	0
#define _LPTIM_CFGR2_IN2SEL_MASK	GENMASK_32(5, 4)
#define _LPTIM_CFGR2_IN2SEL_SHIFT	4

/* LPTIM_HWCFGR register fields */
#define _LPTIM_HWCFGR_CFG1_MASK		GENMASK_32(7, 0)
#define _LPTIM_HWCFGR_CFG1_SHIFT	0
#define _LPTIM_HWCFGR_CFG2_MASK		GENMASK_32(15, 8)
#define _LPTIM_HWCFGR_CFG2_SHIFT	8
#define _LPTIM_HWCFGR_CFG3_MASK		GENMASK_32(19, 16)
#define _LPTIM_HWCFGR_CFG3_SHIFT	16
#define _LPTIM_HWCFGR_CFG4_MASK		GENMASK_32(31, 24)
#define _LPTIM_HWCFGR_CFG4_SHIFT	24

/* LPTIM_VERR register fields */
#define _LPTIM_VERR_MINREV_MASK		GENMASK_32(3, 0)
#define _LPTIM_VERR_MINREV_SHIFT	0
#define _LPTIM_VERR_MAJREV_MASK		GENMASK_32(7, 4)
#define _LPTIM_VERR_MAJREV_SHIFT	4

#define _LPTIM_FLD_PREP(field, value) (((uint32_t)(value) << (field ## _SHIFT)) & (field ## _MASK))
#define _LPTIM_FLD_GET(field, value)  (((uint32_t)(value) & (field ## _MASK)) >> (field ## _SHIFT))

/*
 * no common errno between component
 * define lptimer internal errno
 */
#define LPTIM_ERR_NOMEM		12	/* Out of memory */
#define LPTIM_ERR_NODEV		19	/* No such device */
#define LPTIM_ERR_INVAL		22	/* Invalid argument */
#define LPTIM_ERR_NOTSUP	45	/* Operation not supported */

#define TIMEOUT_US_100MS	U(100000)

struct lptimer_compat_data {
	bool ier_wr_disabled;		/* disable lptimer to set dier */
};

struct lptimer_device {
	const void *fdt;
	int node;
	struct stm32_lptimer_platdata pdata;
	struct lptimer_driver_data *ddata;
	struct counter_device *counter_dev;
	struct itr_handler *itr;
	const struct lptimer_compat_data *cdata;
};

struct stm32_lptimer_config {
	struct lptimer_ext_input_cfg ext_input;
	struct lptimer_ext_trigger_cfg ext_trigger;
};

static TEE_Result stm32_lpt_counter_set_alarm(struct counter_device *counter)
{
	struct lptimer_device *lpt_dev = counter_priv(counter);
	uintptr_t base = lpt_dev->pdata.base;
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t cr = 0;
	uint32_t val = 0;

	/* ARR must be strictly greater than the CMP, within max_ticks range */
	if (counter->alarm.ticks >= counter->max_ticks)
		return TEE_ERROR_GENERIC;

	res = clk_enable(lpt_dev->pdata.clock);
	if (res)
		return res;

	cr = io_read32(base + _LPTIM_CR);
	/*
	 * LPTIM_CMP & ARR register must only be modified
	 * when the LPTIM is enabled
	 */
	io_setbits32(base + _LPTIM_CR, _LPTIM_CR_ENABLE);
	io_write32(base + _LPTIM_ARR, counter->alarm.ticks + 1);
	io_write32(base + _LPTIM_CMP, counter->alarm.ticks);

	/* Enforce CMP and ARR are correctly set, then clear these flags */
	if (IO_READ32_POLL_TIMEOUT(base + _LPTIM_ISR, val,
				   (val & _LPTIM_CMPARR_OK) == _LPTIM_CMPARR_OK,
				   0, TIMEOUT_US_100MS)) {
		io_write32(base + _LPTIM_CR, cr);
		clk_disable(lpt_dev->pdata.clock);
		return TEE_ERROR_GENERIC;
	}
	io_write32(base + _LPTIM_ICR, _LPTIM_CMPARR_OK);

	if (lpt_dev->cdata && lpt_dev->cdata->ier_wr_disabled) {
		/*
		 * LPTIM_IER register must only be modified
		 * when the LPTIM is disabled
		 */
		io_clrbits32(base + _LPTIM_CR, _LPTIM_CR_ENABLE);
	}

	/* enable, Compare match Interrupt */
	io_setbits32(base + _LPTIM_IER, _LPTIM_IXX_CMPM);

	/* restore cr */
	io_write32(base + _LPTIM_CR, cr);

	counter->alarm.is_enabled = true;

	/* restart timer if it was enabled before setting the alarm */
	if (cr & _LPTIM_CR_ENABLE)
		io_write32(base + _LPTIM_CR,
			   _LPTIM_CR_ENABLE | _LPTIM_CR_CNTSTRT);

	clk_disable(lpt_dev->pdata.clock);

	return TEE_SUCCESS;
}

static TEE_Result stm32_lpt_counter_cancel_alarm(struct counter_device *counter)
{
	struct lptimer_device *lpt_dev = counter_priv(counter);
	uintptr_t base = lpt_dev->pdata.base;
	uint32_t cr = 0;

	cr = io_read32(base + _LPTIM_CR);

	if (lpt_dev->cdata && lpt_dev->cdata->ier_wr_disabled) {
		/*
		 * LPTIM_IER register must only be modified
		 * when the LPTIM is disabled
		 */
		io_clrbits32(base + _LPTIM_CR, _LPTIM_CR_ENABLE);
	}
	io_clrbits32(base + _LPTIM_IER, _LPTIM_IXX_CMPM);

	/*
	 * LPTIM_CMP & ARR registers must only be modified
	 * when the LPTIM is enabled
	 */
	io_setbits32(base + _LPTIM_CR, _LPTIM_CR_ENABLE);
	io_write32(base + _LPTIM_CMP, 0);

	/* restore cr */
	io_write32(base + _LPTIM_CR, cr);

	counter->alarm.is_enabled = false;

	return TEE_SUCCESS;
}

static TEE_Result _stm32_lpt_counter_get_value(struct lptimer_device *lpt_dev,
					       uint32_t *ticks)
{
	uint64_t timeout = timeout_init_us(TIMEOUT_US_100MS);
	uintptr_t base = lpt_dev->pdata.base;
	uint32_t cnt = 0;
	uint32_t cnt_prev = 0;

	/*
	 * Note: a read access can be considered reliable when the
	 * values of the two consecutive read accesses are equal
	 */
	cnt = io_read32(base + _LPTIM_CNT);
	do {
		cnt_prev = cnt;
		cnt = io_read32(base + _LPTIM_CNT);
		if (timeout_elapsed(timeout))
			return TEE_ERROR_GENERIC;
	} while (cnt_prev != cnt);

	*ticks = cnt;

	return TEE_SUCCESS;
}

static TEE_Result stm32_lpt_counter_get_value(struct counter_device *counter,
					      uint32_t *ticks)
{
	struct lptimer_device *lpt_dev = counter_priv(counter);
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!ticks)
		return TEE_ERROR_BAD_PARAMETERS;

	res = clk_enable(lpt_dev->pdata.clock);
	if (res)
		return res;

	res = _stm32_lpt_counter_get_value(lpt_dev, ticks);

	clk_disable(lpt_dev->pdata.clock);

	return res;
}

static TEE_Result stm32_lpt_counter_stop(struct counter_device *counter)
{
	struct lptimer_device *lpt_dev = counter_priv(counter);
	uintptr_t base = lpt_dev->pdata.base;

	io_write32(base + _LPTIM_CR, 0);
	io_write32(base + _LPTIM_CMP, 0);

	clk_disable(lpt_dev->pdata.clock);

	return TEE_SUCCESS;
}

static TEE_Result stm32_lpt_counter_start(struct counter_device *counter,
					  void *config)
{
	struct lptimer_device *lpt_dev = counter_priv(counter);
	struct stm32_lptimer_platdata *pdata = &lpt_dev->pdata;
	struct stm32_lptimer_config *conf =
		(struct stm32_lptimer_config *)config;
	uintptr_t base = pdata->base;
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t cfgr = 0;
	uint32_t cfgr2 = 0;

	if (conf && conf->ext_input.num) {
		cfgr += _LPTIM_CFGR_COUNTMODE;

		cfgr2 += _LPTIM_FLD_PREP(_LPTIM_CFGR2_IN1SEL,
					 conf->ext_input.mux[0]);
		if (conf->ext_input.num == 2)
			cfgr2 += _LPTIM_FLD_PREP(_LPTIM_CFGR2_IN2SEL,
						 conf->ext_input.mux[1]);
	}

	if (conf && conf->ext_trigger.num) {
		cfgr += _LPTIM_FLD_PREP(_LPTIM_CFGR_TRIGSEL,
					conf->ext_trigger.mux);
		cfgr += _LPTIM_FLD_PREP(_LPTIM_CFGR_TRIGEN,
					conf->ext_trigger.polarity);
		cfgr |= _LPTIM_CFGR_TIMOUT;
	}

	res = clk_enable(lpt_dev->pdata.clock);
	if (res)
		return res;

	/*
	 * LPTIM CFGR2 & IER registers must only be modified
	 * when the LPTIM is disabled
	 */
	io_write32(base + _LPTIM_CR, 0);
	io_write32(base + _LPTIM_CFGR, cfgr);
	io_write32(base + _LPTIM_CFGR2, cfgr2);
	io_write32(base + _LPTIM_CR, _LPTIM_CR_ENABLE);
	/* LPTIM_CR_CNTSTRT can be set only when the LPTIM is enable */
	io_write32(base + _LPTIM_CR, _LPTIM_CR_ENABLE | _LPTIM_CR_CNTSTRT);

	return TEE_SUCCESS;
}

#ifdef CFG_EMBED_DTB
static uint32_t get_param(const void *params, unsigned int step)
{
	const fdt32_t *param_dt = (fdt32_t *)params;

	assert(param_dt);

	return fdt32_to_cpu(*(param_dt + step));
}
#else
static uint32_t get_param(const void *params, unsigned int step)
{
	const uint32_t *tim_param = (uint32_t *)params;

	assert(tim_param);

	return tim_params[step];
}
#endif

static TEE_Result
stm32_lptimer_counter_set_config(struct counter_device *counter,
				 const void *param,
				 int len, void **config)
{
	struct lptimer_device *lpt_dev = counter_priv(counter);
	struct lptimer_driver_data *ddata = lpt_dev->ddata;
	struct stm32_lptimer_config *conf;
	struct lptimer_ext_input_cfg *ext_input;
	struct lptimer_ext_trigger_cfg *ext_trigger;

	if (!param)
		return TEE_ERROR_BAD_PARAMETERS;

	if (len < 4 || len > 5)
		return TEE_ERROR_BAD_PARAMETERS;

	conf = calloc(1, sizeof(*conf));
	if (!conf)
		return TEE_ERROR_OUT_OF_MEMORY;

	ext_trigger = &conf->ext_trigger;
	ext_trigger->polarity = get_param(param, 0);
	ext_trigger->mux = get_param(param, 1);
	ext_trigger->num = 1;

	if (ext_trigger->mux >= ddata->nb_ext_trigger) {
		free(conf);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	DMSG("external trigger pol:%"PRId8" mux:%"PRId8,
	     ext_trigger->polarity, ext_trigger->mux);

	/* external-input = < polarity mux1 mux2 > */
	ext_input = &conf->ext_input;
	ext_input->num = len - 3;
	ext_input->polarity = get_param(param, 2);
	ext_input->mux[0] = get_param(param, 3);

	if (ext_input->num == 2)
		ext_input->mux[1] = get_param(param, 4);

	if (ext_input->mux[0] >= ddata->nb_ext_input ||
	    ext_input->mux[1] >= ddata->nb_ext_input) {
		free(conf);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	DMSG("external input nb:%"PRIu8" pol:%"PRIu8" mux[0]:%"PRIu8" mux[1]:%"PRIu8,
	     ext_input->num, ext_input->polarity,
	     ext_input->mux[0], ext_input->mux[1]);

	*config = conf;

	return TEE_SUCCESS;
}

static void stm32_lptimer_counter_release_config(void *config)
{
	struct lptimer_config *conf = (struct lptimer_config *)config;

	free(conf);
}

static const struct counter_ops stm32_lpt_counter_ops = {
	.start = stm32_lpt_counter_start,
	.stop = stm32_lpt_counter_stop,
	.get_value = stm32_lpt_counter_get_value,
	.set_alarm = stm32_lpt_counter_set_alarm,
	.cancel_alarm = stm32_lpt_counter_cancel_alarm,
	.set_config = stm32_lptimer_counter_set_config,
	.release_config = stm32_lptimer_counter_release_config,
};

static enum itr_return stm32_lptimer_itr(struct itr_handler *h)
{
	struct lptimer_device *lpt_dev = h->data;
	uintptr_t base = lpt_dev->pdata.base;
	uint32_t isr = 0;

	/* counter may get stopped from the callback, keep the clock */
	if (clk_enable(lpt_dev->pdata.clock))
		panic();

	isr = io_read32(base + _LPTIM_ISR);

	if (isr & _LPTIM_IXX_CMPM) {
		uint32_t ticks = 0;
		void *priv = lpt_dev->counter_dev->alarm.priv;

		if (_stm32_lpt_counter_get_value(lpt_dev, &ticks))
			EMSG("Failed to get counter value");

		lpt_dev->counter_dev->alarm.callback(ticks, priv);
	}

	io_write32(base + _LPTIM_ICR, isr);

	clk_disable(lpt_dev->pdata.clock);

	return ITRR_HANDLED;
}

static TEE_Result stm32_lptimer_set_driverdata(struct lptimer_device *lpt_dev)
{
	struct lptimer_driver_data *ddata = lpt_dev->ddata;
	uintptr_t base = lpt_dev->pdata.base;
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t regval = 0;

	res = clk_enable(lpt_dev->pdata.clock);
	if (res)
		return res;

	regval = io_read32(base + _LPTIM_HWCFGR);
	ddata->nb_ext_input = _LPTIM_FLD_GET(_LPTIM_HWCFGR_CFG1, regval);
	ddata->nb_ext_trigger = _LPTIM_FLD_GET(_LPTIM_HWCFGR_CFG2, regval);
	ddata->encoder = _LPTIM_FLD_GET(_LPTIM_HWCFGR_CFG3, regval) != 0;

	regval = io_read8(base + _LPTIM_VERR);

	DMSG("LPTIM version %"PRIu32".%"PRIu32,
	     _LPTIM_FLD_GET(_LPTIM_VERR_MAJREV, regval),
	     _LPTIM_FLD_GET(_LPTIM_VERR_MINREV, regval));

	DMSG("HW cap: ext_input:%"PRIu8" ext trigger:%"PRIu8" encoder:%s",
	     ddata->nb_ext_input, ddata->nb_ext_trigger,
	     ddata->encoder ? "true" : "false");

	clk_disable(lpt_dev->pdata.clock);

	return TEE_SUCCESS;
}

#ifdef CFG_EMBED_DTB
static const char * const stm32_lptimer_counter_compat[] = {
	"st,stm32-lptimer-counter",
	"st,stm32mp25-lptimer-counter",
};

struct dt_id_attr {
	/* The effective size of the array is meaningless here */
	fdt32_t id_attr[1];
};

static TEE_Result stm32_lptimer_parse_fdt(struct lptimer_device *lpt_dev)
{
	struct stm32_lptimer_platdata *pdata = &lpt_dev->pdata;
	struct dt_node_info dt_info = { };
	struct io_pa_va base = { };
	const void *fdt = lpt_dev->fdt;
	int node = lpt_dev->node;
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int i = 0;
	int len = 0;

	assert(lpt_dev && lpt_dev->fdt);

	_fdt_fill_device_info(fdt, &dt_info, node);
	if (dt_info.status == DT_STATUS_DISABLED ||
	    dt_info.reg == DT_INFO_INVALID_REG ||
	    dt_info.clock == DT_INFO_INVALID_CLOCK ||
	    dt_info.interrupt == DT_INFO_INVALID_INTERRUPT)
		return -LPTIM_ERR_INVAL;

	pdata->name = fdt_get_name(fdt, node, &len);
	pdata->phandle = fdt_get_phandle(fdt, node);
	base.pa = dt_info.reg;
	pdata->base = io_pa_or_va_secure(&base, 1);
	pdata->irq = dt_info.interrupt;

	res = clk_dt_get_by_index(fdt, node, 0, &pdata->clock);
	if (res)
		return res;

	res = stm32_lptimer_set_driverdata(lpt_dev);
	if (res)
		return res;

	node = fdt_subnode_offset(fdt, node, "counter");
	if (node >= 0 && _fdt_get_status(fdt, node) != DT_STATUS_DISABLED) {
		for (i = 0; i < ARRAY_SIZE(stm32_lptimer_counter_compat); i++) {
			const char *compat = stm32_lptimer_counter_compat[i];

			if (!fdt_node_check_compatible(fdt, node, compat)) {
				pdata->mode = LPTIMER_MODE_COUNTER;
				break;
			}
		}
	}

	return 0;
}

__weak
TEE_Result stm32_lptimer_get_platdata(struct stm32_lptimer_platdata *pdata __unused)
{
	/* In DT config, the platform datas are fill by DT file */
	return TEE_SUCCESS;
}

#else
static TEE_Result stm32_lptimer_parse_fdt(struct lptimer_device *lpt_dev __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}

/*
 * This function could be overridden by platform to define
 * pdata of lptimer driver
 */
__weak TEE_Result stm32_lptimer_get_platdata(struct stm32_lptimer_platdata *pdata __unused)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}
#endif /*CFG_EMBED_DTB*/

static struct lptimer_device *stm32_lptimer_alloc(void)
{
	struct lptimer_device *lpt_dev = NULL;
	struct lptimer_driver_data *lpt_ddata = NULL;

	lpt_dev = calloc(1, sizeof(*lpt_dev));
	lpt_ddata = calloc(1, sizeof(*lpt_ddata));

	if (lpt_dev && lpt_ddata) {
		lpt_dev->ddata = lpt_ddata;
		return lpt_dev;
	}

	free(lpt_dev);
	free(lpt_ddata);

	return NULL;
}

static void stm32_lptimer_free(struct lptimer_device *lpt_dev)
{
	if (lpt_dev) {
		itr_free(lpt_dev->itr);

		if (lpt_dev->counter_dev)
			counter_dev_free(lpt_dev->counter_dev);

		free(lpt_dev->ddata);
		free(lpt_dev);
	}
}

static TEE_Result probe_lpt_device(struct lptimer_device *lpt_dev)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	assert(lpt_dev);

	res = stm32_lptimer_get_platdata(&lpt_dev->pdata);
	if (res)
		return res;

	res = stm32_lptimer_parse_fdt(lpt_dev);
	if (res)
		return res;

	lpt_dev->itr = itr_alloc_add(lpt_dev->pdata.irq, stm32_lptimer_itr,
				     ITRF_TRIGGER_LEVEL, lpt_dev);
	if (!lpt_dev->itr)
		return TEE_ERROR_OUT_OF_MEMORY;

	itr_enable(lpt_dev->itr->it);

	if (lpt_dev->pdata.mode & LPTIMER_MODE_COUNTER) {
		lpt_dev->counter_dev = counter_dev_alloc();
		lpt_dev->counter_dev->ops = &stm32_lpt_counter_ops;
		lpt_dev->counter_dev->max_ticks = _LPTIM_CNT_MASK;
		lpt_dev->counter_dev->priv = lpt_dev;
		lpt_dev->counter_dev->name = lpt_dev->pdata.name;
		lpt_dev->counter_dev->phandle = lpt_dev->pdata.phandle;

		res = counter_dev_register(lpt_dev->counter_dev);
	}

	return res;
}

static TEE_Result stm32_lptimer_probe(const void *fdt, int node,
				      const void *compat_data __unused)
{
	struct lptimer_device *lpt_dev = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	lpt_dev = stm32_lptimer_alloc();
	if (!lpt_dev)
		return TEE_ERROR_OUT_OF_MEMORY;

	lpt_dev->fdt = fdt;
	lpt_dev->node = node;
	lpt_dev->cdata = compat_data;
	res = probe_lpt_device(lpt_dev);

	if (res)
		stm32_lptimer_free(lpt_dev);

	return res;
}

static const struct lptimer_compat_data lptimer_cdata = {
	.ier_wr_disabled = true,
};

static const struct lptimer_compat_data lptimer_mp25_cdata = {
	.ier_wr_disabled = false,
};

static const struct dt_device_match stm32_lptimer_match_table[] = {
	{
		.compatible = "st,stm32-lptimer",
		.compat_data = &lptimer_cdata,
	},
	{
		.compatible = "st,stm32mp25-lptimer",
		.compat_data = &lptimer_mp25_cdata,
	},
	{ }
};

DEFINE_DT_DRIVER(stm32_lptimer_dt_driver) = {
	.name = "stm32-lptimer",
	.match_table = stm32_lptimer_match_table,
	.probe = stm32_lptimer_probe,
};

