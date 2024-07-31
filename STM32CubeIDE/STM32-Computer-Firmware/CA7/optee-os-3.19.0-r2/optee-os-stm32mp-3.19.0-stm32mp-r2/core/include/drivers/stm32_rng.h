/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2024, STMicroelectronics
 */

#ifndef __DRIVERS_STM32_RNG_H
#define __DRIVERS_STM32_RNG_H

#include <stdint.h>
#include <stddef.h>
#include <tee_api_types.h>

/* Initialize STM32 RNG peripheral */
TEE_Result stm32_rng_init(void);

/* Read @size random byte to @out from STM32 RNG peripheral */
TEE_Result stm32_rng_read(uint8_t *out, size_t size);

#endif /* __DRIVERS_STM32_RNG_H */
