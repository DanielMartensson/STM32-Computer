/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018, ARM Limited and Contributors. All rights reserved.
 */

#ifndef __STM32MP_PM_CONTEXT_H__
#define __STM32MP_PM_CONTEXT_H__

#ifndef __ASSEMBLER__
#include <compiler.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stm32mp_pm.h>
#include <tee_api_types.h>
#endif

#define PM_CTX_CCM_KEY_SIZE		32
#define PM_CTX_CCM_CTR1_SIZE		16
#define PM_CTX_CCM_B0_SIZE		16
#define PM_CTX_CCM_CTR0_SIZE		16
#define PM_CTX_CCM_TAG_SIZE		16

#ifndef __ASSEMBLER__
/*
 * All materials for the CCM sequence using CRYP support are preloaded
 * in this specific structure. Note that the sequence does not use AAD.
 *
 * @key: AES key material buffer
 * @ctr1: Preformatted 128bit CTR1 block
 * @ctr1: Preformatted 128bit B0 block
 * @ctr1: Preformatted 128bit CTR0 block
 * @tag: Buffer where the generated CCM tag is stored
 */
struct ccm_unpg_ctx {
	uint32_t key[PM_CTX_CCM_KEY_SIZE / sizeof(uint32_t)];
	uint32_t ctr1[PM_CTX_CCM_CTR1_SIZE / sizeof(uint32_t)];
	uint32_t b0[PM_CTX_CCM_B0_SIZE / sizeof(uint32_t)];
	uint32_t ctr0[PM_CTX_CCM_CTR0_SIZE / sizeof(uint32_t)];
	uint32_t tag[PM_CTX_CCM_TAG_SIZE / sizeof(uint32_t)];
};

/*
 * This structure is used by pm_helpers.S at early resume from retention RAM.
 * It is defined here and used by context_asm_defines.c to generate offset
 * macros for the assembly implementation in pm_helpers.S.
 *
 * To lower the memory footprint of suspend sequence, The same function is
 * used for encryption (executed from TEE RAM with MMU enabled) and for
 * decryption (executed from BKPSRAM with MMU disabled). Therefore some
 * required addresses are provided by the caller through this structure
 * especially some SoC interface registers that are likely to have different
 * physical and virtual addresses.
 *
 * @resume_pa: OP-TEE resume physical entry in TEE RAM (once restored)
 * @teeram_bkp_pa: Physical base address in TEE RAM backup in DDR
 * @cryp1_base: Base address of the CRYP1 registers (physical or virtual)
 * @rcc_base: Base address of the RCC registers (physical or virtual)
 * @stgen_base: Base address of the STGEN registers (physical or virtual)
 * @stgen_cnt: STGEN cycle counter backup cell and measure of cycles spent
 * @ccm_ref_tag: 128bit arrays storing tag generated during encryption
 * @ccm_ctx: Structure storing CCM configuration and generated tag
 * @resume_sequence: Code/data array for the BKPSRAM resume sequence
 */
struct retram_resume_ctx {
	uint32_t resume_pa;
	uint32_t teeram_bkp_pa;
	uint32_t cryp1_base;
	uint32_t rcc_base;
	uint32_t stgen_base;
	uint32_t stgen_cnt;
	uint8_t ccm_ref_tag[PM_CTX_CCM_TAG_SIZE];
	struct ccm_unpg_ctx ccm_ctx;
	/* Last start the resume routine ARM (32bit) instructions sequence */
	uint32_t resume_sequence[];
};

extern const uint8_t stm32mp_bkpsram_image_end[];
void stm32mp_bkpsram_resume(void);
void stm32mp_sysram_resume(void);

void stm32mp_cpu_reset_state(void);

TEE_Result stm32mp_pm_save_context(unsigned int soc_mode);
void stm32mp_pm_restore_context(unsigned int soc_mode);
void stm32mp_pm_shutdown_context(void);
void stm32mp_pm_wipe_context(void);

#ifndef CFG_STM32MP1_OPTEE_IN_SYSRAM
TEE_Result stm32mp_pm_call_bl2_lp_entry(unsigned int soc_mode);
#endif

int stm32mp1_set_pm_domain_state(enum stm32mp1_pm_domain domain, bool status);

uint32_t stm32mp1_get_lp_soc_mode(uint32_t psci_mode);
int stm32mp1_set_lp_deepest_soc_mode(uint32_t psci_mode, uint32_t soc_mode);

uintptr_t stm32mp_pm_retram_resume_ep(void);
bool stm32mp1_is_retram_during_standby(void);

int stm32mp_ccm_encrypt_teeram(struct retram_resume_ctx *ctx,
			       void *dst, void *src, size_t size);
int stm32mp_ccm_decrypt_teeram(struct retram_resume_ctx *ctx,
			       void *dst, void *src, size_t size);
#endif /*__ASSEMBLER__*/

#endif /*__STM32MP_PM_CONTEXT_H__*/
