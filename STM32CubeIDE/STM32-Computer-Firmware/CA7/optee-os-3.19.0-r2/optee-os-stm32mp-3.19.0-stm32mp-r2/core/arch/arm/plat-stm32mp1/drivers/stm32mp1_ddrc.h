/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2017-2021, STMicroelectronics - All Rights Reserved
 */

#ifndef __STM32MP1_DDRC_H__
#define __STM32MP1_DDRC_H__

#include <util.h>

/*  DDR Self Refresh (SR) modes */
enum stm32mp1_ddr_sr_mode {
	DDR_SR_MODE_INVALID = 0,
	DDR_SSR_MODE,
	DDR_HSR_MODE,
	DDR_ASR_MODE,
};

void ddr_save_sr_mode(enum stm32mp1_ddr_sr_mode mode);
void ddr_restore_sr_mode(void);

/* Return 32bit calibration value used for DDRPHY */
uint32_t get_ddrphy_calibration(void);

/*
 * Entry/exit DDR selfrefresh mode
 * Return 0 on success and a non-null value on error
 */
int ddr_standby_sr_entry(void);
int ddr_standby_sr_exit(void);

#endif /*__STM32MP1_DDRC_H__*/
