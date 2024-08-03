// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2021, STMicroelectronics - All Rights Reserved
 *
 * Driver for Microchip MCP23017 16-Bit I/O Expander with I2C interface
 */

#define LOG_CATEGORY UCLASS_PINCTRL

#include <common.h>
#include <dm.h>
#include <log.h>
#include <i2c.h>
#include <reset.h>
#include <asm/gpio.h>
#include <dm/device.h>
#include <dm/device-internal.h>
#include <dm/device_compat.h>
#include <dm/lists.h>
#include <dm/pinctrl.h>
#include <linux/bitfield.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <power/regulator.h>

/* register offset for IOCON.BANK = 0, the normal mode at reset */
#define MCP_REG_IODIR	0x00		/* init/reset:  all ones */
#define MCP_REG_IOCON	0x0A
#define IOCON_SEQOP	BIT(5)
#define MCP_REG_GPPU	0x0C
#define MCP_REG_GPIO	0x12

#define MCP_REG_SIZE	8
#define MCP_MAX_GPIO	16

static int mcp23017_read(struct udevice *dev, uint offset)
{
	return  dm_i2c_reg_read(dev_get_parent(dev), offset);
}

static int mcp23017_write(struct udevice *dev, uint reg, unsigned int val)
{
	dev_dbg(dev, "%s reg = 0x%x val = 0x%x\n", __func__, reg, val);

	return dm_i2c_reg_write(dev_get_parent(dev), reg, val);
}

static int mcp23017_read_reg(struct udevice *dev, u8 reg, uint offset)
{
	u8 mask = BIT(offset);
	int ret;

	ret = mcp23017_read(dev, reg);

	dev_dbg(dev, "%s reg = 0x%x offset = %d ret = 0x%x mask = 0x%x\n",
		__func__, reg, offset, ret, mask);

	return ret < 0 ? ret : !!(ret & mask);
}

static int mcp23017_write_reg(struct udevice *dev, u8 reg, uint offset,
			      uint val)
{
	u8 mask = BIT(offset);
	int ret;

	ret = mcp23017_read(dev, reg);
	if (ret < 0)
		return ret;
	ret = (ret & ~mask) | (val ? mask : 0);

	return mcp23017_write(dev, reg, ret);
}

static int mcp23017_conf_set_gppu(struct udevice *dev, unsigned int offset,
				  uint pupd)
{
	int reg = MCP_REG_GPPU + offset / MCP_REG_SIZE;
	int bit = offset % MCP_REG_SIZE;

	return mcp23017_write_reg(dev, reg, bit, pupd);
}

static int mcp23017_conf_get_gppu(struct udevice *dev, unsigned int offset)
{
	int reg = MCP_REG_GPPU + offset / MCP_REG_SIZE;
	int bit = offset % MCP_REG_SIZE;
	int ret = mcp23017_read_reg(dev, reg, bit);

	return ret;
}

static int mcp23017_gpio_get(struct udevice *dev, unsigned int offset)
{
	int reg = MCP_REG_GPIO + offset / MCP_REG_SIZE;
	int bit = offset % MCP_REG_SIZE;
	int ret = mcp23017_read_reg(dev, reg, bit);

	return ret;
}

static int mcp23017_gpio_set(struct udevice *dev, unsigned int offset, int value)
{
	int reg = MCP_REG_GPIO + offset / MCP_REG_SIZE;
	int bit = offset % MCP_REG_SIZE;

	return mcp23017_write_reg(dev, reg, bit, value);
}

static int mcp23017_gpio_get_function(struct udevice *dev, unsigned int offset)
{
	int ret;
	int reg = MCP_REG_IODIR + offset / MCP_REG_SIZE;
	int bit = offset % MCP_REG_SIZE;

	ret = mcp23017_read_reg(dev, reg, bit);

	if (ret < 0)
		return ret;
	/* On mcp23017, gpio pins direction is (0)output, (1)input. */
	return ret ? GPIOF_INPUT : GPIOF_OUTPUT;
}

static int mcp23017_gpio_direction_input(struct udevice *dev, unsigned int offset)
{
	int reg = MCP_REG_IODIR + offset / MCP_REG_SIZE;
	int bit = offset % MCP_REG_SIZE;

	return mcp23017_write_reg(dev, reg, bit, 1);
}

