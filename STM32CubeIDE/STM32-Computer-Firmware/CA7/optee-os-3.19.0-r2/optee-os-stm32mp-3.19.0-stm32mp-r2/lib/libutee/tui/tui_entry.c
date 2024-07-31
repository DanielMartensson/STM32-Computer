// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include <tee_api.h>
#include <tee_tui_api.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"
#include "image.h"
#include "tui_private.h"

struct tui_correction {
	struct tui_widget *input;
	TEE_TUIScreenButtonInfo *bi;
	struct tui_widget tw;
};

struct tui_input {
	TEE_TUIEntryField *field;
	struct tui_keyboard *keyboard;
	size_t min_len;
	size_t max_len;
	bool temp_clear;
	struct tui_widget tw;
};

struct field_geometry {
	size_t input_width;
	size_t input_height;
	size_t input_x;
	size_t input_yoffs;
	size_t input_max_len;
	size_t corr_width;
	size_t corr_height;
	size_t corr_x;
	size_t corr_yoffs;
	size_t max_height;
};

static bool __maybe_unused is_tui_correction(struct tui_widget *tw);
static bool is_tui_input(struct tui_widget *tw);

static struct tui_correction *to_tui_correction(struct tui_widget *tw)
{
	assert(is_tui_correction(tw));
	return container_of(tw, struct tui_correction, tw);
}

static struct tui_input *to_tui_input(struct tui_widget *tw)
{
	assert(is_tui_input(tw));
	return container_of(tw, struct tui_input, tw);
}

static void state_set_input(struct tui_state *ts, struct tui_widget *tw)
{
	if (ts->input != tw) {
		ts->input = tw;
		__tui_keyboard_select_layout(ts);
	}
}

static TEE_Result op_correction_paint(struct tui_widget *tw,
				      struct tui_state *ts)
{
	TEE_Result res;
	struct image *image;
	struct tui_correction *tc = to_tui_correction(tw);
	char *text = tc->bi->buttonText;

	if (!tw->updated)
		return TEE_SUCCESS;

	image = __tui_widget_image_alloc(tw, COLOR_RGB_LIGHT_GOLDENROD);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!font_render_text(image, 0, 0, text, COLOR_RGB_BLACK)) {
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}
	res = __tui_widget_display(tw, ts, image);
	if (res == TEE_SUCCESS)
		tw->updated = false;
out:
	image_free(image);
	return res;
}

static void op_correction_focus(struct tui_widget *tw,
				struct tui_state *ts)
{
	struct tui_correction *tc = to_tui_correction(tw);

	if (tw != ts->focus) {
		tw->updated = true;
		ts->focus = tw;
		state_set_input(ts, tc->input);
	}
}

static void op_correction_click(struct tui_widget *tw,
				struct tui_state *ts, ssize_t x __unused,
				ssize_t y __unused)
{
	if (tw != ts->focus) {
		ts->focus->ops->unfocus(ts->focus, ts);
		tw->ops->focus(tw, ts);
	}

	ts->input->ops->input(ts->input, ts, 0x08);
}

static void op_correction_destroy(struct tui_widget *tw)
{
	free(to_tui_correction(tw));
}

static const struct tui_widget_ops correction_ops = {
	.paint = op_correction_paint,
	.focus = op_correction_focus,
	.unfocus = __tui_widget_op_unfocus,
	.click = op_correction_click,
	.input = __tui_widget_op_dummy_input,
	.destroy = op_correction_destroy,
};

static bool is_tui_correction(struct tui_widget *tw)
{
	return tw->ops == &correction_ops;
}

static struct tui_correction *add_entry_correction(struct tui_widget_head *twh,
						   struct field_geometry *fg,
						   size_t ypos,
						   TEE_TUIScreenButtonInfo *bi,
						   struct tui_widget *input)
{
	struct tui_correction *twc = calloc(1, sizeof(*twc));

	if (!twc)
		return NULL;

	twc->tw.ops = &correction_ops;
	twc->tw.updated = true;
	twc->bi = bi;
	twc->input = input;
	twc->tw.y = ypos + fg->corr_yoffs;
	twc->tw.x = fg->corr_x;
	twc->tw.width = fg->corr_width;
	twc->tw.height = fg->corr_height;
	__tui_widget_add(twh, &twc->tw);
	return twc;
}

