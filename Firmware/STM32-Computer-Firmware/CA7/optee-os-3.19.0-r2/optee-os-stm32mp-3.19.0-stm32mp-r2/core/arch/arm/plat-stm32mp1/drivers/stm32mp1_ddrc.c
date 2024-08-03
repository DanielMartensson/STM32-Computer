// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2017-2021, STMicroelectronics - All Rights Reserved
 */

#include <arm32.h>
#include <boot_api.h>
#include <drivers/stm32_rtc.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp1_ddrc.h>
#include <drivers/stm32mp1_pwr.h>
#include <drivers/stm32mp1_rcc.h>
#include <kernel/delay.h>
#include <kernel/panic.h>
#include <io.h>
#include <mm/core_memprot.h>
#include <platform_config.h>
#include <string.h>
#include <trace.h>

/* DDR Controller */
/* DDR Controller registers offsets */
#define DDRCTRL_MSTR				0x000
#define DDRCTRL_STAT				0x004
#define DDRCTRL_MRCTRL0				0x010
#define DDRCTRL_MRSTAT				0x018
#define DDRCTRL_PWRCTL				0x030
#define DDRCTRL_PWRTMG				0x034
#define DDRCTRL_HWLPCTL				0x038
#define DDRCTRL_RFSHCTL3			0x060
#define DDRCTRL_RFSHTMG				0x064
#define DDRCTRL_INIT0				0x0D0
#define DDRCTRL_DFIMISC				0x1B0
#define DDRCTRL_DBG1				0x304
#define DDRCTRL_DBGCAM				0x308
#define DDRCTRL_DBGCMD				0x30C
#define DDRCTRL_DBGSTAT				0x310
#define DDRCTRL_SWCTL				0x320
#define DDRCTRL_SWSTAT				0x324
#define DDRCTRL_PSTAT				0x3FC
#define DDRCTRL_PCTRL_0				0x490
#define DDRCTRL_PCTRL_1				0x540

/* DDR Controller Register fields */
#define DDRCTRL_MSTR_DDR3			BIT(0)
#define DDRCTRL_MSTR_LPDDR2			BIT(2)
#define DDRCTRL_MSTR_LPDDR3			BIT(3)
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_MASK	GENMASK_32(13, 12)
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_FULL	0
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_HALF	BIT(12)
#define DDRCTRL_MSTR_DATA_BUS_WIDTH_QUARTER	BIT(13)
#define DDRCTRL_MSTR_DLL_OFF_MODE		BIT(15)

#define DDRCTRL_STAT_OPERATING_MODE_MASK	GENMASK_32(2, 0)
#define DDRCTRL_STAT_OPERATING_MODE_NORMAL	BIT(0)
#define DDRCTRL_STAT_OPERATING_MODE_SR		(BIT(0) | BIT(1))
#define DDRCTRL_STAT_SELFREF_TYPE_MASK		GENMASK_32(5, 4)
#define DDRCTRL_STAT_SELFREF_TYPE_ASR		(BIT(4) | BIT(5))
#define DDRCTRL_STAT_SELFREF_TYPE_SR		BIT(5)

#define DDRCTRL_MRCTRL0_MR_TYPE_WRITE		0
/* only one rank supported */
#define DDRCTRL_MRCTRL0_MR_RANK_SHIFT		4
#define DDRCTRL_MRCTRL0_MR_RANK_ALL	\
					BIT(DDRCTRL_MRCTRL0_MR_RANK_SHIFT)
#define DDRCTRL_MRCTRL0_MR_ADDR_SHIFT		12
#define DDRCTRL_MRCTRL0_MR_ADDR_MASK		GENMASK_32(15, 12)
#define DDRCTRL_MRCTRL0_MR_WR			BIT(31)

#define DDRCTRL_MRSTAT_MR_WR_BUSY		BIT(0)

#define DDRCTRL_PWRCTL_SELFREF_EN		BIT(0)
#define DDRCTRL_PWRCTL_POWERDOWN_EN		BIT(1)
#define DDRCTRL_PWRCTL_EN_DFI_DRAM_CLK_DISABLE	BIT(3)
#define DDRCTRL_PWRCTL_SELFREF_SW		BIT(5)

