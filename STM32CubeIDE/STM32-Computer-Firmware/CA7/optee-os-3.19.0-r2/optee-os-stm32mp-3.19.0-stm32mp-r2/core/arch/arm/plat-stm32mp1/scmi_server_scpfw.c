// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2022, STMicroelectronics
 */
#include <assert.h>
#include <compiler.h>
#include <config.h>
#include <confine_array_index.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/rstctrl.h>
#include <drivers/scmi-msg.h>
#include <drivers/scmi.h>
#include <drivers/stm32_cpu_opp.h>
#include <drivers/stm32_firewall.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <initcall.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <platform_config.h>
#include <scmi_agent_configuration.h>
#include <scmi_regulator_consumer.h>
#include <stdint.h>
#include <speculation_barrier.h>
#include <stm32_util.h>
#include <string.h>
#include <tee_api_defines.h>
#include <util.h>

#define TIMEOUT_US_1MS		1000

/*
 * struct stm32_scmi_clk - Data for the exposed clock
 * @clock_id: Clock identifier in RCC clock driver
 * @name: Clock string ID exposed to channel
 * @enabled: State of the SCMI clock at initialization
 * @change_rate: SCMMI agent is allowed to change the rate
 * @clk: Clock device manipulated by the SCMI channel
 */
struct stm32_scmi_clk {
	unsigned long clock_id;
	const char *name;
	bool enabled;
	bool change_rate;
	struct clk *clk;
};

/*
 * struct stm32_scmi_rd - Data for the exposed reset controller
 * @reset_id: Reset identifier in RCC reset driver
 * @base: Physical controller address
 * @name: Reset string ID exposed to channel
 * @rstctrl: Reset controller manipulated by the SCMI channel
 * @rstctrl_be: Backend reset controller device
 */
struct stm32_scmi_rd {
	unsigned long reset_id;
	paddr_t base;
	const char *name;
	struct rstctrl *rstctrl;
	struct rstctrl *rstctrl_be;
};

/*
 * struct stm32_scmi_perfd - Data for the exposed performance domains
 * @name: Performance domain string ID (aka name) exposed to channel
 */
struct stm32_scmi_perfd {
	const char *name;
};

#define CLOCK_CELL(_scmi_id, _id, _name, _init_enabled) \
	[_scmi_id] = { \
		.clock_id = _id, \
		.name = _name, \
		.enabled = _init_enabled, \
	}

#define CLOCK_CELL_RATES(_scmi_id, _id, _name, _init_enabled) \
	[_scmi_id] = { \
		.clock_id = _id, \
		.name = _name, \
		.enabled = _init_enabled, \
		.change_rate = true, \
	}

#define RESET_CELL(_scmi_id, _id, _base, _name) \
	[_scmi_id] = { \
		.reset_id = _id, \
		.base = _base, \
		.name = _name, \
	}

#define PERFD_CELL(_scmi_id, _name) \
	[_scmi_id] = { \
		.name = (_name), \
	}

struct channel_resources {
	struct stm32_scmi_clk *clock;
	size_t clock_count;
	struct stm32_scmi_rd *rd;
	size_t rd_count;
	struct stm32_scmi_perfd *perfd;
	size_t perfd_count;
};

