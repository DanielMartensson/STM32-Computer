// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2018-2022, STMicroelectronics
 */

#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/counter.h>
#include <drivers/stm32_tim.h>
#include <io.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <util.h>

/* TIM offset register */
#define _TIM_CR1		0x00		/* Control Register 1      */
#define _TIM_CR2		0x04		/* Control Register 2      */
#define _TIM_SMCR		0x08		/* Slave mode control reg  */
#define _TIM_DIER		0x0C		/* DMA/interrupt register  */
#define _TIM_SR			0x10		/* Status register	   */
#define _TIM_EGR		0x14		/* Event Generation Reg    */
#define _TIM_CCMR1		0x18		/* Capt/Comp 1 Mode Reg    */
#define _TIM_CCMR2		0x1C		/* Capt/Comp 2 Mode Reg    */
#define _TIM_CCER		0x20		/* Capt/Comp Enable Reg    */
#define _TIM_CNT		0x24		/* Counter		   */
#define _TIM_PSC		0x28		/* Prescaler               */
#define _TIM_ARR		0x2C		/* Auto-Reload Register    */
#define _TIM_CCR1		0x34		/* Capt/Comp Register 1    */
#define _TIM_CCR2		0x38		/* Capt/Comp Register 2    */
#define _TIM_CCR3		0x3C		/* Capt/Comp Register 3    */
#define _TIM_CCR4		0x40		/* Capt/Comp Register 4    */
#define _TIM_BDTR		0x44		/* Break and Dead-Time Reg */
#define _TIM_DCR		0x48		/* DMA control register    */
#define _TIM_DMAR		0x4C		/* DMA transfer register   */
#define _TIM_AF1		0x60		/* Alt Function Reg 1      */
#define _TIM_AF2		0x64		/* Alt Function Reg 2      */
#define _TIM_TISEL		0x68		/* Input Selection         */

/* TIM_CR1 register fields */
#define _TIM_CR1_CEN		BIT(0)

/* TIM SMCR register fields */
#define _TIM_SMCR_SMS_MASK	GENMASK_32(2, 0)
#define _TIM_SMCR_SMS_SHIFT	0
#define _TIM_SMCR_TS_MASK	GENMASK_32(6, 4)
#define _TIM_SMCR_TS_SHIFT	4
#define _TIM_SMCR_SMS_RESET	BIT(2)
#define _TIM_SMCR_TS_TI1FP1	0x5

/* TIM DIER register fileds */
#define _TIM_DIER_CC1E_MASK	BIT(1)
#define _TIM_DIER_CC1E_SHIFT	1

/* TIM SR register fields */
#define _TIM_SR_CC1IF_MASK	BIT(1)
#define _TIM_SR_CC1IF_SHIFT	1
#define _TIM_SR_CC1OF_MASK	BIT(9)
#define _TIM_SR_CC1OF_SHIFT	9

/* TIM CCMR register fields */
#define _TIM_CCMR1_CC1S_MASK	GENMASK_32(1, 0)
#define _TIM_CCMR1_CC1S_SHIFT	0
#define _TIM_CCMR1_CC1S_TI1	BIT(0)		/* IC1/IC3 selects TI1/TI3 */
#define _TIM_CCMR1_CC1S_TI2	BIT(1)		/* IC1/IC3 selects TI2/TI4 */
#define _TIM_CCMR1_CC2S_TI2	BIT(8)		/* IC2/IC4 selects TI2/TI4 */
#define _TIM_CCMR1_CC2S_TI1	BIT(9)		/* IC2/IC4 selects TI1/TI3 */

/* TIM CNT register fields */
#define _TIM_CNT_MASK		GENMASK_32(15, 0)

/* TIM CCER register fields */
#define _TIM_CCER_CC1E_MASK	BIT(0)
#define _TIM_CCER_CC1E_SHIFT	0
#define _TIM_CCER_CC1P_MASK	BIT(1)
#define _TIM_CCER_CC1P_SHIFT	1

/* TIM TISEL register fields */
#define _TIM_TISEL_TI1SEL_MASK	GENMASK_32(3, 0)
#define _TIM_TISEL_TI1SEL_SHIFT	0
#define _TIM_TISEL_TI2SEL_MASK	GENMASK_32(11, 8)
#define _TIM_TISEL_TI2SEL_SHIFT	8

#define _TIM_COUNTER_MODE	BIT(0)
#define _TIM_TIMEOUT_US		500000U

#define _TIM_FLD_PREP(field, value) (((uint32_t)(value) << (field ## _SHIFT)) & (field ## _MASK))
#define _TIM_FLD_GET(field, value)  (((uint32_t)(value) & (field ## _MASK)) >> (field ## _SHIFT))

/*
 * No common errno between component.
 * Define timer internal errno
 */
#define TIM_ERR_NODEV		19	/* No such device */
#define TIM_ERR_NOTSUP		45	/* Operation not supported */

struct timer_device {
	const void *fdt;
	int node;
	struct stm32_timer_platdata pdata;
	struct counter_device *counter_dev;
};