#define DDRCTRL_PWRTMG_SELFREF_TO_X32_MASK	GENMASK_32(19, 12)
#define DDRCTRL_PWRTMG_SELFREF_TO_X32_0		BIT(16)

#define DDRCTRL_RFSHCTL3_DIS_AUTO_REFRESH	BIT(0)

#define DDRCTRL_HWLPCTL_HW_LP_EN		BIT(0)

#define DDRCTRL_RFSHTMG_T_RFC_NOM_X1_X32_MASK	GENMASK_32(27, 16)
#define DDRCTRL_RFSHTMG_T_RFC_NOM_X1_X32_SHIFT	16

#define DDRCTRL_INIT0_SKIP_DRAM_INIT_MASK	GENMASK_32(31, 30)
#define DDRCTRL_INIT0_SKIP_DRAM_INIT_NORMAL	BIT(30)

#define DDRCTRL_DFIMISC_DFI_INIT_COMPLETE_EN	BIT(0)

#define DDRCTRL_DBG1_DIS_HIF			BIT(1)

#define DDRCTRL_DBGCAM_WR_DATA_PIPELINE_EMPTY	BIT(29)
#define DDRCTRL_DBGCAM_RD_DATA_PIPELINE_EMPTY	BIT(28)
#define DDRCTRL_DBGCAM_DBG_WR_Q_EMPTY		BIT(26)
#define DDRCTRL_DBGCAM_DBG_LPR_Q_DEPTH		GENMASK_32(12, 8)
#define DDRCTRL_DBGCAM_DBG_HPR_Q_DEPTH		GENMASK_32(4, 0)

#define DDRCTRL_DBGCAM_DATA_PIPELINE_EMPTY \
		(DDRCTRL_DBGCAM_WR_DATA_PIPELINE_EMPTY | \
		 DDRCTRL_DBGCAM_RD_DATA_PIPELINE_EMPTY)

#define DDRCTRL_DBGCAM_DBG_Q_DEPTH \
		(DDRCTRL_DBGCAM_DBG_WR_Q_EMPTY | \
		 DDRCTRL_DBGCAM_DBG_LPR_Q_DEPTH | \
		 DDRCTRL_DBGCAM_DBG_HPR_Q_DEPTH)

#define DDRCTRL_DBGCMD_RANK0_REFRESH		BIT(0)

#define DDRCTRL_DBGSTAT_RANK0_REFRESH_BUSY	BIT(0)

#define DDRCTRL_SWCTL_SW_DONE			BIT(0)

#define DDRCTRL_SWSTAT_SW_DONE_ACK		BIT(0)

#define DDRCTRL_PCTRL_N_PORT_EN			BIT(0)

/* DDR PHY registers offsets */
#define DDRPHYC_PIR				0x004
#define DDRPHYC_PGCR				0x008
#define DDRPHYC_PGSR				0x00C
#define DDRPHYC_DLLGCR				0x010
#define DDRPHYC_ACDLLCR				0x014
#define DDRPHYC_PTR0				0x018
#define DDRPHYC_ACIOCR				0x024
#define DDRPHYC_DXCCR				0x028
#define DDRPHYC_DSGCR				0x02C
#define DDRPHYC_ZQ0CR0				0x180
#define DDRPHYC_DX0GCR				0x1C0
#define DDRPHYC_DX0DLLCR			0x1CC
#define DDRPHYC_DX1GCR				0x200
#define DDRPHYC_DX1DLLCR			0x20C
#define DDRPHYC_DX2GCR				0x240
#define DDRPHYC_DX2DLLCR			0x24C
#define DDRPHYC_DX3GCR				0x280
#define DDRPHYC_DX3DLLCR			0x28C

/* DDR PHY Register fields */
#define DDRPHYC_PIR_INIT			BIT(0)
#define DDRPHYC_PIR_DLLSRST			BIT(1)
#define DDRPHYC_PIR_DLLLOCK			BIT(2)
#define DDRPHYC_PIR_ZCAL			BIT(3)
#define DDRPHYC_PIR_ITMSRST			BIT(4)
#define DDRPHYC_PIR_DRAMRST			BIT(5)
#define DDRPHYC_PIR_DRAMINIT			BIT(6)
#define DDRPHYC_PIR_QSTRN			BIT(7)
#define DDRPHYC_PIR_ICPC			BIT(16)
#define DDRPHYC_PIR_ZCALBYP			BIT(30)
#define DDRPHYC_PIR_INITSTEPS_MASK		GENMASK(31, 7)

