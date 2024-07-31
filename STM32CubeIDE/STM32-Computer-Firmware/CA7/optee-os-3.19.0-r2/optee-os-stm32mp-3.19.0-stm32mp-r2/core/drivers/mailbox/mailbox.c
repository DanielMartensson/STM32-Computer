// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024 STMicroelectronics - All Rights Reserved
 */

#include <assert.h>
#include <compiler.h>
#include <drivers/mailbox.h>
#include <initcall.h>
#include <keep.h>
#include <kernel/boot.h>
#include <kernel/delay.h>
#include <kernel/dt.h>
#include <kernel/notif.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <kernel/spinlock.h>
#include <libfdt.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

/*
 * MBOX_EVENT_RX Receive a message
 * MBOX_EVENT_TX Transmit a message
 */
enum mbox_event {
	MBOX_EVENT_RX,
	MBOX_EVENT_TX,
	MBOX_EVENT_NUM
};

/*
 * struct mbox_chan - Mailbox channel handle
 *
 * @id: Channel identifier
 * @notify: Boolean reflecting notif_id being allocated
 * @notify_id: Async notification ID used for the mailbox events or 0
 * @cb: Notification callback function
 * @cookie: Consumer data callback parameter
 * @mbox_dev: Mailbox device info
 * @lock: Lock for mailbox channel concurrent access
 * @used: Boolean for marking channel usage
 * @rx_count: Counter of rx event received from device
 * @rx_done: Counter of rx event provided to channel consumer
 * @data: Pointer to data information from device
 */
struct mbox_chan {
	unsigned int id;
	bool notify[MBOX_EVENT_NUM];
	uint32_t notify_id[MBOX_EVENT_NUM];
	mbox_callback_t cb[MBOX_EVENT_NUM];
	void *cookie;
	struct mbox_dev *mbox_dev;
	unsigned int lock;
	bool used;
	size_t rx_count;
	size_t rx_done;
	void *data;
};

/*
 * struct mbox_dev - Mailbox device descriptor
 *
 * @phandle: Device's handle
 * @num_chan: Number of @hdl array element
 * @desc: Mailbox descriptor
 * @link: Link to next mbox_dev
 * @hdl: Array for channel consumer handle
 */
struct mbox_dev {
	int phandle;
	size_t num_chan;
	const struct mbox_desc *desc;
	SLIST_ENTRY(mbox_dev) link;
	/* Flexible array size must be last */
	struct mbox_chan hdl[];
};

/* Device list variable */
static SLIST_HEAD(, mbox_dev) mailbox_device_list =
	SLIST_HEAD_INITIALIZER(mailbox_device_list);

/* Lock to prevent Device list concurrent access */
static unsigned int list_lock = SPINLOCK_UNLOCK;

static void mbox_process_cb(const struct mbox_dev *mbx_dev, unsigned int id,
			    bool notify, uint32_t notify_id, uint32_t evt)
{
	if (!mbx_dev->hdl[id].cb[evt] && notify) {
		/* Notify the context */
		notif_send_async(notify_id);
	} else if (mbx_dev->hdl[id].cb[evt]) {
		/* Callback consumer function with data */
		mbx_dev->hdl[id].cb[evt](mbx_dev->hdl[id].cookie);
	}
}

/* Notifier rx callback with consumer data */
static void mbox_rx_callback(const struct mbox_dev *mbx_dev,
			     unsigned int id, void *data)
{
	uint32_t exceptions = 0;
	bool notify = false;
	struct mbox_chan *handle = NULL;
	uint32_t notify_id = 0;

	if (id < mbx_dev->num_chan) {
		handle = (struct mbox_chan *)&mbx_dev->hdl[id];
		exceptions = cpu_spin_lock_xsave(&handle->lock);

		if (handle->used) {
			/* Increase reception count */
			handle->rx_count++;
			if  (((size_t)(handle->rx_done + 1))
			    != handle->rx_count
			    ) {
				EMSG("Received Event Not consumed");
				handle->rx_done++;
				assert(handle->rx_done == handle->rx_count);
			}
			/* Retrieve data */
			handle->data = data;
			notify = handle->notify[MBOX_EVENT_RX];
			notify_id = handle->notify_id[MBOX_EVENT_RX];
			cpu_spin_unlock_xrestore(&handle->lock,
						 exceptions);
			mbox_process_cb(mbx_dev, id, notify, notify_id,
					MBOX_EVENT_RX);
			return;
		}
		cpu_spin_unlock_xrestore(&handle->lock, exceptions);
	}
	EMSG("Unexpected interrupt");
}

