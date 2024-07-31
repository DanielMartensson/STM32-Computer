/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2016-2021, Linaro Limited
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <drivers/frame_buffer.h>
#include <tee_api_types.h>
#include <sys/queue.h>

struct disp_dev_list {
	void *device;
	TEE_Result (*device_init)(void *device);
	TEE_Result (*device_final)(void *device);
	TEE_Result (*device_activate)(void *device,
				      const struct frame_buffer *fb,
				      uint32_t x0, uint32_t y0);
	TEE_Result (*device_get_display_size)(void *device, uint32_t *width,
					      uint32_t *height);
	SLIST_ENTRY(disp_dev_list) link;
};

#ifdef CFG_DISPLAY
TEE_Result display_init(void);
struct frame_buffer *display_get_frame_buffer(void);
void display_final(void);
void display_register_device(struct disp_dev_list *ddev);
TEE_Result display_get_fb_addr_from_dtb(paddr_t *pa, size_t *size);
#else
static inline TEE_Result display_init(void)
{ return TEE_ERROR_NOT_SUPPORTED; }

static inline struct frame_buffer *display_get_frame_buffer(void)
{ return NULL; }

static inline void display_final(void)
{ }

static inline void display_register_device(struct disp_dev_list *ddev __unused)
{ }

static inline TEE_Result display_get_fb_addr_from_dtb(paddr_t *pa __unused,
					size_t *size __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}
#endif /* CFG_DISPLAY */

#endif /* __DISPLAY_H */