#define DDRPHYC_PGCR_DFTCMP			BIT(2)
#define DDRPHYC_PGCR_PDDISDX			BIT(24)
#define DDRPHYC_PGCR_RFSHDT_MASK		GENMASK_32(28, 25)

#define DDRPHYC_PGSR_IDONE			BIT(0)
#define DDRPHYC_PGSR_DTERR			BIT(5)
#define DDRPHYC_PGSR_DTIERR			BIT(6)
#define DDRPHYC_PGSR_DFTERR			BIT(7)
#define DDRPHYC_PGSR_RVERR			BIT(8)
#define DDRPHYC_PGSR_RVEIRR			BIT(9)

#define DDRPHYC_DLLGCR_BPS200			BIT(23)

#define DDRPHYC_ACDLLCR_DLLSRST			BIT(30)
#define DDRPHYC_ACDLLCR_DLLDIS			BIT(31)

#define DDRPHYC_PTR0_TDLLSRST_OFFSET		0
#define DDRPHYC_PTR0_TDLLSRST_MASK		GENMASK_32(5, 0)
#define DDRPHYC_PTR0_TDLLLOCK_OFFSET		6
#define DDRPHYC_PTR0_TDLLLOCK_MASK		GENMASK_32(17, 6)
#define DDRPHYC_PTR0_TITMSRST_OFFSET		18
#define DDRPHYC_PTR0_TITMSRST_MASK		GENMASK_32(21, 18)

#define DDRPHYC_ACIOCR_ACOE			BIT(1)
#define DDRPHYC_ACIOCR_ACPDD			BIT(3)
#define DDRPHYC_ACIOCR_ACPDR			BIT(4)
#define DDRPHYC_ACIOCR_CKPDD_MASK		GENMASK_32(10, 8)
#define DDRPHYC_ACIOCR_CKPDD_0			BIT(8)
#define DDRPHYC_ACIOCR_CKPDR_MASK		GENMASK_32(13, 11)
#define DDRPHYC_ACIOCR_CKPDR_0			BIT(11)
#define DDRPHYC_ACIOCR_CSPDD_MASK		GENMASK_32(21, 18)
#define DDRPHYC_ACIOCR_CSPDD_0			BIT(18)
#define DDRPHYC_ACIOCR_RSTPDD			BIT(27)
#define DDRPHYC_ACIOCR_RSTPDR			BIT(28)

#define DDRPHYC_DXCCR_DXPDD			BIT(2)
#define DDRPHYC_DXCCR_DXPDR			BIT(3)

#define DDRPHYC_DSGCR_CKEPDD_MASK		GENMASK_32(19, 16)
#define DDRPHYC_DSGCR_CKEPDD_0			BIT(16)
#define DDRPHYC_DSGCR_ODTPDD_MASK		GENMASK_32(23, 20)
#define DDRPHYC_DSGCR_ODTPDD_0			BIT(20)
#define DDRPHYC_DSGCR_NL2PD			BIT(24)

#define DDRPHYC_ZQ0CRN_ZDATA_MASK		GENMASK_32(27, 0)
#define DDRPHYC_ZQ0CRN_ZDATA_SHIFT		0
#define DDRPHYC_ZQ0CRN_ZDEN			BIT(28)
#define DDRPHYC_ZQ0CRN_ZQPD			BIT(31)

#define DDRPHYC_DXNGCR_DXEN			BIT(0)

#define DDRPHYC_DXNDLLCR_DLLSRST		BIT(30)
#define DDRPHYC_DXNDLLCR_DLLDIS			BIT(31)
#define DDRPHYC_DXNDLLCR_SDPHASE_MASK		GENMASK(17, 14)
#define DDRPHYC_DXNDLLCR_SDPHASE_SHIFT		14

#define TIMEOUT_500US		500

static enum stm32mp1_ddr_sr_mode saved_ddr_sr_mode;

static vaddr_t get_ddrctrl_base(void)
{
	static struct io_pa_va base __nex_data = { .pa = DDRCTRL_BASE };

	return io_pa_or_va(&base, 1);
}

