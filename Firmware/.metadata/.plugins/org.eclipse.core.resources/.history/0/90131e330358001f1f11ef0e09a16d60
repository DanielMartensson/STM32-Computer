// SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0+)
/*
 * Copyright (C) 2018-2022, STMicroelectronics
 */

#include <assert.h>
#include <config.h>
#include <drivers/stm32mp1_rcc.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/stm32_gpio.h>
#include <drivers/stm32_shared_io.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp1_rcc_util.h>
#include <initcall.h>
#include <io.h>
#include <keep.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/boot.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <platform_config.h>
#include <stdio.h>
#include <stm32_util.h>
#include <trace.h>
#include <util.h>

#include "clk-stm32-core.h"

#define MAX_OPP		PLAT_MAX_OPP_NB

#define DT_OPP_COMPAT		"operating-points-v2"

/* PLL settings computation related definitions */
#define POST_DIVM_MIN	8000000
#define POST_DIVM_MAX	16000000
#define DIVM_MIN	0
#define DIVM_MAX	63
#define DIVN_MIN	24
#define DIVN_MAX	99
#define DIVP_MIN	0
#define DIVP_MAX	127
#define FRAC_MAX	8192
#define VCO_MIN		800000000
#define VCO_MAX		1600000000

#define PLL1_SETTINGS_VALID_ID		0x504C4C31 /* "PLL1" */

static TEE_Result stm32_clock_pm(enum pm_op op, unsigned int pm_hint __unused,
				 const struct pm_callback_handle *hdl __unused);

static unsigned long clk_stm32_get_rate_oscillateur(int osc_id);

/* Identifiers for root oscillators */
enum stm32mp_osc_id {
	OSC_HSI,
	OSC_HSE,
	OSC_CSI,
	OSC_LSI,
	OSC_LSE,
	OSC_I2S_CKIN,
	OSC_USB_PHY_48,
	NB_OSC,
	_UNKNOWN_OSC_ID = 0xffU
};

/* Identifiers for parent clocks */
enum stm32mp1_parent_id {
	_HSI,
	_HSE,
	_CSI,
	_LSI,
	_LSE,
	_I2S_CKIN,
	_USB_PHY_48,
	_HSI_KER,
	_HSE_KER,
	_HSE_KER_DIV2,
	_HSE_RTC,
	_CSI_KER,
	_PLL1_P,
	_PLL1_Q,
	_PLL1_R,
	_PLL2_P,
	_PLL2_Q,
	_PLL2_R,
	_PLL3_P,
	_PLL3_Q,
	_PLL3_R,
	_PLL4_P,
	_PLL4_Q,
	_PLL4_R,
	_ACLK,
	_PCLK1,
	_PCLK2,
	_PCLK3,
	_PCLK4,
	_PCLK5,
	_HCLK5,
	_HCLK6,
	_HCLK2,
	_CK_PER,
	_CK_MPU,
	_CK_MCU,
	_PARENT_NB,
	_UNKNOWN_ID = 0xff,
};

/*
 * Identifiers for parent clock selectors.
 * This enum lists only the parent clocks we are interested in.
 */
enum stm32mp1_parent_sel {
	_STGEN_SEL,
	_I2C46_SEL,
	_SPI6_SEL,
	_USART1_SEL,
	_RNG1_SEL,
	_UART6_SEL,
	_UART24_SEL,
	_UART35_SEL,
	_UART78_SEL,
	_AXISS_SEL,
	_MCUSS_SEL,
	_USBPHY_SEL,
	_USBO_SEL,
	_RTC_SEL,
	_MPU_SEL,
	_PARENT_SEL_NB,
	_UNKNOWN_SEL = 0xff,
};

static const uint8_t parent_id_clock_id[_PARENT_NB] = {
	[_HSE] = CK_HSE,
	[_HSI] = CK_HSI,
	[_CSI] = CK_CSI,
	[_LSE] = CK_LSE,
	[_LSI] = CK_LSI,
	[_I2S_CKIN] = _UNKNOWN_ID,
	[_USB_PHY_48] = _UNKNOWN_ID,
	[_HSI_KER] = CK_HSI,
	[_HSE_KER] = CK_HSE,
	[_HSE_KER_DIV2] = CK_HSE_DIV2,
	[_HSE_RTC] = _UNKNOWN_ID,
	[_CSI_KER] = CK_CSI,
	[_PLL1_P] = PLL1_P,
	[_PLL1_Q] = PLL1_Q,
	[_PLL1_R] = PLL1_R,
	[_PLL2_P] = PLL2_P,
	[_PLL2_Q] = PLL2_Q,
	[_PLL2_R] = PLL2_R,
	[_PLL3_P] = PLL3_P,
	[_PLL3_Q] = PLL3_Q,
	[_PLL3_R] = PLL3_R,
	[_PLL4_P] = PLL4_P,
	[_PLL4_Q] = PLL4_Q,
	[_PLL4_R] = PLL4_R,
	[_ACLK] = CK_AXI,
	[_PCLK1] = CK_AXI,
	[_PCLK2] = CK_AXI,
	[_PCLK3] = CK_AXI,
	[_PCLK4] = CK_AXI,
	[_PCLK5] = CK_AXI,
	[_HCLK5] = CK_AXI,
	[_HCLK6] = CK_AXI,
	[_HCLK2] = CK_AXI,
	[_CK_PER] = CK_PER,
	[_CK_MPU] = CK_MPU,
	[_CK_MCU] = CK_MCU,
};

static enum stm32mp1_parent_id osc_id2parent_id(enum stm32mp_osc_id osc_id)
{
	assert(osc_id >= OSC_HSI && osc_id < NB_OSC);
	COMPILE_TIME_ASSERT((int)OSC_HSI == (int)_HSI &&
			    (int)OSC_HSE == (int)_HSE &&
			    (int)OSC_CSI == (int)_CSI &&
			    (int)OSC_LSI == (int)_LSI &&
			    (int)OSC_LSE == (int)_LSE &&
			    (int)OSC_I2S_CKIN == (int)_I2S_CKIN &&
			    (int)OSC_USB_PHY_48 == (int)_USB_PHY_48);

	return (enum stm32mp1_parent_id)osc_id;
}

static enum stm32mp1_parent_id clock_id2parent_id(unsigned long id)
{
	size_t n = 0;

	COMPILE_TIME_ASSERT(STM32MP1_LAST_CLK < _UNKNOWN_ID);

	for (n = 0; n < ARRAY_SIZE(parent_id_clock_id); n++)
		if (parent_id_clock_id[n] == id)
			return (enum stm32mp1_parent_id)n;

	return _UNKNOWN_ID;
}

/* Identifiers for PLLs and their configuration resources */
enum stm32mp1_pll_id {
	_PLL1,
	_PLL2,
	_PLL3,
	_PLL4,
	_PLL_NB
};

enum stm32mp1_div_id {
	_DIV_P,
	_DIV_Q,
	_DIV_R,
	_DIV_NB,
};

enum stm32mp1_pllcfg {
	PLLCFG_M,
	PLLCFG_N,
	PLLCFG_P,
	PLLCFG_Q,
	PLLCFG_R,
	PLLCFG_O,
	PLLCFG_NB
};

enum stm32mp1_plltype {
	PLL_800,
	PLL_1600,
	PLL_TYPE_NB
};

/*
 * Clock generic gates clocks which state is controlled by a single RCC bit
 *
 * @offset: RCC register byte offset from RCC base where clock is controlled
 * @bit: Bit position in the RCC 32bit register
 * @clock_id: Identifier used for the clock in the clock driver API
 * @set_clr: Non-null if and only-if RCC register is a CLEAR/SET register
 *	(CLEAR register is at offset RCC_MP_ENCLRR_OFFSET from SET register)
 * @secure: One of N_S or SEC, defined below
 * @sel: _UNKNOWN_ID (fixed parent) or reference to parent clock selector
 *	(8bit storage of ID from enum stm32mp1_parent_sel)
 * @fixed: _UNKNOWN_ID (selectable paranet) or reference to parent clock
 *	(8bit storage of ID from enum stm32mp1_parent_id)
 */
struct stm32mp1_clk_gate {
	uint16_t offset;
	uint8_t bit;
	uint8_t clock_id;
	uint8_t set_clr;
	uint8_t secure;
	uint8_t sel; /* Relates to enum stm32mp1_parent_sel */
	uint8_t fixed; /* Relates to enum stm32mp1_parent_id */
};

/* Parent clock selection: select register info, parent clocks references */
struct stm32mp1_clk_sel {
	uint16_t offset;
	uint8_t src;
	uint8_t msk;
	uint8_t nb_parent;
	const uint8_t *parent;
};

#define REFCLK_SIZE 4
/* PLL control: type, control register offsets, up-to-4 selectable parent */
struct stm32mp1_clk_pll {
	enum stm32mp1_plltype plltype;
	uint16_t rckxselr;
	uint16_t pllxcfgr1;
	uint16_t pllxcfgr2;
	uint16_t pllxfracr;
	uint16_t pllxcr;
	uint16_t pllxcsgr;
	enum stm32mp_osc_id refclk[REFCLK_SIZE];
};

struct stm32mp1_pll {
	uint8_t refclk_min;
	uint8_t refclk_max;
};

/* Compact structure of 32bit cells, copied raw when suspending */
struct stm32mp1_pll_settings {
	uint32_t valid_id;
	uint32_t freq[MAX_OPP];
	uint32_t volt[MAX_OPP];
	uint32_t cfg[MAX_OPP][PLLCFG_NB];
	uint32_t frac[MAX_OPP];
};

#define N_S	0	/* Non-secure can access RCC interface */
#define SEC	1	/* RCC[TZEN] protects RCC interface */

/* Clocks with selectable source and not set/clr register access */
#define _CLK_SELEC(_sec, _offset, _bit, _clock_id, _parent_sel)	\
	{							\
		.offset = (_offset),				\
		.bit = (_bit),					\
		.clock_id = (_clock_id),			\
		.set_clr = 0,					\
		.secure = (_sec),				\
		.sel = (_parent_sel),				\
		.fixed = _UNKNOWN_ID,				\
	}

/* Clocks with fixed source and not set/clr register access */
#define _CLK_FIXED(_sec, _offset, _bit, _clock_id, _parent)		\
	{							\
		.offset = (_offset),				\
		.bit = (_bit),					\
		.clock_id = (_clock_id),			\
		.set_clr = 0,					\
		.secure = (_sec),				\
		.sel = _UNKNOWN_SEL,				\
		.fixed = (_parent),				\
	}

/* Clocks with selectable source and set/clr register access */
#define _CLK_SC_SELEC(_sec, _offset, _bit, _clock_id, _parent_sel)	\
	{							\
		.offset = (_offset),				\
		.bit = (_bit),					\
		.clock_id = (_clock_id),			\
		.set_clr = 1,					\
		.secure = (_sec),				\
		.sel = (_parent_sel),				\
		.fixed = _UNKNOWN_ID,				\
	}

/* Clocks with fixed source and set/clr register access */
#define _CLK_SC_FIXED(_sec, _offset, _bit, _clock_id, _parent)	\
	{							\
		.offset = (_offset),				\
		.bit = (_bit),					\
		.clock_id = (_clock_id),			\
		.set_clr = 1,					\
		.secure = (_sec),				\
		.sel = _UNKNOWN_SEL,				\
		.fixed = (_parent),				\
	}

/*
 * Clocks with selectable source and set/clr register access
 * and enable bit position defined by a label (argument _bit)
 */
#define _CLK_SC2_SELEC(_sec, _offset, _bit, _clock_id, _parent_sel)	\
	{							\
		.offset = (_offset),				\
		.clock_id = (_clock_id),			\
		.bit = _offset ## _ ## _bit ## _POS,		\
		.set_clr = 1,					\
		.secure = (_sec),				\
		.sel = (_parent_sel),				\
		.fixed = _UNKNOWN_ID,				\
	}
#define _CLK_SC2_FIXED(_sec, _offset, _bit, _clock_id, _parent)	\
	{							\
		.offset = (_offset),				\
		.clock_id = (_clock_id),			\
		.bit = _offset ## _ ## _bit ## _POS,		\
		.set_clr = 1,					\
		.secure = (_sec),				\
		.sel = _UNKNOWN_SEL,				\
		.fixed = (_parent),				\
	}

#define _CLK_PARENT(idx, _offset, _src, _mask, _parent)		\
	[(idx)] = {						\
		.offset = (_offset),				\
		.src = (_src),					\
		.msk = (_mask),					\
		.parent = (_parent),				\
		.nb_parent = ARRAY_SIZE(_parent)		\
	}

#define _CLK_PLL(_idx, _type, _off1, _off2, _off3, _off4,	\
		 _off5, _off6, _p1, _p2, _p3, _p4)		\
	[(_idx)] = {						\
		.plltype = (_type),				\
		.rckxselr = (_off1),				\
		.pllxcfgr1 = (_off2),				\
		.pllxcfgr2 = (_off3),				\
		.pllxfracr = (_off4),				\
		.pllxcr = (_off5),				\
		.pllxcsgr = (_off6),				\
		.refclk[0] = (_p1),				\
		.refclk[1] = (_p2),				\
		.refclk[2] = (_p3),				\
		.refclk[3] = (_p4),				\
	}

#define NB_GATES	ARRAY_SIZE(stm32mp1_clk_gate)

static const struct stm32mp1_clk_gate stm32mp1_clk_gate[] = {
	_CLK_FIXED(SEC, RCC_DDRITFCR, 0, DDRC1, _ACLK),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 1, DDRC1LP, _ACLK),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 2, DDRC2, _ACLK),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 3, DDRC2LP, _ACLK),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 4, DDRPHYC, _PLL2_R),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 5, DDRPHYCLP, _PLL2_R),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 6, DDRCAPB, _PCLK4),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 7, DDRCAPBLP, _PCLK4),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 8, AXIDCG, _ACLK),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 9, DDRPHYCAPB, _PCLK4),
	_CLK_FIXED(SEC, RCC_DDRITFCR, 10, DDRPHYCAPBLP, _PCLK4),

	_CLK_SC2_SELEC(SEC, RCC_MP_APB5ENSETR, SPI6EN, SPI6_K, _SPI6_SEL),
	_CLK_SC2_SELEC(SEC, RCC_MP_APB5ENSETR, I2C4EN, I2C4_K, _I2C46_SEL),
	_CLK_SC2_SELEC(SEC, RCC_MP_APB5ENSETR, I2C6EN, I2C6_K, _I2C46_SEL),
	_CLK_SC2_SELEC(SEC, RCC_MP_APB5ENSETR, USART1EN, USART1_K, _USART1_SEL),
	_CLK_SC2_FIXED(SEC, RCC_MP_APB5ENSETR, RTCAPBEN, RTCAPB, _PCLK5),
	_CLK_SC2_FIXED(SEC, RCC_MP_APB5ENSETR, TZC1EN, TZC1, _PCLK5),
	_CLK_SC2_FIXED(SEC, RCC_MP_APB5ENSETR, TZC2EN, TZC2, _PCLK5),
	_CLK_SC2_FIXED(SEC, RCC_MP_APB5ENSETR, TZPCEN, TZPC, _PCLK5),
	_CLK_SC2_FIXED(SEC, RCC_MP_APB5ENSETR, IWDG1APBEN, IWDG1, _PCLK5),
	_CLK_SC2_FIXED(SEC, RCC_MP_APB5ENSETR, BSECEN, BSEC, _PCLK5),
	_CLK_SC2_SELEC(SEC, RCC_MP_APB5ENSETR, STGENEN, STGEN_K, _STGEN_SEL),

	_CLK_SC2_FIXED(SEC, RCC_MP_AHB5ENSETR, GPIOZEN, GPIOZ, _HCLK5),
	_CLK_SC2_FIXED(SEC, RCC_MP_AHB5ENSETR, CRYP1EN, CRYP1, _HCLK5),
	_CLK_SC2_FIXED(SEC, RCC_MP_AHB5ENSETR, HASH1EN, HASH1, _HCLK5),
	_CLK_SC2_SELEC(SEC, RCC_MP_AHB5ENSETR, RNG1EN, RNG1_K, _RNG1_SEL),
	_CLK_SC2_FIXED(SEC, RCC_MP_AHB5ENSETR, BKPSRAMEN, BKPSRAM, _HCLK5),

	_CLK_SC2_FIXED(SEC, RCC_MP_TZAHB6ENSETR, MDMA, MDMA, _HCLK6),

	_CLK_SELEC(SEC, RCC_BDCR, RCC_BDCR_RTCCKEN_POS, RTC, _RTC_SEL),

	/* Non-secure clocks */
#ifdef CFG_WITH_NSEC_GPIOS
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 0, GPIOA, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 1, GPIOB, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 2, GPIOC, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 3, GPIOD, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 4, GPIOE, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 5, GPIOF, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 6, GPIOG, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 7, GPIOH, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 8, GPIOI, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 9, GPIOJ, _UNKNOWN_ID),
	_CLK_SC_FIXED(N_S, RCC_MP_AHB4ENSETR, 10, GPIOK, _UNKNOWN_ID),
#endif
	_CLK_SC_FIXED(N_S, RCC_MP_APB1ENSETR, 6, TIM12_K, _PCLK1),
#ifdef CFG_WITH_NSEC_UARTS
	_CLK_SC_SELEC(N_S, RCC_MP_APB1ENSETR, 14, USART2_K, _UART24_SEL),
	_CLK_SC_SELEC(N_S, RCC_MP_APB1ENSETR, 15, USART3_K, _UART35_SEL),
	_CLK_SC_SELEC(N_S, RCC_MP_APB1ENSETR, 16, UART4_K, _UART24_SEL),
	_CLK_SC_SELEC(N_S, RCC_MP_APB1ENSETR, 17, UART5_K, _UART35_SEL),
	_CLK_SC_SELEC(N_S, RCC_MP_APB1ENSETR, 18, UART7_K, _UART78_SEL),
	_CLK_SC_SELEC(N_S, RCC_MP_APB1ENSETR, 19, UART8_K, _UART78_SEL),
#endif
	_CLK_SC_FIXED(N_S, RCC_MP_APB2ENSETR, 2, TIM15_K, _PCLK2),
#ifdef CFG_WITH_NSEC_UARTS
	_CLK_SC_SELEC(N_S, RCC_MP_APB2ENSETR, 13, USART6_K, _UART6_SEL),
#endif
	_CLK_SC_FIXED(N_S, RCC_MP_APB3ENSETR, 11, SYSCFG, _UNKNOWN_ID),
	_CLK_SC_SELEC(N_S, RCC_MP_APB4ENSETR, 8, DDRPERFM, _UNKNOWN_SEL),
	_CLK_SC_SELEC(N_S, RCC_MP_APB4ENSETR, 15, IWDG2, _UNKNOWN_SEL),

	_CLK_SELEC(N_S, RCC_DBGCFGR, 8, CK_DBG, _UNKNOWN_SEL),
};
DECLARE_KEEP_PAGER(stm32mp1_clk_gate);

const uint8_t stm32mp1_clk_on[] = {
	CK_HSE, CK_CSI, CK_LSI, CK_LSE, CK_HSI, CK_HSE_DIV2,
	PLL1_P, PLL1_Q, PLL1_R, PLL2_P, PLL2_Q, PLL2_R, PLL3_P, PLL3_Q, PLL3_R,
	CK_AXI, CK_MPU, CK_MCU,
};

/* Parents for secure aware clocks in the xxxSELR value ordering */
static const uint8_t stgen_parents[] = {
	_HSI_KER, _HSE_KER
};

