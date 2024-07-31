// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020-2021, STMicroelectronics
 * Copyright (c) 2016-2021, Linaro Limited
 */

#include <assert.h>
#include <display.h>
#include <drivers/frame_buffer.h>
#include <drivers/stm32_etzpc.h>
#include <io.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/panic.h>
#include <libfdt.h>
#include <mm/core_mmu.h>
#include <mm/core_memprot.h>
#include <stddef.h>
#include <stdint.h>
#include <tee_api_types.h>
#include <trace.h>

static SLIST_HEAD(, disp_dev_list) display_device_list =
	SLIST_HEAD_INITIALIZER(disp_dev_list);

/*
 * Trusted UI frame buffer description
 */
static struct frame_buffer tui_frame_buffer = {
	.width      = 480,
	.height     = 272,
	.width_dpi  = 160,
	.height_dpi = 160,
	.bpp        = FB_24BPP, // never used actually.
};

struct frame_buffer *display_get_frame_buffer(void)
{
	return &tui_frame_buffer;
}

TEE_Result display_get_fb_addr_from_dtb(paddr_t *pa, size_t *size)
{
	static paddr_t fb_pa;
	static ssize_t fb_sz;

	/* Note physical address 0x0 is reserved in OP-TEE core */
	if (!fb_pa) {
		const void *fdt = get_embedded_dt();
		int node = fdt_path_offset(fdt, "/reserved-memory");

		node = fdt_subnode_offset(fdt, node, "optee-framebuffer");
		if (node < 0) {
			EMSG("Can't find optee-framebuffer");
			return TEE_ERROR_ITEM_NOT_FOUND;
		}

		fb_pa = _fdt_reg_base_address(fdt, node);
		fb_sz = _fdt_reg_size(fdt, node);

		DMSG("TUI frame buffer phys range: [%#"PRIxPA" %#"PRIxPA"]",
		     fb_pa, fb_pa + fb_sz - 1);
	}

	if (fb_pa == DT_INFO_INVALID_REG || fb_sz < 0) {
		DMSG("Can't retrieve TUI frame buffer location");
			return TEE_ERROR_GENERIC;
	}

	if (pa)
		*pa = fb_pa;
	if (size)
		*size = (size_t)fb_sz;

	return TEE_SUCCESS;
}

static TEE_Result get_fb_from_dtb(struct frame_buffer *fb)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	paddr_t fb_pa = 0;
	size_t fb_sz = 0;

	res = display_get_fb_addr_from_dtb(&fb_pa, &fb_sz);
	if (res)
		return res;

	if (!core_mmu_add_mapping(MEM_AREA_RAM_SEC, fb_pa, fb_sz)) {
		DMSG("Can't map TUI frame buffer");
		return TEE_ERROR_GENERIC;
	}

	fb->base = phys_to_virt(fb_pa, MEM_AREA_RAM_SEC, fb_sz);
	assert(fb->base);

	DMSG("TUI frame buffer virt range: [%p %#"PRIxPTR"]",
	     fb->base, (uintptr_t)fb->base + fb_sz - 1);

	return TEE_SUCCESS;
}

TEE_Result display_init(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t width = 0, height = 0;
	struct disp_dev_list *ddev = NULL;

	if (get_fb_from_dtb(&tui_frame_buffer))
		panic();

	SLIST_FOREACH(ddev, &display_device_list, link) {
		if (ddev->device_init) {
			res = ddev->device_init(ddev->device);
			if (res != TEE_SUCCESS)
				return res;
		}
	}

	SLIST_FOREACH(ddev, &display_device_list, link) {
		if (ddev->device_get_display_size) {
			res = ddev->device_get_display_size(ddev->device,
							    &width,
							    &height);
			if (res != TEE_SUCCESS)
				return res;
		}
	}

	DMSG("TUI frame buffer size: %"PRIu32" x %"PRIu32, width, height);
	tui_frame_buffer.width = width;
	tui_frame_buffer.height = height;

	SLIST_FOREACH(ddev, &display_device_list, link) {
		if (ddev->device_activate) {
			res = ddev->device_activate(ddev->device,
						    display_get_frame_buffer(),
						    0, 0);
			if (res != TEE_SUCCESS)
				return res;
		}
	}

	return TEE_SUCCESS;
}

void display_final(void)
{
	struct disp_dev_list *ddev = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	SLIST_FOREACH(ddev, &display_device_list, link) {
		if (ddev->device_final) {
			res = ddev->device_final(ddev->device);
			if (res != TEE_SUCCESS)
				return;
		}
	}
}

void display_register_device(struct disp_dev_list *ddev)
{
	SLIST_INSERT_HEAD(&display_device_list, ddev, link);
}
