/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2018-2021, STMicroelectronics
 * Copyright (c) 2017-2021 NXP
 * Copyright (c) 2016-2021, ARM Limited and Contributors. All rights reserved.
 */

#include <arm.h>
#include <arm32_macros.S>
#include <asm.S>
#include <boot_api.h>
#include <drivers/stm32mp1_rcc.h>
#include <generated/context_asm_defines.h>
#include <kernel/cache_helpers.h>
#include <kernel/tz_proc_def.h>
#include <kernel/tz_ssvce_def.h>
#include <platform_config.h>
#include <mm/core_mmu.h>
#include <util.h>

#include "context.h"

/*
 * Right bit shift distance to reach timeout from a 1s STGEN freq count
 * Value N relates to 1000ms / 2^N, i.e 7 relates to 7.8125ms=~8ms
 */
#define CCM_TIMEOUT_128MS		2
#define CCM_TIMEOUT_8MS			7
#define CCM_TIMEOUT_1MS			10
#define CCM_TIMEOUT_16US		16
#define CCM_TIMEOUT			CCM_TIMEOUT_8MS

/*
 * CRYP interface register used for AES CCM
 */
#define CRYP_CR				0x000
#define CRYP_SR				0x004
#define CRYP_DIN			0x008
#define CRYP_DOUT			0x00c
#define CRYP_KEYR_BASE			0x020
#define CRYP_IVR_BASE			0x040

#define CRYP_CR_ALGODIR_DECRYPT		BIT(2)
#define CRYP_CR_ALGOMODE_MASK		(BIT(19) | GENMASK_32(5, 3))
#define CRYP_CR_ALGOMODE(m)		(((m & BIT(3)) << 16) | (m & 0x7) << 3)
#define ALGOMODE_AES_CCM		0x9
#define CRYP_CR_DATATYPE_SHIFT		6
#define CRYP_CR_DATATYPE_8BIT		(2 << CRYP_CR_DATATYPE_SHIFT)
#define CRYP_CR_KEYSIZE_SHIFT		8
#define CRYP_CR_KEYSIZE_256BIT		(2U << CRYP_CR_KEYSIZE_SHIFT)
#define CRYP_CR_CRYPEN			BIT(15)
#define CRYP_CR_FFLUSH			BIT(14)
#define CRYP_CR_GCM_CCMPH_SHIFT		16
#define CRYP_CR_PHASE_MASK		(0x3 << CRYP_CR_GCM_CCMPH_SHIFT)
#define CRYP_CR_INIT_PHASE		(0 << CRYP_CR_GCM_CCMPH_SHIFT)
#define CRYP_CR_HEADER_PHASE		(1 << CRYP_CR_GCM_CCMPH_SHIFT)
#define CRYP_CR_PAYLOAD_PHASE		(2 << CRYP_CR_GCM_CCMPH_SHIFT)
#define CRYP_CR_FINAL_PHASE		(3 << CRYP_CR_GCM_CCMPH_SHIFT)

#define CRYP_SR_BUSY			BIT(4)
#define CRYP_SR_OFFU			BIT(3)
#define CRYP_SR_OFNE			BIT(2)
#define CRYP_SR_IFNF			BIT(1)
#define CRYP_SR_IFEM			BIT(0)

/*
 * Enable TRACE_SYSRAM_RESTORE to get some UART console traces
 * at resume time.
 */
#if defined(TRACE_SYSRAM_RESTORE)