static const uint8_t i2c46_parents[] = {
	_PCLK5, _PLL3_Q, _HSI_KER, _CSI_KER
};

static const uint8_t spi6_parents[] = {
	_PCLK5, _PLL4_Q, _HSI_KER, _CSI_KER, _HSE_KER, _PLL3_Q
};

static const uint8_t usart1_parents[] = {
	_PCLK5, _PLL3_Q, _HSI_KER, _CSI_KER, _PLL4_Q, _HSE_KER
};

static const uint8_t rng1_parents[] = {
	_CSI, _PLL4_R, _LSE, _LSI
};

static const uint8_t mpu_parents[] = {
	_HSI, _HSE, _PLL1_P, _PLL1_P /* specific div */
};

/* Parents for (some) non-secure clocks */
#ifdef CFG_WITH_NSEC_UARTS
static const uint8_t uart6_parents[] = {
	_PCLK2, _PLL4_Q, _HSI_KER, _CSI_KER, _HSE_KER
};

static const uint8_t uart234578_parents[] = {
	_PCLK1, _PLL4_Q, _HSI_KER, _CSI_KER, _HSE_KER
};
#endif

static const uint8_t axiss_parents[] = {
	_HSI, _HSE, _PLL2_P
};

static const uint8_t mcuss_parents[] = {
	_HSI, _HSE, _CSI, _PLL3_P
};

static const uint8_t rtc_parents[] = {
	_UNKNOWN_ID, _LSE, _LSI, _HSE_RTC
};

static const struct stm32mp1_clk_sel stm32mp1_clk_sel[_PARENT_SEL_NB] = {
	/* Secure aware clocks */
	_CLK_PARENT(_STGEN_SEL, RCC_STGENCKSELR, 0, 0x3, stgen_parents),
	_CLK_PARENT(_I2C46_SEL, RCC_I2C46CKSELR, 0, 0x7, i2c46_parents),
	_CLK_PARENT(_SPI6_SEL, RCC_SPI6CKSELR, 0, 0x7, spi6_parents),
	_CLK_PARENT(_USART1_SEL, RCC_UART1CKSELR, 0, 0x7, usart1_parents),
	_CLK_PARENT(_RNG1_SEL, RCC_RNG1CKSELR, 0, 0x3, rng1_parents),
	_CLK_PARENT(_RTC_SEL, RCC_BDCR, 16, 0x3, rtc_parents),
	_CLK_PARENT(_MPU_SEL, RCC_MPCKSELR, 0, 0x3, mpu_parents),
	/* Always non-secure clocks (maybe used in some way in secure world) */
#ifdef CFG_WITH_NSEC_UARTS
	_CLK_PARENT(_UART6_SEL, RCC_UART6CKSELR, 0, 0x7, uart6_parents),
	_CLK_PARENT(_UART24_SEL, RCC_UART24CKSELR, 0, 0x7, uart234578_parents),
	_CLK_PARENT(_UART35_SEL, RCC_UART35CKSELR, 0, 0x7, uart234578_parents),
	_CLK_PARENT(_UART78_SEL, RCC_UART78CKSELR, 0, 0x7, uart234578_parents),
#endif
	_CLK_PARENT(_AXISS_SEL, RCC_ASSCKSELR, 0, 0x3, axiss_parents),
	_CLK_PARENT(_MCUSS_SEL, RCC_MSSCKSELR, 0, 0x3, mcuss_parents),
};

/* Define characteristics of PLL according type */
static const struct stm32mp1_pll stm32mp1_pll[PLL_TYPE_NB] = {
	[PLL_800] = {
		.refclk_min = 4,
		.refclk_max = 16,
	},
	[PLL_1600] = {
		.refclk_min = 8,
		.refclk_max = 16,
	},
};

/* PLLNCFGR2 register divider by output */
static const uint8_t pllncfgr2[_DIV_NB] = {
	[_DIV_P] = RCC_PLLNCFGR2_DIVP_SHIFT,
	[_DIV_Q] = RCC_PLLNCFGR2_DIVQ_SHIFT,
	[_DIV_R] = RCC_PLLNCFGR2_DIVR_SHIFT,
};

static const struct stm32mp1_clk_pll stm32mp1_clk_pll[_PLL_NB] = {
	_CLK_PLL(_PLL1, PLL_1600,
		 RCC_RCK12SELR, RCC_PLL1CFGR1, RCC_PLL1CFGR2,
		 RCC_PLL1FRACR, RCC_PLL1CR, RCC_PLL1CSGR,
		 OSC_HSI, OSC_HSE, _UNKNOWN_OSC_ID, _UNKNOWN_OSC_ID),
	_CLK_PLL(_PLL2, PLL_1600,
		 RCC_RCK12SELR, RCC_PLL2CFGR1, RCC_PLL2CFGR2,
		 RCC_PLL2FRACR, RCC_PLL2CR, RCC_PLL2CSGR,
		 OSC_HSI, OSC_HSE, _UNKNOWN_OSC_ID, _UNKNOWN_OSC_ID),
	_CLK_PLL(_PLL3, PLL_800,
		 RCC_RCK3SELR, RCC_PLL3CFGR1, RCC_PLL3CFGR2,
		 RCC_PLL3FRACR, RCC_PLL3CR, RCC_PLL3CSGR,
		 OSC_HSI, OSC_HSE, OSC_CSI, _UNKNOWN_OSC_ID),
	_CLK_PLL(_PLL4, PLL_800,
		 RCC_RCK4SELR, RCC_PLL4CFGR1, RCC_PLL4CFGR2,
		 RCC_PLL4FRACR, RCC_PLL4CR, RCC_PLL4CSGR,
		 OSC_HSI, OSC_HSE, OSC_CSI, OSC_I2S_CKIN),
};

/* Prescaler table lookups for clock computation */
/* div = /1 /2 /4 /8 / 16 /64 /128 /512 */
static const uint8_t stm32mp1_mcu_div[16] = {
	0, 1, 2, 3, 4, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9
};

/* div = /1 /2 /4 /8 /16 : same divider for MPU and APBX */
#define stm32mp1_mpu_div	stm32mp1_mpu_apbx_div
#define stm32mp1_apbx_div	stm32mp1_mpu_apbx_div
static const uint8_t stm32mp1_mpu_apbx_div[8] = {
	0, 1, 2, 3, 4, 4, 4, 4
};

/* div = /1 /2 /3 /4 */
static const uint8_t stm32mp1_axi_div[8] = {
	1, 2, 3, 4, 4, 4, 4, 4
};

static const char __maybe_unused *const stm32mp1_clk_parent_name[_PARENT_NB] = {
	[_HSI] = "HSI",
	[_HSE] = "HSE",
	[_CSI] = "CSI",
	[_LSI] = "LSI",
	[_LSE] = "LSE",
	[_I2S_CKIN] = "I2S_CKIN",
	[_HSI_KER] = "HSI_KER",
	[_HSE_KER] = "HSE_KER",
	[_HSE_KER_DIV2] = "HSE_KER_DIV2",
	[_HSE_RTC] = "HSE_RTC",
	[_CSI_KER] = "CSI_KER",
	[_PLL1_P] = "PLL1_P",
	[_PLL1_Q] = "PLL1_Q",
	[_PLL1_R] = "PLL1_R",
	[_PLL2_P] = "PLL2_P",
	[_PLL2_Q] = "PLL2_Q",
	[_PLL2_R] = "PLL2_R",
	[_PLL3_P] = "PLL3_P",
	[_PLL3_Q] = "PLL3_Q",
	[_PLL3_R] = "PLL3_R",
	[_PLL4_P] = "PLL4_P",
	[_PLL4_Q] = "PLL4_Q",
	[_PLL4_R] = "PLL4_R",
	[_ACLK] = "ACLK",
	[_PCLK1] = "PCLK1",
	[_PCLK2] = "PCLK2",
	[_PCLK3] = "PCLK3",
	[_PCLK4] = "PCLK4",
	[_PCLK5] = "PCLK5",
	[_HCLK2] = "HCLK2",
	[_HCLK5] = "HCLK5",
	[_HCLK6] = "HCLK6",
	[_CK_PER] = "CK_PER",
	[_CK_MPU] = "CK_MPU",
	[_CK_MCU] = "CK_MCU",
	[_USB_PHY_48] = "USB_PHY_48",
};

/* Storage of the precomputed SoC settings for PLL1 various OPPs */
static struct stm32mp1_pll_settings pll1_settings;
static uint32_t current_opp_khz;

static unsigned long osc_frequency(enum stm32mp_osc_id idx)
{
	if (idx >= NB_OSC) {
		DMSG("clk id %d not found", idx);
		return 0;
	}

	return clk_stm32_get_rate_oscillateur(idx);
}

static const struct stm32mp1_clk_gate *gate_ref(unsigned int idx)
{
	return &stm32mp1_clk_gate[idx];
}

static const struct stm32mp1_clk_sel *clk_sel_ref(unsigned int idx)
{
	return &stm32mp1_clk_sel[idx];
}

static const struct stm32mp1_clk_pll *pll_ref(unsigned int idx)
{
	return &stm32mp1_clk_pll[idx];
}

static int stm32mp1_clk_get_gated_id(unsigned long id)
{
	unsigned int i = 0;

	for (i = 0; i < NB_GATES; i++)
		if (gate_ref(i)->clock_id == id)
			return i;

	DMSG("clk id %lu not found", id);
	return -1;
}

static enum stm32mp1_parent_sel stm32mp1_clk_get_sel(int i)
{
	return (enum stm32mp1_parent_sel)gate_ref(i)->sel;
}

static enum stm32mp1_parent_id stm32mp1_clk_get_fixed_parent(int i)
{
	return (enum stm32mp1_parent_id)gate_ref(i)->fixed;
}

static int stm32mp1_clk_get_parent(unsigned long id)
{
	const struct stm32mp1_clk_sel *sel = NULL;
	enum stm32mp1_parent_id parent_id = 0;
	uint32_t p_sel = 0;
	int i = 0;
	enum stm32mp1_parent_id p = _UNKNOWN_ID;
	enum stm32mp1_parent_sel s = _UNKNOWN_SEL;
	vaddr_t rcc_base = stm32_rcc_base();

	parent_id = clock_id2parent_id(id);
	if (parent_id != _UNKNOWN_ID)
		return (int)parent_id;

	i = stm32mp1_clk_get_gated_id(id);
	if (i < 0)
		panic();

	p = stm32mp1_clk_get_fixed_parent(i);
	if (p < _PARENT_NB)
		return (int)p;

	s = stm32mp1_clk_get_sel(i);
	if (s == _UNKNOWN_SEL)
		return -1;
	if (s >= _PARENT_SEL_NB)
		panic();

	sel = clk_sel_ref(s);
	p_sel = (io_read32(rcc_base + sel->offset) >> sel->src) & sel->msk;
	if (p_sel < sel->nb_parent)
		return (int)sel->parent[p_sel];

	DMSG("No parent selected for clk %lu", id);
	return -1;
}

static unsigned long stm32mp1_pll_get_fref(const struct stm32mp1_clk_pll *pll)
{
	uint32_t selr = io_read32(stm32_rcc_base() + pll->rckxselr);
	uint32_t src = selr & RCC_SELR_REFCLK_SRC_MASK;

	return osc_frequency(pll->refclk[src]);
}

/*
 * pll_get_fvco() : return the VCO or (VCO / 2) frequency for the requested PLL
 * - PLL1 & PLL2 => return VCO / 2 with Fpll_y_ck = FVCO / 2 * (DIVy + 1)
 * - PLL3 & PLL4 => return VCO     with Fpll_y_ck = FVCO / (DIVy + 1)
 * => in all cases Fpll_y_ck = pll_get_fvco() / (DIVy + 1)
 */
static unsigned long stm32mp1_pll_get_fvco(const struct stm32mp1_clk_pll *pll)
{
	unsigned long refclk = 0;
	unsigned long fvco = 0;
	uint32_t cfgr1 = 0;
	uint32_t fracr = 0;
	uint32_t divm = 0;
	uint32_t divn = 0;

	cfgr1 = io_read32(stm32_rcc_base() + pll->pllxcfgr1);
	fracr = io_read32(stm32_rcc_base() + pll->pllxfracr);

	divm = (cfgr1 & RCC_PLLNCFGR1_DIVM_MASK) >> RCC_PLLNCFGR1_DIVM_SHIFT;
	divn = cfgr1 & RCC_PLLNCFGR1_DIVN_MASK;

	refclk = stm32mp1_pll_get_fref(pll);

	/*
	 * With FRACV :
	 *   Fvco = Fck_ref * ((DIVN + 1) + FRACV / 2^13) / (DIVM + 1)
	 * Without FRACV
	 *   Fvco = Fck_ref * ((DIVN + 1) / (DIVM + 1)
	 */
	if (fracr & RCC_PLLNFRACR_FRACLE) {
		unsigned long long numerator = 0;
		unsigned long long denominator = 0;
		uint32_t fracv = (fracr & RCC_PLLNFRACR_FRACV_MASK) >>
				 RCC_PLLNFRACR_FRACV_SHIFT;

		numerator = (((unsigned long long)divn + 1U) << 13) + fracv;
		numerator = refclk * numerator;
		denominator = ((unsigned long long)divm + 1U) << 13;
		fvco = (unsigned long)(numerator / denominator);
	} else {
		fvco = (unsigned long)(refclk * (divn + 1U) / (divm + 1U));
	}

	return fvco;
}

static unsigned long stm32mp1_read_pll_freq(enum stm32mp1_pll_id pll_id,
					    enum stm32mp1_div_id div_id)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);
	unsigned long dfout = 0;
	uint32_t cfgr2 = 0;
	uint32_t divy = 0;

	if (div_id >= _DIV_NB)
		return 0;

	cfgr2 = io_read32(stm32_rcc_base() + pll->pllxcfgr2);
	divy = (cfgr2 >> pllncfgr2[div_id]) & RCC_PLLNCFGR2_DIVX_MASK;

	dfout = stm32mp1_pll_get_fvco(pll) / (divy + 1U);

	return dfout;
}

static void pll_start(enum stm32mp1_pll_id pll_id)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);
	uint32_t pllxcr = stm32_rcc_base() + pll->pllxcr;

	if (io_read32(pllxcr) & RCC_PLLNCR_PLLON)
		return;

	io_clrsetbits32(pllxcr,	RCC_PLLNCR_DIVPEN | RCC_PLLNCR_DIVQEN |
			RCC_PLLNCR_DIVREN, RCC_PLLNCR_PLLON);
}

#define PLLRDY_TIMEOUT_US	(200 * 1000)

static int pll_output(enum stm32mp1_pll_id pll_id, uint32_t output)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);
	uint32_t pllxcr = stm32_rcc_base() + pll->pllxcr;
	uint64_t start = 0;

	start = timeout_init_us(PLLRDY_TIMEOUT_US);
	/* Wait PLL lock */
	while (!(io_read32(pllxcr) & RCC_PLLNCR_PLLRDY))
		if (timeout_elapsed(start))
			break;

	if (!(io_read32(pllxcr) & RCC_PLLNCR_PLLRDY)) {
		EMSG("PLL%d start failed @ 0x%"PRIx32": 0x%"PRIx32,
		     pll_id, pllxcr, io_read32(pllxcr));

		return -1;
	}

	/* Start the requested output */
	io_setbits32(pllxcr, output << RCC_PLLNCR_DIVEN_SHIFT);

	return 0;
}

static int pll_stop(enum stm32mp1_pll_id pll_id)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);
	uint32_t pllxcr = stm32_rcc_base() + pll->pllxcr;
	uint64_t start = 0;

	/* Stop all output */
	io_clrbits32(pllxcr, RCC_PLLNCR_DIVPEN | RCC_PLLNCR_DIVQEN |
		     RCC_PLLNCR_DIVREN);

	/* Stop PLL */
	io_clrbits32(pllxcr, RCC_PLLNCR_PLLON);

	start = timeout_init_us(PLLRDY_TIMEOUT_US);
	/* Wait PLL stopped */
	while (io_read32(pllxcr) & RCC_PLLNCR_PLLRDY)
		if (timeout_elapsed(start))
			break;

	if (io_read32(pllxcr) & RCC_PLLNCR_PLLRDY) {
		EMSG("PLL%d stop failed @ 0x%"PRIx32": 0x%"PRIx32,
		     pll_id, pllxcr, io_read32(pllxcr));

		return -1;
	}

	return 0;
}

static uint32_t pll_compute_pllxcfgr2(uint32_t *pllcfg)
{
	uint32_t value = 0;

	value = (pllcfg[PLLCFG_P] << RCC_PLLNCFGR2_DIVP_SHIFT) &
		RCC_PLLNCFGR2_DIVP_MASK;
	value |= (pllcfg[PLLCFG_Q] << RCC_PLLNCFGR2_DIVQ_SHIFT) &
		 RCC_PLLNCFGR2_DIVQ_MASK;
	value |= (pllcfg[PLLCFG_R] << RCC_PLLNCFGR2_DIVR_SHIFT) &
		 RCC_PLLNCFGR2_DIVR_MASK;

	return value;
}

static void pll_config_output(enum stm32mp1_pll_id pll_id, uint32_t *pllcfg)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);
	uintptr_t rcc_base = stm32_rcc_base();
	uint32_t value = 0;

	value = pll_compute_pllxcfgr2(pllcfg);

	io_write32(rcc_base + pll->pllxcfgr2, value);
}

static int pll_compute_pllxcfgr1(const struct stm32mp1_clk_pll *pll,
				 uint32_t *pllcfg, uint32_t *cfgr1)
{
	uint32_t rcc_base = stm32_rcc_base();
	enum stm32mp1_plltype type = pll->plltype;
	unsigned long refclk = 0;
	uint32_t ifrge = 0;
	uint32_t src = 0;

	src = io_read32(rcc_base + pll->rckxselr) &
	      RCC_SELR_REFCLK_SRC_MASK;

	refclk = osc_frequency(pll->refclk[src]) /
		 (pllcfg[PLLCFG_M] + 1U);

	if ((refclk < (stm32mp1_pll[type].refclk_min * 1000000U)) ||
	    (refclk > (stm32mp1_pll[type].refclk_max * 1000000U)))
		return -1;

	if ((type == PLL_800) && (refclk >= 8000000U))
		ifrge = 1U;

	*cfgr1 = (pllcfg[PLLCFG_N] << RCC_PLLNCFGR1_DIVN_SHIFT) &
		 RCC_PLLNCFGR1_DIVN_MASK;
	*cfgr1 |= (pllcfg[PLLCFG_M] << RCC_PLLNCFGR1_DIVM_SHIFT) &
		  RCC_PLLNCFGR1_DIVM_MASK;
	*cfgr1 |= (ifrge << RCC_PLLNCFGR1_IFRGE_SHIFT) &
		  RCC_PLLNCFGR1_IFRGE_MASK;

	return 0;
}

static int pll_config(enum stm32mp1_pll_id pll_id, uint32_t *pllcfg,
		      uint32_t fracv)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);
	uint32_t rcc_base = stm32_rcc_base();
	uint32_t value = 0;
	int ret = 0;

	ret = pll_compute_pllxcfgr1(pll, pllcfg, &value);
	if (ret)
		return ret;

	io_write32(rcc_base + pll->pllxcfgr1, value);

	/* Fractional configuration */
	io_write32(rcc_base + pll->pllxfracr, value);

	/* Frac must be enabled only once its configuration is loaded */
	value = fracv << RCC_PLLNFRACR_FRACV_SHIFT;
	io_write32(rcc_base + pll->pllxfracr, value);
	value = io_read32(rcc_base + pll->pllxfracr);
	io_write32(rcc_base + pll->pllxfracr, value | RCC_PLLNFRACR_FRACLE);

	pll_config_output(pll_id, pllcfg);

	return 0;
}

