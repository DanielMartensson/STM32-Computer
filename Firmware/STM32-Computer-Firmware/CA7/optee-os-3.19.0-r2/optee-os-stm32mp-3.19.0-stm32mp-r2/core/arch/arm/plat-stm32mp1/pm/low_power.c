// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2017-2022, STMicroelectronics - All Rights Reserved
 */

#include <arm.h>
#include <assert.h>
#include <boot_api.h>
#include <console.h>
#include <config.h>
#include <drivers/clk.h>
#include <drivers/gic.h>
#include <drivers/regulator.h>
#include <drivers/stm32_etzpc.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp1_ddrc.h>
#include <drivers/stm32mp1_pmic.h>
#include <drivers/stm32mp1_pwr.h>
#ifdef CFG_STM32MP13
#include <drivers/stm32mp13_rcc.h>
#else
#include <drivers/stm32mp1_rcc.h>
#endif
#include <drivers/stpmic1.h>
#include <drivers/wdt.h>
#include <initcall.h>
#include <io.h>
#include <keep.h>
#include <kernel/cache_helpers.h>
#include <kernel/delay.h>
#include <kernel/misc.h>
#include <kernel/panic.h>
#include <mm/core_memprot.h>
#include <platform_config.h>
#include <sm/psci.h>
#include <stdbool.h>
#include <stm32mp_pm.h>
#include <stm32_util.h>
#include <trace.h>

#include "context.h"
#include "power.h"

#define TIMEOUT_US_1MS		1000

#define PWRLP_TEMPO_5_HSI	5

static uint8_t gicd_rcc_wakeup;
static uint8_t gicc_pmr;

struct pwr_lp_config {
	uint32_t pwr_cr1;
	uint32_t pwr_mpucr;
	const char *regul_suspend_node_name;
};

#define TIMEOUT_US_10MS		(10 * 1000)

#ifdef CFG_STM32MP13
#define PWR_CR1_MASK	(PWR_CR1_LPDS | PWR_CR1_LPCFG | PWR_CR1_LVDS |\
			 PWR_CR1_STOP2)
#define PWR_MPUCR_MASK	(PWR_MPUCR_CSSF | PWR_MPUCR_PDDS)

static const struct pwr_lp_config config_pwr[STM32_PM_MAX_SOC_MODE] = {
	[STM32_PM_CSLEEP_RUN] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = 0U,
		.regul_suspend_node_name = NULL,
	},
	[STM32_PM_CSTOP_ALLOW_STOP] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = PWR_MPUCR_CSSF,
		.regul_suspend_node_name = NULL,
	},
	[STM32_PM_CSTOP_ALLOW_LP_STOP] = {
		.pwr_cr1 = PWR_CR1_LPDS,
		.pwr_mpucr = PWR_MPUCR_CSSF,
		.regul_suspend_node_name = "lp-stop",
	},
	[STM32_PM_CSTOP_ALLOW_LPLV_STOP] = {
		.pwr_cr1 = PWR_CR1_LVDS | PWR_CR1_LPDS,
		.pwr_mpucr = PWR_MPUCR_CSSF,
		.regul_suspend_node_name = "lplv-stop",
	},
	[STM32_PM_CSTOP_ALLOW_LPLV_STOP2] = {
		.pwr_cr1 = PWR_CR1_LVDS | PWR_CR1_LPDS | PWR_CR1_STOP2,
		.pwr_mpucr = PWR_MPUCR_CSSF,
		.regul_suspend_node_name = "lplv-stop2",
	},
	[STM32_PM_CSTOP_ALLOW_STANDBY_DDR_SR] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = PWR_MPUCR_CSSF |
			PWR_MPUCR_PDDS,
		.regul_suspend_node_name = "standby-ddr-sr",
	},
	[STM32_PM_CSTOP_ALLOW_STANDBY_DDR_OFF] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = PWR_MPUCR_CSSF |
			PWR_MPUCR_PDDS,
		.regul_suspend_node_name = "standby-ddr-off",
	},
	[STM32_PM_SHUTDOWN] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = 0U,
		.regul_suspend_node_name = "standby-ddr-off",
	},
};
#else
#define PWR_CR1_MASK	(PWR_CR1_LPDS | PWR_CR1_LPCFG | PWR_CR1_LVDS)
#define PWR_MPUCR_MASK	(PWR_MPUCR_CSTDBYDIS | PWR_MPUCR_CSSF | PWR_MPUCR_PDDS)

