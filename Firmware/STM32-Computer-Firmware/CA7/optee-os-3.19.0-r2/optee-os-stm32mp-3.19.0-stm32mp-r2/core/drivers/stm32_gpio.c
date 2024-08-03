// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2017-2022, STMicroelectronics
 *
 * STM32 GPIO driver is used as pin controller for stm32mp SoCs.
 * The driver API is defined in header file stm32_gpio.h.
 */

#include <assert.h>
#include <config.h>
#include <drivers/clk.h>
#include <drivers/clk_dt.h>
#include <drivers/stm32_gpio.h>
#include <drivers/stm32_rif.h>
#include <gpio.h>
#include <io.h>
#include <kernel/dt.h>
#include <kernel/boot.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <mm/core_memprot.h>
#include <stdbool.h>
#include <stdio.h>
#include <stm32_util.h>
#include <trace.h>
#include <util.h>

#define GPIO_MODER_OFFSET	U(0x00)
#define GPIO_OTYPER_OFFSET	U(0x04)
#define GPIO_OSPEEDR_OFFSET	U(0x08)
#define GPIO_PUPDR_OFFSET	U(0x0c)
#define GPIO_IDR_OFFSET		U(0x10)
#define GPIO_ODR_OFFSET		U(0x14)
#define GPIO_BSRR_OFFSET	U(0x18)
#define GPIO_AFRL_OFFSET	U(0x20)
#define GPIO_AFRH_OFFSET	U(0x24)
#define GPIO_SECR_OFFSET	U(0x30)
#define GPIO_PRIVCFGR_OFFSET	U(0x34)
#define GPIO_RCFGLOCKR_OFFSET	U(0x38)
#define GPIO_CIDCFGR(X)		(U(0x50) + U(0x8) * (X))
#define GPIO_SEMCR(X)		(U(0x54) + U(0x8) * (X))

#define GPIO_ALT_LOWER_LIMIT	U(0x8)

/*
 * CIDCFGR register bitfields
 */
#define GPIO_CIDCFGR_SEMWL_MASK	GENMASK_32(23, 16)
#define GPIO_CIDCFGR_SCID_MASK	GENMASK_32(6, 4)
#define GPIO_CIDCFGR_CONF_MASK	(_CIDCFGR_CFEN | _CIDCFGR_SEMEN |	\
				 GPIO_CIDCFGR_SCID_MASK |		\
				 GPIO_CIDCFGR_SEMWL_MASK)

/*
 * PRIVCFGR register bitfields
 */
#define GPIO_PRIVCFGR_MASK	GENMASK_32(15, 0)

/*
 * SECCFGR register bitfields
 */
#define GPIO_SECCFGR_MASK	GENMASK_32(15, 0)

/*
 * RCFGLOCKR register bitfields
 */
#define GPIO_RCFGLOCKR_MASK	GENMASK_32(15, 0)

/*
 * SEMCR register bitfields
 */
#define GPIO_SEMCR_SCID_M	GENMASK_32(6, 4)

#define GPIO_MODE_MASK		GENMASK_32(1, 0)
#define GPIO_OSPEED_MASK	GENMASK_32(1, 0)
#define GPIO_PUPD_PULL_MASK	GENMASK_32(1, 0)
#define GPIO_ALTERNATE_MASK	GENMASK_32(3, 0)

#define DT_GPIO_BANK_SHIFT	U(12)
#define DT_GPIO_BANK_MASK	GENMASK_32(16, 12)
#define DT_GPIO_PIN_SHIFT	U(8)
#define DT_GPIO_PIN_MASK	GENMASK_32(11, 8)
#define DT_GPIO_MODE_MASK	GENMASK_32(7, 0)

/* Banks are named "GPIOX" with X upper case letter starting from 'A' */
#define DT_GPIO_BANK_NAME0	"GPIOA"

/*
 * Miscellaneous
 */
#define PROP_NAME_MAX		U(20)

#define GPIO_MAX_CID_SUPPORTED	U(3)

struct stm32_pinctrl_backup {
	struct stm32_pinctrl pinctrl;

	STAILQ_ENTRY(stm32_pinctrl_backup) link;
};

STAILQ_HEAD(stm32_pinctrl_backup_head, stm32_pinctrl_backup);

/**
 * struct stm32_gpio_bank describes a GPIO bank instance
 * @base: base address of the GPIO controller registers.
 * @clock: clock identifier.
 * @ngpios: number of GPIOs.
 * @bank_id: Id of the bank.
 * @lock: lock protecting the GPIO bank access.
 * @secure_support: True if bank supports pin security protection, else false
 * @secure_extended: True if driver supports RIF, else false.
 * @ready: True if configuration is applied, else false
 * @conf_data: RIF configuration data
 * @link: Link in bank list
 * @backups: Backup copy of applied pinctrl
 */
struct stm32_gpio_bank {
	vaddr_t base;
	struct clk *clock;
	unsigned int ngpios;
	unsigned int bank_id;
	unsigned int lock;
	bool secure_support;
	bool secure_extended;
	bool ready;
	struct rif_conf_data conf_data;
	struct stm32_pinctrl_backup_head backups;

	STAILQ_ENTRY(stm32_gpio_bank) link;
};

/**
 * struct stm32_data_pinctrl - Data used by DT_DRIVER provider interface
 * @fdt: FDT base address
 * @phandle: Pinctrl node phandle.
 */
