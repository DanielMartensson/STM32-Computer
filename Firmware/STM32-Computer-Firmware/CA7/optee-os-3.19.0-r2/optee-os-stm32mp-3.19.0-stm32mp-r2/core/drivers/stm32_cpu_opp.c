// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2022-2024, STMicroelectronics
 */

#include <config.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/regulator.h>
#include <drivers/stm32_cpu_opp.h>
#ifdef CFG_STM32MP13
#include <drivers/stm32mp1_pwr.h>
#endif
#include <initcall.h>
#include <io.h>
#include <kernel/boot.h>
#include <kernel/dt.h>
#include <kernel/mutex.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <libfdt.h>
#ifdef CFG_SCMI_SCPFW
#include <scmi_agent_configuration.h>
#endif
#include <stm32_util.h>
#include <trace.h>
#include <util.h>

/*
 * struct cpu_dvfs - CPU DVFS registered operating points
 * @freq_khz: CPU frequency in kilohertz (kHz)
 * @volt_mv: CPU voltage level in millivolts (mV)
 */
struct cpu_dvfs {
	unsigned int freq_khz;
	unsigned int volt_mv;
};

/*
 * struct cpu_opp - CPU operating point
 *
 * @current_opp: Index of current CPU operating point in @dvfs array
 * @opp_count: Number of cells of @dvfs
 * @clock: CPU clock handle
 * @rdev: CPU regulator supply handle
 * @dvfs: Arrays of the supported CPU operating points
 * @scp_clock: Clock instance exposed to scp-firmware SCMI DVFS
 * @scp_rdev: Regulator instance exposed to scp-firmware SCMI DVFS
 * @cpu_opp_levels_mv: Helper array for struct regul_ops::list_voltages
 * @default_opp_freq: Operating point frequency to use during initialization
 */
struct cpu_opp {
	unsigned int current_opp;
	unsigned int opp_count;
	struct clk *clock;
	struct rdev *rdev;
	struct cpu_dvfs *dvfs;
#ifdef CFG_SCPFW_MOD_DVFS
	struct clk scp_clock;
	struct rdev scp_rdev;
	uint16_t *cpu_opp_levels_mv;
	unsigned int default_opp_freq;
#endif
};

static struct cpu_opp cpu_opp;

#ifndef CFG_SCPFW_MOD_DVFS
/* Mutex for protecting CPU OPP changes */
static struct mutex cpu_opp_mu = MUTEX_INITIALIZER;
#endif

#define MPU_RAM_LOW_SPEED_THRESHOLD 1320

#ifndef CFG_SCPFW_MOD_DVFS
size_t stm32_cpu_opp_count(void)
{
	return cpu_opp.opp_count;
}
#endif

unsigned int stm32_cpu_opp_level(size_t opp_index)
{
	assert(opp_index < cpu_opp.opp_count);

	return cpu_opp.dvfs[opp_index].freq_khz;
}

static TEE_Result _set_opp_clk_rate(unsigned int opp)
{
#ifdef CFG_STM32MP15
	return stm32mp1_set_opp_khz(cpu_opp.dvfs[opp].freq_khz);
#else
	return clk_set_rate(cpu_opp.clock, cpu_opp.dvfs[opp].freq_khz * 1000UL);
#endif
}

static TEE_Result opp_set_voltage(struct rdev *rdev, uint16_t volt_mv)
{
	return regulator_set_voltage(rdev, volt_mv);
}

static bool opp_voltage_is_supported(struct rdev *rdev, uint16_t volt_mv)
{
	uint16_t min_mv = 0;
	uint16_t max_mv = 0;

	regulator_get_range(rdev, &min_mv, &max_mv);

	if (volt_mv < min_mv || volt_mv > max_mv)
		return false;

	return true;
}

static TEE_Result set_clock_then_voltage(unsigned int opp)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	if (_set_opp_clk_rate(opp)) {
		EMSG("Failed to set clock to %ukHz",
		     cpu_opp.dvfs[opp].freq_khz);
		return TEE_ERROR_GENERIC;
	}

