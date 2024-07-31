/*
 * Copyright (c) 2023-2024, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdint.h>

#include <bl31/bl31.h>
#include <common/bl_common.h>
#include <common/runtime_svc.h>
#include <drivers/generic_delay_timer.h>
#include <drivers/st/stm32_console.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <plat/common/platform.h>

#include <platform_def.h>
#include <stm32mp2_context.h>

static entry_point_info_t bl32_image_ep_info;
static entry_point_info_t bl33_image_ep_info;

void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
				u_register_t arg2, u_register_t arg3)
{
	bl_params_t *params_from_bl2;
	char name[STM32_SOC_NAME_SIZE];
	int ret;

	stm32mp_setup_early_console();

	mmap_add_region(BL_CODE_BASE, BL_CODE_BASE,
			BL_CODE_END - BL_CODE_BASE,
			MT_CODE | MT_SECURE);

	/*
	 * Map soc_fw_config device tree with secure property, i.e. default region.
	 * DDR region definitions will be finalized at BL32 level.
	 */
	mmap_add_region(arg1, arg1, STM32MP_SOC_FW_CONFIG_MAX_SIZE, MT_RO_DATA | MT_SECURE);

#if USE_COHERENT_MEM
	/* Map coherent memory */
	mmap_add_region(BL_COHERENT_RAM_BASE, BL_COHERENT_RAM_BASE,
			BL_COHERENT_RAM_END - BL_COHERENT_RAM_BASE,
			MT_DEVICE | MT_RW | MT_SECURE);
#endif

	configure_mmu();

	ret = dt_open_and_check(arg1);
	if (ret < 0) {
		EARLY_ERROR("%s: failed to open DT (%d)\n", __func__, ret);
		panic();
	}

	ret = stm32mp2_clk_init();
	if (ret < 0) {
		EARLY_ERROR("%s: failed init clocks (%d)\n", __func__, ret);
		panic();
	}

	stm32_tamp_nvram_init();

	(void)stm32mp_uart_console_setup();

	assert(arg0 != 0UL);
	params_from_bl2 = (bl_params_t *)arg0;
	assert(params_from_bl2 != NULL);
	assert(params_from_bl2->h.type == PARAM_BL_PARAMS);
	assert(params_from_bl2->h.version >= VERSION_2);

	bl_params_node_t *bl_params = params_from_bl2->head;

	while (bl_params != NULL) {
		/*
		 * Copy BL33 entry point information.
		 * They are stored in Secure RAM, in BL2's address space.
		 */
		if (bl_params->image_id == BL33_IMAGE_ID) {
			bl33_image_ep_info = *bl_params->ep_info;
			/*
			 *  Check if hw_configuration is given to BL32 and
			 *  share it to BL33
			 */
			if (arg2 != 0U) {
				bl33_image_ep_info.args.arg0 = 0U;
				bl33_image_ep_info.args.arg1 = 0U;
				bl33_image_ep_info.args.arg2 = arg2;
			}
		}

		if (bl_params->image_id == BL32_IMAGE_ID) {
			bl32_image_ep_info = *bl_params->ep_info;

			if (arg2 != 0U) {
				bl32_image_ep_info.args.arg3 = arg2;
			}
		}

		bl_params = bl_params->next_params_info;
	}

	/*
	 * This stm32mp_get_soc_name() call is mandatory here to store SoC part number,
	 * and avoid using BL31 DT at runtime to check stm32mp_is_single_core().
	 */
	stm32mp_get_soc_name(name);
	VERBOSE("CPU: %s\n", name);
}

void bl31_plat_arch_setup(void)
{
	generic_delay_timer_init();

	stm32mp_gic_init();

#if !STM32MP_UART_PROGRAMMER && !STM32MP_USB_PROGRAMMER
	if (stm32mp_is_wakeup_from_standby()) {
		/* Initialize the runtime services e.g. PSCI. */
		runtime_svc_init();

		stm32_pm_context_restore();

		/* Jump manually to BL31 warm entry point, with MMU disabled. */
		dsbsy();
		flush_dcache_range(STM32MP_SYSRAM_BASE, STM32MP_SYSRAM_SIZE);
		disable_mmu_el3();
		bl31_warm_entrypoint();
		panic();
	}
#endif /* !STM32MP_UART_PROGRAMMER && !STM32MP_USB_PROGRAMMER */
}

void bl31_platform_setup(void)
{
}

void bl31_plat_runtime_setup(void)
{
	/* We reinit the nvram driver Optee should have update the register access configuration
	 * based on the device tree
	 */
	int ret = stm32_tamp_nvram_update_rights();

	if (ret != 0) {
		ERROR("Failed to update TAMP backup registers rights\n");
		panic();
	}
	console_switch_state(CONSOLE_FLAG_RUNTIME);
}

entry_point_info_t *bl31_plat_get_next_image_ep_info(unsigned int type)
{
	if (type == NON_SECURE)
		return &bl33_image_ep_info;
	if (type == SECURE)
		return &bl32_image_ep_info;

	return NULL;
}

#if STM32MP_UART_PROGRAMMER || STM32MP_USB_PROGRAMMER
static const plat_psci_ops_t null_psci_ops;

/* Stub PSCI platform functions */
plat_local_state_t plat_get_target_pwr_state(unsigned int lvl,
					     const plat_local_state_t *states,
					     unsigned int ncpu)
{
	return 0U;
}

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	*psci_ops = &null_psci_ops;
	return 0;
}
#endif /* STM32MP_UART_PROGRAMMER || STM32MP_USB_PROGRAMMER */