struct stm32_data_pinctrl {
	const void *fdt;
	uint32_t phandle;
};

static STAILQ_HEAD(, stm32_gpio_bank) bank_list =
		STAILQ_HEAD_INITIALIZER(bank_list);

/**
 * struct stm32_gpio_compat_data describes GPIO associated data
 * for compatible list.
 *
 * @secure_control:	identify gpio security bank capability.
 */
struct stm32_gpio_compat_data {
	bool secure_control;
	bool secure_extended;
};

static const struct stm32_gpio_compat_data stm32_gpio_non_sec = {
	.secure_control = false,
	.secure_extended = false,
};

static const struct stm32_gpio_compat_data stm32_gpio_secure_capable = {
	.secure_control = true,
	.secure_extended = false,
};

static const struct stm32_gpio_compat_data stm32_gpio_with_sec_ext = {
	.secure_control = true,
	.secure_extended = true,
};

static struct stm32_gpio_bank *stm32_gpio_get_bank(unsigned int bank_id)
{
	struct stm32_gpio_bank *bank = NULL;

	STAILQ_FOREACH(bank, &bank_list, link)
		if (bank_id == bank->bank_id)
			break;

	return bank;
}

static int __maybe_unused stm32_gpio_bank_get_count(unsigned int bank_id)
{
	struct stm32_gpio_bank *bank = stm32_gpio_get_bank(bank_id);

	if (!bank) {
		EMSG("Error: can not find GPIO bank %c", bank_id + 'A');
		return -1;
	}

	return bank->ngpios;
}

static enum gpio_level
stm32_gpio_get_level(struct gpio_chip *chip __maybe_unused,
		     unsigned int gpio)
{
	uint32_t bank_id = (gpio & DT_GPIO_BANK_MASK) >> DT_GPIO_BANK_SHIFT;
	uint32_t pin = (gpio & DT_GPIO_PIN_MASK) >> DT_GPIO_PIN_SHIFT;
	struct stm32_gpio_bank *bank = stm32_gpio_get_bank(bank_id);
	enum gpio_level level = GPIO_LEVEL_LOW;

	if (!bank) {
		EMSG("Error: can not find GPIO bank %c", bank_id + 'A');
		return level;
	}

	if (io_read32(bank->base + GPIO_IDR_OFFSET) & BIT(pin))
		level = GPIO_LEVEL_HIGH;

	return level;
}

static void stm32_gpio_set_level(struct gpio_chip *chip __maybe_unused,
				 unsigned int gpio, enum gpio_level level)
{
	uint32_t bank_id = (gpio & DT_GPIO_BANK_MASK) >> DT_GPIO_BANK_SHIFT;
	uint32_t pin = (gpio & DT_GPIO_PIN_MASK) >> DT_GPIO_PIN_SHIFT;
	struct stm32_gpio_bank *bank = stm32_gpio_get_bank(bank_id);

	if (!bank) {
		EMSG("Error: can not find GPIO bank %c", bank_id + 'A');
		return;
	}

	if (level == GPIO_LEVEL_HIGH)
		io_write32(bank->base + GPIO_BSRR_OFFSET, BIT(pin));
	else
		io_write32(bank->base + GPIO_BSRR_OFFSET, BIT(pin + 16));
}

static const struct gpio_ops stm32_gpio_ops = {
	.get_value = stm32_gpio_get_level,
	.set_value = stm32_gpio_set_level,
};

const struct gpio_ops *stm32_gpio_get_ops(void)
{
	return &stm32_gpio_ops;
}

unsigned int stm32_pinctrl_get_gpio_id(struct stm32_pinctrl *pin)
{
	return ((unsigned int)pin->bank << DT_GPIO_BANK_SHIFT) |
	       ((unsigned int)pin->pin << DT_GPIO_PIN_SHIFT);
}

/* Apply GPIO (@bank/@pin) configuration described by @cfg */
static void set_gpio_cfg(uint32_t bank_id, uint32_t pin, struct gpio_cfg *cfg)
{
	struct stm32_gpio_bank *bank = stm32_gpio_get_bank(bank_id);
	uint32_t exceptions = 0;

	if (!bank) {
		EMSG("Error: can not find GPIO bank %c", bank_id + 'A');
		panic();
	}

	exceptions = cpu_spin_lock_xsave(&bank->lock);

	/* Load GPIO MODE value, 2bit value shifted by twice the pin number */
	io_clrsetbits32(bank->base + GPIO_MODER_OFFSET,
			GPIO_MODE_MASK << (pin << 1),
			cfg->mode << (pin << 1));

	/* Load GPIO Output TYPE value, 1bit shifted by pin number value */
	io_clrsetbits32(bank->base + GPIO_OTYPER_OFFSET, BIT(pin),
			cfg->otype << pin);

	/* Load GPIO Output Speed confguration, 2bit value */
	io_clrsetbits32(bank->base + GPIO_OSPEEDR_OFFSET,
			GPIO_OSPEED_MASK << (pin << 1),
			cfg->ospeed << (pin << 1));

	/* Load GPIO pull configuration, 2bit value */
	io_clrsetbits32(bank->base + GPIO_PUPDR_OFFSET,
			GPIO_PUPD_PULL_MASK << (pin << 1),
			cfg->pupd << (pin << 1));

	/* Load pin mux Alternate Function configuration, 4bit value */
	if (pin < GPIO_ALT_LOWER_LIMIT) {
		io_clrsetbits32(bank->base + GPIO_AFRL_OFFSET,
				GPIO_ALTERNATE_MASK << (pin << 2),
				cfg->af << (pin << 2));
	} else {
		size_t shift = (pin - GPIO_ALT_LOWER_LIMIT) << 2;

		io_clrsetbits32(bank->base + GPIO_AFRH_OFFSET,
				GPIO_ALTERNATE_MASK << shift,
				cfg->af << shift);
	}

	/* Load GPIO Output direction confuguration, 1bit */
	io_clrsetbits32(bank->base + GPIO_ODR_OFFSET, BIT(pin), cfg->od << pin);

	cpu_spin_unlock_xrestore(&bank->lock, exceptions);
}