/* Notifier tx callback with consumer data */
static void mbox_tx_callback(const struct mbox_dev *mbx_dev,
			     unsigned int id)
{
	uint32_t exceptions = 0;
	bool notify = false;
	uint32_t notify_id = 0;
	struct mbox_chan *handle = NULL;
	bool used = false;

	if (id >= mbx_dev->num_chan) {
		EMSG("Unexpected interrupt");
		return;
	}
	handle = (struct mbox_chan *)&mbx_dev->hdl[id];
	exceptions = cpu_spin_lock_xsave(&handle->lock);
	notify = handle->notify[MBOX_EVENT_TX];
	notify_id = handle->notify_id[MBOX_EVENT_TX];
	used = mbx_dev->hdl[id].used;
	cpu_spin_unlock_xrestore(&handle->lock,
				 exceptions);
	if (used)
		mbox_process_cb(mbx_dev, id, notify, notify_id, MBOX_EVENT_TX);
	else
		EMSG("Unused interrupt %u", id);
}

/* Driver interface */
static TEE_Result mbox_register_internal(const int phandle,
					 const struct mbox_desc *desc)
{
	struct mbox_dev *mdev = NULL;
	uint32_t exceptions = 0;
	size_t num_chan = desc->ops->max_channel_count(desc);
	size_t i = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	/* Allocate a mailbox channel desc */
	mdev = calloc(1, sizeof(*mdev) + num_chan * sizeof(*mdev->hdl));
	if (!mdev)
		return TEE_ERROR_OUT_OF_MEMORY;

	/* Populate mailbox channel desc */
	mdev->phandle = phandle;
	mdev->desc = desc;
	mdev->num_chan = num_chan;
	for (i = 0; i < num_chan; i++)
		mdev->hdl[i].lock = SPINLOCK_UNLOCK;

	/* Register incoming message mailbox Framework call bacK */
	res = desc->ops->register_callback(desc, mbox_rx_callback,
					   mbox_tx_callback, mdev);
	if (!res) {
		/* Insert device in mailbox device list */
		exceptions = cpu_spin_lock_xsave(&list_lock);
		SLIST_INSERT_HEAD(&mailbox_device_list, mdev, link);
		cpu_spin_unlock_xrestore(&list_lock, exceptions);
	} else {
		free(mdev);
	}
	return res;
}

TEE_Result mbox_register(const struct mbox_desc *desc)
{
	return mbox_register_internal(0, desc);
}

TEE_Result mbox_dt_register(const void *fdt, int nodeoffset,
			    const struct mbox_desc *desc)
{
	return mbox_register_internal(fdt_get_phandle(fdt, nodeoffset), desc);
}

static TEE_Result mbox_register_chan_internal(mbox_callback_t rcv_cb,
					      mbox_callback_t txc_cb,
					      void *cookie,
					      struct mbox_dev *mbx_dev,
					      unsigned int chan_id,
					      struct mbox_chan **chan)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	struct mbox_chan *hdl = NULL;
	uint32_t exceptions = 0;
	const struct mbox_ops *ops = NULL;

	/* Check channel usable */
	if (chan_id >= mbx_dev->num_chan)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Protect against potential asynchronous event */
	exceptions = cpu_spin_lock_xsave(&mbx_dev->hdl[chan_id].lock);
	if (mbx_dev->hdl[chan_id].used) {
		cpu_spin_unlock_xrestore(&mbx_dev->hdl[chan_id].lock,
					 exceptions);
		return TEE_ERROR_BAD_STATE;
	}

	/* Check callback against OPTEE configuration */
	if (!rcv_cb || !txc_cb) {
		if (!IS_ENABLED(CFG_CORE_ASYNC_NOTIF)) {
			/*
			 * When no callback, recv API is blocking,
			 * and sync notif support is required.
			 */
			cpu_spin_unlock_xrestore(&mbx_dev->hdl[chan_id].lock,
						 exceptions);
			return TEE_ERROR_NOT_SUPPORTED;
		}
	}

	/* Fill channel mailbox handle */
	hdl = &mbx_dev->hdl[chan_id];
	hdl->cb[MBOX_EVENT_RX] = rcv_cb;
	hdl->cb[MBOX_EVENT_TX] = txc_cb;

	hdl->cookie = cookie;
	hdl->mbox_dev = mbx_dev;
	hdl->id = chan_id;
	if (!rcv_cb) {
		res = notif_alloc_async_value(&hdl->notify_id[MBOX_EVENT_RX]);
		if (res) {
			cpu_spin_unlock_xrestore(&mbx_dev->hdl[chan_id].lock,
						 exceptions);
			return res;
		}
	}
	if (!txc_cb) {
		res = notif_alloc_async_value(&hdl->notify_id[MBOX_EVENT_TX]);
		if (res) {
			cpu_spin_unlock_xrestore(&mbx_dev->hdl[chan_id].lock,
						 exceptions);
			return res;
		}
	}
	hdl->used = true;
	cpu_spin_unlock_xrestore(&mbx_dev->hdl[chan_id].lock, exceptions);

	/* Enable mailbox channel */
	ops = mbx_dev->desc->ops;
	res = ops->channel_interrupt(mbx_dev->desc, true, chan_id);
	if (res) {
		exceptions = cpu_spin_lock_xsave(&mbx_dev->hdl[chan_id].lock);
		hdl->used = false;
		cpu_spin_unlock_xrestore(&mbx_dev->hdl[chan_id].lock,
					 exceptions);
		return res;
	}
	*chan = hdl;

	return TEE_SUCCESS;
}