#ifdef CFG_STM32MP13
static struct stm32_scmi_clk stm32_scmi_clock[] = {
	CLOCK_CELL(CK_SCMI_HSE, CK_HSE, "ck_hse", true),
	CLOCK_CELL(CK_SCMI_HSI, CK_HSI, "ck_hsi", true),
	CLOCK_CELL(CK_SCMI_CSI, CK_CSI, "ck_csi", true),
	CLOCK_CELL(CK_SCMI_LSE, CK_LSE, "ck_lse", true),
	CLOCK_CELL(CK_SCMI_LSI, CK_LSI, "ck_lsi", true),
	CLOCK_CELL(CK_SCMI_HSE_DIV2, CK_HSE_DIV2, "clk-hse-div2", true),
	CLOCK_CELL(CK_SCMI_PLL2_Q, PLL2_Q, "pll2_q", true),
	CLOCK_CELL(CK_SCMI_PLL2_R, PLL2_R, "pll2_r", true),
	CLOCK_CELL(CK_SCMI_PLL3_P, PLL3_P, "pll3_p", true),
	CLOCK_CELL(CK_SCMI_PLL3_Q, PLL3_Q, "pll3_q", true),
	CLOCK_CELL(CK_SCMI_PLL3_R, PLL3_R, "pll3_r", true),
	CLOCK_CELL(CK_SCMI_PLL4_P, PLL4_P, "pll4_p", true),
	CLOCK_CELL_RATES(CK_SCMI_PLL4_Q, PLL4_Q, "pll4_q", true),
	CLOCK_CELL(CK_SCMI_PLL4_R, PLL4_R, "pll4_r", true),
	CLOCK_CELL(CK_SCMI_MPU, CK_MPU, "ck_mpu", true),
	CLOCK_CELL(CK_SCMI_AXI, CK_AXI, "ck_axi", true),
	CLOCK_CELL(CK_SCMI_MLAHB, CK_MLAHB, "ck_mlahb", true),
	CLOCK_CELL(CK_SCMI_CKPER, CK_PER, "ck_per", true),
	CLOCK_CELL(CK_SCMI_PCLK1, PCLK1, "pclk1", true),
	CLOCK_CELL(CK_SCMI_PCLK2, PCLK2, "pclk2", true),
	CLOCK_CELL(CK_SCMI_PCLK3, PCLK3, "pclk3", true),
	CLOCK_CELL(CK_SCMI_PCLK4, PCLK4, "pclk4", true),
	CLOCK_CELL(CK_SCMI_PCLK5, PCLK5, "pclk5", true),
	CLOCK_CELL(CK_SCMI_PCLK6, PCLK6, "pclk6", true),
	CLOCK_CELL(CK_SCMI_CKTIMG1, CK_TIMG1, "timg1_ck", true),
	CLOCK_CELL(CK_SCMI_CKTIMG2, CK_TIMG2, "timg2_ck", true),
	CLOCK_CELL(CK_SCMI_CKTIMG3, CK_TIMG3, "timg3_ck", true),
	CLOCK_CELL(CK_SCMI_RTC, RTC, "ck_rtc", true),
	CLOCK_CELL(CK_SCMI_RTCAPB, RTCAPB, "rtcapb", true),
	CLOCK_CELL(CK_SCMI_BSEC, BSEC, "bsec", true),
};
#endif

#ifdef CFG_STM32MP15
static struct stm32_scmi_clk stm32_scmi_clock[] = {
	CLOCK_CELL(CK_SCMI_HSE, CK_HSE, "ck_hse", true),
	CLOCK_CELL(CK_SCMI_HSI, CK_HSI, "ck_hsi", true),
	CLOCK_CELL(CK_SCMI_CSI, CK_CSI, "ck_csi", true),
	CLOCK_CELL(CK_SCMI_LSE, CK_LSE, "ck_lse", true),
	CLOCK_CELL(CK_SCMI_LSI, CK_LSI, "ck_lsi", true),
	CLOCK_CELL(CK_SCMI_PLL2_Q, PLL2_Q, "pll2_q", true),
	CLOCK_CELL(CK_SCMI_PLL2_R, PLL2_R, "pll2_r", true),
	CLOCK_CELL_RATES(CK_SCMI_MPU, CK_MPU, "ck_mpu", true),
	CLOCK_CELL(CK_SCMI_AXI, CK_AXI, "ck_axi", true),
	CLOCK_CELL(CK_SCMI_BSEC, BSEC, "bsec", true),
	CLOCK_CELL(CK_SCMI_CRYP1, CRYP1, "cryp1", false),
	CLOCK_CELL(CK_SCMI_GPIOZ, GPIOZ, "gpioz", false),
	CLOCK_CELL(CK_SCMI_HASH1, HASH1, "hash1", false),
	CLOCK_CELL(CK_SCMI_I2C4, I2C4_K, "i2c4_k", false),
	CLOCK_CELL(CK_SCMI_I2C6, I2C6_K, "i2c6_k", false),
	CLOCK_CELL(CK_SCMI_IWDG1, IWDG1, "iwdg1", false),
	CLOCK_CELL(CK_SCMI_RNG1, RNG1_K, "rng1_k", true),
	CLOCK_CELL(CK_SCMI_RTC, RTC, "ck_rtc", true),
	CLOCK_CELL(CK_SCMI_RTCAPB, RTCAPB, "rtcapb", true),
	CLOCK_CELL(CK_SCMI_SPI6, SPI6_K, "spi6_k", false),
	CLOCK_CELL(CK_SCMI_USART1, USART1_K, "usart1_k", false),
};
#endif

