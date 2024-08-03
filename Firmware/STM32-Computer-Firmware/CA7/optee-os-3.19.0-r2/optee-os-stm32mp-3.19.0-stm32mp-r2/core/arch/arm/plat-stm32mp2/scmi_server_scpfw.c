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
#include <drivers/stm32_remoteproc.h>
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
 * @is_exposed: True if SMCI clock is exposed, false if it is not reachable
 * @name: Clock string ID exposed to channel
 * @enabled: State of the SCMI clock at initialization
 * @change_rate: SCMMI agent is allowed to change the rate
 * @clk: Clock device manipulated by the SCMI channel
 */
struct stm32_scmi_clk {
	unsigned long clock_id;
	bool is_exposed;
	const char *name;
	bool enabled;
	bool change_rate;
	struct clk *clk;
};

/*
 * struct stm32_scmi_rd - Data for the exposed reset controller
 * @reset_id: Reset identifier in RCC reset driver
 * @is_exposed: True if SMCI reset is exposed, false if it is not reachable
 * @base: Physical controller address
 * @name: Reset string ID exposed to channel
 * @rstctrl: Reset controller manipulated by the SCMI channel
 * @rstctrl_be: Backend reset controller device
 */
struct stm32_scmi_rd {
	unsigned long reset_id;
	bool is_exposed;
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

/*
 * struct stm32_scmi_pd - Data for the exposed power domains
 * @name: Power domain name exposed to the channel
 * @clock_id: Clock identifier in RCC clock driver
 * @regu_name: Regulator node name in OP-TEE secure FDT
 */
struct stm32_scmi_pd {
	const char *name;
	unsigned long clock_id;
	const char *regu_name;
};

#define CLOCK_CELL(_scmi_id, _id, _name, _init_enabled) \
	[_scmi_id] = { \
		.clock_id = _id, \
		.name = _name, \
		.enabled = _init_enabled, \
		.is_exposed = true, \
	}

#define CLOCK_CELL_RATES(_scmi_id, _id, _name, _init_enabled) \
	[_scmi_id] = { \
		.clock_id = _id, \
		.name = _name, \
		.enabled = _init_enabled, \
		.change_rate = true, \
		.is_exposed = true, \
	}

#define RESET_CELL(_scmi_id, _id, _base, _name) \
	[_scmi_id] = { \
		.reset_id = _id, \
		.base = _base, \
		.name = _name, \
		.is_exposed = true, \
	}

#define PERFD_CELL(_scmi_id, _name) \
	[_scmi_id] = { \
		.name = (_name), \
	}

#define PD_CELL(_scmi_id, _name, _clock_id, _regu_name) \
	[_scmi_id] = { \
		.name = (_name), \
		.clock_id = (_clock_id), \
		.regu_name = (_regu_name), \
	}

struct channel_resources {
	struct stm32_scmi_clk *clock;
	size_t clock_count;
	struct stm32_scmi_rd *rd;
	size_t rd_count;
	struct stm32_scmi_perfd *perfd;
	size_t perfd_count;
	struct stm32_scmi_pd *pd;
	size_t pd_count;
};

static struct stm32_scmi_clk stm32_scmi_clock[] = {
	/*
	 * Some SCMI clocks are allowed to manipulate their rate:
	 * FLXEGEN_10: SPI2 & SPI3
	 * FLEXGEN_16: SPI1
	 * FLEXGEN_24: SAI1
	 * FLEXGEN_24: SAI2
	 * FLEXGEN_25: SAI3 & SAI4
	 * FLEXGEN_27: LTDC
	 */
	CLOCK_CELL(CK_SCMI_ICN_HS_MCU, CK_ICN_HS_MCU, "ck_icn_hs_mcu", true),

	CLOCK_CELL(CK_SCMI_ICN_SDMMC, CK_ICN_SDMMC, "ck_icn_sdmmc", true),

	CLOCK_CELL(CK_SCMI_ICN_DDR, CK_ICN_DDR, "ck_icn_ddr", true),
	CLOCK_CELL(CK_SCMI_ICN_DISPLAY, CK_ICN_DISPLAY, "ck_icn_display", true),
	CLOCK_CELL(CK_SCMI_ICN_HSL, CK_ICN_HSL, "ck_icn_hsl", true),
	CLOCK_CELL(CK_SCMI_ICN_NIC, CK_ICN_NIC, "ck_icn_nic", true),
	CLOCK_CELL(CK_SCMI_ICN_VID, CK_ICN_VID, "ck_icn_vid", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_07, CK_FLEXGEN_07, "ck_flexgen_07", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_08, CK_FLEXGEN_08, "ck_flexgen_08", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_09, CK_FLEXGEN_09, "ck_flexgen_09", true),

