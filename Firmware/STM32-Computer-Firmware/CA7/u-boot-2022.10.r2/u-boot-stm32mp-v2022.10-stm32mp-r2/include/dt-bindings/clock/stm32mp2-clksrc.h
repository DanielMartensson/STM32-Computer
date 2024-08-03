/* SPDX-License-Identifier: GPL-2.0-or-later OR BSD-3-Clause */
/*
 * Copyright (C) 2017-2019, STMicroelectronics - All Rights Reserved
 */

#ifndef _DT_BINDINGS_CLOCK_STM32MP2_CLKSRC_H_
#define _DT_BINDINGS_CLOCK_STM32MP2_CLKSRC_H_

/* PLLs source clocks */
#define PLL_SRC_HSI		0x0
#define PLL_SRC_HSE		0x1
#define PLL_SRC_CSI		0x2
#define PLL_SRC_DISABLED	0x3

/*
 * Configure a PLL with its clock source
 * pll_nb: PLL number from 1 to 8
 * pll_src: one of the 3 previous PLLs source clocks defines
 */
#define PLL_CFG(pll_nb, pll_src) \
	(((pll_nb) - 1) | (pll_src << 4))

/* XBAR source clocks */
#define XBAR_SRC_PLL4		0x0
#define XBAR_SRC_PLL5		0x1
#define XBAR_SRC_PLL6		0x2
#define XBAR_SRC_PLL7		0x3
#define XBAR_SRC_PLL8		0x4
#define XBAR_SRC_HSI		0x5
#define XBAR_SRC_HSE		0x6
#define XBAR_SRC_CSI		0x7
#define XBAR_SRC_HSI_KER	0x8
#define XBAR_SRC_HSE_KER	0x9
#define XBAR_SRC_CSI_KER	0xA
#define XBAR_SRC_SPDIF_SYMB	0xB
#define XBAR_SRC_I2S		0xC
#define XBAR_SRC_LSI		0xD
#define XBAR_SRC_LSE		0xE
#define XBAR_SRC_DISABLED	0xF

/*
 * Configure a XBAR channel with its clock source
 * channel_nb: XBAR channel number from 0 to 63
 * channel_src: one of the 15 previous XBAR source clocks defines
 * channel_prediv: value of the PREDIV in channel RCC_PREDIVxCFGR register
 *		   can be either 1, 2, 4 or 1024
 * channel_findiv: value of the FINDIV in channel RCC_FINDIVxCFGR register
 *		   from 1 to 64
 */
#define XBAR_CFG(channel_nb, channel_src, channel_prediv, channel_findiv) \
	((channel_nb) | ((channel_src) << 6) |\
	 ((channel_prediv) << 10) | (((channel_findiv) - 1) << 20))

/* st,clksrc: mandatory clock source */

#define CLK_CA35SS_EXT2F	0x0
#define CLK_CA35SS_PLL1		0x1

#define CLK_RTC_DISABLED	0x0
#define CLK_RTC_LSE		0x1
#define CLK_RTC_LSI		0x2
#define CLK_RTC_HSE		0x3

#define CLK_MCO1_HSI		0x00008000
#define CLK_MCO1_HSE		0x00008001
#define CLK_MCO1_CSI		0x00008002
#define CLK_MCO1_LSI		0x00008003
#define CLK_MCO1_LSE		0x00008004
#define CLK_MCO1_DISABLED	0x0000800F

#define CLK_MCO2_MPU		0x00008040
#define CLK_MCO2_AXI		0x00008041
#define CLK_MCO2_MCU		0x00008042
#define CLK_MCO2_PLL4P		0x00008043
#define CLK_MCO2_HSE		0x00008044
#define CLK_MCO2_HSI		0x00008045
#define CLK_MCO2_DISABLED	0x0000804F

/* define for st,pll /csg */
#define SSCG_MODE_CENTER_SPREAD	0
#define SSCG_MODE_DOWN_SPREAD	1

/* define for st,drive */
#define LSEDRV_LOWEST		0
#define LSEDRV_MEDIUM_LOW	1
#define LSEDRV_MEDIUM_HIGH	2
#define LSEDRV_HIGHEST		3

#endif /* _DT_BINDINGS_CLOCK_STM32MP2_CLKSRC_H_ */
