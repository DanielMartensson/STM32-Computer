// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include <tee_api.h>
#include <tee_tui_api.h>
#include <stdlib.h>
#include "font.h"
#include "image.h"
#include "tui_private.h"

struct tui_key {
	char value;
	const char *text;
	struct tui_widget tw;
};

struct tui_switchkey {
	const char *text;
	size_t layout_idx;
	struct tui_widget tw;
};

struct tui_keyboard {
	size_t layout_idx;
	size_t num_layouts;
	struct keyboard_head *kbh;
	struct tui_widget tw;
};

struct keyboard_head {
	TEE_TUIEntryFieldType type;
	struct tui_widget_head twh;
};

struct keyboard_layout {
	TEE_TUIEntryFieldType type;
	const char *values;
	size_t columns;
	size_t rows;
	const char *switch_str;
	size_t switch_idx;
};

static const struct keyboard_layout keyboard_layouts[] = {
	{
		.type = TEE_TUI_ALPHANUMERICAL,
		.values = (const char[]) {
			'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
			'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
			'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,
			'z', 'x', 'c', 'v', 'b', 'n', 'm', 0, 0, 0,
		},
		.columns = 10,
		.rows = 4,
		.switch_str = "<abc>",
		.switch_idx = 1,
	},
	{
		.type = TEE_TUI_ALPHANUMERICAL,
		.values = (const char[]) {
			'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
			'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
			'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0,
			'Z', 'X', 'C', 'V', 'B', 'N', 'M', 0, 0, 0,
		},
		.columns = 10,
		.rows = 4,
		.switch_str = "<ABC>",
		.switch_idx = 2,
	},
	{
		.type = TEE_TUI_ALPHANUMERICAL,
		.values = (const char[]) {
			'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0,
			'!', '@', '#', '$', '%', '^', '&', '*', '-', '+', '=',
			',', '.', '?', ';', ':', '\'', '"', '`', '/', '\\', '_',
			'(', ')', '<', '>', '[', ']', '{', '}', '|', 0, 0,
		},
		.columns = 11,
		.rows = 4,
		.switch_idx = 0,
		.switch_str = "<!@#>",
	},
	{
		.type = TEE_TUI_NUMERICAL,
		.values = (const char []) {
			'1', '2', '3',
			'4', '5', '6',
			'7', '8', '9',
			 0, '0', 0,
		},
		.columns = 3,
		.rows = 4,
	},
};

static void op_dummy_unfocus(struct tui_widget *tw __unused,
			     struct tui_state *ts __unused)
{
	/* Do nothing, this widget can't be in focus */
}

static void op_dummy_focus(struct tui_widget *tw __unused,
			   struct tui_state *ts __unused)
{
	/* Do nothing, this widget can't be in focus */
}

static bool __maybe_unused is_tui_key(struct tui_widget *tw);
static struct tui_key *to_tui_key(struct tui_widget *tw)
{
	assert(is_tui_key(tw));
	return container_of(tw, struct tui_key, tw);
}

static bool __maybe_unused is_tui_switchkey(struct tui_widget *tw);
static struct tui_switchkey *to_tui_switchkey(struct tui_widget *tw)
{
	assert(is_tui_switchkey(tw));
	return container_of(tw, struct tui_switchkey, tw);
}

static bool is_tui_keyboard(struct tui_widget *tw);
static struct tui_keyboard *to_tui_keyboard(struct tui_widget *tw)
{
	assert(is_tui_keyboard(tw));
	return container_of(tw, struct tui_keyboard, tw);
}

