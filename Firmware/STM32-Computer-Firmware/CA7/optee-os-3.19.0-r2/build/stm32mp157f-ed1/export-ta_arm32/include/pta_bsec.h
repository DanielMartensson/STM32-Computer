/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2021, STMicroelectronics - All Rights Reserved
 */

#ifndef __PTA_BSEC_H
#define __PTA_BSEC_H

#define PTA_BSEC_UUID { 0x94cf71ad, 0x80e6, 0x40b5, \
	{ 0xa7, 0xc6, 0x3d, 0xc5, 0x01, 0xeb, 0x28, 0x03 } }

/**
 * Read OTP memory
 *
 * [in]		value[0].a		OTP start offset in byte
 * [in]		value[0].b		Access type (0 : shadow,
 *					1 : fuse, 2 : lock)
 * [out]	memref[1].buffer	Output buffer to store read values
 * [out]	memref[1].size		Size of OTP to be read
 *
 * Return codes:
 * TEE_SUCCESS - Invoke command success
 * TEE_ERROR_BAD_PARAMETERS - Incorrect input param
 */
#define PTA_BSEC_READ_MEM		0x0	/* Read OTP */

/**
 * Write OTP memory
 *
 * [in]		value[0].a		OTP start offset in byte
 * [in]		value[0].b		Access type (0 : shadow,
 *					1 : fuse, 2 : lock)
 * [in]		memref[1].buffer	Input buffer to read values
 * [in]		memref[1].size		Size of OTP to be written
 *
 * Return codes:
 * TEE_SUCCESS - Invoke command success
 * TEE_ERROR_BAD_PARAMETERS - Incorrect input param
 */
#define PTA_BSEC_WRITE_MEM		0x1	/* Write OTP */

/**
 * Get BSEC state
 * Return the chip security level by reading the BSEC state
 *
 * [out]	value[0].a		BSEC state
 *					0 : Secure Open state
 *					1 : Secure Close state
 *					3 : Invalid State
 * Return codes:
 * TEE_SUCCESS - Invoke command success
 * TEE_ERROR_BAD_PARAMETERS - Incorrect input param
 */
#define PTA_BSEC_GET_STATE		0x3	/* Get BSEC state */

/* Value of PTA_BSEC access type = value[in] b */
#define SHADOW_ACCESS			0
#define FUSE_ACCESS			1
#define LOCK_ACCESS			2

/* Bitfield definition for LOCK status */
/* warning: bit 31 is reserved in PTA NVMEM for OTP_UPDATE_REQ */
#define LOCK_PERM			BIT(30)
#define LOCK_SHADOW_R			BIT(29)
#define LOCK_SHADOW_W			BIT(28)
#define LOCK_SHADOW_P			BIT(27)
#define LOCK_ERROR			BIT(26)
#define STATUS_PROVISIONING		BIT(1)
#define STATUS_SECURE			BIT(0)

#define LOCK_MASK_SHADOW		GENMASK_32(29, 27)
#endif /* __PTA_BSEC_H */