#ifdef CFG_STM32MP13
	if (cpu_opp.dvfs[opp].volt_mv <= MPU_RAM_LOW_SPEED_THRESHOLD)
		io_setbits32(stm32_pwr_base(), PWR_CR1_MPU_RAM_LOW_SPEED);
#endif

	res = opp_set_voltage(cpu_opp.rdev, cpu_opp.dvfs[opp].volt_mv);
	if (res) {
		unsigned int current_opp = cpu_opp.current_opp;

		if (current_opp == cpu_opp.opp_count)
			panic();

		if (_set_opp_clk_rate(current_opp))
			EMSG("Failed to restore clock");

		return res;
	}

	return TEE_SUCCESS;
}

static TEE_Result set_voltage_then_clock(unsigned int opp)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	res = opp_set_voltage(cpu_opp.rdev, cpu_opp.dvfs[opp].volt_mv);
	if (res)
		return res;

#ifdef CFG_STM32MP13
	if (cpu_opp.dvfs[opp].volt_mv > MPU_RAM_LOW_SPEED_THRESHOLD)
		io_clrbits32(stm32_pwr_base(), PWR_CR1_MPU_RAM_LOW_SPEED);
#endif

	if (_set_opp_clk_rate(opp)) {
		unsigned int current_opp = cpu_opp.current_opp;
		unsigned int previous_volt = 0U;

		EMSG("Failed to set clock");

		if (current_opp == cpu_opp.opp_count)
			panic();

		previous_volt = cpu_opp.dvfs[current_opp].volt_mv;

		opp_set_voltage(cpu_opp.rdev, previous_volt);

		return TEE_ERROR_GENERIC;
	}

	return TEE_SUCCESS;
}

#ifdef CFG_SCPFW_MOD_DVFS
/* Expose a CPU clock instance for scp-firmware DVFS module */
static TEE_Result set_cpu_rate(struct clk *clk __unused, unsigned long rate,
			       unsigned long parent_rate __unused)
{
	unsigned int __maybe_unused khz = rate / 1000;

#ifdef CFG_STM32MP15
	res = stm32mp1_set_opp_khz(khz);
	if (res)
		return res;
#else
	if (rate != clk_get_rate(cpu_opp.clock))
		return TEE_ERROR_GENERIC;
#endif

	return TEE_SUCCESS;
}

static unsigned long read_cpu_rate(struct clk *clk __unused,
				   unsigned long parent_rate)
{
	return parent_rate;
}

static TEE_Result get_cpu_rates_array(struct clk *clk __unused,
				      size_t start_index, unsigned long *rates,
				      size_t *nb_elts)
{
	size_t rates_cells = *nb_elts;
	size_t opp = 0;

	if (start_index >= cpu_opp.opp_count)
		return TEE_ERROR_BAD_PARAMETERS;

	*nb_elts = cpu_opp.opp_count - start_index;

	if (!rates || rates_cells < *nb_elts)
		return TEE_ERROR_SHORT_BUFFER;

	for (opp = start_index; opp < cpu_opp.opp_count; opp++)
		rates[opp] = (unsigned long)stm32_cpu_opp_level(opp) * 1000;

	return TEE_SUCCESS;
}

static const struct clk_ops stm32_cpu_opp_clk_ops = {
	.get_rate = read_cpu_rate,
	.set_rate = set_cpu_rate,
	.get_rates_array = get_cpu_rates_array,
};

/*
 * Expose a regulator for PSU (Power Supply Unit) instance used
 * by scp-firmware DVFS module
 */
static TEE_Result set_regu_state(const struct regul_desc *desc __unused,
				 bool enabled __unused)
{
	return TEE_SUCCESS;
}

static TEE_Result read_regu_state(const struct regul_desc *desc __unused,
				  bool *enabled)
{
	*enabled = true;

	return TEE_SUCCESS;
}

static TEE_Result set_regu_voltage(const struct regul_desc *desc __unused,
				   uint16_t mv)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint16_t cur_mv = 0;

	res = regulator_get_voltage(cpu_opp.rdev, &cur_mv);
	if (res)
		return res;

	if (mv == cur_mv)
		return TEE_SUCCESS;

