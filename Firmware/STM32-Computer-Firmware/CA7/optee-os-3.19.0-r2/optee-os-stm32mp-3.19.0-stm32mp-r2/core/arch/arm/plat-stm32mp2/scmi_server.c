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
#include <stdint.h>
#include <speculation_barrier.h>
#include <stm32_util.h>
#include <string.h>
#include <tee_api_defines.h>
#include <util.h>

#define TIMEOUT_US_1MS		1000

#define SCMI_CLOCK_NAME_SIZE	16
#define SCMI_RD_NAME_SIZE	16
#define SCMI_VOLTD_NAME_SIZE	16

/*
 * struct stm32_scmi_clk - Data for the exposed clock
 * @clock_id: Clock identifier in RCC clock driver
 * @name: Clock string ID exposed to channel
 * @enabled: State of the SCMI clock
 */
struct stm32_scmi_clk {
	unsigned long clock_id;
	struct clk *clk;
	const char *name;
	bool enabled;
	bool is_exposed;
};

/*
 * struct stm32_scmi_rd - Data for the exposed reset controller
 * @reset_id: Reset identifier in RCC reset driver
 * @base: Physical controller address
 * @name: Reset string ID exposed to channel
 * @rstctrl: Reset controller device
 */
struct stm32_scmi_rd {
	unsigned long reset_id;
	paddr_t base;
	const char *name;
	struct rstctrl *rstctrl;
	bool is_exposed;
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
		.enabled = false, \
		.is_exposed = true,\
	}

#define RESET_CELL(_scmi_id, _id, _base, _name) \
	[_scmi_id] = { \
		.reset_id = _id, \
		.base = _base, \
		.name = _name, \
		.is_exposed = true,\
	}

#define PERFD_CELL(_scmi_id, _name) \
	[_scmi_id] = { \
		.name = (_name), \
	}

struct channel_resources {
	struct scmi_msg_channel *channel;
	struct stm32_scmi_clk *clock;
	size_t clock_count;
	struct stm32_scmi_rd *rd;
	size_t rd_count;
	struct stm32_scmi_perfd *perfd;
	size_t perfd_count;
};

static struct stm32_scmi_clk stm32_scmi_clock[] = {
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

	CLOCK_CELL(CK_SCMI_FLEXGEN_10, CK_FLEXGEN_10, "ck_flexgen_10", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_11, CK_FLEXGEN_11, "ck_flexgen_11", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_12, CK_FLEXGEN_12, "ck_flexgen_12", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_13, CK_FLEXGEN_13, "ck_flexgen_13", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_14, CK_FLEXGEN_14, "ck_flexgen_14", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_15, CK_FLEXGEN_15, "ck_flexgen_15", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_16, CK_FLEXGEN_16, "ck_flexgen_16", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_17, CK_FLEXGEN_17, "ck_flexgen_17", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_18, CK_FLEXGEN_18, "ck_flexgen_18", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_19, CK_FLEXGEN_19, "ck_flexgen_19", true),

	CLOCK_CELL(CK_SCMI_FLEXGEN_20, CK_FLEXGEN_20, "ck_flexgen_20", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_21, CK_FLEXGEN_21, "ck_flexgen_21", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_22, CK_FLEXGEN_22, "ck_flexgen_22", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_23, CK_FLEXGEN_23, "ck_flexgen_23", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_24, CK_FLEXGEN_24, "ck_flexgen_24", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_25, CK_FLEXGEN_25, "ck_flexgen_25", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_26, CK_FLEXGEN_26, "ck_flexgen_26", true),
	CLOCK_CELL(CK_SCMI_FLEXGEN_27, CK_FLEXGEN_27, "ck_flexgen_27", true),
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
	CLOCK_CELL(CK_SCMI_KER_STM, CK_KER_STM, "ck_icn_s_stm", true),
};

