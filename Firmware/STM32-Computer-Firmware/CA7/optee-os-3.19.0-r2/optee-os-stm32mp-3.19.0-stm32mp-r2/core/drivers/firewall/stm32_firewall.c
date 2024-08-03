// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (C) 2021, STMicroelectronics - All Rights Reserved
 */

#include <assert.h>
#include <drivers/stm32_firewall.h>
#include <kernel/boot.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <util.h>

#define MAX_COMPAT_LENGTH	30

static LIST_HEAD(fw_dev_list_head, stm32_firewall_device) fw_dev_list =
	LIST_HEAD_INITIALIZER(fw_dev_list_head);

static unsigned int list_lock = SPINLOCK_UNLOCK;

static const char firewall_compatible_exceptions[][MAX_COMPAT_LENGTH] = {
	"st,stm32mp15-i2c-non-secure",
	"st,stm32-ltdc"
};

static struct stm32_firewall_device *find_device(paddr_t base, size_t size,
						 unsigned int *idx)
{
	struct stm32_firewall_device *fdev = NULL;
	unsigned int i = 0;

	assert(idx);

	LIST_FOREACH(fdev, &fw_dev_list, dev_list) {
		for (i = 0; i < fdev->compat->compat_size; i++) {
			struct stm32_firewall_reg *reg = fdev->compat->reg + i;

			if (!reg->addr && !reg->size)
				continue;

			if (!reg->size) {
				if (base != reg->addr)
					continue;

				if (size)
					IMSG("IOMEM %#"PRIxPA": non-null size",
					     base);

				*idx = i;
				return fdev;
			}

			if (reg->size && base >= reg->addr &&
			    base + size - 1 <= reg->addr + reg->size - 1) {
				*idx = i;
				return fdev;
			}
		}
	}

	return NULL;
}

TEE_Result stm32_firewall_check_access(paddr_t base, size_t size,
				       const struct stm32_firewall_cfg *cfg)
{
	TEE_Result res = TEE_ERROR_ITEM_NOT_FOUND;
	struct stm32_firewall_device *fdev = NULL;
	uint32_t exceptions = 0;
	unsigned int index = 0;

	exceptions = cpu_spin_lock_xsave(&list_lock);
	fdev = find_device(base, size, &index);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	if (fdev && fdev->ops->has_access)
		res = fdev->ops->has_access(fdev, index, base, size, cfg);

	return res;
}

TEE_Result stm32_firewall_set_config(paddr_t base, size_t size,
				     const struct stm32_firewall_cfg *cfg)
{
	TEE_Result res = TEE_ERROR_ITEM_NOT_FOUND;
	struct stm32_firewall_device *fdev = NULL;
	uint32_t exceptions = 0;
	unsigned int index = 0;

	exceptions = cpu_spin_lock_xsave(&list_lock);
	fdev = find_device(base, size, &index);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	if (fdev && fdev->ops->configure_access)
		res = fdev->ops->configure_access(fdev, index, base, size, cfg);

	return res;
}

TEE_Result stm32_firewall_get_data_config(paddr_t base, size_t size,
					  const void *fdt, int node,
					  int *ndata, void **data_cfg)
{
	TEE_Result res = TEE_ERROR_ITEM_NOT_FOUND;
	struct stm32_firewall_device *fdev = NULL;
	uint32_t exceptions = 0;
	unsigned int index = 0;

	exceptions = cpu_spin_lock_xsave(&list_lock);
	fdev = find_device(base, size, &index);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	if (fdev && fdev->ops->get_data_config)
		res = fdev->ops->get_data_config(fdt, node, ndata, data_cfg);

	return res;
}

TEE_Result stm32_firewall_set_data_config(paddr_t base, size_t size,
					  int ndata, void *data_cfg)
{
	TEE_Result res = TEE_ERROR_ITEM_NOT_FOUND;
	struct stm32_firewall_device *fdev = NULL;
	uint32_t exceptions = 0;
	unsigned int index = 0;

	exceptions = cpu_spin_lock_xsave(&list_lock);
	fdev = find_device(base, size, &index);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	if (fdev && fdev->ops->set_data_config)
		res = fdev->ops->set_data_config(ndata, data_cfg);

	return res;
}

