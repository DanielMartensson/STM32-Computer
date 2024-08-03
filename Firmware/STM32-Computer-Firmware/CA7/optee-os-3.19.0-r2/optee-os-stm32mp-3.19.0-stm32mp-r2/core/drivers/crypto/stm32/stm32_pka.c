// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2021, STMicroelectronics - All Rights Reserved
 */

#include <assert.h>
#include <config.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <io.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/mutex.h>
#include <kernel/pm.h>
#include <kernel/pm.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <stm32_util.h>
#include <tee_api_types.h>

#include "common.h"
#include "stm32_pka.h"

/*
 * For our comprehension in this file
 *  _len are in BITs
 *  _size are in BYTEs
 *  _nbw are in number of PKA_word (PKA_word = u64)
 */

#define INT8_LEN			8U
#define INT64_LEN			(INT8_LEN * sizeof(uint64_t))
#define WORD_SIZE			(sizeof(uint64_t))
#define OP_NBW_FROM_LEN(len)		(ROUNDUP_DIV(len, INT64_LEN) + 1)
#define OP_NBW_FROM_SIZE(s)		OP_NBW_FROM_LEN((s) * INT8_LEN)
#define OP_SIZE_FROM_SIZE(s)		(OP_NBW_FROM_SIZE(s) * WORD_SIZE)

#define MAX_EO_NBW			OP_NBW_FROM_LEN(PKA_MAX_ECC_LEN)

/* PKA registers */
#define _PKA_CR				0x0U
#define _PKA_SR				0x4U
#define _PKA_CLRFR			0x8U
#define _PKA_VERR			0x1FF4
#define _PKA_IPIDR			0x1FF8U

/* PKA control register fields */
#define _PKA_CR_MODE_MASK		GENMASK_32(13, 8)
#define _PKA_CR_MODE_SHIFT		8U
#define _PKA_CR_MODE_ADD		0x9U
#define _PKA_CR_MODE_R2MODN		0x01U
#define _PKA_CR_MODE_POINT_CHECK	0x28U
#define _PKA_CR_MODE_ECC_KP		0x20U
#define _PKA_CR_MODE_ECDSA_SIGN		0x24U
#define _PKA_CR_MODE_ECDSA_VERIF	0x26U
#define _PKA_CR_START			BIT(1)
#define _PKA_CR_EN			BIT(0)

/* PKA status register fields */
#define _PKA_SR_BUSY			BIT(16)
#define _PKA_SR_LMF			BIT(1)
#define _PKA_SR_INITOK			BIT(0)

/* PKA it flag fields (used in CR, SR and CLRFR) */
#define _PKA_IT_MASK			(GENMASK_32(21, 19) | BIT(17))
#define _PKA_IT_SHIFT			17U
#define _PKA_IT_OPERR			BIT(21)
#define _PKA_IT_ADDRERR			BIT(20)
#define _PKA_IT_RAMERR			BIT(19)
#define _PKA_IT_PROCEND			BIT(17)

/* PKA version register fields */
#define _PKA_VERR_MAJREV_MASK		GENMASK_32(7, 4)
#define _PKA_VERR_MAJREV_SHIFT		4U
#define _PKA_VERR_MINREV_MASK		GENMASK_32(3, 0)
#define _PKA_VERR_MINREV_SHIFT		0U

/* PKA identification register value */
#define _PKA_IDID			0x00170072U

/* RAM magic offset */
#define _PKA_RAM_START			0x400U
#define _PKA_RAM_SIZE			5336U

/* Montgomery parameter computation (R*R mod n) */
#define _PKA_RAM_R2MODN_N_LEN		0x408U /* 64 */
#define _PKA_RAM_R2MODN_PRIME_N		0x1088U /* EOS */
#define _PKA_RAM_R2MODN_OUT		0x620U /* EOS */

/* ECC check if point P is on curve */
#define _PKA_RAM_ONCURVE_N_LEN		0x408U /* 64 */
#define _PKA_RAM_ONCURVE_A_SIGN		0x410U /* 64 */
#define _PKA_RAM_ONCURVE_A		0x418U /* EOS */
#define _PKA_RAM_ONCURVE_B		0x520U /* EOS */
#define _PKA_RAM_ONCURVE_P		0x470U /* EOS */
#define _PKA_RAM_ONCURVE_XP		0x578U /* EOS */
#define _PKA_RAM_ONCURVE_YP		0x5D0U /* EOS */
#define _PKA_RAM_ONCURVE_R2MODN		0x4C8U /* EOS */
#define _PKA_RAM_ONCURVE_RES		0x680U /* 64 */
#define _PKA_RAM_ONCURVE_RES_YES	0xD60DULL
#define _PKA_RAM_ONCURVE_RES_NO		0xA3B7ULL
#define _PKA_RAM_ONCURVE_RES_TOOBIG	0xF946ULL

/* ECC Fp scalar multiplication (kP) */
#define _PKA_RAM_KP_N_LEN		0x400U /* 64 */
#define _PKA_RAM_KP_P_LEN		0x408U /* 64 */
#define _PKA_RAM_KP_A_SIGN		0x410U /* 64 */
#define _PKA_RAM_KP_A			0x418U /* EOS */
#define _PKA_RAM_KP_B			0x520U /* EOS */
#define _PKA_RAM_KP_P			0x1088U /* EOS */
#define _PKA_RAM_KP_K			0x12A0U /* EOS */
#define _PKA_RAM_KP_XP			0x578U /* EOS */
#define _PKA_RAM_KP_YP			0x470U /* EOS */
#define _PKA_RAM_KP_PRIME_N		0xF88U /* EOS */
#define _PKA_RAM_KP_RES			0x680U /* 64 */
#define _PKA_RAM_KP_RES_SUCCESS		0xD60DULL
#define _PKA_RAM_KP_RES_FAIL		0xCBC9ULL
#define _PKA_RAM_KP_X			0x578U /* EOS*/
#define _PKA_RAM_KP_Y			0x5D0U /* EOS*/

/* ECDSA sign */
#define _PKA_RAM_SIGN_N_LEN		0x400U /* 64 */
#define _PKA_RAM_SIGN_P_LEN		0x408U /* 64 */
#define _PKA_RAM_SIGN_A_SIGN		0x410U /* 64 */
#define _PKA_RAM_SIGN_A			0x418U /* EOS */
#define _PKA_RAM_SIGN_B			0x520U /* EOS */
#define _PKA_RAM_SIGN_P			0x1088U /* EOS */
#define _PKA_RAM_SIGN_K			0x12A0U /* EOS */
#define _PKA_RAM_SIGN_XG		0x578U /* EOS */
#define _PKA_RAM_SIGN_YG		0x470U /* EOS */
#define _PKA_RAM_SIGN_HASH_Z		0xFE8U /* EOS */
#define _PKA_RAM_SIGN_D			0xF28U /* EOS */
#define _PKA_RAM_SIGN_PRIME_N		0xF88U /* EOS */
#define _PKA_RAM_SIGN_RES		0xFE0U /* 64 */
#define _PKA_RAM_SIGN_RES_SUCCESS	0xD60DULL
#define _PKA_RAM_SIGN_RES_FAIL		0xCBC9ULL
#define _PKA_RAM_SIGN_RES_R0		0xA3B7ULL
#define _PKA_RAM_SIGN_RES_S0		0xF946ULL
#define _PKA_RAM_SIGN_R			0x730U /* EOS*/
#define _PKA_RAM_SIGN_S			0x788U /* EOS*/

