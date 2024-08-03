// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2017-2022, STMicroelectronics
 */

#include <config.h>
#include <drivers/stm32_bsec.h>
#include <drivers/stm32_etzpc.h>
#include <drivers/stm32_firewall.h>
#include <drivers/stm32_gpio.h>
#include <drivers/stm32mp1_rcc.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <initcall.h>
#include <io.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <platform_config.h>
#include <stdbool.h>
#include <stm32_util.h>
#include <string.h>

/*
 * Once one starts to get the resource registering state, one cannot register
 * new resources. This ensures resource state cannot change.
 */
static bool registering_locked;

/*
 * Shared peripherals and resources registration
 *
 * Each resource assignation is stored in a table. The state defaults
 * to PERIPH_UNREGISTERED if the resource is not explicitly assigned.
 *
 * Resource driver that as not embedded (a.k.a their related CFG_xxx build
 * directive is disabled) are assigned to the non-secure world.
 *
 * Each IO of the GPIOZ IO can be secure or non-secure.
 *
 * It is the platform responsibility the ensure resource assignation
 * matches the access permission firewalls configuration.
 */
enum shres_state {
	SHRES_UNREGISTERED = 0,
	SHRES_SECURE,
	SHRES_NON_SECURE,
};

/* Use a byte array to store each resource state */
static uint8_t shres_state[STM32MP1_SHRES_COUNT];

static const char __maybe_unused *shres2str_id_tbl[STM32MP1_SHRES_COUNT] = {
	[STM32MP1_SHRES_IWDG1] = "IWDG1",
	[STM32MP1_SHRES_USART1] = "USART1",
	[STM32MP1_SHRES_SPI6] = "SPI6",
	[STM32MP1_SHRES_I2C4] = "I2C4",
	[STM32MP1_SHRES_RNG1] = "RNG1",
	[STM32MP1_SHRES_HASH1] = "HASH1",
	[STM32MP1_SHRES_CRYP1] = "CRYP1",
	[STM32MP1_SHRES_I2C6] = "I2C6",
	[STM32MP1_SHRES_RTC] = "RTC",
	[STM32MP1_SHRES_MCU] = "MCU",
	[STM32MP1_SHRES_PLL3] = "PLL3",
	[STM32MP1_SHRES_MDMA] = "MDMA",
};

static __maybe_unused const char *shres2str_id(enum stm32mp_shres id)
{
	return shres2str_id_tbl[id];
}

static const char *shres2str_state_tbl[4] __maybe_unused = {
	[SHRES_UNREGISTERED] = "unregistered",
	[SHRES_NON_SECURE] = "non-secure",
	[SHRES_SECURE] = "secure",
};

static __maybe_unused const char *shres2str_state(enum shres_state id)
{
	return shres2str_state_tbl[id];
}

static void register_periph(enum stm32mp_shres id, enum shres_state state)
{
	assert(id < STM32MP1_SHRES_COUNT &&
	       (state == SHRES_SECURE || state == SHRES_NON_SECURE));

	if (registering_locked)
		panic();

	if (shres_state[id] != SHRES_UNREGISTERED &&
	    shres_state[id] != state) {
		DMSG("Cannot change %s from %s to %s",
		     shres2str_id(id),
		     shres2str_state(shres_state[id]),
		     shres2str_state(state));
		panic();
	}

	if (shres_state[id] == SHRES_UNREGISTERED)
		DMSG("Register %s as %s",
		     shres2str_id(id), shres2str_state(state));

	shres_state[id] = state;

	/* Explore clock tree to lock secure clock dependencies */
	if (state == SHRES_SECURE) {
		switch (id) {
		case STM32MP1_SHRES_IWDG1:
			stm32mp_register_clock_parents_secure(IWDG1);
			break;
		case STM32MP1_SHRES_USART1:
			stm32mp_register_clock_parents_secure(USART1_K);
			break;
		case STM32MP1_SHRES_SPI6:
			stm32mp_register_clock_parents_secure(SPI6_K);
			break;
		case STM32MP1_SHRES_I2C4:
			stm32mp_register_clock_parents_secure(I2C4_K);
			break;
		case STM32MP1_SHRES_RNG1:
			stm32mp_register_clock_parents_secure(RNG1_K);
			break;
		case STM32MP1_SHRES_HASH1:
			stm32mp_register_clock_parents_secure(HASH1);
			break;
		case STM32MP1_SHRES_CRYP1:
			stm32mp_register_clock_parents_secure(CRYP1);
			break;
		case STM32MP1_SHRES_I2C6:
			stm32mp_register_clock_parents_secure(I2C6_K);
			break;
		case STM32MP1_SHRES_RTC:
			stm32mp_register_clock_parents_secure(RTC);
			break;
		default:
			/* No expected resource dependency */
			break;
		}
	}
}

