/*
 * Copyright (C) 2022, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STM32MP_RIFSC_REGS_H
#define STM32MP_RIFSC_REGS_H

/* RIFSC offset register */
#define _RIFSC_RISC_CR			U(0x0)
#define _RIFSC_RISC_SECCFGR(id)		(U(0x10) + U(0x4) * ((id) / 32))
#define _RIFSC_RISC_PRIVCFGR(id)	(U(0x30) + U(0x4) * ((id) / 32))
#define _RIFSC_RISC_RCFGLOCKR(id)	(U(0x50) + U(0x4) * ((id) / 32))
#define _RIFSC_RISC_PERy_CIDCFGR(id)	(U(0x100) + U(0x8) * (id))
#define _RIFSC_RISC_PERy_SEMCR(id)	(U(0x104) + U(0x8) * (id))
#define _RIFSC_RIMC_CR			U(0xC00)
#define _RIFSC_RIMC_SR			U(0xC04)
#define _RIFSC_RIMC_ATTR(x)		(U(0xC10) + U(0x4) * (x))
#define _RIFSC_PPSR(x)			(U(0xFB0) + U(0x4) * (x))
#define _RIFSC_HWCFGR3			U(0xFE8)
#define _RIFSC_HWCFGR2			U(0xFEC)
#define _RIFSC_HWCFGR1			U(0xFF0)
#define _RIFSC_VERR			U(0xFF4)
#define _RFISC_IPIDR			U(0xFF8)
#define _RFISC_SIDR			U(0xFFC)

/* RIFSC_RIMC_ATTRx register fields */
#define RIFSC_RIMC_ATTRx_CIDSEL			BIT(2)
#define RIFSC_RIMC_ATTRx_MCID_MASK		GENMASK(6, 4)
#define RIFSC_RIMC_ATTRx_MCID_SHIFT		4
#define RIFSC_RIMC_ATTRx_MSEC			BIT(8)
#define RIFSC_RIMC_ATTRx_MPRIV			BIT(9)

#endif /* STM32MP_RIFSC_REGS_H */