static unsigned long get_clock_rate(enum stm32mp1_parent_id p)
{
	uint32_t reg = 0;
	unsigned long clock = 0;
	vaddr_t rcc_base = stm32_rcc_base();

	switch (p) {
	case _CK_MPU:
	/* MPU sub system */
		reg = io_read32(rcc_base + RCC_MPCKSELR);
		switch (reg & RCC_SELR_SRC_MASK) {
		case RCC_MPCKSELR_HSI:
			clock = osc_frequency(OSC_HSI);
			break;
		case RCC_MPCKSELR_HSE:
			clock = osc_frequency(OSC_HSE);
			break;
		case RCC_MPCKSELR_PLL:
			clock = stm32mp1_read_pll_freq(_PLL1, _DIV_P);
			break;
		case RCC_MPCKSELR_PLL_MPUDIV:
			reg = io_read32(rcc_base + RCC_MPCKDIVR);
			if (reg & RCC_MPUDIV_MASK)
				clock = stm32mp1_read_pll_freq(_PLL1, _DIV_P) >>
					stm32mp1_mpu_div[reg & RCC_MPUDIV_MASK];
			else
				clock = 0;
			break;
		default:
			break;
		}
		break;
	/* AXI sub system */
	case _ACLK:
	case _HCLK2:
	case _HCLK5:
	case _HCLK6:
	case _PCLK4:
	case _PCLK5:
		reg = io_read32(rcc_base + RCC_ASSCKSELR);
		switch (reg & RCC_SELR_SRC_MASK) {
		case RCC_ASSCKSELR_HSI:
			clock = osc_frequency(OSC_HSI);
			break;
		case RCC_ASSCKSELR_HSE:
			clock = osc_frequency(OSC_HSE);
			break;
		case RCC_ASSCKSELR_PLL:
			clock = stm32mp1_read_pll_freq(_PLL2, _DIV_P);
			break;
		default:
			break;
		}

		/* System clock divider */
		reg = io_read32(rcc_base + RCC_AXIDIVR);
		clock /= stm32mp1_axi_div[reg & RCC_AXIDIV_MASK];

		switch (p) {
		case _PCLK4:
			reg = io_read32(rcc_base + RCC_APB4DIVR);
			clock >>= stm32mp1_apbx_div[reg & RCC_APBXDIV_MASK];
			break;
		case _PCLK5:
			reg = io_read32(rcc_base + RCC_APB5DIVR);
			clock >>= stm32mp1_apbx_div[reg & RCC_APBXDIV_MASK];
			break;
		default:
			break;
		}
		break;
	/* MCU sub system */
	case _CK_MCU:
	case _PCLK1:
	case _PCLK2:
	case _PCLK3:
		reg = io_read32(rcc_base + RCC_MSSCKSELR);
		switch (reg & RCC_SELR_SRC_MASK) {
		case RCC_MSSCKSELR_HSI:
			clock = osc_frequency(OSC_HSI);
			break;
		case RCC_MSSCKSELR_HSE:
			clock = osc_frequency(OSC_HSE);
			break;
		case RCC_MSSCKSELR_CSI:
			clock = osc_frequency(OSC_CSI);
			break;
		case RCC_MSSCKSELR_PLL:
			clock = stm32mp1_read_pll_freq(_PLL3, _DIV_P);
			break;
		default:
			break;
		}

		/* MCU clock divider */
		reg = io_read32(rcc_base + RCC_MCUDIVR);
		clock >>= stm32mp1_mcu_div[reg & RCC_MCUDIV_MASK];

		switch (p) {
		case _PCLK1:
			reg = io_read32(rcc_base + RCC_APB1DIVR);
			clock >>= stm32mp1_apbx_div[reg & RCC_APBXDIV_MASK];
			break;
		case _PCLK2:
			reg = io_read32(rcc_base + RCC_APB2DIVR);
			clock >>= stm32mp1_apbx_div[reg & RCC_APBXDIV_MASK];
			break;
		case _PCLK3:
			reg = io_read32(rcc_base + RCC_APB3DIVR);
			clock >>= stm32mp1_apbx_div[reg & RCC_APBXDIV_MASK];
			break;
		case _CK_MCU:
		default:
			break;
		}
		break;
	case _CK_PER:
		reg = io_read32(rcc_base + RCC_CPERCKSELR);
		switch (reg & RCC_SELR_SRC_MASK) {
		case RCC_CPERCKSELR_HSI:
			clock = osc_frequency(OSC_HSI);
			break;
		case RCC_CPERCKSELR_HSE:
			clock = osc_frequency(OSC_HSE);
			break;
		case RCC_CPERCKSELR_CSI:
			clock = osc_frequency(OSC_CSI);
			break;
		default:
			break;
		}
		break;
	case _HSI:
	case _HSI_KER:
		clock = osc_frequency(OSC_HSI);
		break;
	case _CSI:
	case _CSI_KER:
		clock = osc_frequency(OSC_CSI);
		break;
	case _HSE:
	case _HSE_KER:
		clock = osc_frequency(OSC_HSE);
		break;
	case _HSE_KER_DIV2:
		clock = osc_frequency(OSC_HSE) >> 1;
		break;
	case _HSE_RTC:
		clock = osc_frequency(OSC_HSE);
		clock /= (io_read32(rcc_base + RCC_RTCDIVR) &
			  RCC_DIVR_DIV_MASK) + 1;
		break;
	case _LSI:
		clock = osc_frequency(OSC_LSI);
		break;
	case _LSE:
		clock = osc_frequency(OSC_LSE);
		break;
	/* PLL */
	case _PLL1_P:
		clock = stm32mp1_read_pll_freq(_PLL1, _DIV_P);
		break;
	case _PLL1_Q:
		clock = stm32mp1_read_pll_freq(_PLL1, _DIV_Q);
		break;
	case _PLL1_R:
		clock = stm32mp1_read_pll_freq(_PLL1, _DIV_R);
		break;
	case _PLL2_P:
		clock = stm32mp1_read_pll_freq(_PLL2, _DIV_P);
		break;
	case _PLL2_Q:
		clock = stm32mp1_read_pll_freq(_PLL2, _DIV_Q);
		break;
	case _PLL2_R:
		clock = stm32mp1_read_pll_freq(_PLL2, _DIV_R);
		break;
	case _PLL3_P:
		clock = stm32mp1_read_pll_freq(_PLL3, _DIV_P);
		break;
	case _PLL3_Q:
		clock = stm32mp1_read_pll_freq(_PLL3, _DIV_Q);
		break;
	case _PLL3_R:
		clock = stm32mp1_read_pll_freq(_PLL3, _DIV_R);
		break;
	case _PLL4_P:
		clock = stm32mp1_read_pll_freq(_PLL4, _DIV_P);
		break;
	case _PLL4_Q:
		clock = stm32mp1_read_pll_freq(_PLL4, _DIV_Q);
		break;
	case _PLL4_R:
		clock = stm32mp1_read_pll_freq(_PLL4, _DIV_R);
		break;
	/* Other */
	case _USB_PHY_48:
		clock = osc_frequency(OSC_USB_PHY_48);
		break;
	default:
		break;
	}

	return clock;
}

static bool __maybe_unused
__clk_is_enabled(const struct stm32mp1_clk_gate *gate)
{
	vaddr_t base = stm32_rcc_base();

	return io_read32(base + gate->offset) & BIT(gate->bit);
}

static void __clk_enable(const struct stm32mp1_clk_gate *gate)
{
	vaddr_t base = stm32_rcc_base();
	uint32_t bit = BIT(gate->bit);

	if (gate->set_clr)
		io_write32(base + gate->offset, bit);
	else
		io_setbits32_stm32shregs(base + gate->offset, bit);

	FMSG("Clock %u has been enabled", gate->clock_id);
}

static void __clk_disable(const struct stm32mp1_clk_gate *gate)
{
	vaddr_t base = stm32_rcc_base();
	uint32_t bit = BIT(gate->bit);

	if (gate->set_clr)
		io_write32(base + gate->offset + RCC_MP_ENCLRR_OFFSET, bit);
	else
		io_clrbits32_stm32shregs(base + gate->offset, bit);

	FMSG("Clock %u has been disabled", gate->clock_id);
}

static long get_timer_rate(long parent_rate, unsigned int apb_bus)
{
	uint32_t timgxpre = 0;
	uint32_t apbxdiv = 0;
	vaddr_t rcc_base = stm32_rcc_base();

	switch (apb_bus) {
	case 1:
		apbxdiv = io_read32(rcc_base + RCC_APB1DIVR) &
			  RCC_APBXDIV_MASK;
		timgxpre = io_read32(rcc_base + RCC_TIMG1PRER) &
			   RCC_TIMGXPRER_TIMGXPRE;
		break;
	case 2:
		apbxdiv = io_read32(rcc_base + RCC_APB2DIVR) &
			  RCC_APBXDIV_MASK;
		timgxpre = io_read32(rcc_base + RCC_TIMG2PRER) &
			   RCC_TIMGXPRER_TIMGXPRE;
		break;
	default:
		panic();
		break;
	}

	if (apbxdiv == 0)
		return parent_rate;

	return parent_rate * (timgxpre + 1) * 2;
}

static unsigned long _stm32_clock_get_rate(unsigned long id)
{
	enum stm32mp1_parent_id p = _UNKNOWN_ID;
	unsigned long rate = 0;

	p = stm32mp1_clk_get_parent(id);
	if (p < 0)
		return 0;

	rate = get_clock_rate(p);

	if ((id >= TIM2_K) && (id <= TIM14_K))
		rate = get_timer_rate(rate, 1);

	if ((id >= TIM1_K) && (id <= TIM17_K))
		rate = get_timer_rate(rate, 2);

	return rate;
}

/*
 * Get the parent ID of the target parent clock, or -1 if no parent found.
 */
static enum stm32mp1_parent_id get_parent_id_parent(enum stm32mp1_parent_id id)
{
	enum stm32mp1_parent_sel s = _UNKNOWN_SEL;
	enum stm32mp1_pll_id pll_id = _PLL_NB;
	uint32_t p_sel = 0;

	switch (id) {
	case _ACLK:
	case _HCLK5:
	case _HCLK6:
	case _PCLK4:
	case _PCLK5:
		s = _AXISS_SEL;
		break;
	case _PLL1_P:
	case _PLL1_Q:
	case _PLL1_R:
		pll_id = _PLL1;
		break;
	case _PLL2_P:
	case _PLL2_Q:
	case _PLL2_R:
		pll_id = _PLL2;
		break;
	case _PLL3_P:
	case _PLL3_Q:
	case _PLL3_R:
		pll_id = _PLL3;
		break;
	case _PLL4_P:
	case _PLL4_Q:
	case _PLL4_R:
		pll_id = _PLL4;
		break;
	case _PCLK1:
	case _PCLK2:
	case _HCLK2:
	case _CK_PER:
	case _CK_MPU:
	case _CK_MCU:
	case _USB_PHY_48:
		/* We do not expected to access these */
		panic();
		break;
	default:
		/* Other parents have no parent */
		return -1;
	}

	if (s != _UNKNOWN_SEL) {
		const struct stm32mp1_clk_sel *sel = clk_sel_ref(s);
		vaddr_t rcc_base = stm32_rcc_base();

		p_sel = (io_read32(rcc_base + sel->offset) >> sel->src) &
			sel->msk;

		if (p_sel < sel->nb_parent)
			return sel->parent[p_sel];
	} else {
		const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);

		p_sel = io_read32(stm32_rcc_base() + pll->rckxselr) &
			RCC_SELR_REFCLK_SRC_MASK;

		if (pll->refclk[p_sel] != _UNKNOWN_OSC_ID)
			return osc_id2parent_id(pll->refclk[p_sel]);
	}

	FMSG("No parent found for %s", stm32mp1_clk_parent_name[id]);
	return -1;
}

/* We are only interested in knowing if PLL3 shall be secure or not */
static void secure_parent_clocks(enum stm32mp1_parent_id parent_id)
{
	enum stm32mp1_parent_id grandparent_id = _UNKNOWN_ID;

	switch (parent_id) {
	case _ACLK:
	case _HCLK2:
	case _HCLK5:
	case _HCLK6:
	case _PCLK4:
	case _PCLK5:
		/* Intermediate clock mux or clock, go deeper in clock tree */
		break;
	case _HSI:
	case _HSI_KER:
	case _LSI:
	case _CSI:
	case _CSI_KER:
	case _HSE:
	case _HSE_KER:
	case _HSE_KER_DIV2:
	case _HSE_RTC:
	case _LSE:
	case _PLL1_P:
	case _PLL1_Q:
	case _PLL1_R:
	case _PLL2_P:
	case _PLL2_Q:
	case _PLL2_R:
		/* Always secure clocks, no need to go further */
		return;
	case _PLL3_P:
	case _PLL3_Q:
	case _PLL3_R:
		/* PLL3 is a shared resource, registered and don't go further */
		stm32mp_register_secure_periph(STM32MP1_SHRES_PLL3);
		return;
	default:
		DMSG("Cannot lookup parent clock %s",
		     stm32mp1_clk_parent_name[parent_id]);
		panic();
	}

	grandparent_id = get_parent_id_parent(parent_id);
	if (grandparent_id >= 0)
		secure_parent_clocks(grandparent_id);
}

void stm32mp_register_clock_parents_secure(unsigned long clock_id)
{
	enum stm32mp1_parent_id parent_id = stm32mp1_clk_get_parent(clock_id);

	if (parent_id < 0) {
		DMSG("No parent for clock %lu", clock_id);
		return;
	}

	secure_parent_clocks(parent_id);
}

static void save_current_opp(void)
{
	unsigned long freq_khz = UDIV_ROUND_NEAREST(_stm32_clock_get_rate(CK_MPU),
						    1000UL);
	if (freq_khz > (unsigned long)UINT32_MAX)
		panic();

	current_opp_khz = (uint32_t)freq_khz;
}

static void __maybe_unused enable_rcc_tzen(void)
{
	io_setbits32(stm32_rcc_base() + RCC_TZCR, RCC_TZCR_TZEN);
}

static void __maybe_unused disable_rcc_tzen(void)
{
	IMSG("RCC is non-secure");
	io_clrbits32(stm32_rcc_base() + RCC_TZCR, RCC_TZCR_TZEN);
}

void stm32mp1_clk_mcuss_protect(bool enable)
{
	uintptr_t rcc_base = stm32_rcc_base();

	if (enable)
		io_setbits32(rcc_base + RCC_TZCR, RCC_TZCR_MCKPROT);
	else
		io_clrbits32(rcc_base + RCC_TZCR, RCC_TZCR_MCKPROT);
}

/*
 * Gets OPP parameters (frequency in KHz and voltage in mV) from an OPP table
 * subnode. Platform HW support capabilities are also checked.
 */
static int get_opp_freqvolt_from_dt_subnode(void *fdt, int subnode,
					    uint32_t *freq_khz,
					    uint32_t *voltage_mv)
{
	const fdt64_t *cuint64 = NULL;
	const fdt32_t *cuint32 = NULL;
	uint64_t read_freq_64 = 0;
	uint32_t read_voltage_32 = 0;

	assert(freq_khz);
	assert(voltage_mv);

	cuint32 = fdt_getprop(fdt, subnode, "opp-supported-hw", NULL);
	if (cuint32)
		if (!stm32mp_supports_cpu_opp(fdt32_to_cpu(*cuint32))) {
			DMSG("Invalid opp-supported-hw 0x%"PRIx32,
			     fdt32_to_cpu(*cuint32));
			return -FDT_ERR_BADVALUE;
		}

	cuint64 = fdt_getprop(fdt, subnode, "opp-hz", NULL);
	if (!cuint64) {
		DMSG("Missing opp-hz");
		return -FDT_ERR_NOTFOUND;
	}

	/* Frequency value expressed in KHz must fit on 32 bits */
	read_freq_64 = fdt64_to_cpu(*cuint64) / 1000ULL;
	if (read_freq_64 > (uint64_t)UINT32_MAX) {
		DMSG("Invalid opp-hz %"PRIu64, read_freq_64);
		return -FDT_ERR_BADVALUE;
	}

	cuint32 = fdt_getprop(fdt, subnode, "opp-microvolt", NULL);
	if (!cuint32) {
		DMSG("Missing opp-microvolt");
		return -FDT_ERR_NOTFOUND;
	}

	/* Millivolt value must fit on 16 bits */
	read_voltage_32 = fdt32_to_cpu(*cuint32) / 1000U;
	if (read_voltage_32 > UINT16_MAX) {
		DMSG("Invalid opp-microvolt %"PRIu32, read_voltage_32);
		return -FDT_ERR_BADVALUE;
	}

	*freq_khz = (uint32_t)read_freq_64;

	*voltage_mv = read_voltage_32;

	return 0;
}

/*
 * Parses OPP table in DT and finds all parameters supported by the HW
 * platform. If found, the corresponding frequency and voltage values are
 * respectively stored in @pll1_settings structure.
 * Note that @*count has to be set by caller to the effective size allocated
 * for both tables. Its value is then replaced by the number of filled elements.
 */
static int get_all_opp_freqvolt_from_dt(uint32_t *count)
{
	void *fdt = NULL;
	int node = 0;
	int subnode = 0;
	uint32_t idx = 0;

	assert(count);

	fdt = get_embedded_dt();
	node = fdt_node_offset_by_compatible(fdt, -1, DT_OPP_COMPAT);
	if (node < 0)
		return node;

	fdt_for_each_subnode(subnode, fdt, node) {
		uint32_t read_freq = 0;
		uint32_t read_voltage = 0;

		if (get_opp_freqvolt_from_dt_subnode(fdt, subnode, &read_freq,
						     &read_voltage))
			continue;

		if (idx >= *count)
			return -FDT_ERR_NOSPACE;

		pll1_settings.freq[idx] = read_freq;
		pll1_settings.volt[idx] = read_voltage;
		idx++;
	}

	if (!idx)
		return -FDT_ERR_NOTFOUND;

	*count = idx;

	return 0;
}

