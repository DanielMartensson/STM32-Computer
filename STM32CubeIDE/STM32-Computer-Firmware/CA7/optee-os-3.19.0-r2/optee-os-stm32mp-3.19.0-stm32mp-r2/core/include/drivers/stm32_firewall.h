/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021, STMicroelectronics - All Rights Reserved
 */

#ifndef __DRIVERS_STM32_FIREWALL_H
#define __DRIVERS_STM32_FIREWALL_H

#include <kernel/dt.h>
#include <kernel/pm.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/queue.h>
#include <tee_api_types.h>
#include <types_ext.h>

/*
 * Firewall configuration
 *
 * 32b configuration ID contains, from LSbits to MSbits:
 * - 8 bit access permissions bit flags (rw, u/p, s/ns)
 * - 8 bit attribute flags (locking)
 * - 16 bit platform specific ID of the access master
 */

/* Master configuration bits */
#define FWLL_MASTER_SHIFT	16
#define FWLL_MASTER(x)		((x) << FWLL_MASTER_SHIFT)
#define FWLL_MASTER_MASK	GENMASK_32(31, 16)

/* Lock configuration */
#define FWLL_CONF_LOCK		BIT(15)

/* Access configuration */
#define FWLL_CONF_MASK		GENMASK_32(7, 0)
#define FWLL_NO_ACCESS		0
#define FWLL_SEC_READ		BIT(0)
#define FWLL_SEC_WRITE		BIT(1)
#define FWLL_SEC_PRIV		BIT(2)
#define FWLL_SEC_UNPRIV		BIT(3)
#define FWLL_NSEC_READ		BIT(4)
#define FWLL_NSEC_WRITE		BIT(5)
#define FWLL_NSEC_PRIV		BIT(6)
#define FWLL_NSEC_UNPRIV	BIT(7)

#define FWLL_SEC_PRIV_RW	(FWLL_SEC_READ | \
				 FWLL_SEC_WRITE | \
				 FWLL_SEC_PRIV)

#define FWLL_SEC_UNPRIV_RW	(FWLL_SEC_READ | \
				 FWLL_SEC_WRITE | \
				 FWLL_SEC_UNPRIV)

#define FWLL_SEC_RW		(FWLL_SEC_READ | \
				 FWLL_SEC_WRITE | \
				 FWLL_SEC_PRIV | \
				 FWLL_SEC_UNPRIV)

#define FWLL_NSEC_PRIV_RW	(FWLL_NSEC_READ | \
				 FWLL_NSEC_WRITE | \
				 FWLL_NSEC_PRIV)

#define FWLL_NSEC_UNPRIV_RW	(FWLL_NSEC_READ | \
				 FWLL_NSEC_WRITE | \
				 FWLL_NSEC_UNPRIV)

#define FWLL_NSEC_RW		(FWLL_NSEC_READ | \
				 FWLL_NSEC_WRITE | \
				 FWLL_NSEC_PRIV | \
				 FWLL_NSEC_UNPRIV)

#define FWLL_FULL_ACCESS	(FWLL_SEC_RW | \
				 FWLL_NSEC_RW)

struct stm32_firewall_device;

/**
 * @brief struct stm32_firewall_reg - define the firewall device management
 *
 * @addr : controlled physical address.
 * @size : length of the controlled area.
 */
struct stm32_firewall_reg {
	paddr_t addr;
	size_t size;
};

/**
 * @brief struct stm32_firewall_compat - Device tree compatibility table
 *
 * @reg: stm32_firewall_reg structure.
 * @compat_size: Array size of the stm32_firewall_reg structure.
 */
struct stm32_firewall_compat {
	struct stm32_firewall_reg *reg;
	size_t compat_size;
};

/**
 * @brief struct stm32_firewall_cfg - Firewall configuration structure
 *
 * @access: Bit flags for permission, FWLL_[N]SEC_*
 */
struct stm32_firewall_cfg {
	uint32_t access;
};

/**
 * @brief struct stm32_firewall_ops
 *
 * @has_access: Check the access regarding the requested configuration.
 * @configure_access: Configure access based on requested configuration.
 * @request_access: Lock access for the requester.
 * @release_access: Unlock access of the requester.
 */
struct stm32_firewall_ops {
	TEE_Result (*has_access)(struct stm32_firewall_device *fdev,
				 unsigned int id,
				 paddr_t addr, size_t sz,
				 const struct stm32_firewall_cfg *cfg);
	TEE_Result (*configure_access)(struct stm32_firewall_device *fdev,
				       unsigned int id,
				       paddr_t addr, size_t sz,
				       const struct stm32_firewall_cfg *cfg);
	TEE_Result (*request_access)(struct stm32_firewall_device *fdev,
				     unsigned int id,
				     paddr_t addr, size_t sz,
				     const struct stm32_firewall_cfg *cfg);
	TEE_Result (*release_access)(struct stm32_firewall_device *fdev,
				     unsigned int id,
				     paddr_t addr, size_t sz,
				     const struct stm32_firewall_cfg *cfg);
	TEE_Result (*get_data_config)(const void *fdt, int node,
				      int *ndata, void **data_cfg);
	TEE_Result (*set_data_config)(int ndata, void *data_cfg);
	TEE_Result (*put_data_config)(int ndata, void *data_cfg);
};