/* Read current GPIO pin configuration and save it to 16bit formatted @cfg */
static void get_gpio_cfg(struct stm32_gpio_bank *bank, unsigned int pin,
			 struct gpio_cfg *cfg)
{
	cfg->mode = (io_read32(bank->base + GPIO_MODER_OFFSET) >> (pin << 1)) &
		    GPIO_MODE_MASK;

	cfg->otype = (io_read32(bank->base + GPIO_OTYPER_OFFSET) >> pin) & 1;

	cfg->ospeed = (io_read32(bank->base + GPIO_OSPEEDR_OFFSET) >>
		       (pin << 1)) & GPIO_OSPEED_MASK;

	cfg->pupd = (io_read32(bank->base + GPIO_PUPDR_OFFSET) >> (pin << 1)) &
		    GPIO_PUPD_PULL_MASK;

	if (pin < GPIO_ALT_LOWER_LIMIT) {
		size_t shift = pin << 2;

		cfg->af = (io_read32(bank->base + GPIO_AFRL_OFFSET) >> shift) &
			  GPIO_ALTERNATE_MASK;
	} else {
		size_t shift = (pin - GPIO_ALT_LOWER_LIMIT) << 2;

		cfg->af = (io_read32(bank->base + GPIO_AFRH_OFFSET) >> shift) &
			  GPIO_ALTERNATE_MASK;
	}

	cfg->od = (io_read32(bank->base + GPIO_ODR_OFFSET) >> pin) & 1;
}

static TEE_Result stm32_pinctrl_backup(struct stm32_pinctrl *pinctrl)
{
	struct stm32_pinctrl_backup *backup = NULL;
	struct stm32_gpio_bank *bank = stm32_gpio_get_bank(pinctrl->bank);

	if (!bank) {
		EMSG("Error: can not find GPIO bank %c", pinctrl->bank + 'A');
		return TEE_ERROR_GENERIC;
	}

	backup = calloc(1, sizeof(*backup));
	if (!backup) {
		EMSG("Error: can't backup pinctrl of %c%d", pinctrl->bank + 'A',
		     pinctrl->pin);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	backup->pinctrl.bank = pinctrl->bank;
	backup->pinctrl.pin = pinctrl->pin;
	backup->pinctrl.config = pinctrl->config;

	STAILQ_INSERT_TAIL(&bank->backups, backup, link);

	return TEE_SUCCESS;
}

void stm32_pinctrl_load_config(struct stm32_pinctrl_list *list)
{
	struct stm32_pinctrl *p = NULL;

	STAILQ_FOREACH(p, list, link)
		set_gpio_cfg(p->bank, p->pin, &p->config);
}

static TEE_Result add_pm_pin(struct stm32_gpio_bank *bank, unsigned int pin)
{
	struct stm32_pinctrl pinctrl = {
		.bank = bank->bank_id,
		.pin = pin,
	};
	struct stm32_pinctrl_backup *backup = NULL;

	STAILQ_FOREACH(backup, &bank->backups, link)
		if (backup->pinctrl.bank == bank->bank_id &&
		    backup->pinctrl.pin == pin)
			return TEE_SUCCESS;

	DMSG("Add PM on pin %c%d", bank->bank_id + 'A', pin);

	return stm32_pinctrl_backup(&pinctrl);
}

static void remove_pm_pin(struct stm32_gpio_bank *bank, unsigned int pin)
{
	struct stm32_pinctrl_backup *backup = NULL;
	struct stm32_pinctrl_backup *next = NULL;

	STAILQ_FOREACH_SAFE(backup, &bank->backups, link, next) {
		if (backup->pinctrl.bank == bank->bank_id &&
		    backup->pinctrl.pin == pin) {
			DMSG("Remove PM on pin %c%d", bank->bank_id + 'A', pin);
			STAILQ_REMOVE(&bank->backups, backup,
				      stm32_pinctrl_backup, link);
			free(backup);
			break;
		}
	}
}

/*
 * Load GPIO pin secure state and ensure pin configuration of secure pins is
 * restored on PM resume transition.
 */
TEE_Result stm32_gpio_set_secure_cfg(unsigned int bank_id, unsigned int pin,
				     bool secure)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct stm32_gpio_bank *bank = NULL;
	uint32_t exceptions = 0;

	bank = stm32_gpio_get_bank(bank_id);
	if (!bank)
		return TEE_ERROR_BAD_PARAMETERS;

	if (!bank->secure_support) {
		assert(!secure);
		return TEE_SUCCESS;
	}

	exceptions = cpu_spin_lock_xsave(&bank->lock);

	if (secure) {
		res = add_pm_pin(bank, pin);
		if (res)
			return res;

		io_setbits32(bank->base + GPIO_SECR_OFFSET, BIT(pin));
	} else {
		remove_pm_pin(bank, pin);
		io_clrbits32(bank->base + GPIO_SECR_OFFSET, BIT(pin));
	}

	cpu_spin_unlock_xrestore(&bank->lock, exceptions);

	return TEE_SUCCESS;
}