static int mcp23017_gpio_direction_output(struct udevice *dev,
					  unsigned int offset, int value)
{
	int reg = MCP_REG_IODIR + offset / MCP_REG_SIZE;
	int bit = offset % MCP_REG_SIZE;
	int ret = mcp23017_gpio_set(dev, offset, value);

	if (ret < 0)
		return ret;

	return mcp23017_write_reg(dev, reg, bit, 0);
}

static int mcp23017_gpio_set_flags(struct udevice *dev, unsigned int offset,
				   ulong flags)
{
	int ret = -ENOTSUPP;

	if (flags & GPIOD_IS_OUT) {
		bool value = flags & GPIOD_IS_OUT_ACTIVE;

		if (flags & GPIOD_OPEN_SOURCE)
			return -ENOTSUPP;
		if (flags & GPIOD_OPEN_DRAIN)
			return -ENOTSUPP;
		ret = mcp23017_gpio_direction_output(dev, offset, value);
	} else if (flags & GPIOD_IS_IN) {
		ret = mcp23017_gpio_direction_input(dev, offset);
		if (ret)
			return ret;
		if (flags & GPIOD_PULL_UP) {
			ret = mcp23017_conf_set_gppu(dev, offset, 1);
			if (ret)
				return ret;
		} else {
			ret = mcp23017_conf_set_gppu(dev, offset, 0);
			if (ret)
				return ret;
		}
	}

	return ret;
}

static int mcp23017_gpio_get_flags(struct udevice *dev, unsigned int offset,
				   ulong *flags)
{
	ulong dir_flags = 0;
	int ret;

	if (mcp23017_gpio_get_function(dev, offset) == GPIOF_OUTPUT) {
		dir_flags |= GPIOD_IS_OUT;

		ret = mcp23017_gpio_get(dev, offset);
		if (ret < 0)
			return ret;
		if (ret)
			dir_flags |= GPIOD_IS_OUT_ACTIVE;
	} else {
		dir_flags |= GPIOD_IS_IN;

		ret = mcp23017_conf_get_gppu(dev, offset);
		if (ret < 0)
			return ret;
		if (ret == 1)
			dir_flags |= GPIOD_PULL_UP;
	}
	*flags = dir_flags;

	return 0;
}

static int mcp23017_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);

	uc_priv->bank_name = "mcp_gpio";
	uc_priv->gpio_count = MCP_MAX_GPIO;

	return 0;
}

static const struct dm_gpio_ops mcp23017_gpio_ops = {
	.set_value = mcp23017_gpio_set,
	.get_value = mcp23017_gpio_get,
	.get_function = mcp23017_gpio_get_function,
	.direction_input = mcp23017_gpio_direction_input,
	.direction_output = mcp23017_gpio_direction_output,
	.set_flags = mcp23017_gpio_set_flags,
	.get_flags = mcp23017_gpio_get_flags,
};

U_BOOT_DRIVER(mcp23017_gpio) = {
	.name	= "mcp23017-gpio",
	.id	= UCLASS_GPIO,
	.probe	= mcp23017_gpio_probe,
	.ops	= &mcp23017_gpio_ops,
};

#if CONFIG_IS_ENABLED(PINCONF)
static const struct pinconf_param mcp23017_pinctrl_conf_params[] = {
	{ "bias-pull-up", PIN_CONFIG_BIAS_PULL_UP, 0 },
	{ "output-high", PIN_CONFIG_OUTPUT, 1 },
	{ "output-low", PIN_CONFIG_OUTPUT, 0 },
};

static int mcp23017_pinctrl_conf_set(struct udevice *dev, unsigned int pin,
				     unsigned int param, unsigned int arg)
{
	int ret, dir;

	/* directly call the generic gpio function, only based on i2c parent */
	dir = mcp23017_gpio_get_function(dev, pin);

	if (dir < 0)
		return dir;

	switch (param) {
	case PIN_CONFIG_BIAS_PULL_UP:
		ret = mcp23017_conf_set_gppu(dev, pin, 1);
		break;
	case PIN_CONFIG_OUTPUT:
		ret = mcp23017_gpio_direction_output(dev, pin, arg);
		break;
	default:
		return -ENOTSUPP;
	}

	return ret;
}
#endif

static int mcp23017_pinctrl_get_pins_count(struct udevice *dev)
{
	return MCP_MAX_GPIO;
}

static char pin_name[PINNAME_SIZE];
static const char *mcp23017_pinctrl_get_pin_name(struct udevice *dev,
						 unsigned int selector)
{
	snprintf(pin_name, PINNAME_SIZE, "mcp_gpio%u", selector);

	return pin_name;
}

