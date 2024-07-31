// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2017-2022, STMicroelectronics
 * Copyright (c) 2016-2022, Linaro Limited
 */

#include <config.h>
#include <console.h>
#include <drivers/clk_dt.h>
#include <drivers/counter.h>
#include <drivers/gic.h>
#include <drivers/rstctrl.h>
#include <drivers/stm32_bsec.h>
#include <drivers/stm32_rif.h>
#include <drivers/stm32_risab.h>
#include <drivers/stm32_risaf.h>
#include <drivers/stm32_serc.h>
#include <drivers/stm32_tamp.h>
#include <drivers/stm32_uart.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp25_pwr.h>
#include <drivers/stm32mp25_rcc.h>
#include <initcall.h>
#include <kernel/abort.h>
#include <kernel/dt.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/interrupt.h>
#include <kernel/misc.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <platform_config.h>
#include <stm32_util.h>
#include <trace.h>
#include <util.h>

register_phys_mem_pgdir(MEM_AREA_IO_NSEC, APB1_BASE, APB1_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_NSEC, APB2_BASE, APB2_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_NSEC, APB3_BASE, APB3_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_NSEC, APB4_BASE, APB4_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_NSEC, AHB4_BASE, AHB4_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_NSEC, AHB5_BASE, AHB5_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_NSEC, SAHB_BASE, SAHB_SIZE);

register_phys_mem_pgdir(MEM_AREA_IO_SEC, APB1_BASE, APB1_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, APB2_BASE, APB2_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, APB3_BASE, APB3_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, APB4_BASE, APB4_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, AHB2_BASE, AHB2_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, AHB3_BASE, AHB3_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, AHB4_BASE, AHB4_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, AHB5_BASE, AHB5_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, SAPB_BASE, SAPB_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, SAHB_BASE, SAHB_SIZE);

register_phys_mem_pgdir(MEM_AREA_IO_SEC, GIC_BASE, GIC_SIZE);

register_phys_mem_pgdir(MEM_AREA_IO_NSEC, DBGMCU_BASE, DBGMCU_SIZE);

register_ddr(DDR_BASE, CFG_DRAM_SIZE);

/* Map beginning SRAM1 as read write for BSEC shadow */
register_phys_mem_pgdir(MEM_AREA_RAM_SEC, SRAM1_BASE, SIZE_4K);

static struct gic_data gic_data;

#define _ID2STR(id)		(#id)
#define ID2STR(id)		_ID2STR(id)

static TEE_Result platform_banner(void)
{
	IMSG("Platform stm32mp2: flavor %s - DT %s",
		ID2STR(PLATFORM_FLAVOR),
		ID2STR(CFG_EMBED_DTB_SOURCE_FILE));

	IMSG("OP-TEE ST profile: %s", TO_STR(CFG_STM32MP_PROFILE));

	return TEE_SUCCESS;
}
service_init(platform_banner);

/*
 * Console
 *
 * CFG_STM32_EARLY_CONSOLE_UART specifies the ID of the UART used for
 * trace console. Value 0 disables the early console.
 *
 * We cannot use the generic serial_console support since probing
 * the console requires the platform clock driver to be already
 * up and ready which is done only once service_init are completed.
 */
static struct stm32_uart_pdata console_data;

void console_init(void)
{
	/* Early console initialization before MMU setup */
	struct uart {
		paddr_t pa;
		bool secure;
	} uarts[] = {
		[0] = { .pa = 0 },
		[1] = { .pa = USART1_BASE, .secure = true, },
		[2] = { .pa = USART2_BASE, .secure = false, },
		[3] = { .pa = USART3_BASE, .secure = false, },
		[4] = { .pa = UART4_BASE, .secure = false, },
		[5] = { .pa = UART5_BASE, .secure = false, },
		[6] = { .pa = USART6_BASE, .secure = false, },
		[7] = { .pa = UART7_BASE, .secure = false, },
		[8] = { .pa = UART8_BASE, .secure = false, },
		[9] = { .pa = UART9_BASE, .secure = false, },
	};

	COMPILE_TIME_ASSERT(ARRAY_SIZE(uarts) > CFG_STM32_EARLY_CONSOLE_UART);

	if (!uarts[CFG_STM32_EARLY_CONSOLE_UART].pa)
		return;

	/* No clock yet bound to the UART console */
	console_data.clock = NULL;

	console_data.secure = uarts[CFG_STM32_EARLY_CONSOLE_UART].secure;
#if CFG_STM32_UART
	stm32_uart_init(&console_data, uarts[CFG_STM32_EARLY_CONSOLE_UART].pa);
#endif
	register_serial_console(&console_data.chip);

	IMSG("Early console on UART#%u", CFG_STM32_EARLY_CONSOLE_UART);
}