struct stm32_timer_config {
	uint8_t num;
	uint8_t polarity;
	uint8_t input[2];
};

static int _stm32_tim_counter_get_value(struct timer_device *tim_dev)
{
	uintptr_t base = tim_dev->pdata.base;
	uint32_t cnt = 0;
	uint32_t cnt_prev = 0;
	uint64_t timeout_ref = 0;

	cnt = io_read32(base + _TIM_CCR1);
	do {
		io_clrbits32(base + _TIM_SR, _TIM_SR_CC1IF_MASK);
		cnt_prev = cnt;
		timeout_ref = timeout_init_us(_TIM_TIMEOUT_US);
		while (!timeout_elapsed(timeout_ref))
			if ((io_read32(base + _TIM_SR) & _TIM_SR_CC1IF_MASK))
				break;

		if (!(io_read32(base + _TIM_SR) & _TIM_SR_CC1IF_MASK)) {
			cnt = 0;
			break;
		}

		cnt = io_read32(base + _TIM_CCR1);
	} while (cnt_prev != cnt);

	return cnt;
}

static TEE_Result stm32_tim_counter_get_value(struct counter_device *counter,
					      uint32_t *ticks)
{
	struct timer_device *tim_dev = counter_priv(counter);

	if (!ticks)
		return TEE_ERROR_BAD_PARAMETERS;

	*ticks = _stm32_tim_counter_get_value(tim_dev);

	return TEE_SUCCESS;
}

static TEE_Result stm32_tim_counter_stop(struct counter_device *counter)
{
	struct timer_device *tim_dev = counter_priv(counter);
	uintptr_t base = tim_dev->pdata.base;

	io_write32(base + _TIM_CR1, 0);
	io_write32(base + _TIM_SR, 0);
	io_write32(base + _TIM_DIER, 0);
	io_write32(base + _TIM_CNT, 0);
	io_write32(base + _TIM_SMCR, 0);
	io_write32(base + _TIM_CCER, 0);
	io_write32(base + _TIM_CCMR1, 0);

	clk_disable(tim_dev->pdata.clock);

	return TEE_SUCCESS;
}

static TEE_Result stm32_tim_counter_start(struct counter_device *counter,
					  void *config __maybe_unused)
{
	struct timer_device *tim_dev = counter_priv(counter);
	struct stm32_timer_platdata *pdata = &tim_dev->pdata;
	struct stm32_timer_config *conf = (struct stm32_timer_config *)config;
	uintptr_t base = pdata->base;
	uint32_t tisel = 0;
	uint32_t ccmr1 = 0;
	uint32_t ccer = 0;
	uint32_t smcr = 0;
	uint32_t dier = 0;

	assert(conf);

	if (conf->num) {
		smcr = _TIM_FLD_PREP(_TIM_SMCR_TS, _TIM_SMCR_TS_TI1FP1);
		smcr += _TIM_FLD_PREP(_TIM_SMCR_SMS, _TIM_SMCR_SMS_RESET);
		tisel = _TIM_FLD_PREP(_TIM_TISEL_TI1SEL, conf->input[0]);
		ccmr1 = _TIM_FLD_PREP(_TIM_CCMR1_CC1S, _TIM_CCMR1_CC1S_TI1);
		ccer = _TIM_FLD_PREP(_TIM_CCER_CC1P, conf->polarity);
		ccer += _TIM_FLD_PREP(_TIM_CCER_CC1E, 1);
		dier = _TIM_FLD_PREP(_TIM_DIER_CC1E, 1);
	}

	clk_enable(tim_dev->pdata.clock);

	io_write32(base + _TIM_CR1, 0);
	io_write32(base + _TIM_CR2, 0);
	io_write32(base + _TIM_SR, 0);
	io_write32(base + _TIM_DIER, dier);
	io_write32(base + _TIM_CCMR1, ccmr1);
	io_write32(base + _TIM_CCER, ccer);
	io_write32(base + _TIM_CNT, 0);
	io_write32(base + _TIM_SMCR, smcr);
	io_write32(base + _TIM_TISEL, tisel);
	io_write32(base + _TIM_CR1, _TIM_CR1_CEN);

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
stm32_tim_counter_set_config(struct counter_device *counter __maybe_unused,
			     const void *params,
			     int len, void **config)
{
	struct stm32_timer_config *conf = NULL;

	if (!params || !config)
		return TEE_ERROR_BAD_PARAMETERS;

	if (len < 2 || len > 3)
		return TEE_ERROR_BAD_PARAMETERS;

	conf = calloc(1, sizeof(*conf));
	if (!conf)
		return TEE_ERROR_OUT_OF_MEMORY;

	conf->num = len - 1;
	conf->polarity = get_param(params, 0);
	conf->input[0] = get_param(params, 1);

	if (conf->num == 2)
		conf->input[1] = get_param(params, 2);

	*config = conf;

	return TEE_SUCCESS;
}

static void stm32_tim_counter_release_config(void *config)
{
	struct stm32_timer_config *conf = (struct stm32_timer_config *)config;

	free(conf);
}

static const struct counter_ops stm32_tim_counter_ops = {
	.start = stm32_tim_counter_start,
	.stop = stm32_tim_counter_stop,
	.get_value = stm32_tim_counter_get_value,
	.set_config = stm32_tim_counter_set_config,
	.release_config = stm32_tim_counter_release_config,
};

#ifdef CFG_EMBED_DTB
#define TIMER_COUNTER_COMPAT		"st,stm32-timer-counter"

struct dt_id_attr {
	/* The effective size of the array is meaningless here */
	fdt32_t id_attr[1];
};

static TEE_Result stm32_timer_parse_fdt(struct timer_device *tim_dev)
{
	struct stm32_timer_platdata *pdata = &tim_dev->pdata;
	struct dt_node_info dt_info = { };
	struct io_pa_va base = { };
	const void *fdt = tim_dev->fdt;
	int node = tim_dev->node;
	TEE_Result res = TEE_ERROR_GENERIC;
	int len = 0;

	assert(tim_dev && tim_dev->fdt);

	_fdt_fill_device_info(fdt, &dt_info, node);
	if (dt_info.reg == DT_INFO_INVALID_REG ||
	    dt_info.reg_size == DT_INFO_INVALID_REG_SIZE ||
	    dt_info.clock == DT_INFO_INVALID_CLOCK)
		return TEE_ERROR_BAD_PARAMETERS;

	pdata->name = fdt_get_name(fdt, node, &len);
	pdata->phandle = fdt_get_phandle(fdt, node);
	base.pa = dt_info.reg;
	pdata->base = io_pa_or_va(&base, dt_info.reg_size);
	res = clk_dt_get_by_index(fdt, node, 0, &pdata->clock);
	if (res)
		return res;

	node = fdt_subnode_offset(fdt, node, "counter");
	if (node >= 0 &&
	    !fdt_node_check_compatible(fdt, node, TIMER_COUNTER_COMPAT) &&
	    _fdt_get_status(fdt, node) != DT_STATUS_DISABLED)
		pdata->mode = _TIM_COUNTER_MODE;

	return TEE_SUCCESS;
}

__weak
TEE_Result stm32_timer_get_platdata(struct stm32_timer_platdata *pdata __unused)
{
	/* In DT config, the platform datas are filled by DT file */
	return TEE_SUCCESS;
}

#else
static TEE_Result stm32_timer_parse_fdt(struct timer_device *tim_dev)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}

