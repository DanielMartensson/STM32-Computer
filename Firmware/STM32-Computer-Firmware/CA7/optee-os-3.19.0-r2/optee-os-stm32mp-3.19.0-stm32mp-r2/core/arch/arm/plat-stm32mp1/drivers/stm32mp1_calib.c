// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2018-2021, STMicroelectronics
 */

#include <arm.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/counter.h>
#include <drivers/stm32mp_dt_bindings.h>
#ifdef CFG_STM32MP13
#include <drivers/stm32mp13_rcc.h>
#endif
#ifdef CFG_STM32MP15
#include <drivers/stm32mp1_rcc.h>
#endif
#include <initcall.h>
#include <io.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/interrupt.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/pseudo_ta.h>
#include <kernel/timer.h>
#include <libfdt.h>
#include <limits.h>
#include <mm/core_memprot.h>
#include <stm32_pta_calib.h>
#include <stm32_util.h>
#include <string.h>

#define OSC_CALIBRATION_COMPAT		"st,osc-calibration"
#define CSI_CALIBRATION_COMPAT		"st,csi-cal"
#define HSI_CALIBRATION_COMPAT		"st,hsi-cal"
#define CALIBRATION_TIMEOUT_US		100000
#define PRESCAL_HSI			10
#define PRESCAL_CSI			7

#define RCC_HSICFGR_HSICAL_MASK_LIMITED	GENMASK_32(24, 16)

enum osc_calibration {
	HSI_CAL = 0,
	CSI_CAL,
	MAX_CAL,
};

/* List of forbidden values for HSI and CSI */
static const uint16_t fbv_hsi[] = {
	512, 480, 448, 416, 384, 352, 320, 288,
	256, 224, 192, 160, 128, 96, 64, 32, 0,
};

static const uint16_t fbv_csi[] = {
	256, 240, 224, 208, 192, 176, 160, 144,
	128, 112, 96, 80, 64, 48, 32, 16, 0,
};

struct stm32mp1_trim_boundary_t {
	/* Max boundary trim value around forbidden value */
	unsigned int max;
	/* Min boundary trim value around forbidden value */
	unsigned int min;
};

struct counter_calib {
	struct counter_device *counter;
	struct clk *clock;
	void *config;
};

struct stm32mp1_clk_cal {
	const char *compatible;
	const char *name;
	const uint16_t *fbv;
	unsigned int cal_ref;
	int trim_max;
	int trim_min;
	unsigned int boundary_max;
	unsigned long ref_freq;
	unsigned int freq_margin;
	bool is_used;
	bool force_init;
	unsigned int prescaler;
	struct counter_calib cnt_cal;
	int (*init_cal)(struct stm32mp1_clk_cal *cal);
	void (*set_trim)(unsigned int cal, unsigned int cal_ref);
	unsigned int (*get_cal)(void);
	struct stm32mp1_trim_boundary_t boundary[16];
};

struct stm32mp1_osc_cal {
	uint32_t period_ms;
	struct stm32mp1_clk_cal cal[MAX_CAL];
};

static void stm32mp1_clk_hsi_set_trim(unsigned int cal, unsigned int cal_ref);
static unsigned int stm32mp1_clk_hsi_get_cal(void);
static void stm32mp1_clk_csi_set_trim(unsigned int cal, unsigned int cal_ref);
static unsigned int stm32mp1_clk_csi_get_cal(void);

static int init_hsi_calibration(struct stm32mp1_clk_cal *hsi_cal);
static int init_csi_calibration(struct stm32mp1_clk_cal *csi_cal);

static struct stm32mp1_clk_cal hsi_calib_config = {
	.compatible = HSI_CALIBRATION_COMPAT,
	.fbv = fbv_hsi,
	.trim_max = 63,
	.trim_min = -64,
	.ref_freq = 0,
	.freq_margin = 5,
	.prescaler = PRESCAL_HSI,
	.init_cal = init_hsi_calibration,
	.set_trim = stm32mp1_clk_hsi_set_trim,
	.get_cal = stm32mp1_clk_hsi_get_cal,
};

