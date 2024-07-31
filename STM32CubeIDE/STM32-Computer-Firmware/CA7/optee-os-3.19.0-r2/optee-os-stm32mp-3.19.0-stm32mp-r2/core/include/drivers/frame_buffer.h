/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */
#ifndef __DRIVERS__FRAME_BUFFER_H
#define __DRIVERS__FRAME_BUFFER_H

#include <types_ext.h>

enum frame_buffer_bpp {
	FB_24BPP,
};

struct frame_buffer {
	size_t width;
	size_t height;
	size_t width_dpi;
	size_t height_dpi;
	enum frame_buffer_bpp bpp;
	void *base;
};

size_t frame_buffer_get_image_size(struct frame_buffer *fb, size_t width,
				   size_t height);
void frame_buffer_clear(struct frame_buffer *fb, uint32_t color);
void frame_buffer_set_image(struct frame_buffer *fb, size_t xpos, size_t ypos,
			    size_t width, size_t height, const void *image);

#endif /*__DRIVERS__FRAME_BUFFER_H*/