static TEE_Result op_key_paint(struct tui_widget *tw, struct tui_state *ts)
{
	TEE_Result res;
	struct tui_key *k = to_tui_key(tw);
	struct image *image;
	char text[2] = { k->value, '\0' };
	const char *t;

	if (k->text)
		t = k->text;
	else
		t = text;

	if (!tw->updated)
		return TEE_SUCCESS;

	image = __tui_widget_image_alloc(tw, COLOR_RGB_LIGHT_GOLDENROD);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!font_render_text(image, 0, 0, t, COLOR_RGB_BLACK)) {
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

static void op_key_click(struct tui_widget *tw, struct tui_state *ts,
			 ssize_t x __unused, ssize_t y __unused)
{
	struct tui_key *k = to_tui_key(tw);

	ts->input->ops->input(ts->input, ts, k->value);
}

static void op_key_destroy(struct tui_widget *tw)
{
	free(to_tui_key(tw));
}

static const struct tui_widget_ops key_ops = {
	.paint = op_key_paint,
	.focus = op_dummy_focus,
	.unfocus = op_dummy_unfocus,
	.click = op_key_click,
	.input = __tui_widget_op_dummy_input,
	.destroy = op_key_destroy,
};

static bool is_tui_key(struct tui_widget *tw)
{
	return tw->ops == &key_ops;
}

static TEE_Result add_key(struct tui_widget_head *twh, char value,
			  const char *text, size_t x, size_t y,
			  size_t w, size_t h)
{
	struct tui_key *tk = calloc(1, sizeof(*tk));

	if (!tk)
		return TEE_ERROR_OUT_OF_MEMORY;
	tk->tw.ops = &key_ops;
	tk->tw.updated = true;
	tk->tw.x = x;
	tk->tw.y = y;
	tk->tw.width = w;
	tk->tw.height = h;
	tk->value = value;
	tk->text = text;
	TAILQ_INSERT_TAIL(twh, &tk->tw, link);
	return TEE_SUCCESS;
}

static TEE_Result op_switchkey_paint(struct tui_widget *tw,
				     struct tui_state *ts)
{
	TEE_Result res;
	struct tui_switchkey *k = to_tui_switchkey(tw);
	struct image *image;

	if (!tw->updated)
		return TEE_SUCCESS;

	image = __tui_widget_image_alloc(tw, COLOR_RGB_LIGHT_GOLDENROD);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!font_render_text(image, 0, 0, k->text, COLOR_RGB_BLACK)) {
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

static void op_switchkey_click(struct tui_widget *tw, struct tui_state *ts,
			       ssize_t x __unused, ssize_t y __unused)
{
	struct tui_switchkey *k = to_tui_switchkey(tw);
	struct tui_keyboard *kb = to_tui_keyboard(ts->keyboard);

	kb->layout_idx = k->layout_idx;
	ts->keyboard->updated = true;
}

static void op_switchkey_destroy(struct tui_widget *tw)
{
	free(to_tui_switchkey(tw));
}

static const struct tui_widget_ops switchkey_ops = {
	.paint = op_switchkey_paint,
	.focus = op_dummy_focus,
	.unfocus = op_dummy_unfocus,
	.click = op_switchkey_click,
	.input = __tui_widget_op_dummy_input,
	.destroy = op_switchkey_destroy,
};

static bool is_tui_switchkey(struct tui_widget *tw)
{
	return tw->ops == &switchkey_ops;
}

static TEE_Result add_switchkey(struct tui_widget_head *twh,
				const char *text, size_t layout_idx,
				size_t x, size_t y)
{
	struct tui_switchkey *k;
	size_t w;
	size_t h;

	if (!font_check_text_format(text, &w, &h, NULL))
		return TEE_ERROR_GENERIC;

	k = calloc(1, sizeof(*k));
	if (!k)
		return TEE_ERROR_OUT_OF_MEMORY;
	k->tw.ops = &switchkey_ops;
	k->tw.updated = true;
	k->tw.x = x;
	k->tw.y = y;
	k->tw.width = w;
	k->tw.height = h;
	k->text = text;
	k->layout_idx = layout_idx;
	TAILQ_INSERT_TAIL(twh, &k->tw, link);
	return TEE_SUCCESS;
}

static TEE_Result op_keyboard_paint(struct tui_widget *tw, struct tui_state *ts)
{
	TEE_Result res;
	struct tui_keyboard *tkb = to_tui_keyboard(tw);
	struct tui_widget *tw_key;
	struct image *image;

	if (!tw->updated)
		return TEE_SUCCESS;

	image = __tui_widget_image_alloc(tw, COLOR_RGB_LIGHT_GRAY);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;
	res = __tui_widget_display(tw, ts, image);
	image_free(image);
	if (res != TEE_SUCCESS)
		return res;

	TAILQ_FOREACH(tw_key, &tkb->kbh[tkb->layout_idx].twh, link) {
		tw_key->updated = true;
		res = tw_key->ops->paint(tw_key, ts);
		if (res != TEE_SUCCESS)
			return res;
	}
	tw->updated = false;
	return TEE_SUCCESS;
}

static void op_keyboard_click(struct tui_widget *tw, struct tui_state *ts,
			      ssize_t x, ssize_t y)
{
	struct tui_keyboard *tkb = to_tui_keyboard(tw);
	struct tui_widget *tw_key;

	tw_key = __tui_widget_find(&tkb->kbh[tkb->layout_idx].twh, x, y);
	if (tw_key)
		tw_key->ops->click(tw_key, ts, x, y);
}

static void op_keyboard_destroy(struct tui_widget *tw)
{
	struct tui_keyboard *tkb = to_tui_keyboard(tw);
	struct tui_widget *tw_key;
	size_t n;

	for (n = 0; n < tkb->num_layouts; n++) {
		while (true) {
			tw_key = TAILQ_FIRST(&tkb->kbh[n].twh);
			if (!tw_key)
				break;
			TAILQ_REMOVE(&tkb->kbh[n].twh, tw_key, link);
			tw_key->ops->destroy(tw_key);
		}
	}
	free(tkb->kbh);
	free(tkb);
}

static const struct tui_widget_ops keyboard_ops = {
	.paint = op_keyboard_paint,
	.focus = op_dummy_focus,
	.unfocus = op_dummy_unfocus,
	.click = op_keyboard_click,
	.input = __tui_widget_op_dummy_input,
	.destroy = op_keyboard_destroy,
};

static bool is_tui_keyboard(struct tui_widget *tw)
{
	return tw->ops == &keyboard_ops;
}

TEE_Result __tui_keyboard_create(struct tui_widget_head *twh, size_t *ypos,
				 size_t scr_width)
{
	TEE_Result res;
	struct tui_keyboard *tkb;
	size_t row;
	size_t col;
	size_t hk_max = 0;
	size_t wk_max = 0;
	size_t hmax = 0;
	size_t wmax = 0;
	size_t h;
	size_t w;
	size_t n;

	for (n = 0; n < ARRAY_SIZE(keyboard_layouts); n++) {
		for (row = 0; row < keyboard_layouts[n].rows; row++) {
			for (col = 0; col < keyboard_layouts[n].columns;
			     col++) {
				char text[2] = {
					keyboard_layouts[n].values[row *
						keyboard_layouts[n].columns +
						col],
					'\0',
				 };

				if (!text[0])
					continue;
				if (!font_check_text_format(text, &w, &h, NULL))
					return TEE_ERROR_GENERIC;
				if (w > wk_max)
					wk_max = w;
				if (h > hk_max)
					hk_max = h;
			}
		}
	}

	for (n = 0; n < ARRAY_SIZE(keyboard_layouts); n++) {
		size_t num_rows = keyboard_layouts[n].rows;

		if (keyboard_layouts[n].type == TEE_TUI_ALPHANUMERICAL)
			num_rows++;
		hmax = MAX(num_rows * hk_max, hmax);
		wmax = MAX(keyboard_layouts[n].columns * wk_max, wmax);
	}

	tkb = calloc(1, sizeof(*tkb));
	if (!tkb)
		return TEE_ERROR_OUT_OF_MEMORY;

	tkb->kbh = calloc(ARRAY_SIZE(keyboard_layouts), sizeof(*tkb->kbh));
	if (!tkb->kbh) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	for (n = 0; n < ARRAY_SIZE(keyboard_layouts); n++)
		TAILQ_INIT(&tkb->kbh[n].twh);

	tkb->tw.ops = &keyboard_ops;
	tkb->tw.updated = true;
	tkb->tw.width = wmax;
	tkb->tw.height = hmax;
	tkb->num_layouts = ARRAY_SIZE(keyboard_layouts);

	if (tkb->tw.width > scr_width) {
		res = TEE_ERROR_GENERIC;
		goto out;
	}
	tkb->tw.x = (scr_width - tkb->tw.width) / 2;
	tkb->tw.y = *ypos;

	for (n = 0; n < ARRAY_SIZE(keyboard_layouts); n++) {
		size_t xoffs;

		xoffs = (wmax - (keyboard_layouts[n].columns * wk_max)) / 2;
		tkb->kbh[n].type = keyboard_layouts[n].type;

		for (row = 0; row < keyboard_layouts[n].rows; row++) {
			for (col = 0; col < keyboard_layouts[n].columns;
			     col++) {
				char value = keyboard_layouts[n].values[row *
					keyboard_layouts[n].columns +
					col];
				if (!value)
					continue;
				res = add_key(&tkb->kbh[n].twh, value, NULL,
					      tkb->tw.x + col * wk_max + xoffs,
					      tkb->tw.y + row * hk_max,
					      wk_max, hk_max);
				if (res != TEE_SUCCESS)
					goto out;
			}
		}

		if (keyboard_layouts[n].type == TEE_TUI_ALPHANUMERICAL) {
			const char *str = "<space>";
			size_t switch_idx = keyboard_layouts[n].switch_idx;

			res = add_switchkey(&tkb->kbh[n].twh,
					    keyboard_layouts[switch_idx].switch_str,
					    switch_idx, tkb->tw.x + xoffs,
					    tkb->tw.y +
					    keyboard_layouts[n].rows * hk_max);
			if (res != TEE_SUCCESS)
				goto out;

			if (!font_check_text_format(str, &w, NULL, NULL)) {
				res = TEE_ERROR_GENERIC;
				goto out;
			}

			res = add_key(&tkb->kbh[n].twh, ' ', str,
				      tkb->tw.x + 5 * wk_max + xoffs,
				      tkb->tw.y +
					keyboard_layouts[n].rows * hk_max,
				      w, hk_max);
			if (res != TEE_SUCCESS)
				goto out;
		}
	}

	TAILQ_INSERT_TAIL(twh, &tkb->tw, link);
out:
	if (res != TEE_SUCCESS)
		op_keyboard_destroy(&tkb->tw);
	return res;
}

void __tui_keyboard_select_layout(struct tui_state *ts)
{
	struct tui_keyboard *kb = to_tui_keyboard(ts->keyboard);
	const TEE_TUIEntryFieldType type =
		__tui_entry_field_get_input_type(ts->input);

	if (type == kb->kbh[kb->layout_idx].type)
		return;
	kb->layout_idx = 0;
	kb->tw.updated = true;

	while (type != kb->kbh[kb->layout_idx].type)
		kb->layout_idx = (kb->layout_idx + 1) % kb->num_layouts;
}

struct tui_widget *__tui_keyboard_find(struct tui_widget_head *twh)
{
	struct tui_widget *tw;

	TAILQ_FOREACH(tw, twh, link)
		if (is_tui_keyboard(tw))
			return tw;
	return NULL;
}