static TEE_Result op_input_paint(struct tui_widget *tw, struct tui_state *ts)
{
	TEE_Result res;
	struct tui_input *ti = to_tui_input(tw);
	char text[MAX_ENTRY_FIELD_LENGTH + 1];
	size_t l = ti->field->bufferLength;
	struct image *image;

	if (!tw->updated)
		return TEE_SUCCESS;

	image = __tui_widget_image_alloc(tw, COLOR_RGB_LIGHT_GOLDENROD);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!l)
		goto display;

	switch (ti->field->mode) {
	case TEE_TUI_TEMPORARY_CLEAR_MODE:
		if (ti->temp_clear) {
			memset(text, '*', l - 1);
			text[l - 1] = ti->field->buffer[l - 1];
			break;
		}

		/*FALLTHROUGH*/
	case TEE_TUI_HIDDEN_MODE:
		memset(text, '*', l);
		break;
	case TEE_TUI_CLEAR_MODE:
		memcpy(text, ti->field->buffer, l);
		break;
	default:
		res = TEE_ERROR_GENERIC;
		goto out;
	}
	text[l] = '\0';

	if (!font_render_text(image, 0, 0, text, COLOR_RGB_BLACK)) {
		res = TEE_ERROR_BAD_FORMAT;
		goto out;
	}
display:
	res = __tui_widget_display(tw, ts, image);
	if (res == TEE_SUCCESS)
		tw->updated = false;
out:
	image_free(image);
	return res;
}

static void op_input_focus(struct tui_widget *tw, struct tui_state *ts)
{
	if (tw != ts->focus) {
		tw->updated = true;
		ts->focus = tw;
		state_set_input(ts, tw);
	}
}

static void op_input_click(struct tui_widget *tw, struct tui_state *ts,
			   ssize_t x __unused, ssize_t y __unused)
{
	if (tw != ts->focus) {
		ts->focus->ops->unfocus(ts->focus, ts);
		tw->ops->focus(tw, ts);
	}
}

static bool op_input_input(struct tui_widget *tw,
			   struct tui_state *ts __unused, uint32_t key)
{
	struct tui_input *ti = to_tui_input(tw);

	if ((key == 0x7f || key == 0x08)) {
		if (ti->field->bufferLength) {
			ti->field->bufferLength--;
			ti->field->buffer[ti->field->bufferLength] = '\0';
			tw->updated = true;
			return true;
		}

		EMSG("Input buffer empty, ignoring key %#" PRIX32, key);
		return false;
	}

	if (ti->field->bufferLength >= ti->max_len) {
		EMSG("Input buffer full, ignoring key %#" PRIX32, key);
		return false;
	}

	if ((ti->field->type == TEE_TUI_ALPHANUMERICAL &&
	     key >= 0x20 && key <= 0x7d) ||
	    (ti->field->type == TEE_TUI_NUMERICAL &&
	     key >= '0' && key <= '9')) {
		ti->field->buffer[ti->field->bufferLength] = key;
		ti->field->bufferLength++;
		ti->temp_clear = true;
		tw->updated = true;
		return true;
	}

	EMSG("Ignoring invalid key %#" PRIX32, key);
	return false;
}

static void op_input_destroy(struct tui_widget *tw)
{
	free(to_tui_input(tw));
}

TEE_TUIEntryFieldType __tui_entry_field_get_input_type(struct tui_widget *tw)
{
	return to_tui_input(tw)->field->type;
}

static const struct tui_widget_ops input_ops = {
	.paint = op_input_paint,
	.focus = op_input_focus,
	.unfocus = __tui_widget_op_unfocus,
	.click = op_input_click,
	.input = op_input_input,
	.destroy = op_input_destroy,
};

static bool is_tui_input(struct tui_widget *tw)
{
	return tw->ops == &input_ops;
}

static struct tui_input *add_entry_input(struct tui_widget_head *twh,
					 struct field_geometry *fg, size_t ypos,
					 TEE_TUIEntryField *ef)
{
	struct tui_input *twi = calloc(1, sizeof(*twi));

	if (!twi)
		return NULL;

	twi->tw.ops = &input_ops;
	twi->tw.updated = true;
	twi->field = ef;
	twi->min_len = ef->minExpectedLength;
	if (ef->maxExpectedLength)
		twi->max_len = ef->maxExpectedLength;
	else
		twi->max_len = fg->input_max_len;
	twi->tw.y = ypos + fg->input_yoffs;
	twi->tw.x = fg->input_x;
	twi->tw.width = fg->input_width;
	twi->tw.height = fg->input_height;
	__tui_widget_add(twh, &twi->tw);
	return twi;
}

static TEE_Result get_entry_field_size(TEE_TUIEntryField *efs, size_t num_efs,
				       TEE_TUIScreenButtonInfo *cbi,
				       size_t scr_width,
				       struct field_geometry *fg)
{
	size_t max_label_width = 0;
	size_t max_len = 0;
	size_t max_width;
	size_t n;
	size_t l;

	/*
	 * An entry field looks like:
	 * +------------------------------------------------------------+
	 * | Label text ...                                             |
	 * +------------------------------------+-----------------------+
	 * | Input field                        | Correction button     |
	 * +------------------------------------+-----------------------+
	 * Each of the boxes in the pictures can have different heights and
	 * width. The correction button should be on the same level (tw.y)
	 * as the input field, this is accomplished by setting the center
	 * line at the same y level.
	 *
	 * If there's multiple entry fields they should have the same width
	 * to line up nicely, this is done by scanning the required maximum
	 * width of each box in entry fields and pick that larges value for
	 * each.
	 */