#ifdef CFG_STM32MP13
static struct stm32_scmi_rd stm32_scmi_reset[] = {
	RESET_CELL(RST_SCMI_LTDC, LTDC_R, LTDC_BASE, "ltdc"),
	RESET_CELL(RST_SCMI_MDMA, MDMA_R, MDMA_BASE, "mdma"),
};
#endif

#ifdef CFG_STM32MP15
static struct stm32_scmi_rd stm32_scmi_reset[] = {
	RESET_CELL(RST_SCMI_SPI6, SPI6_R, SPI6_BASE, "spi6"),
	RESET_CELL(RST_SCMI_I2C4, I2C4_R, I2C4_BASE, "i2c4"),
	RESET_CELL(RST_SCMI_I2C6, I2C6_R, I2C6_BASE, "i2c6"),
	RESET_CELL(RST_SCMI_USART1, USART1_R, USART1_BASE, "usart1"),
	RESET_CELL(RST_SCMI_STGEN, STGEN_R, STGEN_BASE, "stgen"),
	RESET_CELL(RST_SCMI_GPIOZ, GPIOZ_R, GPIOZ_BASE, "gpioz"),
	RESET_CELL(RST_SCMI_CRYP1, CRYP1_R, CRYP1_BASE, "cryp1"),
	RESET_CELL(RST_SCMI_HASH1, HASH1_R, HASH1_BASE, "hash1"),
	RESET_CELL(RST_SCMI_RNG1, RNG1_R, RNG1_BASE, "rng1"),
	RESET_CELL(RST_SCMI_MDMA, MDMA_R, MDMA_BASE, "mdma"),
	RESET_CELL(RST_SCMI_MCU, MCU_R, 0, "mcu"),
	RESET_CELL(RST_SCMI_MCU_HOLD_BOOT, MCU_HOLD_BOOT_R, 0,
		   "mcu_hold_boot"),
};
#endif

static const struct channel_resources scmi_channel[] = {
	[0] = {
		.clock = stm32_scmi_clock,
		.clock_count = ARRAY_SIZE(stm32_scmi_clock),
		.rd = stm32_scmi_reset,
		.rd_count = ARRAY_SIZE(stm32_scmi_reset),
	},
};

/*
 * Platform clocks exposed with SCMI
 */
static struct clk plat_clocks[ARRAY_SIZE(stm32_scmi_clock)];

static TEE_Result plat_scmi_clk_get_rates_steps(struct clk *clk,
						unsigned long *min,
						unsigned long *max,
						unsigned long *step)
{
	struct stm32_scmi_clk *scmi_clk = clk->priv;

	if (scmi_clk->change_rate) {
		*min = 0;
		*max = UINT32_MAX;
		*step = 0;
	} else {
		*min = clk->rate;
		*max = *min;
		*step = 0;
	}

	return TEE_SUCCESS;
}

static const struct clk_ops plat_scmi_clk_ops = {
	.get_rates_steps = plat_scmi_clk_get_rates_steps,
};

static void get_scmi_clocks(void)
{
	size_t i = 0;

	for (i = 0; i < ARRAY_SIZE(stm32_scmi_clock); i++) {
		struct stm32_scmi_clk *scmi_clk = stm32_scmi_clock + i;
		struct clk *clk = plat_clocks + i;

		clk->ops = &plat_scmi_clk_ops;
		clk->priv = scmi_clk;
		clk->name = scmi_clk->name;
		clk->parent = stm32mp_rcc_clock_id_to_clk(scmi_clk->clock_id);
		assert(clk->parent);

		if (scmi_clk->change_rate)
			clk->flags = CLK_SET_RATE_PARENT;

		clk->flags |= CLK_DUTY_CYCLE_PARENT;

		if (clk_register(clk))
			panic();

		scmi_clk->clk = clk;
	}
}

