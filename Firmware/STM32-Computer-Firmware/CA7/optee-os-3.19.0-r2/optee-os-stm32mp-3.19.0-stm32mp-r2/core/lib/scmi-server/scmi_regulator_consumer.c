// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020-2023, STMicroelectronics
 */

#include <assert.h>
#include <config.h>
#include <drivers/regulator.h>
#include <initcall.h>
#include <kernel/boot.h>
#include <kernel/dt.h>
#include <kernel/panic.h>
#include <libfdt.h>
#include <malloc.h>
#include <mod_stm32_regu_consumer.h>
#include <scmi_agent_configuration.h>
#include <scmi_regulator_consumer.h>
#include <trace.h>

static TEE_Result init_channel(void *fdt, int node)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct scmi_server_regu_channel voltd_channel = { };
	size_t voltd_domain_count = 0;
	const fdt32_t *cuint = NULL;
	int subnode = 0;
	size_t n = 0;

	cuint = fdt_getprop(fdt, node, "scmi-channel-id", NULL);
	if (!cuint)
		panic("Can't read property scmi-channel-id");

	voltd_channel.channel_id = fdt32_to_cpu(*cuint);

	/* Compute the number of domains to allocate */
	fdt_for_each_subnode(subnode, fdt, node) {
		paddr_t reg = _fdt_reg_base_address(fdt, subnode);

		assert(reg != DT_INFO_INVALID_REG);
		if ((size_t)reg > voltd_domain_count)
			voltd_domain_count = (uint32_t)reg;
	}

	voltd_channel.regu_count = voltd_domain_count + 1;

	voltd_channel.regu = calloc(voltd_channel.regu_count,
				    sizeof(*voltd_channel.regu));
	if (!voltd_channel.regu)
		return TEE_ERROR_OUT_OF_MEMORY;

	fdt_for_each_subnode(subnode, fdt, node) {
		struct scmi_server_regu *regu = NULL;
		struct rdev *rdev = NULL;
		uint32_t domain_id = 0;

		rdev = regulator_get_by_supply_name(fdt, subnode, "voltd");
		if (!rdev) {
			DMSG("Regulator not found for voltd %s, skipped",
			     fdt_get_name(fdt, subnode, NULL));
			continue;
		}

		domain_id = (uint32_t)_fdt_reg_base_address(fdt, subnode);
		regu = voltd_channel.regu + domain_id;
		regu->domain_id = domain_id;

		/* Check that the domain_id is not already used */
		if (regu->rdev) {
			EMSG("Domain ID %"PRIu32" already used", domain_id);
			panic();
		}
		regu->rdev = rdev;

		/*
		 * Synchronize SCMI regulator current configuration
		 * Boot-on can be disabled by non secure
		 * Always-on can not be updated but status will be synchronized
		 * in non secure.
		 */
		if (rdev->flags & REGUL_BOOT_ON ||
		    rdev->flags & REGUL_ALWAYS_ON) {
			if (regulator_enable(rdev))
				IMSG("Failed to enable SCMI regul");
			else
				regu->enabled = true;
		}

		DMSG("scmi voltd shares %s (node %s) on domain ID %"PRIu32,
		     rdev->reg_name, rdev->desc->node_name, domain_id);
	}

	/*
	 * Assign domain IDs to un-exposed regulator as SCMI specification
	 * require the resource is defined even if not accessible.
	 */
	for (n = 0; n < voltd_channel.regu_count; n++) {
		if (voltd_channel.regu[n].rdev)
			assert(voltd_channel.regu[n].domain_id == n);
		else
			voltd_channel.regu[n].domain_id = n;
	}

	res = scmi_scpfw_cfg_add_regu(0/*agent*/, voltd_channel.channel_id,
				      voltd_channel.regu,
				      voltd_channel.regu_count);
	if (res)
		panic();

	/* We can free voltd_channel resources since SCMI server handles them */
	free(voltd_channel.regu);

	return TEE_SUCCESS;
}

TEE_Result scmi_regulator_consumer_init(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	void *fdt = NULL;
	int node = -1;

	if (!IS_ENABLED(CFG_SCMI_SERVER_REGULATOR_CONSUMER))
		return TEE_SUCCESS;

	fdt = get_embedded_dt();
	if (!fdt)
		return TEE_SUCCESS;

	node = fdt_node_offset_by_compatible(fdt, node,
					     "st,scmi-regulator-consumer");
	if (node == -FDT_ERR_NOTFOUND)
		return TEE_SUCCESS;
	if (node < 0)
		return TEE_ERROR_GENERIC;

	res = init_channel(fdt, node);
	if (res)
		return res;

	node = fdt_node_offset_by_compatible(fdt, node,
					     "st,scmi-regulator-consumer");
	if (node != -FDT_ERR_NOTFOUND)
		panic("Too many st,scmi-regulator-consumer compatible nodes");

	return TEE_SUCCESS;
}