static struct stm32_scmi_rd stm32_scmi_reset_domain[] = {
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

struct stm32_scmi_perfd stm32_scmi_performance_domain[] = {
	#ifdef CFG_SCMI_MSG_PERF_DOMAIN
	PERFD_CELL(0 /* PERFD_SCMI_CPU_OPP */, "cpu-opp"),
	#endif
};

/* Currently supporting Clocks and Reset Domains */
static const uint8_t plat_protocol_list[] = {
	SCMI_PROTOCOL_ID_CLOCK,
	SCMI_PROTOCOL_ID_RESET_DOMAIN,
#ifdef CFG_SCMI_MSG_REGULATOR_CONSUMER
	SCMI_PROTOCOL_ID_VOLTAGE_DOMAIN,
#endif
#ifdef CFG_SCMI_MSG_PERF_DOMAIN
	SCMI_PROTOCOL_ID_PERF,
#endif
	0U /* Null termination */
};

static const struct channel_resources scmi_channel[] = {
	[0] = {
		.channel = &(struct scmi_msg_channel){ },
		.clock = stm32_scmi_clock,
		.clock_count = ARRAY_SIZE(stm32_scmi_clock),
		.rd = stm32_scmi_reset_domain,
		.rd_count = ARRAY_SIZE(stm32_scmi_reset_domain),
		.perfd = stm32_scmi_performance_domain,
		.perfd_count = ARRAY_SIZE(stm32_scmi_performance_domain),
	},
};

static const struct channel_resources *find_resource(unsigned int channel_id)
{
	assert(channel_id < ARRAY_SIZE(scmi_channel));

	return scmi_channel + channel_id;
}

struct scmi_msg_channel *plat_scmi_get_channel(unsigned int channel_id)
{
	const size_t max_id = ARRAY_SIZE(scmi_channel);
	unsigned int confined_id = confine_array_index(channel_id, max_id);

	if (channel_id >= max_id)
		return NULL;

