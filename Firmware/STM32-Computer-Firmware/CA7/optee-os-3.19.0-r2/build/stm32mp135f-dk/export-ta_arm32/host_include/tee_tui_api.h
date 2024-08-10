/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#ifndef __TEE_TUI_API_H
#define __TEE_TUI_API_H

#include <tee_tui_api_types.h>

TEE_Result TEE_TUICheckTextFormat(const char *text, uint32_t *width,
				  uint32_t *height, uint32_t *lastIndex);
TEE_Result TEE_TUIGetScreenInfo(TEE_TUIScreenOrientation screenOrientation,
				uint32_t nbEntryFields,
				TEE_TUIScreenInfo *screenInfo);
TEE_Result TEE_TUIInitSession(void);
TEE_Result TEE_TUICloseSession(void);
TEE_Result TEE_TUIDisplayScreen(const TEE_TUIScreenConfiguration *screenConfiguration,
				bool closeTUISession,
				TEE_TUIEntryField *entryFields,
				uint32_t entryFieldCount,
				TEE_TUIButtonType *selectedButton);

#endif /*__TEE_TUI_API_H*/
