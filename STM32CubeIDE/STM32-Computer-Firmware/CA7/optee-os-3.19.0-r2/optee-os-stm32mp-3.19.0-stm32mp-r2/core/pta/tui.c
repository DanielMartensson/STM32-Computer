// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include <kernel/pseudo_ta.h>
#include <kernel/user_mode_ctx.h>
#include <pta_tui.h>
#include <tee/tui.h>
#include <drivers/frame_buffer.h>
#include <display.h>
#include <assert.h>

static TEE_Result taf_get_screen_info(struct ts_session *s __unused,
				      uint32_t param_types,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	struct frame_buffer *fb;
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
					  TEE_PARAM_TYPE_VALUE_OUTPUT,
					  TEE_PARAM_TYPE_VALUE_OUTPUT,
					  TEE_PARAM_TYPE_NONE);

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	fb = display_get_frame_buffer();
	params[0].value.a = fb->width;
	params[0].value.b = fb->height;
	params[1].value.a = fb->width_dpi;
	params[1].value.b = fb->height_dpi;
	COMPILE_TIME_ASSERT(PTA_TUI_SCREEN_24BPP == FB_24BPP);
	params[2].value.a = fb->bpp;
	return TEE_SUCCESS;
}

static TEE_Result taf_init_session(struct ts_session *s,
				   uint32_t param_types,
				   TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	TEE_Result res;
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE);

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	res = tui_init_session(s);
	if (res == TEE_SUCCESS)
		tui_clear_session_busy(s);
	return res;
}

static TEE_Result taf_close_session(struct ts_session *s,
				    uint32_t param_types,
				    TEE_Param params[TEE_NUM_PARAMS] __unused)
{
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE);

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	return tui_close_session(s);
}

static TEE_Result taf_init_screen(struct ts_session *s,
				  uint32_t param_types,
				  TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res;
	struct frame_buffer *fb;
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE,
					  TEE_PARAM_TYPE_NONE);

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	res = tui_set_session_busy(s);
	if (res != TEE_SUCCESS)
		return res;
	fb = display_get_frame_buffer();
	frame_buffer_clear(fb, params[0].value.a);
	tui_clear_session_busy(s);
	return TEE_SUCCESS;
}

static TEE_Result taf_set_screen_image(struct ts_session *s,
				       uint32_t param_types,
				       TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res;
	struct frame_buffer *fb;
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
					  TEE_PARAM_TYPE_VALUE_INPUT,
					  TEE_PARAM_TYPE_MEMREF_INPUT,
					  TEE_PARAM_TYPE_NONE);

	if (exp_pt != param_types || !params[2].memref.buffer)
		return TEE_ERROR_BAD_PARAMETERS;

	fb = display_get_frame_buffer();
	if (params[2].memref.size !=
	    frame_buffer_get_image_size(fb, params[1].value.a,
					params[1].value.b))
		return TEE_ERROR_BAD_PARAMETERS;

	res = tui_set_session_busy(s);
	if (res != TEE_SUCCESS)
		return res;

	frame_buffer_set_image(fb, params[0].value.a, params[0].value.b,
			       params[1].value.a, params[1].value.b,
			       params[2].memref.buffer);

	tui_clear_session_busy(s);
	return TEE_SUCCESS;
}

static TEE_Result taf_display_screen(struct ts_session *s,
				     uint32_t param_types,
				     TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res;
	bool clear_input;
	bool is_key;
	bool is_timedout;
	size_t timeout;
	uint32_t key;
	size_t xpos;
	size_t ypos;
	uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
					  TEE_PARAM_TYPE_VALUE_OUTPUT,
					  TEE_PARAM_TYPE_VALUE_OUTPUT,
					  TEE_PARAM_TYPE_VALUE_OUTPUT);

	if (exp_pt != param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	res = tui_set_session_busy(s);
	if (res != TEE_SUCCESS)
		return res;

	clear_input = params[0].value.a;
	timeout = params[0].value.b;

	if (clear_input)
		tui_input_enable();

	res = tui_input_get(s, timeout, &is_timedout,
			    &is_key, &key, &xpos, &ypos);

	params[1].value.a = is_timedout;
	params[2].value.a = is_key;
	params[2].value.b = key;
	params[3].value.a = xpos;
	params[3].value.b = ypos;
	tui_clear_session_busy(s);
	return res;
}

typedef TEE_Result (*ta_func)(struct ts_session *s, uint32_t param_types,
			      TEE_Param params[TEE_NUM_PARAMS]);

static const ta_func ta_funcs[] = {
	[PTA_TUI_GET_SCREEN_INFO] = taf_get_screen_info,
	[PTA_TUI_INIT_SESSION] = taf_init_session,
	[PTA_TUI_CLOSE_SESSION] = taf_close_session,
	[PTA_TUI_INIT_SCREEN] = taf_init_screen,
	[PTA_TUI_SET_SCREEN_IMAGE] = taf_set_screen_image,
	[PTA_TUI_DISPLAY_SCREEN] = taf_display_screen,
};

static TEE_Result open_session(uint32_t param_types __unused,
			       TEE_Param pParams[TEE_NUM_PARAMS] __unused,
			       void **sess_ctx __unused)
{
	struct ts_session *s = NULL;

	/* Check that we're called from a TA */
	s = ts_get_calling_session();
	if (!s)
		return TEE_ERROR_ACCESS_DENIED;
	if (!is_user_ta_ctx(s->ctx))
		return TEE_ERROR_ACCESS_DENIED;

	return TEE_SUCCESS;
}

static void close_session(void *sess_ctx __unused)
{
}

static TEE_Result invoke_command(void *sess_ctx __unused, uint32_t cmd_id,
				 uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	struct ts_session *s = ts_get_calling_session();

	if (cmd_id < ARRAY_SIZE(ta_funcs) && ta_funcs[cmd_id])
		return ta_funcs[cmd_id](s, param_types, params);

	return TEE_ERROR_NOT_IMPLEMENTED;
}

pseudo_ta_register(.uuid = PTA_TUI_UUID, .name = "tui",
		   .flags = PTA_DEFAULT_FLAGS,
		   .open_session_entry_point = open_session,
		   .close_session_entry_point = close_session,
		   .invoke_command_entry_point = invoke_command);