	fg->corr_width = cbi->buttonWidth;
	fg->corr_height = cbi->buttonHeight;

	fg->input_max_len = font_get_max_field_length(scr_width -
						      fg->corr_width -
						      4 * BORDER_SIZE);
	for (n = 0; n < num_efs; n++) {
		if (efs[n].maxExpectedLength > fg->input_max_len)
			return TEE_ERROR_GENERIC;
		l = efs[n].maxExpectedLength;
		if (l > max_len)
			max_len = l;

		if (efs[n].label) {
			if (!font_check_text_format(efs[n].label, &l,
						    NULL, NULL))
				return TEE_ERROR_GENERIC;
			if (l > max_label_width)
				max_label_width = l;
		}
	}

	fg->input_width = font_get_max_field_width(max_len) + BORDER_SIZE * 2;
	fg->input_height = font_get_text_height() + BORDER_SIZE * 2;

	max_width = MAX(fg->input_width + fg->corr_width, max_label_width);
	fg->max_height = MAX(fg->input_height, fg->corr_height);

	if (max_width > scr_width)
		return TEE_ERROR_GENERIC;

	fg->input_x = (scr_width / 2) - (max_width / 2);
	fg->corr_x = fg->input_x + fg->input_width;

	fg->input_yoffs = (fg->max_height / 2) - (fg->input_height / 2);
	fg->corr_yoffs = (fg->max_height / 2) - (fg->corr_height / 2);
	return TEE_SUCCESS;
}

static TEE_Result set_entry_label(size_t xpos, size_t *ypos, const char *text)
{
	TEE_Result res;
	struct image *image;
	size_t w;
	size_t h;

	if (!text)
		return TEE_SUCCESS;

	if (!font_check_text_format(text, &w, &h, NULL))
		return TEE_ERROR_GENERIC;

	image = image_alloc(w, h, COLOR_RGB_YELLOW);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!font_render_text(image, 0, 0, text, COLOR_RGB_BLACK)) {
		res = TEE_ERROR_GENERIC;
		goto out;
	}

	res = __tui_pta_set_screen_image(xpos, *ypos, image);
	if (res != TEE_SUCCESS)
		goto out;

	(*ypos) += h;
out:
	image_free(image);
	return res;
}

TEE_Result __tui_entry_fields_create(struct tui_widget_head *twh,
				     size_t *ypos, size_t scr_width,
				     TEE_TUIScreenButtonInfo *cbi,
				     TEE_TUIEntryField *efs, size_t num_efs)
{
	TEE_Result res;
	struct field_geometry fg;
	size_t yp = *ypos;
	size_t n;

	for (n = 0; n < num_efs; n++) {
		if (!efs[n].maxExpectedLength)
			efs[n].maxExpectedLength = efs[n].bufferLength;
		efs[n].bufferLength = 0;
	}

	res = get_entry_field_size(efs, num_efs, cbi, scr_width,
				   &fg);
	if (res != TEE_SUCCESS)
		return res;

	for (n = 0; n < num_efs; n++) {
		struct tui_input *twi;

		res = set_entry_label(fg.input_x, &yp, efs[n].label);
		if (res != TEE_SUCCESS)
			return res;

		twi = add_entry_input(twh, &fg, yp, efs + n);
		if (!twi)
			return TEE_ERROR_OUT_OF_MEMORY;

		if (!add_entry_correction(twh, &fg, yp, cbi, &twi->tw))
			return TEE_ERROR_OUT_OF_MEMORY;

		yp += fg.max_height;
	}

	*ypos = yp;

	return TEE_SUCCESS;
}

bool __tui_entry_fields_are_ok(struct tui_widget_head *twh)
{
	struct tui_widget *tw;

	TAILQ_FOREACH(tw, twh, link) {
		if (is_tui_input(tw)) {
			struct tui_input *ti = to_tui_input(tw);

			if (ti->field->bufferLength < ti->min_len)
				return false;
		}
	}

	return true;
}

bool __tui_entry_field_has_temp_clear(struct tui_widget *tw)
{
	if (tw) {
		struct tui_input *ti = to_tui_input(tw);

		return ti->temp_clear;
	}

	return false;
}

void __tui_entry_field_reset_temp_clear(struct tui_widget *tw)
{
	if (tw) {
		struct tui_input *ti = to_tui_input(tw);

		ti->temp_clear = false;
		tw->updated = true;
	}
}

struct tui_widget *__tui_entry_field_find_first(struct tui_widget_head *twh)
{
	struct tui_widget *tw;

	TAILQ_FOREACH(tw, twh, link)
		if (is_tui_input(tw))
			return tw;
	return NULL;
}