static const struct pwr_lp_config config_pwr[STM32_PM_MAX_SOC_MODE] = {
	[STM32_PM_CSLEEP_RUN] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = 0U,
		.regul_suspend_node_name = NULL,
	},
	[STM32_PM_CSTOP_ALLOW_STOP] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = PWR_MPUCR_CSTDBYDIS | PWR_MPUCR_CSSF,
		.regul_suspend_node_name = NULL,
	},
	[STM32_PM_CSTOP_ALLOW_LP_STOP] = {
		.pwr_cr1 = PWR_CR1_LPDS,
		.pwr_mpucr = PWR_MPUCR_CSTDBYDIS | PWR_MPUCR_CSSF,
		.regul_suspend_node_name = "lp-stop",
	},
	[STM32_PM_CSTOP_ALLOW_LPLV_STOP] = {
		.pwr_cr1 = PWR_CR1_LVDS | PWR_CR1_LPDS,
		.pwr_mpucr = PWR_MPUCR_CSTDBYDIS | PWR_MPUCR_CSSF,
		.regul_suspend_node_name = "lplv-stop",
	},
	[STM32_PM_CSTOP_ALLOW_STANDBY_DDR_SR] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = PWR_MPUCR_CSTDBYDIS | PWR_MPUCR_CSSF |
			PWR_MPUCR_PDDS,
		.regul_suspend_node_name = "standby-ddr-sr",
	},
	[STM32_PM_CSTOP_ALLOW_STANDBY_DDR_OFF] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = PWR_MPUCR_CSTDBYDIS | PWR_MPUCR_CSSF |
			PWR_MPUCR_PDDS,
		.regul_suspend_node_name = "standby-ddr-off",
	},
	[STM32_PM_SHUTDOWN] = {
		.pwr_cr1 = 0U,
		.pwr_mpucr = 0U,
		.regul_suspend_node_name = "standby-ddr-off",
	},
};
#endif

static void set_rcc_it_priority(uint8_t *it_prio, uint8_t *pmr)
{
	*it_prio = itr_set_ipriority(RCC_WAKEUP_IT, GIC_HIGHEST_SEC_PRIORITY);
	*pmr = itr_set_pmr(STM32MP_GIC_PRIORITY_CSTOP);
}

static void restore_rcc_it_priority(uint8_t it_prio, uint8_t pmr)
{
	(void)itr_set_ipriority(RCC_WAKEUP_IT, it_prio);
	(void)itr_set_pmr(pmr);
}

const char *stm32mp_pm_hint2mode_name(uint32_t pm_hint)
{
	assert(pm_hint < ARRAY_SIZE(config_pwr));

	return config_pwr[pm_hint].regul_suspend_node_name;
}

const char *plat_get_lp_mode_name(int mode)
{
	return stm32mp_pm_hint2mode_name(mode);
}

#define CONSOLE_FLUSH_DELAY_MS		10

#if TRACE_LEVEL >= TRACE_DEBUG
static void wait_console_flushed(void)
{
	console_flush();
	mdelay(CONSOLE_FLUSH_DELAY_MS);
}
#else
static void wait_console_flushed(void)
{
}
#endif

static void cpu_wfi(void)
{
	dsb();
	isb();
	wfi();
}

void stm32_pm_cpu_wfi(void)
{
	wait_console_flushed();
	cpu_wfi();
}

/*
 * stm32_enter_cstop - Prepare CSTOP mode
 *
 * @mode - Target low power mode
 */
