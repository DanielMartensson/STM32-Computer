/*
 * Copyright (c) 2021, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STM32MP2_RISAF_H
#define STM32MP2_RISAF_H

int stm32mp2_risaf_write_master_key(int instance, uint8_t *mkey);
int stm32mp2_risaf_lock(int instance);
int stm32mp2_risaf_is_locked(int instance, bool *state);
int stm32mp2_risaf_init(void);

#endif /* STM32MP2_RISAF_H */
