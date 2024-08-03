/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#ifndef __TUI_PRIVATE_H
#define __TUI_PRIVATE_H

#include <sys/queue.h>
#include <tee_api.h>
#include <tee_tui_api.h>
#include <types_ext.h>
#include <util.h>
#include <assert.h>
#include "image.h"

#define MAX_ENTRY_FIELDS	2
#define MAX_ENTRY_FIELD_LENGTH	32
#define BORDER_SIZE		2

enum button_pos {
	POS_LEFT,
	POS_MIDDLE,
	POS_RIGHT,
};

struct tui_state {
	TEE_TUIButtonType *clicked_button;
	struct tui_widget *focus;
	struct tui_widget *input;
	struct tui_widget *keyboard;
};

struct tui_widget_ops {
	TEE_Result (*paint)(struct tui_widget *tw, struct tui_state *ts);
	void (*focus)(struct tui_widget *tw, struct tui_state *ts);
	void (*unfocus)(struct tui_widget *tw, struct tui_state *ts);
	void (*click)(struct tui_widget *tw, struct tui_state *ts,
		      ssize_t x, ssize_t y);
	bool (*input)(struct tui_widget *tw, struct tui_state *ts,
		      uint32_t key);
	void (*destroy)(struct tui_widget *tw);
};

struct tui_widget {
	size_t x;
	size_t y;
	size_t width;
	size_t height;
	bool updated;
	const struct tui_widget_ops *ops;
	TAILQ_ENTRY(tui_widget) link;
};

TAILQ_HEAD(tui_widget_head, tui_widget);

/*
 * All functions here are prefixed with __tui since they are global symbols
 * even if not directly exported to the TA.
 */

TEE_Result __tui_pta_get_screen_info(size_t *width, size_t *height,
				     size_t *width_dpi, size_t *height_dpi,
				     uint32_t *bpp);
TEE_Result __tui_pta_init_session(void);
TEE_Result __tui_pta_close_session(void);
TEE_Result __tui_pta_init_screen(uint32_t color);
TEE_Result __tui_pta_set_screen_image(size_t xpos, size_t ypos,
				      struct image *image);
TEE_Result __tui_pta_display_screen(bool clear_input, size_t timeout,
				    bool *is_timedout, bool *is_key,
				    uint32_t *key, size_t *xpos, size_t *ypos);

void __tui_widget_add(struct tui_widget_head *twh, struct tui_widget *tw);
struct image *__tui_widget_image_alloc(struct tui_widget *tw, uint32_t color);
TEE_Result __tui_widget_display(struct tui_widget *tw, struct tui_state *ts,
				struct image *image);
struct tui_widget *__tui_widget_find(struct tui_widget_head *twh, size_t xpos,
				     size_t ypos);
bool __tui_widget_op_dummy_input(struct tui_widget *tw, struct tui_state *ts,
				 uint32_t key);
void __tui_widget_op_unfocus(struct tui_widget *tw, struct tui_state *ts);

TEE_Result __tui_button_create(struct tui_widget_head *twh,
			       TEE_TUIButtonType btype,
			       TEE_TUIScreenButtonInfo *binfo,
			       const TEE_TUIButton *custom_button,
			       enum button_pos bpos, size_t scr_width,
			       size_t scr_height);

TEE_Result __tui_keyboard_create(struct tui_widget_head *twh, size_t *ypos,
				 size_t scr_width);
void __tui_keyboard_select_layout(struct tui_state *ts);
struct tui_widget *__tui_keyboard_find(struct tui_widget_head *twh);

TEE_Result __tui_entry_fields_create(struct tui_widget_head *twh,
				     size_t *ypos, size_t scr_width,
				     TEE_TUIScreenButtonInfo *cbi,
				     TEE_TUIEntryField *efs, size_t num_efs);
TEE_TUIEntryFieldType __tui_entry_field_get_input_type(struct tui_widget *tw);
struct tui_widget *__tui_entry_field_find_first(struct tui_widget_head *twh);
bool __tui_entry_field_has_temp_clear(struct tui_widget *tw);
void __tui_entry_field_reset_temp_clear(struct tui_widget *tw);
bool __tui_entry_fields_are_ok(struct tui_widget_head *twh);

#endif /*__TUI_PRIVATE_H*/

