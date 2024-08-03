// SPDX-License-Identifier: GPL-2.0-or-later OR BSD-3-Clause
/*
 * Copyright (C) 2023, STMicroelectronics - All Rights Reserved
 */

#define LOG_CATEGORY UCLASS_SIMPLE_BUS

#include <common.h>
#include <dm.h>
#include <asm/io.h>
#include <dm/device.h>
#include <dm/device_compat.h>
#include <dm/lists.h>
#include <linux/bitfield.h>
#include <mach/etzpc.h>

/* ETZPC peripheral as firewall bus */
/* ETZPC registers */
#define ETZPC_DECPROT			0x10

/* ETZPC miscellaneous */
#define ETZPC_PROT_MASK			GENMASK(1, 0)
#define ETZPC_PROT_A7NS			0x3
#define ETZPC_DECPROT_SHIFT		1

#define IDS_PER_DECPROT_REGS		16
#define STM32MP15_ETZPC_ENTRIES		96
#define STM32MP13_ETZPC_ENTRIES		64

/*
 * struct stm32_sys_bus_match_data: Match data for ETZPC device
 *
 * @max_entries: Number of securable peripherals in ETZPC
 */
struct stm32_sys_bus_match_data {
	unsigned int max_entries;
};

/*
 * struct stm32_etzpc_plat: Information about ETZPC device
 *
 * @base: Base address of ETZPC
 */
struct stm32_etzpc_plat {
	void *base;
};

static int etzpc_parse_feature_domain(ofnode node, struct ofnode_phandle_args *args)
{
	int ret;

	ret = ofnode_parse_phandle_with_args(node, "feature-domains",
					     "#feature-domain-cells", 0,
					     0, args);
	if (ret) {
		log_debug("failed to parse feature-domains (%d)\n", ret);
		return ret;
	}

	if (args->args_count != 1) {
		log_debug("invalid domain args_count: %d\n", args->args_count);
		return -EINVAL;
	}

	return 0;
}

static int etzpc_check_access(void *base, u32 id)
{
	u32 reg_offset, offset, sec_val;

	/* Check access configuration, 16 peripherals per register */
	reg_offset = ETZPC_DECPROT + 0x4 * (id / IDS_PER_DECPROT_REGS);
	offset = (id % IDS_PER_DECPROT_REGS) << ETZPC_DECPROT_SHIFT;

	/* Verify peripheral is non-secure and attributed to cortex A7 */
	sec_val = (readl(base + reg_offset) >> offset) & ETZPC_PROT_MASK;
	if (sec_val != ETZPC_PROT_A7NS) {
		log_debug("Invalid bus configuration: reg_offset %#x, value %d\n",
			  reg_offset, sec_val);
		return -EACCES;
	}

	return 0;
}

int stm32_etzpc_check_access_by_id(ofnode device_node, u32 id)
{
	struct stm32_sys_bus_match_data *pconf;
	struct stm32_etzpc_plat *plat;
	struct ofnode_phandle_args args;
	struct udevice *dev;
	int err;

	err = etzpc_parse_feature_domain(device_node, &args);
	if (err)
		return err;

	if (id == -1U)
		id = args.args[0];

	err = uclass_get_device_by_ofnode(UCLASS_NOP, args.node, &dev);
	if (err || dev->driver != DM_DRIVER_GET(stm32_etzpc)) {
		log_err("No device found\n");
		return -EINVAL;
	}

	plat = dev_get_plat(dev);
	pconf = (struct stm32_sys_bus_match_data *)dev_get_driver_data(dev);

	if (id >= pconf->max_entries) {
		dev_err(dev, "Invalid sys bus ID for %s\n", ofnode_get_name(device_node));
		return -EINVAL;
	}

	return etzpc_check_access(plat->base, id);
}

int stm32_etzpc_check_access(ofnode device_node)
{
	return stm32_etzpc_check_access_by_id(device_node, -1U);
}

static int stm32_etzpc_bind(struct udevice *dev)
{
	struct stm32_etzpc_plat *plat = dev_get_plat(dev);
	struct stm32_sys_bus_match_data *pconf;
	struct ofnode_phandle_args args;
	int ret = 0, err = 0;
	ofnode node, parent;

	plat->base = dev_read_addr_ptr(dev);
	if (!plat->base) {
		dev_err(dev, "can't get registers base address\n");
		return -ENOENT;
	}

	pconf = (struct stm32_sys_bus_match_data *)dev_get_driver_data(dev);
	parent = dev_ofnode(dev);
	for (node = ofnode_first_subnode(parent);
	     ofnode_valid(node);
	     node = ofnode_next_subnode(node)) {
		const char *node_name = ofnode_get_name(node);

		if (!ofnode_is_enabled(node))
			continue;

		err = etzpc_parse_feature_domain(node, &args);
		if (err) {
			dev_err(dev, "%s failed to parse child on bus (%d)\n", node_name, err);
			continue;
		}

		if (!ofnode_equal(args.node, parent)) {
			dev_err(dev, "%s phandle to %s\n",
				node_name, ofnode_get_name(args.node));
			continue;
		}

		if (args.args[0] >= pconf->max_entries) {
			dev_err(dev, "Invalid sys bus ID for %s\n", node_name);
			return -EINVAL;
		}

		err = etzpc_check_access(plat->base, args.args[0]);
		if (err) {
			dev_info(dev, "%s not allowed on bus (%d)\n", node_name, err);
			continue;
		}

		err = lists_bind_fdt(dev, node, NULL, NULL,
				     gd->flags & GD_FLG_RELOC ? false : true);
		if (err) {
			ret = err;
			dev_err(dev, "%s failed to bind on bus (%d)\n", node_name, ret);
		}
	}

	if (ret)
		dev_err(dev, "Some child failed to bind (%d)\n", ret);

	return ret;
}

static const struct stm32_sys_bus_match_data stm32mp15_sys_bus_data = {
	.max_entries = STM32MP15_ETZPC_ENTRIES,
};

static const struct stm32_sys_bus_match_data stm32mp13_sys_bus_data = {
	.max_entries = STM32MP13_ETZPC_ENTRIES,
};

static const struct udevice_id stm32_etzpc_ids[] = {
	{ .compatible = "st,stm32mp13-sys-bus", .data = (ulong)&stm32mp13_sys_bus_data },
	{ .compatible = "st,stm32mp15-sys-bus", .data = (ulong)&stm32mp15_sys_bus_data },
	{},
};

U_BOOT_DRIVER(stm32_etzpc) = {
	.name = "stm32_etzpc",
	.id = UCLASS_NOP,
	.of_match = stm32_etzpc_ids,
	.bind = stm32_etzpc_bind,
	.plat_auto = sizeof(struct stm32_etzpc_plat),
};