	CLOCK_CELL_RATES(CK_SCMI_FLEXGEN_10, CK_FLEXGEN_10, "ck_flexgen_10",
			 true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_11, CK_FLEXGEN_11, "ck_flexgen_11", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_12, CK_FLEXGEN_12, "ck_flexgen_12", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_13, CK_FLEXGEN_13, "ck_flexgen_13", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_14, CK_FLEXGEN_14, "ck_flexgen_14", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_15, CK_FLEXGEN_15, "ck_flexgen_15", true),
	CLOCK_CELL_RATES(CK_SCMI_FLEXGEN_16, CK_FLEXGEN_16, "ck_flexgen_16",
			 true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_17, CK_FLEXGEN_17, "ck_flexgen_17", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_18, CK_FLEXGEN_18, "ck_flexgen_18", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_19, CK_FLEXGEN_19, "ck_flexgen_19", true),

	CLOCK_CELL(CK_SCMI_FLEXGEN_20, CK_FLEXGEN_20, "ck_flexgen_20", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_21, CK_FLEXGEN_21, "ck_flexgen_21", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_22, CK_FLEXGEN_22, "ck_flexgen_22", true),
	CLOCK_CELL_RATES(CK_SCMI_FLEXGEN_23, CK_FLEXGEN_23, "ck_flexgen_23",
			 true),
	CLOCK_CELL_RATES(CK_SCMI_FLEXGEN_24, CK_FLEXGEN_24, "ck_flexgen_24",
			 true),
	CLOCK_CELL_RATES(CK_SCMI_FLEXGEN_25, CK_FLEXGEN_25, "ck_flexgen_25",
			 true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_26, CK_FLEXGEN_26, "ck_flexgen_26", true),
	CLOCK_CELL_RATES(CK_SCMI_FLEXGEN_27, CK_FLEXGEN_27, "ck_flexgen_27",
			 true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_28, CK_FLEXGEN_28, "ck_flexgen_28", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_29, CK_FLEXGEN_29, "ck_flexgen_29", true),

	CLOCK_CELL(CK_SCMI_FLEXGEN_30, CK_FLEXGEN_30, "ck_flexgen_30", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_31, CK_FLEXGEN_31, "ck_flexgen_31", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_32, CK_FLEXGEN_32, "ck_flexgen_32", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_33, CK_FLEXGEN_33, "ck_flexgen_33", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_34, CK_FLEXGEN_34, "ck_flexgen_34", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_35, CK_FLEXGEN_35, "ck_flexgen_35", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_36, CK_FLEXGEN_36, "ck_flexgen_36", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_37, CK_FLEXGEN_37, "ck_flexgen_37", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_38, CK_FLEXGEN_38, "ck_flexgen_38", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_39, CK_FLEXGEN_39, "ck_flexgen_39", true),

	CLOCK_CELL(CK_SCMI_FLEXGEN_40, CK_FLEXGEN_40, "ck_flexgen_40", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_41, CK_FLEXGEN_41, "ck_flexgen_41", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_42, CK_FLEXGEN_42, "ck_flexgen_42", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_43, CK_FLEXGEN_43, "ck_flexgen_43", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_44, CK_FLEXGEN_44, "ck_flexgen_44", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_45, CK_FLEXGEN_45, "ck_flexgen_45", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_46, CK_FLEXGEN_46, "ck_flexgen_46", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_47, CK_FLEXGEN_47, "ck_flexgen_47", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_48, CK_FLEXGEN_48, "ck_flexgen_48", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_49, CK_FLEXGEN_49, "ck_flexgen_49", true),

	CLOCK_CELL(CK_SCMI_FLEXGEN_50, CK_FLEXGEN_50, "ck_flexgen_50", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_51, CK_FLEXGEN_51, "ck_flexgen_51", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_52, CK_FLEXGEN_52, "ck_flexgen_52", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_53, CK_FLEXGEN_53, "ck_flexgen_53", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_54, CK_FLEXGEN_54, "ck_flexgen_54", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_55, CK_FLEXGEN_55, "ck_flexgen_55", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_56, CK_FLEXGEN_56, "ck_flexgen_56", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_57, CK_FLEXGEN_57, "ck_flexgen_57", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_58, CK_FLEXGEN_58, "ck_flexgen_58", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_59, CK_FLEXGEN_59, "ck_flexgen_59", true),