TEE_Result stm32_pinctrl_set_secure_cfg(struct stm32_pinctrl_list *list,
					bool secure)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct stm32_pinctrl *p = NULL;

	STAILQ_FOREACH(p, list, link) {
		res = stm32_gpio_set_secure_cfg(p->bank, p->pin, secure);
		if (res)
			return res;
	}

	return TEE_SUCCESS;
}

/* Count pins described in the DT node and get related data if possible */
static TEE_Result get_pinctrl_from_fdt(const void *fdt, int node,
				       struct stm32_pinctrl_list *list)
{
	const fdt32_t *cuint = NULL;
	const fdt32_t *slewrate = NULL;
	int len = 0;
	int pinctrl_node = 0;
	uint32_t i = 0;
	uint32_t speed = GPIO_OSPEED_LOW;
	uint32_t pull = GPIO_PUPD_NO_PULL;

	cuint = fdt_getprop(fdt, node, "pinmux", &len);
	if (!cuint)
		panic();

	pinctrl_node = fdt_parent_offset(fdt, fdt_parent_offset(fdt, node));
	if (pinctrl_node < 0)
		panic();

	slewrate = fdt_getprop(fdt, node, "slew-rate", NULL);
	if (slewrate)
		speed = fdt32_to_cpu(*slewrate);

	if (fdt_getprop(fdt, node, "bias-pull-up", NULL))
		pull = GPIO_PUPD_PULL_UP;
	if (fdt_getprop(fdt, node, "bias-pull-down", NULL))
		pull = GPIO_PUPD_PULL_DOWN;

	for (i = 0; i < ((uint32_t)len / sizeof(uint32_t)); i++) {
		struct stm32_pinctrl *ref = NULL;
		uint32_t pincfg = 0;
		uint32_t bank = 0;
		uint32_t pin = 0;
		uint32_t mode = 0;
		uint32_t alternate = 0;
		uint32_t od = 0;
		bool opendrain = false;

		ref = calloc(1, sizeof(*ref));
		if (!ref)
			return TEE_ERROR_OUT_OF_MEMORY;

		pincfg = fdt32_to_cpu(*cuint);
		cuint++;

		bank = (pincfg & DT_GPIO_BANK_MASK) >> DT_GPIO_BANK_SHIFT;

		pin = (pincfg & DT_GPIO_PIN_MASK) >> DT_GPIO_PIN_SHIFT;

		mode = pincfg & DT_GPIO_MODE_MASK;

		switch (mode) {
		case 0:
			mode = GPIO_MODE_INPUT;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			alternate = mode - 1U;
			mode = GPIO_MODE_ALTERNATE;
			break;
		case 17:
			mode = GPIO_MODE_ANALOG;
			break;
		default:
			mode = GPIO_MODE_OUTPUT;
			break;
		}

		if (fdt_getprop(fdt, node, "drive-open-drain", NULL))
			opendrain = true;

		if (fdt_getprop(fdt, node, "output-high", NULL)) {
			if (mode == GPIO_MODE_INPUT) {
				mode = GPIO_MODE_OUTPUT;
				od = 1;
			}
		}

		if (fdt_getprop(fdt, node, "output-low", NULL)) {
			if (mode == GPIO_MODE_INPUT) {
				mode = GPIO_MODE_OUTPUT;
				od = 0;
			}
		}

		ref->bank = (uint8_t)bank;
		ref->pin = (uint8_t)pin;
		ref->config.mode = mode;
		ref->config.otype = opendrain ? 1 : 0;
		ref->config.ospeed = speed;
		ref->config.pupd = pull;
		ref->config.od = od;
		ref->config.af = alternate;

		STAILQ_INSERT_TAIL(list, ref, link);
	}

	return TEE_SUCCESS;
}

static TEE_Result add_pinctrl(const void *fdt, const int phandle,
			      struct stm32_pinctrl_list **list)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int node = 0;
	int subnode = 0;

	assert(list);
	if (!*list) {
		*list = calloc(1, sizeof(**list));
		if (!*list)
			return TEE_ERROR_OUT_OF_MEMORY;

		STAILQ_INIT(*list);
	}

	node = fdt_node_offset_by_phandle(fdt, phandle);
	if (node < 0)
		panic();

	fdt_for_each_subnode(subnode, fdt, node) {
		res = get_pinctrl_from_fdt(fdt, subnode, *list);
		if (res) {
			EMSG("Failed to get pinctrl: %#"PRIx32, res);
			return res;
		}
	}

	return TEE_SUCCESS;
}

static __unused struct stm32_pinctrl_list *
stm32_pinctrl_dt_get(struct dt_pargs *args __maybe_unused, void *data,
		     TEE_Result *res)
{
	struct stm32_pinctrl_list *list = NULL;
	struct stm32_data_pinctrl *data_pin = (struct stm32_data_pinctrl *)data;

	*res = add_pinctrl(data_pin->fdt, data_pin->phandle, &list);
	return list;
}

/*  Informative unused helper function */
static __unused void free_banks(void)
{
	struct stm32_gpio_bank *bank = NULL;
	struct stm32_gpio_bank *next = NULL;

	STAILQ_FOREACH_SAFE(bank, &bank_list, link, next)
		free(bank);
}