/*
 * This function could be overridden by platform to define
 * pdata of timer driver
 */
__weak TEE_Result stm32_timer_get_platdata(struct stm32_timer_platdata *pdata)
{
	return TEE_ERROR_NOT_IMPLEMENTED;
}
#endif /*CFG_EMBED_DTB*/

static struct timer_device *stm32_timer_alloc(void)
{
	struct timer_device *tim_dev;

	tim_dev = calloc(1, sizeof(*tim_dev));

	if (tim_dev)
		return tim_dev;

	return NULL;
}

static void stm32_timer_free(struct timer_device *tim_dev)
{
	if (tim_dev && tim_dev->counter_dev)
		counter_dev_free(tim_dev->counter_dev);

	free(tim_dev);
}

static TEE_Result stm32_timer_probe(struct timer_device *tim_dev)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	assert(tim_dev);

	res = stm32_timer_get_platdata(&tim_dev->pdata);
	if (res)
		return res;

	res = stm32_timer_parse_fdt(tim_dev);
	if (res && res != TEE_ERROR_NOT_IMPLEMENTED)
		return res;

	if (tim_dev->pdata.mode & _TIM_COUNTER_MODE) {
		tim_dev->counter_dev = counter_dev_alloc();
		if (!tim_dev->counter_dev)
			return TEE_ERROR_OUT_OF_MEMORY;

		tim_dev->counter_dev->ops = &stm32_tim_counter_ops;
		tim_dev->counter_dev->max_ticks = _TIM_CNT_MASK;
		tim_dev->counter_dev->priv = tim_dev;
		tim_dev->counter_dev->name = tim_dev->pdata.name;
		tim_dev->counter_dev->phandle = tim_dev->pdata.phandle;

		res = counter_dev_register(tim_dev->counter_dev);
		if (res)
			counter_dev_free(tim_dev->counter_dev);
	}

	return res;
}

static TEE_Result stm32_timer_fdt_probe(const void *fdt, int node,
					const void *compat_data __unused)
{
	struct timer_device *tim_dev = NULL;
	int err = 0;

	tim_dev = stm32_timer_alloc();
	if (tim_dev) {
		tim_dev->fdt = fdt;
		tim_dev->node = node;
		err = stm32_timer_probe(tim_dev);
	}

	if (!tim_dev || err)
		stm32_timer_free(tim_dev);

	return TEE_SUCCESS;
}

static const struct dt_device_match stm32_timer_match_table[] = {
	{ .compatible = "st,stm32-timers" },
	{ }
};

DEFINE_DT_DRIVER(timer_dt_driver) = {
	.name = "stm32-timer",
	.match_table = stm32_timer_match_table,
	.probe = &stm32_timer_fdt_probe,
};