static struct stm32mp1_clk_cal csi_calib_config = {
	.compatible = CSI_CALIBRATION_COMPAT,
	.fbv = fbv_csi,
	.trim_max = 15,
	.trim_min = -16,
	.ref_freq = 0,
	.freq_margin = 8,
	.prescaler = PRESCAL_CSI,
	.init_cal = init_csi_calibration,
	.set_trim = stm32mp1_clk_csi_set_trim,
	.get_cal = stm32mp1_clk_csi_get_cal,
};

static struct stm32mp1_osc_cal *osc_cal_device;

static unsigned int stm32mp1_clk_hsi_get_cal(void)
{
	return (io_read32(stm32_rcc_base() + RCC_HSICFGR) &
		RCC_HSICFGR_HSICAL_MASK_LIMITED) >> RCC_HSICFGR_HSICAL_SHIFT;
}

static void stm32mp1_clk_hsi_set_trim(unsigned int cal, unsigned int cal_ref)
{
	int clk_trim = 0;
	uint32_t trim = 0;

	clk_trim = cal - cal_ref;
	trim = ((uint32_t)clk_trim << RCC_HSICFGR_HSITRIM_SHIFT) &
		RCC_HSICFGR_HSITRIM_MASK;

	io_clrsetbits32(stm32_rcc_base() + RCC_HSICFGR,
			RCC_HSICFGR_HSITRIM_MASK, trim);
}

static unsigned int stm32mp1_clk_csi_get_cal(void)
{
	return (io_read32(stm32_rcc_base() + RCC_CSICFGR) &
		RCC_CSICFGR_CSICAL_MASK) >> RCC_CSICFGR_CSICAL_SHIFT;
}

static void stm32mp1_clk_csi_set_trim(unsigned int cal, unsigned int cal_ref)
{
	uint32_t trim_max = RCC_CSICFGR_CSITRIM_MASK >>
			    (RCC_CSICFGR_CSITRIM_SHIFT + 1);
	uint32_t clk_trim = trim_max + cal - cal_ref + 1;
	uint32_t trim = (clk_trim << RCC_CSICFGR_CSITRIM_SHIFT) &
		RCC_CSICFGR_CSITRIM_MASK;

	io_clrsetbits32(stm32_rcc_base() + RCC_CSICFGR,
			RCC_CSICFGR_CSITRIM_MASK, trim);
}

static unsigned long get_freq(struct stm32mp1_clk_cal *clk_cal)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int ticks = 0;
	unsigned long freq = 0;
	struct counter_calib *cnt = &clk_cal->cnt_cal;

	res = counter_get_value(cnt->counter, &ticks);
	if (res && !ticks)
		panic();

	/*
	 * Calibration uses timer in input mode.
	 * It uses the default timer clock to increase
	 * timer counter
	 * Warning: If a prescaler is set in timer, rate
	 * must be adapted accordingly.
	 */
	freq = (clk_get_rate(cnt->clock) / ticks) << clk_cal->prescaler;

	FMSG("%s Freq is %lu", clk_cal->name, freq);

	if (!freq)
		panic("Freq calculation error");

	return freq;
}

static unsigned int trim_increase(struct stm32mp1_clk_cal *clk_cal,
				  unsigned int cal)
{
	struct stm32mp1_trim_boundary_t *boundary = NULL;
	unsigned int new_cal = 0;
	int i = 0;

	/* By default: last calibration value */
	new_cal = cal;

	/* Start from Lowest cal value */
	for (i = (int)clk_cal->boundary_max - 1; i >= 0; i--) {
		boundary = &clk_cal->boundary[i];

		if (cal < boundary->min) {
			new_cal = boundary->min;
			break;
		}

		if (cal >= boundary->min && cal < boundary->max) {
			new_cal = cal + 1;
			break;
		}
	}

	return new_cal;
}

