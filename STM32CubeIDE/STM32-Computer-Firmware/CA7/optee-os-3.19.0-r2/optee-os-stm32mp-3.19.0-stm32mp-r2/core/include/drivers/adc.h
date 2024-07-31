/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#ifndef __DRIVERS_ADC_H
#define __DRIVERS_ADC_H

#include <kernel/dt_driver.h>
#include <kernel/mutex.h>

struct adc_ops;

/**
 * struct adc_evt - event configuration filled by ADC user
 * @id: ADC event identifier
 * @lt: ADC event low threshold (raw value)
 * @ht: ADC event high threshold (raw value)
 */
struct adc_evt {
	unsigned int id;
	uint32_t lt;
	uint32_t ht;
};

/**
 * struct adc_chan - ADC channel descriptor
 * @id: ADC channel identifier
 * @name: ADC channel name
 * @smpt: ADC channel minimum sampling time
 */
struct adc_chan {
	unsigned int id;
	const char *name;
	uint32_t smpt;
};

/**
 * struct adc_device - ADC device
 * @ops: ADC device operations pointer
 * @name: ADC device instance name
 * @state: ADC state (set when ADC is active)
 * @lock: Protect ADC device against concurrent accessesw
 * @channels: ADC channels table
 * @channels_nb: ADC channels number
 * @channel_mask: Bit mask of available channels
 * @data_mask: Conversion output data mask
 * @data: Pointer to private data
 * @vref_mv: Reference supply voltage level in mV
 */
struct adc_device {
	struct adc_ops *ops;
	char *name;
	int state;
	struct mutex lock; /* Protection against concurrent accesses */
	struct adc_chan *channels;
	size_t channels_nb;
	uint32_t channel_mask;
	uint32_t data_mask;
	void *data;
	uint16_t vref_mv;
};

/**
 * struct adc_consumer - ADC consumer
 * @dev: Pointer to ADC device provider
 * @evt: Pointer to event descriptor
 * @channel: ADC consumer channel
 * @link: ADC consumer chained list pointer
 */
struct adc_consumer {
	struct adc_device *dev;
	struct adc_evt *evt;
	uint32_t channel;
	SLIST_ENTRY(adc_consumer) link;
};

/**
 * struct adc_ops - ADC driver callbacks
 * @read_channel: function to read a data from an ADC device channel
 */
struct adc_ops {
	TEE_Result (*read_channel)(struct adc_device *dev, uint32_t channel,
				   uint32_t *data);
	TEE_Result (*set_event)(struct adc_device *dev, struct adc_evt *evt,
				uint32_t channel);
	TEE_Result (*clr_event)(struct adc_device *dev, struct adc_evt *evt,
				uint32_t channel);
	TEE_Result (*start_conv)(struct adc_device *dev, uint32_t channel_mask);
	TEE_Result (*stop_conv)(struct adc_device *dev);
};

static inline void *adc_get_drv_data(struct adc_device *dev)
{
	return dev->data;
}

static inline void adc_set_drv_data(struct adc_device *dev, void *data)
{
	dev->data = data;
}

static inline void adc_register(struct adc_device *dev, struct adc_ops *ops)
{
	dev->ops = ops;
	mutex_init(&dev->lock);
}

static inline void adc_unregister(struct adc_device *dev)
{
	dev->ops = NULL;
}

/**
 * adc_get_by_name() - get ADC device handle from ADC name
 *
 * @adc_name: ADC instance name
 * @adc_dev: [out] Pointer to the ADC device handle
 * Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_get_by_name(const char *adc_name, struct adc_device **adc_dev);

/**
 * adc_trylock() - try to get lock on an ADC device
 *
 * @dev: ADC device
 * Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_trylock(struct adc_device *dev);

/**
 * adc_unlock() - try to get lock on an ADC device
 *
 * @dev: ADC device
 */
void adc_unlock(struct adc_device *dev);

/**
 * adc_clr_event_all() - clear an event on all consumers of an ADC device
 *
 * @adc_name: ADC instance name
 * @id: event identifier
 * @Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_clr_event_all(const char *adc_name, unsigned int id);

/**
 * adc_consumer_get_by_name() - get consumer from ADC name and channel index
 *
 * @adc_name: ADC instance name
 * @channel: ADC channel number
 * @cons: [out] Pointer to the ADC consumer handle
 * Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_get_by_name(const char *adc_name, uint32_t channel,
				    struct adc_consumer **cons);

/**
 * adc_consumer_print_list() - display the list of registered ADC consumers
 */
void adc_consumer_print_list(void);

/**
 * adc_print_info() - display ADC device info
 *
 * @adc_name: ADC instance name
 * Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_print_info(const char *adc_name);

/**
 * adc_consumer_register() - register an ADC device consumer in ADC framework
 *
 * @dev: ADC device
 * @channel: ADC channel number
 * @cons: [out] Pointer to the registered ADC consumer handle
 * Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_register(struct adc_device *dev, uint32_t channel,
				 struct adc_consumer **cons);

/**
 * adc_consumer_get_all() - get all ADC consumers for node
 *
 * @fdt: Pointer to the device tree blob
 * @node: Offset of a tree node
 * @nb_cons: [out] Pointer to ADC consumers number in @cons array
 * @cons: [out] Pointer to consumers pointer array
 * Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_get_all(const void *fdt, int node, size_t *nb_cons,
				struct adc_consumer ***cons);

/**
 * adc_consumer_read_processed() - launch a single conversion on an ADC channel
 *
 * @cons: ADC consumer handle
 * @uv: [out] Pointer to the converted data in uV
 * Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_read_processed(struct adc_consumer *cons, int32_t *uv);

/**
 * adc_consumer_set_event() - configure an event for an ADC consumer
 *
 * @cons: ADC consumer handle
 * @evt: Event descriptor pointer
 * @Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_set_event(struct adc_consumer *cons,
				  struct adc_evt *evt);

/**
 * adc_consumer_clr_event() - clear an event for an ADC consumer
 *
 * @cons: ADC consumer handle
 * @evt: Event descriptor pointer
 * @Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_clr_event(struct adc_consumer *cons,
				  struct adc_evt *evt);

/**
 * adc_consumer_start_conv() - launch channel(s) conversions on an ADC device
 *
 * @dev: ADC device handle
 * @channel_mask: ADC channel mask
 * @Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_start_conv(struct adc_device *dev,
				   uint32_t channel_mask);

/**
 * adc_consumer_stop_conv() - stop channel(s) conversions on an ADC device
 *
 * @dev: ADC device handle
 * @Return: TEE_SUCCESS on success, error code otherwise
 */
TEE_Result adc_consumer_stop_conv(struct adc_device *dev);
#endif /* __DRIVERS_ADC_H */
