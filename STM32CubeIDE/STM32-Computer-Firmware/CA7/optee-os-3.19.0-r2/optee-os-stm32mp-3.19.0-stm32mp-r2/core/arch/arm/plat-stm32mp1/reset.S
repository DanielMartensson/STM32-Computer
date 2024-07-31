/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018, STMicroelectronics
 */

#include <arm.h>
#include <arm32_macros.S>
#include <asm.S>

.section .text
.balign 4
.code 32

#define STM32MP1_NSACR_PRESERVE_MASK	(0xfff << 20)

FUNC plat_cpu_reset_early , :
	ldr	r0, =SCR_SIF
	write_scr r0

	read_nsacr r0
	mov_imm	r1, STM32MP1_NSACR_PRESERVE_MASK
	and	r0, r0, r1
	write_nsacr r0
	isb

	read_actlr r0
	orr	r0, r0, #ACTLR_SMP
	write_actlr r0

	/*
	 * Always reset CNTVOFF for the dear non secure world.
	 * This operation requires being in Monitor mode and
	 * non secure state.
	 */
	mrs	r1, cpsr
	cps	#CPSR_MODE_MON
	isb

	read_scr r2
	orr	r0, r2, #SCR_NS
	write_scr r0
	isb

	mov	r0, #0
	write_cntvoff r0, r0

	write_scr r2
	isb
	msr	cpsr, r1
	isb

	bx	lr
END_FUNC plat_cpu_reset_early