static unsigned int trim_decrease(struct stm32mp1_clk_cal *clk_cal,
				  unsigned int cal)
{
	struct stm32mp1_trim_boundary_t *boundary = NULL;
	unsigned int new_cal = 0;
	unsigned int i = 0;

	/* By default: last calibration value */
	new_cal = cal;

	/* Start from Highest cal value */
	for (i = 0; i < clk_cal->boundary_max; i++) {
		boundary = &clk_cal->boundary[i];

		if (cal > boundary->max) {
			new_cal = boundary->max;
			break;
		}

		if (cal > boundary->min && cal <= boundary->max) {
			new_cal = cal - 1;
			break;
		}
	}

	return new_cal;
}

static void osc_calibration(struct stm32mp1_clk_cal *clk_cal)
{
	unsigned long margin = (clk_cal->ref_freq *
				clk_cal->freq_margin) / 1000;
	unsigned long min = clk_cal->ref_freq - margin;
	unsigned long max = clk_cal->ref_freq + margin;
	unsigned long freq = get_freq(clk_cal);
	int trim = 0;
	int new_trim = 0;
	unsigned long conv = 0;
	unsigned long min_conv = ULONG_MAX;
	uint64_t timeout_ref = 0;

	if (freq >= min && freq <= max && clk_cal->force_init)
		return;

	clk_cal->force_init = true;
	trim = clk_cal->get_cal();
	timeout_ref = timeout_init_us(CALIBRATION_TIMEOUT_US);
	do {
		if (freq < clk_cal->ref_freq)
			new_trim = trim_increase(clk_cal, trim);
		else
			new_trim = trim_decrease(clk_cal, trim);

		clk_cal->set_trim(new_trim, clk_cal->cal_ref);
		freq = get_freq(clk_cal);
		conv = (clk_cal->ref_freq < freq) ?
			freq - clk_cal->ref_freq : clk_cal->ref_freq - freq;
		if (conv < min_conv) {
			min_conv = conv;
			trim = new_trim;
		} else if (conv == min_conv) {
			break;
		}

		if (timeout_elapsed(timeout_ref)) {
			EMSG("%s Timeout: %lu , trim %i\n",
			     clk_cal->name, freq, trim);
			break;
		}

	} while (conv == min_conv);

	DMSG("Calibration : Freq %lu , trim %i\n", freq, trim);

	clk_cal->set_trim(trim, clk_cal->cal_ref);
	freq = get_freq(clk_cal);
	if (freq < min || freq > max)
		EMSG("%s Freq out of range: %lu , trim %i\n", clk_cal->name,
		     freq, trim);

}

static void save_trim(struct stm32mp1_clk_cal *clk_cal,
		      unsigned int i, unsigned int max, unsigned int min)
{
	clk_cal->boundary[i].max = max;
	clk_cal->boundary[i].min = min;
}

static int trim_find_prev_boundary(struct stm32mp1_clk_cal *clk_cal,
				   unsigned int x1)
{
	unsigned int x = x1;
	unsigned long freq = 0;

	clk_cal->set_trim(x1 + 1, clk_cal->cal_ref);
	freq = get_freq(clk_cal);

	while (x >= (clk_cal->cal_ref + clk_cal->trim_min)) {
		x--;
		clk_cal->set_trim(x, clk_cal->cal_ref);

		if (get_freq(clk_cal) <= freq)
			break;
	};

	return x;
}