	CLOCK_CELL(CK_SCMI_FLEXGEN_60, CK_FLEXGEN_60, "ck_flexgen_60", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_61, CK_FLEXGEN_61, "ck_flexgen_61", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_62, CK_FLEXGEN_62, "ck_flexgen_62", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_63, CK_FLEXGEN_63, "ck_flexgen_63", true),

	CLOCK_CELL(CK_SCMI_ICN_LS_MCU, CK_ICN_LS_MCU, "ck_icn_ls_mcu", true),
	CLOCK_CELL(CK_SCMI_HSE, HSE_CK, "hse_ck", true),
	CLOCK_CELL(CK_SCMI_LSE, LSE_CK, "lse_ck", true),
	CLOCK_CELL(CK_SCMI_HSI, HSI_CK, "hsi_ck", true),
	CLOCK_CELL(CK_SCMI_LSI, LSI_CK, "lsi_ck", true),
	CLOCK_CELL(CK_SCMI_MSI, MSI_CK, "msi_ck", true),

	CLOCK_CELL(CK_SCMI_RTCCK, RTC_CK, "rtc_ck", true),

	CLOCK_CELL(CK_SCMI_ICN_APB1, CK_ICN_APB1, "ck_icn_apb1", true),
	CLOCK_CELL(CK_SCMI_ICN_APB2, CK_ICN_APB2, "ck_icn_apb2", true),
	CLOCK_CELL(CK_SCMI_ICN_APB3, CK_ICN_APB3, "ck_icn_apb3", true),
	CLOCK_CELL(CK_SCMI_ICN_APB4, CK_ICN_APB4, "ck_icn_apb4", true),
	CLOCK_CELL(CK_SCMI_ICN_APBDBG, CK_ICN_APBDBG, "ck_icn_apbdbg", true),

	CLOCK_CELL(CK_SCMI_TIMG1, TIMG1_CK, "timg1_ck", true),
	CLOCK_CELL(CK_SCMI_TIMG2, TIMG2_CK, "timg2_ck", true),