/*  Framework API */
TEE_Result mbox_dt_register_chan_by_name(mbox_callback_t rcv_cb,
					 mbox_callback_t txc_cb,
					 void *cookie, void *fdt,
					 int node, const char *name,
					 struct mbox_chan **chan)
{
	int idx = 0;
	int len = 0;
	const uint32_t *prop = NULL;
	int phandle = 0;
	struct mbox_dev *mdev = NULL;
	struct mbox_dev *mbx_dev = NULL;
	uint32_t exceptions = 0;
	unsigned int chan_id = 0;

	if (!fdt || !name || !chan)
		return TEE_ERROR_BAD_PARAMETERS;

	idx = fdt_stringlist_search(fdt, node, "mbox-names", name);
	if (idx < 0) {
		EMSG("dt field mbox-name not found");
		return TEE_ERROR_GENERIC;
	}

	prop = fdt_getprop(fdt, node, "mboxes", &len);
	if (!prop) {
		EMSG("dt field mboxes not found FDT_ERR :%d", -len);
		return TEE_ERROR_GENERIC;
	}

	/* Property is phandle and chan number */
	len /= 2 * sizeof(uint32_t);
	if (idx >= len)
		return  TEE_ERROR_BAD_PARAMETERS;

	phandle = fdt32_to_cpu(prop[2 * idx]);
	chan_id = (unsigned int)fdt32_to_cpu(prop[2 * idx + 1]);

	/* Search phandle matching a registered mailbox device */
	exceptions = cpu_spin_lock_xsave(&list_lock);
	SLIST_FOREACH(mdev, &mailbox_device_list, link)
		if (phandle == mdev->phandle)
			mbx_dev = mdev;
	cpu_spin_unlock_xrestore(&list_lock, exceptions);
	if (!mbx_dev)
		return TEE_ERROR_DEFER_DRIVER_INIT;

	return mbox_register_chan_internal(rcv_cb, txc_cb, cookie, mbx_dev,
					   chan_id,
					   chan);
}

TEE_Result mbox_dt_register_chan(mbox_callback_t rcv_cb,
				 mbox_callback_t txc_cb, void *cookie,
				 void *fdt, int node,
				 struct mbox_chan **chan)
{
	int len = 0;
	const fdt32_t *cuint = NULL;
	int phandle = 0;
	uint32_t chan_id = 0;
	struct mbox_dev *mbx_dev = NULL;
	uint32_t exceptions = 0;
	struct mbox_dev *mdev = NULL;

	if (!fdt || !chan)
		return TEE_ERROR_BAD_PARAMETERS;
	cuint = fdt_getprop(fdt, node, "mboxes", &len);
	if (!cuint) {
		EMSG("dt field mboxes not found FDT_ERR :%d", -len);
		return TEE_ERROR_GENERIC;
	}
	phandle = fdt32_to_cpu(*cuint);
	chan_id = (uint32_t)fdt32_to_cpu(*(cuint + 1));

	/* Search phandle matching a registered mailbox device */
	exceptions = cpu_spin_lock_xsave(&list_lock);
	SLIST_FOREACH(mdev, &mailbox_device_list, link)
		if (phandle == mdev->phandle)
			mbx_dev = mdev;
	cpu_spin_unlock_xrestore(&list_lock, exceptions);
	if (!mbx_dev)
		return TEE_ERROR_DEFER_DRIVER_INIT;

	return mbox_register_chan_internal(rcv_cb, txc_cb, cookie, mbx_dev,
					   chan_id,
					   chan);
}

TEE_Result mbox_register_chan(mbox_callback_t rcv_cb,
			      mbox_callback_t txc_cb, void *cookie,
			      const struct mbox_desc *desc,
			      unsigned int chan_id,
			      struct mbox_chan **chan)
{
	struct mbox_dev *mbx_dev = NULL;
	struct mbox_dev *mdev = NULL;
	uint32_t exceptions = 0;

