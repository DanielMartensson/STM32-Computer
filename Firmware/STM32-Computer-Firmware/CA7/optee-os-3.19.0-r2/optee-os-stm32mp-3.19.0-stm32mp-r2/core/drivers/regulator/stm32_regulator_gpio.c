// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020-2022, STMicroelectronics
 */

#include <drivers/regulator.h>
#include <drivers/stm32_gpio.h>
#include <gpio.h>
#include <kernel/dt.h>
#include <libfdt.h>
#include <stdio.h>
#include <stm32_util.h>
#include <trace.h>

#define GPIO_REGULATOR_NAME_LEN		U(16)
#define MAX_VOLTAGE_LIST_SIZE		U(2)

struct gpio_regul {
	struct regul_desc desc;
	struct regul_ops ops;
	unsigned int enable_gpio;
	unsigned int voltage_gpio;
	uint16_t gpio_voltage_table[MAX_VOLTAGE_LIST_SIZE];
	char name[GPIO_REGULATOR_NAME_LEN];
	bool enable_active_high;
	bool voltage_level_high;
};

static TEE_Result set_state_always_on(const struct regul_desc *desc __unused,
				      bool enable __unused)
{
	return TEE_SUCCESS;
}

static TEE_Result get_state_always_on(const struct regul_desc *desc __unused,
				      bool *enabled __unused)
{
	*enabled = true;

	return TEE_SUCCESS;
}

static TEE_Result gpio_set_state(const struct regul_desc *desc, bool enable)
{
	struct gpio_regul *gr = (struct gpio_regul *)desc->driver_data;
	enum gpio_level level = GPIO_LEVEL_LOW;

	FMSG("regul %s set state=%u\n", desc->node_name, enable);

	if (!gr->enable_active_high)
		enable = !enable;

	level = enable ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW;

	stm32_gpio_get_ops()->set_value(NULL, gr->enable_gpio, level);

	return TEE_SUCCESS;
}

static TEE_Result gpio_get_state(const struct regul_desc *desc, bool *enabled)
{
	struct gpio_regul *gr = (struct gpio_regul *)desc->driver_data;
	enum gpio_level level = GPIO_LEVEL_LOW;

	level = stm32_gpio_get_ops()->get_value(NULL, gr->enable_gpio);

	*enabled = level == GPIO_LEVEL_HIGH;

	if (!gr->enable_active_high)
		*enabled = !(*enabled);

	FMSG("regul %s get state=%u\n", desc->node_name, *enabled);

	return TEE_SUCCESS;
}

static TEE_Result gpio_set_voltage(const struct regul_desc *desc, uint16_t mv)
{
	struct gpio_regul *gr = (struct gpio_regul *)desc->driver_data;
	enum gpio_level level = GPIO_LEVEL_LOW;

	FMSG("%s: set volt", desc->node_name);

	if (mv == gr->gpio_voltage_table[1])
		level = GPIO_LEVEL_HIGH;
	else if (mv != gr->gpio_voltage_table[0])
		return TEE_ERROR_BAD_PARAMETERS;

	if (!gr->voltage_level_high)
		level = level == GPIO_LEVEL_HIGH ? GPIO_LEVEL_LOW :
						   GPIO_LEVEL_HIGH;

	stm32_gpio_get_ops()->set_value(NULL, gr->voltage_gpio, level);

	return TEE_SUCCESS;
}

static TEE_Result gpio_get_voltage(const struct regul_desc *desc, uint16_t *mv)
{
	struct gpio_regul *gr = (struct gpio_regul *)desc->driver_data;
	enum gpio_level level = GPIO_LEVEL_LOW;

	FMSG("%s: get volt", desc->node_name);

	level = stm32_gpio_get_ops()->get_value(NULL, gr->voltage_gpio);

	if (!gr->voltage_level_high)
		level = level == GPIO_LEVEL_HIGH ? GPIO_LEVEL_LOW :
						   GPIO_LEVEL_HIGH;

	*mv = level == GPIO_LEVEL_HIGH ? gr->gpio_voltage_table[1] :
					 gr->gpio_voltage_table[0];

	return TEE_SUCCESS;
}

static TEE_Result gpio_list_voltages(const struct regul_desc *desc,
				     uint16_t **levels, size_t *count)
{
	struct gpio_regul *gr = (struct gpio_regul *)desc->driver_data;

	*count = 2;
	*levels = gr->gpio_voltage_table;

	return TEE_SUCCESS;
}

static TEE_Result get_pinctrl_from_index(struct stm32_pinctrl_list *list,
					 size_t index,
					 struct stm32_pinctrl **pin)
{
	size_t count = 0;

