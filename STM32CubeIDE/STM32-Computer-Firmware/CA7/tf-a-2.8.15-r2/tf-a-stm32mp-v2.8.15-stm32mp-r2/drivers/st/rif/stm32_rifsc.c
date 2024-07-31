/*
 * Copyright (c) 2023, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdint.h>

#include <arch_helpers.h>
#include <drivers/st/stm32_rifsc.h>
#include <dt-bindings/soc/rif.h>
#include <lib/mmio.h>

#include <platform_def.h>

/* RIFSC general register field description */
/* RIFSC_CIDCFGR register fields */
#define _RIFSC_RISC_CIDCFGR(x)		(U(0x100) + U(0x8) * (x))
#define _RIFSC_RISC_CFEN_MASK		BIT(0)
#define _RIFSC_RISC_SEM_EN_MASK		BIT(1)
#define _RIFSC_RISC_SEML_SHIFT		U(16)
#define _RIFSC_RISC_SEML_MASK		GENMASK_32(23, 16)

/* RIFSC_SEMCR register fields */
#define _RIFSC_RISC_SEMCR(x)		(U(0x104) + U(0x8) * (x))
#define _RIFSC_RISC_SEM_MUTEX		BIT(0)
#define _RIFSC_RISC_SEMCID_SHIFT	U(4)
#define _RIFSC_RISC_SEMCID_MASK		GENMASK_32(6, 4)

static unsigned long rifsc_semaphores[] = {
	STM32MP25_RIFSC_RNG_ID,
	STM32MP25_RIFSC_PKA_ID,
	STM32MP25_RIFSC_SAES_ID,
};

int stm32_rifsc_semaphore_init(void)
{
	unsigned long i = 0;

	for (i = 0; i < ARRAY_SIZE(rifsc_semaphores); i++) {
		uint32_t cidcfgr = mmio_read_32(RIFSC_BASE +
						_RIFSC_RISC_CIDCFGR(rifsc_semaphores[i]));
		uint32_t semcfgr = mmio_read_32(RIFSC_BASE +
						_RIFSC_RISC_SEMCR(rifsc_semaphores[i]));

		if (!(((cidcfgr & _RIFSC_RISC_CFEN_MASK) != 0U) &&
		      ((cidcfgr & _RIFSC_RISC_SEM_EN_MASK) != 0U) &&
		      ((cidcfgr & _RIFSC_RISC_SEML_MASK) >> _RIFSC_RISC_SEML_SHIFT) != RIF_CID1)) {
			continue;
		}

		if (((semcfgr & _RIFSC_RISC_SEM_MUTEX) != 0U) &&
		    ((semcfgr & _RIFSC_RISC_SEMCID_MASK) >> _RIFSC_RISC_SEMCID_SHIFT) != RIF_CID1) {
			return -EACCES;
		}

		mmio_write_32(RIFSC_BASE + _RIFSC_RISC_SEMCR(rifsc_semaphores[i]),
			      _RIFSC_RISC_SEM_MUTEX);

		if (((semcfgr & _RIFSC_RISC_SEM_MUTEX) != 0U) &&
		    ((semcfgr & _RIFSC_RISC_SEMCID_MASK) >> _RIFSC_RISC_SEMCID_SHIFT) != RIF_CID1) {
			return -EACCES;
		}
	}

	return 0;
}
