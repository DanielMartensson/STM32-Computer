// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */
#include <kernel/spinlock.h>
#include <kernel/tee_ta_manager.h>
#include <kernel/tee_time.h>
#include <kernel/thread.h>
#include <kernel/user_mode_ctx.h>
#include <types_ext.h>
#include <tee_api_types.h>
#include <tee/tui.h>
#include <utee_defines.h>

#define TUI_INPUT_BUFFER_SIZE	16

/**
 * struct tui_input - Trusted UI input state
 *
 * @lock - Lock changes
 * @queue - Array of possible input events location in TUI
 * @write_pos
 * @read_pos
 */
struct tui_input {
	unsigned int lock;
	struct {
		int16_t key;
		uint16_t x;
		uint16_t y;
	} queue[TUI_INPUT_BUFFER_SIZE];
	size_t write_pos;
	size_t read_pos;
};

struct tui_input tui_input = {
	.lock = SPINLOCK_UNLOCK,
	.read_pos = TUI_INPUT_BUFFER_SIZE,
};

TEE_Result tui_input_enable(void)
{
	uint32_t exceptions = cpu_spin_lock_xsave(&tui_input.lock);

	tui_input.read_pos = tui_input.write_pos;

	cpu_spin_unlock_xrestore(&tui_input.lock, exceptions);

	return TEE_SUCCESS;
}

void tui_input_disable(void)
{
	uint32_t exceptions = cpu_spin_lock_xsave(&tui_input.lock);

	tui_input.read_pos = TUI_INPUT_BUFFER_SIZE;

	cpu_spin_unlock_xrestore(&tui_input.lock, exceptions);
}

static size_t next_pos(size_t pos)
{
	return (pos + 1) % TUI_INPUT_BUFFER_SIZE;
}

TEE_Result tui_input_get(struct ts_session *s, size_t timeout,
			 bool *is_timedout, bool *is_key,
			 uint32_t *key, size_t *xpos, size_t *ypos)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t exceptions = 0;
	TEE_Time cur_time = { };
	TEE_Time base_time = { };
	TEE_Time timeout_time = {
		.seconds = timeout / TEE_TIME_MILLIS_BASE,
		.millis = timeout % TEE_TIME_MILLIS_BASE,
	};

	res = tee_time_get_sys_time(&base_time);
	if (res)
		return res;
	cur_time = base_time;
	TEE_TIME_ADD(timeout_time, base_time, timeout_time);

	exceptions = cpu_spin_lock_xsave(&tui_input.lock);

	if (tui_input.read_pos >= TUI_INPUT_BUFFER_SIZE) {
		/* Input cancelled or not enabled */
		res = TEE_ERROR_EXTERNAL_CANCEL;
		goto out;
	}

	while (tui_input.read_pos == tui_input.write_pos &&
	       TEE_TIME_LT(cur_time, timeout_time)) {
		cpu_spin_unlock_xrestore(&tui_input.lock, exceptions);

		/*
		 * Will take eventual interrupt(s) here.
		 */

		res = tee_time_get_sys_time(&cur_time);
		if (res)
			return res;

		res = tui_get_time_to_session_timeout(s, &cur_time, NULL);
		if (res)
			return res;

		if (tee_ta_session_is_cancelled(to_ta_session(s), &cur_time))
			return TEE_ERROR_CANCEL;

		exceptions = cpu_spin_lock_xsave(&tui_input.lock);
	}

	if (tui_input.read_pos >= TUI_INPUT_BUFFER_SIZE) {
		/* Input cancelled or not enabled */
		res = TEE_ERROR_EXTERNAL_CANCEL;
		goto out;
	}

	if (res == TEE_SUCCESS) {
		if (TEE_TIME_LT(cur_time, timeout_time)) {
			*is_timedout = false;
			*is_key = !!tui_input.queue[tui_input.read_pos].key;
			*key = tui_input.queue[tui_input.read_pos].key;
			*xpos = tui_input.queue[tui_input.read_pos].x;
			*ypos = tui_input.queue[tui_input.read_pos].y;
			tui_input.read_pos = next_pos(tui_input.read_pos);
		} else {
			*is_timedout = true;
			*is_key = false;
			*key = 0;
			*xpos = 0;
			*ypos = 0;
		}
	}

out:
	cpu_spin_unlock_xrestore(&tui_input.lock, exceptions);

	return res;
}

void tui_async_input(uint16_t key, uint16_t x, uint16_t y)
{
	uint32_t exceptions = cpu_spin_lock_xsave(&tui_input.lock);

	if (tui_input.read_pos < TUI_INPUT_BUFFER_SIZE &&
	    next_pos(tui_input.write_pos) != tui_input.read_pos) {
		tui_input.queue[tui_input.write_pos].key = key;
		tui_input.queue[tui_input.write_pos].x = x;
		tui_input.queue[tui_input.write_pos].y = y;
		tui_input.write_pos = next_pos(tui_input.write_pos);
	}

	cpu_spin_unlock_xrestore(&tui_input.lock, exceptions);
}
