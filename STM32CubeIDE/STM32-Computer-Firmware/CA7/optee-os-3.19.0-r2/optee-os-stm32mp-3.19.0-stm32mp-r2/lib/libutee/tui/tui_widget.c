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

struct image *__tui_widget_image_alloc(struct tui_widget *tw, uint32_t color)
{
	return image_alloc(tw->width, tw->height, color);
}

TEE_Result __tui_widget_display(struct tui_widget *tw, struct tui_state *ts,
				struct image *image)
{
	if (tw == ts->focus)
		image_set_border(image, BORDER_SIZE, COLOR_RGB_RED);

	return __tui_pta_set_screen_image(tw->x, tw->y, image);
}

bool __tui_widget_op_dummy_input(struct tui_widget *tw __unused,
				 struct tui_state *ts __unused,
				 uint32_t key __unused)
{
	EMSG("Ignoring key %#" PRIX32, key);
	return false;
}

void __tui_widget_op_unfocus(struct tui_widget *tw, struct tui_state *ts)
{
	if (tw == ts->focus) {
		tw->updated = true;
		ts->focus = NULL;
	}
}

void __tui_widget_add(struct tui_widget_head *twh, struct tui_widget *tw)
{
	struct tui_widget *t;

	TAILQ_FOREACH(t, twh, link) {
		if (tw->y < t->y || (tw->y == t->y && tw->x < t->y)) {
			TAILQ_INSERT_BEFORE(t, tw, link);
			return;
		}
	}
	TAILQ_INSERT_TAIL(twh, tw, link);
}

struct tui_widget *__tui_widget_find(struct tui_widget_head *twh, size_t xpos,
				     size_t ypos)
{
	struct tui_widget *tw;

	TAILQ_FOREACH(tw, twh, link)
		if (xpos >= tw->x && xpos < (tw->x + tw->width) &&
		    ypos >= tw->y && ypos < (tw->y + tw->height))
			return tw;
	return NULL;
}

/*
 * Button implementation.
 */

struct tui_button {
	TEE_TUIButtonType type;
	char *text;
	struct tui_widget tw;
};

static bool __maybe_unused is_tui_button(struct tui_widget *tw);

static struct tui_button *to_tui_button(struct tui_widget *tw)
{
	assert(is_tui_button(tw));
	return container_of(tw, struct tui_button, tw);
}

static void op_button_focus(struct tui_widget *tw, struct tui_state *ts)
{
	if (tw != ts->focus) {
		tw->updated = true;
		ts->focus = tw;
	}
}

static TEE_Result op_button_paint(struct tui_widget *tw, struct tui_state *ts)
{
	TEE_Result res;
	struct tui_button *b = to_tui_button(tw);
	struct image *image;

	if (!tw->updated)
		return TEE_SUCCESS;

	image = __tui_widget_image_alloc(tw, COLOR_RGB_LIGHT_GOLDENROD);
	if (!image)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!font_render_text(image, 0, 0, b->text, COLOR_RGB_BLACK)) {
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

static void op_button_click(struct tui_widget *tw, struct tui_state *ts,
			    ssize_t x __unused, ssize_t y __unused)
{
	struct tui_button *b = to_tui_button(tw);

	ts->clicked_button = &b->type;
	if (tw != ts->focus) {
		ts->focus->ops->unfocus(ts->focus, ts);
		tw->ops->focus(tw, ts);
	}
}

static void op_button_destroy(struct tui_widget *tw)
{
	free(to_tui_button(tw));
}

static const struct tui_widget_ops button_ops = {
	.paint = op_button_paint,
	.focus = op_button_focus,
	.unfocus = __tui_widget_op_unfocus,
	.click = op_button_click,
	.input = __tui_widget_op_dummy_input,
	.destroy = op_button_destroy,
};

static bool is_tui_button(struct tui_widget *tw)
{
	return tw->ops == &button_ops;
}

TEE_Result __tui_button_create(struct tui_widget_head *twh,
			       TEE_TUIButtonType btype,
			       TEE_TUIScreenButtonInfo *binfo,
			       const TEE_TUIButton *custom_button __unused,
			       enum button_pos bpos,
			       size_t scr_width, size_t scr_height)
{
	TEE_Result res;
	struct tui_button *b = calloc(1, sizeof(*b));
	size_t width;
	size_t height;

	if (!b)
		return TEE_ERROR_OUT_OF_MEMORY;

	b->tw.ops = &button_ops;
	b->tw.updated = true;
	b->type = btype;

	b->text = binfo->buttonText;
	if (!font_check_text_format(b->text, &width, &height, NULL)) {
		res = TEE_ERROR_GENERIC;
		goto out;
	}

	b->tw.width = width + BORDER_SIZE * 2;
	b->tw.height = height + BORDER_SIZE * 2;
	b->tw.y = scr_height - b->tw.height;
	switch (bpos) {
	case POS_LEFT:
		b->tw.x = 0;
		break;
	case POS_MIDDLE:
		b->tw.x = (scr_width / 2) - (b->tw.width / 2);
		break;
	case POS_RIGHT:
		b->tw.x = scr_width - b->tw.width;
		break;
	default:
		res = TEE_ERROR_GENERIC;
		goto out;
	}

	res = TEE_SUCCESS;
	__tui_widget_add(twh, &b->tw);
out:
	if (res != TEE_SUCCESS)
		free(b);
	return res;
}
