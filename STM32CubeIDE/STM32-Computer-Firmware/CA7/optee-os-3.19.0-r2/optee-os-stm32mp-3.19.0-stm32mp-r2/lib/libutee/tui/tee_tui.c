// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include <sys/queue.h>
#include <tee_api.h>
#include <tee_tui_api.h>
#include <util.h>
#include <string.h>
#include <stdlib.h>
#include "font.h"
#include "image.h"
#include "tui_private.h"
#include <assert.h>

#define MAX_LABEL_ROWS		4

/* update_button_info() has to be called before using this the first time */
static TEE_TUIScreenButtonInfo button_info[TEE_TUI_NUMBER_BUTTON_TYPES] = {
	[TEE_TUI_CORRECTION] = {
			.buttonText = (char *)"Correction",
			.buttonTextCustom = false,
			.buttonImageCustom = true,
		},
	[TEE_TUI_OK] = {
			.buttonText = (char *)"OK",
			.buttonTextCustom = false,
			.buttonImageCustom = true,
		},
	[TEE_TUI_CANCEL] = {
			.buttonText = (char *)"Cancel",
			.buttonTextCustom = false,
			.buttonImageCustom = true,
		},
	[TEE_TUI_VALIDATE] = {
			.buttonText = (char *)"Validate",
			.buttonTextCustom = false,
			.buttonImageCustom = true,
		},
	[TEE_TUI_PREVIOUS] = {
			.buttonText = (char *)"Previous",
			.buttonTextCustom = false,
			.buttonImageCustom = true,
		},
	[TEE_TUI_NEXT] = {
			.buttonText = (char *)"Next",
			.buttonTextCustom = false,
			.buttonImageCustom = true,
		},
};

static bool check_result(TEE_Result res, const TEE_Result *results,
			 size_t num_results)
{
	size_t n;

	for (n = 0; n < num_results; n++)
		if (res == results[n])
			return true;
	return false;
}

#define CHECK_RESULT(r, results) \
	do { \
		if (!check_result((r), (results), ARRAY_SIZE(results))) \
			TEE_Panic(r); \
	} while (0)

TEE_Result TEE_TUICheckTextFormat(const char *text, uint32_t *width,
				  uint32_t *height, uint32_t *lastIndex)
{
	TEE_Result res;
	size_t w;
	size_t h;
	size_t last_index;

	if (font_check_text_format(text, &w, &h, &last_index))
		res = TEE_SUCCESS;
	else
		res = TEE_ERROR_NOT_SUPPORTED;
	*width = w;
	*height = h;
	*lastIndex = last_index;
	return res;
}

static TEE_Result update_button_info(void)
{
	size_t n;
	size_t h;
	size_t w;

	for (n = 0; n < TEE_TUI_NUMBER_BUTTON_TYPES; n++) {
		if (button_info[n].buttonWidth)
			continue;

		if (!font_check_text_format(button_info[n].buttonText, &w, &h,
					    NULL))
			return TEE_ERROR_GENERIC;
		button_info[n].buttonWidth = w;
		button_info[n].buttonHeight = h;
	}

	return TEE_SUCCESS;
}

TEE_Result TEE_TUIGetScreenInfo(TEE_TUIScreenOrientation screenOrientation,
				uint32_t nbEntryFields,
				TEE_TUIScreenInfo *screenInfo)
{
	TEE_Result res;
	const TEE_Result results[] = { TEE_ERROR_NOT_SUPPORTED, TEE_SUCCESS, };
	size_t width;
	size_t height;
	size_t width_dpi;
	size_t height_dpi;
	uint32_t bpp;
	uint32_t label_color = COLOR_RGB_BLACK;

	res = update_button_info();
	if (res != TEE_SUCCESS)
		TEE_Panic(res);
	res = __tui_pta_get_screen_info(&width, &height, &width_dpi,
					&height_dpi, &bpp);
	CHECK_RESULT(res, results);
	if (res != TEE_SUCCESS)
		return res;
	if (screenOrientation != TEE_TUI_LANDSCAPE)
		res = TEE_ERROR_NOT_SUPPORTED;
	if (nbEntryFields > MAX_ENTRY_FIELDS)
		res = TEE_ERROR_NOT_SUPPORTED;
	screenInfo->grayscaleBitsDepth = 255;
	screenInfo->redBitsDepth = 255;
	screenInfo->greenBitsDepth = 255;
	screenInfo->blueBitsDepth = 255;
	screenInfo->widthInch = width / width_dpi;
	screenInfo->heightInch = height / height_dpi;
	screenInfo->maxEntryFields = MAX_ENTRY_FIELDS;
	screenInfo->entryFieldLabelWidth =
		width - button_info[TEE_TUI_CORRECTION].buttonWidth;
	screenInfo->entryFieldLabelHeight = font_get_text_height();
	screenInfo->maxEntryFieldLength =
		font_get_max_field_length(screenInfo->entryFieldLabelWidth -
					  4 * BORDER_SIZE);
	screenInfo->labelColor[0] = label_color;
	screenInfo->labelColor[1] = label_color >> 8;
	screenInfo->labelColor[2] = label_color >> 16;
	screenInfo->labelWidth = width;
	screenInfo->labelHeight = font_get_text_height() * MAX_LABEL_ROWS;
	memcpy(screenInfo->buttonInfo, button_info, sizeof(button_info));
	return res;
}