static int clk_compute_pll1_settings(unsigned long input_freq, int idx)
{
	unsigned long post_divm = 0;
	unsigned long long output_freq = 0;
	unsigned long long freq = 0;
	unsigned long long vco = 0;
	int divm = 0;
	int divn = 0;
	int divp = 0;
	int frac = 0;
	int i = 0;
	unsigned int diff = 0;
	unsigned int best_diff = UINT_MAX;

	output_freq = (unsigned long long)pll1_settings.freq[idx] * 1000U;

	/* Following parameters have always the same value */
	pll1_settings.cfg[idx][PLLCFG_Q] = 0;
	pll1_settings.cfg[idx][PLLCFG_R] = 0;
	pll1_settings.cfg[idx][PLLCFG_O] = PQR(1, 0, 0);

	for (divm = DIVM_MAX; divm >= DIVM_MIN; divm--)	{
		post_divm = input_freq / (unsigned long)(divm + 1);

		if ((post_divm < POST_DIVM_MIN) ||
		    (post_divm > POST_DIVM_MAX))
			continue;

		for (divp = DIVP_MIN; divp <= DIVP_MAX; divp++) {

			freq = output_freq * (divm + 1) * (divp + 1);

			divn = (int)((freq / input_freq) - 1);
			if ((divn < DIVN_MIN) || (divn > DIVN_MAX))
				continue;

			frac = (int)(((freq * FRAC_MAX) / input_freq) -
				     ((divn + 1) * FRAC_MAX));

			/* 2 loops to refine the fractional part */
			for (i = 2; i != 0; i--) {
				if (frac > FRAC_MAX)
					break;

				vco = (post_divm * (divn + 1)) +
				      ((post_divm * (unsigned long long)frac) /
				       FRAC_MAX);

				if ((vco < (VCO_MIN / 2)) ||
				    (vco > (VCO_MAX / 2))) {
					frac++;
					continue;
				}

				freq = vco / (divp + 1);
				if (output_freq < freq)
					diff = (unsigned int)(freq -
							      output_freq);
				else
					diff = (unsigned int)(output_freq -
							      freq);

				if (diff < best_diff)  {
					pll1_settings.cfg[idx][PLLCFG_M] = divm;
					pll1_settings.cfg[idx][PLLCFG_N] = divn;
					pll1_settings.cfg[idx][PLLCFG_P] = divp;
					pll1_settings.frac[idx] = frac;

					if (!diff)
						return 0;

					best_diff = diff;
				}

				frac++;
			}
		}
	}

	if (best_diff == UINT_MAX) {
		pll1_settings.cfg[idx][PLLCFG_O] = 0;
		return -1;
	}

	return 0;
}

static int clk_get_pll1_settings(uint32_t clksrc, int index)
{
	unsigned long input_freq = 0;
	unsigned int i = 0;

	for (i = 0; i < MAX_OPP; i++)
		if (pll1_settings.freq[i] == pll1_settings.freq[index])
			break;

	if (((i == MAX_OPP) &&
	     !stm32mp1_clk_pll1_settings_are_valid()) ||
	    ((i < MAX_OPP) && !pll1_settings.cfg[i][PLLCFG_O])) {
		/*
		 * Either PLL1 settings structure is completely empty,
		 * or these settings are not yet computed: do it.
		 */
		switch (clksrc) {
		case CLK_PLL12_HSI:
			input_freq = _stm32_clock_get_rate(CK_HSI);
			break;
		case CLK_PLL12_HSE:
			input_freq = _stm32_clock_get_rate(CK_HSE);
			break;
		default:
			panic();
		}

		return clk_compute_pll1_settings(input_freq, index);
	}

	if (i < MAX_OPP) {
		if (pll1_settings.cfg[i][PLLCFG_O])
			return 0;

		/*
		 * Index is in range and PLL1 settings are computed:
		 * use content to answer to the request.
		 */
		memcpy(&pll1_settings.cfg[index][0], &pll1_settings.cfg[i][0],
		       sizeof(uint32_t) * PLLCFG_NB);
		pll1_settings.frac[index] = pll1_settings.frac[i];

		return 0;
	}

	return -1;
}

static int clk_save_current_pll1_settings(uint32_t buck1_voltage)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(_PLL1);
	uint32_t rcc_base = stm32_rcc_base();
	uint32_t freq = 0;
	unsigned int i = 0;

	freq = UDIV_ROUND_NEAREST(_stm32_clock_get_rate(CK_MPU), 1000L);

	for (i = 0; i < MAX_OPP; i++)
		if (pll1_settings.freq[i] == freq)
			break;

	if ((i == MAX_OPP) ||
	    ((pll1_settings.volt[i] != buck1_voltage) && buck1_voltage))
		return -1;

	pll1_settings.cfg[i][PLLCFG_M] = (io_read32(rcc_base + pll->pllxcfgr1) &
					  RCC_PLLNCFGR1_DIVM_MASK) >>
					 RCC_PLLNCFGR1_DIVM_SHIFT;

	pll1_settings.cfg[i][PLLCFG_N] = (io_read32(rcc_base + pll->pllxcfgr1) &
					  RCC_PLLNCFGR1_DIVN_MASK) >>
					 RCC_PLLNCFGR1_DIVN_SHIFT;

	pll1_settings.cfg[i][PLLCFG_P] = (io_read32(rcc_base + pll->pllxcfgr2) &
					  RCC_PLLNCFGR2_DIVP_MASK) >>
					 RCC_PLLNCFGR2_DIVP_SHIFT;

	pll1_settings.cfg[i][PLLCFG_Q] = (io_read32(rcc_base + pll->pllxcfgr2) &
					  RCC_PLLNCFGR2_DIVQ_MASK) >>
					 RCC_PLLNCFGR2_DIVQ_SHIFT;

	pll1_settings.cfg[i][PLLCFG_R] = (io_read32(rcc_base + pll->pllxcfgr2) &
					  RCC_PLLNCFGR2_DIVR_MASK) >>
					 RCC_PLLNCFGR2_DIVR_SHIFT;

	pll1_settings.cfg[i][PLLCFG_O] = io_read32(rcc_base + pll->pllxcr) >>
					 RCC_PLLNCR_DIVEN_SHIFT;

	pll1_settings.frac[i] = (io_read32(rcc_base + pll->pllxfracr) &
				 RCC_PLLNFRACR_FRACV_MASK) >>
				RCC_PLLNFRACR_FRACV_SHIFT;

	return i;
}

static uint32_t stm32mp1_clk_get_pll1_current_clksrc(void)
{
	uint32_t value = 0;
	const struct stm32mp1_clk_pll *pll = pll_ref(_PLL1);
	uint32_t rcc_base = stm32_rcc_base();

	value = io_read32(rcc_base + pll->rckxselr);

	switch (value & RCC_SELR_REFCLK_SRC_MASK) {
	case 0:
		return CLK_PLL12_HSI;
	case 1:
		return CLK_PLL12_HSE;
	default:
		panic();
	}
}

int stm32mp1_clk_compute_all_pll1_settings(uint32_t buck1_voltage)
{
	unsigned int i = 0;
	int ret = 0;
	int index = 0;
	uint32_t count = MAX_OPP;
	uint32_t clksrc = 0;

	ret = get_all_opp_freqvolt_from_dt(&count);
	switch (ret) {
	case 0:
		break;
	case -FDT_ERR_NOTFOUND:
		DMSG("Cannot find all OPP info in DT: use default settings.");
		return 0;
	default:
		EMSG("Inconsistent OPP settings found in DT, ignored.");
		return 0;
	}

	index = clk_save_current_pll1_settings(buck1_voltage);

	clksrc = stm32mp1_clk_get_pll1_current_clksrc();

	for (i = 0; i < count; i++) {
		if (index >= 0 && i == (unsigned int)index)
			continue;

		ret = clk_get_pll1_settings(clksrc, i);
		if (ret != 0)
			return ret;
	}

	pll1_settings.valid_id = PLL1_SETTINGS_VALID_ID;

	return 0;
}

void stm32mp1_clk_lp_save_opp_pll1_settings(uint8_t *data, size_t size)
{
	if ((size != sizeof(pll1_settings)) ||
	    !stm32mp1_clk_pll1_settings_are_valid())
		panic();

	memcpy(data, &pll1_settings, size);
}

bool stm32mp1_clk_pll1_settings_are_valid(void)
{
	return pll1_settings.valid_id == PLL1_SETTINGS_VALID_ID;
}

#ifdef CFG_DRIVERS_CLK
/*
 * Conversion between clk references and clock gates and clock on internals
 *
 * stm32mp1_clk first cells follow stm32mp1_clk_gate[] ordering.
 * stm32mp1_clk last cells follow stm32mp1_clk_on[] ordering.
 */
static struct clk stm32mp1_clk[ARRAY_SIZE(stm32mp1_clk_gate) +
			       ARRAY_SIZE(stm32mp1_clk_on)];

#define CLK_ON_INDEX_OFFSET	((int)ARRAY_SIZE(stm32mp1_clk_gate))

static bool clk_is_gate(struct clk *clk)
{
	int clk_index = clk - stm32mp1_clk;

	assert(clk_index >= 0 && clk_index < (int)ARRAY_SIZE(stm32mp1_clk));
	return clk_index < CLK_ON_INDEX_OFFSET;
}

static unsigned long clk_to_clock_id(struct clk *clk)
{
	int gate_index = clk - stm32mp1_clk;
	int on_index = gate_index - CLK_ON_INDEX_OFFSET;

	if (clk_is_gate(clk))
		return stm32mp1_clk_gate[gate_index].clock_id;

	return stm32mp1_clk_on[on_index];
}

static const struct stm32mp1_clk_gate *clk_to_gate_ref(struct clk *clk)
{
	int gate_index = clk - stm32mp1_clk;

	assert(clk_is_gate(clk));

	return stm32mp1_clk_gate + gate_index;
}

static int clock_id_to_gate_index(unsigned long clock_id)
{
	size_t n = 0;

	for (n = 0; n < ARRAY_SIZE(stm32mp1_clk_gate); n++)
		if (stm32mp1_clk_gate[n].clock_id == clock_id)
			return n;

	return -1;
}

static int clock_id_to_always_on_index(unsigned long clock_id)
{
	size_t n = 0;

	for (n = 0; n < ARRAY_SIZE(stm32mp1_clk_on); n++)
		if (stm32mp1_clk_on[n] == clock_id)
			return n;

	return -1;
}

static struct clk *clock_id_to_clk(unsigned long clock_id)
{
	int gate_index = clock_id_to_gate_index(clock_id);
	int on_index = clock_id_to_always_on_index(clock_id);

	if (gate_index >= 0)
		return stm32mp1_clk + gate_index;

	if (on_index >= 0)
		return stm32mp1_clk + CLK_ON_INDEX_OFFSET + on_index;

	return NULL;
}

struct clk *stm32mp_rcc_clock_id_to_clk(unsigned long clock_id)
{
	return clock_id_to_clk(clock_id);
}

unsigned int stm32mp_rcc_clk_to_clock_id(struct clk *clk)
{
	return clk_to_clock_id(clk);
}

#if (CFG_TEE_CORE_LOG_LEVEL >= TRACE_DEBUG) && defined(CFG_TEE_CORE_DEBUG)
struct clk_name {
	unsigned int clock_id;
	const char *name;
};

#define CLOCK_NAME(_binding, _name) \
	{ .clock_id = (_binding), .name = (_name) }

/* Store names only for some clocks */
const struct clk_name exposed_clk_name[] = {
	/* Clocks used by platform drivers not yet probed from DT */
	CLOCK_NAME(CK_DBG, "dbg"),
	CLOCK_NAME(CK_MCU, "mcu"),
	CLOCK_NAME(RTCAPB, "rtcapb"),
	CLOCK_NAME(BKPSRAM, "bkpsram"),
	CLOCK_NAME(RTC, "rtc"),
	CLOCK_NAME(CRYP1, "crpy1"),
	CLOCK_NAME(SYSCFG, "syscfg"),
	CLOCK_NAME(GPIOA, "gpioa"),
	CLOCK_NAME(GPIOB, "gpiob"),
	CLOCK_NAME(GPIOC, "gpioc"),
	CLOCK_NAME(GPIOD, "gpiod"),
	CLOCK_NAME(GPIOE, "gpioe"),
	CLOCK_NAME(GPIOF, "gpiof"),
	CLOCK_NAME(GPIOG, "gpiog"),
	CLOCK_NAME(GPIOH, "gpioh"),
	CLOCK_NAME(GPIOI, "gpioi"),
	CLOCK_NAME(GPIOJ, "gpioj"),
	CLOCK_NAME(GPIOK, "gpiok"),
	CLOCK_NAME(GPIOZ, "gpioz"),
	/* Clock exposed by SCMI. SCMI clock fmro DT bindings to come... */
	CLOCK_NAME(CK_HSE, "hse"),
	CLOCK_NAME(CK_HSI, "hsi"),
	CLOCK_NAME(CK_CSI, "csi"),
	CLOCK_NAME(CK_LSE, "lse"),
	CLOCK_NAME(CK_LSI, "lsi"),
	CLOCK_NAME(PLL2_Q, "pll2q"),
	CLOCK_NAME(PLL2_R, "pll2r"),
	CLOCK_NAME(PLL3_Q, "pll3q"),
	CLOCK_NAME(PLL3_R, "pll3r"),
	CLOCK_NAME(CRYP1, "cryp1"),
	CLOCK_NAME(HASH1, "hash1"),
	CLOCK_NAME(I2C4_K, "i2c4"),
	CLOCK_NAME(I2C6_K, "i2c6"),
	CLOCK_NAME(IWDG1, "iwdg"),
	CLOCK_NAME(RNG1_K, "rng1"),
	CLOCK_NAME(SPI6_K, "spi6"),
	CLOCK_NAME(USART1_K, "usart1"),
	CLOCK_NAME(CK_MCU, "mcu"),
};
DECLARE_KEEP_PAGER(exposed_clk_name);

static const char *clk_op_get_name(struct clk *clk)
{
	unsigned long clock_id = clk_to_clock_id(clk);
	size_t n = 0;

	for (n = 0; n < ARRAY_SIZE(exposed_clk_name); n++)
		if (exposed_clk_name[n].clock_id == clock_id)
			return exposed_clk_name[n].name;

	return NULL;
}
#else
static const char *clk_op_get_name(struct clk *clk __unused)
{
	return NULL;
}
#endif /*CFG_TEE_CORE_LOG_LEVEL*/

static unsigned long clk_op_compute_rate(struct clk *clk,
					 unsigned long parent_rate __unused)
{
	return _stm32_clock_get_rate(clk_to_clock_id(clk));
}
DECLARE_KEEP_PAGER(clk_op_compute_rate);