static vaddr_t get_ddrphy_base(void)
{
	static struct io_pa_va base __nex_data = { .pa = DDRPHYC_BASE };

	return io_pa_or_va(&base, 1);
}

static void ddr_disable_clock(void)
{
	vaddr_t rcc_base = stm32_rcc_base();

	/* Disable all clocks */
	io_clrbits32(rcc_base + RCC_DDRITFCR,
		     RCC_DDRITFCR_DDRC1EN |
		     RCC_DDRITFCR_DDRC2EN |
		     RCC_DDRITFCR_DDRPHYCAPBEN |
		     RCC_DDRITFCR_DDRCAPBEN);
}

static void ddr_enable_clock(void)
{
	vaddr_t rcc_base = stm32_rcc_base();

	/* Enable all clocks */
	io_setbits32(rcc_base + RCC_DDRITFCR,
		     RCC_DDRITFCR_DDRC1EN |
		     RCC_DDRITFCR_DDRC2EN |
		     RCC_DDRITFCR_DDRPHYCEN |
		     RCC_DDRITFCR_DDRPHYCAPBEN |
		     RCC_DDRITFCR_DDRCAPBEN);
}

static void do_sw_handshake(void)
{
	vaddr_t ddrctrl_base = get_ddrctrl_base();

	io_clrbits32(ddrctrl_base + DDRCTRL_SWCTL, DDRCTRL_SWCTL_SW_DONE);
}

static void do_sw_ack(void)
{
	uint64_t timeout_ref = 0;
	vaddr_t ddrctrl_base = get_ddrctrl_base();

	io_setbits32(ddrctrl_base + DDRCTRL_SWCTL, DDRCTRL_SWCTL_SW_DONE);

	timeout_ref = timeout_init_us(TIMEOUT_500US);
	while (!timeout_elapsed(timeout_ref))
		if (io_read32(ddrctrl_base + DDRCTRL_SWSTAT) &
		    DDRCTRL_SWSTAT_SW_DONE_ACK)
			return;

	panic();
}

