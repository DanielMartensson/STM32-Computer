/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#ifndef __PTA_TUI_H
#define __PTA_TUI_H

#define PTA_TUI_UUID	{ 0x9c199eb0, 0x4d2d, 0x41a5, { \
			  0x8c, 0xe9, 0xf2, 0x08, 0x52, 0x15, 0x77, 0xd1 } }

#define PTA_TUI_SCREEN_24BPP	0

/*
 * [out]  value[0].a:	width
 * [out]  value[0].b:	height
 * [out]  value[1].a:	width_dpi
 * [out]  value[1].b:	height_dpi
 * [out]  value[2].a:	bpp
 * Returns TEE_Result
 */
#define PTA_TUI_GET_SCREEN_INFO		1

/* See TEE_TUIInitSession() */
#define PTA_TUI_INIT_SESSION		3

/* See TEE_TUICloseSession() */
#define PTA_TUI_CLOSE_SESSION		4

/*
 * [in] value[0].a:	color
 */
#define PTA_TUI_INIT_SCREEN		5

/*
 * [in] value[0].a:	xpos
 * [in] value[0].b:	ypos
 * [in] value[1].a:	width
 * [in] value[1].b:	height
 * [in] memref[2]:	image
 */
#define PTA_TUI_SET_SCREEN_IMAGE	6

/*
 * See TEE_TUIDisplayScreen()
 * [in]  value[0].a	clear_input	(bool)
 * [in]  value[0].b	time_out	(milliseconds)
 * [out] value[1].a	is_timedout	(bool)
 * [out] value[2].a	is_key		(bool)
 * [out] value[2].b	key
 * [out] value[3].a	xpos
 * [out] value[3].b	ypos
 */
#define PTA_TUI_DISPLAY_SCREEN		7

#endif /*__PTA_TUI_H*/
