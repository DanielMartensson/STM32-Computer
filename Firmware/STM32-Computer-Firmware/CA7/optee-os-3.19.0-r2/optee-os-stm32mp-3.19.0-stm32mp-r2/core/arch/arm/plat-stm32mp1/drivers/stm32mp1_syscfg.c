// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2019-2022, STMicroelectronics
 */

#include <config.h>
#include <drivers/clk.h>
#include <drivers/regulator.h>
#include <drivers/stm32_bsec.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp1_syscfg.h>
#include <initcall.h>
#include <kernel/delay.h>
#include <kernel/pm.h>
#include <mm/core_memprot.h>
#include <stm32_util.h>
#include <io.h>
#include <trace.h>
#include <types_ext.h>

/*
 * SYSCFG register offsets (base relative)
 */
#define SYSCFG_SRAM3ERASER			U(0x10)
#define SYSCFG_SRAM3KR				U(0x14)
#define SYSCFG_IOCTRLSETR			U(0x18)
#define SYSCFG_CMPCR				U(0x20)
#define SYSCFG_CMPENSETR			U(0x24)
#define SYSCFG_CMPENCLRR			U(0x28)
#define SYSCFG_CMPSD1CR				U(0x30)
#define SYSCFG_CMPSD1ENSETR			U(0x34)
#define SYSCFG_CMPSD1ENCLRR			U(0x38)
#define SYSCFG_CMPSD2CR				U(0x40)
#define SYSCFG_CMPSD2ENSETR			U(0x44)
#define SYSCFG_CMPSD2ENCLRR			U(0x48)
#define SYSCFG_HSLVEN0R				U(0x50)
#define SYSCFG_IDC				U(0x380)
#define SYSCFG_IOSIZE				U(0x400)

/*
 * SYSCFG_SRAM3ERASE Register
 */
#define SYSCFG_SRAM3KR_KEY1			U(0xCA)
#define SYSCFG_SRAM3KR_KEY2			U(0x53)

#define SYSCFG_SRAM3ERASER_SRAM3EO		BIT(1)
#define SYSCFG_SRAM3ERASER_SRAM3ER		BIT(0)

#define SYSCFG_SRAM3ERASE_TIMEOUT_US		U(1000)

/*
 * SYSCFG_IOCTRLSETR Register
 */
#define SYSCFG_IOCTRLSETR_HSLVEN_TRACE		BIT(0)
#define SYSCFG_IOCTRLSETR_HSLVEN_QUADSPI	BIT(1)
#define SYSCFG_IOCTRLSETR_HSLVEN_ETH		BIT(2)
#define SYSCFG_IOCTRLSETR_HSLVEN_SDMMC		BIT(3)
#define SYSCFG_IOCTRLSETR_HSLVEN_SPI		BIT(4)

/*
 * SYSCFG_CMPCR Register
 */
#define SYSCFG_CMPCR_SW_CTRL			BIT(1)
#define SYSCFG_CMPCR_READY			BIT(8)
#define SYSCFG_CMPCR_RANSRC			GENMASK_32(19, 16)
#define SYSCFG_CMPCR_RANSRC_SHIFT		U(16)
#define SYSCFG_CMPCR_RAPSRC			GENMASK_32(23, 20)
#define SYSCFG_CMPCR_ANSRC_SHIFT		U(24)

#define SYSCFG_CMPCR_READY_TIMEOUT_US		U(1000)

#define CMPENSETR_OFFSET			U(0x4)
#define CMPENCLRR_OFFSET			U(0x8)

/*
 * SYSCFG_CMPENSETR Register
 */
#define SYSCFG_CMPENSETR_MPU_EN			BIT(0)

/*
 * SYSCFG_IDC Register
 */
#define SYSCFG_IDC_DEV_ID_MASK			GENMASK_32(11, 0)
#define SYSCFG_IDC_REV_ID_MASK			GENMASK_32(31, 16)
#define SYSCFG_IDC_REV_ID_SHIFT			U(16)

/*
 * HSLV definitions
 */
#define SYSCFG_HSLV_KEY				U(0x1018)

static bool vdd_low_voltage;

static vaddr_t get_syscfg_base(void)
{
	static struct io_pa_va base = { .pa = SYSCFG_BASE };

	return io_pa_or_va(&base, SYSCFG_IOSIZE);
}

uint32_t stm32mp_syscfg_get_chip_dev_id(void)
{
	if (IS_ENABLED(CFG_STM32MP13))
		return io_read32(get_syscfg_base() + SYSCFG_IDC) &
		       SYSCFG_IDC_DEV_ID_MASK;

	return 0;
}