static int ddr_sw_self_refresh_in(void)
{
	uint64_t timeout_ref = 0;
	uint32_t operating_mode = 0;
	uint32_t selref_type = 0;
	uint8_t op_mode_changed = 0;
	vaddr_t pwr_base = stm32_pwr_base();
	vaddr_t rcc_base = stm32_rcc_base();
	vaddr_t ddrctrl_base = get_ddrctrl_base();
	vaddr_t ddrphy_base = get_ddrphy_base();

	io_clrbits32(rcc_base + RCC_DDRITFCR, RCC_DDRITFCR_AXIDCGEN);

	/* Blocks AXI ports from taking anymore transactions */
	io_clrbits32(ddrctrl_base + DDRCTRL_PCTRL_0, DDRCTRL_PCTRL_N_PORT_EN);
	io_clrbits32(ddrctrl_base + DDRCTRL_PCTRL_1, DDRCTRL_PCTRL_N_PORT_EN);

	/*
	 * Waits unit all AXI ports are idle
	 * Poll PSTAT.rd_port_busy_n = 0
	 * Poll PSTAT.wr_port_busy_n = 0
	 */
	timeout_ref = timeout_init_us(TIMEOUT_500US);
	while (io_read32(ddrctrl_base + DDRCTRL_PSTAT))
		if (timeout_elapsed(timeout_ref))
			goto pstat_failed;

	/* SW Self-Refresh entry */
	io_setbits32(ddrctrl_base + DDRCTRL_PWRCTL, DDRCTRL_PWRCTL_SELFREF_SW);

	/*
	 * Wait operating mode change in self-refresh mode
	 * with STAT.operating_mode[1:0]==11.
	 * Ensure transition to self-refresh was due to software
	 * by checking also that STAT.selfref_type[1:0]=2.
	 */
	timeout_ref = timeout_init_us(TIMEOUT_500US);
	while (!timeout_elapsed(timeout_ref)) {
		uint32_t stat = io_read32(ddrctrl_base + DDRCTRL_STAT);

		operating_mode = stat & DDRCTRL_STAT_OPERATING_MODE_MASK;
		selref_type = stat & DDRCTRL_STAT_SELFREF_TYPE_MASK;

		if (operating_mode == DDRCTRL_STAT_OPERATING_MODE_SR &&
		    selref_type == DDRCTRL_STAT_SELFREF_TYPE_SR) {
			op_mode_changed = 1;
			break;
		}
	}

	if (op_mode_changed == 0U)
		goto selfref_sw_failed;

	/* IOs powering down (PUBL registers) */
	io_setbits32(ddrphy_base + DDRPHYC_ACIOCR, DDRPHYC_ACIOCR_ACPDD);
	io_setbits32(ddrphy_base + DDRPHYC_ACIOCR, DDRPHYC_ACIOCR_ACPDR);
	io_clrsetbits32(ddrphy_base + DDRPHYC_ACIOCR,
			DDRPHYC_ACIOCR_CKPDD_MASK, DDRPHYC_ACIOCR_CKPDD_0);
	io_clrsetbits32(ddrphy_base + DDRPHYC_ACIOCR,
			DDRPHYC_ACIOCR_CKPDR_MASK, DDRPHYC_ACIOCR_CKPDR_0);
	io_clrsetbits32(ddrphy_base + DDRPHYC_ACIOCR,
			DDRPHYC_ACIOCR_CSPDD_MASK, DDRPHYC_ACIOCR_CSPDD_0);
	io_clrbits32(ddrphy_base + DDRPHYC_ACIOCR, DDRPHYC_ACIOCR_ACOE);
	io_setbits32(ddrphy_base + DDRPHYC_DXCCR, DDRPHYC_DXCCR_DXPDD);
	io_setbits32(ddrphy_base + DDRPHYC_DXCCR, DDRPHYC_DXCCR_DXPDR);
	io_clrsetbits32(ddrphy_base + DDRPHYC_DSGCR,
			DDRPHYC_DSGCR_ODTPDD_MASK, DDRPHYC_DSGCR_ODTPDD_0);
	io_setbits32(ddrphy_base + DDRPHYC_DSGCR, DDRPHYC_DSGCR_NL2PD);
	io_clrsetbits32(ddrphy_base + DDRPHYC_DSGCR,
			DDRPHYC_DSGCR_CKEPDD_MASK, DDRPHYC_DSGCR_CKEPDD_0);

	/* Disable PZQ cell (PUBL register) */
	io_setbits32(ddrphy_base + DDRPHYC_ZQ0CR0, DDRPHYC_ZQ0CRN_ZQPD);

	/* Activate sw retention in PWRCTRL */
	io_setbits32(pwr_base + PWR_CR3_OFF, PWR_CR3_DDRRETEN);

	/* Switch controller clocks (uMCTL2/PUBL) to DLL ref clock */
	io_setbits32(rcc_base + RCC_DDRITFCR, RCC_DDRITFCR_GSKPCTRL);

	/* Disable all DLLs: GLITCH window */
	io_setbits32(ddrphy_base + DDRPHYC_ACDLLCR, DDRPHYC_ACDLLCR_DLLDIS);
	io_setbits32(ddrphy_base + DDRPHYC_DX0DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);
	io_setbits32(ddrphy_base + DDRPHYC_DX1DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);
	io_setbits32(ddrphy_base + DDRPHYC_DX2DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);
	io_setbits32(ddrphy_base + DDRPHYC_DX3DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);

	/* Switch controller clocks (uMCTL2/PUBL) to DLL output clock */
	io_clrbits32(rcc_base + RCC_DDRITFCR, RCC_DDRITFCR_GSKPCTRL);

	/* Disable all clocks */
	ddr_disable_clock();

	return 0;

selfref_sw_failed:
	/* This bit should be cleared to restore DDR in its previous state */
	io_clrbits32(ddrctrl_base + DDRCTRL_PWRCTL, DDRCTRL_PWRCTL_SELFREF_SW);

pstat_failed:
	io_setbits32(ddrctrl_base + DDRCTRL_PCTRL_0, DDRCTRL_PCTRL_N_PORT_EN);
	io_setbits32(ddrctrl_base + DDRCTRL_PCTRL_1, DDRCTRL_PCTRL_N_PORT_EN);

	return -1;
}