static TEE_Result
stm32_pinctrl_dt_get_by_idx_prop(const char *prop_name, const void *fdt,
				 int nodeoffset,
				 struct stm32_pinctrl_list **plist,
				 bool load_config)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	int len = 0;
	unsigned int i = 0;
	struct stm32_pinctrl_list *glist = NULL;
	struct stm32_pinctrl_list *list = NULL;
	struct stm32_pinctrl *pinctrl = NULL;

	fdt_getprop(fdt, nodeoffset, prop_name, &len);
	if (len <= 0)
		return TEE_ERROR_ITEM_NOT_FOUND;

	for (i = 0; i < (len / sizeof(uint32_t)); i++) {
		list = dt_driver_device_from_node_idx_prop(prop_name,
							   fdt, nodeoffset, i,
							   DT_DRIVER_PINCTRL,
							   &res);
		if (res)
			goto err;

		if (!glist)
			glist = list;
		else
			STAILQ_CONCAT(glist, list);
	}

	if (load_config)
		stm32_pinctrl_load_config(glist);

	*plist = glist;

	return TEE_SUCCESS;

err:
	if (glist) {
		while (!STAILQ_EMPTY(glist)) {
			pinctrl = STAILQ_FIRST(glist);
			STAILQ_REMOVE_HEAD(glist, link);
			free(pinctrl);
		}
		free(glist);
	}

	return res;
}

TEE_Result stm32_pinctrl_dt_get_by_index(const void *fdt, int nodeoffset,
					 unsigned int index,
					 struct stm32_pinctrl_list **plist)
{
	char prop_name[PROP_NAME_MAX] = { };
	int check = 0;

	check = snprintf(prop_name, sizeof(prop_name), "pinctrl-%d", index);
	if (check < 0 || check >= (int)sizeof(prop_name)) {
		DMSG("Wrong property name for pinctrl");
		return TEE_ERROR_GENERIC;
	}

	return stm32_pinctrl_dt_get_by_idx_prop(prop_name, fdt,
						nodeoffset, plist, true);
}

TEE_Result stm32_pinctrl_dt_get_by_name(const void *fdt, int nodeoffset,
					const char *name,
					struct stm32_pinctrl_list **plist)
{
	int idx = 0;
	int check = 0;
	char prop_name[PROP_NAME_MAX] = { };

	idx = fdt_stringlist_search(fdt, nodeoffset, "pinctrl-names", name);
	if (idx == -FDT_ERR_NOTFOUND)
		return TEE_ERROR_ITEM_NOT_FOUND;
	else if (idx < 0)
		return TEE_ERROR_GENERIC;

	check = snprintf(prop_name, sizeof(prop_name), "pinctrl-%d", idx);
	if (check < 0 || check >= (int)sizeof(prop_name)) {
		DMSG("Unexpected name property %s", name);
		return TEE_ERROR_GENERIC;
	}

	return stm32_pinctrl_dt_get_by_idx_prop(prop_name, fdt,
						nodeoffset, plist, true);
}

TEE_Result stm32_pinctrl_dt_read_by_index(const void *fdt, int nodeoffset,
					  unsigned int index,
					  struct stm32_pinctrl_list **plist)
{
	char prop_name[PROP_NAME_MAX] = { };
	int check = 0;

	check = snprintf(prop_name, sizeof(prop_name), "pinctrl-%d", index);
	if (check < 0 || check >= (int)sizeof(prop_name)) {
		DMSG("Wrong property name for pinctrl");
		return TEE_ERROR_GENERIC;
	}

	return stm32_pinctrl_dt_get_by_idx_prop(prop_name, fdt,
						nodeoffset, plist, false);
}

