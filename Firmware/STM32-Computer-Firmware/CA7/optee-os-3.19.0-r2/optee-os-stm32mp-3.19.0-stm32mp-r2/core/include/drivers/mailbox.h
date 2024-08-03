/* SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 STMicroelectronics - All Rights Reserved
 */

/**
 * @file
 *
 * @brief inter-processor mailbox communication framework.
 */

#ifndef __DRIVERS_MAILBOX_H
#define __DRIVERS_MAILBOX_H

#include <sys/queue.h>
#include <tee_api_defines_extensions.h>
#include <tee_api_types.h>
#include <util.h>

/*
 * Opaque handle for a mailbox channel
 */
struct mbox_chan;

/*
 * struct mbox_desc - mailbox descriptor provided by mailbox device.
 */
struct mbox_desc;

/**
 * @typedef mbox_callback_t
 * @brief Callback API for incoming mbox messages and outgoing mbox messages
 * completion.
 *
 * These callbacks are executed in an interrupt context.
 * Registration of callbacks is done with mbox_register_chan() or
 * mbox_register_chan_by_name().
 *
 * @cookie: Pointer to consumer private data provided during registration.
 */
typedef void (*mbox_callback_t)(const void *cookie);

/**
 * @brief Get a mailbox channel and register callback handlers for it.
 *
 * A mailbox channel cannot be shared by several consumers.
 *
 * @rcv_cb: Receiver Callback API executed in an interrupt context or NULL.
 * @txc_cb: Transmitter Callback API executed in an interrupt context or NULL.
 * @cookie: Pointer to the consumer data input parameter for callback.
 * @desc: Pointer to the mailbox descriptor.
 * @chan_id: Channel ID number.
 * @chan: Pointer to mbox chan pointer handle.
 *
 * @retval: Return TEE_SUCCESS on success or error return code.
 *
 * If @rcv_cb is non zero, callback is executed in interrupt context. Callback
 * can trigger an event in bottom half, a function from bottom half should call
 * API mbox_recv() (non blocking API) to consume the incoming message.
 * Callback can call mbox_recv() (non blocking API to consume the message).
 * If @rcv_cb is zero, a thread can call mbox_recv() with block API parameter
 * to consume the incoming message.
 *
 * If @txc_cb is non NULL, callback is executed in interrupt context.
 * If @txc_cb is NULL, a thread can call API mbox_send() with block API
 * parameter to wait till message is consumed by remote.
 */
TEE_Result mbox_register_chan(mbox_callback_t rcv_cb,
			      mbox_callback_t txc_cb,
			      void *cookie,
			      const struct mbox_desc *desc,
			      unsigned int chan_id,
			      struct mbox_chan **chan);

/**
 * @brief Get a mailbox channel and register callback handlers for it,
 * the 1st channel node is selected for the mbox consumer.
 *
 * A mailbox channel cannot be shared by several consumers.
 *
 * @rcv_cb: Receiver Callback API executed in interrupt context.
 * @txc_cb: Transmitter Callback API executer in interrupt context.
 * @cookie: Pointer to the consumer data input parameter for callback.
 * @fdt: Pointer to the device tree blob.
 * @node: Device node in the device tree.
 * @chan: Pointer to mbox chan pointer handle.
 *
 * @retval: Return TEE_SUCCESS on success or error return code.
 *
 * If @rcv_cb is non zero, callback is executed in interrupt context. Callback
 * can trigger an event in bottom half, a function from bottom half should call
 * API mbox_recv() (non blocking API) to consume the incoming message.
 * Callback can call mbox_recv() (non blocking API to consume the message).
 * If @rcv_cb is zero, a thread can call mbox_recv() with block API parameter
 * to consume the incoming message.
 *
 * If @txc_cb is non zero, callback is executed in interrupt context.
 * If @txc_cb is Zero, a thread can call API mbox_send() with block API
 * parameter to wait till message is consumed by remote.
 */
TEE_Result mbox_dt_register_chan(mbox_callback_t rcv_cb,
				 mbox_callback_t txc_cb,
				 void *cookie,
				 void *fdt, int node,
				 struct mbox_chan **chan);