static TEE_Result clk_op_enable(struct clk *clk)
{
	if (clk_is_gate(clk)) {
		__clk_enable(clk_to_gate_ref(clk));

		/* Make sure the clock is enabled before returning to caller */
		dsb();
	}

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(clk_op_enable);

static void clk_op_disable(struct clk *clk)
{
	if (clk_is_gate(clk)) {
		/* Make sure the previous operations are visible */
		dsb();

		__clk_disable(clk_to_gate_ref(clk));
	}
}
DECLARE_KEEP_PAGER(clk_op_disable);

/* This variable is weak to break its dependency chain when linked as unpaged */
const struct clk_ops stm32mp1_clk_ops
__weak __relrodata_unpaged("stm32mp1_clk_ops") = {
	.enable = clk_op_enable,
	.disable = clk_op_disable,
	.get_rate = clk_op_compute_rate,
};

static TEE_Result register_stm32mp1_clocks(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t n = 0;

	for (n = 0; n < ARRAY_SIZE(stm32mp1_clk); n++) {
		stm32mp1_clk[n].ops = &stm32mp1_clk_ops;
		stm32mp1_clk[n].name = clk_op_get_name(stm32mp1_clk + n);
		refcount_set(&stm32mp1_clk[n].enabled_count, 0);

		res = clk_register(stm32mp1_clk + n);
		if (res)
			return res;
	}

	return TEE_SUCCESS;
}

#ifdef CFG_DRIVERS_CLK_DT
static struct clk *stm32mp1_clk_dt_get_clk(struct dt_pargs *pargs,
					   void *data __unused, TEE_Result *res)
{
	unsigned long clock_id = pargs->args[0];
	struct clk *clk = NULL;

	*res = TEE_ERROR_BAD_PARAMETERS;

	if (pargs->args_count != 1)
		return NULL;

	clk = clock_id_to_clk(clock_id);
	if (!clk)
		return NULL;

	*res = TEE_SUCCESS;
	return clk;
}

/* Non-null reference for compat data */
static const uint8_t non_secure_rcc;

static void enable_static_secure_clocks(void)
{
	unsigned int idx = 0;
	const unsigned long secure_enable[] = {
		DDRC1, DDRC1LP, DDRC2, DDRC2LP, DDRPHYC, DDRPHYCLP, DDRCAPB,
		AXIDCG, DDRPHYCAPB, DDRPHYCAPBLP, TZPC, TZC1, TZC2, STGEN_K,
		BSEC,
	};

	for (idx = 0; idx < ARRAY_SIZE(secure_enable); idx++) {
		clk_enable(stm32mp_rcc_clock_id_to_clk(secure_enable[idx]));
		stm32mp_register_clock_parents_secure(secure_enable[idx]);
	}

	if (CFG_TEE_CORE_NB_CORE > 1)
		clk_enable(stm32mp_rcc_clock_id_to_clk(RTCAPB));
}

struct stm32_osci_dt_cfg {
	unsigned long freq;
	bool bypass;
	bool digbyp;
	bool css;
	uint32_t drive;
};

enum pll_mn {
	PLL_CFG_M,
	PLL_CFG_N,
	PLL_DIV_MN_NB
};

enum pll_pqr {
	PLL_CFG_P,
	PLL_CFG_Q,
	PLL_CFG_R,
	PLL_DIV_PQR_NB
};

enum pll_csg {
	PLL_CSG_MOD_PER,
	PLL_CSG_INC_STEP,
	PLL_CSG_SSCG_MODE,
	PLL_CSG_NB
};

struct stm32_pll_vco {
	uint32_t status;
	uint32_t src;
	uint32_t div_mn[PLL_DIV_MN_NB];
	uint32_t frac;
	bool csg_enabled;
	uint32_t csg[PLL_CSG_NB];
};

struct stm32_pll_output {
	uint32_t output[PLL_DIV_PQR_NB];
};

struct stm32_pll_dt_cfg {
	struct stm32_pll_vco vco;
	struct stm32_pll_output output;
};

struct stm32_clk_opp_cfg {
	uint32_t frq;
	uint32_t src;
	uint32_t div;
	struct stm32_pll_dt_cfg pll_cfg;
};

struct stm32_clk_opp_dt_cfg {
	struct stm32_clk_opp_cfg mpu_opp[MAX_OPP];
	struct stm32_clk_opp_cfg axi_opp[MAX_OPP];
	struct stm32_clk_opp_cfg mcu_opp[MAX_OPP];
};

struct stm32_clk_platdata {
	uintptr_t rcc_base;
	uint32_t nosci;
	struct stm32_osci_dt_cfg *osci;
	uint32_t npll;
	struct stm32_pll_dt_cfg *pll;
	struct stm32_clk_opp_dt_cfg *opp;
	uint32_t nclksrc;
	uint32_t *clksrc;
	uint32_t nclkdiv;
	uint32_t *clkdiv;
	struct stm32_pinctrl_list *pinctrl_cfg;
};

/*
 * GATE CONFIG
 */

/* Warning GATE_XXX_RDY must follow GATE_XXX */
enum enum_gate_cfg {
	GATE_LSE,
	GATE_LSE_RDY,
	GATE_LSI,
	GATE_LSI_RDY,
	GATE_HSI,
	GATE_HSI_RDY,
	GATE_CSI,
	GATE_CSI_RDY,
	GATE_HSE,
	GATE_HSE_RDY,
	GATE_PLL1,
	GATE_PLL1_RDY,
	GATE_PLL2,
	GATE_PLL2_RDY,
	GATE_PLL3,
	GATE_PLL3_RDY,
	GATE_PLL4,
	GATE_PLL4_RDY,
	GATE_HSIDIVRDY,
	GATE_MPUSRCRDY,
	GATE_AXISSRCRDY,
	GATE_MCUSSRCRDY,
	GATE_PLL12SRCRDY,
	GATE_PLL3SRCRDY,
	GATE_PLL4SRCRDY,
	GATE_MPUDIVRDY,
	GATE_AXIDIVRDY,
	GATE_MCUDIVRDY,
	GATE_APB1DIVRDY,
	GATE_APB2DIVRDY,
	GATE_APB3DIVRDY,
	GATE_APB4DIVRDY,
	GATE_APB5DIVRDY,
	GATE_RTCCK,
	GATE_MCO1,
	GATE_MCO2,
	GATE_NB
};

#define GATE_CFG(_id, _offset, _bit_idx, _offset_clr)\
	[(_id)] = {\
		.offset		= (_offset),\
		.bit_idx	= (_bit_idx),\
		.set_clr	= (_offset_clr),\
	}

static const struct gate_cfg gates_mp15[GATE_NB] = {
	GATE_CFG(GATE_LSE,		RCC_BDCR,		0,	0),
	GATE_CFG(GATE_LSE_RDY,		RCC_BDCR,		2,	0),
	GATE_CFG(GATE_RTCCK,		RCC_BDCR,		20,	0),
	GATE_CFG(GATE_LSI,		RCC_RDLSICR,		0,	0),
	GATE_CFG(GATE_LSI_RDY,		RCC_RDLSICR,		1,	0),
	GATE_CFG(GATE_HSI,		RCC_OCENSETR,		0,	1),
	GATE_CFG(GATE_HSI_RDY,		RCC_OCRDYR,		0,	0),
	GATE_CFG(GATE_CSI,		RCC_OCENSETR,		4,	1),
	GATE_CFG(GATE_CSI_RDY,		RCC_OCRDYR,		4,	0),
	GATE_CFG(GATE_HSE,		RCC_OCENSETR,		8,	1),
	GATE_CFG(GATE_HSE_RDY,		RCC_OCRDYR,		8,	0),
	GATE_CFG(GATE_HSIDIVRDY,	RCC_OCRDYR,		2,	0),
	GATE_CFG(GATE_MPUSRCRDY,	RCC_MPCKSELR,		31,	0),
	GATE_CFG(GATE_AXISSRCRDY,	RCC_ASSCKSELR,		31,	0),
	GATE_CFG(GATE_MCUSSRCRDY,	RCC_MSSCKSELR,		31,	0),
	GATE_CFG(GATE_PLL12SRCRDY,	RCC_RCK12SELR,		31,	0),
	GATE_CFG(GATE_PLL3SRCRDY,	RCC_RCK3SELR,		31,	0),
	GATE_CFG(GATE_PLL4SRCRDY,	RCC_RCK4SELR,		31,	0),
	GATE_CFG(GATE_MPUDIVRDY,	RCC_MPCKDIVR,		31,	0),
	GATE_CFG(GATE_AXIDIVRDY,	RCC_AXIDIVR,		31,	0),
	GATE_CFG(GATE_MCUDIVRDY,	RCC_MCUDIVR,		31,	0),
	GATE_CFG(GATE_APB1DIVRDY,	RCC_APB1DIVR,		31,	0),
	GATE_CFG(GATE_APB2DIVRDY,	RCC_APB2DIVR,		31,	0),
	GATE_CFG(GATE_APB3DIVRDY,	RCC_APB3DIVR,		31,	0),
	GATE_CFG(GATE_APB4DIVRDY,	RCC_APB4DIVR,		31,	0),
	GATE_CFG(GATE_APB5DIVRDY,	RCC_APB5DIVR,		31,	0),
	GATE_CFG(GATE_PLL1,		RCC_PLL1CR,		0,	0),
	GATE_CFG(GATE_PLL1_RDY,		RCC_PLL1CR,		1,	0),
	GATE_CFG(GATE_PLL2,		RCC_PLL2CR,		0,	0),
	GATE_CFG(GATE_PLL2_RDY,		RCC_PLL2CR,		1,	0),
	GATE_CFG(GATE_PLL3,		RCC_PLL3CR,		0,	0),
	GATE_CFG(GATE_PLL3_RDY,		RCC_PLL3CR,		1,	0),
	GATE_CFG(GATE_PLL4,		RCC_PLL4CR,		0,	0),
	GATE_CFG(GATE_PLL4_RDY,		RCC_PLL4CR,		1,	0),
	GATE_CFG(GATE_MCO1,		RCC_MCO1CFGR,		12,	0),
	GATE_CFG(GATE_MCO2,		RCC_MCO2CFGR,		12,	0),
};

/*
 * DIV CONFIG
 */

static const struct div_table_cfg axi_div_table[] = {
	{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 4 },
	{ 4, 4 }, { 5, 4 }, { 6, 4 }, { 7, 4 },
	{ 0 },
};

static const struct div_table_cfg mcu_div_table[] = {
	{ 0, 1 }, { 1, 2 }, { 2, 4 }, { 3, 8 },
	{ 4, 16 }, { 5, 32 }, { 6, 64 }, { 7, 128 },
	{ 8, 256 }, { 9, 512 }, { 10, 512}, { 11, 512 },
	{ 12, 512 }, { 13, 512 }, { 14, 512}, { 15, 512 },
	{ 0 },
};

static const struct div_table_cfg apb_div_table[] = {
	{ 0, 1 }, { 1, 2 }, { 2, 4 }, { 3, 8 },
	{ 4, 16 }, { 5, 16 }, { 6, 16 }, { 7, 16 },
	{ 0 },
};

static const struct div_table_cfg ck_trace_div_table[] = {
	{ 0, 1 }, { 1, 2 }, { 2, 4 }, { 3, 8 },
	{ 4, 16 }, { 5, 16 }, { 6, 16 }, { 7, 16 },
	{ 0 },
};

#define DIVRDY_CFG(_id, _offset, _shift, _width, _flags, _table, _ready)\
	[(_id)] = {\
		.offset	= (_offset),\
		.shift	= (_shift),\
		.width	= (_width),\
		.flags	= (_flags),\
		.table	= (_table),\
		.ready	= (_ready),\
	}

#define DIV_CFG(_id, _offset, _shift, _width, _flags, _table)\
	DIVRDY_CFG(_id, _offset, _shift, _width, _flags, _table, DIV_NO_RDY)

static const struct div_cfg dividers_mp15[] = {
	DIVRDY_CFG(DIV_MPU, RCC_MPCKDIVR, 0, 4, 0, NULL,
		   GATE_MPUDIVRDY),
	DIVRDY_CFG(DIV_AXI, RCC_AXIDIVR, 0, 3, 0, axi_div_table,
		   GATE_AXIDIVRDY),
	DIVRDY_CFG(DIV_MCU,	RCC_MCUDIVR, 0, 4, 0, mcu_div_table,
		   GATE_MCUDIVRDY),
	DIVRDY_CFG(DIV_APB1, RCC_APB1DIVR, 0, 3, 0, apb_div_table,
		   GATE_APB1DIVRDY),
	DIVRDY_CFG(DIV_APB2, RCC_APB2DIVR, 0, 3, 0, apb_div_table,
		   GATE_APB2DIVRDY),
	DIVRDY_CFG(DIV_APB3, RCC_APB3DIVR, 0, 3, 0, apb_div_table,
		   GATE_APB3DIVRDY),
	DIVRDY_CFG(DIV_APB4, RCC_APB4DIVR, 0, 3, 0, apb_div_table,
		   GATE_APB4DIVRDY),
	DIVRDY_CFG(DIV_APB5, RCC_APB5DIVR, 0, 3, 0, apb_div_table,
		   GATE_APB5DIVRDY),
	DIVRDY_CFG(DIV_HSI,	RCC_HSICFGR, 0, 2, CLK_DIVIDER_POWER_OF_TWO,
		   NULL, GATE_HSIDIVRDY),
	DIV_CFG(DIV_RTC,	RCC_RTCDIVR, 0, 6, 0, NULL),
	DIV_CFG(DIV_MCO1,	RCC_MCO1CFGR, 4, 4, 0, NULL),
	DIV_CFG(DIV_MCO2,	RCC_MCO2CFGR, 4, 4, 0, NULL),
	DIV_CFG(DIV_TRACE,	RCC_DBGCFGR, 0, 3, 0, ck_trace_div_table),
	DIV_CFG(DIV_ETHPTP,	RCC_ETHCKSELR, 4, 4, 0, NULL),
};

/*
 * MUX CONFIG
 */
#define MUXRDY_CFG(_id, _offset, _shift, _witdh, _rdy)\
	[(_id)] = {\
			.offset	= (_offset),\
			.shift	= (_shift),\
			.width	= (_witdh),\
			.ready	= (_rdy),\
	}

#define MUX_CFG(_id, _offset, _shift, _witdh)\
	MUXRDY_CFG(_id, _offset, _shift, _witdh, MUX_NO_RDY)

static const struct mux_cfg parent_mp15[MUX_NB] = {
	MUX_CFG(MUX_PLL12,	RCC_RCK12SELR, 0, 2),
	MUX_CFG(MUX_PLL3,	RCC_RCK3SELR, 0, 2),
	MUX_CFG(MUX_PLL4,	RCC_RCK4SELR, 0, 2),
	MUX_CFG(MUX_CKPER,	RCC_CPERCKSELR, 0, 2),
	MUXRDY_CFG(MUX_MPU,	RCC_MPCKSELR, 0, 2, GATE_MPUSRCRDY),
	MUXRDY_CFG(MUX_AXI,	RCC_ASSCKSELR, 0, 3, GATE_AXISSRCRDY),
	MUXRDY_CFG(MUX_MCU,	RCC_MSSCKSELR, 0, 2, GATE_MCUSSRCRDY),
	MUX_CFG(MUX_RTC,	RCC_BDCR, 16, 2),
	MUX_CFG(MUX_SDMMC12,	RCC_SDMMC12CKSELR, 0, 3),
	MUX_CFG(MUX_SPI2S23,	RCC_SPI2S23CKSELR, 0, 3),
	MUX_CFG(MUX_SPI45,	RCC_SPI45CKSELR, 0, 3),
	MUX_CFG(MUX_I2C12,	RCC_I2C12CKSELR, 0, 3),
	MUX_CFG(MUX_I2C35,	RCC_I2C35CKSELR, 0, 3),
	MUX_CFG(MUX_LPTIM23,	RCC_LPTIM23CKSELR, 0, 3),
	MUX_CFG(MUX_LPTIM45,	RCC_LPTIM45CKSELR, 0, 3),
	MUX_CFG(MUX_UART24,	RCC_UART24CKSELR, 0, 3),
	MUX_CFG(MUX_UART35,	RCC_UART35CKSELR, 0, 3),
	MUX_CFG(MUX_UART78,	RCC_UART78CKSELR, 0, 3),
	MUX_CFG(MUX_SAI1,	RCC_SAI1CKSELR, 0, 3),
	MUX_CFG(MUX_ETH,	RCC_ETHCKSELR, 0, 2),
	MUX_CFG(MUX_I2C46,	RCC_I2C46CKSELR, 0, 3),
	MUX_CFG(MUX_RNG2,	RCC_RNG2CKSELR, 0, 2),
	MUX_CFG(MUX_SDMMC3,	RCC_SDMMC3CKSELR, 0, 3),
	MUX_CFG(MUX_FMC,	RCC_FMCCKSELR, 0, 2),
	MUX_CFG(MUX_QSPI,	RCC_QSPICKSELR, 0, 2),
	MUX_CFG(MUX_USBPHY,	RCC_USBCKSELR, 0, 2),
	MUX_CFG(MUX_USBO,	RCC_USBCKSELR, 4, 1),
	MUX_CFG(MUX_SPDIF,	RCC_SPDIFCKSELR, 0, 2),
	MUX_CFG(MUX_SPI2S1,	RCC_SPI2S1CKSELR, 0, 3),
	MUX_CFG(MUX_CEC,	RCC_CECCKSELR, 0, 2),
	MUX_CFG(MUX_LPTIM1,	RCC_LPTIM1CKSELR, 0, 3),
	MUX_CFG(MUX_UART6,	RCC_UART6CKSELR, 0, 3),
	MUX_CFG(MUX_FDCAN,	RCC_FDCANCKSELR, 0, 2),
	MUX_CFG(MUX_SAI2,	RCC_SAI2CKSELR, 0, 3),
	MUX_CFG(MUX_SAI3,	RCC_SAI3CKSELR, 0, 3),
	MUX_CFG(MUX_SAI4,	RCC_SAI4CKSELR, 0, 3),
	MUX_CFG(MUX_ADC,	RCC_ADCCKSELR, 0, 2),
	MUX_CFG(MUX_DSI,	RCC_DSICKSELR, 0, 1),
	MUX_CFG(MUX_RNG1,	RCC_RNG1CKSELR, 0, 2),
	MUX_CFG(MUX_STGEN,	RCC_STGENCKSELR, 0, 2),
	MUX_CFG(MUX_UART1,	RCC_UART1CKSELR, 0, 3),
	MUX_CFG(MUX_SPI6,	RCC_SPI6CKSELR, 0, 3),
	MUX_CFG(MUX_MCO1,	RCC_MCO1CFGR, 0, 3),
	MUX_CFG(MUX_MCO2,	RCC_MCO2CFGR, 0, 3),
};

struct stm32_osc_cfg {
	int osc_id;
};

struct clk_stm32_bypass {
	uint16_t offset;
	uint8_t bit_byp;
	uint8_t bit_digbyp;
};

struct clk_stm32_css {
	uint16_t offset;
	uint8_t bit_css;
};

struct clk_stm32_drive {
	uint16_t offset;
	uint8_t drv_shift;
	uint8_t drv_width;
	uint8_t drv_default;
};

struct clk_oscillator_data {
	const char *name;
	uint16_t gate_id;
	struct clk_stm32_bypass *bypass;
	struct clk_stm32_css *css;
	struct clk_stm32_drive *drive;
};

#define BYPASS(_offset, _bit_byp, _bit_digbyp) (&(struct clk_stm32_bypass){\
	.offset		= (_offset),\
	.bit_byp	= (_bit_byp),\
	.bit_digbyp	= (_bit_digbyp),\
})

#define CSS(_offset, _bit_css) (&(struct clk_stm32_css){\
	.offset		= (_offset),\
	.bit_css	= (_bit_css),\
})

#define DRIVE(_offset, _shift, _width, _default) (&(struct clk_stm32_drive){\
	.offset		= (_offset),\
	.drv_shift	= (_shift),\
	.drv_width	= (_width),\
	.drv_default	= (_default),\
})

#define OSCILLATOR(idx_osc, _name, _gate_id, _bypass, _css, _drive) \
	[(idx_osc)] = (struct clk_oscillator_data){\
		.name		= (_name),\
		.gate_id	= (_gate_id),\
		.bypass		= (_bypass),\
		.css		= (_css),\
		.drive		= (_drive),\
	}

static const struct clk_oscillator_data stm32mp15_osc_data[NB_OSC] = {
	OSCILLATOR(OSC_HSI, "clk-hsi", GATE_HSI,
		   NULL, NULL, NULL),

	OSCILLATOR(OSC_LSI, "clk-lsi", GATE_LSI,
		   NULL, NULL, NULL),

	OSCILLATOR(OSC_CSI, "clk-csi", GATE_CSI,
		   NULL, NULL, NULL),

	OSCILLATOR(OSC_LSE, "clk-lse", GATE_LSE,
		   BYPASS(RCC_BDCR, 1, 3),
		   CSS(RCC_BDCR, 8),
		   DRIVE(RCC_BDCR, 4, 2, 2)),

	OSCILLATOR(OSC_HSE, "clk-hse", GATE_HSE,
		   BYPASS(RCC_OCENSETR, 10, 7),
		   CSS(RCC_OCENSETR, 11),
		   NULL),
	OSCILLATOR(OSC_I2S_CKIN, "i2s_ckin", -1, NULL, NULL, NULL),
	OSCILLATOR(OSC_USB_PHY_48, "ck_usbo_48m", -1, NULL, NULL, NULL),
};

static const struct clk_oscillator_data *clk_oscillator_get_data(int osc_id)
{
	assert(osc_id >= 0 && osc_id < (int)ARRAY_SIZE(stm32mp15_osc_data));

	return &stm32mp15_osc_data[osc_id];
}

static unsigned long clk_stm32_get_rate_oscillateur(int osc_id)
{
	struct clk_stm32_priv *priv = clk_stm32_get_priv();
	struct stm32_clk_platdata *pdata = priv->pdata;
	struct stm32_osci_dt_cfg *osci = &pdata->osci[osc_id];

	return osci->freq;
}

static void
clk_oscillator_set_bypass(struct clk_stm32_priv *priv,
			  const struct clk_oscillator_data *osc_data,
			  bool digbyp, bool bypass)
{
	const struct clk_stm32_bypass *bypass_data = osc_data->bypass;
	uintptr_t address = 0;

	if (!bypass_data)
		return;

	address = priv->base + bypass_data->offset;

	if (digbyp)
		io_setbits32(address, BIT(bypass_data->bit_digbyp));

	if (bypass || digbyp)
		io_setbits32(address, BIT(bypass_data->bit_byp));
}

static void clk_oscillator_set_css(struct clk_stm32_priv *priv,
				   const struct clk_oscillator_data *osc_data,
				   bool css)
{
	struct clk_stm32_css *css_data = osc_data->css;
	uintptr_t address = 0;

	if (!css_data)
		return;

	address = priv->base + css_data->offset;

	if (css)
		io_setbits32(address, BIT(css_data->bit_css));
}

static void clk_oscillator_set_drive(struct clk_stm32_priv *priv,
				     const struct clk_oscillator_data *osc_data,
				     uint8_t lsedrv)
{
	struct clk_stm32_drive *drive_data = osc_data->drive;
	uintptr_t address = 0;
	uint32_t mask = 0;
	uint32_t value = 0;

	if (!drive_data)
		return;

	address = priv->base + drive_data->offset;

	mask = (BIT(drive_data->drv_width) - 1U) << drive_data->drv_shift;

	/*
	 * Warning: not recommended to switch directly from "high drive"
	 * to "medium low drive", and vice-versa.
	 */
	value = (io_read32(address) & mask) >> drive_data->drv_shift;

	while (value != lsedrv) {
		if (value > lsedrv)
			value--;
		else
			value++;

		io_clrsetbits32(address, mask, value << drive_data->drv_shift);
	}
}

static void stm32_enable_oscillator_hse(struct clk_stm32_priv *priv,
					struct stm32_clk_platdata *pdata)
{
	struct stm32_osci_dt_cfg *osci = &pdata->osci[OSC_HSE];
	const struct clk_oscillator_data *osc_data = NULL;

	if (osci->freq == 0U)
		return;

	osc_data = clk_oscillator_get_data(OSC_HSE);

	clk_oscillator_set_bypass(priv, osc_data,  osci->digbyp, osci->bypass);

	/* Enable clock and wait ready bit */
	if (stm32_gate_rdy_enable(osc_data->gate_id)) {
		EMSG("timeout to enable hse clock");
		panic();
	}

	clk_oscillator_set_css(priv, osc_data, osci->css);
}

static void stm32_enable_oscillator_lse(struct clk_stm32_priv *priv,
					struct stm32_clk_platdata *pdata)
{
	struct stm32_osci_dt_cfg *osci = &pdata->osci[OSC_LSE];
	const struct clk_oscillator_data *osc_data = NULL;

	if (osci->freq == 0U)
		return;

	osc_data = clk_oscillator_get_data(OSC_LSE);

	if (stm32_gate_is_enabled(osc_data->gate_id))
		return;

	clk_oscillator_set_bypass(priv, osc_data, osci->digbyp, osci->bypass);

	clk_oscillator_set_drive(priv, osc_data,  osci->drive);

	/* Enable lse clock, but don't wait ready bit */
	stm32_gate_enable(osc_data->gate_id);
}

