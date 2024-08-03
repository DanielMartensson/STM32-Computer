/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2017-2018, ARM Limited and Contributors. All rights reserved.
 */

#ifndef __STM32MP_PM_POWER_H__
#define __STM32MP_PM_POWER_H__

#include <compiler.h>
#include <stdint.h>
#include <stddef.h>

const char *plat_get_lp_mode_name(int mode);

bool need_to_backup_cpu_context(unsigned int soc_mode);
bool need_to_backup_stop_context(unsigned int soc_mode);

void stm32_enter_csleep(void);

void stm32_enter_cstop(uint32_t mode);
void stm32_exit_cstop(void);

void stm32_enter_cstop_shutdown(uint32_t mode) __noreturn;
void stm32_enter_cstop_reset(uint32_t mode) __noreturn;

void stm32_pm_cpu_power_down_wfi(void) __noreturn;
void stm32_pm_cpu_wfi(void);

#endif /*__STM32MP_PM_POWER_H__*/