/* Register resource by ID */
void stm32mp_register_secure_periph(enum stm32mp_shres id)
{
	register_periph(id, SHRES_SECURE);
}

void stm32mp_register_non_secure_periph(enum stm32mp_shres id)
{
	register_periph(id, SHRES_NON_SECURE);
}

/* Register resource by IO memory base address */
static void register_periph_iomem(vaddr_t base, enum shres_state state)
{
	enum stm32mp_shres id = STM32MP1_SHRES_COUNT;

	switch (base) {
	case IWDG1_BASE:
		id = STM32MP1_SHRES_IWDG1;
		break;
	case USART1_BASE:
		id = STM32MP1_SHRES_USART1;
		break;
	case SPI6_BASE:
		id = STM32MP1_SHRES_SPI6;
		break;
	case I2C4_BASE:
		id = STM32MP1_SHRES_I2C4;
		break;
	case I2C6_BASE:
		id = STM32MP1_SHRES_I2C6;
		break;
	case RTC_BASE:
		id = STM32MP1_SHRES_RTC;
		break;
	case RNG1_BASE:
		id = STM32MP1_SHRES_RNG1;
		break;
	case CRYP1_BASE:
		id = STM32MP1_SHRES_CRYP1;
		break;
	case HASH1_BASE:
		id = STM32MP1_SHRES_HASH1;
		break;

#ifdef CFG_WITH_NSEC_UARTS
	case USART2_BASE:
	case USART3_BASE:
	case UART4_BASE:
	case UART5_BASE:
	case USART6_BASE:
	case UART7_BASE:
	case UART8_BASE:
	/* Fall through */
#endif
	case IWDG2_BASE:
		/* Allow drivers to register some non-secure resources */
		DMSG("IO for non-secure resource 0x%lx", base);
		if (state != SHRES_NON_SECURE)
			panic();

		return;

	default:
		panic();
		break;
	}

	register_periph(id, state);
}

void stm32mp_register_secure_periph_iomem(vaddr_t base)
{
	register_periph_iomem(base, SHRES_SECURE);
}

void stm32mp_register_non_secure_periph_iomem(vaddr_t base)
{
	register_periph_iomem(base, SHRES_NON_SECURE);
}

static void lock_registering(void)
{
	registering_locked = true;
}

bool stm32mp_periph_is_secure(enum stm32mp_shres id)
{
	lock_registering();

	return shres_state[id] == SHRES_SECURE;
}

bool stm32mp_nsec_can_access_clock(unsigned long clock_id)
{
	enum stm32mp_shres shres_id = STM32MP1_SHRES_COUNT;

	/* Oscillators and PLLs are visible from non-secure world */
	COMPILE_TIME_ASSERT(CK_HSE == 0 &&
			    (CK_HSE + 1) == CK_CSI &&
			    (CK_HSE + 2) == CK_LSI &&
			    (CK_HSE + 3) == CK_LSE &&
			    (CK_HSE + 4) == CK_HSI &&
			    (CK_HSE + 5) == CK_HSE_DIV2 &&
			    (PLL1_P + 1) == PLL1_Q &&
			    (PLL1_P + 2) == PLL1_R &&
			    (PLL1_P + 3) == PLL2_P &&
			    (PLL1_P + 4) == PLL2_Q &&
			    (PLL1_P + 5) == PLL2_R &&
			    (PLL1_P + 6) == PLL3_P &&
			    (PLL1_P + 7) == PLL3_Q &&
			    (PLL1_P + 8) == PLL3_R);

	if (clock_id <= CK_HSE_DIV2 ||
	    (clock_id >= PLL1_P && clock_id <= PLL3_R))
		return true;

	switch (clock_id) {
	case RTCAPB:
	case CK_MPU:
	case CK_AXI:
	case BSEC:
		return true;
	case GPIOZ:
		return true;
	case SPI6_K:
		shres_id = STM32MP1_SHRES_SPI6;
		break;
	case I2C4_K:
		shres_id = STM32MP1_SHRES_I2C4;
		break;
	case I2C6_K:
		shres_id = STM32MP1_SHRES_I2C6;
		break;
	case USART1_K:
		shres_id = STM32MP1_SHRES_USART1;
		break;
	case IWDG1:
		shres_id = STM32MP1_SHRES_IWDG1;
		break;
	case CRYP1:
		shres_id = STM32MP1_SHRES_CRYP1;
		break;
	case HASH1:
		shres_id = STM32MP1_SHRES_HASH1;
		break;
	case RNG1_K:
		shres_id = STM32MP1_SHRES_RNG1;
		break;
	case RTC:
		shres_id = STM32MP1_SHRES_RTC;
		break;
	case CK_MCU:
		shres_id = STM32MP1_SHRES_MCU;
		break;
	default:
		return false;
	}

	return !stm32mp_periph_is_secure(shres_id);
}