static void
stm32_enable_oscillator_lsi(struct clk_stm32_priv *priv __maybe_unused,
			    struct stm32_clk_platdata *pdata)
{
	struct stm32_osci_dt_cfg *osci = &pdata->osci[OSC_LSI];
	const struct clk_oscillator_data *osc_data = NULL;

	if (osci->freq == 0U)
		return;

	osc_data = clk_oscillator_get_data(OSC_LSI);

	/* Enable clock and wait ready bit */
	if (stm32_gate_rdy_enable(osc_data->gate_id)) {
		EMSG("timeout to enable lsi clock");
		panic();
	}
}

static void
stm32_enable_oscillator_csi(struct clk_stm32_priv *priv __maybe_unused,
			    struct stm32_clk_platdata *pdata)
{
	struct stm32_osci_dt_cfg *osci = &pdata->osci[OSC_CSI];
	const struct clk_oscillator_data *osc_data = NULL;

	if (osci->freq == 0U)
		return;

	osc_data = clk_oscillator_get_data(OSC_CSI);

	/* Enable clock and wait ready bit */
	if (stm32_gate_rdy_enable(osc_data->gate_id)) {
		EMSG("timeout to enable csi clock");
		panic();
	}
}

static int stm32_clk_oscillators_lse_set_css(struct clk_stm32_priv *priv,
					     struct stm32_clk_platdata *pdata)

{
	struct stm32_osci_dt_cfg *osci = &pdata->osci[OSC_LSE];
	const struct clk_oscillator_data *osc_data = NULL;

	osc_data = clk_oscillator_get_data(OSC_LSE);

	clk_oscillator_set_css(priv, osc_data, osci->css);

	return 0;
}

static int
stm32_clk_oscillators_wait_lse_ready(struct clk_stm32_priv *priv __maybe_unused,
				     struct stm32_clk_platdata *pdata)
{
	struct stm32_osci_dt_cfg *osci = &pdata->osci[OSC_LSE];
	const struct clk_oscillator_data *osc_data = NULL;

	if (osci->freq == 0U)
		return 0;

	osc_data = clk_oscillator_get_data(OSC_LSE);

	if (stm32_gate_wait_ready(osc_data->gate_id, true))
		return -1;

	return 0;
}

static void stm32_clk_oscillators_enable(struct clk_stm32_priv *priv,
					 struct stm32_clk_platdata *pdata)
{
	stm32_enable_oscillator_hse(priv, pdata);
	stm32_enable_oscillator_lse(priv, pdata);
	stm32_enable_oscillator_lsi(priv, pdata);
	stm32_enable_oscillator_csi(priv, pdata);
}

#define RCC_OFFSET_PLLXCR		0
#define RCC_OFFSET_PLLXCFGR1		4
#define RCC_OFFSET_PLLXCFGR2		8
#define RCC_OFFSET_PLLXFRACR		12
#define RCC_OFFSET_PLLXCSGR		16

struct stm32_clk_pll {
	enum stm32mp1_plltype plltype;
	uint16_t gate_id;
	uint16_t mux_id;
	uint16_t reg_pllxcr;
};

#define CLK_PLL_CFG(_idx, _type, _gate_id, _mux_id, _reg)\
	[(_idx)] = {\
		.gate_id = (_gate_id),\
		.mux_id = (_mux_id),\
		.plltype = (_type),\
		.reg_pllxcr = (_reg),\
	}

static const struct stm32_clk_pll stm32_mp15_clk_pll[_PLL_NB] = {
	CLK_PLL_CFG(_PLL1, PLL_1600, GATE_PLL1, MUX_PLL12, RCC_PLL1CR),
	CLK_PLL_CFG(_PLL2, PLL_1600, GATE_PLL2, MUX_PLL12, RCC_PLL2CR),
	CLK_PLL_CFG(_PLL3, PLL_800, GATE_PLL3, MUX_PLL3, RCC_PLL3CR),
	CLK_PLL_CFG(_PLL4, PLL_800, GATE_PLL4, MUX_PLL4, RCC_PLL4CR),
};

static const struct stm32_clk_pll *clk_stm32_pll_data(unsigned int idx)
{
	return &stm32_mp15_clk_pll[idx];
}

#ifdef CFG_STM32_CLK_DEBUG
static void clk_stm32_debug_display_pll_cfg(int pll_id,
					    struct stm32_pll_dt_cfg *pll)
{
	struct stm32_pll_vco *vco = &pll->vco;
	struct stm32_pll_output *out = &pll->output;
	unsigned int j = 0;

	printf("PLL%d : %s", pll_id + 1, vco->status ? "" : "disabled");

	if (!vco->status) {
		printf("\n");
		return;
	}

	printf(" vco = < ");

	for (j = 0; j < PLL_DIV_MN_NB; j++)
		printf("%d ", vco->div_mn[j]);

	printf("> ");

	printf("frac = 0x%x ", vco->frac);

	printf("src = 0x%x ", vco->src);

	if (vco->csg_enabled) {
		printf("csg = < ");

		for (j = 0; j < PLL_CSG_NB; j++)
			printf("%d ", vco->csg[j]);

		printf("> ");
	}

	printf("output = < ");

	for (j = 0; j < PLL_DIV_PQR_NB; j++)
		printf("%d ", out->output[j]);

	printf(">\n");
}

static void clk_stm32_debug_display_opp_cfg(const char *opp_name,
					    struct stm32_clk_opp_cfg *opp_cfg)
{
	unsigned int i = 0;

	printf("\nOPP %s :\n", opp_name);

	for (i = 0; i < MAX_OPP; i++) {
		if (opp_cfg->frq == 0UL)
			break;

		printf("frequency = %d src = 0x%x div = 0x%x ", opp_cfg->frq,
		       opp_cfg->src, opp_cfg->div);

		clk_stm32_debug_display_pll_cfg(_PLL1, &opp_cfg->pll_cfg);

		opp_cfg++;
	}

	printf("\n");
}

static void clk_stm32_debug_display_opp_dt_cfg(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	struct stm32_clk_opp_dt_cfg *opp = pdata->opp;

	clk_stm32_debug_display_opp_cfg("st,ck_mpu", opp->mpu_opp);
	clk_stm32_debug_display_opp_cfg("st,ck_axi", pdata->opp->axi_opp);
	clk_stm32_debug_display_opp_cfg("st,ck_mcu", pdata->opp->mcu_opp);
}

static void clk_stm32_debug_display_pll_dt_cfg(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	size_t i = 0;

	for (i = _PLL1; i < pdata->npll; i++)
		clk_stm32_debug_display_pll_cfg(i, &pdata->pll[i]);
}

static void clk_stm32_debug_display_osc_dt_cfg(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	size_t nb = pdata->nosci;
	size_t i = 0;

	printf("\nNumber of oscillators = %d\n", nb);

	for (i = 0; i < nb; i++) {
		struct stm32_osci_dt_cfg *osc = &pdata->osci[i];
		const struct clk_oscillator_data *osc_data = NULL;

		osc_data = clk_oscillator_get_data(i);

		printf("%s %ld bypass = %d digbyp = %d css = %d drive = %d\n",
		       osc_data->name,
		       osc->freq,
		       osc->bypass,
		       osc->digbyp,
		       osc->css,
		       osc->drive);
	}
}

static void clk_stm32_debug_display_src_dt_cfg(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	size_t i = 0;

	printf("nb st,clksrc = %d\n", pdata->nclksrc);

	for (i = 0; i < pdata->nclksrc; i++)
		printf("\t0x%x\n", pdata->clksrc[i]);

	printf("\n");
}

static void clk_stm32_debug_display_div_dt_cfg(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	size_t i = 0;

	printf("nb st,clkdiv = %d\n", pdata->nclkdiv);

	for (i = 0; i < pdata->nclkdiv; i++)
		printf("\t%d\n", pdata->clkdiv[i]);

	printf("\n");
}

static void clk_stm32_debug_display_pdata(void)
{
	struct clk_stm32_priv *priv = clk_stm32_get_priv();

	clk_stm32_debug_display_pll_dt_cfg(priv);
	clk_stm32_debug_display_opp_dt_cfg(priv);
	clk_stm32_debug_display_osc_dt_cfg(priv);
	clk_stm32_debug_display_src_dt_cfg(priv);
	clk_stm32_debug_display_div_dt_cfg(priv);
}
#endif

static int fdt_clk_stm32_parse_oscillator(const void *fdt, int node,
					  const char *name,
					  struct stm32_osci_dt_cfg *osci)
{
	int subnode = 0;

	fdt_for_each_subnode(subnode, fdt, node) {
		const char *cchar = NULL;
		const fdt32_t *cuint = NULL;
		int ret = 0;

		cchar = fdt_get_name(fdt, subnode, &ret);
		if (!cchar)
			return ret;

		if (strncmp(cchar, name, (size_t)ret) ||
		    _fdt_get_status(fdt, subnode) == DT_STATUS_DISABLED)
			continue;

		cuint = fdt_getprop(fdt, subnode, "clock-frequency", &ret);
		if (!cuint)
			panic();

		osci->freq = fdt32_to_cpu(*cuint);

		if (fdt_getprop(fdt, subnode, "st,bypass", NULL))
			osci->bypass = true;

		if (fdt_getprop(fdt, subnode, "st,digbypass", NULL))
			osci->digbyp = true;

		if (fdt_getprop(fdt, subnode, "st,css", NULL))
			osci->css = true;

		osci->drive = _fdt_read_uint32_default(fdt, subnode, "st,drive",
						       LSEDRV_MEDIUM_HIGH);

		return 0;
	}

	return -FDT_ERR_NOTFOUND;
}

static int fdt_stm32_clk_parse_all_oscillator(const void *fdt,
					      int node __maybe_unused,
					      struct stm32_clk_platdata *pdata)
{
	int fdt_err = 0;
	size_t i = 0;
	int osc_node = 0;

	osc_node = fdt_path_offset(fdt, "/clocks");
	if (osc_node < 0)
		return -FDT_ERR_NOTFOUND;

	for (i = 0; i < NB_OSC; i++) {
		struct stm32_osci_dt_cfg *osci = &pdata->osci[i];
		const struct clk_oscillator_data *osc_data = NULL;

		osc_data = clk_oscillator_get_data(i);

		fdt_err = fdt_clk_stm32_parse_oscillator(fdt, osc_node,
							 osc_data->name, osci);
		if (fdt_err < 0)
			osci->freq = 0UL;
	}

	return 0;
}

static int fdt_clk_stm32_load_vco_config(const void *fdt, int subnode,
					 struct stm32_pll_vco *vco)
{
	int ret = 0;

	ret = _fdt_read_uint32_array(fdt, subnode, "divmn", vco->div_mn,
				     PLL_DIV_MN_NB);
	if (ret != 0)
		return ret;

	ret = _fdt_read_uint32_array(fdt, subnode, "csg", vco->csg,
				     PLL_CSG_NB);

	vco->csg_enabled = (ret == 0);

	if (ret == -FDT_ERR_NOTFOUND)
		ret = 0;

	if (ret != 0)
		return ret;

	vco->status = RCC_PLLNCR_DIVPEN | RCC_PLLNCR_DIVQEN |
		      RCC_PLLNCR_DIVREN | RCC_PLLNCR_PLLON;

	vco->frac = _fdt_read_uint32_default(fdt, subnode, "frac", 0);

	vco->src = _fdt_read_uint32_default(fdt, subnode, "src", UINT32_MAX);

	return 0;
}

static int fdt_clk_stm32_load_output_config(const void *fdt, int subnode,
					    struct stm32_pll_output *output)
{
	return _fdt_read_uint32_array(fdt, subnode, "st,pll_div_pqr",
				      output->output, (int)PLL_DIV_PQR_NB);
}

static int fdt_clk_stm32_parse_pll(const void *fdt, int subnode,
				   struct stm32_pll_dt_cfg *pll)
{
	const fdt32_t *cuint = NULL;
	int subnode_pll = 0;
	int subnode_vco = 0;
	int err = 0;

	cuint = fdt_getprop(fdt, subnode, "st,pll", NULL);
	if (!cuint)
		return -FDT_ERR_NOTFOUND;

	subnode_pll = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(*cuint));
	if (subnode_pll < 0)
		return -FDT_ERR_NOTFOUND;

	cuint = fdt_getprop(fdt, subnode_pll, "st,pll_vco", NULL);
	if (!cuint)
		return -FDT_ERR_NOTFOUND;

	subnode_vco = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(*cuint));
	if (subnode_vco < 0)
		return -FDT_ERR_NOTFOUND;

	err = fdt_clk_stm32_load_vco_config(fdt, subnode_vco, &pll->vco);
	if (err != 0)
		return err;

	err = fdt_clk_stm32_load_output_config(fdt, subnode_pll, &pll->output);
	if (err != 0)
		return err;

	return 0;
}

#define RCC_PLL_NAME_SIZE	12

static int fdt_stm32_clk_parse_all_pll(const void *fdt, int node,
				       struct stm32_clk_platdata *pdata)
{
	size_t i = 0;

	for (i = _PLL1; i < pdata->npll; i++) {
		struct stm32_pll_dt_cfg *pll = pdata->pll + i;
		char name[RCC_PLL_NAME_SIZE] = { 0 };
		int subnode = 0;
		int err = 0;

		err = snprintf(name, sizeof(name), "st,pll@%d", i);
		if (err < 0 || (size_t)err >= sizeof(name))
			panic();

		subnode = fdt_subnode_offset(fdt, node, name);
		if (subnode < 0)
			continue;

		err = fdt_clk_stm32_parse_pll(fdt, subnode, pll);
		if (err != 0)
			panic();
	}

	return 0;
}

static int fdt_stm32_clk_parse_opp(const void *fdt, int node,
				   const char *opp_name,
				   struct stm32_clk_opp_cfg *opp_cfg)
{
	int subnode = 0;
	unsigned int nb_opp = 0;
	int ret = 0;

	node = fdt_subnode_offset(fdt, node, opp_name);
	if (node == -FDT_ERR_NOTFOUND)
		return 0;
	if (node < 0)
		return node;

	fdt_for_each_subnode(subnode, fdt, node) {
		if (nb_opp >= MAX_OPP) {
			EMSG("%d MAX opp in %s", MAX_OPP, opp_name);
			panic();
		}

		opp_cfg->frq = _fdt_read_uint32_default(fdt, subnode,
							"hz",
							UINT32_MAX);

		opp_cfg->src = _fdt_read_uint32_default(fdt, subnode,
							"st,clksrc",
							UINT32_MAX);

		opp_cfg->div = _fdt_read_uint32_default(fdt, subnode,
							"st,clkdiv",
							UINT32_MAX);

		ret = fdt_clk_stm32_parse_pll(fdt, subnode, &opp_cfg->pll_cfg);
		if (ret)
			return ret;

		opp_cfg++;
		nb_opp++;
	}

	return 0;
}

static int fdt_stm32_clk_parse_all_opp(const void *fdt, int node,
				       struct stm32_clk_platdata *pdata)
{
	struct stm32_clk_opp_dt_cfg *opp = pdata->opp;
	int ret = 0;

	node = fdt_subnode_offset(fdt, node, "st,clk_opp");
	/* No opp are defined */
	if (node == -FDT_ERR_NOTFOUND)
		return 0;
	if (node < 0)
		return node;

	ret = fdt_stm32_clk_parse_opp(fdt, node, "st,ck_mpu", opp->mpu_opp);
	if (ret)
		return ret;

	ret = fdt_stm32_clk_parse_opp(fdt, node, "st,ck_axi", opp->axi_opp);
	if (ret)
		return ret;

	ret = fdt_stm32_clk_parse_opp(fdt, node, "st,ck_mcu", opp->mcu_opp);
	if (ret)
		return ret;

	return 0;
}

static int fdt_stm32_clk_parse(const void *fdt, int node,
			       struct stm32_clk_platdata *pdata)
{
	int err = 0;

	err = fdt_stm32_clk_parse_all_oscillator(fdt, node, pdata);
	if (err != 0)
		return err;

	err = fdt_stm32_clk_parse_all_pll(fdt, node, pdata);
	if (err != 0)
		return err;

	err = fdt_stm32_clk_parse_all_opp(fdt, node, pdata);
	if (err != 0)
		return err;

	err = fdt_clk_stm32_parse_by_name(fdt, node, "st,clkdiv", pdata->clkdiv,
					  &pdata->nclkdiv);
	if (err != 0)
		return err;

	err = fdt_clk_stm32_parse_by_name(fdt, node, "st,clksrc", pdata->clksrc,
					  &pdata->nclksrc);
	if (err != 0)
		return err;

	return 0;
}

/* Clock TREE configuration */
static int stm32_clk_configure_mux(uint32_t data)
{
	int mux = (data & MUX_ID_MASK) >> MUX_ID_SHIFT;
	int sel = (data & MUX_SEL_MASK) >> MUX_SEL_SHIFT;

	if (mux == MUX_RTC) {
		/* Mux RTC clock only is selector is valid and RTC not yet
		 * enabled
		 */
		if (sel == 0)
			return 0;

		if (stm32_gate_is_enabled(GATE_RTCCK))
			return 0;
	}

	if (stm32_mux_set_parent(mux, sel))
		return -1;

	return 0;
}

static int stm32_clk_configure_clk(struct clk_stm32_priv *priv __maybe_unused,
				   uint32_t data)
{
	int sel = (data & CLK_SEL_MASK) >> CLK_SEL_SHIFT;
	int enable = (data & CLK_ON_MASK) >> CLK_ON_SHIFT;
	int clk_id = (data & CLK_ID_MASK) >> CLK_ID_SHIFT;

	if (clk_id == RTC) {
		uintptr_t address = priv->base + RCC_BDCR;

		if (((io_read32(address) & RCC_BDCR_RTCCKEN) == 0U) || enable) {
			io_clrsetbits32(address, RCC_BDCR_RTCSRC_MASK,
					(sel & RCC_SELR_SRC_MASK) <<
					RCC_BDCR_RTCSRC_SHIFT);

			io_setbits32(address, RCC_BDCR_RTCCKEN);
		}
	}

	return 0;
}

static int stm32_clk_configure_by_addr_val(struct clk_stm32_priv *priv,
					   uint32_t data)
{
	uint32_t addr = data >> CLK_ADDR_SHIFT;
	uint32_t val = data & CLK_ADDR_VAL_MASK;

	io_setbits32(priv->base + addr, val);

	return 0;
}

static int stm32_clk_source_configure(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	bool ckper_disabled = false;
	int ret = 0;
	size_t i = 0;

	for (i = 0; i < pdata->nclksrc; i++) {
		uint32_t val = pdata->clksrc[i];
		uint32_t cmd = 0;
		uint32_t cmd_data = 0;

		if (val & CMD_ADDR_BIT) {
			ret = stm32_clk_configure_by_addr_val(priv, val &
							      ~CMD_ADDR_BIT);
			if (ret != 0)
				return ret;

			continue;
		}

		if (val == (uint32_t)CLK_CKPER_DISABLED) {
			ckper_disabled = true;
			continue;
		}

		cmd = (val & CMD_MASK) >> CMD_SHIFT;
		cmd_data = val & ~CMD_MASK;

		switch (cmd) {
		case CMD_MUX:
			ret = stm32_clk_configure_mux(cmd_data);
			break;

		case CMD_CLK:
			ret = stm32_clk_configure_clk(priv, cmd_data);
			break;
		default:
			ret = -1;
			break;
		}

		if (ret != 0)
			return ret;
	}

	/*
	 * CKPER is source for some peripheral clocks
	 * (FMC-NAND / QPSI-NOR) and switching source is allowed
	 * only if previous clock is still ON
	 * => deactivate CKPER only after switching clock
	 */
	if (ckper_disabled) {
		ret = stm32_clk_configure_mux(CLK_CKPER_DISABLED);
		if (ret != 0)
			return ret;
	}

	return 0;
}