TEE_Result TEE_TUIInitSession(void)
{
	TEE_Result res;
	const TEE_Result results[] = {
		TEE_ERROR_BUSY, TEE_ERROR_OUT_OF_MEMORY, TEE_SUCCESS,
	};

	res = __tui_pta_init_session();
	CHECK_RESULT(res, results);
	return res;
}

TEE_Result TEE_TUICloseSession(void)
{
	TEE_Result res;
	const TEE_Result results[] = {
		TEE_ERROR_BUSY, TEE_ERROR_BAD_STATE, TEE_SUCCESS,
	};

	res = __tui_pta_close_session();
	CHECK_RESULT(res, results);
	return res;
}

static TEE_Result set_label_image(struct image *image, size_t x, size_t y,
				  const TEE_TUIImage *tui_image)
{
	TEE_Result res = TEE_SUCCESS;
	TEE_ObjectHandle handle = TEE_HANDLE_NULL;
	void *buf = NULL;
	TEE_ObjectInfo obj_info;
	uint32_t count;

	switch (tui_image->source) {
	case TEE_TUI_REF_SOURCE:
		if (!image_set_png(image, x, y, tui_image->ref.image,
				   tui_image->ref.imageLength))
			res = TEE_ERROR_BAD_FORMAT;
		break;
	case TEE_TUI_OBJECT_SOURCE:
		res = TEE_OpenPersistentObject(tui_image->object.storageID,
					       tui_image->object.objectID,
					       tui_image->object.objectIDLen,
					       TEE_DATA_FLAG_ACCESS_READ |
					       TEE_DATA_FLAG_SHARE_READ,
					       &handle);
		if (res != TEE_SUCCESS)
			break;

		res = TEE_GetObjectInfo1(handle, &obj_info);
		if (res != TEE_SUCCESS)
			break;

		buf = malloc(obj_info.dataSize);
		if (!buf) {
			res = TEE_ERROR_OUT_OF_MEMORY;
			break;
		}

		res = TEE_ReadObjectData(handle, buf, obj_info.dataSize,
					 &count);
		if (res != TEE_SUCCESS)
			break;

		if (count != obj_info.dataSize) {
			res = TEE_ERROR_BAD_FORMAT;
			break;
		}

		if (!image_set_png(image, x, y, buf, obj_info.dataSize))
			res = TEE_ERROR_BAD_FORMAT;
		break;

	default:
		res = TEE_ERROR_BAD_FORMAT;
	}

	TEE_CloseObject(handle);
	free(buf);
	return res;
}

static TEE_Result set_label(const TEE_TUIScreenLabel *label, size_t *ypos)
{
	TEE_Result res;
	struct image *image = NULL;
	size_t w;
	size_t h;
	size_t xmin;
	size_t ymin;
	size_t xmax;
	size_t ymax;
	uint32_t color;

	if (!font_check_text_format(label->text, &w, &h, NULL))
		TEE_Panic(TEE_ERROR_NOT_SUPPORTED);

	if (label->image.source != TEE_TUI_NO_SOURCE) {
		xmin = MIN(label->imageXOffset, label->textXOffset);
		ymin = MIN(label->imageYOffset, label->textYOffset);
		xmax = MAX(label->imageXOffset + label->image.width,
			   label->textXOffset + w);
		ymax = MAX(label->imageYOffset + label->image.height,
			   label->textYOffset + h);
	} else {
		xmin = label->textXOffset;
		ymin = label->textYOffset;
		xmax = label->textXOffset + w;
		ymax = label->textYOffset + h;
	}
	ymin += *ypos;
	ymax += *ypos;
	*ypos = ymax;

	image = image_alloc(xmax - xmin, ymax - ymin,
			    COLOR_RGB_LIGHT_GOLDENROD);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (label->image.source != TEE_TUI_NO_SOURCE) {
		res = set_label_image(image, label->imageXOffset,
				      label->imageYOffset, &label->image);
		if (res != TEE_SUCCESS)
			goto out;
	}

	color = (label->textColor[0] << 16) | (label->textColor[1] << 8) |
		label->textColor[2];
	if (!font_render_text(image, label->textYOffset - ymin,
			      label->textXOffset - xmin, label->text, color)) {
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}

	res = __tui_pta_set_screen_image(ymin, xmin, image);
out:
	image_free(image);
	return res;
}

