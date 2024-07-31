// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include <tee_api.h>
#include <tee_tui_api.h>
#include <pta_tui.h>
#include <string.h>
#include "image.h"
#include "tui_private.h"

static TEE_Result invoke_tui_pta(uint32_t cmd_id, uint32_t param_types,
				 TEE_Param params[TEE_NUM_PARAMS])
{
	static TEE_TASessionHandle sess = TEE_HANDLE_NULL;
	static const TEE_UUID core_uuid = PTA_TUI_UUID;

	if (sess == TEE_HANDLE_NULL) {
		TEE_Result res = TEE_OpenTASession(&core_uuid, 0, 0, NULL,
						   &sess, NULL);

		if (res != TEE_SUCCESS)
			return res;
	}

	return TEE_InvokeTACommand(sess, 0, cmd_id, param_types, params, NULL);
}

TEE_Result __tui_pta_get_screen_info(size_t *width, size_t *height,
				     size_t *width_dpi, size_t *height_dpi,
				     uint32_t *bpp)
{
	TEE_Result res;
	uint32_t param_types;
	TEE_Param params[TEE_NUM_PARAMS];

	param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
				      TEE_PARAM_TYPE_VALUE_OUTPUT,
				      TEE_PARAM_TYPE_VALUE_OUTPUT,
				      TEE_PARAM_TYPE_NONE);
	memset(params, 0, sizeof(params));
	res = invoke_tui_pta(PTA_TUI_GET_SCREEN_INFO, param_types, params);
	*width = params[0].value.a;
	*height = params[0].value.b;
	if (width_dpi)
		*width_dpi = params[1].value.a;
	if (height_dpi)
		*height_dpi = params[1].value.b;
	if (bpp)
		*bpp = params[2].value.a;
	return res;
}

TEE_Result __tui_pta_init_session(void)
{
	uint32_t param_types;
	TEE_Param params[TEE_NUM_PARAMS];

	param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE);
	memset(params, 0, sizeof(params));
	return invoke_tui_pta(PTA_TUI_INIT_SESSION, param_types, params);
}

TEE_Result __tui_pta_close_session(void)
{
	uint32_t param_types;
	TEE_Param params[TEE_NUM_PARAMS];

	param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE);
	memset(params, 0, sizeof(params));
	return invoke_tui_pta(PTA_TUI_CLOSE_SESSION, param_types, params);
}

TEE_Result __tui_pta_init_screen(uint32_t color)
{
	uint32_t param_types;
	TEE_Param params[TEE_NUM_PARAMS];

	param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE);
	memset(params, 0, sizeof(params));
	params[0].value.a = color;
	return invoke_tui_pta(PTA_TUI_INIT_SCREEN, param_types, params);
}

TEE_Result __tui_pta_set_screen_image(size_t xpos, size_t ypos,
				      struct image *image)
{
	uint32_t param_types;
	TEE_Param params[TEE_NUM_PARAMS];

	param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				      TEE_PARAM_TYPE_VALUE_INPUT,
				      TEE_PARAM_TYPE_MEMREF_INPUT,
				      TEE_PARAM_TYPE_NONE);
	memset(params, 0, sizeof(params));
	params[0].value.a = xpos;
	params[0].value.b = ypos;
	params[1].value.a = image->width;
	params[1].value.b = image->height;
	params[2].memref.buffer = image->buf;
	params[2].memref.size = image->blen;
	return invoke_tui_pta(PTA_TUI_SET_SCREEN_IMAGE, param_types, params);
}

TEE_Result __tui_pta_display_screen(bool clear_input, size_t timeout,
				    bool *is_timedout, bool *is_key,
				    uint32_t *key, size_t *xpos, size_t *ypos)
{
	TEE_Result res;
	uint32_t param_types;
	TEE_Param params[TEE_NUM_PARAMS];

	param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				      TEE_PARAM_TYPE_VALUE_OUTPUT,
				      TEE_PARAM_TYPE_VALUE_OUTPUT,
				      TEE_PARAM_TYPE_VALUE_OUTPUT);
	memset(params, 0, sizeof(params));
	params[0].value.a = clear_input;
	params[0].value.b = timeout;
	res = invoke_tui_pta(PTA_TUI_DISPLAY_SCREEN, param_types, params);
	*is_timedout = params[1].value.a;
	*is_key = params[2].value.a;
	*key = params[2].value.b;
	*xpos = params[3].value.a;
	*ypos = params[3].value.b;
	return res;
}