/* ECDSA verification */
#define _PKA_RAM_VERIF_N_LEN		0x408U /* 64 */
#define _PKA_RAM_VERIF_P_LEN		0x4C8U /* 64 */
#define _PKA_RAM_VERIF_A_SIGN		0x468U /* 64 */
#define _PKA_RAM_VERIF_A		0x470U /* EOS */
#define _PKA_RAM_VERIF_P		0x4D0U /* EOS */
#define _PKA_RAM_VERIF_XG		0x678U /* EOS */
#define _PKA_RAM_VERIF_YG		0x6D0U /* EOS */
#define _PKA_RAM_VERIF_XQ		0x12F8U /* EOS */
#define _PKA_RAM_VERIF_YQ		0x1350U /* EOS */
#define _PKA_RAM_VERIF_SIGN_R		0x10E0U /* EOS */
#define _PKA_RAM_VERIF_SIGN_S		0xC68U /* EOS */
#define _PKA_RAM_VERIF_HASH_Z		0x13A8U /* EOS */
#define _PKA_RAM_VERIF_PRIME_N		0x1088U /* EOS */
#define _PKA_RAM_VERIF_RES		0x5D0U /* 64 */
#define _PKA_RAM_VERIF_RES_VALID	0xD60DULL
#define _PKA_RAM_VERIF_RES_INVALID	0xA3B7ULL

#define PKA_TIMEOUT_US			1000000U
#define TIMEOUT_US_1MS			1000U
#define PKA_RESET_DELAY			20U

enum pka_op {
	sign,
	verif,
	scalar_mul,
	on_curve,

	pka_op_last
};

enum pka_ram_index {
	N_LEN,
	P_LEN,
	A_SIGN,
	COEFF_A,
	COEFF_B,
	PRIME_N,
	VAL_P,
	GPOINT_X,
	GPOINT_Y,

	PKA_RAM_INDEX_LAST,
};

static uint32_t pka_ram[pka_op_last][PKA_RAM_INDEX_LAST] = {
	[sign] = {
		[N_LEN]    = _PKA_RAM_SIGN_N_LEN,
		[P_LEN]    = _PKA_RAM_SIGN_P_LEN,
		[A_SIGN]   = _PKA_RAM_SIGN_A_SIGN,
		[COEFF_A]  = _PKA_RAM_SIGN_A,
		[COEFF_B]  = _PKA_RAM_SIGN_B,
		[PRIME_N]  = _PKA_RAM_SIGN_PRIME_N,
		[VAL_P]    = _PKA_RAM_SIGN_P,
		[GPOINT_X] = _PKA_RAM_SIGN_XG,
		[GPOINT_Y] = _PKA_RAM_SIGN_YG
	},
	[verif] = {
		[N_LEN]    = _PKA_RAM_VERIF_N_LEN,
		[P_LEN]    = _PKA_RAM_VERIF_P_LEN,
		[A_SIGN]   = _PKA_RAM_VERIF_A_SIGN,
		[COEFF_A]  = _PKA_RAM_VERIF_A,
		[COEFF_B]  = 0U,
		[PRIME_N]  = _PKA_RAM_VERIF_PRIME_N,
		[VAL_P]    = _PKA_RAM_VERIF_P,
		[GPOINT_X] = _PKA_RAM_VERIF_XG,
		[GPOINT_Y] = _PKA_RAM_VERIF_YG
	},
	[scalar_mul] = {
		[N_LEN]    = _PKA_RAM_KP_N_LEN,
		[P_LEN]    = _PKA_RAM_KP_P_LEN,
		[A_SIGN]   = _PKA_RAM_KP_A_SIGN,
		[COEFF_A]  = _PKA_RAM_KP_A,
		[COEFF_B]  = _PKA_RAM_KP_B,
		[PRIME_N]  = _PKA_RAM_KP_PRIME_N,
		[VAL_P]    = _PKA_RAM_KP_P,
		[GPOINT_X] = 0,
		[GPOINT_Y] = 0,
	},
	[on_curve] = {
		[N_LEN]    = _PKA_RAM_ONCURVE_N_LEN,
		[P_LEN]    = 0,
		[A_SIGN]   = _PKA_RAM_ONCURVE_A_SIGN,
		[COEFF_A]  = _PKA_RAM_ONCURVE_A,
		[COEFF_B]  = _PKA_RAM_ONCURVE_B,
		[PRIME_N]  = 0,
		[VAL_P]    = _PKA_RAM_ONCURVE_P,
		[GPOINT_X] = 0,
		[GPOINT_Y] = 0,
	},
};

struct curve_parameters {
	uint32_t a_sign;          /* 0 positive, 1 negative */
	struct stm32_pka_bn a;    /* Curve coefficient |a| */
	struct stm32_pka_bn b;    /* Curve coefficient b */
	struct stm32_pka_bn p;    /* Curve modulus value */
	uint32_t p_len;           /* We also need the p len in bits */
	struct stm32_pka_point g; /* Curve base G point */
	struct stm32_pka_bn n;    /* Curve prime order n */
	uint32_t n_len;           /* We also need the n len in bits */
};