static TEE_Result apply_rif_config(struct stm32_gpio_bank *bank)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t cidcfgr = 0;
	unsigned int i = 0;

	for (i = 0; i < bank->ngpios; i++) {
		if (!(BIT(i) & bank->conf_data.access_mask[0]))
			continue;

		/*
		 * Whatever the TDCID state, try to clear the configurable part
		 * of the CIDCFGR register.
		 * If TDCID, register will be cleared, if not, the clear will
		 * be ignored.
		 * When TDCID, OP-TEE should be the one to set the CID filtering
		 * configuration. Clearing previous configuration prevents
		 * undesired events during the only legitimate configuration.
		 */
		io_clrbits32(bank->base + GPIO_CIDCFGR(i),
			     GPIO_CIDCFGR_CONF_MASK);

		cidcfgr = io_read32(bank->base + GPIO_CIDCFGR(i));

		/* Check if the controller is in semaphore mode */
		if (SEM_MODE_INCORRECT(cidcfgr))
			continue;

		/* If not TDCID, we want to acquire semaphores assigned to us */
		res = stm32_rif_acquire_semaphore(bank->base + GPIO_SEMCR(i),
						  GPIO_MAX_CID_SUPPORTED);
		if (res) {
			EMSG("Couldn't acquire semaphore for pin %u", i);
			return res;
		}
	}

	/* Security and privilege RIF configuration */
	io_clrsetbits32(bank->base + GPIO_PRIVCFGR_OFFSET, GPIO_PRIVCFGR_MASK,
			bank->conf_data.priv_conf[0]);
	io_clrsetbits32(bank->base + GPIO_SECR_OFFSET, GPIO_SECCFGR_MASK,
			bank->conf_data.sec_conf[0]);

	for (i = 0; i < bank->ngpios; i++) {
		if (!(BIT(i) & bank->conf_data.access_mask[0]))
			continue;

		io_clrsetbits32(bank->base + GPIO_CIDCFGR(i),
				GPIO_CIDCFGR_CONF_MASK,
				bank->conf_data.cid_confs[i]);

		cidcfgr = io_read32(bank->base + GPIO_CIDCFGR(i));

		/*
		 * Take semaphore if the resource is in semaphore mode
		 * and secured.
		 */
		if (SEM_MODE_INCORRECT(cidcfgr) ||
		    !(io_read32(bank->base + GPIO_SECR_OFFSET) & BIT(i))) {
			res =
			stm32_rif_release_semaphore(bank->base +
						    GPIO_SEMCR(i),
						    GPIO_MAX_CID_SUPPORTED);
			if (res) {
				EMSG("Couldn't release semaphore for pin%u", i);
				return res;
			}
		} else {
			res =
			stm32_rif_acquire_semaphore(bank->base +
						    GPIO_SEMCR(i),
						    GPIO_MAX_CID_SUPPORTED);
			if (res) {
				EMSG("Couldn't acquire semaphore for pin%u", i);
				return res;
			}
		}
	}

	/*
	 * Lock RIF configuration if configured. This cannot be undone until
	 * next reset.
	 */
	io_clrsetbits32(bank->base + GPIO_RCFGLOCKR_OFFSET, GPIO_RCFGLOCKR_MASK,
			bank->conf_data.lock_conf[0]);

	if (IS_ENABLED(CFG_TEE_CORE_DEBUG)) {
		/* Check that RIF config are applied, panic otherwise */
		if ((io_read32(bank->base + GPIO_PRIVCFGR_OFFSET) &
		     bank->conf_data.access_mask[0]) !=
		    bank->conf_data.priv_conf[0]) {
			EMSG("GPIO bank priv conf (ID: %u) is incorrect",
			     bank->bank_id);
			panic();
		}

		if ((io_read32(bank->base + GPIO_SECR_OFFSET) &
		     bank->conf_data.access_mask[0]) !=
		    bank->conf_data.sec_conf[0]) {
			EMSG("GPIO bank sec conf (ID: %u) is incorrect",
			     bank->bank_id);
			panic();
		}
	}

	return TEE_SUCCESS;
}

static void stm32_gpio_save_rif_config(struct stm32_gpio_bank *bank)
{
	size_t i = 0;

	for (i = 0; i < bank->ngpios; i++)
		bank->conf_data.cid_confs[i] = io_read32(bank->base +
							 GPIO_CIDCFGR(i));

	bank->conf_data.priv_conf[0] = io_read32(bank->base +
						 GPIO_PRIVCFGR_OFFSET);
	bank->conf_data.sec_conf[0] = io_read32(bank->base +
						 GPIO_SECR_OFFSET);
	bank->conf_data.lock_conf[0] = io_read32(bank->base +
						 GPIO_RCFGLOCKR_OFFSET);
}

static void stm32_parse_gpio_rif_conf(struct stm32_gpio_bank *bank,
				      const void *fdt, int node)
{
	uint32_t rif_conf = 0;
	unsigned int i = 0;
	unsigned int nb_rif_conf = 0;
	int lenp = 0;
	const fdt32_t *cuint = NULL;

	cuint = fdt_getprop(fdt, node, "st,protreg", &lenp);
	if (!cuint)
		panic("No RIF configuration available");

	nb_rif_conf = MIN((unsigned int)(lenp / sizeof(uint32_t)),
			  bank->ngpios);

	bank->conf_data.cid_confs = calloc(bank->ngpios, sizeof(uint32_t));
	bank->conf_data.priv_conf = calloc(1, sizeof(uint32_t));
	bank->conf_data.lock_conf = calloc(1, sizeof(uint32_t));
	bank->conf_data.access_mask = calloc(1, sizeof(uint32_t));
	if (!bank->conf_data.cid_confs || !bank->conf_data.access_mask ||
	    !bank->conf_data.priv_conf || !bank->conf_data.lock_conf)
		panic("Missing memory capacity for GPIOS RIF configuration");

	for (i = 0; i < nb_rif_conf; i++) {
		rif_conf = fdt32_to_cpu(cuint[i]);

		stm32_rif_parse_cfg(rif_conf, &bank->conf_data,
				    GPIO_MAX_CID_SUPPORTED, bank->ngpios);
	}
}