static TEE_Result create_buttons(struct tui_widget_head *twh, size_t scr_width,
				 size_t scr_height,
				 TEE_TUIButton * const *buttons,
				 const bool req_buttons[TEE_TUI_NUMBER_BUTTON_TYPES])
{
	TEE_Result res;
	size_t n;
	uint32_t req = 0;
	enum button_pos bpos[3];
	TEE_TUIButtonType btypes[3];
	size_t num_buttons;

	for (n = 0; n < TEE_TUI_NUMBER_BUTTON_TYPES; n++)
		req |= req_buttons[n] << n;

	switch (req & ~(1 << TEE_TUI_CORRECTION)) {
	case (1 << TEE_TUI_OK):
		bpos[0] = POS_MIDDLE;
		btypes[0] = TEE_TUI_OK;
		num_buttons = 1;
		break;
	case (1 << TEE_TUI_OK) | (1 << TEE_TUI_PREVIOUS):
		bpos[0] = POS_MIDDLE;
		btypes[0] = TEE_TUI_OK;
		bpos[1] = POS_LEFT;
		btypes[1] = TEE_TUI_PREVIOUS;
		num_buttons = 2;
		break;
	case (1 << TEE_TUI_CANCEL) | (1 << TEE_TUI_VALIDATE):
		bpos[0] = POS_LEFT;
		btypes[0] = TEE_TUI_CANCEL;
		bpos[1] = POS_RIGHT;
		btypes[1] = TEE_TUI_VALIDATE;
		num_buttons = 2;
		break;
	case (1 << TEE_TUI_CANCEL) | (1 << TEE_TUI_NEXT):
		bpos[0] = POS_LEFT;
		btypes[0] = TEE_TUI_CANCEL;
		bpos[1] = POS_RIGHT;
		btypes[1] = TEE_TUI_NEXT;
		num_buttons = 2;
		break;
	case (1 << TEE_TUI_CANCEL) | (1 << TEE_TUI_PREVIOUS) |
	     (1 << TEE_TUI_NEXT):
		bpos[0] = POS_MIDDLE;
		btypes[0] = TEE_TUI_CANCEL;
		bpos[1] = POS_LEFT;
		btypes[1] = TEE_TUI_PREVIOUS;
		bpos[2] = POS_RIGHT;
		btypes[2] = TEE_TUI_NEXT;
		num_buttons = 3;
		break;
	case (1 << TEE_TUI_CANCEL) | (1 << TEE_TUI_VALIDATE) |
	     (1 << TEE_TUI_PREVIOUS):
		bpos[0] = POS_MIDDLE;
		btypes[0] = TEE_TUI_CANCEL;
		bpos[1] = POS_LEFT;
		btypes[1] = TEE_TUI_PREVIOUS;
		bpos[2] = POS_RIGHT;
		btypes[2] = TEE_TUI_VALIDATE;
		num_buttons = 3;
		break;
	default:
		return TEE_ERROR_NOT_SUPPORTED;
	}

	for (n = 0; n < num_buttons; n++) {
		res = __tui_button_create(twh, btypes[n],
					  button_info + btypes[n], buttons[n],
					  bpos[n], scr_width, scr_height);
		if (res != TEE_SUCCESS)
			return res;
	}
	return TEE_SUCCESS;
}

static void twh_final(struct tui_widget_head *twh)
{
	while (!TAILQ_EMPTY(twh)) {
		struct tui_widget *tw = TAILQ_FIRST(twh);

		TAILQ_REMOVE(twh, tw, link);
		tw->ops->destroy(tw);
	}
}

static bool display_should_exit(struct tui_widget_head *twh,
				struct tui_state *ts)
{
	if (!ts->clicked_button)
		return false;

	if (*ts->clicked_button == TEE_TUI_VALIDATE)
		return __tui_entry_fields_are_ok(twh);

	return true;
}