/*
 * Platform reset controllers exposed with SCMI
 */
static struct rstctrl plat_resets[ARRAY_SIZE(stm32_scmi_reset)];

static TEE_Result plat_scmi_reset_assert_level(struct rstctrl *rstctrl,
					       unsigned int to_us)
{
	const struct stm32_firewall_cfg nsec_cfg[] = {
		{ FWLL_NSEC_PRIV_RW | FWLL_MASTER(0) },
		{ }, /* Null terminated */
	};
	int index = rstctrl - plat_resets;
	paddr_t domain_iobase = 0;

	assert(index >= 0 && (size_t)index < ARRAY_SIZE(stm32_scmi_reset));
	domain_iobase = stm32_scmi_reset[index].base;

	if (domain_iobase &&
	    stm32_firewall_check_access(domain_iobase, 0, nsec_cfg))
		return TEE_ERROR_ACCESS_DENIED;

	return rstctrl_assert_to(stm32_scmi_reset[index].rstctrl_be, to_us);
}

static TEE_Result plat_scmi_reset_deassert_level(struct rstctrl *rstctrl,
						 unsigned int to_us)
{
	const struct stm32_firewall_cfg nsec_cfg[] = {
		{ FWLL_NSEC_PRIV_RW | FWLL_MASTER(0) },
		{ }, /* Null terminated */
	};
	int index = rstctrl - plat_resets;
	paddr_t domain_iobase = 0;

	assert(index >= 0 && (size_t)index < ARRAY_SIZE(stm32_scmi_reset));
	domain_iobase = stm32_scmi_reset[index].base;

	if (domain_iobase &&
	    stm32_firewall_check_access(domain_iobase, 0, nsec_cfg))
		return TEE_ERROR_ACCESS_DENIED;

	return rstctrl_deassert_to(stm32_scmi_reset[index].rstctrl_be, to_us);
}

static const char *plat_scmi_reset_get_name(struct rstctrl *rstctrl)
{
	int index = rstctrl - plat_resets;

	assert(index >= 0 && (size_t)index < ARRAY_SIZE(stm32_scmi_reset));

	return rstctrl_name(stm32_scmi_reset[index].rstctrl_be);
}

static const struct rstctrl_ops plat_scmi_reset_ops = {
	.assert_level = plat_scmi_reset_assert_level,
	.deassert_level = plat_scmi_reset_deassert_level,
	.get_name = plat_scmi_reset_get_name,
};

static void get_scmi_resets(void)
{
	size_t i = 0;

	for (i = 0; i < ARRAY_SIZE(stm32_scmi_reset); i++) {
		struct stm32_scmi_rd *scmi_reset = stm32_scmi_reset + i;
		struct rstctrl *rstctrl = plat_resets + i;

		rstctrl->ops = &plat_scmi_reset_ops;

		scmi_reset->rstctrl_be =
			stm32mp_rcc_reset_id_to_rstctrl(scmi_reset->reset_id);
		assert(scmi_reset->rstctrl_be);

		scmi_reset->rstctrl = rstctrl;
	}
}

/*
 * Build the structure used to initialize SCP firmware.
 * We target a description from the DT so SCP firmware configuration
 * but we currently rely on local data (clock, reset) and 2 DT node
 * (voltage domain/regulators and DVFS).
 *
 * At early_init initcall level, prepare scpfw_cfg agent and channel parts.
 * At driver_init_late initcall level, get clock and reset devices.
 * When scmi-regulator-consumer driver probes, it adds the regulators.
 * When CPU OPP driver probesn it adds the DVFS part.
 */
static struct scpfw_config scpfw_cfg;

struct scpfw_config *scmi_scpfw_get_configuration(void)
{
	assert(scpfw_cfg.agent_count);

	return &scpfw_cfg;
}

