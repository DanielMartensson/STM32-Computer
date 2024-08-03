/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#ifndef __FONT_H
#define __FONT_H

#include <types_ext.h>
#include "image.h"

struct font_letter {
	const unsigned char *blob;
	unsigned int blob_size;
	unsigned int width;
};

struct font {
	unsigned int first;
	unsigned int last;
	const struct font_letter *letters;
	unsigned int height;
	unsigned int max_width;
};

bool font_set_fonts(const struct font *regular, const struct font *bold);

bool font_check_text_format(const char *text, size_t *width, size_t *height,
			    size_t *last_index);

size_t font_get_max_field_length(size_t width);

size_t font_get_max_field_width(size_t num_letters);

size_t font_get_text_height(void);

bool font_render_text(struct image *image, size_t xpos, size_t ypos,
		      const char *text, uint32_t color);

#endif /*__FONT_H*/