#ifdef CFG_STM32MP13
	if (cur_mv > MPU_RAM_LOW_SPEED_THRESHOLD &&
	    mv <= MPU_RAM_LOW_SPEED_THRESHOLD)
		io_setbits32(stm32_pwr_base(), PWR_CR1_MPU_RAM_LOW_SPEED);
#endif

	res = opp_set_voltage(cpu_opp.rdev, mv);
	if (res)
		return res;

#ifdef CFG_STM32MP13
	if (cur_mv <= MPU_RAM_LOW_SPEED_THRESHOLD &&
	    mv > MPU_RAM_LOW_SPEED_THRESHOLD)
		io_clrbits32(stm32_pwr_base(), PWR_CR1_MPU_RAM_LOW_SPEED);
#endif

	return TEE_SUCCESS;
}

static TEE_Result read_regu_voltage(const struct regul_desc *desc __unused,
				    uint16_t *mv)
{
	return regulator_get_voltage(cpu_opp.rdev, mv);
}

static TEE_Result list_regu_voltages(const struct regul_desc *desc __unused,
				     uint16_t **levels, size_t *count)
{
	if (levels)
		*levels = cpu_opp.cpu_opp_levels_mv;
	if (count)
		*count = cpu_opp.opp_count;

	return TEE_SUCCESS;
}

static const struct regul_ops stm32_cpu_opp_regu = {
	.set_state = set_regu_state,
	.get_state = read_regu_state,
	.set_voltage = set_regu_voltage,
	.get_voltage = read_regu_voltage,
	.list_voltages = list_regu_voltages,
};

static const struct regul_desc stm32_scp_rdev_desc = {
	.ops = &stm32_cpu_opp_regu,
};

static int cmp_cpu_opp_by_freq(const void *a, const void *b)
{
	const struct cpu_dvfs *opp_a = a;
	const struct cpu_dvfs *opp_b = b;

	if (opp_a->freq_khz == opp_b->freq_khz)
		return CMP_TRILEAN(opp_a->volt_mv, opp_b->volt_mv);
	else
		return CMP_TRILEAN(opp_a->freq_khz, opp_b->freq_khz);
}

static uint16_t min_cpu_voltage(struct cpu_dvfs *dvfs, size_t count)
{
	unsigned int min_mv = UINT_MAX;
	size_t n = 0;

	for (n = 0; n < count; n++)
		min_mv = MIN(min_mv, dvfs[n].volt_mv);

	assert(min_mv <= UINT16_MAX);
	return min_mv;
}

static uint16_t max_cpu_voltage(struct cpu_dvfs *dvfs, size_t count)
{
	unsigned int max_mv = 0;
	size_t n = 0;

	for (n = 0; n < count; n++)
		max_mv = MAX(max_mv, dvfs[n].volt_mv);

	assert(max_mv > 0 && max_mv <= UINT16_MAX);
	return max_mv;
}

