// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#include <assert.h>
#include <drivers/adc.h>
#include <io.h>
#include <libfdt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <trace.h>
#include <util.h>

static SLIST_HEAD(adc_consumer_list_head, adc_consumer) adc_cons_list =
	SLIST_HEAD_INITIALIZER(adc_consumer_list_head);

TEE_Result adc_trylock(struct adc_device *dev)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	res = mutex_trylock(&dev->lock) ? TEE_SUCCESS : TEE_ERROR_BUSY;

	if (dev->state) {
		DMSG("ADC is %s already running", dev->name);
		mutex_unlock(&dev->lock);
		res = TEE_ERROR_BUSY;
	}

	return res;
}

void adc_unlock(struct adc_device *dev)
{
	mutex_unlock(&dev->lock);
}

TEE_Result adc_get_by_name(const char *adc_name, struct adc_device **adc_dev)
{
	struct adc_consumer *c = NULL;

	SLIST_FOREACH(c, &adc_cons_list, link) {
		if (!strncmp(adc_name, c->dev->name, strlen(c->dev->name))) {
			*adc_dev = c->dev;
			return TEE_SUCCESS;
		}
	}

	EMSG("ADC Device '%s' not found", adc_name);

	return TEE_ERROR_ITEM_NOT_FOUND;
}

TEE_Result adc_print_info(const char *adc_name)
{
	struct adc_device *adc_dev = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t i;

	res = adc_get_by_name(adc_name, &adc_dev);
	if (res) {
		EMSG("ADC Device '%s' not found", adc_name);
		return res;
	}

	IMSG("ADC Device '%s':", adc_dev->name);
	IMSG("Channel mask: %#"PRIx32, adc_dev->channel_mask);
	IMSG("Channels:");
	for (i = 0; i < adc_dev->channels_nb; i++)
		IMSG("\tChannel id=%u label='%s'",
		     adc_dev->channels[i].id,
		     adc_dev->channels[i].name);

	IMSG("Data mask: %#"PRIx32, adc_dev->data_mask);
	IMSG("VDD: %"PRIu16"mV", adc_dev->vref_mv);

	return TEE_SUCCESS;
}

static TEE_Result adc_channel_read_raw(struct adc_device *dev,
				       uint32_t channel, uint32_t *data)
{
	struct adc_ops *ops = dev->ops;

	if (!ops || !ops->read_channel)
		return TEE_ERROR_NOT_IMPLEMENTED;

	return ops->read_channel(dev, channel, data);
}

static TEE_Result adc_conv_raw_data(struct adc_device *dev,
				    uint32_t data_raw, int *data_uv)
{
	uint64_t data = data_raw;

	data *= 1000 * dev->vref_mv;
	*data_uv = UDIV_ROUND_NEAREST(data, dev->data_mask);

	return TEE_SUCCESS;
}

TEE_Result adc_clr_event_all(const char *adc_name, unsigned int id)
{
	struct adc_device *adc_dev = NULL;
	struct adc_consumer *c = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	res = adc_get_by_name(adc_name, &adc_dev);
	if (res)
		return res;

	SLIST_FOREACH(c, &adc_cons_list, link) {
		if (c->evt && c->dev == adc_dev && c->evt->id == id) {
			res = adc_consumer_clr_event(c, c->evt);
			if (res)
				return res;
		}
	}

	return res;
}

static TEE_Result adc_consumer_lookup(const char *adc_name, uint32_t channel,
				      struct adc_consumer **adc_cons)
{
	struct adc_consumer *c = NULL;

	SLIST_FOREACH(c, &adc_cons_list, link)
		if (c->channel == channel && !strcmp(adc_name, c->dev->name))
			break;

	if (!c) {
		EMSG("No consumer found for channel %"PRIu32" on ADC %s",
		     channel, adc_name);
		return TEE_ERROR_ITEM_NOT_FOUND;
	}

	*adc_cons = c;

	return TEE_SUCCESS;
}

TEE_Result adc_consumer_get_by_name(const char *adc_name, uint32_t channel,
				    struct adc_consumer **cons)
{
	return adc_consumer_lookup(adc_name, channel, cons);
}

TEE_Result adc_consumer_register(struct adc_device *dev,
				 uint32_t channel, struct adc_consumer **cons)
{
	struct adc_consumer *adc_cons = NULL;

	adc_cons = calloc(1, sizeof(*adc_cons));
	if (!adc_cons)
		return TEE_ERROR_OUT_OF_MEMORY;

	adc_cons->dev = dev;
	adc_cons->channel = channel;
	*cons = adc_cons;

	SLIST_INSERT_HEAD(&adc_cons_list, adc_cons, link);

	return TEE_SUCCESS;
}

void adc_consumer_print_list(void)
{
	struct adc_consumer *cons = NULL;

	IMSG("ADC consumers:");

	SLIST_FOREACH(cons, &adc_cons_list, link) {
		IMSG("ADC %s : channel %"PRIu32,
		     cons->dev->name, cons->channel);
		if (cons->evt)
			IMSG("Event id=%u lt=%#"PRIx32" ht=%#"PRIx32,
			     cons->evt->id, cons->evt->lt, cons->evt->ht);
	}
}

