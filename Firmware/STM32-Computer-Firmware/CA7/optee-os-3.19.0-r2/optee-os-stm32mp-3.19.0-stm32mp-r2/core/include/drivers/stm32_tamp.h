/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021-2023, STMicroelectronics
 */

#ifndef __DRIVERS_STM32_TAMP_H__
#define __DRIVERS_STM32_TAMP_H__

#include <compiler.h>
#include <drivers/clk.h>
#include <drivers/stm32_rif.h>
#include <mm/core_memprot.h>
#include <stddef.h>
#include <stdint.h>
#include <tee_api_types.h>
#include <types_ext.h>

/* Tamper ident */
enum stm32_tamp_id {
	INT_TAMP1 = 0,
	INT_TAMP2,
	INT_TAMP3,
	INT_TAMP4,
	INT_TAMP5,
	INT_TAMP6,
	INT_TAMP7,
	INT_TAMP8,
	INT_TAMP9,
	INT_TAMP10,
	INT_TAMP11,
	INT_TAMP12,
	INT_TAMP13,
	INT_TAMP14,
	INT_TAMP15,
	INT_TAMP16,
	INT_TAMP_NB,

	EXT_TAMP1 = INT_TAMP_NB,
	EXT_TAMP2,
	EXT_TAMP3,
	EXT_TAMP4,
	EXT_TAMP5,
	EXT_TAMP6,
	EXT_TAMP7,
	EXT_TAMP8,

	LAST_TAMP,
	INVALID_TAMP = 0xFFFF,
};

/* Callback return bitmask values */
#define TAMP_CB_ACK		BIT(0)
#define TAMP_CB_RESET		BIT(1)
#define TAMP_CB_ACK_AND_RESET	(TAMP_CB_RESET | TAMP_CB_ACK)

/*
 * struct stm32_bkpregs_conf - Interface for stm32_tamp_set_secure_bkpregs()
 * @nb_zone1_regs - Number of backup registers in zone 1
 * @nb_zone2_regs - Number of backup registers in zone 2
 *
 * TAMP backup registers access permissions
 *
 * Zone 1: read/write in secure state, no access in non-secure state
 * Zone 2: read/write in secure state, read-only in non-secure state
 * Zone 3: read/write in secure state, read/write in non-secure state
 *
 * Protection zone 1
 * If nb_zone1_regs == 0 no backup register are in zone 1.
 * Otherwise backup registers from TAMP_BKP0R to TAMP_BKP<x>R are in zone 1,
 * with <x> = (@nb_zone1_regs - 1).
 *
 * Protection zone 2
 * If nb_zone2_regs == 0 no backup register are in zone 2.
 * Otherwise backup registers from TAMP_BKP<y>R ro TAMP_BKP<z>R are in zone 2,
 * with <y> = @nb_zone1_regs and <z> = (@nb_zone1_regs1 + @nb_zone2_regs - 1).
 *
 * Protection zone 3
 * Backup registers from TAMP_BKP<t>R to last backup register are in zone 3,
 * with <t> = (@nb_zone1_regs1 + @nb_zone2_regs).
 */
struct stm32_bkpregs_conf {
	uint32_t nb_zone1_regs;
	uint32_t nb_zone2_regs;
	uint32_t *rif_offsets;
};

/* Define TAMPER modes */
#define TAMP_ERASE		0x0U
#define TAMP_NOERASE		BIT(1)
#define TAMP_NO_EVT_MASK	0x0U
#define TAMP_EVT_MASK		BIT(2)
#define TAMP_MODE_MASK		GENMASK_32(15, 0)

/* Compatibility tags */
#define TAMP_HAS_REGISTER_SECCFGR	BIT(0)
#define TAMP_HAS_REGISTER_PRIVCFGR	BIT(1)
#define TAMP_HAS_REGISTER_ERCFGR	BIT(2)
#define TAMP_HAS_REGISTER_ATCR2		BIT(3)
#define TAMP_HAS_REGISTER_CR3		BIT(4)
#define TAMP_HAS_CR2_SECRET_STATUS	BIT(5)
#define TAMP_SIZE_ATCR1_ATCKSEL_IS_4	BIT(7)
#define TAMP_HAS_RIF_SUPPORT		BIT(31)

struct stm32_tamp_compat {
	struct stm32_tamp_conf *ext_tamp;
	struct stm32_tamp_conf *int_tamp;
	const struct stm32_tamp_pin_map *pin_map;
	int nb_monotonic_counter;
	uint32_t ext_tamp_size;
	uint32_t int_tamp_size;
	uint32_t pin_map_size;
	uint32_t tags;
	uint32_t exti_wakeup_line;
};

struct stm32_tamp_platdata {
	struct io_pa_va base;
	struct clk *clock;
	struct stm32_tamp_compat *compat;
	struct rif_conf_data conf_data;
	struct stm32_bkpregs_conf *bkpregs_conf;
	struct stm32_exti_pdata *exti;
	int it;
	unsigned int nb_resources;
	uint32_t passive_conf;
	uint32_t active_conf;
	uint32_t pins_conf;
	uint32_t out_pins;
	bool is_wakeup_source;
	bool mask_pot_reset;
};

#endif /* __DRIVERS_STM32_TAMP_H__ */