	STAILQ_FOREACH(*pin, list, link) {
		if (count == index)
			return TEE_SUCCESS;

		count++;
	}

	EMSG("Pin index not found in pinctrl list");

	return TEE_ERROR_GENERIC;
}

static TEE_Result gpio_regulator_probe(const void *fdt, int node,
				       const void *compat_data __unused)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t len = 0;
	struct gpio_regul *gr = NULL;
	const char *reg_name = NULL;
	const fdt32_t *cuint = NULL;
	struct stm32_pinctrl_list *list = NULL;

	gr = calloc(1, sizeof(*gr));
	if (!gr)
		return TEE_ERROR_OUT_OF_MEMORY;

	reg_name = fdt_get_name(fdt, node, NULL);
	len = snprintf(gr->name, sizeof(gr->name) - 1, "%s", reg_name);
	if (len <= 0 ||  (len >= (sizeof(gr->name) - 1))) {
		res = TEE_ERROR_BAD_PARAMETERS;
		goto err;
	}

	FMSG("%s: probe", gr->name);

	gr->desc.node_name = gr->name;
	gr->desc.driver_data = gr;

	gr->ops.set_state = set_state_always_on;
	gr->ops.get_state = get_state_always_on;
	gr->desc.ops = &gr->ops;

	res = stm32_pinctrl_dt_get_by_index(fdt, node, 0, &list);
	if (res)
		goto err;

	cuint = fdt_getprop(fdt, node, "st,voltage_pin_index", NULL);
	if (cuint) {
		size_t index = (size_t)fdt32_to_cpu(*cuint);
		struct stm32_pinctrl *pin = NULL;
		const uint32_t *prop = NULL;
		int count = 0;

		res = get_pinctrl_from_index(list, index, &pin);
		if (res)
			goto err;

		gr->voltage_gpio = stm32_pinctrl_get_gpio_id(pin);

		prop = fdt_getprop(fdt, node, "states", &count);
		if (!prop) {
			res = TEE_ERROR_ITEM_NOT_FOUND;
			goto err;
		}

		/* The driver support only one GPIO to adjust voltage */
		if ((count / sizeof(uint32_t)) != 4) {
			res = TEE_ERROR_BAD_PARAMETERS;
			goto err;
		}

		gr->gpio_voltage_table[0] = (uint16_t)(fdt32_to_cpu(prop[0]) /
						       U(1000));
		gr->gpio_voltage_table[1] = (uint16_t)(fdt32_to_cpu(prop[2]) /
						       U(1000));

		/* Low voltage should be lower than high voltage.*/
		if (gr->gpio_voltage_table[0] >= gr->gpio_voltage_table[1]) {
			res = TEE_ERROR_BAD_PARAMETERS;
			goto err;
		}

		/* GPIO should have different states for both voltages */
		if (fdt32_to_cpu(prop[1]) == fdt32_to_cpu(prop[3])) {
			res = TEE_ERROR_BAD_PARAMETERS;
			goto err;
		}

		if ((uint32_t)fdt32_to_cpu(prop[1]) == 0U)
			gr->voltage_level_high = true;

		gr->ops.set_voltage = gpio_set_voltage;
		gr->ops.get_voltage = gpio_get_voltage;
		gr->ops.list_voltages = gpio_list_voltages;
	}

	cuint = fdt_getprop(fdt, node, "st,enable_pin_index", NULL);
	if (cuint) {
		size_t index = (size_t)fdt32_to_cpu(*cuint);
		struct stm32_pinctrl *pin = NULL;

		res = get_pinctrl_from_index(list, index, &pin);
		if (res)
			goto err;

		gr->enable_gpio = stm32_pinctrl_get_gpio_id(pin);
		if (fdt_getprop(fdt, node, "enable-active-high", NULL))
			gr->enable_active_high = true;

		gr->ops.set_state = gpio_set_state;
		gr->ops.get_state = gpio_get_state;
	}

	res = stm32_pinctrl_set_secure_cfg(list, true /*secure*/);
	if (res)
		goto err;

	res = regulator_register(&gr->desc, node);
	if (res) {
		EMSG("regulator_register(%s) failed with %#"PRIx32,
		     reg_name, res);
		panic();
	}

err:
	if (res)
		free(gr);
	return res;
}

static const struct dt_device_match gpio_regulator_match_table[] = {
	{ .compatible = "st,stm32-regulator-gpio" },
	{ }
};

DEFINE_DT_DRIVER(stm32_gpio_regulator_dt_driver) = {
	.name = "stm32-gpio-regulator",
	.match_table = gpio_regulator_match_table,
	.probe = gpio_regulator_probe,
};
