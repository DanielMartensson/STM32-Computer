/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2022, STMicroelectronics
 */

#ifndef __STM32_UTIL_H__
#define __STM32_UTIL_H__

#include <assert.h>
#include <drivers/clk.h>
#include <drivers/stm32mp1_rcc_util.h>
#include <kernel/panic.h>
#include <stdbool.h>
#include <stdint.h>
#include <tee_api_types.h>
#include <types_ext.h>

/* SoC versioning and device ID */
TEE_Result stm32mp1_dbgmcu_get_chip_version(uint32_t *chip_version);
TEE_Result stm32mp1_dbgmcu_get_chip_dev_id(uint32_t *chip_dev_id);

/* OPP service */
bool stm32mp_supports_cpu_opp(uint32_t opp_id);

/*  Crypto HW support */
bool stm32mp_supports_hw_cryp(void);

/*  Second core support */
bool stm32mp_supports_second_core(void);

/* Backup registers and RAM utils */
vaddr_t stm32mp_bkpreg(unsigned int idx);
vaddr_t stm32mp_bkpsram_base(void);

/* Platform util for the STGEN driver */
vaddr_t stm32mp_stgen_base(void);

/* Get device ID from SYSCFG registers */
uint32_t stm32mp_syscfg_get_chip_dev_id(void);

/* Erase ESRAM3 */
TEE_Result stm32mp_syscfg_erase_sram3(void);

/* Platform util for the GIC */
vaddr_t get_gicc_base(void);
vaddr_t get_gicd_base(void);

#if TRACE_LEVEL >= TRACE_DEBUG
/*
 * stm32mp_dump_core_registers - Print CPU registers to console
 *
 * @panicking:  False if we are not called from a panic sequence. True if we
 *              are panicking. Trace messages are emitted only once this
 *              function is called with @panicking being true. Until then,
 *              calling with @panicking being false emits no trace.
 */
void stm32mp_dump_core_registers(bool panicking);
#else
static inline void stm32mp_dump_core_registers(bool panicking __unused) { }
#endif

/* Platform util for PMIC support */
bool stm32mp_with_pmic(void);

/**
 * stm32_reconfigure_region() - Allows to reconfigure a memory region.
 * @fdt:	Device tree file to work on
 * @phandle:	Phandle of the memory region
 * @cfg:	When @cfg points to a NULL reference, the function grants
 *		access to OP-TEE compartment and returns in @cfg a pointer
 *		to the previously applied configuration, to be later used in a
 *		call to the same API function.
 *		When @cfg points to a non-NULL reference, that reference is
 *		expected to be obtained from a previous call to this function
 *		and contains the configuration to be applied. In such case, the
 *		function sets @cfg output value to a NULL reference.
 */
TEE_Result stm32_reconfigure_region(const void *fdt, int phandle,
				    const void **cfg);

/* Power management service */
#ifdef CFG_PSCI_ARM32
void stm32mp_register_online_cpu(void);
#else
static inline void stm32mp_register_online_cpu(void)
{
}
#endif

/*
 * Generic spinlock function that bypass spinlock if MMU is disabled or
 * lock is NULL.
 */
uint32_t may_spin_lock(unsigned int *lock);
void may_spin_unlock(unsigned int *lock, uint32_t exceptions);

#ifdef CFG_STM32MP1_SHARED_RESOURCES
/* Return true if @clock_id is shared by secure and non-secure worlds */
bool stm32mp_nsec_can_access_clock(unsigned long clock_id);
#else /* CFG_STM32MP1_SHARED_RESOURCES */
static inline bool stm32mp_nsec_can_access_clock(unsigned long clock_id
						 __unused)
{
	return true;
}
#endif /* CFG_STM32MP1_SHARED_RESOURCES */

/*
 * Shared reference counter: increments by 2 on secure increment
 * request, decrements by 2 on secure decrement request. Bit #0
 * is set to 1 on non-secure increment request and reset to 0 on
 * non-secure decrement request. These counters initialize to
 * either 0, 1 or 2 upon their expect default state.
 * Counters saturate to UINT_MAX / 2.
 */
#define SHREFCNT_NONSECURE_FLAG		0x1ul
#define SHREFCNT_SECURE_STEP		0x2ul
#define SHREFCNT_MAX			(UINT_MAX / 2)