static TEE_Result display_screen(struct tui_widget_head *twh,
				 TEE_TUIButtonType *selectedButton)
{
	TEE_Result res;
	bool is_key;
	bool is_timedout;
	uint32_t key;
	size_t timeout;
	size_t xpos;
	size_t ypos;
	struct tui_widget *tw;
	struct tui_state ts = { NULL };

	/* Find keyboard and save pointer */
	ts.keyboard = __tui_keyboard_find(twh);

	/* Find first input and give focus */
	tw = __tui_entry_field_find_first(twh);
	if (tw) {
		/* If there's no keyboard something is wrong */
		if (!ts.keyboard)
			return TEE_ERROR_GENERIC;
		tw->ops->focus(tw, &ts);
	}

	/*
	 * If there's no focus yet, select the first widget what can have
	 * focus.
	 */
	TAILQ_FOREACH(tw, twh, link) {
		if (!ts.focus)
			tw->ops->focus(tw, &ts);
	}

	/* If no widget has focus by now something is wrong */
	if (!ts.focus)
		return TEE_ERROR_GENERIC;

	while (!display_should_exit(twh, &ts)) {
		TAILQ_FOREACH(tw, twh, link) {
			res = tw->ops->paint(tw, &ts);
			if (res != TEE_SUCCESS)
				return res;
		}
		ts.clicked_button = NULL;
		if (__tui_entry_field_has_temp_clear(ts.input))
			timeout = 1000;
		else
			timeout = SIZE_MAX;
		res = __tui_pta_display_screen(true, timeout, &is_timedout,
					       &is_key, &key, &xpos, &ypos);
		if (res != TEE_SUCCESS)
			return res;

		if (is_timedout) {
			__tui_entry_field_reset_temp_clear(ts.input);
			continue;
		}

		if (!is_key) {
			tw = __tui_widget_find(twh, xpos, ypos);
			if (!tw)
				continue;

			EMSG("(%zu,%zu) clicked widget at (%zu,%zu)(%zu,%zu)",
			     xpos, ypos, tw->x, tw->y, tw->width, tw->height);
			tw->ops->click(tw, &ts, xpos, ypos);
			continue;
		}

		switch (key) {
		case '\t':
			tw = ts.focus;
			tw->ops->unfocus(tw, &ts);
			while (!ts.focus) {
				tw = TAILQ_NEXT(tw, link);
				if (!tw)
					tw = TAILQ_FIRST(twh);
				tw->ops->focus(tw, &ts);
			}
			break;
		case '\n':
		case '\r':
			ts.focus->ops->click(ts.focus, &ts, -1, -1);
			break;
		default:
			ts.focus->ops->input(ts.focus, &ts, key);
			break;
		}
	}
	if (!ts.clicked_button)
		return TEE_ERROR_GENERIC;
	*selectedButton = *ts.clicked_button;
	return TEE_SUCCESS;
}

TEE_Result TEE_TUIDisplayScreen(const TEE_TUIScreenConfiguration *sc,
				bool closeTUISession,
				TEE_TUIEntryField *entryFields,
				uint32_t entryFieldCount,
				TEE_TUIButtonType *selectedButton)
{
	TEE_Result res;
	size_t ypos = 0;
	struct tui_widget_head twh = TAILQ_HEAD_INITIALIZER(twh);
	size_t scr_height;
	size_t scr_width;
	const TEE_Result results[] = {
		TEE_ERROR_OUT_OF_MEMORY, TEE_ERROR_ITEM_NOT_FOUND,
		TEE_ERROR_ACCESS_CONFLICT, TEE_ERROR_BAD_FORMAT,
		TEE_ERROR_BAD_STATE, TEE_ERROR_BUSY, TEE_ERROR_CANCEL,
		TEE_ERROR_EXTERNAL_CANCEL, TEE_SUCCESS,
	};

	if (sc->screenOrientation != TEE_TUI_LANDSCAPE)
		TEE_Panic(TEE_ERROR_NOT_SUPPORTED);

	res = __tui_pta_get_screen_info(&scr_width, &scr_height,
					NULL, NULL, NULL);
	CHECK_RESULT(res, results);
	if (res != TEE_SUCCESS)
		return res;

	res = __tui_pta_init_screen(COLOR_RGB_LIGHT_GRAY);
	CHECK_RESULT(res, results);
	if (res != TEE_SUCCESS)
		goto out;

	res = set_label(&sc->label, &ypos);
	CHECK_RESULT(res, results);
	if (res != TEE_SUCCESS)
		goto out;

	if (entryFieldCount) {
		res = __tui_entry_fields_create(&twh, &ypos, scr_width,
						button_info +
						TEE_TUI_CORRECTION,
						entryFields, entryFieldCount);
		CHECK_RESULT(res, results);
		if (res != TEE_SUCCESS)
			goto out;

		res = __tui_keyboard_create(&twh, &ypos, scr_width);
		CHECK_RESULT(res, results);
		if (res != TEE_SUCCESS)
			goto out;
	}

	res = create_buttons(&twh, scr_width, scr_height, sc->buttons,
			     sc->requestedButtons);
	CHECK_RESULT(res, results);
	if (res != TEE_SUCCESS)
		goto out;

	res = display_screen(&twh, selectedButton);

out:
	twh_final(&twh);
	if (closeTUISession)
		TEE_TUICloseSession();
	return res;
}