static TEE_Result scmi_scpfw_cfg_early_init(void)
{
	/* There is 1 agent with 1 channel */

	scpfw_cfg.agent_count = 1;
	scpfw_cfg.agent_config = calloc(1, sizeof(*scpfw_cfg.agent_config));

	scpfw_cfg.agent_config[0].name = "agent";
	scpfw_cfg.agent_config[0].agent_id = 1;
	scpfw_cfg.agent_config[0].channel_count = ARRAY_SIZE(scmi_channel);
	scpfw_cfg.agent_config[0].channel_config =
		calloc(scpfw_cfg.agent_config[0].channel_count,
		       sizeof(*scpfw_cfg.agent_config[0].channel_config));

	return TEE_SUCCESS;
}

early_init(scmi_scpfw_cfg_early_init);

static TEE_Result scmi_scpfw_cfg_init(void)
{
	struct scpfw_channel_config *channel_cfg = NULL;
	size_t i = 0;
	size_t j = 0;

	get_scmi_clocks();
	get_scmi_resets();

	for (i = 0; i < ARRAY_SIZE(scmi_channel); i++) {
		/* Clock and reset are exposed to agent#0/channel#0 */
		channel_cfg = scpfw_cfg.agent_config[i].channel_config;
		channel_cfg->name = "channel";

		channel_cfg->clock_count = ARRAY_SIZE(stm32_scmi_clock);
		if (channel_cfg->clock_count) {
			channel_cfg->clock =
				calloc(channel_cfg->clock_count,
				       sizeof(*channel_cfg->clock));

			for (j = 0; j < channel_cfg->clock_count; j++) {
				channel_cfg->clock[j] = (struct scmi_clock){
					.name = stm32_scmi_clock[j].name,
					.clk = stm32_scmi_clock[j].clk,
					.enabled = stm32_scmi_clock[j].enabled,
				};
			}
		}

		channel_cfg->reset_count = ARRAY_SIZE(stm32_scmi_reset);
		if (channel_cfg->reset_count) {
			channel_cfg->reset =
				calloc(channel_cfg->reset_count,
				       sizeof(*channel_cfg->reset));

			for (j = 0; j < channel_cfg->reset_count; j++) {
				channel_cfg->reset[j] = (struct scmi_reset){
					.name = stm32_scmi_reset[j].name,
					.rstctrl = stm32_scmi_reset[j].rstctrl,
				};
			}
		}
	}

	/* DVFS will be populated from cpu_opp driver */

	/* Regulators will be populated from scmi_regulator_consumer driver */

	return TEE_SUCCESS;
}

driver_init_late(scmi_scpfw_cfg_init);

void scmi_scpfw_release_configuration(void)
{
	struct scpfw_channel_config *channel_cfg = NULL;
	struct scpfw_agent_config *agent_cfg = NULL;
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;

	for (i = 0; i < scpfw_cfg.agent_count; i++) {
		agent_cfg = scpfw_cfg.agent_config + i;

		for (j = 0; j < agent_cfg->channel_count; j++) {
			channel_cfg = agent_cfg->channel_config + j;

			free(channel_cfg->clock);
			free(channel_cfg->reset);
			free(channel_cfg->voltd);

			for (k = 0; k < channel_cfg->perfd_count; k++) {
				free(channel_cfg->perfd[k].dvfs_opp_khz);
				free(channel_cfg->perfd[k].dvfs_opp_mv);
			}

			free(channel_cfg->perfd);
		}

		free(agent_cfg->channel_config);
	}

	free(scpfw_cfg.agent_config);
}

static unsigned int voltd_id_count(struct scmi_server_regu *regu,
				   size_t regu_count)
{
	unsigned int max_id = 0;
	size_t i = 0;

	for (i = 0; i < regu_count; i++)
		max_id = MAX(max_id, regu[i].domain_id);

	return max_id + 1;
}