static TEE_Result adc_consumer_get_count(const void *fdt, int node,
					 size_t *nb_cons)
{
	int len = 0;
	int channel_cells = 0;
	int parent_node = 0;
	int idx = 0;
	int cnt = 0;
	uint32_t phandle = 0;
	const uint32_t *prop = NULL;

	prop = fdt_getprop(fdt, node, "io-channels", &len);
	if (!prop) {
		*nb_cons = 0;
		return TEE_SUCCESS;
	}

	len /= sizeof(uint32_t);
	while (idx < len) {
		phandle = fdt32_to_cpu(prop[idx]);
		parent_node = fdt_node_offset_by_phandle(fdt, phandle);
		if (parent_node < 0)
			return TEE_ERROR_GENERIC;

		channel_cells = fdt_get_dt_driver_cells(fdt, parent_node,
							DT_DRIVER_ADC);
		if (channel_cells < 0)
			return TEE_ERROR_GENERIC;

		idx += 1 + channel_cells;
		cnt++;
	}

	*nb_cons = cnt;

	return TEE_SUCCESS;
}

TEE_Result adc_consumer_get_all(const void *fdt, int node, size_t *nb_cons,
				struct adc_consumer ***cons)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t i = 0;
	size_t cnt = 0;
	struct adc_consumer **c = NULL;
	struct adc_consumer **p = NULL;

	res = adc_consumer_get_count(fdt, node, &cnt);
	if (res)
		return res;

	if (!cnt) {
		DMSG("No ADC consumers found");
		return res;
	}
	*nb_cons = cnt;

	c = calloc(cnt, sizeof(*c));
	if (!c)
		return TEE_ERROR_OUT_OF_MEMORY;
	p = c;

	while (i < cnt) {
		*c = dt_driver_device_from_node_idx_prop("io-channels",
							 fdt, node, i,
							 DT_DRIVER_ADC,
							 &res);
		if (res) {
			free(p);

			if (res != TEE_ERROR_DEFER_DRIVER_INIT)
				EMSG("Failed to get ADC channel");
			return res;
		}

		DMSG("Found channel %d on ADC %s",
		     (*c)->channel, (*c)->dev->name);

		i++;
		c++;
	}

	*cons = p;

	return TEE_SUCCESS;
}

TEE_Result adc_consumer_read_processed(struct adc_consumer *cons,
				       int32_t *uv)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t data = 0;

	res = adc_trylock(cons->dev);
	if (res)
		return res;

	res = adc_channel_read_raw(cons->dev, cons->channel, &data);
	if (!res)
		adc_conv_raw_data(cons->dev, data, uv);

	adc_unlock(cons->dev);

	return res;
}

TEE_Result adc_consumer_set_event(struct adc_consumer *cons,
				  struct adc_evt *evt)
{
	struct adc_ops *ops = cons->dev->ops;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!ops || !ops->set_event)
		return TEE_ERROR_NOT_IMPLEMENTED;

	res = adc_trylock(cons->dev);
	if (res)
		return res;

	res = ops->set_event(cons->dev, evt, cons->channel);
	if (res)
		goto err;

	cons->evt = calloc(1, sizeof(*cons->evt));
	if (!cons->evt) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto err;
	}

	memcpy(cons->evt, evt, sizeof(*cons->evt));
	res = TEE_SUCCESS;

err:
	adc_unlock(cons->dev);

	return res;
}

TEE_Result adc_consumer_clr_event(struct adc_consumer *cons,
				  struct adc_evt *evt)
{
	struct adc_ops *ops = cons->dev->ops;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!ops || !ops->clr_event)
		return TEE_ERROR_NOT_IMPLEMENTED;

	res = adc_trylock(cons->dev);
	if (res)
		return res;

	if (!cons->evt) {
		EMSG("No event found");
		res = TEE_ERROR_NO_DATA;
		goto err;
	}

	free(cons->evt);
	cons->evt = NULL;

	res = ops->clr_event(cons->dev, evt, cons->channel);

err:
	adc_unlock(cons->dev);

	return res;
}

TEE_Result adc_consumer_start_conv(struct adc_device *adc_dev,
				   uint32_t channel_mask)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct adc_ops *ops = NULL;

	if ((channel_mask & adc_dev->channel_mask) != channel_mask ||
	    !channel_mask) {
		EMSG("Wrong channel mask %#"PRIx32" for ADC %s",
		     channel_mask, adc_dev->name);

		return TEE_ERROR_BAD_PARAMETERS;
	}

	res = adc_trylock(adc_dev);
	if (res)
		return res;

	ops = adc_dev->ops;
	if (!ops || !ops->start_conv) {
		res = TEE_ERROR_NOT_IMPLEMENTED;
		goto err;
	}

	res = ops->start_conv(adc_dev, channel_mask);
	if (!res)
		adc_dev->state = 1;

err:
	adc_unlock(adc_dev);

	return res;
}

TEE_Result adc_consumer_stop_conv(struct adc_device *adc_dev)
{
	struct adc_ops *ops = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	mutex_lock(&adc_dev->lock);

	ops = adc_dev->ops;
	if (!ops || !ops->stop_conv)
		return TEE_ERROR_NOT_IMPLEMENTED;

	if (!adc_dev->state) {
		res = TEE_ERROR_BAD_STATE;
		goto err;
	}

	res = ops->stop_conv(adc_dev);
	if (res) {
		EMSG("Failed to stop ADC");
		goto err;
	}
	adc_dev->state = 0;

err:
	mutex_unlock(&adc_dev->lock);

	return res;
}