/**
 * @brief struct stm32_firewall_device
 *
 * @name: device name.
 * @ops: Operation table of the firewall.
 * @compat: List of the devices controlled by the firewall instance
 * @priv: Optional private data supplied by driver.
 * @dev_list: Firewall instance reference registered firewall list
 */
struct stm32_firewall_device {
	const char *name;
	const struct stm32_firewall_ops *ops;
	const struct stm32_firewall_compat *compat;
	void *priv;

	LIST_ENTRY(stm32_firewall_device) dev_list;
};

/* API for provider */

/**
 * @brief stm32_firewall_priv - Retrieve firewall private data
 *
 * @fdev: stm32_firewall_device struct to be freed.
 *
 * Return firewall private data
 */
static inline void *stm32_firewall_priv(struct stm32_firewall_device *fdev)
{
	return fdev->priv;
}

/**
 * @brief stm32_firewall_check_access - Check the configuration access
 * regarding a defined range.
 *
 * @base: Physical address
 * @size: Size to be checked
 * @cfg: Tested configuration
 *
 * Returns TEE_SUCCESS if access is permitted, or an error code otherwise
 */
TEE_Result stm32_firewall_check_access(paddr_t base, size_t size,
				       const struct stm32_firewall_cfg *cfg);

/**
 * @brief stm32_firewall_set_config - Define the configuration access
 * regarding a defined range.
 *
 * @base: Physical address
 * @size: Size to be checked
 * @cfg: Tested configuration
 *
 * Returns a TEE_Result compliant value.
 */
TEE_Result stm32_firewall_set_config(paddr_t base, size_t size,
				     const struct stm32_firewall_cfg *cfg);

/**
 * @brief stm32_firewall_get_data_config - Get the configuration access
 * from the device tree.
 *
 * @base: Physical address.
 * @size: Size to be checked.
 * @fdt: Device tree address.
 * @node: Bus node.
 * @ndata: Number of data configuration.
 * @data_cfg: Data configuration.
 *
 * Returns a TEE_Result compliant value.
 */
TEE_Result stm32_firewall_get_data_config(paddr_t base, size_t size,
					  const void *fdt, int node,
					  int *ndata, void **data_cfg);

/**
 * @brief stm32_firewall_set_data_config - Set the configuration access.
 *
 * @base: Physical address.
 * @size: Size to be checked.
 * @ndata: Number of data configuration.
 * @data_cfg: Data configuration.
 *
 * Returns a TEE_Result compliant value.
 */
TEE_Result stm32_firewall_set_data_config(paddr_t base, size_t size,
					  int ndata, void *data_cfg);

/**
 * @brief stm32_firewall_put_data_config - Put the configuration access.
 *
 * @base: Physical address.
 * @size: Size to be checked.
 * @ndata: Number of data configuration.
 * @data_cfg: Data configuration.
 *
 * Returns a TEE_Result compliant value.
 */
TEE_Result stm32_firewall_put_data_config(paddr_t base, size_t size,
					  int ndata, void *data_cfg);

/**
 * @brief stm32_firewall_dev_alloc - Allocate a new firewall device.
 *
 * Returns a stm32_firewall_device struct or NULL if allocation failed.
 */
struct stm32_firewall_device *stm32_firewall_dev_alloc(void);

/**
 * @brief stm32_firewall_dev_free - Free a firewall device.
 *
 * @fdev: stm32_firewall_device struct to be freed.
 */
void stm32_firewall_dev_free(struct stm32_firewall_device *fdev);

/**
 * @brief stm32_firewall_dev_register - Register a firewall device
 * list.
 *
 * @fdev: stm32_firewall_device struct to be added.
 *
 * Returns a TEE_Result compliant value.
 */
TEE_Result stm32_firewall_dev_register(struct stm32_firewall_device *fdev);

/**
 * @brief stm32_firewall_bus_probe - Probe a firewall bus driver child.
 *
 * @fdev: firewall_device struct to be probed.
 * @fdt: device tree address.
 * @node: firewall bus node.
 *
 * Return a TEE_Result compliant value.
 */
TEE_Result stm32_firewall_bus_probe(struct stm32_firewall_device *fdev,
				    const void *fdt, int node);
#endif /* __DRIVERS_STM32_FIREWALL_H */