static const struct curve_parameters curve_def[] = {
	[PKA_NIST_P192] = {
		.p_len = 192U,
		.p = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xfe,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff},
			.size = 24U,
		},
		.n_len = 192U,
		.n = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0x99, 0xde, 0xf8, 0x36,
					  0x14, 0x6b, 0xc9, 0xb1,
					  0xb4, 0xd2, 0x28, 0x31},
			.size = 24U,
		},
		.a_sign = 1U,
		.a = { .val = (uint8_t[]){0x03},
			.size = 1U,
		},
		.b = { .val = (uint8_t[]){0x64, 0x21, 0x05, 0x19,
					  0xe5, 0x9c, 0x80, 0xe7,
					  0x0f, 0xa7, 0xe9, 0xab,
					  0x72, 0x24, 0x30, 0x49,
					  0xfe, 0xb8, 0xde, 0xec,
					  0xc1, 0x46, 0xb9, 0xb1},
			.size = 24U,
		},
		.g = {
			.x = { .val = (uint8_t[]){0x18, 0x8d, 0xa8, 0x0e,
						  0xb0, 0x30, 0x90, 0xf6,
						  0x7c, 0xbf, 0x20, 0xeb,
						  0x43, 0xa1, 0x88, 0x00,
						  0xf4, 0xff, 0x0a, 0xfd,
						  0x82, 0xff, 0x10, 0x12},
				.size = 24U,
			},
			.y = { .val = (uint8_t[]){0x07, 0x19, 0x2b, 0x95,
						  0xff, 0xc8, 0xda, 0x78,
						  0x63, 0x10, 0x11, 0xed,
						  0x6b, 0x24, 0xcd, 0xd5,
						  0x73, 0xf9, 0x77, 0xa1,
						  0x1e, 0x79, 0x48, 0x11},
				.size = 24U,
			},
		},
	},
	[PKA_NIST_P224] = {
		.p_len = 224U,
		.p = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x01},
			.size = 28U,
		},
		.n_len = 224U,
		.n = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0x16, 0xa2,
					  0xe0, 0xb8, 0xf0, 0x3e,
					  0x13, 0xdd, 0x29, 0x45,
					  0x5c, 0x5c, 0x2a, 0x3d},
			.size = 28U,
		},
		.a_sign = 1U,
		.a = { .val = (uint8_t[]){0x03},
			.size = 1U,
		},
		.b = { .val = (uint8_t[]){0xb4, 0x05, 0x0a, 0x85,
					  0x0c, 0x04, 0xb3, 0xab,
					  0xf5, 0x41, 0x32, 0x56,
					  0x50, 0x44, 0xb0, 0xb7,
					  0xd7, 0xbf, 0xd8, 0xba,
					  0x27, 0x0b, 0x39, 0x43,
					  0x23, 0x55, 0xff, 0xb4},
			.size = 28U,
		},
		.g = {
			.x = { .val = (uint8_t[]){0xb7, 0x0e, 0x0c, 0xbd,
						  0x6b, 0xb4, 0xbf, 0x7f,
						  0x32, 0x13, 0x90, 0xb9,
						  0x4a, 0x03, 0xc1, 0xd3,
						  0x56, 0xc2, 0x11, 0x22,
						  0x34, 0x32, 0x80, 0xd6,
						  0x11, 0x5c, 0x1d, 0x21},
				.size = 28U,
			},
			.y = { .val = (uint8_t[]){0xbd, 0x37, 0x63, 0x88,
						  0xb5, 0xf7, 0x23, 0xfb,
						  0x4c, 0x22, 0xdf, 0xe6,
						  0xcd, 0x43, 0x75, 0xa0,
						  0x5a, 0x07, 0x47, 0x64,
						  0x44, 0xd5, 0x81, 0x99,
						  0x85, 0x00, 0x7e, 0x34},
				.size = 28U,
			},
		},
	},
	[PKA_NIST_P256] = {
		.p_len = 256U,
		.p = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0x00, 0x00, 0x00, 0x01,
					  0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff},
			.size = 32U,
		},
		.n_len = 256U,
		.n = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0x00, 0x00, 0x00, 0x00,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xbc, 0xe6, 0xfa, 0xad,
					  0xa7, 0x17, 0x9e, 0x84,
					  0xf3, 0xb9, 0xca, 0xc2,
					  0xfc, 0x63, 0x25, 0x51},
			.size = 32U,
		},
		.a_sign = 1U,
		.a = { .val = (uint8_t[]){0x03},
			.size = 1U,
		},
		.b = { .val = (uint8_t[]){0x5a, 0xc6, 0x35, 0xd8,
					  0xaa, 0x3a, 0x93, 0xe7,
					  0xb3, 0xeb, 0xbd, 0x55,
					  0x76, 0x98, 0x86, 0xbc,
					  0x65, 0x1d, 0x06, 0xb0,
					  0xcc, 0x53, 0xb0, 0xf6,
					  0x3b, 0xce, 0x3c, 0x3e,
					  0x27, 0xd2, 0x60, 0x4b},
			.size = 32U,
		},
		.g = {
			.x = { .val = (uint8_t[]){0x6b, 0x17, 0xd1, 0xf2,
						  0xe1, 0x2c, 0x42, 0x47,
						  0xf8, 0xbc, 0xe6, 0xe5,
						  0x63, 0xa4, 0x40, 0xf2,
						  0x77, 0x03, 0x7d, 0x81,
						  0x2d, 0xeb, 0x33, 0xa0,
						  0xf4, 0xa1, 0x39, 0x45,
						  0xd8, 0x98, 0xc2, 0x96},
				.size = 32U,
			},
			.y = { .val = (uint8_t[]){0x4f, 0xe3, 0x42, 0xe2,
						  0xfe, 0x1a, 0x7f, 0x9b,
						  0x8e, 0xe7, 0xeb, 0x4a,
						  0x7c, 0x0f, 0x9e, 0x16,
						  0x2b, 0xce, 0x33, 0x57,
						  0x6b, 0x31, 0x5e, 0xce,
						  0xcb, 0xb6, 0x40, 0x68,
						  0x37, 0xbf, 0x51, 0xf5},
				.size = 32U,
			},
		},
	},
	[PKA_NIST_P384] = {
		.p_len = 384U,
		.p = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xfe,
					  0xff, 0xff, 0xff, 0xff,
					  0x00, 0x00, 0x00, 0x00,
					  0x00, 0x00, 0x00, 0x00,
					  0xff, 0xff, 0xff, 0xff},
			.size = 48U,
		},
		.n_len = 384U,
		.n = { .val = (uint8_t[]){0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xc7, 0x63, 0x4d, 0x81,
					  0xf4, 0x37, 0x2d, 0xdf,
					  0x58, 0x1a, 0x0d, 0xb2,
					  0x48, 0xb0, 0xa7, 0x7a,
					  0xec, 0xec, 0x19, 0x6a,
					  0xcc, 0xc5, 0x29, 0x73},
			.size = 48U,
		},
		.a_sign = 1U,
		.a = { .val = (uint8_t[]){0x03},
			.size = 1U,
		},
		.b = { .val = (uint8_t[]){0xb3, 0x31, 0x2f, 0xa7,
					  0xe2, 0x3e, 0xe7, 0xe4,
					  0x98, 0x8e, 0x05, 0x6b,
					  0xe3, 0xf8, 0x2d, 0x19,
					  0x18, 0x1d, 0x9c, 0x6e,
					  0xfe, 0x81, 0x41, 0x12,
					  0x03, 0x14, 0x08, 0x8f,
					  0x50, 0x13, 0x87, 0x5a,
					  0xc6, 0x56, 0x39, 0x8d,
					  0x8a, 0x2e, 0xd1, 0x9d,
					  0x2a, 0x85, 0xc8, 0xed,
					  0xd3, 0xec, 0x2a, 0xef},
			.size = 48U,
		},
		.g = {
			.x = { .val = (uint8_t[]){0xaa, 0x87, 0xca, 0x22,
						  0xbe, 0x8b, 0x05, 0x37,
						  0x8e, 0xb1, 0xc7, 0x1e,
						  0xf3, 0x20, 0xad, 0x74,
						  0x6e, 0x1d, 0x3b, 0x62,
						  0x8b, 0xa7, 0x9b, 0x98,
						  0x59, 0xf7, 0x41, 0xe0,
						  0x82, 0x54, 0x2a, 0x38,
						  0x55, 0x02, 0xf2, 0x5d,
						  0xbf, 0x55, 0x29, 0x6c,
						  0x3a, 0x54, 0x5e, 0x38,
						  0x72, 0x76, 0x0a, 0xb7},
				.size = 48U,
			},
			.y = { .val = (uint8_t[]){0x36, 0x17, 0xde, 0x4a,
						  0x96, 0x26, 0x2c, 0x6f,
						  0x5d, 0x9e, 0x98, 0xbf,
						  0x92, 0x92, 0xdc, 0x29,
						  0xf8, 0xf4, 0x1d, 0xbd,
						  0x28, 0x9a, 0x14, 0x7c,
						  0xe9, 0xda, 0x31, 0x13,
						  0xb5, 0xf0, 0xb8, 0xc0,
						  0x0a, 0x60, 0xb1, 0xce,
						  0x1d, 0x7e, 0x81, 0x9d,
						  0x7a, 0x43, 0x1d, 0x7c,
						  0x90, 0xea, 0x0e, 0x5f},
				.size = 48U,
			},
		},
	},
	[PKA_NIST_P521] = {
		.p_len = 521U,
		.p = { .val = (uint8_t[]){            0x01, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff},
			.size = 66U,
		},
		.n_len = 521U,
		.n = { .val = (uint8_t[]){            0x01, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xff,
					  0xff, 0xff, 0xff, 0xfa,
					  0x51, 0x86, 0x87, 0x83,
					  0xbf, 0x2f, 0x96, 0x6b,
					  0x7f, 0xcc, 0x01, 0x48,
					  0xf7, 0x09, 0xa5, 0xd0,
					  0x3b, 0xb5, 0xc9, 0xb8,
					  0x89, 0x9c, 0x47, 0xae,
					  0xbb, 0x6f, 0xb7, 0x1e,
					  0x91, 0x38, 0x64, 0x09},
			.size = 66U,
		},
		.a_sign = 1U,
		.a = { .val = (uint8_t[]){0x03},
			.size = 1U,
		},
		.b = { .val = (uint8_t[]){                  0x51,
					  0x95, 0x3e, 0xb9, 0x61,
					  0x8e, 0x1c, 0x9a, 0x1f,
					  0x92, 0x9a, 0x21, 0xa0,
					  0xb6, 0x85, 0x40, 0xee,
					  0xa2, 0xda, 0x72, 0x5b,
					  0x99, 0xb3, 0x15, 0xf3,
					  0xb8, 0xb4, 0x89, 0x91,
					  0x8e, 0xf1, 0x09, 0xe1,
					  0x56, 0x19, 0x39, 0x51,
					  0xec, 0x7e, 0x93, 0x7b,
					  0x16, 0x52, 0xc0, 0xbd,
					  0x3b, 0xb1, 0xbf, 0x07,
					  0x35, 0x73, 0xdf, 0x88,
					  0x3d, 0x2c, 0x34, 0xf1,
					  0xef, 0x45, 0x1f, 0xd4,
					  0x6b, 0x50, 0x3f, 0x00},
			.size = 65U,
		},
		.g = {
			.x = { .val = (uint8_t[]){                  0xc6,
						  0x85, 0x8e, 0x06, 0xb7,
						  0x04, 0x04, 0xe9, 0xcd,
						  0x9e, 0x3e, 0xcb, 0x66,
						  0x23, 0x95, 0xb4, 0x42,
						  0x9c, 0x64, 0x81, 0x39,
						  0x05, 0x3f, 0xb5, 0x21,
						  0xf8, 0x28, 0xaf, 0x60,
						  0x6b, 0x4d, 0x3d, 0xba,
						  0xa1, 0x4b, 0x5e, 0x77,
						  0xef, 0xe7, 0x59, 0x28,
						  0xfe, 0x1d, 0xc1, 0x27,
						  0xa2, 0xff, 0xa8, 0xde,
						  0x33, 0x48, 0xb3, 0xc1,
						  0x85, 0x6a, 0x42, 0x9b,
						  0xf9, 0x7e, 0x7e, 0x31,
						  0xc2, 0xe5, 0xbd, 0x66},
				.size = 65U,
			},
			.y = { .val = (uint8_t[]){            0x01, 0x18,
						  0x39, 0x29, 0x6a, 0x78,
						  0x9a, 0x3b, 0xc0, 0x04,
						  0x5c, 0x8a, 0x5f, 0xb4,
						  0x2c, 0x7d, 0x1b, 0xd9,
						  0x98, 0xf5, 0x44, 0x49,
						  0x57, 0x9b, 0x44, 0x68,
						  0x17, 0xaf, 0xbd, 0x17,
						  0x27, 0x3e, 0x66, 0x2c,
						  0x97, 0xee, 0x72, 0x99,
						  0x5e, 0xf4, 0x26, 0x40,
						  0xc5, 0x50, 0xb9, 0x01,
						  0x3f, 0xad, 0x07, 0x61,
						  0x35, 0x3c, 0x70, 0x86,
						  0xa2, 0x72, 0xc2, 0x40,
						  0x88, 0xbe, 0x94, 0x76,
						  0x9f, 0xd1, 0x66, 0x50},
				.size = 66U,
			},
		},
	},
};