TEE_Result scmi_scpfw_cfg_add_regu(unsigned int agent_id,
				   unsigned int channel_id,
				   struct scmi_server_regu *regu,
				   size_t regu_count)
{
	struct scpfw_channel_config *channel_cfg = NULL;
	struct scpfw_agent_config *agent_cfg = NULL;
	size_t i = 0;

	agent_cfg = scpfw_cfg.agent_config + agent_id;
	channel_cfg = agent_cfg->channel_config + channel_id;

	if (!regu_count)
		return TEE_SUCCESS;

	if (agent_id >= scpfw_cfg.agent_count ||
	    channel_id >= agent_cfg->channel_count)
		return TEE_ERROR_BAD_PARAMETERS;

	if (channel_cfg->voltd) {
		EMSG("Voltage domain already loaded: agent %u, channel %u",
		     agent_id, channel_id);
		panic();
	}

	channel_cfg->voltd_count = voltd_id_count(regu, regu_count);
	channel_cfg->voltd = calloc(channel_cfg->voltd_count,
				    sizeof(*channel_cfg->voltd));
	assert(channel_cfg->voltd);

	for (i = 0; i < regu_count; i++) {
		unsigned int domain_id = regu[i].domain_id;

		if (regu[i].rdev) {
			channel_cfg->voltd[domain_id] = (struct scmi_voltd){
				.name = regu[i].rdev->reg_name,
				.rdev = regu[i].rdev,
				.enabled = regu[i].enabled,
			};
		}
	}

	return TEE_SUCCESS;
}

TEE_Result scmi_scpfw_cfg_add_dvfs(unsigned int agent_id,
				   unsigned int channel_id,
				   unsigned int domain_id,
				   struct rdev *regulator, struct clk *clk,
				   unsigned int *dvfs_khz,
				   unsigned int *dvfs_mv, size_t dvfs_count,
				   size_t initial_index)
{
	struct scpfw_channel_config *channel_cfg = NULL;
	struct scpfw_agent_config *agent_cfg = NULL;
	unsigned int *dvfs_opp_khz = NULL;
	unsigned int *dvfs_opp_mv = NULL;

	agent_cfg = scpfw_cfg.agent_config + agent_id;
	channel_cfg = agent_cfg->channel_config + channel_id;

	if (agent_id >= scpfw_cfg.agent_count ||
	    channel_id >= agent_cfg->channel_count || !dvfs_count)
		return TEE_ERROR_BAD_PARAMETERS;

	dvfs_opp_khz = calloc(dvfs_count, sizeof(*dvfs_opp_khz));
	dvfs_opp_mv = calloc(dvfs_count, sizeof(*dvfs_opp_mv));
	if (!dvfs_opp_khz || !dvfs_opp_mv) {
		free(dvfs_opp_khz);
		free(dvfs_opp_mv);

		return TEE_ERROR_OUT_OF_MEMORY;
	}
	memcpy(dvfs_opp_khz, dvfs_khz, dvfs_count * sizeof(*dvfs_opp_khz));
	memcpy(dvfs_opp_mv, dvfs_mv, dvfs_count * sizeof(*dvfs_opp_mv));

	if (channel_cfg->perfd_count <= domain_id) {
		void *ptr = realloc(channel_cfg->perfd,
				    (domain_id + 1) *
				    sizeof(*channel_cfg->perfd));

		if (!ptr) {
			free(dvfs_opp_khz);
			free(dvfs_opp_mv);
			return TEE_ERROR_OUT_OF_MEMORY;
		}

		channel_cfg->perfd = ptr;
		memset(channel_cfg->perfd + channel_cfg->perfd_count, 0,
		       (domain_id + 1 - channel_cfg->perfd_count) *
		       sizeof(*channel_cfg->perfd));

		channel_cfg->perfd_count = domain_id + 1;
	}

	if (channel_cfg->perfd->dvfs_opp_khz) {
		EMSG("Perf domain already loaded: agent %u, channel %u, domain %u",
		     agent_id, channel_id, domain_id);
		panic();
	}

	channel_cfg->perfd[domain_id] = (struct scmi_perfd){
		.name = "DVFS",
		.initial_opp = initial_index,
		.dvfs_opp_count = dvfs_count,
		.dvfs_opp_khz = dvfs_opp_khz,
		.dvfs_opp_mv = dvfs_opp_mv,
		.clk = clk,
		.rdev = regulator,
	};

	return TEE_SUCCESS;
}