/* Return 1 if refcnt increments from 0, else return 0 */
static inline int incr_shrefcnt(unsigned int *refcnt, bool secure)
{
	int rc = !*refcnt;

	if (secure) {
		if (*refcnt < SHREFCNT_MAX) {
			*refcnt += SHREFCNT_SECURE_STEP;
			assert(*refcnt < SHREFCNT_MAX);
		}
	} else {
		*refcnt |= SHREFCNT_NONSECURE_FLAG;
	}

	return rc;
}

/* Return 1 if refcnt decrements to 0, else return 0 */
static inline int decr_shrefcnt(unsigned int *refcnt, bool secure)
{
	int  rc = 0;

	if (secure) {
		if (*refcnt < SHREFCNT_MAX) {
			if (*refcnt < SHREFCNT_SECURE_STEP)
				panic();

			*refcnt -= SHREFCNT_SECURE_STEP;
			rc = !*refcnt;
		}
	} else {
		rc = (*refcnt == SHREFCNT_NONSECURE_FLAG);
		*refcnt &= ~SHREFCNT_NONSECURE_FLAG;
	}

	return rc;
}

static inline int incr_refcnt(unsigned int *refcnt)
{
	return incr_shrefcnt(refcnt, true);
}

static inline int decr_refcnt(unsigned int *refcnt)
{
	return decr_shrefcnt(refcnt, true);
}

/*
 * Shared peripherals and resources registration
 *
 * Resources listed in enum stm32mp_shres assigned at run-time to the
 * non-secure world, to the secure world or shared by both worlds.
 * In the later case, there must exist a secure service in OP-TEE
 * for the non-secure world to access the resource.
 *
 * Resources may be a peripheral, a bus, a clock or a memory.
 *
 * Shared resources driver API functions allows drivers to register the
 * resource as secure, non-secure or shared and to get the resource
 * assignation state.
 */
enum stm32mp_shres {
	STM32MP1_SHRES_IWDG1,
	STM32MP1_SHRES_USART1,
	STM32MP1_SHRES_SPI6,
	STM32MP1_SHRES_I2C4,
	STM32MP1_SHRES_RNG1,
	STM32MP1_SHRES_HASH1,
	STM32MP1_SHRES_CRYP1,
	STM32MP1_SHRES_I2C6,
	STM32MP1_SHRES_RTC,
	STM32MP1_SHRES_MCU,
	STM32MP1_SHRES_PLL3,
	STM32MP1_SHRES_MDMA,

	STM32MP1_SHRES_COUNT
};

#ifdef CFG_STM32MP1_SHARED_RESOURCES
/* Register resource @id as a secure peripheral */
void stm32mp_register_secure_periph(enum stm32mp_shres id);

/* Register resource @id as a non-secure peripheral */
void stm32mp_register_non_secure_periph(enum stm32mp_shres id);

/*
 * Register resource identified by @base as a secure peripheral
 * @base: IOMEM physical base address of the resource
 */
void stm32mp_register_secure_periph_iomem(vaddr_t base);

/*
 * Register resource identified by @base as a non-secure peripheral
 * @base: IOMEM physical base address of the resource
 */
void stm32mp_register_non_secure_periph_iomem(vaddr_t base);

/* Return true if and only if resource @id is registered as secure */
bool stm32mp_periph_is_secure(enum stm32mp_shres id);

#else /* CFG_STM32MP1_SHARED_RESOURCES */

static inline void stm32mp_register_secure_periph(enum stm32mp_shres id
						  __unused)
{
}

static inline void stm32mp_register_non_secure_periph(enum stm32mp_shres id
						      __unused)
{
}

static inline void stm32mp_register_secure_periph_iomem(vaddr_t base __unused)
{
}

static inline void stm32mp_register_non_secure_periph_iomem(vaddr_t base
							    __unused)
{
}

static inline void stm32mp_register_secure_gpio(unsigned int bank __unused,
						unsigned int pin __unused)
{
}

static inline void stm32mp_register_non_secure_gpio(unsigned int bank __unused,
						    unsigned int pin __unused)
{
}

static inline bool stm32mp_periph_is_secure(enum stm32mp_shres id __unused)
{
	return true;
}

static inline bool stm32mp_gpio_bank_is_secure(unsigned int bank __unused)
{
	return true;
}

static inline bool stm32mp_gpio_bank_is_shared(unsigned int bank __unused)
{
	return false;
}

static inline bool stm32mp_gpio_bank_is_non_secure(unsigned int bank __unused)
{
	return false;
}

#endif /* CFG_STM32MP1_SHARED_RESOURCES */

void __noreturn do_reset(const char *str);

TEE_Result stm32_activate_internal_tamper(int id);

#endif /*__STM32_UTIL_H__*/