	if (!chan || !desc)
		return TEE_ERROR_BAD_PARAMETERS;
	/* Search mbox_desc matching a registered mailbox device */
	exceptions = cpu_spin_lock_xsave(&list_lock);
	SLIST_FOREACH(mdev, &mailbox_device_list, link)
		if (desc == mdev->desc)
			mbx_dev = mdev;
	cpu_spin_unlock_xrestore(&list_lock, exceptions);
	if (!mbx_dev)
		return TEE_ERROR_DEFER_DRIVER_INIT;

	return mbox_register_chan_internal(rcv_cb, txc_cb, cookie, mbx_dev,
					   chan_id,
					   chan);
}

TEE_Result mbox_data_max_size(const struct mbox_chan *handle, size_t *size)
{
	const struct mbox_ops *ops = handle->mbox_dev->desc->ops;

	*size = ops->max_data_size(handle->mbox_dev->desc);

	return TEE_SUCCESS;
}

TEE_Result mbox_send(const struct mbox_chan *handle, bool wait,
		     const void *data, size_t size)
{
	struct mbox_chan *hdl = (struct mbox_chan *)handle;
	const struct mbox_ops *ops = hdl->mbox_dev->desc->ops;
	uint32_t except = 0;
	TEE_Result res = TEE_ERROR_GENERIC;

	/* Check parameter */
	if (ops->max_data_size(hdl->mbox_dev->desc) < size)
		return TEE_ERROR_EXCESS_DATA;
	if (wait && hdl->cb[MBOX_EVENT_TX])
		return TEE_ERROR_BAD_PARAMETERS;

	except = cpu_spin_lock_xsave(&hdl->lock);
	if (hdl->notify[MBOX_EVENT_TX]) {
		cpu_spin_unlock_xrestore(&hdl->lock, except);
		EMSG("Unexpected simultaneous send on a channel");
		return TEE_ERROR_ACCESS_CONFLICT;
	}
	if (wait)
		hdl->notify[MBOX_EVENT_TX] = true;

	cpu_spin_unlock_xrestore(&hdl->lock, except);
	res = ops->send(hdl->mbox_dev->desc, hdl->id, data, size);
	if (wait) {
		if (!res)
			notif_wait(hdl->notify_id[MBOX_EVENT_TX]);
		except = cpu_spin_lock_xsave(&hdl->lock);
		hdl->notify[MBOX_EVENT_TX] = false;
		cpu_spin_unlock_xrestore(&hdl->lock, except);
	}

	return res;
}

TEE_Result mbox_recv(const struct mbox_chan *handle, bool wait,
		     void *data, size_t size)
{
	struct mbox_chan *hdl = (struct mbox_chan *)handle;
	const struct mbox_ops *ops = hdl->mbox_dev->desc->ops;
	uint32_t except = 0;
	void *copy_data = NULL;
	vaddr_t s = 0;

	if (wait && hdl->cb[MBOX_EVENT_RX])
		return TEE_ERROR_BAD_PARAMETERS;

	if (ops->max_data_size(hdl->mbox_dev->desc) < size)
		return TEE_ERROR_EXCESS_DATA;

	if ((size && !data) ||
	    (data && size && ADD_OVERFLOW((vaddr_t)data, size, &s)))
		return TEE_ERROR_BAD_PARAMETERS;

	except = cpu_spin_lock_xsave(&hdl->lock);
	if (hdl->notify[MBOX_EVENT_RX]) {
		cpu_spin_unlock_xrestore(&hdl->lock, except);
		return TEE_ERROR_ACCESS_CONFLICT;
	}

	if (hdl->rx_count == hdl->rx_done) {
		if (!wait) {
			cpu_spin_unlock_xrestore(&hdl->lock, except);
			return TEE_ERROR_NO_DATA;
		}
		hdl->notify[MBOX_EVENT_RX] = true;
		cpu_spin_unlock_xrestore(&hdl->lock, except);
		notif_wait(hdl->notify_id[MBOX_EVENT_RX]);
		except = cpu_spin_lock_xsave(&hdl->lock);
		hdl->notify[MBOX_EVENT_RX] = false;
	}

	if (size && !hdl->data) {
		cpu_spin_unlock_xrestore(&hdl->lock, except);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	copy_data = hdl->data;
	hdl->data = NULL;
	hdl->rx_done++;
	cpu_spin_unlock_xrestore(&hdl->lock, except);
	if (size)
		memcpy(data, copy_data, size);
	/* Acknowledge receive interrupt */
	ops->complete(hdl->mbox_dev->desc, hdl->id);

	return TEE_SUCCESS;
}