static void setup_scmi_server_resources(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int *dvfs_khz = NULL;
	unsigned int *dvfs_mv = NULL;
	size_t opp = 0;
	struct cpu_dvfs *sorted_dvfs = NULL;
	unsigned int scpfw_default_opp_index = UINT_MAX;

	/*
	 * Sort operating points by increasing frequencies as expected by
	 * SCP-firmware DVFS module.
	 */
	sorted_dvfs = calloc(cpu_opp.opp_count, sizeof(*sorted_dvfs));
	assert(sorted_dvfs);
	memcpy(sorted_dvfs, cpu_opp.dvfs,
	       cpu_opp.opp_count * sizeof(*sorted_dvfs));

	qsort(sorted_dvfs, cpu_opp.opp_count, sizeof(*sorted_dvfs),
	      cmp_cpu_opp_by_freq);

	/* Setup  a clock for scp-firmare DVFS module clock instance */
	cpu_opp.scp_clock = (struct clk){
		.ops = &stm32_cpu_opp_clk_ops,
		.name = "stm32-cpu-opp",
		.rate = cpu_opp.dvfs[cpu_opp.current_opp].freq_khz * 1000,
#ifndef CFG_STM32MP15
		.flags = CLK_SET_RATE_PARENT,
		.parent = cpu_opp.clock,
#endif
	};

	res = clk_register(&cpu_opp.scp_clock);
	if (res)
		panic();

	/* Setup a regulator for scp-firmare DVFS module PSU instance */
	cpu_opp.scp_rdev = (struct rdev){
		.desc = &stm32_scp_rdev_desc,
		.min_mv = min_cpu_voltage(sorted_dvfs, cpu_opp.opp_count),
		.max_mv = max_cpu_voltage(sorted_dvfs, cpu_opp.opp_count),
		.reg_name = "stm32-cpu-opp",
	};

	cpu_opp.cpu_opp_levels_mv = calloc(cpu_opp.opp_count,
					   sizeof(*cpu_opp.cpu_opp_levels_mv));
	assert(cpu_opp.cpu_opp_levels_mv);

	for (opp = 0; opp < cpu_opp.opp_count; opp++)
		cpu_opp.cpu_opp_levels_mv[opp] = sorted_dvfs[opp].volt_mv;

	/* Feed SCP-firmware with CPU DVFS configuration data */
	dvfs_khz = calloc(cpu_opp.opp_count, sizeof(*dvfs_khz));
	dvfs_mv = calloc(cpu_opp.opp_count, sizeof(*dvfs_mv));
	assert(dvfs_khz && dvfs_mv);

	for (opp = 0; opp < cpu_opp.opp_count; opp++) {
		dvfs_khz[opp] = sorted_dvfs[opp].freq_khz;
		dvfs_mv[opp] = sorted_dvfs[opp].volt_mv;
		if (cpu_opp.default_opp_freq == dvfs_khz[opp])
			scpfw_default_opp_index = opp;
	}
	if (scpfw_default_opp_index == UINT_MAX)
		panic();

	free(sorted_dvfs);

	res = scmi_scpfw_cfg_add_dvfs(0 /*agent*/, 0 /*channel*/, 0 /*domain*/,
				      &cpu_opp.scp_rdev, &cpu_opp.scp_clock,
				      dvfs_khz, dvfs_mv, cpu_opp.opp_count,
				      scpfw_default_opp_index);
	if (res)
		panic();

	free(dvfs_khz);
	free(dvfs_mv);
}
#else /*CFG_SCPFW_MOD_DVFS*/
static void setup_scmi_server_resources(void)
{
}