static void enable_io_compensation(int cmpcr_offset)
{
	vaddr_t cmpcr_base = get_syscfg_base() + cmpcr_offset;
	uint64_t timeout_ref = 0;

	if (io_read32(cmpcr_base) & SYSCFG_CMPCR_READY)
		return;

	io_setbits32(cmpcr_base + CMPENSETR_OFFSET, SYSCFG_CMPENSETR_MPU_EN);

	timeout_ref = timeout_init_us(SYSCFG_CMPCR_READY_TIMEOUT_US);

	while (!(io_read32(cmpcr_base) & SYSCFG_CMPCR_READY)) {
		if (timeout_elapsed(timeout_ref)) {
			if (!(io_read32(cmpcr_base) & SYSCFG_CMPCR_READY))
				EMSG("IO compensation cell not ready");
			/* Allow an almost silent failure here */
			break;
		}
	}

	io_clrbits32(cmpcr_base, SYSCFG_CMPCR_SW_CTRL);

	DMSG("SYSCFG.cmpcr = %#"PRIx32, io_read32(cmpcr_base));
}

static void disable_io_compensation(int cmpcr_offset)
{
	vaddr_t cmpcr_base = get_syscfg_base() + cmpcr_offset;
	uint32_t value_cmpcr = 0;
	uint32_t apsrc_ansrc = 0;
	uint32_t value_cmpcr2 = 0;

	value_cmpcr = io_read32(cmpcr_base);
	value_cmpcr2 = io_read32(cmpcr_base + CMPENSETR_OFFSET);
	if (!(value_cmpcr & SYSCFG_CMPCR_READY &&
	      value_cmpcr2 & SYSCFG_CMPENSETR_MPU_EN))
		return;

	/* copy apsrc / ansrc in ransrc /rapsrc */
	apsrc_ansrc = value_cmpcr >> SYSCFG_CMPCR_ANSRC_SHIFT;
	value_cmpcr &= ~(SYSCFG_CMPCR_RANSRC | SYSCFG_CMPCR_RAPSRC);
	value_cmpcr |= apsrc_ansrc << SYSCFG_CMPCR_RANSRC_SHIFT;

	io_write32(cmpcr_base, value_cmpcr | SYSCFG_CMPCR_SW_CTRL);

	DMSG("SYSCFG.cmpcr = %#"PRIx32, io_read32(cmpcr_base));

	io_setbits32(cmpcr_base + CMPENCLRR_OFFSET, SYSCFG_CMPENSETR_MPU_EN);
}

TEE_Result stm32mp_syscfg_erase_sram3(void)
{
	vaddr_t base = get_syscfg_base();
	uint64_t timeout_ref = 0;

	if (!IS_ENABLED(CFG_STM32MP13))
		return TEE_ERROR_NOT_SUPPORTED;

	/* Unlock SYSCFG_SRAM3ERASER_SRAM3ER */
	io_write32(base + SYSCFG_SRAM3KR, SYSCFG_SRAM3KR_KEY1);
	io_write32(base + SYSCFG_SRAM3KR, SYSCFG_SRAM3KR_KEY2);

	/* Request SRAM3 erase */
	io_setbits32(base + SYSCFG_SRAM3ERASER, SYSCFG_SRAM3ERASER_SRAM3ER);

	/* Lock SYSCFG_SRAM3ERASER_SRAM3ER */
	io_write32(base + SYSCFG_SRAM3KR, 0);

	/* Wait end of SRAM3 erase */
	timeout_ref = timeout_init_us(SYSCFG_SRAM3ERASE_TIMEOUT_US);
	while (io_read32(base + SYSCFG_SRAM3ERASER) &
	       SYSCFG_SRAM3ERASER_SRAM3EO) {
		if (timeout_elapsed(timeout_ref))
			break;
	}

	/* Timeout may append due to a schedule after the while(test) */
	if (io_read32(base + SYSCFG_SRAM3ERASER) & SYSCFG_SRAM3ERASER_SRAM3EO)
		return TEE_ERROR_BUSY;

	return TEE_SUCCESS;
}

static TEE_Result stm32mp_syscfg_enable_io_compensation(void)
{
	if (clk_enable(stm32mp_rcc_clock_id_to_clk(CK_CSI)) ||
	    clk_enable(stm32mp_rcc_clock_id_to_clk(SYSCFG)))
		panic();

	enable_io_compensation(SYSCFG_CMPCR);

	/* Make sure the write above is visible */
	dsb();

	return TEE_SUCCESS;
}

driver_init(stm32mp_syscfg_enable_io_compensation);