#define MAX_HSI_HZ		64000000

static int stm32_clk_hsidiv_configure(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	struct stm32_osci_dt_cfg *osci = &pdata->osci[OSC_HSI];

	return stm32_div_set_rate(DIV_HSI, osci->freq, MAX_HSI_HZ);
}

static TEE_Result stm32_clk_configure_div(uint32_t data)
{
	int div_id = (data & DIV_ID_MASK) >> DIV_ID_SHIFT;
	int div_n = (data & DIV_DIVN_MASK) >> DIV_DIVN_SHIFT;

	return stm32_div_set_value(div_id, div_n);
}

static int stm32_clk_dividers_configure(struct clk_stm32_priv *priv)
{
	struct stm32_clk_platdata *pdata = priv->pdata;
	unsigned int i = 0;

	for (i = 0; i < pdata->nclkdiv; i++) {
		if (stm32_clk_configure_div(pdata->clkdiv[i]))
			return -1;
	}

	return 0;
}

static unsigned long clk_stm32_pll_get_oscillator_rate(int sel)
{
	const int osc[] = { OSC_HSI, OSC_HSE, OSC_CSI };

	assert(sel >= 0 && sel < (int)ARRAY_SIZE(osc));

	return clk_stm32_get_rate_oscillateur(osc[sel]);
}

static int clk_stm32_pll_compute_cfgr1(const struct stm32_clk_pll *pll,
				       struct stm32_pll_vco *vco,
				       uint32_t *value)
{
	int sel = (vco->src & MUX_SEL_MASK) >> MUX_SEL_SHIFT;
	uint32_t divm = vco->div_mn[PLL_CFG_M];
	uint32_t divn = vco->div_mn[PLL_CFG_N];
	unsigned long refclk = 0UL;

	refclk = clk_stm32_pll_get_oscillator_rate(sel) / (divm + 1U);

	if ((refclk < (stm32mp1_pll[pll->plltype].refclk_min * 1000000U)) ||
	    (refclk > (stm32mp1_pll[pll->plltype].refclk_max * 1000000U)))
		return -1;

	*value = 0;

	if (pll->plltype == PLL_800 && refclk >= 8000000U)
		*value = 1U << RCC_PLLNCFGR1_IFRGE_SHIFT;

	*value |= (divn << RCC_PLLNCFGR1_DIVN_SHIFT) & RCC_PLLNCFGR1_DIVN_MASK;
	*value |= (divm << RCC_PLLNCFGR1_DIVM_SHIFT) & RCC_PLLNCFGR1_DIVM_MASK;

	return 0;
}

static uint32_t  clk_stm32_pll_compute_cfgr2(struct stm32_pll_output *out)
{
	uint32_t value = 0;

	value |= (out->output[PLL_CFG_P] << RCC_PLLNCFGR2_DIVP_SHIFT) &
		 RCC_PLLNCFGR2_DIVP_MASK;
	value |= (out->output[PLL_CFG_Q] << RCC_PLLNCFGR2_DIVQ_SHIFT) &
		 RCC_PLLNCFGR2_DIVQ_MASK;
	value |= (out->output[PLL_CFG_R] << RCC_PLLNCFGR2_DIVR_SHIFT) &
		 RCC_PLLNCFGR2_DIVR_MASK;

	return value;
}

/*
 * Check if PLL1 can be configured on the fly.
 * @result (-1) => config on the fly is not possible.
 *         (0)  => config on the fly is possible.
 *         (+1) => same parameters, no need to reconfigure.
 * Return value is 0 if no error.
 */
static int clk_stm32_is_pll_config_on_the_fly(struct clk_stm32_priv *priv,
					      const struct stm32_clk_pll *pll,
					      struct stm32_pll_dt_cfg *pll_conf,
					      int *result)
{
	uintptr_t pll_base = priv->base + pll->reg_pllxcr;
	struct stm32_pll_vco *vco = &pll_conf->vco;
	struct stm32_pll_output *out = &pll_conf->output;
	uint32_t fracr = 0;
	uint32_t value = 0;
	int ret = 0;
	size_t sel = 0;

	ret = clk_stm32_pll_compute_cfgr1(pll, vco, &value);
	if (ret != 0)
		return ret;

	sel = (vco->src & MUX_SEL_MASK) >> MUX_SEL_SHIFT;
	if (sel != stm32_mux_get_parent(pll->mux_id)) {
		/* Clock source of the PLL is different */
		*result = -1;
		return 0;
	}

	if (io_read32(pll_base + RCC_OFFSET_PLLXCFGR1) != value) {
		/* Different DIVN/DIVM, can't config on the fly */
		*result = -1;
		return 0;
	}

	*result = 1;

	fracr = vco->frac << RCC_PLLNFRACR_FRACV_SHIFT;
	fracr |= RCC_PLLNCFGR1_DIVM_MASK;
	value = clk_stm32_pll_compute_cfgr2(out);

	if ((io_read32(pll_base + RCC_OFFSET_PLLXFRACR) == fracr) &&
	    (io_read32(pll_base + RCC_OFFSET_PLLXCFGR2) == value)) {
		/* Same parameters, no need to config */
		*result = 1;
	} else {
		*result = 0;
	}

	return 0;
}

static void clk_stm32_pll_config_vco(struct clk_stm32_priv *priv,
				     const struct stm32_clk_pll *pll,
				     struct stm32_pll_vco *vco)
{
	uintptr_t pll_base = priv->base + pll->reg_pllxcr;
	uint32_t value = 0;

	if (clk_stm32_pll_compute_cfgr1(pll, vco, &value) != 0) {
		EMSG("Invalid Vref clock");
		panic();
	}

	/* Write N / M / IFREGE fields */
	io_write32(pll_base + RCC_OFFSET_PLLXCFGR1, value);

	/* Fractional configuration */
	io_write32(pll_base + RCC_OFFSET_PLLXFRACR, 0);

	/* Frac must be enabled only once its configuration is loaded */
	io_write32(pll_base + RCC_OFFSET_PLLXFRACR,
		   vco->frac << RCC_PLLNFRACR_FRACV_SHIFT);

	io_setbits32(pll_base + RCC_OFFSET_PLLXFRACR, RCC_PLLNFRACR_FRACLE);
}

static void clk_stm32_pll_config_csg(struct clk_stm32_priv *priv,
				     const struct stm32_clk_pll *pll,
				     struct stm32_pll_vco *vco)
{
	uintptr_t pll_base = priv->base + pll->reg_pllxcr;
	uint32_t mod_per = 0;
	uint32_t inc_step = 0;
	uint32_t sscg_mode = 0;
	uint32_t value = 0;

	if (!vco->csg_enabled)
		return;

	mod_per = vco->csg[PLL_CSG_MOD_PER];
	inc_step = vco->csg[PLL_CSG_INC_STEP];
	sscg_mode = vco->csg[PLL_CSG_SSCG_MODE];

	value |= (mod_per << RCC_PLLNCSGR_MOD_PER_SHIFT) &
		 RCC_PLLNCSGR_MOD_PER_MASK;
	value |= (inc_step << RCC_PLLNCSGR_INC_STEP_SHIFT) &
		 RCC_PLLNCSGR_INC_STEP_MASK;
	value |= (sscg_mode << RCC_PLLNCSGR_SSCG_MODE_SHIFT) &
		 RCC_PLLNCSGR_SSCG_MODE_MASK;

	io_write32(pll_base + RCC_OFFSET_PLLXCSGR, value);
	io_setbits32(pll_base + RCC_OFFSET_PLLXCR, RCC_PLLNCR_SSCG_CTRL);
}

static void clk_stm32_pll_config_out(struct clk_stm32_priv *priv,
				     const struct stm32_clk_pll *pll,
				     struct stm32_pll_output *out)
{
	uintptr_t pll_base = priv->base + pll->reg_pllxcr;
	uint32_t value = 0;

	value = clk_stm32_pll_compute_cfgr2(out);

	io_write32(pll_base + RCC_OFFSET_PLLXCFGR2, value);
}

static struct stm32_pll_dt_cfg *clk_stm32_pll_get_pdata(int pll_idx)
{
	struct clk_stm32_priv *priv = clk_stm32_get_priv();
	struct stm32_clk_platdata *pdata = priv->pdata;

	return &pdata->pll[pll_idx];
}

static int clk_stm32_pll_init_switch_to_hsi_clk_system(int mux_sys)
{
	int sel = 0;

	if (mux_sys == -1)
		return -1;

	/* Make a backup to the current parent */
	sel = stm32_mux_get_parent(mux_sys);

	/* Switch to HSI */
	if (stm32_mux_set_parent(mux_sys, 0))
		return -1;

	return sel;
}

static uint32_t
clk_stm32_pll_backup_output_diven(const struct stm32_clk_pll *pll)
{
	struct clk_stm32_priv *priv = clk_stm32_get_priv();
	uintptr_t addr = priv->base + pll->reg_pllxcr;

	return io_read32(addr + RCC_OFFSET_PLLXCR) &
			 (RCC_PLLNCR_DIVPEN | RCC_PLLNCR_DIVQEN |
			  RCC_PLLNCR_DIVREN);
}

static void clk_stm32_pll_restore_output_diven(const struct stm32_clk_pll *pll,
					       uint32_t value)
{
	struct clk_stm32_priv *priv = clk_stm32_get_priv();
	uintptr_t addr = priv->base + pll->reg_pllxcr;
	const uint32_t mask = RCC_PLLNCR_DIVPEN | RCC_PLLNCR_DIVQEN |
			      RCC_PLLNCR_DIVREN;

	io_clrsetbits32(addr, mask, value & mask);
}

static int clk_stm32_pll_init(struct clk_stm32_priv *priv, int pll_idx,
			      struct stm32_pll_dt_cfg *pll_conf)
{
	const struct stm32_clk_pll *pll = clk_stm32_pll_data(pll_idx);
	int config_on_the_fly = -1;
	int ret = 0;
	uint8_t sel = 0;
	uint32_t save_div_pqr_en = 0;
	int mux_system[] = { MUX_MPU, MUX_AXI, MUX_MCU, -1 };
	int mux_sys  = mux_system[pll_idx];

	ret = clk_stm32_is_pll_config_on_the_fly(priv, pll, pll_conf,
						 &config_on_the_fly);
	if (ret != 0)
		return ret;

	/* Backup status of DIV DIVPEN / DIVQEN / DIVREN */
	save_div_pqr_en = clk_stm32_pll_backup_output_diven(pll);

	if (config_on_the_fly == -1) {
		/* Make a backup to the current parent and switch to HSI */
		sel = clk_stm32_pll_init_switch_to_hsi_clk_system(mux_sys);

		/* Stop the PLL before */
		if (stm32_gate_is_enabled(pll->gate_id))  {
			io_clrbits32(priv->base + pll->reg_pllxcr,
				     RCC_PLLNCR_DIVPEN | RCC_PLLNCR_DIVQEN |
				     RCC_PLLNCR_DIVREN);

			if (stm32_gate_rdy_disable(pll->gate_id))
				return -1;
		}

		/* Configure PLLs source */
		ret = stm32_clk_configure_mux(pll_conf->vco.src);
		if (ret)
			return ret;

		clk_stm32_pll_config_vco(priv, pll, &pll_conf->vco);
	}

	if (config_on_the_fly != 1) {
		clk_stm32_pll_config_out(priv, pll, &pll_conf->output);
		clk_stm32_pll_config_csg(priv, pll, &pll_conf->vco);
	}

	if (!stm32_gate_is_enabled(pll->gate_id)) {
		if (stm32_gate_rdy_enable(pll->gate_id))
			return -1;

		clk_stm32_pll_restore_output_diven(pll, save_div_pqr_en);
	}

	if ((config_on_the_fly == -1) && (mux_sys != -1)) {
		/* Restore to backup parent */
		if (stm32_mux_set_parent(mux_sys, sel))
			return -1;
	}

	return 0;
}

static int stm32_clk_pll_configure(struct clk_stm32_priv *priv)
{
	struct stm32_pll_dt_cfg *pll_conf = NULL;
	size_t i = 0;
	const int plls[] = { _PLL1, _PLL3, _PLL4 };

	for (i = 0; i < ARRAY_SIZE(plls); i++) {
		pll_conf = clk_stm32_pll_get_pdata(plls[i]);

		if (pll_conf->vco.status) {
			int err = 0;

			err = clk_stm32_pll_init(priv, plls[i], pll_conf);
			if (err)
				return err;
		}
	}

	return 0;
}

static int stm32mp1_init_clock_tree(struct clk_stm32_priv *priv,
				    struct stm32_clk_platdata *pdata)
{
	int ret = 0;

	/*
	 * Switch ON oscillators found in device-tree.
	 * Note: HSI already ON after BootROM stage.
	 */
	stm32_clk_oscillators_enable(priv, pdata);

	ret = stm32_clk_hsidiv_configure(priv);
	if (ret != 0)
		panic();

	ret = stm32_clk_dividers_configure(priv);
	if (ret != 0)
		panic();

	ret = stm32_clk_pll_configure(priv);
	if (ret != 0)
		panic();

	/* Wait LSE ready before to use it */
	ret = stm32_clk_oscillators_wait_lse_ready(priv, pdata);
	if (ret != 0)
		panic();

	/* Configure with expected clock source */
	ret = stm32_clk_source_configure(priv);
	if (ret != 0)
		panic();

	/* Configure LSE CSS after RTC source configuration */
	ret = stm32_clk_oscillators_lse_set_css(priv, pdata);
	if (ret != 0)
		panic();

	/* Software Self-Refresh mode (SSR) during DDR initilialization */
	io_clrsetbits32(priv->base + RCC_DDRITFCR, RCC_DDRITFCR_DDRCKMOD_MASK,
			RCC_DDRITFCR_DDRCKMOD_SSR <<
			RCC_DDRITFCR_DDRCKMOD_SHIFT);

	return 0;
}

static struct stm32_pll_dt_cfg mp15_pll[_PLL_NB];
static struct stm32_clk_opp_dt_cfg mp15_clk_opp;
static struct stm32_osci_dt_cfg mp15_osci[NB_OSC];
static uint32_t mp15_clksrc[MUX_NB];
static uint32_t mp15_clkdiv[DIV_NB];

static struct stm32_clk_platdata stm32mp15_clock_pdata = {
	.osci		= mp15_osci,
	.nosci		= NB_OSC,
	.pll		= mp15_pll,
	.opp		= &mp15_clk_opp,
	.npll		= _PLL_NB,
	.clksrc		= mp15_clksrc,
	.nclksrc	= MUX_NB,
	.clkdiv		= mp15_clkdiv,
	.nclkdiv	= DIV_NB,
};

static struct clk_stm32_priv stm32mp15_clock_data = {
	.muxes			= parent_mp15,
	.nb_muxes		= ARRAY_SIZE(parent_mp15),
	.gates			= gates_mp15,
	.nb_gates		= ARRAY_SIZE(gates_mp15),
	.div			= dividers_mp15,
	.nb_div			= ARRAY_SIZE(dividers_mp15),
	.pdata			= &stm32mp15_clock_pdata,
};
DECLARE_KEEP_PAGER(stm32mp15_clock_data);

static TEE_Result stm32mp1_clock_provider_probe(const void *fdt, int offs,
						const void *compat_data)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct clk_stm32_priv *priv = &stm32mp15_clock_data;
	struct stm32_clk_platdata *pdata = &stm32mp15_clock_pdata;
	int rc = 0;
	int subnode = 0;

	if (compat_data == &non_secure_rcc)
		disable_rcc_tzen();
	else
		enable_rcc_tzen();

	rc = fdt_stm32_clk_parse(fdt, offs, pdata);
	if (rc) {
		EMSG("Failed to parse clock node: %d", rc);
		return TEE_ERROR_GENERIC;
	}

	fdt_for_each_subnode(subnode, fdt, offs) {
		res = dt_driver_maybe_add_probe_node(fdt, subnode);
		if (res) {
			EMSG("Failed on node %s with %#"PRIx32,
			     fdt_get_name(fdt, subnode, NULL), res);
			return res;
		}
	}

	res = clk_stm32_init(priv, stm32_rcc_base());
	if (res)
		return res;

#ifdef CFG_STM32_CLK_DEBUG
	clk_stm32_debug_display_pdata();
#endif
	rc = stm32mp1_init_clock_tree(priv, pdata);
	if (rc)
		return TEE_ERROR_GENERIC;

	res = register_stm32mp1_clocks();
	if (res) {
		EMSG("Failed to register clocks: %#"PRIx32, res);
		panic();
	}

	res = clk_dt_register_clk_provider(fdt, offs, stm32mp1_clk_dt_get_clk,
					   NULL);
	if (res) {
		EMSG("Failed to register clock provider: %#"PRIx32, res);
		panic();
	}

	enable_static_secure_clocks();
	save_current_opp();

	register_pm_core_service_cb(stm32_clock_pm, NULL,
				    "stm32mp15-clk-service");

	return TEE_SUCCESS;
}

static const struct dt_device_match stm32mp1_clock_match_table[] = {
	{  .compatible = "st,stm32mp1-rcc", .compat_data = &non_secure_rcc, },
	{  .compatible = "st,stm32mp1-rcc-secure", },
	{ }
};

DEFINE_DT_DRIVER(stm32mp1_clock_dt_driver) = {
	.name = "stm32mp1_clock",
	.type = DT_DRIVER_CLK,
	.match_table = stm32mp1_clock_match_table,
	.probe = stm32mp1_clock_provider_probe,
};
#else /*CFG_DRIVERS_CLK_DT*/
static void stm32_clock_enable(unsigned long clock_id)
{
	int index = clock_id_to_gate_index(clock_id);

	if (index >= 0) {
		__clk_enable(stm32mp1_clk_gate + index);

		/* Make sure the clock is enable before to return to caller */
		dsb();
	}

	assert(clock_id_to_always_on_index(clock_id) >= 0);
}

static void enable_static_secure_clocks(void)
{
	unsigned int idx = 0;
	const unsigned long secure_enable[] = {
		DDRC1, DDRC1LP, DDRC2, DDRC2LP, DDRPHYC, DDRPHYCLP, DDRCAPB,
		AXIDCG, DDRPHYCAPB, DDRPHYCAPBLP, TZPC, TZC1, TZC2, STGEN_K,
		BSEC,
	};
	for (idx = 0; idx < ARRAY_SIZE(secure_enable); idx++) {
		stm32_clock_enable(secure_enable[idx]);
		stm32mp_register_clock_parents_secure(secure_enable[idx]);
	}

	if (CFG_TEE_CORE_NB_CORE > 1)
		stm32_clock_enable(RTCAPB);
}

int stm32mp1_clk_compute_all_pll1_settings(uint32_t buck1_voltage __unused)
{
	return 0;
}

void stm32mp1_clk_lp_save_opp_pll1_settings(uint8_t *data __unused,
					    size_t size __unused)
{
}

bool stm32mp1_clk_pll1_settings_are_valid(void)
{
	return false;
}

static void enable_static_secure_clocks(void)
{
}

static TEE_Result stm32mp1_clk_early_init(void)
{
	TEE_Result __maybe_unused res = TEE_ERROR_GENERIC;

	res = register_stm32mp1_clocks();
	if (res) {
		EMSG("Failed to register clocks: %#"PRIx32, res);
		panic();
	}

	enable_static_secure_clocks();
	save_current_opp();

	return TEE_SUCCESS;
}