TEE_Result stm32_cpu_opp_set_level(unsigned int level)
{
	unsigned int current_level = 0;
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int opp = 0;

	mutex_lock(&cpu_opp_mu);

	/* Perf level relates straight to CPU frequency in kHz */
	current_level = cpu_opp.dvfs[cpu_opp.current_opp].freq_khz;

	if (level == current_level) {
		mutex_unlock(&cpu_opp_mu);
		return TEE_SUCCESS;
	}

	for (opp = 0; opp < cpu_opp.opp_count; opp++)
		if (level == cpu_opp.dvfs[opp].freq_khz)
			break;

	if (opp == cpu_opp.opp_count) {
		mutex_unlock(&cpu_opp_mu);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (level < current_level)
		res = set_clock_then_voltage(opp);
	else
		res = set_voltage_then_clock(opp);

	if (!res)
		cpu_opp.current_opp = opp;

	mutex_unlock(&cpu_opp_mu);

	return res;
}

TEE_Result stm32_cpu_opp_read_level(unsigned int *level)
{
	if (cpu_opp.current_opp >= cpu_opp.opp_count)
		return TEE_ERROR_BAD_STATE;

	*level = cpu_opp.dvfs[cpu_opp.current_opp].freq_khz;

	return TEE_SUCCESS;
}
#endif /*CFG_SCPFW_MOD_DVFS*/

#ifdef CFG_STM32MP13
static TEE_Result cpu_opp_pm(enum pm_op op, unsigned int pm_hint __unused,
			     const struct pm_callback_handle *hdl __unused)
{
	unsigned long clk_cpu = 0;
	unsigned int opp = 0;

	assert(op == PM_OP_SUSPEND || op == PM_OP_RESUME);

#ifdef CFG_SCPFW_MOD_DVFS
	if (op == PM_OP_SUSPEND) {
		/*
		 * When CFG_SCPFW_MOD_DVFS is enabled, save CPU OPP on suspend
		 * for restoration at resmue. If CPU is not in an expected
		 * OPP state, fallback to default OPP at PM resume.
		 */
		unsigned int cur_khz = 0;
		uint16_t cur_mv = 0;

		cur_khz = clk_get_rate(cpu_opp.clock) / 1000;

		if (regulator_get_voltage(cpu_opp.rdev, &cur_mv))
			panic();

		/* Get current OPP or fallback to default */
		for (opp = 0; opp < cpu_opp.opp_count; opp++) {
			if (cur_khz == cpu_opp.dvfs[opp].freq_khz  &&
			    cur_mv == cpu_opp.dvfs[opp].volt_mv)
				break;
		}
		if (opp >= cpu_opp.opp_count) {
			EMSG("Unexpected OPP state, select default");

			for (opp = 0; opp < cpu_opp.opp_count; opp++)
				if (cpu_opp.default_opp_freq ==
				    cpu_opp.dvfs[opp].freq_khz)
					break;

			if (opp >= cpu_opp.opp_count)
				panic();
		}

		DMSG("Suspend to OPP %u", opp);
		cpu_opp.current_opp = opp;

		return TEE_SUCCESS;
	}
#endif

	if (op == PM_OP_RESUME) {
		opp = cpu_opp.current_opp;

		DMSG("Resume to OPP %u", opp);

		clk_cpu = clk_get_rate(cpu_opp.clock);
		assert(clk_cpu);
		if (cpu_opp.dvfs[opp].freq_khz * 1000U >= clk_cpu)
			return set_voltage_then_clock(opp);
		else
			return set_clock_then_voltage(opp);
	}

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(cpu_opp_pm);
#endif

static TEE_Result stm32_cpu_opp_is_supported(const void *fdt, int subnode)
{
	const fdt32_t *cuint32 = NULL;
	uint32_t opp = 0;

	cuint32 = fdt_getprop(fdt, subnode, "opp-supported-hw", NULL);

	if (!cuint32) {
		DMSG("Can't find property opp-supported-hw");
		return TEE_ERROR_GENERIC;
	}

	opp = fdt32_to_cpu(*cuint32);
	if (!stm32mp_supports_cpu_opp(opp)) {
		DMSG("Not supported opp-supported-hw %#"PRIx32, opp);
		return TEE_ERROR_GENERIC;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_cpu_opp_get_dt_subnode(const void *fdt, int node)
{
	const fdt64_t *cuint64 = NULL;
	const fdt32_t *cuint32 = NULL;
	uint64_t freq_khz = 0;
	uint64_t freq_khz_opp_def = 0;
	uint32_t volt_mv = 0;
	unsigned long clk_cpu = 0;
	unsigned int i = 0;
	int subnode = -1;
	TEE_Result res = TEE_ERROR_GENERIC;
	bool opp_default = false;

	fdt_for_each_subnode(subnode, fdt, node)
		if (!stm32_cpu_opp_is_supported(fdt, subnode))
			cpu_opp.opp_count++;

	cpu_opp.dvfs = calloc(1, cpu_opp.opp_count * sizeof(*cpu_opp.dvfs));
	if (!cpu_opp.dvfs)
		return TEE_ERROR_OUT_OF_MEMORY;

	cpu_opp.current_opp = cpu_opp.opp_count;

	fdt_for_each_subnode(subnode, fdt, node) {
		if (stm32_cpu_opp_is_supported(fdt, subnode))
			continue;

		cuint64 = fdt_getprop(fdt, subnode, "opp-hz", NULL);
		if (!cuint64) {
			DMSG("Missing opp-hz");
			return TEE_ERROR_GENERIC;
		}

		freq_khz = fdt64_to_cpu(*cuint64) / 1000ULL;
		if (freq_khz > (uint64_t)UINT32_MAX) {
			DMSG("Invalid opp-hz %"PRIu64, freq_khz);
			return TEE_ERROR_GENERIC;
		}

		cuint32 = fdt_getprop(fdt, subnode, "opp-microvolt", NULL);
		if (!cuint32) {
			DMSG("Missing opp-microvolt");
			return TEE_ERROR_GENERIC;
		}

		volt_mv = fdt32_to_cpu(*cuint32) / 1000U;

		/* skip OPP when voltage is not supported */
		if (!opp_voltage_is_supported(cpu_opp.rdev, volt_mv)) {
			DMSG("Skip OPP %"PRIu64"kHz/%"PRIu32"mV",
			     freq_khz, volt_mv);
			cpu_opp.opp_count--;
			continue;
		}

		cpu_opp.dvfs[i].freq_khz = freq_khz;
		cpu_opp.dvfs[i].volt_mv = volt_mv;

		DMSG("Found OPP %u (%"PRIu64"kHz/%"PRIu32"mV) from DT",
		     i, freq_khz, volt_mv);

		if (fdt_getprop(fdt, subnode, "st,opp-default", NULL) &&
		    freq_khz > freq_khz_opp_def) {
			opp_default = true;
			cpu_opp.current_opp = i;
			freq_khz_opp_def = freq_khz;
		}

		i++;
	}

	/* Erreur when "st,opp-default" is not present */
	if (!opp_default)
		return TEE_ERROR_GENERIC;

	/* Select the max "st,opp-default" node as current OPP */
#ifdef CFG_SCPFW_MOD_DVFS
	cpu_opp.default_opp_freq = freq_khz_opp_def;
#endif
	clk_cpu = clk_get_rate(cpu_opp.clock);
	assert(clk_cpu);
	if (freq_khz_opp_def * 1000U > clk_cpu)
		res = set_voltage_then_clock(cpu_opp.current_opp);
	else
		res = set_clock_then_voltage(cpu_opp.current_opp);

	if (res)
		return res;

#ifdef CFG_STM32MP13
	register_pm_driver_cb(cpu_opp_pm, NULL, "cpu-opp");
#endif

	return TEE_SUCCESS;
}

static TEE_Result get_cpu_parent(const void *fdt)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	const struct fdt_property *prop = NULL;
	int node = fdt_path_offset(fdt, "/cpus/cpu@0");

	if (node < 0) {
		EMSG("cannot find /cpus/cpu@0 node");
		panic();
	}

	res = clk_dt_get_by_index(fdt, node, 0, &cpu_opp.clock);
	if (res)
		return res;

	prop = fdt_get_property(fdt, node, "operating-points-v2", NULL);
	if (!prop) {
		EMSG("OPP table not defined by CPU");
		return TEE_ERROR_GENERIC;
	}

	cpu_opp.rdev = regulator_get_by_supply_name(fdt, node, "cpu");
	if (!cpu_opp.rdev)
		return TEE_ERROR_DEFER_DRIVER_INIT;

	return TEE_SUCCESS;
}

static TEE_Result
stm32_cpu_opp_init(const void *fdt, int node, const void *compat_data __unused)
{
	TEE_Result res = TEE_SUCCESS;
	uint16_t cpu_voltage __maybe_unused = 0;

	res = get_cpu_parent(fdt);
	if (res)
		return res;

#ifdef CFG_STM32MP15
	res = regulator_get_voltage(cpu_opp.rdev, &cpu_voltage);
	if (res)
		return res;

	if (stm32mp1_clk_compute_all_pll1_settings(cpu_voltage))
		panic();
#endif

	res = stm32_cpu_opp_get_dt_subnode(fdt, node);
	if (res)
		return res;

	if (IS_ENABLED(CFG_SCPFW_MOD_DVFS))
		setup_scmi_server_resources();

	return TEE_SUCCESS;
}

static const struct dt_device_match stm32_cpu_opp_match_table[] = {
	{ .compatible = "operating-points-v2" },
	{ }
};

DEFINE_DT_DRIVER(stm32_opp_dt_driver) = {
	.name = "stm32-cpu-opp",
	.match_table = stm32_cpu_opp_match_table,
	.probe = &stm32_cpu_opp_init,
};
