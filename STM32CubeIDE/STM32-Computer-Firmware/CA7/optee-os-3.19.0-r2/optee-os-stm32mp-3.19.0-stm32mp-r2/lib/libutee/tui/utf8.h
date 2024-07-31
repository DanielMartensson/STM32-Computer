/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#ifndef __UTF8_H
#define __UTF8_H

#include <types_ext.h>

#define UTF8_INVALID_CODE	0xffffffff

uint32_t utf8_get_code_point(const char *utf8_str, size_t *idx);

#endif /*__UTF8_H*/