static bool mckprot_resource(enum stm32mp_shres id)
{
	switch (id) {
	case STM32MP1_SHRES_MCU:
	case STM32MP1_SHRES_PLL3:
		return true;
	default:
		return false;
	}
}

static void check_rcc_secure_configuration(void)
{
	bool secure = stm32_rcc_is_secure();
	bool mckprot = stm32_rcc_is_mckprot();
	bool need_secure = false;
	bool need_mckprot = false;
	enum stm32mp_shres id = STM32MP1_SHRES_COUNT;
	uint32_t state = 0;

	if (stm32_bsec_get_state(&state))
		panic();

	if (state == BSEC_STATE_SEC_CLOSED && !secure)
		panic("Closed device mandates secure RCC");

	for (id = 0; id < STM32MP1_SHRES_COUNT; id++) {
		if  (shres_state[id] != SHRES_SECURE)
			continue;

		need_secure = true;
		if (mckprot_resource(id))
			need_mckprot = true;

		if ((mckprot_resource(id) && !mckprot) || !secure)
			EMSG("Error: RCC TZEN=%u MCKPROT=%u while %s is secure",
			     secure, mckprot, shres2str_id(id));
	}

	DMSG("RCC/PWR secure hardening: TZEN %sable, MCKPROT %sable",
	     secure ? "en" : "dis", mckprot ? "en" : "dis");

	if (need_secure && !secure)
		panic();

	stm32mp1_clk_mcuss_protect(need_mckprot);
}

static vaddr_t stm32mp_get_base_from_shres_id(enum stm32mp_shres id)
{
	vaddr_t base = 0;

	switch (id) {
	case STM32MP1_SHRES_IWDG1:
		base = IWDG1_BASE;
		break;
	case STM32MP1_SHRES_USART1:
		base = USART1_BASE;
		break;
	case STM32MP1_SHRES_SPI6:
		base = SPI6_BASE;
		break;
	case STM32MP1_SHRES_I2C4:
		base = I2C4_BASE;
		break;
	case STM32MP1_SHRES_I2C6:
		base = I2C6_BASE;
		break;
	case STM32MP1_SHRES_RTC:
		base = RTC_BASE;
		break;
	case STM32MP1_SHRES_RNG1:
		base = RNG1_BASE;
		break;
	case STM32MP1_SHRES_CRYP1:
		base = CRYP1_BASE;
		break;
	case STM32MP1_SHRES_HASH1:
		base = HASH1_BASE;
		break;
	default:
		base = 0;
	}

	return base;
}

static void check_iomem_firewall_configuration(void)
{
	TEE_Result res = TEE_SUCCESS;
	bool firewall_is_ns = false;
	bool shres_is_ns = false;
	enum stm32mp_shres id = 0;
	paddr_t base = 0;
	const struct stm32_firewall_cfg nsec_cfg[] = {
		{ FWLL_NSEC_RW | FWLL_MASTER(0) },
		{ }, /* Null terminated */
	};

	for (id = 0; id < STM32MP1_SHRES_COUNT; id++) {
		if (shres_state[id] == SHRES_UNREGISTERED)
			continue;

		shres_is_ns = !stm32mp_periph_is_secure(id);

		base = stm32mp_get_base_from_shres_id(id);
		if (!base) {
			DMSG("Shared resource %d not a IOmem entry", id);
			continue;
		}

		res = stm32_firewall_check_access(base, 0, nsec_cfg);
		firewall_is_ns = (res == TEE_SUCCESS);

		if (firewall_is_ns != shres_is_ns) {
			EMSG("mismatch config for id : %d", id);
			panic();
		}
	}
}

static TEE_Result stm32mp1_init_final_shres(void)
{
	enum stm32mp_shres id = STM32MP1_SHRES_COUNT;

	lock_registering();

	for (id = (enum stm32mp_shres)0; id < STM32MP1_SHRES_COUNT; id++) {
		uint8_t __maybe_unused *state = &shres_state[id];

		DMSG("stm32mp %-8s (%2u): %-14s",
		     shres2str_id(id), id, shres2str_state(*state));
	}

	check_rcc_secure_configuration();

	check_iomem_firewall_configuration();

	return TEE_SUCCESS;
}
/* Finalize shres after drivers initialization, hence driver_init_late() */
driver_init_late(stm32mp1_init_final_shres);