/**
 * @brief Get a mailbox channel and register callback handlers for it,
 * the name of channel is used to fix the channel node for the mbox consumer.
 *
 * A mailbox channel cannot be shared by several consumers.
 *
 * @rcv_cb: Receiver Callback API executed in irq context.
 * @txc_cb: Transmitter Callback API executed in irq context.
 * @cookie: Pointer to the consumer data input parameter for callback.
 * @fdt: Pointer to the device tree blob.
 * @node: Device node in the device tree.
 * @name: Pointer to channel name.
 * @chan: Pointer to mbox chan pointer handle.
 *
 * @retval: Return TEE_SUCCESS on success or appropriate error code.
 *
 * If @rcv_cb is non zero, callback is executed in irq context. Callback
 * can trigger an event in bottom half, a function from bottom half should call
 * API mbox_recv() (non blocking API) to consume the incoming message.
 * Callback can call mbox_recv() (non blocking API to consume the message).
 * If @rcv_cb is zero, a thread can call mbox_recv() with block API parameter
 * to consume the incoming message.
 *
 * If @txc_cb is non zero, callback is executed in irq context.
 * If @txc_cb is Zero, a thread can call API mbox_send() with block API
 * parameter to wait till message is consumed by remote.
 */
TEE_Result mbox_dt_register_chan_by_name(mbox_callback_t rcv_cb,
					 mbox_callback_t txc_cb,
					 void *cookie, void *fdt,
					 int node,
					 const char *name,
					 struct mbox_chan **chan);

/**
 * @brief Retrieve max data size supported by the device used by
 * a mbox consumer.
 *
 * @handle: Pointer to mbox channel handle.
 * @size: Pointer to max size of the data supported by the device.
 *
 * @retval: Return TEE_SUCCESS on success or appropriate error code.
 */
TEE_Result mbox_data_max_size(const struct mbox_chan *handle, size_t *size);

/**
 * @brief Send a message over the mbox device.
 *
 * A message is considered consumed once the remote interrupt handle
 * finishes.
 *
 * @handle: Pointer to mbox channel handle.
 * @wait: If true, busy-wait for remote to consume the message. The
 *	  message is considered consumed once the remote interrupt handle
 *	  finishes. For handle registered without transmit complete call back,
 *	  true returns TEE_ERROR_BAD_PARAMETERS.
 * @data: Pointer to the data sent in the message.
 * @size: Size of the data. The caller uses mbox_data_max_size() to know
 * the device max data size.
 *
 * @retval: TEE_ERROR_BUSY If the remote hasn't yet read the last data sent.
 * @retval: TEE_ERROR_EXCESS_DATA If size exceeds the max data size
 *	    supported by the device.
 * @retval: TEE_ERROR_BAD_PARAMETER If there was a bad parameter, such as:
 *	    too-large id value or the device isn't an outbound mbox channel.
 * @retval: TEE_SUCCESS On success.
 */
TEE_Result mbox_send(const struct mbox_chan *handle, bool wait,
		     const void *data, size_t size);

/**
 * @brief Receive a message over the mbox device.
 *
 * This API consummes the incoming message if any.
 * @handle: Pointer to mbox channel handle.
 * @wait: If true, wait till a message is received. For handle being
 * configured without receive call back.
 * For handle registered with received call back, true returns
 * TEE_ERROR_BAD_PARAMETERS.
 * @data: Pointer to the data area for retrieving message content.
 * @size: Size of data to be recopied.
 *
 * @retval: TEE_NO_DATA when no data available.
 * @retval: TEE_ERROR_BAD_PARAMETERS If error on parameters.
 * @retval: TEE_ERROR_EXCESS_DATA If size exceeds the max
 *	    data size supported by the device.
 * @retval: TEE_SUCCESS on success.
 */
TEE_Result mbox_recv(const struct mbox_chan *handle, bool wait,
		     void *data, size_t size);

/*
 * Driver Interface
 */

/**
 * struct mbox_dev - Opaque mailbox framework device .
 */
struct mbox_dev;

/**
 * @typedef mbox_rx_callback_t
 * @brief Mailbox device callback API for incoming mbox messages.
 *
 * These callbacks execute in interrupt context. Therefore, use only
 * interrupt-safe APIs. Registration of callbacks is done with
 * mbox_register_callback() API.
 *
 * @dev: Pointer to mailbox framework device info
 * @id: Channel identifier.
 * @data: Message data pointer.
 */
typedef void (*mbox_rx_callback_t)(const struct mbox_dev *dev,
				   unsigned int id,
				   void *data);