static int ddr_sw_self_refresh_exit(void)
{
	uint64_t timeout_ref = 0;
	vaddr_t rcc_base = stm32_rcc_base();
	vaddr_t pwr_base = stm32_pwr_base();
	vaddr_t ddrctrl_base = get_ddrctrl_base();
	vaddr_t ddrphy_base = get_ddrphy_base();

	/* Enable all clocks */
	ddr_enable_clock();

	do_sw_handshake();

	/* Mask dfi_init_complete_en */
	io_clrbits32(ddrctrl_base + DDRCTRL_DFIMISC,
		     DDRCTRL_DFIMISC_DFI_INIT_COMPLETE_EN);

	do_sw_ack();

	/* Switch controller clocks (uMCTL2/PUBL) to DLL ref clock */
	io_setbits32(rcc_base + RCC_DDRITFCR, RCC_DDRITFCR_GSKPCTRL);

	/* Enable all DLLs: GLITCH window */
	io_clrbits32(ddrphy_base + DDRPHYC_ACDLLCR, DDRPHYC_ACDLLCR_DLLDIS);
	io_clrbits32(ddrphy_base + DDRPHYC_DX0DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);
	io_clrbits32(ddrphy_base + DDRPHYC_DX1DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);
	io_clrbits32(ddrphy_base + DDRPHYC_DX2DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);
	io_clrbits32(ddrphy_base + DDRPHYC_DX3DLLCR, DDRPHYC_DXNDLLCR_DLLDIS);

	/* Additional delay to avoid early DLL clock switch */
	udelay(50);

	/* Switch controller clocks (uMCTL2/PUBL) to DLL ref clock */
	io_clrbits32(rcc_base + RCC_DDRITFCR, RCC_DDRITFCR_GSKPCTRL);
	io_clrbits32(ddrphy_base + DDRPHYC_ACDLLCR, DDRPHYC_ACDLLCR_DLLSRST);
	udelay(10);
	io_setbits32(ddrphy_base + DDRPHYC_ACDLLCR, DDRPHYC_ACDLLCR_DLLSRST);

	/* PHY partial init: (DLL lock and ITM reset) */
	io_write32(ddrphy_base + DDRPHYC_PIR,
		   DDRPHYC_PIR_DLLSRST | DDRPHYC_PIR_DLLLOCK |
		   DDRPHYC_PIR_ITMSRST | DDRPHYC_PIR_INIT);

	/* Need to wait at least 10 clock cycles before accessing PGSR */
	udelay(10);

	timeout_ref = timeout_init_us(TIMEOUT_500US);
	while (!(io_read32(ddrphy_base + DDRPHYC_PGSR) & DDRPHYC_PGSR_IDONE))
		if (timeout_elapsed(timeout_ref))
			return -1;

	do_sw_handshake();

	/* Unmask dfi_init_complete_en to uMCTL2 */
	io_setbits32(ddrctrl_base + DDRCTRL_DFIMISC,
		     DDRCTRL_DFIMISC_DFI_INIT_COMPLETE_EN);

	do_sw_ack();

	/* Deactivate sw retention in PWR */
	io_clrbits32(pwr_base + PWR_CR3_OFF, PWR_CR3_DDRRETEN);

	/* Enable PZQ cell (PUBL register) */
	io_clrbits32(ddrphy_base + DDRPHYC_ZQ0CR0, DDRPHYC_ZQ0CRN_ZQPD);

	/* Enable pad drivers */
	io_clrbits32(ddrphy_base + DDRPHYC_ACIOCR, DDRPHYC_ACIOCR_ACPDD);
	io_setbits32(ddrphy_base + DDRPHYC_ACIOCR, DDRPHYC_ACIOCR_ACOE);
	io_clrbits32(ddrphy_base + DDRPHYC_ACIOCR, DDRPHYC_ACIOCR_CKPDD_MASK);
	io_clrbits32(ddrphy_base + DDRPHYC_ACIOCR, DDRPHYC_ACIOCR_CSPDD_MASK);
	io_clrbits32(ddrphy_base + DDRPHYC_DXCCR, DDRPHYC_DXCCR_DXPDD);
	io_clrbits32(ddrphy_base + DDRPHYC_DXCCR, DDRPHYC_DXCCR_DXPDR);
	io_clrbits32(ddrphy_base + DDRPHYC_DSGCR, DDRPHYC_DSGCR_ODTPDD_MASK);
	io_clrbits32(ddrphy_base + DDRPHYC_DSGCR, DDRPHYC_DSGCR_NL2PD);
	io_clrbits32(ddrphy_base + DDRPHYC_DSGCR, DDRPHYC_DSGCR_CKEPDD_MASK);

	/* Remove selfrefresh */
	io_clrbits32(ddrctrl_base + DDRCTRL_PWRCTL, DDRCTRL_PWRCTL_SELFREF_SW);

	/* Wait operating_mode == normal */
	timeout_ref = timeout_init_us(TIMEOUT_500US);
	while (1) {
		if ((io_read32(ddrctrl_base + DDRCTRL_STAT) &
		     DDRCTRL_STAT_OPERATING_MODE_MASK) ==
		    DDRCTRL_STAT_OPERATING_MODE_NORMAL)
			break;

		if (timeout_elapsed(timeout_ref))
			return -1;
	}

	/* AXI ports are no longer blocked from taking transactions */
	io_setbits32(ddrctrl_base + DDRCTRL_PCTRL_0, DDRCTRL_PCTRL_N_PORT_EN);
	io_setbits32(ddrctrl_base + DDRCTRL_PCTRL_1, DDRCTRL_PCTRL_N_PORT_EN);

	io_setbits32(rcc_base + RCC_DDRITFCR, RCC_DDRITFCR_AXIDCGEN);

	return 0;
}