static struct stm32_pka_platdata pka_pdata;
static struct mutex pka_lock = MUTEX_INITIALIZER;

static TEE_Result pka_wait_bit(const vaddr_t base, const uint32_t bit)
{
	uint64_t timeout = timeout_init_us(PKA_TIMEOUT_US);

	while ((io_read32(base + _PKA_SR) & bit) != bit)
		if (timeout_elapsed(timeout))
			break;

	if ((io_read32(base + _PKA_SR) & bit) != bit) {
		DMSG("timeout waiting 0x%x", bit);
		return TEE_ERROR_BUSY;
	}

	return TEE_SUCCESS;
}

static void pka_disable(const vaddr_t base)
{
	io_clrbits32(base + _PKA_CR, _PKA_CR_EN);
}

static TEE_Result pka_enable(const vaddr_t base, const uint32_t mode)
{
	/* Set mode and disable interrupts */
	io_clrsetbits32(base + _PKA_CR, _PKA_IT_MASK | _PKA_CR_MODE_MASK,
			_PKA_CR_MODE_MASK & (mode << _PKA_CR_MODE_SHIFT));

	io_setbits32(base + _PKA_CR, _PKA_CR_EN);

	return pka_wait_bit(base, _PKA_SR_INITOK);
}

/*
 * Data are already loaded in PKA internal RAM,
 * MODE is set,
 * we start process, and wait for its end.
 */
static TEE_Result stm32_pka_process(const vaddr_t base)
{
	io_setbits32(base + _PKA_CR, _PKA_CR_START);

	return pka_wait_bit(base, _PKA_IT_PROCEND);
}

/**
 * @brief  Read ECC operand from PKA RAM.
 * @note  PKA expects to read u64 word, each u64 are: the least significant bit
 *        is bit 0; the most significant bit is bit 63.
 *        We read eo_nbw (ECC operand Size) u64, value that depends on the
 *        chosen prime modulus length in bits.
 *        First less significant u64 is read from low address.
 *        Most significant u64 from higher address.
 *        And at last address we expect to read a u64(0x0).
 * @note This function doesn't only manage endianness (as bswap64 do), but also
 *       complete most significant incomplete u64 with 0 (if data is not a u64
 *       multiple), and check last u64 is 0.
 * @param addr: PKA_RAM address to read from to the buffer 'data'.
 * @param data[out]: will be a BYTE list with most significant bytes first.
 * @param data_size[in/out]: [in]data size,
 *                           [out]]nb of bytes in data.
 * @param eo_nbw: is ECC Operand size in 64bits word (including the extra 0)
 *                (note it depends on the prime modulus length, not the data
 *                size).
 * @retval TEE_SUCCESS if OK.
 *         TEE_ERROR_BAD_PARAMETERS if data_size and eo_nbw are inconsistent,
 *         i.e. data doesn't fit in defined eo_nbw, or eo_nbw bigger than
 *         hardware limit, or if [in]data_size is too small to get the data.
 */
static TEE_Result read_eo_data(const vaddr_t addr, uint8_t *data,
			       const unsigned int data_size,
			       const unsigned int eo_nbw)
{
	uint32_t word_index = 0;
	int data_index = (int)data_size - 1;
	uint64_t tmp = 0ULL;

	if (eo_nbw < OP_NBW_FROM_SIZE(data_size) || eo_nbw > MAX_EO_NBW)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Fill value */
	for (word_index = 0U; word_index < eo_nbw - 1; word_index++) {
		unsigned int i = 0U; /* Index in the tmp U64 word */

		tmp = io_read64(addr + word_index * sizeof(tmp));

		while ((i < sizeof(tmp)) && (data_index >= 0)) {
			data[data_index] = tmp & 0xFF;
			tmp = tmp >> INT8_LEN;
			i++; /* Move byte index in current (u64)tmp */
			data_index--; /* Move to next most significant byte */
		}
	}

	/* The last u64 should be 0 */
	tmp = io_read64(addr + word_index * sizeof(tmp));
	if (tmp)
		return TEE_ERROR_SECURITY;

	return TEE_SUCCESS;
}

/**
 * @brief  Write ECC operand to PKA RAM.
 * @note  PKA expects to write u64 word, each u64 are: the least significant bit
 *        is bit 0; the most significant bit is bit 63.
 *        We write eo_nbw (ECC operand Size) u64, value that depends on the
 *        chosen prime modulus length in bits.
 *        First less significant u64 is written to low address.
 *        Most significant u64 to higher address.
 *        And at last address we write a u64(0x0).
 * @note This function doesn't only manage endianness (as bswap64 do), but also
 *       complete most significant incomplete u64 with 0 (if data is not a u64
 *       multiple), and fill u64 last address with 0.
 * @param addr: PKA_RAM address to write the buffer 'data'.
 * @param data: is a BYTE list with most significant bytes first.
 * @param data_size: nb of bytes in data.
 * @param eo_nbw: is ECC Operand size in 64bits word (including the extra 0)
 *                (note it depends on the prime modulus length, not the data
 *                size).
 * @retval TEE_SUCCESS if OK.
 *         TEE_ERROR_BAD_PARAMETERS if data_size and eo_nbw are inconsistent,
 *         ie data doesn't fit in defined eo_nbw, or eo_nbw bigger than hardware
 *         limit.
 */