void stm32mp_set_io_comp_by_index(uint32_t index, bool state)
{
	int cmpcr_offset = SYSCFG_CMPSD1CR;

	/* Make sure the previous operations are visible */
	dsb();

	assert(index < SYSCFG_IO_COMP_NB_IDX);

	if (index == SYSCFG_IO_COMP_IDX_SD2)
		cmpcr_offset = SYSCFG_CMPSD2CR;

	if (state)
		enable_io_compensation(cmpcr_offset);
	else
		disable_io_compensation(cmpcr_offset);

	/* Make sure the write above is visible */
	dsb();
}

void stm32mp_set_hslv_by_index(uint32_t index, bool state)
{
	assert(index < SYSCFG_HSLV_NB_IDX);

	/* Make sure the previous operations are visible */
	dsb();

	io_write32(get_syscfg_base() + SYSCFG_HSLVEN0R +
		   index * sizeof(uint32_t), state ? SYSCFG_HSLV_KEY : 0);

	/* Make sure the write above is visible */
	dsb();
}

static void enable_high_speed_mode_low_voltage(void)
{
	if (IS_ENABLED(CFG_STM32MP13_CFG)) {
		unsigned int idx = 0;

		for (idx = 0; idx < SYSCFG_HSLV_NB_IDX; idx++) {
			if (idx != SYSCFG_HSLV_IDX_SDMMC1 &&
			    idx != SYSCFG_HSLV_IDX_SDMMC2)
				stm32mp_set_hslv_by_index(idx, true);
		}
	} else {
		io_write32(get_syscfg_base() + SYSCFG_IOCTRLSETR,
			   SYSCFG_IOCTRLSETR_HSLVEN_TRACE |
			   SYSCFG_IOCTRLSETR_HSLVEN_QUADSPI |
			   SYSCFG_IOCTRLSETR_HSLVEN_ETH |
			   SYSCFG_IOCTRLSETR_HSLVEN_SDMMC |
			   SYSCFG_IOCTRLSETR_HSLVEN_SPI);
	}
}

static TEE_Result syscfg_hslv_pm(enum pm_op op, uint32_t pm_hint __unused,
				 const struct pm_callback_handle
				 *pm_handle __unused)
{
	if (op == PM_OP_RESUME && vdd_low_voltage)
		enable_high_speed_mode_low_voltage();

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(syscfg_hslv_pm);

static TEE_Result stm32mp_syscfg_set_hslv(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t otp_value = 0;
	uint32_t otp_id = 0;
	uint16_t vdd_voltage = 0;
	bool product_below_2v5 = false;
	struct rdev *regu = NULL;

	/*
	 * High Speed Low Voltage Pad mode Enable for SPI, SDMMC, ETH, QSPI
	 * and TRACE. Needed above ~50MHz and conditioned by AFMUX selection.
	 * It could be disabled for low frequencies or if AFMUX is selected
	 * but the function is not used, typically for TRACE.
	 * If high speed low voltage pad mode is enabled, platform will
	 * over consume.
	 *
	 * WARNING:
	 *   Enabling High Speed mode while VDD > 2.7V
	 *   with the OTP product_below_2v5 (OTP 18, BIT 13)
	 *   erroneously set to 1 can damage the SoC!
	 *   => TF-A enables the low power mode only if VDD < 2.7V (in DT)
	 *      but this value needs to be consistent with board design.
	 */
	res = stm32_bsec_find_otp_in_nvmem_layout("hw2_otp", &otp_id,
						  NULL, NULL);
	if (res)
		panic();

	res = stm32_bsec_read_otp(&otp_value, otp_id);
	if (res)
		panic();

	product_below_2v5 = ((otp_value & HW2_OTP_PRODUCT_BELOW_2V5) != 0);

	/* Get VDD supply */
	regu = regulator_get_by_regulator_name("vdd");
	if (!regu)
		panic("VDD regulator not found");

	res = regulator_get_voltage(regu, &vdd_voltage);
	if (res || !vdd_voltage)
		panic("VDD get voltage error");

	/* Check if VDD is Low Voltage */
	if (vdd_voltage < 2700U) {
		vdd_low_voltage = true;
		enable_high_speed_mode_low_voltage();

		if (!product_below_2v5)
			IMSG("Product_below_2v5=0: HSLVEN protected by HW\n");

	} else {
		if (product_below_2v5) {
			EMSG("Product_below_2v5=1:");
			EMSG("\tHSLVEN update is destructive,");
			EMSG("\tno update as VDD > 2.7V");
			panic();
		}
	}

	register_pm_driver_cb(syscfg_hslv_pm, NULL, "stm32mp1-hslv-driver");

	return TEE_SUCCESS;
}

driver_init_late(stm32mp_syscfg_set_hslv);