static void trim_table_init(struct stm32mp1_clk_cal *clk_cal)
{
	const uint16_t *trim_fbv = clk_cal->fbv;
	unsigned int min = 0;
	unsigned int max = 0;
	int boundary = 0;
	int i = 0;

	max = clk_cal->cal_ref + clk_cal->trim_max;
	min = clk_cal->cal_ref + clk_cal->trim_min;

	while (trim_fbv[i]) {
		unsigned int x = 0;
		unsigned int x1 = trim_fbv[i];
		unsigned int x2 = trim_fbv[i + 1];

		if (max <= x2 || min >= x1) {
			i++;
			if (boundary != 0)
				goto out;

			continue;
		}

		/* Take forbiden value + 1 */
		x2 = x2 + 1;
		if (x2 < min)
			x2 = min;

		if (boundary == 0) {
			/* Save first boundary */
			save_trim(clk_cal, boundary, max, x2);
			boundary++;
			i++;
			continue;
		}

		x = trim_find_prev_boundary(clk_cal, x1);
		/* Save boundary values */
		save_trim(clk_cal, boundary, x - 1, x2);
		boundary++;
		i++;
	};
out:
	clk_cal->boundary_max = boundary;
}

static void stm32mp_start_clock_calib(struct stm32mp1_osc_cal *calib)
{
	unsigned int i = 0;

	for (i = 0; i < ARRAY_SIZE(osc_cal_device->cal); i++) {
		if (calib->cal[i].is_used) {
			TEE_Result res = TEE_ERROR_GENERIC;
			struct counter_calib *cnt_cal = &calib->cal[i].cnt_cal;

			/* Start associated counter */
			res = counter_start(cnt_cal->counter,
					    cnt_cal->config);
			if (res)
				panic();

			osc_calibration(&calib->cal[i]);

			res = counter_stop(cnt_cal->counter);
			if (res)
				panic();
		} else {
			if (HSI_CAL == i)
				DMSG("HSI clock calibration not supported");
			else
				DMSG("CSI clock calibration not supported");
		}
	}
}

static void set_cal_counter(struct counter_calib *cnt_cal,
			    const void *fdt, int node)
{
	int phandle = 0;
	int counter_node = 0;
	const int *cuint = NULL;
	struct clk *clock = NULL;
	TEE_Result res = TEE_SUCCESS;

	cnt_cal->counter = fdt_counter_get(fdt, node, &cnt_cal->config);
	if (!cnt_cal->counter || !cnt_cal->config)
		panic();

	cuint = fdt_getprop(fdt, node, "counter", NULL);
	if (!cuint)
		panic();

	phandle = fdt32_to_cpu(*cuint);
	counter_node = fdt_node_offset_by_phandle(fdt, phandle);
	if (!counter_node)
		panic();

	res = clk_dt_get_by_index(fdt, counter_node, 0, &clock);
	if (res)
		panic();

	cnt_cal->clock = clock;

	FMSG("Counter freq is %li", clk_get_rate(cnt_cal->clock));
}

static int init_hsi_calibration(struct stm32mp1_clk_cal *hsi_cal)
{
	struct clk *hsi_clk = NULL;

	hsi_clk = stm32mp_rcc_clock_id_to_clk(CK_HSI);

	hsi_cal->ref_freq = clk_get_rate(hsi_clk);
	hsi_cal->cal_ref = stm32mp1_clk_hsi_get_cal();
	trim_table_init(hsi_cal);
	hsi_cal->set_trim(hsi_cal->cal_ref, hsi_cal->cal_ref);

	return 0;
}

static int init_csi_calibration(struct stm32mp1_clk_cal *csi_cal)
{
	struct clk *csi_clk = NULL;

	csi_clk = stm32mp_rcc_clock_id_to_clk(CK_CSI);

	csi_cal->ref_freq = clk_get_rate(csi_clk);
	csi_cal->cal_ref = stm32mp1_clk_csi_get_cal();
	trim_table_init(csi_cal);
	csi_cal->set_trim(csi_cal->cal_ref, csi_cal->cal_ref);

	return 0;
}