uint32_t get_ddrphy_calibration(void)
{
	vaddr_t ddrphy_base = get_ddrphy_base();
	uint32_t zcal = io_read32(ddrphy_base + DDRPHYC_ZQ0CR0);

	return (zcal & DDRPHYC_ZQ0CRN_ZDATA_MASK) >> DDRPHYC_ZQ0CRN_ZDATA_SHIFT;
}

int ddr_standby_sr_entry(void)
{
	vaddr_t pwr_base = stm32_pwr_base();

	if (ddr_sw_self_refresh_in())
		return -1;

	/* Enable I/O retention mode in standby */
	io_setbits32(pwr_base + PWR_CR3_OFF, PWR_CR3_DDRSREN);

	return 0;
}

int ddr_standby_sr_exit(void)
{
	return ddr_sw_self_refresh_exit();
}

static void ddr_sr_mode_ssr(void)
{
	vaddr_t rcc_ddritfcr = stm32_rcc_base() + RCC_DDRITFCR;
	vaddr_t ddrctrl_base = get_ddrctrl_base();

	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC1LPEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC2LPEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC1EN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC2EN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRCAPBLPEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRPHYCAPBLPEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRCAPBEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRPHYCAPBEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRPHYCEN);
	io_clrbits32(rcc_ddritfcr, RCC_DDRITFCR_AXIDCGEN);
	io_clrbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRCKMOD_MASK);

	/* Disable HW LP interface of uMCTL2 */
	io_clrbits32(ddrctrl_base + DDRCTRL_HWLPCTL, DDRCTRL_HWLPCTL_HW_LP_EN);

	/* Configure Automatic LP modes of uMCTL2 */
	io_clrsetbits32(ddrctrl_base + DDRCTRL_PWRTMG,
			DDRCTRL_PWRTMG_SELFREF_TO_X32_MASK,
			DDRCTRL_PWRTMG_SELFREF_TO_X32_0);

	/*
	 * Disable Clock disable with LP modes
	 * (used in RUN mode for LPDDR2 with specific timing).
	 */
	io_clrbits32(ddrctrl_base + DDRCTRL_PWRCTL,
		     DDRCTRL_PWRCTL_EN_DFI_DRAM_CLK_DISABLE);

	/* Disable automatic Self-Refresh mode */
	io_clrbits32(ddrctrl_base + DDRCTRL_PWRCTL,
		     DDRCTRL_PWRCTL_SELFREF_EN);
}