service_init(stm32mp1_clk_early_init);
#endif /* CFG_DRIVERS_CLK_DT */

#endif /* CFG_DRIVERS_CLK */

/* Start MPU OPP */

/*
 * Check if PLL1 can be configured on the fly.
 * @result  (-1) => config on the fly is not possible.
 *          (0)  => config on the fly is possible.
 *          (+1) => same parameters as those in place, no need to reconfig.
 * Return value is 0 if no error.
 */
static int is_pll_config_on_the_fly(enum stm32mp1_pll_id pll_id,
				    uint32_t *pllcfg, uint32_t fracv,
				    int *result)
{
	const struct stm32mp1_clk_pll *pll = pll_ref(pll_id);
	uintptr_t rcc_base = stm32_rcc_base();
	uint32_t fracr = 0;
	uint32_t value = 0;
	int ret = 0;

	ret = pll_compute_pllxcfgr1(pll, pllcfg, &value);
	if (ret)
		return ret;

	if (io_read32(rcc_base + pll->pllxcfgr1) != value) {
		/* Different DIVN/DIVM, can't config on the fly */
		*result = -1;
		return 0;
	}

	*result = true;

	fracr = fracv << RCC_PLLNFRACR_FRACV_SHIFT;
	fracr |= RCC_PLLNFRACR_FRACLE;
	value = pll_compute_pllxcfgr2(pllcfg);

	if ((io_read32(rcc_base + pll->pllxfracr) == fracr) &&
	    (io_read32(rcc_base + pll->pllxcfgr2) == value))
		/* Same parameters, no need to config */
		*result = 1;
	else
		*result = 0;

	return 0;
}

static int stm32mp1_get_mpu_div(uint32_t freq_khz)
{
	unsigned long freq_pll1_p;
	unsigned long div;

	freq_pll1_p = get_clock_rate(_PLL1_P) / 1000UL;
	if ((freq_pll1_p % freq_khz) != 0U)
		return -1;

	div = freq_pll1_p / freq_khz;

	switch (div) {
	case 1UL:
	case 2UL:
	case 4UL:
	case 8UL:
	case 16UL:
		return __builtin_ffs(div) - 1;
	default:
		return -1;
	}
}

/* Configure PLL1 from input frequency OPP parameters */
static int pll1_config_from_opp_khz(uint32_t freq_khz)
{
	unsigned int idx = 0;
	int ret = 0;
	int div = 0;
	int config_on_the_fly = -1;

	for (idx = 0; idx < MAX_OPP; idx++)
		if (pll1_settings.freq[idx] == freq_khz)
			break;

	if (idx == MAX_OPP)
		return -1;

	div = stm32mp1_get_mpu_div(freq_khz);
	switch (div) {
	case -1:
		break;
	case 0:
		return stm32_clk_configure_mux(CLK_MPU_PLL1P);
	default:
		if (!stm32_div_set_value(DIV_MPU, div))
			return stm32_clk_configure_mux(CLK_MPU_PLL1P_DIV);

		return -1;
	}

	ret = is_pll_config_on_the_fly(_PLL1, &pll1_settings.cfg[idx][0],
				       pll1_settings.frac[idx],
				       &config_on_the_fly);
	if (ret)
		return ret;

	if (config_on_the_fly == 1)
		return 0;

	if (config_on_the_fly == -1) {
		/* Switch to HSI and stop PLL1 before reconfiguration */
		ret = stm32_clk_configure_mux(CLK_MPU_HSI);
		if (ret)
			return ret;

		ret = pll_stop(_PLL1);
		if (ret)
			return ret;
	}

	ret = pll_config(_PLL1, &pll1_settings.cfg[idx][0],
			 pll1_settings.frac[idx]);
	if (ret)
		return ret;

	if (config_on_the_fly == -1) {
		/* Start PLL1 and switch back to after reconfiguration */
		pll_start(_PLL1);

		ret = pll_output(_PLL1, pll1_settings.cfg[idx][PLLCFG_O]);
		if (ret)
			return ret;

		ret = stm32_clk_configure_mux(CLK_MPU_PLL1P);
		if (ret)
			return ret;
	}

	return 0;
}

TEE_Result stm32mp1_set_opp_khz(uint32_t freq_khz)
{
	uint32_t mpu_src = 0;

	if (freq_khz == current_opp_khz)
		return TEE_SUCCESS;

	if (!stm32mp1_clk_pll1_settings_are_valid()) {
		/*
		 * No OPP table in DT or an error occurred during PLL1
		 * settings computation, system can only work on current
		 * operating point so return error.
		 */
		return TEE_ERROR_NO_DATA;
	}

	/* Check that PLL1 is MPU clock source */
	mpu_src = io_read32(stm32_rcc_base() + RCC_MPCKSELR) &
		RCC_SELR_SRC_MASK;
	if ((mpu_src != RCC_MPCKSELR_PLL) &&
	    (mpu_src != RCC_MPCKSELR_PLL_MPUDIV))
		return TEE_ERROR_BAD_STATE;

	if (pll1_config_from_opp_khz(freq_khz)) {
		/* Restore original value */
		if (pll1_config_from_opp_khz(current_opp_khz)) {
			EMSG("No CPU operating point can be set");
			panic();
		}

		return TEE_ERROR_GENERIC;
	}

	current_opp_khz = freq_khz;

	return TEE_SUCCESS;
}

int stm32mp1_round_opp_khz(uint32_t *freq_khz)
{
	unsigned int i = 0;
	uint32_t round_opp = 0;

	if (!stm32mp1_clk_pll1_settings_are_valid()) {
		/*
		 * No OPP table in DT, or an error occurred during PLL1
		 * settings computation, system can only work on current
		 * operating point, so return current CPU frequency.
		 */
		*freq_khz = current_opp_khz;

		return 0;
	}

	for (i = 0; i < MAX_OPP; i++)
		if ((pll1_settings.freq[i] <= *freq_khz) &&
		    (pll1_settings.freq[i] > round_opp))
			round_opp = pll1_settings.freq[i];

	*freq_khz = round_opp;

	return 0;
}
/* End MPU OPP */

struct soc_stop_context {
	uint32_t pll3cr;
	uint32_t pll4cr;
	uint32_t mssckselr;
	uint32_t mcudivr;
};

static struct soc_stop_context soc_stop_ctx;

static void save_pll34_state(void)
{
	uintptr_t rcc_base = stm32_rcc_base();
	struct soc_stop_context *ctx = &soc_stop_ctx;

	ctx->pll3cr = io_read32(rcc_base + RCC_PLL3CR);
	ctx->pll4cr = io_read32(rcc_base + RCC_PLL4CR);
}

static void save_mcu_subsys_clocks(void)
{
	uintptr_t rcc_base = stm32_rcc_base();
	struct soc_stop_context *ctx = &soc_stop_ctx;

	ctx->mssckselr = io_read32(rcc_base + RCC_MSSCKSELR);
	ctx->mcudivr = io_read32(rcc_base + RCC_MCUDIVR) &
		       RCC_MCUDIV_MASK;
}

static void restore_pll34_state(void)
{
	struct soc_stop_context *ctx = &soc_stop_ctx;

	/* Let PLL4 start while we're starting and waiting for PLL3 */
	if (ctx->pll4cr & RCC_PLLNCR_PLLON)
		pll_start(_PLL4);

	if (ctx->pll3cr & RCC_PLLNCR_PLLON) {
		pll_start(_PLL3);
		if (pll_output(_PLL3, ctx->pll3cr >> RCC_PLLNCR_DIVEN_SHIFT)) {
			EMSG("Failed to restore PLL3");
			panic();
		}
	}

	if (ctx->pll4cr & RCC_PLLNCR_PLLON) {
		if (pll_output(_PLL4, ctx->pll4cr >> RCC_PLLNCR_DIVEN_SHIFT)) {
			EMSG("Failed to restore PLL4");
			panic();
		}
	}
}

static void restore_mcu_subsys_clocks(void)
{
	uintptr_t rcc_base = stm32_rcc_base();
	struct soc_stop_context *ctx = &soc_stop_ctx;

	io_write32(rcc_base + RCC_MSSCKSELR, ctx->mssckselr);

	if (stm32_div_set_value(DIV_MCU, ctx->mcudivr)) {
		EMSG("Failed to restore MCUDIVR");
		panic();
	}
}

/*
 * Sequence to save/restore the non-secure configuration.
 * Restoring clocks and muxes need IPs to run on kernel clock
 * hence on configuration is restored at resume, kernel clock
 * should be disable: this mandates secure access.
 *
 * backup_mux*_cfg for the clock muxes.
 * backup_clock_sc_cfg for the set/clear clock gating registers
 * backup_clock_cfg for the regular full write registers
 */

struct backup_mux_cfg {
	uint16_t offset;
	uint8_t value;
	uint8_t bit_len;
};

#define MUXCFG(_offset, _bit_len) \
	{ .offset = (_offset), .bit_len = (_bit_len) }

struct backup_mux_cfg backup_mux0_cfg[] = {
	MUXCFG(RCC_SDMMC12CKSELR, 3),
	MUXCFG(RCC_SPI2S23CKSELR, 3),
	MUXCFG(RCC_SPI45CKSELR, 3),
	MUXCFG(RCC_I2C12CKSELR, 3),
	MUXCFG(RCC_I2C35CKSELR, 3),
	MUXCFG(RCC_LPTIM23CKSELR, 3),
	MUXCFG(RCC_LPTIM45CKSELR, 3),
	MUXCFG(RCC_UART24CKSELR, 3),
	MUXCFG(RCC_UART35CKSELR, 3),
	MUXCFG(RCC_UART78CKSELR, 3),
	MUXCFG(RCC_SAI1CKSELR, 3),
	MUXCFG(RCC_ETHCKSELR, 2),
	MUXCFG(RCC_I2C46CKSELR, 3),
	MUXCFG(RCC_RNG2CKSELR, 2),
	MUXCFG(RCC_SDMMC3CKSELR, 3),
	MUXCFG(RCC_FMCCKSELR, 2),
	MUXCFG(RCC_QSPICKSELR, 2),
	MUXCFG(RCC_USBCKSELR, 2),
	MUXCFG(RCC_SPDIFCKSELR, 2),
	MUXCFG(RCC_SPI2S1CKSELR, 3),
	MUXCFG(RCC_CECCKSELR, 2),
	MUXCFG(RCC_LPTIM1CKSELR, 3),
	MUXCFG(RCC_UART6CKSELR, 3),
	MUXCFG(RCC_FDCANCKSELR, 2),
	MUXCFG(RCC_SAI2CKSELR, 3),
	MUXCFG(RCC_SAI3CKSELR,  3),
	MUXCFG(RCC_SAI4CKSELR, 3),
	MUXCFG(RCC_ADCCKSELR, 2),
	MUXCFG(RCC_DSICKSELR, 1),
	MUXCFG(RCC_CPERCKSELR, 2),
	MUXCFG(RCC_RNG1CKSELR, 2),
	MUXCFG(RCC_STGENCKSELR, 2),
	MUXCFG(RCC_UART1CKSELR, 3),
	MUXCFG(RCC_SPI6CKSELR, 3),
};

struct backup_mux_cfg backup_mux4_cfg[] = {
	MUXCFG(RCC_USBCKSELR, 1),
};

static void backup_mux_cfg(void)
{
	struct backup_mux_cfg *cfg = backup_mux0_cfg;
	size_t count = ARRAY_SIZE(backup_mux0_cfg);
	size_t i = 0;
	uintptr_t base = stm32_rcc_base();

	for (i = 0; i < count; i++)
		cfg[i].value = io_read32(base + cfg[i].offset) &
				GENMASK_32(cfg[i].bit_len - 1, 0);

	cfg = backup_mux4_cfg;
	count = ARRAY_SIZE(backup_mux4_cfg);

	for (i = 0; i < count; i++)
		cfg[i].value = io_read32(base + cfg[i].offset) &
				GENMASK_32(4 + cfg[i].bit_len - 1, 4);
}

static void restore_mux_cfg(void)
{
	struct backup_mux_cfg *cfg = backup_mux0_cfg;
	size_t count = ARRAY_SIZE(backup_mux0_cfg);
	size_t i = 0;
	uintptr_t base = stm32_rcc_base();

	for (i = 0; i < count; i++)
		io_clrsetbits32(base + cfg[i].offset,
				GENMASK_32(cfg[i].bit_len - 1, 0),
				cfg[i].value);

	cfg = backup_mux4_cfg;
	count = ARRAY_SIZE(backup_mux4_cfg);

	for (i = 0; i < count; i++)
		io_clrsetbits32(base + cfg[i].offset,
				GENMASK_32(4 + cfg[i].bit_len - 1, 4),
				cfg[i].value);
}

/* Structure is used for set/clear registers and for regular registers */
struct backup_clock_cfg {
	uint32_t offset;
	uint32_t value;
};

static struct backup_clock_cfg backup_clock_sc_cfg[] = {
	{ .offset = RCC_MP_APB1ENSETR },
	{ .offset = RCC_MP_APB2ENSETR },
	{ .offset = RCC_MP_APB3ENSETR },
	{ .offset = RCC_MP_APB4ENSETR },
	{ .offset = RCC_MP_APB5ENSETR },
	{ .offset = RCC_MP_AHB2ENSETR },
	{ .offset = RCC_MP_AHB3ENSETR },
	{ .offset = RCC_MP_AHB4ENSETR },
	{ .offset = RCC_MP_AHB5ENSETR },
	{ .offset = RCC_MP_AHB6ENSETR },
	{ .offset = RCC_MP_MLAHBENSETR },
};

static struct backup_clock_cfg backup_clock_cfg[] = {
	{ .offset = RCC_TZCR},
	{ .offset = RCC_MCO1CFGR },
	{ .offset = RCC_MCO2CFGR },
	{ .offset = RCC_PLL3CR },
	{ .offset = RCC_PLL4CR },
	{ .offset = RCC_PLL4CFGR2 },
	{ .offset = RCC_MCUDIVR },
	{ .offset = RCC_MSSCKSELR },
};

static void backup_sc_cfg(void)
{
	struct backup_clock_cfg *cfg = backup_clock_sc_cfg;
	size_t count = ARRAY_SIZE(backup_clock_sc_cfg);
	size_t i = 0;
	uintptr_t base = stm32_rcc_base();

	for (i = 0; i < count; i++)
		cfg[i].value = io_read32(base + cfg[i].offset);
}

static void restore_sc_cfg(void)
{
	struct backup_clock_cfg *cfg = backup_clock_sc_cfg;
	size_t count = ARRAY_SIZE(backup_clock_sc_cfg);
	size_t i = 0;
	uintptr_t base = stm32_rcc_base();

	for (i = 0; i < count; i++) {
		io_write32(base + cfg[i].offset, cfg[i].value);
		io_write32(base + cfg[i].offset + RCC_MP_ENCLRR_OFFSET,
			   ~cfg[i].value);
	}
}

static void backup_regular_cfg(void)
{
	struct backup_clock_cfg *cfg = backup_clock_cfg;
	size_t count = ARRAY_SIZE(backup_clock_cfg);
	size_t i = 0;
	uintptr_t base = stm32_rcc_base();

	for (i = 0; i < count; i++)
		cfg[i].value = io_read32(base + cfg[i].offset);
}

static void restore_regular_cfg(void)
{
	struct backup_clock_cfg *cfg = backup_clock_cfg;
	size_t count = ARRAY_SIZE(backup_clock_cfg);
	size_t i = 0;
	uintptr_t base = stm32_rcc_base();

	for (i = 0; i < count; i++)
		io_write32(base + cfg[i].offset, cfg[i].value);
}

static void disable_kernel_clocks(void)
{
	const uint32_t ker_mask = RCC_OCENR_HSIKERON |
				  RCC_OCENR_CSIKERON |
				  RCC_OCENR_HSEKERON;

	/* Disable all ck_xxx_ker clocks */
	io_write32(stm32_rcc_base() + RCC_OCENCLRR, ker_mask);
}

static void enable_kernel_clocks(void)
{
	uintptr_t rcc_base = stm32_rcc_base();
	uint32_t reg = 0;
	const uint32_t ker_mask = RCC_OCENR_HSIKERON |
				  RCC_OCENR_CSIKERON |
				  RCC_OCENR_HSEKERON;

	/* Enable ck_xxx_ker clocks if ck_xxx was on */
	reg = io_read32(rcc_base + RCC_OCENSETR) << 1;
	io_write32(rcc_base + RCC_OCENSETR, reg & ker_mask);
}

static void clear_rcc_reset_status(void)
{
	/* Clear reset status fields */
	io_write32(stm32_rcc_base() + RCC_MP_RSTSCLRR, 0);
}

void stm32mp1_clk_save_context_for_stop(void)
{
	enable_kernel_clocks();
	save_mcu_subsys_clocks();
	save_pll34_state();
}

void stm32mp1_clk_restore_context_for_stop(void)
{
	restore_pll34_state();
	/* Restore MCU clock source after PLL3 is ready */
	restore_mcu_subsys_clocks();
	disable_kernel_clocks();
}

static bool backup_clock_nsec[NB_GATES];

static void sync_non_secure_clk_state(void)
{
	unsigned int idx = 0;

	for (idx = 0; idx < NB_GATES; idx++) {
		struct stm32mp1_clk_gate const *gate = gate_ref(idx);
		struct clk *clk = NULL;

		clk = clock_id_to_clk(gate->clock_id);
		assert(clk);

		if (__clk_is_enabled(gate) && !clk_is_enabled(clk))
			backup_clock_nsec[idx] = true;
		else
			backup_clock_nsec[idx] = false;
	}
}

static void stm32_clock_suspend(void)
{
	sync_non_secure_clk_state();
	backup_regular_cfg();
	backup_sc_cfg();
	backup_mux_cfg();
	save_pll34_state();

	enable_kernel_clocks();
	clear_rcc_reset_status();
}

static void stm32_clock_resume(void)
{
	unsigned int idx = 0;

	restore_pll34_state();
	restore_mux_cfg();
	restore_sc_cfg();
	restore_regular_cfg();

	/* Sync secure and shared clocks physical state on functional state */
	for (idx = 0; idx < NB_GATES; idx++) {
		struct stm32mp1_clk_gate const *gate = gate_ref(idx);
		struct clk *clk = NULL;

		if (backup_clock_nsec[idx])
			continue;

		clk = clock_id_to_clk(gate->clock_id);
		assert(clk);

		if (clk_is_enabled(clk)) {
			DMSG("Force clock %d enable", gate->clock_id);
			__clk_enable(gate);
		} else {
			DMSG("Force clock %d disable", gate->clock_id);
			__clk_disable(gate);
		}
	}

	disable_kernel_clocks();
}

static TEE_Result stm32_clock_pm(enum pm_op op, unsigned int pm_hint __unused,
				 const struct pm_callback_handle *hdl __unused)
{
	if (op == PM_OP_SUSPEND) {
		/* Make sure the pending operations are visible */
		dsb();
		stm32_clock_suspend();
	} else {
		pll1_config_from_opp_khz(current_opp_khz);
		stm32_clock_resume();
		/* Make sure the clock are restored */
		dsb();
	}

	return TEE_SUCCESS;
}
DECLARE_KEEP_PAGER(stm32_clock_pm);


void stm32_reset_system(void)
{
	vaddr_t rcc = stm32_rcc_base();

	/* Make sure the pending operations are visible */
	dsb();

	io_write32(rcc + RCC_MP_GRSTCSETR, RCC_MP_GRSTCSETR_MPSYSRST);
}