static struct stm32_gpio_bank *_fdt_stm32_gpio_controller(const void *fdt,
							  int node,
							  const void *data,
							  int range_offset,
							  TEE_Result *res)
{
	int i = 0;
	size_t blen = 0;
	paddr_t pa = 0;
	int len = 0;
	const fdt32_t *cuint = NULL;
	struct stm32_gpio_bank *bank = NULL;
	struct clk *clk = NULL;
	struct io_pa_va pa_va = { };
	const int dt_name_len = strlen(DT_GPIO_BANK_NAME0);
	const struct stm32_gpio_compat_data *comp_data =
		(struct stm32_gpio_compat_data *)data;

	/* Probe deferrable devices first */
	*res = clk_dt_get_by_index(fdt, node, 0, &clk);
	if (*res)
		return NULL;

	bank = calloc(1, sizeof(*bank));
	if (!bank) {
		*res = TEE_ERROR_OUT_OF_MEMORY;
		return NULL;
	}

	bank->clock = clk;

	/*
	 * Do not rely *only* on the "reg" property to get the address,
	 * but consider also the "ranges" translation property
	 */
	pa = _fdt_reg_base_address(fdt, node);
	if (pa == DT_INFO_INVALID_REG)
		panic("missing reg property");

	pa_va.pa = pa + range_offset;

	blen = _fdt_reg_size(fdt, node);
	if (blen == DT_INFO_INVALID_REG_SIZE)
		panic("missing reg size property");

	DMSG("Bank name %s", fdt_get_name(fdt, node, NULL));
	/* Parse "st,bank-name" to get its id (eg: GPIOA -> 0) */
	cuint = fdt_getprop(fdt, node, "st,bank-name", &len);
	if (!cuint || (len != dt_name_len + 1))
		panic("missing/wrong st,bank-name property");

	if (strncmp((const char *)cuint, DT_GPIO_BANK_NAME0,
		    dt_name_len - 1) != 0)
		panic("wrong st,bank-name property");

	bank->bank_id = strcmp((const char *)cuint, DT_GPIO_BANK_NAME0);

	/* Parse gpio-ranges with its 4 parameters */
	cuint = fdt_getprop(fdt, node, "gpio-ranges", &len);
	if (cuint) {
		len /= sizeof(*cuint);
		if ((len % 4) != 0)
			panic("wrong gpio-ranges syntax");

		/* Get the last defined gpio line (offset + nb of pins) */
		for (i = 0; i < len / 4; i++) {
			bank->ngpios = MAX(bank->ngpios,
				   (unsigned int)(fdt32_to_cpu(*(cuint + 1)) +
						  fdt32_to_cpu(*(cuint + 3))));
			cuint += 4;
		}
	} else if (len != -FDT_ERR_NOTFOUND) {
		panic();
	}

	bank->secure_support = comp_data->secure_control;
	bank->secure_extended = comp_data->secure_extended;

	bank->conf_data.sec_conf = calloc(1, sizeof(uint32_t));
	if (!bank->conf_data.sec_conf) {
		free(bank);
		*res = TEE_ERROR_OUT_OF_MEMORY;
		return NULL;
	}

	if (bank->secure_extended) {
		/* RIF configuration */
		bank->base = io_pa_or_va_secure(&pa_va, blen);

		stm32_parse_gpio_rif_conf(bank, fdt, node);
	} else if (bank->secure_support) {
		/* Secure configuration */
		bank->base = io_pa_or_va_secure(&pa_va, blen);
		cuint = fdt_getprop(fdt, node, "st,protreg", NULL);
		if (cuint)
			bank->conf_data.sec_conf[0] = fdt32_to_cpu(*cuint);
		else
			DMSG("GPIO bank %c assigned to non-secure",
			     bank->bank_id + 'A');
	} else {
		bank->base = io_pa_or_va_nsec(&pa_va, blen);
	}

	*res = TEE_SUCCESS;

	return bank;
}

static void stm32_gpio_get_conf_sec(struct stm32_gpio_bank *bank)
{
	if (bank->secure_support)
		bank->conf_data.sec_conf[0] = io_read32(bank->base +
							GPIO_SECR_OFFSET);
}

static void stm32_gpio_set_conf_sec(struct stm32_gpio_bank *bank)
{
	if (bank->secure_support)
		io_write32(bank->base + GPIO_SECR_OFFSET,
			   bank->conf_data.sec_conf[0]);
}