#ifdef CFG_STM32_UART
static TEE_Result init_console_from_dt(void)
{
	struct stm32_uart_pdata *pd = NULL;
	void *fdt = NULL;
	int node = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	fdt = get_embedded_dt();
	res = get_console_node_from_dt(fdt, &node, NULL, NULL);
	if (res == TEE_ERROR_ITEM_NOT_FOUND) {
		fdt = get_external_dt();
		res = get_console_node_from_dt(fdt, &node, NULL, NULL);
		if (res == TEE_ERROR_ITEM_NOT_FOUND)
			return TEE_SUCCESS;
		if (res != TEE_SUCCESS)
			return res;
	}

	pd = stm32_uart_init_from_dt_node(fdt, node);
	if (!pd) {
		IMSG("DTB disables console");
		register_serial_console(NULL);
		return TEE_SUCCESS;
	}

	/* Replace early console with the new one */
	console_flush();
	console_data = *pd;
	register_serial_console(&console_data.chip);
	IMSG("DTB enables console (%ssecure)", pd->secure ? "" : "non-");
	free(pd);

	return TEE_SUCCESS;
}

/* Probe console from DT once clock inits (service init level) are completed */
service_init_late(init_console_from_dt);
#endif /*STM32_UART*/

vaddr_t stm32_rcc_base(void)
{
	static struct io_pa_va base = { .pa = RCC_BASE };

	return io_pa_or_va_secure(&base, 1);
}

void main_init_gic(void)
{
	gic_init(&gic_data, GIC_BASE + GICC_OFFSET, GIC_BASE + GICD_OFFSET);
	itr_init(&gic_data.chip);
}

void main_secondary_init_gic(void)
{
	gic_cpu_init(&gic_data);
}

void itr_core_handler(void)
{
	gic_it_handle(&gic_data);
}

uint32_t may_spin_lock(unsigned int *lock)
{
	if (!lock || !cpu_mmu_enabled())
		return 0;

	return cpu_spin_lock_xsave(lock);
}

void may_spin_unlock(unsigned int *lock, uint32_t exceptions)
{
	if (!lock || !cpu_mmu_enabled())
		return;

	cpu_spin_unlock_xrestore(lock, exceptions);
}

#ifdef CFG_STM32_RIF
void stm32_rif_access_violation_action(void)
{
#ifdef CFG_STM32_RISAF
	stm32_risaf_dump_erroneous_data();
	stm32_risaf_clear_illegal_access_flags();
#endif
#ifdef CFG_STM32_RISAB
	stm32_risab_dump_erroneous_data();
	stm32_risab_clear_illegal_access_flags();
#endif
}
#endif /* CFG_STM32_RIF */

TEE_Result stm32_rif_reconfigure_mem_region(const void *fdt, int phandle,
					    void **config)
{
	int node = fdt_node_offset_by_phandle(fdt, phandle);
	TEE_Result res = TEE_ERROR_GENERIC;
	paddr_t reg_base = 0;
	size_t reg_size = 0;

	if (node < 0) {
		EMSG("Could not get node offset for mem region, res=%d", node);
		return TEE_ERROR_GENERIC;
	}

	reg_base = _fdt_reg_base_address(fdt, node);
	reg_size = _fdt_reg_size(fdt, node);
	if (reg_base == DT_INFO_INVALID_REG ||
	    reg_size == DT_INFO_INVALID_REG_SIZE)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Only allow RISAF4 and RISABx reconfigurations */
	if (reg_base >= 0xa000000 && reg_base < 0xa0c0000)
		res = stm32_risab_reconfigure_region(reg_base, reg_size,
						     config);
	else if (reg_base >= CFG_DRAM_BASE)
		res = stm32_risaf_reconfigure_region(reg_base, reg_size,
						     config);
	else
		return TEE_ERROR_ACCESS_DENIED;

	return res;
}

#ifdef CFG_STM32_BSEC3
void plat_bsec_get_static_cfg(struct stm32_bsec_static_cfg *cfg)
{
	cfg->base = BSEC3_BASE;
	cfg->shadow = SRAM1_BASE;
	cfg->upper_start = STM32MP2_UPPER_OTP_START;
	cfg->max_id = STM32MP2_OTP_MAX_ID;
}

