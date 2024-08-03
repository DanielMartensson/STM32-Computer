/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-3-Clause) */
/*
 * Copyright (C) 2020-2024, STMicroelectronics - All Rights Reserved
 */

#ifndef _DT_BINDINGS_STM32MP25_RISAF_H
#define _DT_BINDINGS_STM32MP25_RISAF_H

/* RISAF region IDs */
#define RISAF_REG_ID(idx)	(idx)

/* RISAF base region enable modes */
#define RIF_BREN_DIS		0x0
#define RIF_BREN_EN		0x1

/* RISAF encryption modes */
#define RIF_ENC_DIS		0x0
#define RIF_ENC_EN		0x2

#define DT_RISAF_EN_SHIFT	4
#define DT_RISAF_SEC_SHIFT	5
#define DT_RISAF_ENC_SHIFT	6
#define DT_RISAF_PRIV_SHIFT	8
#define DT_RISAF_READ_SHIFT	16
#define DT_RISAF_WRITE_SHIFT	24

#define RISAFPROT(risaf_region, cid_read_list, cid_write_list, cid_priv_list, sec, enc, enabled) \
	(((cid_write_list) << DT_RISAF_WRITE_SHIFT) | \
	 ((cid_read_list) << DT_RISAF_READ_SHIFT) | \
	 ((cid_priv_list) << DT_RISAF_PRIV_SHIFT) | \
	 ((enc) << DT_RISAF_ENC_SHIFT) | \
	 ((sec) << DT_RISAF_SEC_SHIFT) | \
	 ((enabled) << DT_RISAF_EN_SHIFT) | \
	 (risaf_region))

#endif /* _DT_BINDINGS_STM32MP25_RISAF_H */
