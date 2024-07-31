/*
 * Copyright (c) 2022, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BSEC3_REG_H
#define BSEC3_REG_H

#include <lib/utils_def.h>

/* BSEC REGISTER OFFSET (base relative) */
#define BSEC_FVR(x)			(U(0x000) + 4U * (x))
#define BSEC_SPLOCK(x)			(U(0x800) + 4U * (x))
#define BSEC_SWLOCK(x)			(U(0x840) + 4U * (x))
#define BSEC_SRLOCK(x)			(U(0x880) + 4U * (x))
#define BSEC_OTPVLDR(x)			(U(0x8C0) + 4U * (x))
#define BSEC_SFSR(x)			(U(0x940) + 4U * (x))
#define BSEC_OTPCR			U(0xC04)
#define BSEC_WDR			U(0xC08)
#define BSEC_SCRATCHR0			U(0xE00)
#define BSEC_SCRATCHR1			U(0xE04)
#define BSEC_SCRATCHR2			U(0xE08)
#define BSEC_SCRATCHR3			U(0xE0C)
#define BSEC_LOCKR			U(0xE10)
#define BSEC_JTAGINR			U(0xE14)
#define BSEC_JTAGOUTR			U(0xE18)
#define BSEC_DENR			U(0xE20)
#define BSEC_UNMAPR			U(0xE24)
#define BSEC_SR				U(0xE40)
#define BSEC_OTPSR			U(0xE44)
#define BSEC_WRCR			U(0xF00)
#define BSEC_HWCFGR			U(0xFF0)
#define BSEC_VERR			U(0xFF4)
#define BSEC_IPIDR			U(0xFF8)
#define BSEC_SIDR			U(0xFFC)

/* BSEC_OTPCR register fields */
#define BSEC_OTPCR_ADDR_MASK		GENMASK(8, 0)
#define BSEC_OTPCR_ADDR_SHIFT		0
#define BSEC_OTPCR_PROG			BIT(13)
#define BSEC_OTPCR_PPLOCK		BIT(14)
#define BSEC_OTPCR_LASTCID_MASK		GENMASK(21, 19)
#define BSEC_OTPCR_LASTCID_SHIFT	19

/* BSEC_LOCKR register fields */
#define BSEC_LOCKR_GWLOCK_MASK		BIT(0)
#define BSEC_LOCKR_GWLOCK_SHIFT		0
#define BSEC_LOCKR_DENLOCK_MASK		BIT(1)
#define BSEC_LOCKR_DENLOCK_SHIFT	1
#define BSEC_LOCKR_HKLOCK_MASK		BIT(2)
#define BSEC_LOCKR_HKLOCK_SHIFT		2

/* BSEC_DENR register fields */
#define BSEC_DENR_LPDBGEN		BIT(0)
#define BSEC_DENR_DBGENA		BIT(1)
#define BSEC_DENR_NIDENA		BIT(2)
#define BSEC_DENR_DEVICEEN		BIT(3)
#define BSEC_DENR_HDPEN			BIT(4)
#define BSEC_DENR_SPIDENA		BIT(5)
#define BSEC_DENR_SPNIDENA		BIT(6)
#define BSEC_DENR_DBGSWEN		BIT(7)
#define BSEC_DENR_DBGENM		BIT(8)
#define BSEC_DENR_NIDENM		BIT(9)
#define BSEC_DENR_SPIDENM		BIT(10)
#define BSEC_DENR_SPNIDENM		BIT(11)
#define BSEC_DENR_CFGSDIS		BIT(12)
#define BSEC_DENR_CP15SDIS_MASK		GENMASK(14, 13)
#define BSEC_DENR_CP15SDIS_SHIFT	13
#define BSEC_DENR_LPDBGDIS		BIT(15)
#define BSEC_DENR_ALL_MSK		GENMASK(15, 0)

/* BSEC_SR register fields */
#define BSEC_SR_BUSY			BIT(0)
#define BSEC_SR_HVALID			BIT(1)
#define BSEC_SR_RNGERR			BIT(2)
#define BSEC_SR_HKWW_MASK		GENMASK(15, 8)
#define BSEC_SR_HKWW_SHIFT		8
#define BSEC_SR_NVSTATE_MASK		GENMASK(31, 26)
#define BSEC_SR_NVSTATE_SHIFT		26
#define BSEC_SR_NVSTATE_OPEN		U(0x16)
#define BSEC_SR_NVSTATE_CLOSED		U(0x0D)
#define BSEC_SR_NVSTATE_OTP_LOCKED	U(0x23)

/* BSEC_OTPSR register fields */
#define BSEC_OTPSR_BUSY			BIT(0)
#define BSEC_OTPSR_INIT_DONE		BIT(1)
#define BSEC_OTPSR_HIDEUP		BIT(2)
#define BSEC_OTPSR_OTPNVIR		BIT(4)
#define BSEC_OTPSR_OTPERR		BIT(5)
#define BSEC_OTPSR_OTPSEC		BIT(6)
#define BSEC_OTPSR_PROGFAIL		BIT(16)
#define BSEC_OTPSR_DISTURBF		BIT(17)
#define BSEC_OTPSR_DEDF			BIT(18)
#define BSEC_OTPSR_SECF			BIT(19)
#define BSEC_OTPSR_PPLF			BIT(20)
#define BSEC_OTPSR_PPLMF		BIT(21)
#define BSEC_OTPSR_AMEF			BIT(22)

/* BSEC_VERR register fields */
#define BSEC_VERR_MASK			GENMASK(7, 0)

#endif /* BSEC3_REG_H */