#define BSEC3_DEBUG_ALL		GENMASK_32(11, 1)
static TEE_Result init_debug(void)
{
	TEE_Result res = TEE_SUCCESS;
	struct clk *dbg_clk = stm32mp_rcc_clock_id_to_clk(CK_SYSDBG);
	struct clk *flexgen_45_clk = stm32mp_rcc_clock_id_to_clk(CK_FLEXGEN_45);
	uint32_t state = 0;

	res = stm32_bsec_get_state(&state);
	if (res)
		return res;

	if (state != BSEC_STATE_SEC_CLOSED) {
		if (IS_ENABLED(CFG_WARN_INSECURE))
			IMSG("WARNING: All debug access are allowed");

		res = stm32_bsec_write_debug_conf(BSEC3_DEBUG_ALL);
		if (res)
			panic("Debug configuration failed");

		/* Enable DBG as used to access coprocessor debug registers */
		assert(dbg_clk);
		if (clk_enable(dbg_clk))
			panic("Could not enable debug clock");
	}

	if (stm32_bsec_self_hosted_debug_is_enabled()) {
		/* Enable flexgen 45 clock (ck_sys_atb / ck_icn_m_etr) */
		assert(flexgen_45_clk);
		if (clk_enable(flexgen_45_clk))
			panic("Could not enable flexgen45 clock");
	}

	return res;
}
early_init_late(init_debug);
#endif

#ifdef CFG_STM32_CPU_OPP
bool stm32mp_supports_cpu_opp(uint32_t opp_id)
{
	static uint32_t part_number;
	uint32_t otp = 0;
	size_t bit_len = 0;
	uint32_t id = 0;

	if (stm32_bsec_find_otp_in_nvmem_layout("part_number_otp",
						&otp, NULL, &bit_len))
		return false;

	if (stm32_bsec_read_otp(&part_number, otp))
		return -1;

	/* The bit 31 indicate support of 1.5GHz in RPN (variant d/f) */
	if (part_number & BIT(31))
		id = BIT(1);
	else
		id = BIT(0);

	return (opp_id & id) == id;
}
#endif /* CFG_STM32_CPU_OPP */

static bool stm32mp_supports_second_core(void)
{
	if (CFG_TEE_CORE_NB_CORE == 1)
		return false;

	return true;
}

/* SGI9 (secure SGI 1) informs targeted CPU it shall reset */
static enum itr_return sgi9_it_handler(struct itr_handler *hdl  __unused)
{
	DMSG("Halting CPU %zu", get_core_pos());

	while (true)
		cpu_idle();

	return ITRR_HANDLED;
}

static struct itr_handler sgi9_reset_handler = {
	.it = GIC_SEC_SGI_1,
	.handler = sgi9_it_handler,
};

void __noreturn plat_panic(void)
{
	if (stm32mp_supports_second_core()) {
		uint32_t target_mask = 0;

		if (get_core_pos() == 0)
			target_mask = TARGET_CPU1_GIC_MASK;
		else
			target_mask = TARGET_CPU0_GIC_MASK;

		itr_raise_sgi(GIC_SEC_SGI_1, target_mask);
	}

	while (true)
		cpu_idle();
}

static TEE_Result setup_multi_core_panic(void)
{
	itr_add(&sgi9_reset_handler);
	itr_enable(sgi9_reset_handler.it);

	return TEE_SUCCESS;
}
service_init(setup_multi_core_panic);

void plat_abort_handler(struct thread_abort_regs *regs __unused)
{
	/* If fault is ignored, it could be due to a SERC event */
	stm32_serc_handle_ilac();
}

void __noreturn do_reset(const char *str __maybe_unused)
{
	struct rstctrl *rstctrl = NULL;

	if (stm32mp_supports_second_core()) {
		uint32_t target_mask = 0;

		if (get_core_pos() == 0)
			target_mask = TARGET_CPU1_GIC_MASK;
		else
			target_mask = TARGET_CPU0_GIC_MASK;

		itr_raise_sgi(GIC_SEC_SGI_1, target_mask);
		/* Wait that other core is halted */
		mdelay(1);
	}

	IMSG("Forced system reset %s", str);
	console_flush();

	/* Request system reset to RCC driver */
	rstctrl = stm32mp_rcc_reset_id_to_rstctrl(SYS_R);
	rstctrl_assert(rstctrl);
	udelay(100);

	/* Can't occur */
	panic();
}

