/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014-2024, STMicroelectronics International N.V.
 */
#ifndef __RNG_SUPPORT_H__
#define __RNG_SUPPORT_H__

#include <crypto/crypto.h>
#include <stdint.h>

/* Get random bytes from a hardware entropy source */
TEE_Result hw_get_random_bytes(void *buf, size_t blen);

/* Register RNG support, especially to the HWRNG PTA */
static inline void hw_register_get_random_bytes(void)
{
	crypto_rng_register_hw();
}
#endif /* __RNG_SUPPORT_H__ */