void stm32_enter_cstop(uint32_t mode)
{
	uint32_t pwr_cr1 = config_pwr[mode].pwr_cr1;
	vaddr_t pwr_base = stm32_pwr_base();
	vaddr_t rcc_base = stm32_rcc_base();

	IMSG("Enter cstop mode %u", mode);

#ifdef CFG_STM32MP15
	if (mode == STM32_PM_CSTOP_ALLOW_LPLV_STOP2)
		panic("LPLV-Stop2 mode not supported");
#endif

#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	/* Save Self-Refresh (SR) mode and switch to Software SR mode */
	ddr_save_sr_mode(DDR_SSR_MODE);
#endif

	if (stm32mp_with_pmic() && (mode == STM32_PM_CSTOP_ALLOW_LP_STOP ||
				    mode == STM32_PM_CSTOP_ALLOW_LPLV_STOP ||
				    mode == STM32_PM_CSTOP_ALLOW_LPLV_STOP2))
		pwr_cr1 |= PWR_CR1_LPCFG;

	if (mode == STM32_PM_CSTOP_ALLOW_LPLV_STOP2) {
		struct clk *hsi_clk = stm32mp_rcc_clock_id_to_clk(CK_HSI);
		unsigned long freq = clk_get_rate(hsi_clk);
		/* Wait for 2ms before re-enabling PLLs after LPLV-Stop2 mode */
		uint32_t dly = 2U * freq / 1000U;

		io_clrsetbits32(rcc_base + RCC_PWRLPDLYCR,
				RCC_PWRLPDLYCR_PWRLP_DLY_MASK,
				dly);
	} else {
		/* Wait 6 HSI periods after other Stop modes */
		io_clrsetbits32(rcc_base + RCC_PWRLPDLYCR,
				RCC_PWRLPDLYCR_PWRLP_DLY_MASK,
				PWRLP_TEMPO_5_HSI);
	}

	/* Workaround for non secure cache issue: this should not be needed */
	dcache_op_all(DCACHE_OP_CLEAN_INV);

	/* Clear RCC interrupt before enabling it */
	io_setbits32(rcc_base + RCC_MP_CIFR, RCC_MP_CIFR_WKUPF);

	/* Enable RCC Wake-up */
	io_setbits32(rcc_base + RCC_MP_CIER, RCC_MP_CIFR_WKUPF);

	/* Configure low power mode */
	io_clrsetbits32(pwr_base + PWR_MPUCR_OFF, PWR_MPUCR_MASK,
			config_pwr[mode].pwr_mpucr);
	io_clrsetbits32(pwr_base + PWR_CR1_OFF, PWR_CR1_MASK, pwr_cr1);

	/* Clear RCC pending interrupt flags */
	io_write32(rcc_base + RCC_MP_CIFR, RCC_MP_CIFR_MASK);

#ifdef CFG_STM32MP13
	/* Request CSTOP mode to RCC */
	io_setbits32(rcc_base + RCC_MP_SREQSETR, RCC_MP_SREQSETR_STPREQ_P0);
#else
	io_setbits32(rcc_base + RCC_MP_SREQSETR,
		     RCC_MP_SREQSETR_STPREQ_P0 | RCC_MP_SREQSETR_STPREQ_P1);

	watchdog_ping();
#endif

	set_rcc_it_priority(&gicd_rcc_wakeup, &gicc_pmr);

#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	if (ddr_standby_sr_entry() != 0)
		panic();
#endif

	if (mode == STM32_PM_CSTOP_ALLOW_STANDBY_DDR_SR) {
		/* set POPL to 20ms */
		io_clrsetbits32(pwr_base + PWR_CR3_OFF, PWR_CR3_POPL_MASK,
				20U << PWR_CR3_POPL_SHIFT);

#ifdef CFG_STM32MP15
		if (stm32mp1_is_retram_during_standby()) {
			uint64_t to = 0;

			/* Keep retention in standby */
			to = timeout_init_us(TIMEOUT_US_10MS);
			io_setbits32(pwr_base + PWR_CR2_OFF, PWR_CR2_RREN);

			while (!(io_read32(pwr_base + PWR_CR2_OFF) & PWR_CR2_RRRDY))
				if (timeout_elapsed(to))
					panic();
		}
#endif
	}
}

/*
 * stm32_exit_cstop - Exit from CSTOP mode
 */
void stm32_exit_cstop(void)
{
	vaddr_t rcc_base = stm32_rcc_base();

#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	if (ddr_standby_sr_exit())
		panic();

	/* Restore Self-Refresh mode saved in stm32_enter_cstop() */
	ddr_restore_sr_mode();
#endif

	restore_rcc_it_priority(gicd_rcc_wakeup, gicc_pmr);

#ifdef CFG_STM32MP13
	/* Disable STOP request */
	io_setbits32(rcc_base + RCC_MP_SREQCLRR, RCC_MP_SREQSETR_STPREQ_P0);
#else
	/* Disable STOP request */
	io_setbits32(rcc_base + RCC_MP_SREQCLRR,
			RCC_MP_SREQSETR_STPREQ_P0 | RCC_MP_SREQSETR_STPREQ_P1);
#endif

	/* Disable RCC Wake-up */
	io_clrbits32(rcc_base + RCC_MP_CIER, RCC_MP_CIFR_WKUPF);

	dsb();
	isb();

#ifndef CFG_STM32MP13
	/* Disable retention and backup RAM content after stop */
	io_clrbits32(stm32_pwr_base() + PWR_CR2_OFF, PWR_CR2_RREN);
#endif
}

/*
 * stm32_enter_cstop_shutdown - Shutdown CPUs to target low power mode
 * @mode - Target low power mode
 */
