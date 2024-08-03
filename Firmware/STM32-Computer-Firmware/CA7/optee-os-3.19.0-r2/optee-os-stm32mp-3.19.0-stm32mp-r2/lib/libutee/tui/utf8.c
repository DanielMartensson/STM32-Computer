// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include "utf8.h"

uint32_t utf8_get_code_point(const char *utf8_str, size_t *idx)
{
	const uint8_t *p = (const uint8_t *)utf8_str + *idx;
	uint8_t first = p[0];
	uint8_t first_mask;
	uint8_t first_prefix;
	size_t n;
	size_t m;
	uint32_t cp;

	if (!first)
		return 0;

	if (!(p[0] & 0x80)) {
		/* 0vvv_vvvv */
		(*idx)++;
		return p[0];
	}

	/*
	 * n = 1:
	 * first_mask = 0xe0, first_prefix = 0xc0
	 * 110v_vvvv 10vv_vvvv
	 *
	 * n = 2:
	 * first_mask = 0xf0, first_prefix = 0xe0
	 * 1110_vvvv 10vv_vvvv 10vv_vvvv
	 *
	 * n = 3:
	 * first_mask = 0xf8, first_prefix = 0xf0
	 * 1111_0vvv 10vv_vvvv 10vv_vvvv 10vv_vvvv
	 *
	 * n = 4:
	 * first_mask = 0xfc, first_prefix = 0xf8
	 * 1111_10vv 10vv_vvvv 10vv_vvvv 10vv_vvvv 10vv_vvvv
	 *
	 * n = 5:
	 * first_mask = 0xfe, first_prefix = 0xfc
	 * 1111_110v 10vv_vvvv 10vv_vvvv 10vv_vvvv 10vv_vvvv 10vv_vvvv
	 */
	first_mask = 0xe0;
	first_prefix = 0xc0;
	for (n = 1; n < 6 && (p[n] & 0xc0) == 0x80; n++) {
		if ((first & first_mask) == first_prefix) {
			cp = first & ~first_mask;
			for (m = 1; m <= n; m++) {
				cp <<= 6;
				cp |= p[m] & 0x3f;
			}
			(*idx) += n + 1;
			return cp;
		}
		first_prefix = first_mask;
		first_mask = (first_mask >> 1) | 0x80;
	}
	return UTF8_INVALID_CODE;
}