static TEE_Result
stm32mp1_calib_pm(enum pm_op op, unsigned int pm_hint __unused,
		  const struct pm_callback_handle *hdl __unused)
{
	unsigned int i = 0;

	if (op == PM_OP_RESUME)
		return TEE_SUCCESS;

	/* SUSPEND */
	for (i = 0; i < ARRAY_SIZE(osc_cal_device->cal); i++) {
		struct stm32mp1_clk_cal *cal = &osc_cal_device->cal[i];

		if (cal->is_used)
			cal->force_init = false;
	}

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(stm32mp1_calib_pm);

static TEE_Result init_stm32mp1_calib(void)
{
	void *fdt = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;
	int node = 0;
	int subnode = 0;
	unsigned int i = 0;

	fdt = get_embedded_dt();
	if (!fdt)
		panic();

	node = fdt_node_offset_by_compatible(fdt, -1, OSC_CALIBRATION_COMPAT);
	if (node < 0)
		panic();

	osc_cal_device = calloc(1, sizeof(*osc_cal_device));
	if (!osc_cal_device)
		return TEE_ERROR_OUT_OF_MEMORY;

	osc_cal_device->cal[HSI_CAL] = hsi_calib_config;
	osc_cal_device->cal[CSI_CAL] = csi_calib_config;

	/*  Parse subnode to find the calibration */
	fdt_for_each_subnode(subnode, fdt, node) {
		const char *compat = NULL;
		int status = _fdt_get_status(fdt, subnode);

		if (status == DT_STATUS_DISABLED)
			continue;

		compat = fdt_stringlist_get(fdt, subnode,
					    "compatible", 0, NULL);
		if (!compat)
			continue;

		for (i = 0; i < ARRAY_SIZE(osc_cal_device->cal); i++) {
			struct stm32mp1_clk_cal *cal = &osc_cal_device->cal[i];
			struct counter_calib *cnt_cal = &cal->cnt_cal;

			if (strcmp(cal->compatible, compat) != 0)
				continue;

			DMSG("Calibration %s enabled",
			     fdt_get_name(fdt, subnode, NULL));
			cal->is_used = true;
			cal->name = fdt_get_name(fdt, subnode, NULL);

			/* Start counter */
			set_cal_counter(cnt_cal, fdt, subnode);

			res = counter_start(cnt_cal->counter,
					    cnt_cal->config);
			if (res)
				panic();

			res = cal->init_cal(cal);
			if (res) {
				EMSG("Calibration %s failed", cal->name);
				panic();
			}

			res = counter_stop(cnt_cal->counter);
			if (res)
				panic();
		}
	}

	stm32mp_start_clock_calib(osc_cal_device);

	register_pm_driver_cb(stm32mp1_calib_pm, NULL, "service-calib");

	return TEE_SUCCESS;
}

service_init(init_stm32mp1_calib);

typedef TEE_Result (*ta_func)(struct ts_session *s, uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS]);

static TEE_Result request_calibration(struct ts_session *s __unused,
				      uint32_t param_types __unused,
				      TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	assert(osc_cal_device);

	stm32mp_start_clock_calib(osc_cal_device);

	return TEE_SUCCESS;
}

static const ta_func ta_funcs[] = {
	[PTA_CALIB_REQUEST] = request_calibration,
};

static TEE_Result calib_invoke_command(void *sess_ctx __unused, uint32_t cmd_id,
				       uint32_t param_types,
				       TEE_Param params[TEE_NUM_PARAMS])
{
	struct ts_session *s = ts_get_calling_session();

	DMSG("Invoking calibration command");

	if (cmd_id < ARRAY_SIZE(ta_funcs) && ta_funcs[cmd_id])
		return ta_funcs[cmd_id](s, param_types, params);

	return TEE_ERROR_NOT_IMPLEMENTED;
}

pseudo_ta_register(.uuid = PTA_CALIB_UUID, .name = "calib",
		   .flags = PTA_DEFAULT_FLAGS,
		   .invoke_command_entry_point = calib_invoke_command);
