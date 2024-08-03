// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2023, STMicroelectronics
 */

#include <drivers/clk.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp25_rcc.h>
#include <drivers/stm32mp2_rcc_util.h>
#include <kernel/misc.h>
#include <kernel/pm.h>
#include <kernel/thread.h>
#include <mm/core_memprot.h>
#include <sm/psci.h>
#include <stm32mp_pm.h>

#define DBGMCU_CR			U(0x4)
#define DBGMCU_CR_DBG_SLEEP		BIT(0)
#define DBGMCU_CR_DBG_STOP		BIT(1)
#define DBGMCU_CR_DBG_STANDBY		BIT(2)

static uintptr_t stm32_dbgmcu_base(void)
{
	static struct io_pa_va dbgmcu_base = { .pa = DBGMCU_BASE };

	return io_pa_or_va_nsec(&dbgmcu_base, 1);
}

/*
 * Handle low-power emulation mode
 * In PWR debug Standby mode, the low power modes is not signaled to external
 * regulator with signals PWR_CPU_ON and PWR_ON, by example to STPMIC25,
 * and the regulators required by the boot devices are not restored after
 * the simulated low power modes.
 * To avoid a wake-up issue in Rom Code, we force the regulator configuration
 * with a pulse on NRSTC1MS pin. This pulse reset the external mass-storage
 * devices used by CPU1 and restore the regulator configuration. On ST boards
 * with STPMIC25, the NRSTC1MS pin is connected to PWRCTRL3 used to managed the
 * regulator required by ROM code to reboot.
 */
static void stm32_debug_standby(unsigned long a0)
{
	struct clk *dbgmcu_clk = stm32mp_rcc_clock_id_to_clk(CK_ICN_APBDBG);
	uint32_t dbgmcu_cr = U(0);
	static bool info_displayed;

	if (clk_enable(dbgmcu_clk))
		return;

	dbgmcu_cr = io_read32(stm32_dbgmcu_base() + DBGMCU_CR);

	/* WARN ONCE when emulation debug is activated */
	if (!info_displayed &&
	    dbgmcu_cr & (DBGMCU_CR_DBG_SLEEP | DBGMCU_CR_DBG_STOP |
			 DBGMCU_CR_DBG_STANDBY)) {
		MSG("Low power emulation mode enable (DBGMCU_CR=%#"PRIx32")",
		    dbgmcu_cr);
		info_displayed = true;
	}

	/* Pulse on NRSTC1MS pin for standby request */
	if (dbgmcu_cr & DBGMCU_CR_DBG_STANDBY && a0 >= PM_D2_LPLV_LEVEL)
		io_setbits32(stm32_rcc_base() + RCC_C1MSRDCR,
			     RCC_C1MSRDCR_C1MSRST);

	clk_disable(dbgmcu_clk);
}

/**
 * @brief   Handler for system off
 *
 * @param[in] a0   Unused
 * @param[in] a1   Unused
 *
 * @retval 0       if OK, other value else and TF-A will panic
 */
unsigned long thread_system_off_handler(unsigned long a0 __unused,
					unsigned long a1 __unused)
{
	/*
	 * configure targeted mode in PMIC for system OFF,
	 * no need to save context
	 */
	uint32_t pm_hint = PM_HINT_CLOCK_STATE |
		((PM_MAX_LEVEL << PM_HINT_PLATFORM_STATE_SHIFT) &
		  PM_HINT_PLATFORM_STATE_MASK);

	return pm_change_state(PM_OP_SUSPEND, pm_hint);
}

static uint32_t get_pm_hint(unsigned long a0)
{
	uint32_t pm_hint = 0U;

	/* a0 is the highest power level which was powered down. */
	if (a0 < PM_D2_LPLV_LEVEL)
		pm_hint = PM_HINT_CLOCK_STATE;
	else
		pm_hint = PM_HINT_CONTEXT_STATE;

	pm_hint |= ((a0 << PM_HINT_PLATFORM_STATE_SHIFT) &
		    PM_HINT_PLATFORM_STATE_MASK);

	return pm_hint;
}

/**
 * @brief   Handler for cpu resume
 *
 * @param[in] a0   Max power level powered down
 * @param[in] a1   Unused
 *
 * @retval 0       if OK, other value else and TF-A will panic
 */
unsigned long thread_cpu_resume_handler(unsigned long a0,
					unsigned long a1 __unused)
{
	TEE_Result retstatus = TEE_SUCCESS;

	retstatus = pm_change_state(PM_OP_RESUME, get_pm_hint(a0));

	/*
	 * Returned value to the TF-A.
	 * If it is not 0, the system will panic
	 */
	if (retstatus == TEE_SUCCESS)
		return 0;
	else
		return 1;
}

/**
 * @brief   Handler for cpu suspend
 *
 * @param[in] a0   Max power level to power down
 * @param[in] a1   Unused
 *
 * @retval 0       if OK, other value else and TF-A will panic
 */
unsigned long thread_cpu_suspend_handler(unsigned long a0,
					 unsigned long a1 __unused)
{
	TEE_Result retstatus = TEE_SUCCESS;

	retstatus = pm_change_state(PM_OP_SUSPEND, get_pm_hint(a0));

	stm32_debug_standby(a0);

	/*
	 * Returned value to the TF-A.
	 * If it is not 0, the system will panic
	 */
	if (retstatus == TEE_SUCCESS)
		return 0;
	else
		return 1;
}
