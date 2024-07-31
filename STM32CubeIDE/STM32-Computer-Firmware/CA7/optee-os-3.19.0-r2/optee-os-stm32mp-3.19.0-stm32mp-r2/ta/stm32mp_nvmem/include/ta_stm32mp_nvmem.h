/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021, STMicroelectronics
 */

#ifndef TA_STM32MP_NVMEM_H
#define TA_STM32MP_NVMEM_H

#define TA_STM32MP_NVMEM_UUID { 0x1a8342cc, 0x81a5, 0x4512, \
		{ 0x99, 0xfe, 0x9e, 0x2b, 0x3e, 0x37, 0xd6, 0x26 } }

/* TA_STM32MP_NVMEM type = value[in].a */
#define STM32MP_NVMEM_OTP	0

/*
 * Read NVMEM memory for STM32CubeProgrammer
 *
 * [in]		value[0].a:		Type (0 for OTP access)
 * [out]	memref[1].buffer	Output buffer to return all read values
 * [out]	memref[1].size		Size of buffer to be read
 */
#define TA_STM32MP_NVMEM_READ		0x0

/*
 * Write NVMEM memory for STM32CubeProgrammer
 *
 * [in]	     value[0].a		Type (0 for OTP access)
 * [in]      memref[1].buffer	Input buffer to read values
 * [in]      memref[1].size	Size of buffer to be written
 *
 * Return codes:
 * TEE_SUCCESS - Invoke command success
 * TEE_ERROR_BAD_PARAMETERS - Incorrect input param
 */
#define TA_STM32MP_NVMEM_WRITE		0x1

#endif /* TA_STM32MP_NVMEM_H */