TEE_Result stm32_firewall_put_data_config(paddr_t base, size_t size,
					  int ndata, void *data_cfg)
{
	TEE_Result res = TEE_ERROR_ITEM_NOT_FOUND;
	struct stm32_firewall_device *fdev = NULL;
	uint32_t exceptions = 0;
	unsigned int index = 0;

	exceptions = cpu_spin_lock_xsave(&list_lock);
	fdev = find_device(base, size, &index);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	if (fdev && fdev->ops->put_data_config)
		res = fdev->ops->put_data_config(ndata, data_cfg);

	return res;
}

struct stm32_firewall_device *stm32_firewall_dev_alloc(void)
{
	return calloc(1, sizeof(struct stm32_firewall_device));
}

void stm32_firewall_dev_free(struct stm32_firewall_device *fdev)
{
	uint32_t exceptions = 0;

	assert(fdev);

	exceptions = cpu_spin_lock_xsave(&list_lock);
	LIST_REMOVE(fdev, dev_list);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	free(fdev);
}

TEE_Result stm32_firewall_dev_register(struct stm32_firewall_device *fdev)
{
	uint32_t exceptions = 0;

	assert(fdev && fdev->name && fdev->ops);

	exceptions = cpu_spin_lock_xsave(&list_lock);
	LIST_INSERT_HEAD(&fw_dev_list, fdev, dev_list);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	return TEE_SUCCESS;
}

static TEE_Result
stm32_firewall_bus_dt_register(struct stm32_firewall_device *fdev,
			       const void *fdt, int node)
{
	const char __maybe_unused *name = NULL;
	int idx = 0;
	int len = 0;
	int count = 0;
	unsigned int id = 0;
	const char *compat = NULL;
	paddr_t pbase = 0;

	/* Check accesses */
	const struct stm32_firewall_cfg sec_cfg[] = {
		{ FWLL_SEC_RW | FWLL_MASTER(0) },
		{ }, /* Null terminated */
	};

	if (_fdt_get_status(fdt, node) == DT_STATUS_DISABLED)
		return TEE_SUCCESS;

	count = fdt_stringlist_count(fdt, node, "compatible");
	if (count < 0)
		return TEE_SUCCESS;

	pbase = _fdt_reg_base_address(fdt, node);
	if (pbase == DT_INFO_INVALID_REG)
		return TEE_ERROR_GENERIC;

	for (id = 0; id < fdev->compat->compat_size; id++)
		if (fdev->compat->reg[id].addr == pbase)
			break;

	if (id == fdev->compat->compat_size)
		return TEE_ERROR_ITEM_NOT_FOUND;

	name = fdt_get_name(fdt, node, NULL);

	compat = fdt_getprop(fdt, node, "compatible", &len);
	assert(compat);

	if (fdev->ops->has_access(fdev, id, pbase, 0, sec_cfg)) {
		const struct stm32_firewall_cfg non_sec_cfg[] = {
			{ FWLL_NSEC_PRIV_RW | FWLL_MASTER(0) },
			{ }, /* Null terminated */
		};
		int len_array = ARRAY_SIZE(firewall_compatible_exceptions);
		bool exception_found = false;

		if (fdev->ops->has_access(fdev, id, pbase, 0, non_sec_cfg))
			return TEE_SUCCESS;

		for (int i = 0; i < len_array; i++) {
			if (!strcmp(firewall_compatible_exceptions[i],
				    compat))
				exception_found = true;
		}

		if (!exception_found)
			return TEE_SUCCESS;
	}

	for (idx = 0; idx < count; idx++) {
		compat = fdt_stringlist_get(fdt, node, "compatible", idx, &len);
		assert(compat && len > 0);

		add_probe_node_by_compat(fdt, node, compat);
	}

	return TEE_SUCCESS;
}

TEE_Result stm32_firewall_bus_probe(struct stm32_firewall_device *fdev,
				    const void *fdt, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int subnode = 0;

	fdt_for_each_subnode(subnode, fdt, node) {
		res = stm32_firewall_bus_dt_register(fdev, fdt, subnode);
		if (res) {
			DMSG("Failed on node %s with %#"PRIx32,
			     fdt_get_name(fdt, subnode, NULL), res);
			panic();
		}
	}

	return TEE_SUCCESS;
}
