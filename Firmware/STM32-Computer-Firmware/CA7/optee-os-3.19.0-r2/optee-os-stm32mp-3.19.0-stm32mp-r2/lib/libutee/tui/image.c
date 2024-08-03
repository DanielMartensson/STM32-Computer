// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include <stdlib.h>
#include <utee_defines.h>
#include "image.h"

static uint32_t color_to_pixel(uint32_t color)
{
#if defined(CFG_STM32_LTDC)
	return color | 0xFF000000;
#else
	/* Convert from ARGB to RGBA and byte swap */
	return TEE_U32_BSWAP((color >> 24) | (color << 8));
#endif
}

struct image *image_alloc(size_t width, size_t height, uint32_t color)
{
	struct image *image = malloc(sizeof(*image));
	size_t n;
	uint32_t *b;

	if (!image)
		return NULL;
	image->blen = sizeof(uint32_t) * height * width;
	image->buf = malloc(image->blen);
	if (!image->buf) {
		free(image);
		return NULL;
	}
	b = image->buf;
	image->height = height;
	image->width = width;
	for (n = 0; n < (height * width); n++)
		b[n] = color_to_pixel(color);
	return image;
}

void image_free(struct image *image)
{
	if (image) {
		free(image->buf);
		free(image);
	}
}

void *image_get_pixel_ptr(struct image *image, size_t x, size_t y)
{
	uint32_t *b = image->buf;
	size_t pos;

	if (x >= image->width || y >= image->height)
		return NULL;

	pos = y * image->width + x;
	if (pos >= image->blen)
		return NULL;

	return b + pos;
}

bool image_set_pixel(struct image *image, size_t x, size_t y, uint32_t color)
{
	uint32_t *p = image_get_pixel_ptr(image, x, y);

	if (!p)
		return false;
	*p = color_to_pixel(color);
	return true;
}

bool image_set_border(struct image *image, size_t size, uint32_t color)
{
	size_t x;
	size_t y;

	/* Size * 2 since the border appears on both sides etc */
	if (size * 2 > image->width || size * 2 > image->height)
		return false;

	/* Top horizonal line */
	for (y = 0; y < size; y++)
		for (x = 0; x < image->width; x++)
			if (!image_set_pixel(image, x, y, color))
				return false;

	/* Bottom horizonal line */
	for (y = image->height - size; y < image->height; y++)
		for (x = 0; x < image->width; x++)
			if (!image_set_pixel(image, x, y, color))
				return false;

	/* Left vertical line */
	for (y = 0; y < image->height; y++)
		for (x = 0; x < size; x++)
			if (!image_set_pixel(image, x, y, color))
				return false;

	/* Right vertical line */
	for (y = 0; y < image->height; y++)
		for (x = image->width - size; x < image->width; x++)
			if (!image_set_pixel(image, x, y, color))
				return false;

	return true;
}
