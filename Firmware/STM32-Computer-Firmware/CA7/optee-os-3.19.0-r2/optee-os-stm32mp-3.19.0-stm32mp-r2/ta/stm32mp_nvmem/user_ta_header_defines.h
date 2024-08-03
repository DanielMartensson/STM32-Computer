/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021, STMicroelectronics
 */

#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <ta_stm32mp_nvmem.h>

#define TA_UUID				TA_STM32MP_NVMEM_UUID

#define TA_FLAGS			(TA_FLAG_SINGLE_INSTANCE | \
					 TA_FLAG_MULTI_SESSION)

#define TA_STACK_SIZE			(4 * 1024)
#define TA_DATA_SIZE			(16 * 1024)

#endif /* USER_TA_HEADER_DEFINES_H */
