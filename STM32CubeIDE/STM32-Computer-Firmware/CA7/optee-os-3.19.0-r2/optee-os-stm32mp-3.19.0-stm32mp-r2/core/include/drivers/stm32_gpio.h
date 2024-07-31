/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2017-2019, STMicroelectronics
 *
 * STM32 GPIO driver relies on platform util fiunctions to get base address
 * and clock ID of the GPIO banks. The drvier API allows to retrieve pin muxing
 * configuration for given nodes and load them at runtime. A pin control
 * instance provide an active and a standby configuration. Pin onwer is
 * responsible to load to expected configuration during PM state transitions
 * as STM32 GPIO driver does no register callbacks to the PM framework.
 */

#ifndef __STM32_GPIO_H
#define __STM32_GPIO_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/queue.h>
#include <tee_api_types.h>

#define GPIO_MODE_INPUT		U(0x0)
#define GPIO_MODE_OUTPUT	U(0x1)
#define GPIO_MODE_ALTERNATE	U(0x2)
#define GPIO_MODE_ANALOG	U(0x3)

#define GPIO_OTYPE_PUSH_PULL	U(0x0)
#define GPIO_OTYPE_OPEN_DRAIN	U(0x1)

#define GPIO_OSPEED_LOW		U(0x0)
#define GPIO_OSPEED_MEDIUM	U(0x1)
#define GPIO_OSPEED_HIGH	U(0x2)
#define GPIO_OSPEED_VERY_HIGH	U(0x3)

#define GPIO_PUPD_NO_PULL	U(0x0)
#define GPIO_PUPD_PULL_UP	U(0x1)
#define GPIO_PUPD_PULL_DOWN	U(0x2)

#define GPIO_OD_LEVEL_LOW	U(0x0)
#define GPIO_OD_LEVEL_HIGH	U(0x1)

/*
 * GPIO configuration description structured as single 16bit word
 * for efficient save/restore when GPIO pin suspends or resumes.
 *
 * @mode:       One of GPIO_MODE_*
 * @otype:      One of GPIO_OTYPE_*
 * @ospeed:     One of GPIO_OSPEED_*
 * @pupd:       One of GPIO_PUPD_*
 * @od:         One of GPIO_OD_*
 * @af:         Alternate function numerical ID between 0 and 15
 */
struct gpio_cfg {
	uint16_t mode:		2;
	uint16_t otype:		1;
	uint16_t ospeed:	2;
	uint16_t pupd:		2;
	uint16_t od:		1;
	uint16_t af:		4;
};

/*
 * Descrption of a pin and its 2 states muxing
 *
 * @bank: GPIO bank identifier as assigned by the platform
 * @pin: Pin number in the GPIO bank
 * @config: Configuratioh in active state
 * @link: Link to chain stm32_pinctrl structure in the list
 */
struct stm32_pinctrl {
	uint8_t bank;
	uint8_t pin;
	struct gpio_cfg config;

	STAILQ_ENTRY(stm32_pinctrl) link;
};

/*
 * Structure used to define list of stm32_pinctrl
 *
 * @stm32_pinctrl_list	Structure name
 * @stm32_pinctrl	Element of the structure
 */
STAILQ_HEAD(stm32_pinctrl_list, stm32_pinctrl);

/* Get thr GPIO ID related to the pin referred by @pin */
unsigned int stm32_pinctrl_get_gpio_id(struct stm32_pinctrl *pin);

/*
 * Apply series of pin muxing configuration
 *
 * @list: List of the pinctrl configuration to load
 */
void stm32_pinctrl_load_config(struct stm32_pinctrl_list *list);

/*
 * Get a pinctrl configuration reference from an indexed DT pinctrl property
 *
 * @fdt: device tree
 * @node: device node in the device tree
 * @index: Index of the pinctrl property
 * @plist: Output pinctrl list reference
 *
 * Return a TEE_Result compliant code
 */
TEE_Result stm32_pinctrl_dt_get_by_index(const void *fdt, int nodeoffset,
					 unsigned int index,
					 struct stm32_pinctrl_list **plist);

/*
 * Get a pinctrl configuration reference from a named DT pinctrl property
 *
 * @fdt: device tree
 * @node: device node in the device tree
 * @name: Name of the pinctrl
 * @plist: Output pinctrl list reference
 *
 * Return a TEE_Result compliant code
 */
TEE_Result stm32_pinctrl_dt_get_by_name(const void *fdt, int nodeoffset,
					const char *name,
					struct stm32_pinctrl_list **plist);
/*
 * Read a pinctrl configuration from an indexed DT pinctrl property
 * this configuration is not applied as it is done in
 * stm32_pinctrl_dt_get_by_index()
 *
 * @fdt: device tree
 * @node: device node in the device tree
 * @index: Index of the pinctrl property
 * @plist: Output pinctrl list reference
 *
 * Return a TEE_Result compliant code
 */
TEE_Result stm32_pinctrl_dt_read_by_index(const void *fdt, int nodeoffset,
					  unsigned int index,
					  struct stm32_pinctrl_list **plist);

/*
 * Get the gpio_ops handle for a GPIO
 *
 * Return the generic gpio_ops for GPIOS
 */
const struct gpio_ops *stm32_gpio_get_ops(void);

#ifdef CFG_STM32_GPIO
/*
 * Configure pin muxing access permission: can be secure or not
 *
 * @bank: GPIO bank identifier as assigned by the platform
 * @pin: Pin number in the GPIO bank
 * @secure: True if pin is secure, false otherwise
 *
 * Return a TEE_Resutl compliant code
 */
TEE_Result stm32_gpio_set_secure_cfg(unsigned int bank, unsigned int pin,
				     bool secure);

/*
 * Configure pin muxing access permission: can be secure or not
 *
 * @list: Pinctrl list reference
 * @secure: True if referenced pins are secure, false otherwise
 *
 * Return a TEE_Resutl compliant code
 */
TEE_Result stm32_pinctrl_set_secure_cfg(struct stm32_pinctrl_list *list,
					bool secure);
#else
static inline TEE_Result stm32_gpio_set_secure_cfg(unsigned int bank __unused,
						   unsigned int pin __unused,
						   bool secure __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

static inline
TEE_Result stm32_pinctrl_set_secure_cfg(struct stm32_pinctrl_list *l __unused,
					bool secure __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}
#endif
#endif /*__STM32_GPIO_H*/
