/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2020-2021, STMicroelectronics - All Rights Reserved
 */

#ifndef COUNTER_H
#define COUNTER_H

#include <libfdt.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/queue.h>
#include <tee_api_types.h>

/**
 * @brief Alarm structure.
 *
 * @param callback Callback called on alarm (cannot be NULL).
 * @param ticks Number of ticks that triggers the alarm (absolute value).
 * @param priv Private data returned in callback.
 * @param is_enabled True if alarm is enabled, false otherwise.
 */
struct alarm_cfg {
	void (*callback)(unsigned int ticks, void *priv);
	unsigned int ticks;
	void *priv;
	bool is_enabled;
};

struct counter_device;

struct counter_param {
	uint32_t *params;
	size_t len;
};

struct counter_ops {
	TEE_Result (*start)(struct counter_device *counter, void *config);
	TEE_Result (*stop)(struct counter_device *counter);
	TEE_Result (*get_value)(struct counter_device *counter, unsigned int *ticks);
	TEE_Result (*set_alarm)(struct counter_device *counter);
	TEE_Result (*cancel_alarm)(struct counter_device *counter);
	TEE_Result (*set_config)(struct counter_device *counter,
				 const void *param,
				 int len, void **config);
	void (*release_config)(void *config);
};

/**
 * @brief Start counter device in free running mode.
 *
 * @param name name of device.
 * @param phandle dt phandle.
 * @param dev_list list on counter device.
 * @param is_used True if counter is used (exclusive consumer).
 * @param ops Operation table of the counter.
 * @param alarm Alarm configuration of the counter.
 * @param max_ticks Tick max value supported by the counter.
 * @param priv Optional private data supplied by driver.
 */
struct counter_device {
	const char *name;
	int phandle;
	LIST_ENTRY(counter_device) dev_list;
	bool is_used;
	const struct counter_ops *ops;
	struct alarm_cfg alarm;
	unsigned int max_ticks;
	void *priv;
};

/**
 * @brief Start counter device in free running mode.
 */
TEE_Result counter_start(struct counter_device *counter, void *config);

/**
 * @brief Stop counter device.
 */
TEE_Result counter_stop(struct counter_device *counter);

/**
 * @brief Get current counter value.
 */
TEE_Result counter_get_value(struct counter_device *counter, unsigned int *ticks);

/**
 * @brief Set an alarm.
 */
TEE_Result counter_set_alarm(struct counter_device *counter);

/**
 * @brief Cancel an alarm.
 */
TEE_Result counter_cancel_alarm(struct counter_device *counter);

/**
 * @brief Release the counter configuration.
 */
void counter_release_config(struct counter_device *counter, void *config);

#ifdef CFG_DT
/**
 * @brief Parse and lookup a counter referenced by a device node.
 * Retrieve an associated configuration.
 *
 * @retval counter device if successful, else 0 on error.
 */
struct counter_device *fdt_counter_get(const void *fdt,
				       int offs, void **config);
#else
/**
 * @brief Give the counter associated configuration link to given
 * parameters.
 *
 * @retval TEE_SUCCESS if config is returned, error value otherwise.
 */
TEE_Result counter_get_config(struct counter_device *cnt_dev,
			      const struct counter_param *param,
			      void **config);
#endif

/**
 * @brief get a reference on counter device.
 *
 * @retval counter device if successful, else 0 on error.
 */
struct counter_device *counter_get_by_name(const char *name);

/* API for provider */
static inline void *counter_priv(struct counter_device *counter)
{
	return (void *)counter->priv;
}

struct counter_device *counter_dev_alloc(void);
void counter_dev_free(struct counter_device *counter);
TEE_Result counter_dev_register(struct counter_device *counter);

#endif /* COUNTER_H */