static TEE_Result write_eo_data(const vaddr_t addr, const uint8_t *data,
				const unsigned int data_size,
				const unsigned int eo_nbw)
{
	uint32_t word_index = 0;
	int data_index = (int)data_size - 1;

	if (eo_nbw < OP_NBW_FROM_SIZE(data_size) || eo_nbw > MAX_EO_NBW)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Fill value */
	for (word_index = 0U; word_index < eo_nbw; word_index++) {
		uint64_t tmp = 0ULL;
		unsigned int i = 0U; /* Index in the tmp U64 word */

		/* Stop if end of tmp or end of data */
		while ((i < sizeof(tmp)) && (data_index >= 0)) {
			tmp |= (uint64_t)(data[data_index]) << (INT8_LEN * i);
			i++; /* Move byte index in current (u64)tmp */
			data_index--; /* Move to next most significant byte */
		}

		io_write64(addr + word_index * sizeof(tmp), tmp);
	}

	return TEE_SUCCESS;
}

static unsigned int get_ecc_op_nbword(const enum stm32_pka_curve_id cid)
{
	if (cid < 0 || cid >= PKA_LAST_CID)
		return 0;

	return OP_NBW_FROM_LEN(curve_def[cid].n_len);
}

static TEE_Result stm32_pka_configure_curve(const vaddr_t base, const int op,
					    const enum stm32_pka_curve_id cid)
{
	TEE_Result res = TEE_SUCCESS;
	unsigned int eo_nbw = get_ecc_op_nbword(cid);

	io_write64(base + pka_ram[op][N_LEN], curve_def[cid].n_len);
	if (pka_ram[op][P_LEN])
		io_write64(base + pka_ram[op][P_LEN], curve_def[cid].p_len);
	io_write64(base + pka_ram[op][A_SIGN], curve_def[cid].a_sign);

	res = write_eo_data(base + pka_ram[op][COEFF_A], curve_def[cid].a.val,
			    curve_def[cid].a.size, eo_nbw);
	if (res)
		return res;

	if (pka_ram[op][COEFF_B]) {
		res = write_eo_data(base + pka_ram[op][COEFF_B],
				    curve_def[cid].b.val, curve_def[cid].b.size,
				    eo_nbw);
		if (res)
			return res;
	}

	if (pka_ram[op][PRIME_N]) {
		res = write_eo_data(base + pka_ram[op][PRIME_N],
				    curve_def[cid].n.val, curve_def[cid].n.size,
				    eo_nbw);
		if (res)
			return res;
	}

	res = write_eo_data(base + pka_ram[op][VAL_P], curve_def[cid].p.val,
			    curve_def[cid].p.size, eo_nbw);
	if (res)
		return res;

	if (pka_ram[op][GPOINT_X]) {
		res = write_eo_data(base + pka_ram[op][GPOINT_X],
				    curve_def[cid].g.x.val,
				    curve_def[cid].g.x.size, eo_nbw);
		if (res)
			return res;
	}

	if (pka_ram[op][GPOINT_Y]) {
		res = write_eo_data(base + pka_ram[op][GPOINT_Y],
				    curve_def[cid].g.y.val,
				    curve_def[cid].g.y.size, eo_nbw);
		if (res)
			return res;
	}

	return TEE_SUCCESS;
}

/**
 * @brief Check if BigInt stored in d is 0
 *
 * @retval: true: if data represents a 0 value (i.e. all bytes == 0)
 *          false: if data represents a non-zero value.
 */
static bool is_zero(const struct stm32_pka_bn *d)
{
	unsigned int i = 0U;

	if (!d)
		return true;

	for (i = 0U; i < d->size; i++)
		if (d->val[i] != 0U)
			return false;

	return true;
}

/**
 * @brief Compare two BigInt:
 *
 * @retval: true if a < b
 *          false if a >= b
 */
static bool is_smaller(const struct stm32_pka_bn *a,
		       const struct stm32_pka_bn *b)
{
	unsigned int i = MAX(a->size, b->size) + 1;

	do {
		uint8_t _a = 0;
		uint8_t _b = 0;

		i--;
		if (a->size < i)
			_a = 0U;
		else
			_a = a->val[a->size - i];

		if (b->size < i)
			_b = 0U;
		else
			_b = b->val[b->size - i];

		if (_a < _b)
			return true;

		if (_a > _b)
			return false;

	} while (i != 1U);

	return false;
}

TEE_Result stm32_pka_get_max_size(size_t *bytes, size_t *bits,
				  const enum stm32_pka_curve_id cid)
{
	if (cid < 0 || cid >= PKA_LAST_CID)
		return TEE_ERROR_NOT_SUPPORTED;

	if (bits)
		*bits = curve_def[cid].n_len;

	if (bytes)
		*bytes = curve_def[cid].n.size;

	return TEE_SUCCESS;
}

static TEE_Result stm32_pka_compute_r2modn_ret(const vaddr_t base,
					       struct stm32_pka_bn *v,
					       const unsigned int eo_nbw)
{
	uint32_t sr = 0;

	sr = io_read32(base + _PKA_SR);
	if ((sr & (_PKA_IT_OPERR | _PKA_IT_ADDRERR | _PKA_IT_RAMERR)) != 0) {
		IMSG("Detected error(s): %s%s%s",
		     (sr & _PKA_IT_OPERR) ? "Operation " : "",
		     (sr & _PKA_IT_ADDRERR) ? "Address " : "",
		     (sr & _PKA_IT_RAMERR) ? "RAM" : "");
		return TEE_ERROR_SECURITY;
	}

	return read_eo_data(base + _PKA_RAM_R2MODN_OUT, v->val, v->size,
			    eo_nbw);
}

TEE_Result stm32_pka_compute_montgomery(const struct stm32_pka_bn *n,
					const size_t n_len,
					struct stm32_pka_bn *r2modn)
{
	TEE_Result res = TEE_SUCCESS;
	vaddr_t base = io_pa_or_va(&pka_pdata.pa_or_va, 1);
	unsigned int eo_nbw = OP_NBW_FROM_LEN(n_len);

	if (!n_len || !n || !r2modn)
		return TEE_ERROR_BAD_PARAMETERS;

	mutex_lock(pka_pdata.lock);

	if ((io_read32(base + _PKA_SR) & _PKA_SR_BUSY) == _PKA_SR_BUSY) {
		FMSG("busy");
		res = TEE_ERROR_BUSY;
		goto out;
	}

	/* Fill PKA RAM */
	/*    with n_len */
	io_write64(base + _PKA_RAM_R2MODN_N_LEN, n_len);

	/*    with n */
	res = write_eo_data(base + _PKA_RAM_R2MODN_PRIME_N, n->val, n->size,
			    eo_nbw);
	if (res)
		goto out;

	/* Set mode to Montgomery parameter computation */
	res = pka_enable(base, _PKA_CR_MODE_R2MODN);
	if (res) {
		IMSG("Set mode pka error %d", res);
		goto out;
	}

	/* Start processing and wait end */
	res = stm32_pka_process(base);
	if (res) {
		IMSG("process error %d", res);
		goto out;
	}

	/* Get return value */
	res = stm32_pka_compute_r2modn_ret(base, r2modn, eo_nbw);

	/* Unset end proc */
	io_setbits32(base + _PKA_CLRFR, _PKA_IT_PROCEND);

out:
	/* Disable PKA (will stop all pending process and reset RAM) */
	pka_disable(base);

	mutex_unlock(pka_pdata.lock);

	return res;
}

TEE_Result stm32_pka_ecc_compute_montgomery(struct stm32_pka_bn *r2modn,
					    const enum stm32_pka_curve_id cid)
{
	return stm32_pka_compute_montgomery(&curve_def[cid].p,
					    curve_def[cid].p_len, r2modn);
}

