// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2020-2021, STMicroelectronics - All Rights Reserved
 * Author(s): Ludovic Barre, <ludovic.barre@st.com> for STMicroelectronics.
 */

#include <assert.h>
#include <drivers/counter.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <stdlib.h>

static unsigned int list_lock = SPINLOCK_UNLOCK;

static LIST_HEAD(cnt_dev_list_head, counter_device) cnt_dev_list =
	LIST_HEAD_INITIALIZER(cnt_dev_list_head);

TEE_Result counter_start(struct counter_device *counter, void *config)
{
	uint32_t exceptions = 0;

	assert(counter && counter->ops);

	if (!counter->ops->start)
		return TEE_ERROR_NOT_SUPPORTED;

	exceptions = cpu_spin_lock_xsave(&list_lock);
	if (counter->is_used) {
		cpu_spin_unlock_xrestore(&list_lock, exceptions);
		return TEE_ERROR_BUSY;
	}

	counter->is_used = true;
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	return counter->ops->start(counter, config);
}

/*
 * some counter device could not be stopped
 * return TEE_SUCCESS if no stop ops
 */
TEE_Result counter_stop(struct counter_device *counter)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t exceptions = 0;

	assert(counter && counter->ops);

	if (!counter->ops->stop)
		return TEE_SUCCESS;

	res = counter->ops->stop(counter);
	if (!res) {
		exceptions = cpu_spin_lock_xsave(&list_lock);
		counter->is_used = false;
		cpu_spin_unlock_xrestore(&list_lock, exceptions);
	}

	return res;
}

TEE_Result counter_get_value(struct counter_device *counter, unsigned int *ticks)
{
	assert(counter && counter->ops);

	if (!counter->ops->get_value)
		return TEE_ERROR_NOT_SUPPORTED;

	return counter->ops->get_value(counter, ticks);
}

TEE_Result counter_set_alarm(struct counter_device *counter)
{
	struct alarm_cfg *alarm = NULL;
	TEE_Result res = TEE_SUCCESS;

	assert(counter && counter->ops);

	alarm = &counter->alarm;

	if (alarm->is_enabled || !alarm->callback || !counter->ops->set_alarm)
		return TEE_ERROR_NOT_SUPPORTED;

	if (alarm->ticks > counter->max_ticks)
		return TEE_ERROR_NOT_SUPPORTED;

	res = counter->ops->set_alarm(counter);
	if (!res)
		counter->alarm.is_enabled = true;

	return res;
}

TEE_Result counter_cancel_alarm(struct counter_device *counter)
{
	TEE_Result res = TEE_SUCCESS;

	assert(counter);

	if (!counter->ops->cancel_alarm)
		return TEE_ERROR_NOT_SUPPORTED;

	res = counter->ops->cancel_alarm(counter);
	if (!res)
		counter->alarm.is_enabled = false;

	return res;
}

void counter_release_config(struct counter_device *counter,
			    void *config)
{
	assert(counter && config);

	if (counter->ops->release_config)
		counter->ops->release_config(config);
}

#ifdef CFG_DT
struct counter_device *fdt_counter_get(const void *fdt, int offs, void **config)
{
	struct counter_device *cnt_dev = NULL;
	const fdt32_t *cuint = NULL;
	int len = 0;
	uint32_t exceptions = 0;
	int phandle = 0;

	assert(fdt && config);

	cuint = fdt_getprop(fdt, offs, "counter", &len);
	if (!cuint)
		return NULL;

	phandle = fdt32_to_cpu(*cuint);

	exceptions = cpu_spin_lock_xsave(&list_lock);

	LIST_FOREACH(cnt_dev, &cnt_dev_list, dev_list) {
		if (cnt_dev->phandle == phandle)
			break;
	}
	if (!cnt_dev)
		EMSG("No counter device registered for phandle: %d", phandle);

	if ((uint32_t)len < sizeof(uint32_t))
		goto out;

	len -= sizeof(uint32_t);
	cuint++;

	/* Len in bytes */
	len /= sizeof(uint32_t);

	if (cnt_dev && cnt_dev->ops->set_config)
		cnt_dev->ops->set_config(cnt_dev, cuint, len, config);

out:
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	return cnt_dev;
}
#else
TEE_Result counter_get_config(struct counter_device *cnt_dev,
			      const struct counter_param *param,
			      void **config)
{
	if (!param)
		return TEE_ERROR_BAD_PARAMETERS;

	if (!cnt_dev->ops->set_config)
		return TEE_ERROR_NOT_SUPPORTED;

	if (cnt_dev && cnt_dev->ops->set_config)
		cnt_dev->ops->set_config(cnt_dev, param->params,
					 param->len, config);

	return TEE_SUCCESS;
}
#endif

struct counter_device *counter_get_by_name(const char *name)
{
	struct counter_device *cnt_dev = NULL;
	uint32_t exceptions;

	assert(name);

	exceptions = cpu_spin_lock_xsave(&list_lock);

	LIST_FOREACH(cnt_dev, &cnt_dev_list, dev_list) {
		if (!strcmp(cnt_dev->name, name))
			break;
	}

	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	return cnt_dev;
}

struct counter_device *counter_dev_alloc(void)
{
	return calloc(1, sizeof(struct counter_device));
}

void counter_dev_free(struct counter_device *counter)
{
	uint32_t exceptions;

	assert(counter);

	exceptions = cpu_spin_lock_xsave(&list_lock);
	LIST_REMOVE(counter, dev_list);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	free(counter);
}

TEE_Result counter_dev_register(struct counter_device *counter)
{
	uint32_t exceptions;

	assert(counter && counter->name && counter->ops);

	exceptions = cpu_spin_lock_xsave(&list_lock);
	LIST_INSERT_HEAD(&cnt_dev_list, counter, dev_list);
	cpu_spin_unlock_xrestore(&list_lock, exceptions);

	return TEE_SUCCESS;
}
