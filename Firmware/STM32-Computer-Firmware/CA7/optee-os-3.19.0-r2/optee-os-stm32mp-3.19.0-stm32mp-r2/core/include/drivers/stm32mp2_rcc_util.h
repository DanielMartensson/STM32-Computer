/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) STMicroelectronics 2023 - All Rights Reserved
 */

#ifndef __DRIVERS_STM32MP2_RCC_UTIL_H__
#define __DRIVERS_STM32MP2_RCC_UTIL_H__

#include <stdint.h>
#include <tee_api_types.h>
#include <types_ext.h>

/* Platform util for the RCC drivers */
vaddr_t stm32_rcc_base(void);

/* Helper for platform RCC clock driver */
struct clk *stm32mp_rcc_clock_id_to_clk(unsigned long clock_id);

/* Return rstctrl instance related to RCC reset controller DT binding ID */
struct rstctrl *stm32mp_rcc_reset_id_to_rstctrl(unsigned int binding_id);

unsigned long clk_stm32_clock_frequency_calculator(uint32_t ckintsel);
unsigned long clk_stm32_read_hsi_frequency(void);
unsigned long clk_stm32_read_msi_frequency(void);

TEE_Result clk_stm32_init_calib(const void *fdt, int node);

#endif /*__DRIVERS_STM32MP2_RCC_UTIL_H__*/