	CLOCK_CELL(CK_SCMI_BKPSRAM, CK_BUS_BKPSRAM, "ck_bus_bkpsram", true),
	CLOCK_CELL(CK_SCMI_BSEC, CK_BUS_BSEC, "ck_bus_bsec", true),
	CLOCK_CELL(CK_SCMI_BUS_ETR, CK_BUS_ETR, "ck_icn_p_etr", true),
	CLOCK_CELL(CK_SCMI_FMC, CK_KER_FMC, "ck_ker_fmc", true),
	CLOCK_CELL(CK_SCMI_GPIOA, CK_BUS_GPIOA, "ck_bus_gpioa", true),
	CLOCK_CELL(CK_SCMI_GPIOB, CK_BUS_GPIOB, "ck_bus_gpiob", true),
	CLOCK_CELL(CK_SCMI_GPIOC, CK_BUS_GPIOC, "ck_bus_gpioc", true),
	CLOCK_CELL(CK_SCMI_GPIOD, CK_BUS_GPIOD, "ck_bus_gpiod", true),
	CLOCK_CELL(CK_SCMI_GPIOE, CK_BUS_GPIOE, "ck_bus_gpioe", true),
	CLOCK_CELL(CK_SCMI_GPIOF, CK_BUS_GPIOF, "ck_bus_gpiof", true),
	CLOCK_CELL(CK_SCMI_GPIOG, CK_BUS_GPIOG, "ck_bus_gpiog", true),
	CLOCK_CELL(CK_SCMI_GPIOH, CK_BUS_GPIOH, "ck_bus_gpioh", true),
	CLOCK_CELL(CK_SCMI_GPIOI, CK_BUS_GPIOI, "ck_bus_gpioi", true),
	CLOCK_CELL(CK_SCMI_GPIOJ, CK_BUS_GPIOJ, "ck_bus_gpioj", true),
	CLOCK_CELL(CK_SCMI_GPIOK, CK_BUS_GPIOK, "ck_bus_gpiok", true),
	CLOCK_CELL(CK_SCMI_GPIOZ, CK_BUS_GPIOZ, "ck_bus_gpioz", true),
	CLOCK_CELL(CK_SCMI_HPDMA1, CK_BUS_HPDMA1, "ck_bus_hpdma1", true),
	CLOCK_CELL(CK_SCMI_HPDMA2, CK_BUS_HPDMA2, "ck_bus_hpdma2", true),
	CLOCK_CELL(CK_SCMI_HPDMA3, CK_BUS_HPDMA3, "ck_bus_hpdma3", true),
	CLOCK_CELL(CK_SCMI_HSEM, CK_BUS_HSEM, "ck_bus_hsem", true),
	CLOCK_CELL(CK_SCMI_IPCC1, CK_BUS_IPCC1, "ck_bus_ipcc1", true),
	CLOCK_CELL(CK_SCMI_IPCC2, CK_BUS_IPCC2, "ck_bus_ipcc2", true),
	CLOCK_CELL(CK_SCMI_LPDMA, CK_BUS_LPDMA, "ck_bus_lpdma", true),
	CLOCK_CELL(CK_SCMI_RETRAM, CK_BUS_RETRAM, "ck_bus_retram", true),
	CLOCK_CELL(CK_SCMI_RTC, CK_BUS_RTC, "ck_bus_rtc", true),
	CLOCK_CELL(CK_SCMI_SRAM1, CK_BUS_SRAM1, "ck_bus_sram1", true),
	CLOCK_CELL(CK_SCMI_SRAM2, CK_BUS_SRAM2, "ck_bus_sram2", true),
	CLOCK_CELL(CK_SCMI_SYSCPU1, CK_BUS_SYSCPU1, "ck_bus_syscpu1", true),
	CLOCK_CELL(CK_SCMI_CPU1, CK_CPU1, "ck_cpu1", true),
	CLOCK_CELL(CK_SCMI_HSE_DIV2, HSE_DIV2_CK, "hse_div2_ck", false),
	CLOCK_CELL(CK_SCMI_PLL2, PLL2_CK, "ck_pll2", true),
	CLOCK_CELL(CK_SCMI_PLL3, PLL3_CK, "ck_pll3", false),
	CLOCK_CELL(CK_SCMI_LPSRAM1, CK_BUS_LPSRAM1, "ck_icn_lpsram1", true),
	CLOCK_CELL(CK_SCMI_LPSRAM2, CK_BUS_LPSRAM2, "ck_icn_lpsram2 ", true),
	CLOCK_CELL(CK_SCMI_LPSRAM3, CK_BUS_LPSRAM3, "ck_icn_lpsram3", true),
	CLOCK_CELL(CK_SCMI_VDERAM, CK_BUS_VDERAM, "ck_icn_s_vderam", true),
	CLOCK_CELL(CK_SCMI_SYSRAM, CK_BUS_SYSRAM, "ck_icn_s_sysram", true),
	CLOCK_CELL(CK_SCMI_OSPI1, CK_KER_OSPI1, "ck_ker_ospi1", true),
	CLOCK_CELL(CK_SCMI_OSPI2, CK_KER_OSPI2, "ck_ker_ospi2", true),
	CLOCK_CELL(CK_SCMI_TPIU, CK_KER_TPIU, "ck_ker_tpiu", true),
	CLOCK_CELL(CK_SCMI_SYSDBG, CK_SYSDBG, "ck_sys_dbg", true),
	CLOCK_CELL(CK_SCMI_SYSATB, CK_BUS_SYSATB, "ck_sys_atb", true),
	CLOCK_CELL(CK_SCMI_TSDBG, CK_KER_TSDBG, "ck_ker_tsdbg", true),
	CLOCK_CELL(CK_SCMI_KER_ETR, CK_KER_ETR, "ck_icn_m_etr", true),
	CLOCK_CELL(CK_SCMI_BUS_STM, CK_BUS_STM, "ck_icn_p_stm", true),
	CLOCK_CELL(CK_SCMI_KER_STM, CK_BUS_STM, "ck_icn_s_stm", true),
};

static struct stm32_scmi_rd stm32_scmi_reset[] = {
	RESET_CELL(RST_SCMI_C1_R, C1_R, 0, "c1"),
	RESET_CELL(RST_SCMI_C2_R, C2_R, 0, "c2"),
	RESET_CELL(RST_SCMI_C1_HOLDBOOT_R, HOLD_BOOT_C1_R, 0, "c1_holdboot"),
	RESET_CELL(RST_SCMI_C2_HOLDBOOT_R, HOLD_BOOT_C2_R, 0, "c2_holdboot"),
	/* SCMI FMC reset allowed: RCC driver stubs request if necessary */
	RESET_CELL(RST_SCMI_FMC, FMC_R, 0, "fmc"),
	RESET_CELL(RST_SCMI_OSPI1, OSPI1_R, OSPI1_BASE, "ospi1"),
	RESET_CELL(RST_SCMI_OSPI2, OSPI2_R, OSPI2_BASE, "ospi2"),
	RESET_CELL(RST_SCMI_OSPI1DLL, OSPI1DLL_R, OSPI1_BASE, "ospi1_ddl"),
	RESET_CELL(RST_SCMI_OSPI2DLL, OSPI2DLL_R, OSPI2_BASE, "ospi2_ddl"),
};

static struct stm32_scmi_pd stm32_scmi_pd[] = {
	PD_CELL(PD_SCMI_GPU, "gpu", PLL3_CK, "vddgpu"),
};

static const struct channel_resources scmi_channel[] = {
	[0] = {
		.clock = stm32_scmi_clock,
		.clock_count = ARRAY_SIZE(stm32_scmi_clock),
		.rd = stm32_scmi_reset,
		.rd_count = ARRAY_SIZE(stm32_scmi_reset),
		.pd = stm32_scmi_pd,
		.pd_count = ARRAY_SIZE(stm32_scmi_pd),
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

		if (!scmi_clk->is_exposed) {
			/* Empty name clocks discarded by Linux kernel */
			scmi_clk->name = "";
			continue;
		}

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
	unsigned long __maybe_unused reset_id = 0;

	assert(index >= 0 && (size_t)index < ARRAY_SIZE(stm32_scmi_reset));
	domain_iobase = stm32_scmi_reset[index].base;

	if (domain_iobase &&
	    stm32_firewall_check_access(domain_iobase, 0, nsec_cfg))
		return TEE_ERROR_ACCESS_DENIED;

#ifdef CFG_STM32MP_REMOTEPROC
	reset_id = stm32_scmi_reset[index].reset_id;
	if ((reset_id == C2_R || reset_id == HOLD_BOOT_C2_R) &&
	    stm32_rproc_reset_grant_access(STM32MP2_M33_RPROC_ID))
		return TEE_ERROR_ACCESS_DENIED;
#endif

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
	unsigned long __maybe_unused reset_id = 0;

	assert(index >= 0 && (size_t)index < ARRAY_SIZE(stm32_scmi_reset));
	domain_iobase = stm32_scmi_reset[index].base;

	if (domain_iobase &&
	    stm32_firewall_check_access(domain_iobase, 0, nsec_cfg))
		return TEE_ERROR_ACCESS_DENIED;

#ifdef CFG_STM32MP_REMOTEPROC
	reset_id = stm32_scmi_reset[index].reset_id;
	if ((reset_id == C2_R || reset_id == HOLD_BOOT_C2_R) &&
	    stm32_rproc_reset_grant_access(STM32MP2_M33_RPROC_ID))
		return TEE_ERROR_ACCESS_DENIED;
#endif

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

		if (!scmi_reset->is_exposed) {
			/* SCP-firmware needs a name */
			scmi_reset->name = "<reserved>";
			continue;
		}

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
					/* Default OFF for RIF issues */
					.enabled = false,
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

		channel_cfg->pd_count = ARRAY_SIZE(stm32_scmi_pd);
		if (channel_cfg->pd_count) {
			channel_cfg->pd = calloc(channel_cfg->pd_count,
						 sizeof(*channel_cfg->pd));
			assert(channel_cfg->pd);

			for (j = 0; j < channel_cfg->pd_count; j++) {
				struct clk *clk = stm32mp_rcc_clock_id_to_clk(
					stm32_scmi_pd[j].clock_id);
				struct rdev *regu = NULL;

				if (stm32_scmi_pd[j].regu_name != NULL) {
					regu = regulator_get_by_node_name(
						stm32_scmi_pd[j].regu_name);

					if (!regu) {
						EMSG("Regulator %s not found "
						     "for %s power domain",
						     stm32_scmi_pd[j].regu_name,
						     stm32_scmi_pd[j].name);
						panic();
					}
				}

				if (!clk)
					DMSG("Clock ID %ld not found for %s "
					     "power domain",
					     stm32_scmi_pd[j].clock_id,
					     stm32_scmi_pd[j].name);

				channel_cfg->pd[j] = (struct scmi_pd){
					.name = stm32_scmi_pd[j].name,
					.clk = clk,
					.regu = regu,
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
			free(channel_cfg->pd);
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