/* Activate the SoC resources required by internal TAMPER */
TEE_Result stm32_activate_internal_tamper(int id)
{
	TEE_Result res = TEE_ERROR_NOT_SUPPORTED;

	switch (id) {
	case INT_TAMP1: /* Backup domain (V08CAP) voltage monitoring */
	case INT_TAMP2: /* Temperature monitoring */
		stm32mp_pwr_monitoring_enable(PWR_MON_V08CAP_TEMP);
		res = TEE_SUCCESS;
		break;

	case INT_TAMP3: /* LSE monitoring (LSECSS) */
		if (io_read32(stm32_rcc_base() + RCC_BDCR) & RCC_BDCR_LSECSSON)
			res = TEE_SUCCESS;
		break;

	case INT_TAMP4: /* HSE monitoring (CSS + over frequency detection) */
		if (io_read32(stm32_rcc_base() + RCC_OCENSETR) &
		    RCC_OCENSETR_HSECSSON)
			res = TEE_SUCCESS;
		break;

	case INT_TAMP7: /* VDDCORE monitoring under/over voltage */
		stm32mp_pwr_monitoring_enable(PWR_MON_VCORE);
		res = TEE_SUCCESS;
		break;

	case INT_TAMP12: /* VDDCPU (Cortex A35) monitoring under/over voltage */
		stm32mp_pwr_monitoring_enable(PWR_MON_VCPU);
		res = TEE_SUCCESS;
		break;

	case INT_TAMP5:
	case INT_TAMP6:
	case INT_TAMP8:
	case INT_TAMP9:
	case INT_TAMP10:
	case INT_TAMP11:
	case INT_TAMP13:
	case INT_TAMP14:
	case INT_TAMP15:
		res = TEE_SUCCESS;
		break;

	default:
		break;
	}

	return res;
}

#ifdef CFG_STM32_HSE_MONITORING
/* pourcent rate of hse alarm */
#define HSE_ALARM_PERCENT	110
#define FREQ_MONITOR_COMPAT	"st,freq-monitor"

static void stm32_hse_over_frequency(uint32_t ticks __unused,
				     void *user_data __unused)
{
	EMSG("HSE over frequency: nb ticks:%"PRIu32, ticks);
}
DECLARE_KEEP_PAGER(stm32_hse_over_frequency);

static TEE_Result stm32_hse_monitoring(void)
{
	struct counter_device *counter = NULL;
	struct clk *hse_clk = NULL;
	struct clk *hsi_clk = NULL;
	unsigned long hse = 0;
	unsigned long hsi_cal = 0;
	uint32_t ticks = 0;
	void *fdt = NULL;
	void *config = NULL;
	int node = -1;
	TEE_Result res = TEE_ERROR_GENERIC;

	fdt = get_embedded_dt();
	if (!fdt)
		panic();

	node = fdt_node_offset_by_compatible(fdt, 0, FREQ_MONITOR_COMPAT);
	if (node == -FDT_ERR_NOTFOUND)
		return TEE_SUCCESS;
	if (node < 0)
		panic();

	if (_fdt_get_status(fdt, node) == DT_STATUS_DISABLED)
		return TEE_SUCCESS;

	res = clk_dt_get_by_name(fdt, node, "hse", &hse_clk);
	if  (res)
		return res;

	res = clk_dt_get_by_name(fdt, node, "hsi", &hsi_clk);
	if  (res)
		return res;

	hse = clk_get_rate(hse_clk);
	hsi_cal = clk_get_rate(hsi_clk);

	/*
	 * hsi_cal is based on hsi & DIVISOR
	 * DIVISOR is fixed (1024)
	 */
	hsi_cal /= 1024;

	ticks = (hse / 100) * HSE_ALARM_PERCENT;
	ticks /= hsi_cal;

	DMSG("HSE:%luHz HSI cal:%luHz alarm:%"PRIu32, hse, hsi_cal, ticks);

	counter = fdt_counter_get(fdt, node, &config);
	assert(counter && config);

	counter->alarm.callback = stm32_hse_over_frequency;
	counter->alarm.ticks = ticks;

	counter_start(counter, config);
	counter_set_alarm(counter);

	return TEE_SUCCESS;
}

driver_init_late(stm32_hse_monitoring);
#endif /* CFG_STM32_HSE_MONITORING */