void __noreturn stm32_enter_cstop_shutdown(uint32_t mode)
{
#ifdef CFG_STM32MP15
	if (stm32mp_supports_second_core() && (get_core_pos() == 0)) {
		/* Prepare CPU reset */
		io_setbits32(stm32_rcc_base() + RCC_MP_GRSTCSETR,
			     RCC_MP_GRSTCSETR_MPUP1RST);

		itr_raise_sgi(GIC_SEC_SGI_1, TARGET_CPU1_GIC_MASK);
	}
#endif

	switch (mode) {
	case STM32_PM_SHUTDOWN:
		if (stm32mp_with_pmic()) {
			wait_console_flushed();
			stm32mp_pm_get_pmic();
			stpmic1_switch_off();
			udelay(100);
		}
		break;
	case STM32_PM_CSTOP_ALLOW_STANDBY_DDR_OFF:
		stm32mp_pm_shutdown_context();
		stm32_enter_cstop(mode);
		dsb();
		isb();
		for ( ; ; )
			wfi();
		break;
	default:
		break;
	}

	panic();
}

/*
 * stm32_enter_cstop_reset - Reset CPUs to target low power mode
 * @mode - Target low power mode
 */
void __noreturn stm32_enter_cstop_reset(uint32_t mode)
{
	switch (mode) {
	case STM32_PM_SHUTDOWN:
		stm32_reset_system();
		udelay(100);
		break;
	default:
		IMSG("Forced system reset");
		wait_console_flushed();
		stm32_reset_system();
		udelay(100);
		break;
	}

	panic();
}

/*
 * stm32_enter_csleep - enter CSLEEP state while WFI and exit in CRUN
 *
 * Configure PWR for CSLEEP state. CPU shall execute a WFI and return
 * once a interrupt is pending.
 */
void stm32_enter_csleep(void)
{
	vaddr_t pwr_base = stm32_pwr_base();

	io_clrsetbits32(pwr_base + PWR_MPUCR_OFF, PWR_MPUCR_MASK,
			config_pwr[STM32_PM_CSLEEP_RUN].pwr_mpucr);
	io_clrsetbits32(pwr_base + PWR_CR1_OFF, PWR_CR1_MASK,
			config_pwr[STM32_PM_CSLEEP_RUN].pwr_cr1);

	stm32_pm_cpu_wfi();
}

/* RCC Wakeup interrupt is used to wake from suspeneded mode */
static enum itr_return rcc_wakeup_it_handler(struct itr_handler *hdl __unused)
{
	/* This interrupt is not expected to be handled */
	panic("RCC  wakeup interrupt");
	return ITRR_HANDLED;
}

static struct itr_handler rcc_wakeup_handler = {
	.it = RCC_WAKEUP_IT,
	.handler = rcc_wakeup_it_handler,
};
DECLARE_KEEP_PAGER(rcc_wakeup_handler);

static TEE_Result init_low_power(void)
{
	vaddr_t pwr_base __maybe_unused = stm32_pwr_base();
	vaddr_t rcc_base = stm32_rcc_base();
	uint64_t to = 0;

	itr_add(&rcc_wakeup_handler);
	itr_enable(rcc_wakeup_handler.it);

#ifdef CFG_STM32MP13
	/* Disable STOP request */
	io_setbits32(rcc_base + RCC_MP_SREQCLRR, RCC_MP_SREQSETR_STPREQ_P0);
#else
	/*
	 * Configure Standby mode available for MCU by default
	 * and allow to switch in standby SoC in all case
	 */
	io_setbits32(pwr_base + PWR_MCUCR_OFF, PWR_MCUCR_PDDS);

	/* Disable STOP request */
	io_setbits32(rcc_base + RCC_MP_SREQCLRR,
		     RCC_MP_SREQSETR_STPREQ_P0 | RCC_MP_SREQSETR_STPREQ_P1);
#endif

	/* Keep backup RAM content in standby */
	io_setbits32(pwr_base + PWR_CR2_OFF, PWR_CR2_BREN);

	to = timeout_init_us(TIMEOUT_US_10MS);
	while (!(io_read32(pwr_base + PWR_CR2_OFF) & PWR_CR2_BRRDY))
		if (timeout_elapsed(to))
			panic();

	return TEE_SUCCESS;
}
driver_init_late(init_low_power);

/*
 * CPU low power sequences
 */
void __noreturn stm32_pm_cpu_power_down_wfi(void)
{
	vaddr_t __maybe_unused rcc_base = stm32_rcc_base();

	if (get_core_pos() == 0) {
		void (*reset_ep)(void) = stm32mp_sysram_resume;

		stm32_pm_cpu_wfi();

		/* STANDBY not reached: resume from retained SYSRAM */
		stm32_exit_cstop();
		stm32mp_cpu_reset_state();
		reset_ep();
		panic();
	}

	dcache_op_level1(DCACHE_OP_CLEAN);
#ifdef CFG_STM32MP13
	panic();
#else
	io_write32(rcc_base + RCC_MP_GRSTCSETR, RCC_MP_GRSTCSETR_MPUP1RST);
#endif
	cpu_wfi();
	panic();
}
