/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#ifndef __TEE__TUI_H
#define __TEE__TUI_H

#include <kernel/user_mode_ctx.h>
#include <tee_api_types.h>
#include <tee_tui_api_types.h>

/* TUI session timeout in milliseconds */
#define TUI_SESSION_TIMEOUT		10000

#define TUI_PROP_SECURITY_INDICATOR	0

struct tee_ta_session;

#ifdef CFG_WITH_TUI
TEE_Result tui_init_session(struct ts_session *s);

TEE_Result tui_set_session_busy(struct ts_session *s);

TEE_Result tui_clear_session_busy(struct ts_session *s);

TEE_Result tui_close_session(struct ts_session *s);

TEE_Result tui_get_time_to_session_timeout(struct ts_session *s,
					   const TEE_Time *current_time,
					   uint32_t *millis);

TEE_Result tui_input_enable(void);
void tui_input_disable(void);
void tui_async_input(uint16_t key, uint16_t x, uint16_t y);
TEE_Result tui_input_get(struct ts_session *s, size_t timeout,
			 bool *is_timedout, bool *is_key,
			 uint32_t *key, size_t *xpos, size_t *ypos);

#else
static inline void tui_async_input(uint16_t key __unused, uint16_t x __unused,
				   uint16_t y __unused)
{
}

static inline TEE_Result tui_close_session(struct ts_session *s __unused)
{
	return TEE_SUCCESS;
}
#endif
#endif /*__TEE__TUI_H*/