static const char *mcp23017_pinctrl_get_pin_conf(struct udevice *dev,
						 unsigned int pin, int func)
{
	int pupd;

	pupd = mcp23017_conf_get_gppu(dev, pin);
	if (pupd < 0)
		return "";

	if (pupd)
		return "bias-pull-up";
	else
		return "";
}

static int mcp23017_pinctrl_get_pin_muxing(struct udevice *dev,
					   unsigned int selector,
					   char *buf, int size)
{
	int func;

	func = mcp23017_gpio_get_function(dev, selector);
	if (func < 0)
		return func;

	snprintf(buf, size, "%s ", func == GPIOF_INPUT ? "input" : "output");

	strlcat(buf, mcp23017_pinctrl_get_pin_conf(dev, selector, func), size);

	return 0;
}

const struct pinctrl_ops mcp23017_pinctrl_ops = {
	.get_pins_count = mcp23017_pinctrl_get_pins_count,
	.get_pin_name = mcp23017_pinctrl_get_pin_name,
	.set_state = pinctrl_generic_set_state,
	.get_pin_muxing	= mcp23017_pinctrl_get_pin_muxing,
#if CONFIG_IS_ENABLED(PINCONF)
	.pinconf_set = mcp23017_pinctrl_conf_set,
	.pinconf_num_params = ARRAY_SIZE(mcp23017_pinctrl_conf_params),
	.pinconf_params = mcp23017_pinctrl_conf_params,
#endif
};

U_BOOT_DRIVER(mcp23017_pinctrl) = {
	.name = "mcp23017-pinctrl",
	.id = UCLASS_PINCTRL,
	.ops = &mcp23017_pinctrl_ops,
};

static int mcp23017_bind(struct udevice *dev)
{
	int ret;

	ret = device_bind_driver_to_node(dev, "mcp23017-pinctrl", "mcp23017-pinctrl",
					 dev_ofnode(dev), NULL);
	if (ret)
		return ret;

	return device_bind_driver_to_node(dev, "mcp23017-gpio", "mcp23017-gpio",
					  dev_ofnode(dev), NULL);
}

static int mcp23017_chip_init(struct udevice *dev)
{
	int ret, iocon;

	ret = dm_i2c_reg_read(dev, MCP_REG_IOCON);
	dev_dbg(dev, "reg = 0x%x val = 0x%x\n", MCP_REG_IOCON, ret);
	if (ret < 0) {
		dev_err(dev, "Can't read MCP23017 IOCON register (%d)\n", ret);
		return ret;
	}

	/* deactivate Sequential mode if activated */
	if (ret & IOCON_SEQOP) {
		iocon = ret & ~IOCON_SEQOP;

		ret = dm_i2c_reg_write(dev, MCP_REG_IOCON, iocon);
		if (ret < 0) {
			dev_err(dev, "can't write IOCON register (%d)\n", ret);
		} else {
			/* mcp23017 has IOCON twice, make sure they are in sync */
			ret = dm_i2c_reg_write(dev, MCP_REG_IOCON + 1, iocon);
			if (ret < 0)
				dev_err(dev, "can't write IOCON register (%d)\n", ret);
		}
	}

	return ret;
}

static int mcp23017_probe(struct udevice *dev)
{
	struct udevice *vdd;
	struct reset_ctl reset;
	int ret;

	if (CONFIG_IS_ENABLED(DM_REGULATOR)) {
		ret = device_get_supply_regulator(dev, "vdd-supply", &vdd);
		if (ret && ret != -ENOENT) {
			dev_err(dev, "vdd regulator error:%d\n", ret);
			return ret;
		}
		if (!ret) {
			ret = regulator_set_enable(vdd, true);
			if (ret) {
				dev_err(dev, "vdd enable failed: %d\n", ret);
				return ret;
			}
		}
	}

	ret = reset_get_by_index(dev, 0, &reset);
	if (!ret) {
		reset_assert(&reset);
		udelay(2);
		reset_deassert(&reset);
	}

	return mcp23017_chip_init(dev);
}

static const struct udevice_id mcp23017_match[] = {
	{ .compatible = "microchip,mcp23017", },
};

U_BOOT_DRIVER(mcp23017) = {
	.name = "mcp23017",
	.id = UCLASS_I2C_GENERIC,
	.of_match = of_match_ptr(mcp23017_match),
	.probe = mcp23017_probe,
	.bind = mcp23017_bind,
};