static TEE_Result stm32_gpio_pm_resume(void)
{
	struct stm32_gpio_bank *bank = NULL;
	struct stm32_pinctrl_backup *backup = NULL;

	STAILQ_FOREACH(bank, &bank_list, link) {
		if (bank->secure_extended) {
			bool is_tdcid = false;

			if (stm32_rifsc_check_tdcid(&is_tdcid))
				panic();

			if (!is_tdcid)
				return TEE_SUCCESS;

			bank->conf_data.access_mask[0] =
					GENMASK_32(bank->ngpios, 0);

			apply_rif_config(bank);
		} else {
			stm32_gpio_set_conf_sec(bank);
		}

		STAILQ_FOREACH(backup, &bank->backups, link)
			       set_gpio_cfg(backup->pinctrl.bank,
					    backup->pinctrl.pin,
					    &backup->pinctrl.config);
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_gpio_pm_suspend(void)
{
	struct stm32_gpio_bank *bank = NULL;
	struct stm32_pinctrl_backup *backup = NULL;

	STAILQ_FOREACH(bank, &bank_list, link) {
		if (bank->secure_extended) {
			bool is_tdcid = false;

			if (stm32_rifsc_check_tdcid(&is_tdcid))
				panic();

			if (!is_tdcid)
				return TEE_SUCCESS;

			stm32_gpio_save_rif_config(bank);
		} else {
			stm32_gpio_get_conf_sec(bank);
		}

		STAILQ_FOREACH(backup, &bank->backups, link)
			       get_gpio_cfg(bank, backup->pinctrl.pin,
					    &backup->pinctrl.config);
	}

	return TEE_SUCCESS;
}

static TEE_Result
stm32_gpio_pm(enum pm_op op, unsigned int pm_hint,
	      const struct pm_callback_handle *pm_handle __unused)
{
	TEE_Result ret = 0;

	if (!IS_ENABLED(CFG_STM32MP13) && !IS_ENABLED(CFG_STM32MP15) &&
	    !PM_HINT_IS_STATE(pm_hint, CONTEXT))
		return TEE_SUCCESS;

	if (op == PM_OP_RESUME)
		ret = stm32_gpio_pm_resume();
	else
		ret = stm32_gpio_pm_suspend();

	return ret;
}
DECLARE_KEEP_PAGER(stm32_gpio_pm);

static TEE_Result stm32_gpio_parse_pinctrl_node(const void *fdt, int node,
						const void *data)
{
	get_of_device_func get_func = (get_of_device_func)stm32_pinctrl_dt_get;
	TEE_Result res = TEE_SUCCESS;
	const fdt32_t *cuint = NULL;
	int subnode = 0;
	int len = 0;
	int range_offset = 0;

	/* Read the ranges property (for regs memory translation) */
	cuint = fdt_getprop(fdt, node, "ranges", &len);
	if (!cuint)
		panic("missing ranges property");

	len /= sizeof(*cuint);
	if (len == 3)
		range_offset = fdt32_to_cpu(*(cuint + 1)) -
			fdt32_to_cpu(*cuint);

	fdt_for_each_subnode(subnode, fdt, node) {
		/*
		 * Parse all pinctrl sub-nodes which can be
		 * "gpio-controller" or pinctrl definitions.
		 */
		struct stm32_gpio_bank *bank = NULL;

		cuint = fdt_getprop(fdt, subnode, "gpio-controller", NULL);
		if (!cuint) {
			struct stm32_data_pinctrl *pdata = NULL;
			uint32_t phandle = fdt_get_phandle(fdt, subnode);

			if (!phandle) {
				/* Node without phandles will not be consumed */
				continue;
			}

			pdata = calloc(1, sizeof(*pdata));
			pdata->fdt = fdt;
			pdata->phandle = phandle;

			res = dt_driver_register_provider(fdt, subnode,
							  get_func,
							  (void *)pdata,
							  DT_DRIVER_PINCTRL);
			if (res)
				return res;
		} else {
			/* Register "gpio-controller" */
			if (_fdt_get_status(fdt, subnode) == DT_STATUS_DISABLED)
				continue;

			bank = _fdt_stm32_gpio_controller(fdt, subnode, data,
							  range_offset, &res);

			if (bank) {
				STAILQ_INIT(&bank->backups);

				bank->ready = false;

				DMSG("Registered GPIO bank %c (%d pins) @%lx",
				     bank->bank_id + 'A', bank->ngpios,
				     bank->base);
				STAILQ_INSERT_TAIL(&bank_list, bank, link);
			} else if (res) {
				return res;
			}
		}
	}

	return TEE_SUCCESS;
}

/*
 * Several pinctrl nodes can be probed. Their bank will be put in the unique
 * bank_list. To avoid multiple configuration set for a bank when looping
 * over each bank in the bank list, ready is set to true when a bank is
 * configured. Therefore, during other bank probes, the configuration won't
 * be set again.
 */
static TEE_Result apply_sec_cfg(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct stm32_gpio_bank *bank = NULL;

	STAILQ_FOREACH(bank, &bank_list, link) {
		if (bank->ready)
			continue;

		clk_enable(bank->clock);

		if (bank->secure_extended) {
			res = apply_rif_config(bank);
			if (res) {
				EMSG("Failed to set GPIO RIF, bank %u",
				     bank->bank_id);
				return res;
			}
		} else {
			stm32_gpio_set_conf_sec(bank);
		}

		bank->ready = true;
	}

	return TEE_SUCCESS;
}

static TEE_Result stm32_gpio_probe(const void *fdt, int offs,
				   const void *compat_data)
{
	static bool pm_register;
	TEE_Result res = TEE_ERROR_GENERIC;

	/* Look for gpio banks inside that node */
	res = stm32_gpio_parse_pinctrl_node(fdt, offs, compat_data);
	if (res)
		return res;

	if (STAILQ_EMPTY(&bank_list)) {
		DMSG("no gpio bank for that driver");
	} else {
		res = apply_sec_cfg();
		if (res)
			panic();
	}

	if (!pm_register) {
		/* Register to PM once for all probed banks */
		register_pm_core_service_cb(stm32_gpio_pm, NULL,
					    "stm32-gpio-service");
		pm_register = true;
	}

	return TEE_SUCCESS;
}

static const struct dt_device_match stm32_gpio_match_table[] = {
	{
		.compatible = "st,stm32mp135-pinctrl",
		.compat_data = (void *)&stm32_gpio_secure_capable,
	},
	{
		.compatible = "st,stm32mp157-pinctrl",
		.compat_data = (void *)&stm32_gpio_non_sec,
	},
	{
		.compatible = "st,stm32mp157-z-pinctrl",
		.compat_data = (void *)&stm32_gpio_secure_capable,
	},
	{
		.compatible = "st,stm32mp257-pinctrl",
		.compat_data = (void *)&stm32_gpio_with_sec_ext
	},
	{
		.compatible = "st,stm32mp257-z-pinctrl",
		.compat_data = (void *)&stm32_gpio_with_sec_ext
	},
	{ }
};

DEFINE_DT_DRIVER(stm32_gpio_dt_driver) = {
	.name = "stm32_gpio",
	.match_table = stm32_gpio_match_table,
	.probe = stm32_gpio_probe,
};