static void ddr_sr_mode_asr(void)
{
	vaddr_t rcc_ddritfcr = stm32_rcc_base() + RCC_DDRITFCR;
	vaddr_t ddrctrl_base = get_ddrctrl_base();

	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_AXIDCGEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC1LPEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC2LPEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRPHYCLPEN);
	io_clrsetbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRCKMOD_MASK,
			RCC_DDRITFCR_DDRCKMOD_ASR1);

	/* Enable HW LP interface of uMCTL2 */
	io_setbits32(ddrctrl_base + DDRCTRL_HWLPCTL, DDRCTRL_HWLPCTL_HW_LP_EN);

	/* Configure Automatic LP modes of uMCTL2 */
	io_clrsetbits32(ddrctrl_base + DDRCTRL_PWRTMG,
			DDRCTRL_PWRTMG_SELFREF_TO_X32_MASK,
			DDRCTRL_PWRTMG_SELFREF_TO_X32_0);

	/*
	 * Enable Clock disable with LP modes
	 * (used in RUN mode for LPDDR2 with specific timing).
	 */
	io_setbits32(ddrctrl_base + DDRCTRL_PWRCTL,
		     DDRCTRL_PWRCTL_EN_DFI_DRAM_CLK_DISABLE);

	/* Enable automatic Self-Refresh for ASR mode */
	io_setbits32(ddrctrl_base + DDRCTRL_PWRCTL,
		     DDRCTRL_PWRCTL_SELFREF_EN);
}

static void ddr_sr_mode_hsr(void)
{
	vaddr_t rcc_ddritfcr = stm32_rcc_base() + RCC_DDRITFCR;
	vaddr_t ddrctrl_base = get_ddrctrl_base();

	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_AXIDCGEN);
	io_clrbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC1LPEN);
	io_clrbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRC2LPEN);
	io_setbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRPHYCLPEN);
	io_clrsetbits32(rcc_ddritfcr, RCC_DDRITFCR_DDRCKMOD_MASK,
			RCC_DDRITFCR_DDRCKMOD_HSR1);

	/* Enable HW LP interface of uMCTL2 */
	io_setbits32(ddrctrl_base + DDRCTRL_HWLPCTL, DDRCTRL_HWLPCTL_HW_LP_EN);

	/* Configure Automatic LP modes of uMCTL2 */
	io_clrsetbits32(ddrctrl_base + DDRCTRL_PWRTMG,
			DDRCTRL_PWRTMG_SELFREF_TO_X32_MASK,
			DDRCTRL_PWRTMG_SELFREF_TO_X32_0);

	/*
	 * Enable Clock disable with LP modes
	 * (used in RUN mode for LPDDR2 with specific timing).
	 */
	io_setbits32(ddrctrl_base + DDRCTRL_PWRCTL,
		     DDRCTRL_PWRCTL_EN_DFI_DRAM_CLK_DISABLE);
}

static enum stm32mp1_ddr_sr_mode ddr_read_sr_mode(void)
{
	uint32_t pwrctl = io_read32(get_ddrctrl_base() + DDRCTRL_PWRCTL);
	uint32_t mask = DDRCTRL_PWRCTL_EN_DFI_DRAM_CLK_DISABLE |
			DDRCTRL_PWRCTL_SELFREF_EN;

	switch (pwrctl & mask) {
	case 0U:
		return DDR_SSR_MODE;

	case DDRCTRL_PWRCTL_EN_DFI_DRAM_CLK_DISABLE:
		return DDR_HSR_MODE;

	case DDRCTRL_PWRCTL_EN_DFI_DRAM_CLK_DISABLE | DDRCTRL_PWRCTL_SELFREF_EN:
		return DDR_ASR_MODE;

	default:
		return DDR_SR_MODE_INVALID;
	}
}

static void ddr_set_sr_mode(enum stm32mp1_ddr_sr_mode mode)
{
	switch (mode) {
	case DDR_SSR_MODE:
		ddr_sr_mode_ssr();
		break;

	case DDR_HSR_MODE:
		ddr_sr_mode_hsr();
		break;

	case DDR_ASR_MODE:
		ddr_sr_mode_asr();
		break;

	default:
		EMSG("Unknown Self Refresh mode %d", mode);
		panic();
	}
}

void ddr_save_sr_mode(enum stm32mp1_ddr_sr_mode mode)
{
	/* Save current mode before setting new one */
	saved_ddr_sr_mode = ddr_read_sr_mode();
	ddr_set_sr_mode(mode);
}

void ddr_restore_sr_mode(void)
{
	ddr_set_sr_mode(saved_ddr_sr_mode);
}