/**
 * @typedef mbox_tx_callback_t
 * @brief Mailbox device callback API for outgoing mbox messages completion.
 *
 * These callbacks execute in interrupt context. Therefore, use only
 * interrupt-safe APIs. Registration of callbacks is done with
 * mbox_register_callback() API.
 *
 * @dev: Pointer to mailbox framework device info.
 * @id: Channel identifier.
 */
typedef void (*mbox_tx_callback_t)(const struct mbox_dev *dev,
				   unsigned int id);

/*
 * struct mbox_ops - services functions provided by mailbox device.
 * This operation are called by mailbox framework to implement the service on
 * mailbox device.
 */
struct mbox_ops {
	/**
	 * @brief Callback API to send outgoing mbox messages.
	 *
	 * @desc: Pointer to mailbox descriptor.
	 * @id: Channel identifier.
	 * @data: Pointer to outgoing mbox message.
	 * @size: Outgoing mbox message size.
	 *
	 * @retval: TEE_ERROR_BAD_PARAMETERS for incorrect param.
	 * @retval: TEE_ERROR_BUSY if remote not ready for reception.
	 * @retval: TEE_SUCCESS if send is success.
	 */
	TEE_Result (*send)(const struct mbox_desc *desc,
			   unsigned int id,
			   const void *data, size_t size);

	/**
	 * @brief Callback API upon registration.
	 *
	 * @desc: Pointer to mailbox descriptor.
	 * @rcv_cb: Callback reporting channel reception.
	 * @txc_cb: Callback reporting channel transmission completion.
	 * @dev: Pointer to mailbox framework device to be used in callback
	 *	 parameter
	 *
	 * @retval: Return TEE_SUCCESS on success or error return code.
	 */
	TEE_Result (*register_callback)(const struct mbox_desc *desc,
					mbox_rx_callback_t rcv_cb,
					mbox_tx_callback_t txc_cb,
					struct mbox_dev *dev);
	/**
	 * @brief Callback API to get max data size.
	 *
	 * @dev: Pointer to mailbox descriptor.
	 *
	 * @retval: Max data size supported by mailbox device.
	 */
	size_t (*max_data_size)(const struct mbox_desc *desc);

	/**
	 * @brief Callback API to get the maximum number of channel.
	 *
	 * @desc: Pointer to mailbox descriptor.
	 *
	 * @retval: Maximum number of channel supported by device.
	 */
	size_t (*max_channel_count)(const struct mbox_desc *desc);

	/**
	 * @brief Callback API for enabling/disabling channel interrupt.
	 *
	 * @desc: Pointer to mailbox descriptor.
	 * @enable: If true enable channel interrupt else disable
	 *	    channel interrupt.
	 * @id: Channel identifier.
	 *
	 * @retval: Return TEE_SUCCESS on success or error return code.
	 */
	TEE_Result (*channel_interrupt)(const struct mbox_desc *desc,
					bool enable, unsigned int id);

	/**
	 * @brief Callback API for incoming mbox message command completion.
	 *
	 * @dev: Pointer to mailbox descriptor.
	 * @id: Channel identifier.
	 *
	 * @retval: Return TEE_SUCCESS on success or error return code.
	 */
	TEE_Result (*complete)(const struct mbox_desc *desc, unsigned int id);
};

/**
 * struct mbox_desc - mailbox descriptor provided by mailbox device.
 *
 * @ops: Pointer to device service ops.
 * @dev: Pointer to mailbox device private info.
 */
struct mbox_desc {
	const struct mbox_ops *ops;
	void *priv;
};

/**
 * @brief Register a mbox device in mailbox framework.
 *
 * @desc: Pointer to device mbox descriptor.
 *
 * @retval: Return TEE_SUCCESS on success or error return code.
 *
 * @desc reference if used by the mailbox framework, possibly from interrupt
 * execution contexts.
 */
TEE_Result mbox_register(const struct mbox_desc *desc);

/**
 * @brief Register a mbox device in mailbox framework.
 *
 * @fdt:  Pointer to the device tree blob.
 * @nodeoffset: Device node in the device tree.
 * @desc: Pointer to device mbox descriptor.
 *
 * @retval: Return TEE_SUCCESS on success or error return code.
 *
 * @desc reference if used by the mailbox framework, possibly from interrupt
 * execution contexts.
 */
TEE_Result mbox_dt_register(const void *fdt, int nodeoffset,
			    const struct mbox_desc *desc);
#endif /* __DRIVERS_MAILBOX_H */