	return find_resource(confined_id)->channel;
}

static size_t __maybe_unused plat_scmi_protocol_count_paranoid(void)
{
	unsigned int n = 0;
	unsigned int count = 0;
	const size_t channel_count = ARRAY_SIZE(scmi_channel);

	for (n = 0; n < channel_count; n++)
		if (scmi_channel[n].clock_count)
			break;
	if (n < channel_count)
		count++;

	for (n = 0; n < channel_count; n++)
		if (scmi_channel[n].rd_count)
			break;
	if (n < channel_count)
		count++;

	for (n = 0; n < channel_count; n++)
		if (IS_ENABLED(CFG_SCMI_MSG_REGULATOR_CONSUMER) &&
		    plat_scmi_voltd_count(n))
			break;
	if (n < channel_count)
		count++;

	for (n = 0; n < channel_count; n++)
		if (scmi_channel[n].perfd_count)
			break;
	if (n < channel_count)
		count++;

	return count;
}

static const char vendor[] = "ST";
static const char sub_vendor[] = "";

const char *plat_scmi_vendor_name(void)
{
	return vendor;
}

const char *plat_scmi_sub_vendor_name(void)
{
	return sub_vendor;
}

size_t plat_scmi_protocol_count(void)
{
	const size_t count = ARRAY_SIZE(plat_protocol_list) - 1;

	assert(count == plat_scmi_protocol_count_paranoid());

	return count;
}

const uint8_t *plat_scmi_protocol_list(unsigned int channel_id __unused)
{
	assert(plat_scmi_protocol_count_paranoid() ==
	       (ARRAY_SIZE(plat_protocol_list) - 1));

	return plat_protocol_list;
}

/*
 * Platform SCMI clocks
 */
static struct stm32_scmi_clk *find_clock(unsigned int channel_id,
					 unsigned int scmi_id)
{
	const struct channel_resources *resource = find_resource(channel_id);

	if (resource && scmi_id < resource->clock_count)
		return &resource->clock[scmi_id];

	return NULL;
}

size_t plat_scmi_clock_count(unsigned int channel_id)
{
	const struct channel_resources *resource = find_resource(channel_id);

	if (!resource)
		return 0;

	return resource->clock_count;
}

const char *plat_scmi_clock_get_name(unsigned int channel_id,
				     unsigned int scmi_id)
{
	struct stm32_scmi_clk *clock = find_clock(channel_id, scmi_id);

	if (!clock)
		return NULL;

	return clock->name;
}

int32_t plat_scmi_clock_rates_by_step(unsigned int channel_id,
				      unsigned int scmi_id,
				      unsigned long *array)
{
	struct stm32_scmi_clk *clock = find_clock(channel_id, scmi_id);

	if (!clock)
		return SCMI_NOT_FOUND;

	switch (scmi_id) {
	case CK_SCMI_CPU1:		/* CPU1 */
	case CK_SCMI_FLEXGEN_10:	/* SPI2 & SPI3 */
	case CK_SCMI_FLEXGEN_16:	/* SPI1 */
	case CK_SCMI_FLEXGEN_23:	/* SAI1 */
	case CK_SCMI_FLEXGEN_24:	/* SAI2 */
	case CK_SCMI_FLEXGEN_25:	/* SAI3 & SAI4 */
	case CK_SCMI_FLEXGEN_27:	/* LTDC */
		array[0] = 0U;
		array[1] = UINT32_MAX;
		array[2] = 0U;
		break;
	default:
		array[0] = clk_get_rate(clock->clk);
		array[1] = array[0];
		array[2] = 0U;
		break;
	}

	return SCMI_SUCCESS;
}

int32_t plat_scmi_clock_set_rate(unsigned int channel_id, unsigned int scmi_id,
				 unsigned long rate)
{
	struct stm32_scmi_clk *clock = find_clock(channel_id, scmi_id);

	if (!clock)
		return SCMI_NOT_FOUND;

	switch (scmi_id) {
	case CK_SCMI_CPU1:		/* CPU1 */
	case CK_SCMI_FLEXGEN_10:	/* SPI2 & SPI3 */
	case CK_SCMI_FLEXGEN_16:	/* SPI1 */
	case CK_SCMI_FLEXGEN_23:	/* SAI1 */
	case CK_SCMI_FLEXGEN_24:	/* SAI2 */
	case CK_SCMI_FLEXGEN_25:	/* SAI3 & SAI4 */
	case CK_SCMI_FLEXGEN_27:	/* LTDC */
		if (clk_set_rate(clock->clk, rate))
			return SCMI_INVALID_PARAMETERS;
		break;
	default:
		if (rate != clk_get_rate(clock->clk))
			return SCMI_INVALID_PARAMETERS;
		break;
	}

	return SCMI_SUCCESS;
}

unsigned long plat_scmi_clock_get_rate(unsigned int channel_id,
				       unsigned int scmi_id)
{
	struct stm32_scmi_clk *clock = find_clock(channel_id, scmi_id);

	if (!clock)
		return 0;

	return clk_get_rate(clock->clk);
}

unsigned long plat_scmi_clock_round_rate(unsigned int channel_id,
				       unsigned int scmi_id, unsigned long rate)
{
	struct stm32_scmi_clk *clock = find_clock(channel_id, scmi_id);

	if (!clock)
		return 0;

	return clk_round_rate(clock->clk, rate);
}

int32_t plat_scmi_clock_get_state(unsigned int channel_id, unsigned int scmi_id)
{
	struct stm32_scmi_clk *clock = find_clock(channel_id, scmi_id);

	if (!clock)
		return 0;

	return (int32_t)clock->enabled;
}

int32_t plat_scmi_clock_set_state(unsigned int channel_id, unsigned int scmi_id,
				  bool enable_not_disable)
{
	struct stm32_scmi_clk *clock = find_clock(channel_id, scmi_id);

	if (!clock)
		return SCMI_NOT_FOUND;

	if (enable_not_disable) {
		if (!clock->enabled) {
			FMSG("SCMI clock %u enable", scmi_id);
			clk_enable(clock->clk);
			clock->enabled = true;
		}
	} else {
		if (clock->enabled) {
			FMSG("SCMI clock %u disable", scmi_id);
			clk_disable(clock->clk);
			clock->enabled = false;
		}
	}

	return SCMI_SUCCESS;
}

/*
 * Platform SCMI reset domains
 */
static struct stm32_scmi_rd *find_rd(unsigned int channel_id,
				     unsigned int scmi_id)
{
	const struct channel_resources *resource = find_resource(channel_id);

	if (resource && scmi_id < resource->rd_count)
		return &resource->rd[scmi_id];

	return NULL;
}

const char *plat_scmi_rd_get_name(unsigned int channel_id, unsigned int scmi_id)
{
	const struct stm32_scmi_rd *rd = find_rd(channel_id, scmi_id);

	if (!rd)
		return NULL;

	return rd->name;
}

size_t plat_scmi_rd_count(unsigned int channel_id)
{
	const struct channel_resources *resource = find_resource(channel_id);

	if (!resource)
		return 0;

	return resource->rd_count;
}

int32_t plat_scmi_rd_autonomous(unsigned int channel_id, unsigned int scmi_id,
				uint32_t state)
{
	const struct stm32_scmi_rd *rd = find_rd(channel_id, scmi_id);
	const struct stm32_firewall_cfg nsec_cfg[] = {
		{ FWLL_NSEC_PRIV_RW | FWLL_MASTER(0) },
		{ }, /* Null terminated */
	};

	if (!rd)
		return SCMI_NOT_FOUND;
	assert(rd->rstctrl);

	/* Supports only reset with context loss */
	if (state)
		return SCMI_NOT_SUPPORTED;

	if (rd->base && stm32_firewall_check_access(rd->base, 0, nsec_cfg))
		return SCMI_DENIED;

	FMSG("SCMI reset %u cycle", scmi_id);

	if (rstctrl_assert_to(rd->rstctrl, TIMEOUT_US_1MS))
		return SCMI_HARDWARE_ERROR;

	if (rstctrl_deassert_to(rd->rstctrl, TIMEOUT_US_1MS))
		return SCMI_HARDWARE_ERROR;

	return SCMI_SUCCESS;
}

int32_t plat_scmi_rd_set_state(unsigned int channel_id, unsigned int scmi_id,
			       bool assert_not_deassert)
{
	const struct stm32_scmi_rd *rd = find_rd(channel_id, scmi_id);
	TEE_Result res = TEE_ERROR_GENERIC;
	const struct stm32_firewall_cfg nsec_cfg[] = {
		{ FWLL_NSEC_PRIV_RW | FWLL_MASTER(0) },
		{ }, /* Null terminated */
	};

	if (!rd)
		return SCMI_NOT_FOUND;

	if (rd->base && stm32_firewall_check_access(rd->base, 0, nsec_cfg))
		return SCMI_DENIED;

	assert(rd->rstctrl);
	if (assert_not_deassert) {
		FMSG("SCMI reset %u set", scmi_id);
		res = rstctrl_assert_to(rd->rstctrl, TIMEOUT_US_1MS);
	} else {
		FMSG("SCMI reset %u release", scmi_id);
		res = rstctrl_deassert_to(rd->rstctrl, TIMEOUT_US_1MS);
	}

	if (res)
		return SCMI_HARDWARE_ERROR;

	return SCMI_SUCCESS;
}

#ifdef CFG_SCMI_MSG_PERF_DOMAIN
/*
 * Platform SCMI performance domains
 *
 * Note: currently exposes a single performance domain that is the CPU DVFS
 * operating point. Non-secure is allowed to access this performance domain.
 */
static struct stm32_scmi_perfd *find_perfd(unsigned int channel_id,
					   unsigned int domain_id)
{
	const struct channel_resources *resource = find_resource(channel_id);
	size_t n = 0;

	if (resource) {
		for (n = 0; n < resource->perfd_count; n++)
			if (n == domain_id)
				return &resource->perfd[n];
	}

	return NULL;
}

size_t plat_scmi_perf_count(unsigned int channel_id)
{
	const struct channel_resources *resource = find_resource(channel_id);

	if (!resource)
		return 0;

	return resource->perfd_count;
}

const char *plat_scmi_perf_domain_name(unsigned int channel_id,
				       unsigned int domain_id)
{
	struct stm32_scmi_perfd *perfd = find_perfd(channel_id, domain_id);

	if (!perfd)
		return NULL;

	return perfd->name;
}

int32_t plat_scmi_perf_levels_array(unsigned int channel_id,
				    unsigned int domain_id, size_t start_index,
				    unsigned int *levels, size_t *nb_elts)
{
	struct stm32_scmi_perfd *perfd = find_perfd(channel_id, domain_id);
	size_t full_count = 0;
	size_t out_count = 0;
	size_t n = 0;

	if (!perfd)
		return SCMI_NOT_FOUND;

	full_count = stm32_cpu_opp_count();

	if (!levels) {
		*nb_elts = full_count - start_index;

		return SCMI_SUCCESS;
	}

	if (SUB_OVERFLOW(full_count, start_index, &out_count))
		return SCMI_GENERIC_ERROR;

	out_count = MIN(out_count, *nb_elts);

	FMSG("%zu levels: start %zu requested %zu output %zu",
	     full_count, start_index, *nb_elts, out_count);

	for (n = 0; n < out_count; n++)
		levels[n] = stm32_cpu_opp_level(n);

	*nb_elts = out_count;

	return SCMI_SUCCESS;
}

int32_t plat_scmi_perf_level_get(unsigned int channel_id,
				 unsigned int domain_id, unsigned int *level)
{
	struct stm32_scmi_perfd *perfd = find_perfd(channel_id, domain_id);
	unsigned int current_level = 0;

	if (!perfd)
		return SCMI_NOT_FOUND;

	if (stm32_cpu_opp_read_level(&current_level))
		return SCMI_GENERIC_ERROR;

	*level = current_level;

	return SCMI_SUCCESS;
}

int32_t plat_scmi_perf_level_set(unsigned int channel_id,
				 unsigned int domain_id, unsigned int level)
{
	struct stm32_scmi_perfd *perfd = find_perfd(channel_id, domain_id);

	if (!perfd)
		return SCMI_NOT_FOUND;

	switch (stm32_cpu_opp_set_level(level)) {
	case TEE_SUCCESS:
		return SCMI_SUCCESS;
	case TEE_ERROR_BAD_PARAMETERS:
		return SCMI_OUT_OF_RANGE;
	default:
		return SCMI_GENERIC_ERROR;
	}
}
#endif

/*
 * Initialize platform SCMI resources
 */
static TEE_Result stm32mp2_init_scmi_server(void)
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; i < ARRAY_SIZE(scmi_channel); i++) {
		const struct channel_resources *res = scmi_channel + i;
		struct scmi_msg_channel *chan = res->channel;
		size_t voltd_count = 0;

		/* Enforce non-secure shm mapped as device memory */
		if (chan->shm_addr.pa) {
			chan->shm_addr.va =
				(vaddr_t)phys_to_virt(chan->shm_addr.pa,
						      MEM_AREA_IO_NSEC, 1);
			assert(chan->shm_addr.va);

			scmi_smt_init_agent_channel(chan);
		}

		for (j = 0; j < res->clock_count; j++) {
			struct stm32_scmi_clk *clk = &res->clock[j];

			if (!clk->is_exposed)
				continue;

			if (!clk->name ||
			    strlen(clk->name) >= SCMI_CLOCK_NAME_SIZE)
				panic("SCMI clock name invalid");

			clk->clk = stm32mp_rcc_clock_id_to_clk(clk->clock_id);
			assert(clk->clk);
		}

		for (j = 0; j < res->rd_count; j++) {
			struct stm32_scmi_rd *rd = &res->rd[j];
			struct rstctrl *rstctrl = NULL;

			if (!rd->is_exposed)
				continue;

			if (!rd->name ||
			    strlen(rd->name) >= SCMI_RD_NAME_SIZE)
				panic("SCMI reset domain name invalid");

			rstctrl = stm32mp_rcc_reset_id_to_rstctrl(rd->reset_id);
			assert(rstctrl);
			if (rstctrl_get_exclusive(rstctrl))
				panic();

			rd->rstctrl = rstctrl;
		}

		if (IS_ENABLED(CFG_SCMI_MSG_REGULATOR_CONSUMER))
		    voltd_count = plat_scmi_voltd_count(i);

		for (j = 0; j < voltd_count; j++) {
			const char *name = plat_scmi_voltd_get_name(i, j);

			if (!name || strlen(name) >= SCMI_VOLTD_NAME_SIZE)
				panic("SCMI voltage domain name invalid");
		}
	}

	return TEE_SUCCESS;
}
driver_init_late(stm32mp2_init_scmi_server);