#define UART_BASE			UART4_BASE
#define UART_ISR_OFF			0x1c
#define UART_TDR_OFF			0x28
#define USART_ISR_TXE_TXFNF		(1<< 7)

	.macro PRINT_CHAR _reg0, _reg1, _char
		/* Trace only at resume when MMU is OFF */
		read_sctlr \_reg0
		ands	\_reg0, #SCTLR_M
	101:
		bne	102f
		mov_imm	\_reg0, UART4_BASE
		ldr	\_reg1, [\_reg0, #UART_ISR_OFF]
		ands	\_reg1, #USART_ISR_TXE_TXFNF
		beq 101b
		mov_imm	\_reg1, (\_char)
		str	\_reg1, [\_reg0, #UART_TDR_OFF]
	102:
	.endm
#else
	.macro PRINT_CHAR _reg0, _reg1, _char
	.endm
#endif

	/* Bound of the binary image loaded in retained memory */
	.global stm32mp_bkpsram_image_end

/*
 * stm32mp_bkpsram_resume - Restore TEE RAM from backup memory and resume into
 *
 * This function executes at early resume from suspend state. It is the
 * entrypoint of the OP-TEE provided to early boot stage when SoC wakes.
 * This code is located in a retained memory, MMU disabled. This function
 * shall restore TEE RAM content for OP-TEE to resume execution. Once
 * TEE RAM is restored, this function branches to the resident resume entry
 * point in TEE_RAM. This function and its resources shall execute in place.
 */
FUNC stm32mp_bkpsram_resume , :
UNWIND(	.cantunwind)

	PRINT_CHAR r0, r1, '0'

	/*
	 * Almost all sequences here expect PM context structure base address
	 * from CPU register r11.
	 */
	mov_imm	r11, (BKPSRAM_BASE + BKPSRAM_PM_CONTEXT_OFFSET)

	/* stm32mp_ccm_teeram needs some HW interface base addresss */
	mov_imm r0, CRYP1_BASE
	str	r0, [r11, #PM_CTX_CRYP1_BASE]
	mov_imm r0, RCC_BASE
	str	r0, [r11, #PM_CTX_RCC_BASE]
	mov_imm r0, STGEN_BASE
	str	r0, [r11, #PM_CTX_STGEN_BASE]

	bl	_clear_early_mailbox
	bl	_prepare_time

	mov_imm r0, TEE_RAM_START
	ldr	r1, [r11, #PM_CTX_TEERAM_BKP_PA]
	mov_imm r2, TEE_RAM_PH_SIZE
	mov_imm r3, 1
	bl	stm32mp_ccm_teeram
	cmp	r0, #0
	bne	_failed

	PRINT_CHAR r0, r1, 'T'
	PRINT_CHAR r0, r1, 'a'
	PRINT_CHAR r0, r1, 'g'
	PRINT_CHAR r0, r1, '\n'

	/* Compare the generated and reference tags */
	add	r8, r11, #PM_CTX_CCM_TAG
	add	r9, r11, #PM_CTX_CCM_REF_TAG
	ldm	r8, {r2-r5}
	ldm	r9, {r6-r9}
	mov	r0, #0
	cmp	r2, r6
	addeq	r0, #1
	cmp	r3, r7
	addeq	r0, #1
	cmp	r4, r8
	addeq	r0, #1
	cmp	r5, r9
	addeq	r0, #1
	cmp	r0, #4
	bne	_failed
	bl	_save_resume_time

	PRINT_CHAR r1, r2, 'O'
	PRINT_CHAR r1, r2, 'k'
	PRINT_CHAR r1, r2, '\n'

	/* Resume into the restored TEE RAM */
	ldr	r1, [r11, #PM_CTX_RESUME_PA]
	bx	r1

_failed:
	PRINT_CHAR r0, r12, 'F'
	PRINT_CHAR r0, r12, '\n'

	/* Clear context including key and reference tag */
	mov	r0, #0xa5
	mov_imm	r12, BKPSRAM_PM_CONTEXT_SIZE
	add	r12, r11, r12
1:	str	r0, [r11], #4
	cmp	r11, r12
	blt	1b
	b	.

	/*
	 * _clear_early_mailbox - Wipe mailbox in case of reset
	 *
	 * Sratches r0-r4.
	 * All other CPU registers are preserved.
	 */
_clear_early_mailbox:
	/* Clear the backup registers (first enable RTCAPB clock) */
	mov_imm r0, (RCC_BASE + RCC_MP_APB5ENSETR)
	mov_imm	r2, RCC_MP_APB5ENSETR_RTCAPBEN
	ldr	r1, [r0]
	ands	r1, r1, r2
	moveq	r1, r2
	movne	r1, #0
	str	r2, [r0]
	mov_imm	r2, (TAMP_BASE + TAMP_BKP_REGISTER_OFF)
	mov_imm	r3, (BCKR_CORE1_MAGIC_NUMBER * 4)
	mov_imm	r4, BOOT_API_A7_RESET_MAGIC_NUMBER
	str	r4, [r2, r3]
	mov_imm	r3, (BCKR_CORE1_BRANCH_ADDRESS * 4)
	mov	r4, #0
	str	r4, [r2, r3]
	/* Restore RTCAPB clock initial state */
	str	r1, [r0, #RCC_MP_ENCLRR_OFFSET]
	bx	lr

	/*
	 * prepare_time - save/reset cycle counter to prevent later overflow
	 *
	 * Save current 32bit lower counter and reset to 0 so that later
	 * timeout test do not need to care about overflow.
	 *
	 * Expects r11 is context base and lr is return address.
	 * Scrathes r0-r2.
	 * All other CPU registers are preserved.
	 */
_prepare_time:
	ldr	r2, [r11, #PM_CTX_STGEN_BASE]
	/* Disable STGEN counter */
	ldr	r1, [r2, #CNTCR_OFFSET]
	bic	r1, r1, #CNTCR_EN
	str	r1, [r2, #CNTCR_OFFSET]
1:	ldr	r1, [r2, #CNTSR_OFFSET]
	tst	r1, #CNTCR_EN
	bne	1b
	/* Save and reset STGEN counter */
	ldr	r0, [r2, #CNTCVL_OFFSET]
	str	r0, [r11, #PM_CTX_STGEN_CNT]
	mov	r0, #0
	str	r0, [r2, #CNTCVL_OFFSET]
	ldr	r0, [r2, #CNTCVU_OFFSET]
	str	r0, [r2, #CNTCVU_OFFSET]
	/* Enable STGEN counter */
	ldr	r1, [r2, #CNTCR_OFFSET]
	orr	r1, r1, #CNTCR_EN
	str	r1, [r2, #CNTCR_OFFSET]
	bx	lr

	/*
	 * save_resume_time - save time spent and restore STGEN cycle counter
	 *
	 * Restore STGEN counter to initial value incremented by the current
	 * count. Note 32bit upper may need to be incremented.
	 *
	 * Expects r11 is context base and lr is return address.
	 * Scrathes r0-r3.
	 * All other CPU registers are preserved.
	 */
_save_resume_time:
	/* Compute update STGEN counter 32bit LSB value */
	ldr	r2, [r11, #PM_CTX_STGEN_BASE]
	ldr	r0, [r11, #PM_CTX_STGEN_CNT]
	ldr	r3, [r2, #CNTCVL_OFFSET]
	str	r3, [r11, #PM_CTX_STGEN_CNT]
	adds	r0, r0, r3
	/* Disable STGEN  */
	ldr	r1, [r2, #CNTCR_OFFSET]
	bic	r1, r1, #CNTCR_EN
	str	r1, [r2, #CNTCR_OFFSET]
1:	ldr	r1, [r2, #CNTSR_OFFSET]
	tst	r1, #CNTCR_EN
	bne	1b
	/* Update counter (increment 32bit MSB if requried) */
	str	r0, [r2, #CNTCVL_OFFSET]
	ldr	r0, [r2, #CNTCVU_OFFSET]
	addcs	r0, r0, #1
	str	r0, [r2, #CNTCVU_OFFSET]	/* Write CNTCVU value ... */
	ldr	r0, [r2, #CNTCVU_OFFSET]	/* ... and wait it is set */
	/* Enable STGEN  */
	ldr	r0, [r2, #CNTCR_OFFSET]
	orr	r0, r0, #CNTCR_EN
	str	r0, [r2, #CNTCR_OFFSET]
	bx	lr

	/*
	 * _setup_cryp1 - Enable CRYP1 hardware: reset & clock
	 * _reset_cryp1 - Reset CRYP1 hardware
	 *
	 * Function call before and after CCM sequence. Note that the CRYP1
	 * clock remain enabled. It is disabled later by the resume sequence.
	 *
	 * Expects r11 is context base and lr is return address.
	 * Scratches r0-r3.
	 */
_setup_cryp1:
	ldr	r1, [r11, #PM_CTX_RCC_BASE]
	mov_imm r0, RCC_MP_AHB5ENSETR_CRYP1EN
	str	r0, [r1, #RCC_MP_AHB5ENSETR]
	/* Intentionnally fall through reset_cryp1 */
_reset_cryp1:
	ldr	r3, [r11, #PM_CTX_RCC_BASE]
	mov_imm r0, RCC_AHB5RSTSETR_CRYP1RST
	str	r0, [r3, #RCC_AHB5RSTSETR]
1:	ldr	r1, [r3, #RCC_AHB5RSTSETR]
	ands	r1, r1, r0
	beq	1b
	mov_imm r0, RCC_AHB5RSTSETR_CRYP1RST
	str	r0, [r3, #RCC_AHB5RSTCLRR]
1:	ldr	r1, [r3, #RCC_AHB5RSTSETR]
	ands	r1, r1, r0
	bne	1b
	bx	lr

	/*
	 * _ccm_arm_8ms_timeout - Init 8ms threshold for _ccm_failed_on_timeout
	 * _ccm_fail_on_timeout - Check STGEN counter against timeout threshold
	 *
	 * These function are used by the macro wait_flag_timeout_8ms. The
	 * former loads the timeout in CPU register r0 while the later get the
	 * timeout counter threshold from CPU register r0.
	 *
	 * Expect r11 is context base and lr is return address.
	 * Scratch r0-r1.
	 * All other CPU registers are preserved.
	 */
_ccm_arm_8ms_timeout:
	ldr	r1, [r11, #PM_CTX_STGEN_BASE]
	ldr	r0, [r1, #CNTFID_OFFSET]
	lsrs	r0, r0, #CCM_TIMEOUT
	moveq	r0, #1
	ldr	r1, [r1, #CNTCVL_OFFSET]
	adds	r0, r0, r1
	bcs	_ccm_failed_on_timeout
	bx	lr

_ccm_fail_on_timeout:

	ldr	r1, [r11, #PM_CTX_STGEN_BASE]
	ldr	r1, [r1, #CNTCVL_OFFSET]
	cmp	r1, r0
	bge	_ccm_failed_on_timeout
	bx	lr

_ccm_failed_on_timeout:
	PRINT_CHAR r0, r1, 'T'
	PRINT_CHAR r0, r1, 'o'
	PRINT_CHAR r0, r1, '\n'
	b	_ccm_failed

	/*
	 * Macro WAIT_FLAG_TIMEOUT compares timeout threshold (r0) with
	 * current time and branches the CCM failure entry on timeout.
	 * It is assumed the 32bit timestamps cannot overflow.
	 */
	.macro WAIT_FLAG_TIMEOUT register_offset, bit_mask, awaited_mask
		bl	_ccm_arm_8ms_timeout
	1:
		bl	_ccm_fail_on_timeout
		ldr	r1, [r10, #(\register_offset)]
		and	r1, r1, #(\bit_mask)
		cmp	r1, #(\awaited_mask)
		bne	1b
	.endm

/*
 * stm32mp_ccm_teeram - Size optimzed unpaged CCM encryption/decryption
 *
 * This sequence encrypts or decrypts a input block using AES CCM with a
 * 256bit key and no AAD and generates the CCM tag. The key, CTR0, CTR1
 * and B0 block are read from PM context structure. The generated tag is
 * stored in the PM context structure.
 *
 * This function is executed from TEE RAM during suspend sequence to generate
 * the encrypted data and the tag. This function is also executed from BKPSRAM
 * called with MMU disabled. Therefore this sequence shall be comply with
 * position independent code constraints.
 *
 * Expects at entry:
 * lr = caller return address
 * r11 = retram_resume_ctx structure base address
 * r0 = Destination buffer for the output data (ciphertext or plaintext)
 * r1 = Source buffer for the input data (plaintext or ciphertext)
 * r2 = Input (and output) data size in bytes
 * r3 = 1 if decrypting, 0 if encrypting
 */
stm32mp_ccm_teeram:
	/*
	 * Use of the CPU registers in the whole stm32mp_ccm_teeram sequence
	 *
	 * sp: preserved, not used
	 * lr: scratch register used to call subroutines.
	 * r12: saves the caller link register for final return
	 * r11: context from BKPSRAM
	 * r10: CRYP1 base address
	 * r9: destination buffer
	 * r8: source buffer to cipher
	 * r7: data byte counter
	 * r0-r6 are scratch registers
	 */
	mov	r12, lr
	ldr	r10, [r11, #PM_CTX_CRYP1_BASE]
	mov	r9, r0
	mov	r8, r1
	mov	r7, r2
	mov	r6, r3

	PRINT_CHAR r0, r1, '1'

	bl	_setup_cryp1

	PRINT_CHAR r0, r1, '2'

	mov_imm	r0, (CRYP_CR_ALGOMODE(ALGOMODE_AES_CCM) | \
		     CRYP_CR_DATATYPE_8BIT | CRYP_CR_FFLUSH | \
		     CRYP_CR_KEYSIZE_256BIT)
	cmp	r6, #0
	orrne	r0, r0, #CRYP_CR_ALGODIR_DECRYPT
	str	r0, [r10, #CRYP_CR]

	PRINT_CHAR r0, r1, '3'

	/* Check data alignment (addresses and size) */
	ands	r0, r7, #0x0F
	bne	_ccm_failed
	ands	r0, r8, #0x03
	bne	_ccm_failed
	ands	r0, r9, #0x03
	bne	_ccm_failed

	PRINT_CHAR r0, r1, '4'

	ldr	r0, [r11, #PM_CTX_CCM_KEY]
	str	r0, [r10, #CRYP_KEYR_BASE]
	ldr	r0, [r11, #(PM_CTX_CCM_KEY + 4)]
	str	r0, [r10, #(CRYP_KEYR_BASE + 4)]
	ldr	r0, [r11, #(PM_CTX_CCM_KEY + 8)]
	str	r0, [r10, #(CRYP_KEYR_BASE + 8)]
	ldr	r0, [r11, #(PM_CTX_CCM_KEY + 12)]
	str	r0, [r10, #(CRYP_KEYR_BASE + 12)]
	ldr	r0, [r11, #(PM_CTX_CCM_KEY + 16)]
	str	r0, [r10, #(CRYP_KEYR_BASE + 16)]
	ldr	r0, [r11, #(PM_CTX_CCM_KEY + 20)]
	str	r0, [r10, #(CRYP_KEYR_BASE + 20)]
	ldr	r0, [r11, #(PM_CTX_CCM_KEY + 24)]
	str	r0, [r10, #(CRYP_KEYR_BASE + 24)]
	ldr	r0, [r11, #(PM_CTX_CCM_KEY + 28)]
	str	r0, [r10, #(CRYP_KEYR_BASE + 28)]

	ldr	r0, [r11, #PM_CTX_CCM_CTR1]
	str	r0, [r10, #CRYP_IVR_BASE]
	ldr	r0, [r11, #(PM_CTX_CCM_CTR1 + 4)]
	str	r0, [r10, #(CRYP_IVR_BASE + 4)]
	ldr	r0, [r11, #(PM_CTX_CCM_CTR1 + 8)]
	str	r0, [r10, #(CRYP_IVR_BASE + 8)]
	ldr	r0, [r11, #(PM_CTX_CCM_CTR1 + 12)]
	str	r0, [r10, #(CRYP_IVR_BASE + 12)]

	/* Setup CRYP for the CCM Init Phase */
	ldr	r0, [r10, #CRYP_CR]
	orr	r0, r0, #(CRYP_CR_CRYPEN | CRYP_CR_INIT_PHASE)
	str	r0, [r10, #CRYP_CR]
	ldr	r0, [r10, #CRYP_CR]

	ldr	r0, [r11, #PM_CTX_CCM_B0]
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r11, #(PM_CTX_CCM_B0 + 4)]
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r11, #(PM_CTX_CCM_B0 + 8)]
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r11, #(PM_CTX_CCM_B0 + 12)]
	str	r0, [r10, #CRYP_DIN]

	PRINT_CHAR r0, r1, '5'

	WAIT_FLAG_TIMEOUT CRYP_CR, CRYP_CR_CRYPEN, 0

	/* Setup CRYP for the CCM Payload phase */
	ldr	r0, [r10, #CRYP_CR]
	bic	r0, r0, #CRYP_CR_PHASE_MASK
	orr	r0, r0, #CRYP_CR_PAYLOAD_PHASE
	orr	r0, r0, #CRYP_CR_CRYPEN
	str	r0, [r10, #CRYP_CR]
	ldr	r0, [r10, #CRYP_CR]

	PRINT_CHAR r0, r1, '\n'

_next_block:
	PRINT_CHAR r0, r1, 'b'

	WAIT_FLAG_TIMEOUT CRYP_SR, CRYP_SR_IFEM, CRYP_SR_IFEM

	/* Feed input data, r8 stores the current source buffer */
	ldr	r0, [r8], #4
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r8], #4
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r8], #4
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r8], #4
	str	r0, [r10, #CRYP_DIN]

	WAIT_FLAG_TIMEOUT CRYP_SR, CRYP_SR_OFNE, CRYP_SR_OFNE

	/* Store output data, r9 stores the current source buffer */
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r9], #4
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r9], #4
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r9], #4
	/* Before last 32bit word, the output FIFO shall not be empty */
	ldr	r0, [r10, #CRYP_SR]
	ands	r0, r0, #CRYP_SR_OFNE
	beq	_ccm_failed
	/* After last 32bit word for this 128block, FIFO shall be empty */
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r9], #4
	ldr	r0, [r10, #CRYP_SR]
	ands	r0, r0, #CRYP_SR_OFNE
	bne	_ccm_failed

	/* Another round if remaining data */
	subs	r7, r7, #16
	bne	_next_block;

	PRINT_CHAR r0, r1, '\n'
	PRINT_CHAR r0, r1, '6'

	WAIT_FLAG_TIMEOUT CRYP_SR, CRYP_SR_BUSY, 0

	/*
	 * Data processing completed, now remains the tag generation.
	 * Here expect SR[IFNF]=SR[OFNE]=1 and all others bits are 0.
	 */
	ldr	r0, [r10, #CRYP_SR]
	cmp	r0, #(CRYP_SR_IFEM | CRYP_SR_IFNF)
	bne	_ccm_failed

	PRINT_CHAR r0, r1, '7'

	/* Setup CRYP1 for the CCM Final Phase */
	ldr	r0, [r10, #CRYP_CR]
	bic	r0, r0, #CRYP_CR_CRYPEN
	str	r0, [r10, #CRYP_CR]
	ldr	r0, [r10, #CRYP_CR]
	bic	r0, r0, #CRYP_CR_PHASE_MASK
	bic	r0, r0, #CRYP_CR_ALGODIR_DECRYPT
	orr	r0, r0, #CRYP_CR_FINAL_PHASE
	orr	r0, r0, #CRYP_CR_CRYPEN
	str	r0, [r10, #CRYP_CR]
	ldr	r0, [r10, #CRYP_CR]

	/* Load CTR0 to generate the tag */
	ldr	r0, [r11, #PM_CTX_CCM_CTR0]
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r11, #(PM_CTX_CCM_CTR0 + 4)]
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r11, #(PM_CTX_CCM_CTR0 + 8)]
	str	r0, [r10, #CRYP_DIN]
	ldr	r0, [r11, #(PM_CTX_CCM_CTR0 + 12)]
	str	r0, [r10, #CRYP_DIN]

	PRINT_CHAR r0, r1, '8'

	WAIT_FLAG_TIMEOUT CRYP_SR, CRYP_SR_OFNE, CRYP_SR_OFNE

	/* Store generated tag in the PM_CTX structure */
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r11, #PM_CTX_CCM_TAG]
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r11, #(PM_CTX_CCM_TAG + 4)]
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r11, #(PM_CTX_CCM_TAG + 8)]
	/* Before last 32bit word, the output FIFO shall not be empty */
	ldr	r0, [r10, #CRYP_SR]
	ands	r0, r0, #CRYP_SR_OFNE
	beq	_ccm_failed
	/* After last 32bit word for this 128block, FIFO shall be empty */
	ldr	r0, [r10, #CRYP_DOUT]
	str	r0, [r11, #(PM_CTX_CCM_TAG + 12)]
	ldr	r0, [r10, #CRYP_SR]
	ands	r0, r0, #CRYP_SR_OFNE
	bne	_ccm_failed

	PRINT_CHAR r0, r1, '9'

	/* Successful return */
	bl	_reset_cryp1
	mov	r0, #0
	bx	r12

_ccm_failed:

	PRINT_CHAR r0, r1, 'K'
	PRINT_CHAR r0, r1, 'O'

	bl	_reset_cryp1
	mov	r0, #1
	bx	r12

/* End address of the PIC resume sequence copy in retained RAM */
stm32mp_bkpsram_image_end:
	nop

END_FUNC stm32mp_bkpsram_resume

/*
 * int stm32mp_ccm_encrypt_teeram(ctx, dst, src, len)
 */
FUNC stm32mp_ccm_encrypt_teeram , :
	push	{r4-r12, lr}
UNWIND(	.save	{r4-r12, lr})
	mov	r11, r0
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	mov	r3, #0
	push	{r0-r3}
	bl	_prepare_time
	pop	{r0-r3}
	bl	stm32mp_ccm_teeram
	bl	_save_resume_time
	pop	{r4-r12, pc}
END_FUNC stm32mp_ccm_encrypt_teeram

/*
 * int stm32mp_ccm_decrypt_teeram(ctx, cryp_base, dst, src)
 */
FUNC stm32mp_ccm_decrypt_teeram , :
	push	{r4-r12, lr}
UNWIND(	.save	{r4-r12, lr})
	mov	r11, r0
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	mov	r3, #1
	push	{r0-r3}
	bl	_prepare_time
	pop	{r0-r3}
	bl	stm32mp_ccm_teeram
	bl	_save_resume_time
	pop	{r4-r12, pc}
END_FUNC stm32mp_ccm_decrypt_teeram

/*
 * stm32mp_sysram_resume - Resume OP-TEE execution
 *
 * This function is the entry point of OP-TEE core resume sequence in the TEE
 * RAM. When TEE RAM is lost during a power cycle, stm32mp_bkpsram_resume() is
 * called to restore TEE RAM content and branch to this stm32mp_sysram_resume()
 * routine.
 *
 * This function calls the OP-TEE core generic PM resume API
 * sm_pm_cpu_resume().
 */
FUNC stm32mp_sysram_resume, :
UNWIND(	.cantunwind)
	/* Invalidate the data cache */

	read_clidr r2
	ubfx	r3, r2, #CLIDR_LOC_SHIFT, #CLIDR_FIELD_WIDTH
	lsl	r3, r3, #CSSELR_LEVEL_SHIFT
	mov	r1, #0

loop1:
	add	r10, r1, r1, LSR #1	// Work out 3x current cache level
	mov	r12, r2, LSR r10	// extract cache type bits from clidr
	and	r12, r12, #7   		// mask the bits for current cache only
	cmp	r12, #2			// see what cache we have at this level
	blo	level_done      	// no cache or only instruction cache at this level

	write_csselr r1			// select current cache level in csselr
	isb				// isb to sych the new cssr&csidr
	read_ccsidr r12			// read the new ccsidr
	and	r10, r12, #7   		// extract the length of the cache lines
	add	r10, r10, #4        	// add 4 (r10 = line length offset)
	ubfx	r4, r12, #3, #10	// r4 = maximum way number (right aligned)
	clz	r5, r4            	// r5 = the bit position of the way size increment
	mov	r9, r4			// r9 working copy of the aligned max way number

loop2:
	ubfx	r7, r12, #13, #15	// r7 = max set number (right aligned)

loop3:
	orr	r0, r1, r9, LSL r5	// factor in the way number and cache level into r0
	orr	r0, r0, r7, LSL r10	// factor in the set number

	write_dcisw r0

	subs	r7, r7, #1              // decrement the set number
	bhs	loop3
	subs	r9, r9, #1              // decrement the way number
	bhs	loop2
level_done:
	add	r1, r1, #2		// increment the cache number
	cmp	r3, r1
	dsb	sy			// ensure completion of previous cache maintenance instruction
	bhi	loop1

	mov	r6, #0
	write_csselr r6			//select cache level 0 in csselr
	dsb	sy
	isb

	/* Resume sequence executes in Monitor mode */
	cps	#CPSR_MODE_MON

	blx	plat_cpu_reset_early
	b	sm_pm_cpu_resume
END_FUNC stm32mp_sysram_resume

/*
 * stm32mp_cpu_reset_state - set CPU in a reset like state
 *
 * Disable CPU env (interrupts, cache, SMP, MMU) and return.
 * Preserve the execution mode in CPSR.
 */
FUNC stm32mp_cpu_reset_state, :
	push	{r12, lr}
UNWIND(	.save	{r12, lr})

	cpsid	aif

	bl	psci_armv7_cpu_off

	write_bpiall
	dsb
	isb
	read_sctlr r0
	bic	r0, r0, #SCTLR_M
	bic	r0, r0, #SCTLR_I
	write_sctlr r0
	dsb	sy
	isb

	pop	{r12, pc}
END_FUNC stm32mp_cpu_reset_state