static TEE_Result stm32_pka_is_point_on_param(const struct stm32_pka_point *p,
					      enum stm32_pka_curve_id cid)
{
	/* Check Xp < p */
	if (!is_smaller(&p->x, &curve_def[cid].p)) {
		IMSG("Xp < p inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	/* Check Yp < p */
	if (!is_smaller(&p->y, &curve_def[cid].p)) {
		IMSG("Yp < p inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_pka_is_point_on_curve_ret(const vaddr_t base)
{
	uint64_t value = 0;
	uint32_t sr = 0;

	sr = io_read32(base + _PKA_SR);
	if ((sr & (_PKA_IT_OPERR | _PKA_IT_ADDRERR | _PKA_IT_RAMERR)) != 0) {
		IMSG("Detected error(s): %s%s%s",
		     (sr & _PKA_IT_OPERR) ? "Operation " : "",
		     (sr & _PKA_IT_ADDRERR) ? "Address " : "",
		     (sr & _PKA_IT_RAMERR) ? "RAM" : "");
		return TEE_ERROR_SECURITY;
	}

	value = io_read64(base + _PKA_RAM_ONCURVE_RES);
	if (value == _PKA_RAM_ONCURVE_RES_YES)
		return TEE_SUCCESS;
	else
		return TEE_ERROR_GENERIC;
}

TEE_Result stm32_pka_is_point_on_curve(const struct stm32_pka_point *p,
				       const struct stm32_pka_bn *r2modn,
				       const enum stm32_pka_curve_id cid)
{
	TEE_Result res = TEE_SUCCESS;
	vaddr_t base = io_pa_or_va(&pka_pdata.pa_or_va, 1);
	unsigned int eo_nbw = get_ecc_op_nbword(cid);

	if (!eo_nbw || !p)
		return TEE_ERROR_BAD_PARAMETERS;

	mutex_lock(pka_pdata.lock);

	res = stm32_pka_is_point_on_param(p, cid);
	if (res) {
		FMSG("check param error %d", res);
		goto out;
	}

	if ((io_read32(base + _PKA_SR) & _PKA_SR_BUSY) == _PKA_SR_BUSY) {
		FMSG("busy");
		res = TEE_ERROR_BUSY;
		goto out;
	}

	/* Fill PKA RAM */
	/*    With curve id values */
	res = stm32_pka_configure_curve(base, on_curve, cid);
	if (res)
		goto out;

	/*    With Montgomery parameter R*R mod n */
	res = write_eo_data(base + _PKA_RAM_ONCURVE_R2MODN, r2modn->val,
			    r2modn->size, eo_nbw);
	if (res)
		goto out;

	/*    With P */
	res = write_eo_data(base + _PKA_RAM_ONCURVE_XP, p->x.val, p->x.size,
			    eo_nbw);
	if (res)
		goto out;

	res = write_eo_data(base + _PKA_RAM_ONCURVE_YP, p->y.val, p->y.size,
			    eo_nbw);
	if (res)
		goto out;

	/* Set mode to point on the curve check */
	res = pka_enable(base, _PKA_CR_MODE_POINT_CHECK);
	if (res) {
		IMSG("Set mode pka error %d", res);
		goto out;
	}

	/* Start processing and wait end */
	res = stm32_pka_process(base);
	if (res) {
		IMSG("process error %d", res);
		goto out;
	}

	/* Get return value */
	res = stm32_pka_is_point_on_curve_ret(base);

	/* Unset end proc */
	io_setbits32(base + _PKA_CLRFR, _PKA_IT_PROCEND);

out:
	/* Disable PKA (will stop all pending process and reset RAM) */
	pka_disable(base);

	mutex_unlock(pka_pdata.lock);

	return res;
}

static TEE_Result stm32_pka_ecdsa_verif_param(const struct stm32_pka_bn *sig_r,
					      const struct stm32_pka_bn *sig_s,
					      const struct stm32_pka_point *pk,
					      const enum stm32_pka_curve_id cid)
{
	/* Public Key check */
	/* Check Xq < p */
	if (!is_smaller(&pk->x, &curve_def[cid].p)) {
		IMSG("Xq < p inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	/* Check Yq < p */
	if (!is_smaller(&pk->y, &curve_def[cid].p)) {
		IMSG("Yq < p inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	/* Signature check */
	/* Check 0 < r < n */
	if (!is_smaller(sig_r, &curve_def[cid].n) && is_zero(sig_r)) {
		IMSG("0 < r < n inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	/* Check 0 < s < n */
	if (!is_smaller(sig_s, &curve_def[cid].n) && is_zero(sig_s)) {
		IMSG("0 < s < n inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_pka_ecdsa_verif_ret(const vaddr_t base)
{
	uint64_t value = 0;
	uint32_t sr = 0;

	sr = io_read32(base + _PKA_SR);
	if ((sr & (_PKA_IT_OPERR | _PKA_IT_ADDRERR | _PKA_IT_RAMERR)) != 0) {
		IMSG("Detected error(s): %s%s%s",
		     (sr & _PKA_IT_OPERR) ? "Operation " : "",
		     (sr & _PKA_IT_ADDRERR) ? "Address " : "",
		     (sr & _PKA_IT_RAMERR) ? "RAM" : "");
		return TEE_ERROR_SECURITY;
	}

	value = io_read64(base + _PKA_RAM_VERIF_RES);
	if (value == _PKA_RAM_VERIF_RES_VALID)
		return TEE_SUCCESS;

	if (value == _PKA_RAM_VERIF_RES_INVALID)
		return TEE_ERROR_SIGNATURE_INVALID;

	return TEE_ERROR_BAD_PARAMETERS;
}

TEE_Result stm32_pka_ecdsa_verif(const void *hash, unsigned int hash_size,
				 const struct stm32_pka_bn *sig_r,
				 const struct stm32_pka_bn *sig_s,
				 const struct stm32_pka_point *pk,
				 const enum stm32_pka_curve_id cid)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t n_len_bytes = curve_def[cid].n_len / INT8_LEN;
	vaddr_t base = io_pa_or_va(&pka_pdata.pa_or_va, 1);
	unsigned int eo_nbw = get_ecc_op_nbword(cid);

	if (!eo_nbw || !hash || !sig_r || !sig_s || !pk)
		return TEE_ERROR_BAD_PARAMETERS;

	mutex_lock(pka_pdata.lock);

	res = stm32_pka_ecdsa_verif_param(sig_r, sig_s, pk, cid);
	if (res) {
		FMSG("check param error %d", res);
		goto out;
	}

	if ((io_read32(base + _PKA_SR) & _PKA_SR_BUSY) == _PKA_SR_BUSY) {
		FMSG("busy");
		res = TEE_ERROR_BUSY;
		goto out;
	}

	/* Fill PKA RAM */
	/*    With curve id values */
	res = stm32_pka_configure_curve(base, verif, cid);
	if (res)
		goto out;

	/*    With pubkey */
	res = write_eo_data(base + _PKA_RAM_VERIF_XQ, pk->x.val, pk->x.size,
			    eo_nbw);
	if (res)
		goto out;

	res = write_eo_data(base + _PKA_RAM_VERIF_YQ, pk->y.val, pk->y.size,
			    eo_nbw);
	if (res)
		goto out;

	/*    With hash */
	if (n_len_bytes < hash_size) {
		/*
		 * Hash size is greater than ECDSA prime curve size.
		 * Truncate hash and use leftmost bits of the hash.
		 * NIST.FIPS.186-5.pdf
		 */
		hash_size = n_len_bytes;
	}
	res = write_eo_data(base + _PKA_RAM_VERIF_HASH_Z, hash, hash_size,
			    eo_nbw);
	if (res)
		goto out;

	/*    With signature */
	res = write_eo_data(base + _PKA_RAM_VERIF_SIGN_R, sig_r->val,
			    sig_r->size, eo_nbw);
	if (res)
		goto out;

	res = write_eo_data(base + _PKA_RAM_VERIF_SIGN_S, sig_s->val,
			    sig_s->size, eo_nbw);
	if (res)
		goto out;

	/* Set mode to ECDSA signature verification */
	res = pka_enable(base, _PKA_CR_MODE_ECDSA_VERIF);
	if (res) {
		IMSG("set mode pka error %d", res);
		goto out;
	}

	/* Start processing and wait end */
	res = stm32_pka_process(base);
	if (res) {
		IMSG("process error %d", res);
		goto out;
	}

	/* Check return status */
	res = stm32_pka_ecdsa_verif_ret(base);

	/* Unset end proc */
	io_setbits32(base + _PKA_CLRFR, _PKA_IT_PROCEND);

out:
	/* Disable PKA (will stop all pending process and reset RAM) */
	pka_disable(base);

	mutex_unlock(pka_pdata.lock);

	return res;
}

static TEE_Result stm32_pka_ecdsa_sign_param(const struct stm32_pka_bn *k)
{
	if (is_zero(k) || k->size > PKA_MAX_ECC_SIZE) {
		IMSG("0 < k < 2**640 inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_pka_ecdsa_sign_ret(const vaddr_t base,
					   struct stm32_pka_bn *sig_r,
					   struct stm32_pka_bn *sig_s,
					   const unsigned int eo_nbw)
{
	TEE_Result res = TEE_SUCCESS;
	uint64_t value = 0;
	uint32_t sr = 0;

	sr = io_read32(base + _PKA_SR);
	if ((sr & (_PKA_IT_OPERR | _PKA_IT_ADDRERR | _PKA_IT_RAMERR)) != 0) {
		IMSG("Detected error(s): %s%s%s",
		     (sr & _PKA_IT_OPERR) ? "Operation " : "",
		     (sr & _PKA_IT_ADDRERR) ? "Address " : "",
		     (sr & _PKA_IT_RAMERR) ? "RAM" : "");
		return TEE_ERROR_SECURITY;
	}

	value = io_read64(base + _PKA_RAM_SIGN_RES);

	if (value == _PKA_RAM_SIGN_RES_FAIL)
		return TEE_ERROR_SECURITY;

	if (value == _PKA_RAM_SIGN_RES_R0) {
		value = _PKA_RAM_SIGN_RES_SUCCESS;
		memset(sig_r->val, 0, sig_r->size);
	} else {
		res = read_eo_data(base + _PKA_RAM_SIGN_R, sig_r->val,
				   sig_r->size, eo_nbw);
		if (res)
			return res;
	}

	if (value == _PKA_RAM_SIGN_RES_S0) {
		value = _PKA_RAM_SIGN_RES_SUCCESS;
		memset(sig_s->val, 0, sig_s->size);
	} else {
		res = read_eo_data(base + _PKA_RAM_SIGN_S, sig_s->val,
				   sig_s->size, eo_nbw);
		if (res)
			return res;
	}

	if (value != _PKA_RAM_SIGN_RES_SUCCESS)
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}

TEE_Result stm32_pka_ecdsa_sign(const void *hash, unsigned int hash_size,
				struct stm32_pka_bn *sig_r,
				struct stm32_pka_bn *sig_s,
				const struct stm32_pka_bn *d,
				const struct stm32_pka_bn *k,
				const enum stm32_pka_curve_id cid)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t n_len_bytes = curve_def[cid].n_len / INT8_LEN;
	vaddr_t base = io_pa_or_va(&pka_pdata.pa_or_va, 1);
	unsigned int eo_nbw = get_ecc_op_nbword(cid);

	if (!eo_nbw || !hash || !sig_r || !sig_s || !d || !k)
		return TEE_ERROR_BAD_PARAMETERS;

	mutex_lock(pka_pdata.lock);

	res = stm32_pka_ecdsa_sign_param(k);
	if (res) {
		FMSG("check param error %d", res);
		goto out;
	}

	if ((io_read32(base + _PKA_SR) & _PKA_SR_BUSY) == _PKA_SR_BUSY) {
		FMSG("busy");
		res = TEE_ERROR_BUSY;
		goto out;
	}

	/* Fill PKA RAM */
	/*    With curve id values */
	res = stm32_pka_configure_curve(base, sign, cid);
	if (res)
		goto out;

	/*    With K (random number) */
	res = write_eo_data(base + _PKA_RAM_SIGN_K, k->val, k->size, eo_nbw);
	if (res)
		goto out;

	/*    With private key d */
	res = write_eo_data(base + _PKA_RAM_SIGN_D, d->val, d->size, eo_nbw);
	if (res)
		goto out;

	/*    With hash */
	if (n_len_bytes < hash_size) {
		/*
		 * Hash size is greater than ECDSA prime curve size.
		 * Truncate hash and use leftmost bits of the hash.
		 * NIST.FIPS.186-4.pdf
		 */
		hash_size = n_len_bytes;
	}
	res = write_eo_data(base + _PKA_RAM_SIGN_HASH_Z, hash, hash_size,
			    eo_nbw);
	if (res)
		goto out;

	/* Set mode to ECDSA signature */
	res = pka_enable(base, _PKA_CR_MODE_ECDSA_SIGN);
	if (res) {
		IMSG("Set mode pka error %d", res);
		goto out;
	}

	/* Start processing and wait end */
	res = stm32_pka_process(base);
	if (res) {
		IMSG("process error %d", res);
		goto out;
	}

	/* Get return value */
	res = stm32_pka_ecdsa_sign_ret(base, sig_r, sig_s, eo_nbw);

	/* Unset end proc */
	io_setbits32(base + _PKA_CLRFR, _PKA_IT_PROCEND);

out:
	/* Disable PKA (will stop all pending process and reset RAM) */
	pka_disable(base);

	mutex_unlock(pka_pdata.lock);

	return res;
}

static TEE_Result stm32_pka_ecc_sc_mul_param(const struct stm32_pka_bn *k,
					     const struct stm32_pka_point *p,
					     const enum stm32_pka_curve_id cid)
{
	if (is_zero(k) || k->size > PKA_MAX_ECC_SIZE) {
		IMSG("0 < k < 2**640 inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (!is_smaller(&p->x, &curve_def[cid].p)) {
		IMSG("Xp < p inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	if (!is_smaller(&p->y, &curve_def[cid].p)) {
		IMSG("Yp < p inval");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_pka_ecc_kp_ret(const vaddr_t base,
				       struct stm32_pka_point *kp,
				       const unsigned int eo_nbw)
{
	TEE_Result res = TEE_SUCCESS;
	uint64_t value = 0LLU;
	uint32_t sr = 0U;

	sr = io_read32(base + _PKA_SR);
	if ((sr & (_PKA_IT_OPERR | _PKA_IT_ADDRERR | _PKA_IT_RAMERR)) != 0) {
		IMSG("Detected error(s): %s%s%s",
		     (sr & _PKA_IT_OPERR) ? "Operation " : "",
		     (sr & _PKA_IT_ADDRERR) ? "Address " : "",
		     (sr & _PKA_IT_RAMERR) ? "RAM" : "");
		return TEE_ERROR_SECURITY;
	}

	value = io_read64(base + _PKA_RAM_KP_RES);
	if (value == _PKA_RAM_KP_RES_FAIL)
		return TEE_ERROR_SECURITY;

	if (value != _PKA_RAM_KP_RES_SUCCESS)
		return TEE_ERROR_GENERIC;

	res = read_eo_data(base + _PKA_RAM_KP_X, kp->x.val, kp->x.size, eo_nbw);
	if (res)
		return res;

	res = read_eo_data(base + _PKA_RAM_KP_Y, kp->y.val, kp->y.size, eo_nbw);

	return res;
}

TEE_Result stm32_pka_ecc_scalar_mul(const struct stm32_pka_bn *k,
				    const struct stm32_pka_point *p,
				    struct stm32_pka_point *kp,
				    const enum stm32_pka_curve_id cid)
{
	TEE_Result res = TEE_SUCCESS;
	vaddr_t base = io_pa_or_va(&pka_pdata.pa_or_va, 1);
	unsigned int eo_nbw = get_ecc_op_nbword(cid);

	if (!eo_nbw || !k || !p || !kp)
		return TEE_ERROR_BAD_PARAMETERS;

	mutex_lock(pka_pdata.lock);

	res = stm32_pka_ecc_sc_mul_param(k, p, cid);
	if (res) {
		FMSG("check param error %d", res);
		goto out;
	}

	if ((io_read32(base + _PKA_SR) & _PKA_SR_BUSY) == _PKA_SR_BUSY) {
		FMSG("busy");
		res = TEE_ERROR_BUSY;
		goto out;
	}

	/* Fill PKA RAM */
	/*    With curve id values */
	res = stm32_pka_configure_curve(base, scalar_mul, cid);
	if (res)
		goto out;

	/*    With k */
	res = write_eo_data(base + _PKA_RAM_KP_K, k->val, k->size, eo_nbw);
	if (res)
		goto out;

	/*    With xP */
	res = write_eo_data(base + _PKA_RAM_KP_XP, p->x.val, p->x.size, eo_nbw);
	if (res)
		goto out;

	/*    With yP */
	res = write_eo_data(base + _PKA_RAM_KP_YP, p->y.val, p->y.size, eo_nbw);
	if (res)
		goto out;

	/* Set mode to ecc scalar multiplication */
	res = pka_enable(base, _PKA_CR_MODE_ECC_KP);
	if (res) {
		IMSG("Set mode pka error %d", res);
		goto out;
	}

	/* Start processing and wait end */
	res = stm32_pka_process(base);
	if (res) {
		IMSG("process error %d", res);
		goto out;
	}

	/* Get return value */
	res = stm32_pka_ecc_kp_ret(base, kp, eo_nbw);

	/* Unset end proc */
	io_setbits32(base + _PKA_CLRFR, _PKA_IT_PROCEND);

out:
	/* Disable PKA (will stop all pending process and reset RAM) */
	pka_disable(base);

	mutex_unlock(pka_pdata.lock);

	return res;
}

TEE_Result stm32_pka_edac_gen_pubkey(const struct stm32_pka_bn *k,
				     struct stm32_pka_point *pk,
				     const enum stm32_pka_curve_id cid)
{
	return stm32_pka_ecc_scalar_mul(k, &curve_def[cid].g, pk, cid);
}

#ifdef CFG_EMBED_DTB
static TEE_Result stm32_pka_parse_fdt(struct stm32_pka_platdata *pdata,
				      const void *fdt, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct dt_node_info info = { };

	_fdt_fill_device_info(fdt, &info, node);

	if (info.reg_size == DT_INFO_INVALID_REG_SIZE ||
	    info.reg == DT_INFO_INVALID_REG)
		return TEE_ERROR_BAD_PARAMETERS;

	pdata->pa_or_va.pa = info.reg;
	io_pa_or_va_secure(&pdata->pa_or_va, info.reg_size);
	if (!pdata->pa_or_va.va)
		panic();

	res = rstctrl_dt_get_by_index(fdt, node, 0, &pdata->reset);
	if (res != TEE_SUCCESS && res != TEE_ERROR_ITEM_NOT_FOUND)
		return res;

	res = clk_dt_get_by_name(fdt, node, "bus", &pdata->clk);
	if (res)
		return res;

	res = clk_dt_get_by_name(fdt, node, "rng", &pdata->clk_rng);
	if (res)
		return res;

	return TEE_SUCCESS;
}

__weak
int stm32_pka_get_platdata(struct stm32_pka_platdata *pdata __unused)
{
	/* In DT config, the platform data are filled by DT file */
	return 0;
}
#else /* CFG_EMBED_DTB */
static TEE_Result stm32_pka_parse_fdt(struct stm32_pka_platdata *pdata,
				      const void *fdt, int node)
{
	/* Do nothing, there is no fdt to parse in this case */
	return TEE_SUCCESS;
}

/* This function can be overridden by platform to define pdata of PKA driver */
__weak
int stm32_pka_get_platdata(struct stm32_pka_platdata *pdata __unused)
{
	return -1;
}
#endif

static TEE_Result stm32_pka_reset(void)
{
	TEE_Result ret = TEE_SUCCESS;

	if (!pka_pdata.reset)
		return ret;

	ret = rstctrl_assert_to(pka_pdata.reset, TIMEOUT_US_1MS);
	if (ret)
		return ret;

	udelay(PKA_RESET_DELAY);

	return rstctrl_deassert_to(pka_pdata.reset, TIMEOUT_US_1MS);
}

static TEE_Result stm32_pka_pm(enum pm_op op, uint32_t pm_hint,
			       const struct pm_callback_handle *hdl __unused)
{
	TEE_Result ret = TEE_ERROR_NOT_IMPLEMENTED;

	switch (op) {
	case PM_OP_SUSPEND:
		clk_disable(pka_pdata.clk);
		clk_disable(pka_pdata.clk_rng);
		ret = TEE_SUCCESS;
		break;
	case PM_OP_RESUME:
		if (clk_enable(pka_pdata.clk) ||
		    clk_enable(pka_pdata.clk_rng))
			panic();

		if (PM_HINT_IS_STATE(pm_hint, CONTEXT)) {
			ret = stm32_pka_reset();
			if (ret)
				panic();
		}

		ret = TEE_SUCCESS;
		break;
	default:
		break;
	}

	return ret;
}

/*
 * @brief  Initialize the PKA driver.
 * @param  None.
 * @retval 0 if OK, negative value else.
 */
static TEE_Result stm32_pka_probe(const void *fdt, int node,
				  const void *compat_data __unused)
{
	TEE_Result res = TEE_SUCCESS;
	vaddr_t __unused base = 0;
	uint32_t __unused ver = 0U;
	uint32_t __unused id = 0U;

	if (stm32_pka_get_platdata(&pka_pdata))
		return TEE_ERROR_NOT_SUPPORTED;

	res = stm32_pka_parse_fdt(&pka_pdata, fdt, node);
	if (res)
		return res;

	if (clk_enable(pka_pdata.clk) || clk_enable(pka_pdata.clk_rng))
		panic();

	if (stm32_pka_reset())
		panic();

#if TRACE_LEVEL >= TRACE_FLOW
	base = io_pa_or_va(&pka_pdata.pa_or_va, 1);
	id = io_read32(base + _PKA_IPIDR);
	ver = io_read32(base + _PKA_VERR);

	FMSG("STM32 PKA[%x] V%u.%u", id,
	     (ver & _PKA_VERR_MAJREV_MASK) >> _PKA_VERR_MAJREV_SHIFT,
	     (ver & _PKA_VERR_MINREV_MASK) >> _PKA_VERR_MINREV_SHIFT);
#endif

	pka_pdata.lock = &pka_lock;

	if (IS_ENABLED(CFG_CRYPTO_DRV_ECC)) {
		res = stm32_register_ecc();
		if (res) {
			EMSG("Failed to register to ecc: %#"PRIx32, res);
			panic();
		}
	}

	if (IS_ENABLED(CFG_PM))
		register_pm_core_service_cb(stm32_pka_pm, NULL, "stm32-pka");

	return TEE_SUCCESS;
}

#ifdef CFG_EMBED_DTB
static const struct dt_device_match pka_match_table[] = {
	{ .compatible = "st,stm32mp13-pka64" },
	{ }
};

DEFINE_DT_DRIVER(stm32_pka_dt_driver) = {
	.name = "stm32-pka64",
	.match_table = pka_match_table,
	.probe = &stm32_pka_probe,
};
#endif
