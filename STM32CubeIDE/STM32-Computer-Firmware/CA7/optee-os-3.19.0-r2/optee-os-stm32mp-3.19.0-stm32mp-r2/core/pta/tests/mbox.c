// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, STMicroelectronics International N.V.
 */
#include <assert.h>
#include <atomic.h>
#include <config.h>
#include <drivers/mailbox.h>
#include <kernel/boot.h>
#include <kernel/dt.h>
#include <kernel/notif.h>
#include <kernel/panic.h>
#include <kernel/spinlock.h>
#include <kernel/tee_time.h>
#include <libfdt.h>
#include <pta_invoke_tests.h>
#include <stdbool.h>
#include <trace.h>
#include <util.h>

#include "mbox.h"

/*
 * Enable expect LOG macro to enable/disable self tests traces.
 *
 * #define LOG     DMSG_RAW
 * #define LOG(...)
 */
#define LOG(...) trace_printf_helper(0, false, __VA_ARGS__)

#define VIRT_CHAN_NUM 2
#define VIRT_CHAN_SIZE 8

struct virt_mbx_data {
	/* Lock to protect concurrent access */
	unsigned int lock[VIRT_CHAN_NUM];
	mbox_rx_callback_t rx_cb;
	mbox_tx_callback_t tx_cb;
	struct mbox_dev *cb_ctx;
	bool enabled[VIRT_CHAN_NUM];
	/*
	 * Array tx_full and rx_full are used as boolean, since they require
	 * atomic access, they are defined as int to be able to use
	 * atomic_load_int , atomic_store_int
	 */
	int tx_full[VIRT_CHAN_NUM];
	int rx_full[VIRT_CHAN_NUM];
	uint8_t tx_data[VIRT_CHAN_NUM][VIRT_CHAN_SIZE];
	uint8_t rx_data[VIRT_CHAN_NUM][VIRT_CHAN_SIZE];
	/* Handle for test  */
	struct mbox_chan *handle[VIRT_CHAN_NUM];
	/* Copro thread status */
	bool thread[VIRT_CHAN_NUM];
};

static TEE_Result thread_test_recv(struct mbox_desc *desc, unsigned int id,
				   bool reply, int count)
{
	uint32_t except = 0;
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;

	if (id >= VIRT_CHAN_NUM || !data->enabled[id] ||
	    (count > 1 && !reply))
		return TEE_ERROR_BAD_PARAMETERS;

	except = cpu_spin_lock_xsave(&data->lock[id]);
	if (data->thread[id]) {
		cpu_spin_unlock_xrestore(&data->lock[id], except);
		return TEE_ERROR_BUSY;
	}
	data->thread[id] = true;
	cpu_spin_unlock_xrestore(&data->lock[id], except);

	/* Wait till a message is sent */
	LOG("th_recv : Start");
	while (count > 0) {
		LOG("th_recv : wait fifo tx full");
		while (!atomic_load_int(&data->tx_full[id]))
			tee_time_wait(0);
		except = cpu_spin_lock_xsave(&data->lock[id]);
		if (reply) {
			/* Ping pong, recopy received data */
			LOG("th_recv : check fifo rx empty");
			if (atomic_load_int(&data->rx_full[id])) {
				DMSG("rx channel = %d is already full", id);
				return TEE_ERROR_BUSY;
			}
			memcpy(data->rx_data[id], data->tx_data[id],
			       sizeof(data->rx_data[0]));
			atomic_store_int(&data->rx_full[id], 1);
		}
		/* Clean tx data */
		memset(data->tx_data[id], 0xca, sizeof(data->tx_data[id]));
		atomic_store_int(&data->tx_full[id], 0);
		cpu_spin_unlock_xrestore(&data->lock[id], except);

		/* Notify transmission done */
		LOG("th_recv : notify tx done");
		if (data->tx_cb) {
			LOG("th_recv : notify tx done");
			data->tx_cb(data->cb_ctx, id);
		}

		if (data->rx_cb && reply) {
			LOG("th_recv : notify rx ");
			data->rx_cb(data->cb_ctx, id, data->rx_data[id]);
		}
		count--;
	}
	LOG("th_recv : Done");
	except = cpu_spin_lock_xsave(&data->lock[id]);
	data->thread[id] = false;
	cpu_spin_unlock_xrestore(&data->lock[id], except);

	return TEE_SUCCESS;
}

static TEE_Result thread_test_send(struct mbox_desc *desc, unsigned int id,
				   const void *buff, bool wait, int count)
{
	TEE_Result res = TEE_SUCCESS;
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;
	uint32_t except = 0;

	if (id >= VIRT_CHAN_NUM || !data->enabled[id] ||
	    (count > 1 && !wait))
		return TEE_ERROR_BAD_PARAMETERS;
	except = cpu_spin_lock_xsave(&data->lock[id]);
	if (data->thread[id]) {
		cpu_spin_unlock_xrestore(&data->lock[id], except);
		return TEE_ERROR_BUSY;
	}
	data->thread[id] = true;
	cpu_spin_unlock_xrestore(&data->lock[id], except);
	/* Wait till message can be queued */
	LOG("th_send : Start");
	while (count > 0) {
		/* Wait till message can be queued */
		LOG("th_send : wait fifo rx empty");
		while (atomic_load_int(&data->rx_full[id]))
			tee_time_wait(0);
		if (atomic_load_int(&data->tx_full[id]) && wait) {
			DMSG("tx channel = %d is already full", id);
			return TEE_ERROR_BUSY;
		}

		except = cpu_spin_lock_xsave(&data->lock[id]);
		memcpy(data->rx_data[id], data->tx_data[id],
		       sizeof(data->rx_data[0]));
		atomic_store_int(&data->rx_full[id], 1);
		memcpy(data->rx_data[id], buff, sizeof(data->rx_data[0]));
		cpu_spin_unlock_xrestore(&data->lock[id], except);
		if (data->rx_cb) {
			LOG("th_send : notify rx ");
			data->rx_cb(data->cb_ctx, id, data->rx_data[id]);
		}

		if (!wait)
			return TEE_SUCCESS;
		LOG("th_send : wait fifo tx full");
		while (!atomic_load_int(&data->tx_full[id]))
			tee_time_wait(0);
		except = cpu_spin_lock_xsave(&data->lock[id]);
		if (memcmp(data->tx_data[id], buff, sizeof(data->tx_data[0]))) {
			LOG("th_send :Receive message check : KO");
			res = TEE_ERROR_GENERIC;
		} else {
			LOG("th_send :Receive message check : OK");
		}
		atomic_store_int(&data->tx_full[id],  0);
		cpu_spin_unlock_xrestore(&data->lock[id], except);
		/* Notify transmission done */
		if (data->tx_cb) {
			LOG("th_send : notify tx ");
			data->tx_cb(data->cb_ctx, id);
		}
		count--;
	}
	LOG("th_send : Done");
	except = cpu_spin_lock_xsave(&data->lock[id]);
	data->thread[id] = false;
	cpu_spin_unlock_xrestore(&data->lock[id], except);

	return res;
}

static TEE_Result virt_mbox_send(const struct mbox_desc *desc,
				 unsigned int id,
				 const void *buff, size_t len)
{
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;
	uint32_t except = 0;

	/* No data transmission, only doorbell */
	if (len > VIRT_CHAN_SIZE)
		return TEE_ERROR_NOT_SUPPORTED;

	if (id >= VIRT_CHAN_NUM || !(data->enabled[id])) {
		EMSG("invalid id (%d)", id);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	DMSG("Send msg on channel %d", id);

	/* Check that the channel is free (otherwise wait) */
	LOG("send :check tx empty");
	if (atomic_load_int(&data->tx_full[id])) {
		LOG("send: tx full");
		return TEE_ERROR_BUSY;
	}

	except = cpu_spin_lock_xsave(&data->lock[id]);
	memcpy(data->tx_data[id], buff, len);
	atomic_store_int(&data->tx_full[id], 1);
	cpu_spin_unlock_xrestore(&data->lock[id], except);
	LOG("send: done");

	return TEE_SUCCESS;
}

static size_t virt_mbox_get_mtu(const struct mbox_desc *desc __unused)
{
	return (size_t)VIRT_CHAN_SIZE;
}

static size_t virt_mbox_channel_count(const struct mbox_desc *desc __unused)
{
	return VIRT_CHAN_NUM;
}

static TEE_Result virt_mbox_register_callback(const struct mbox_desc *desc,
					      mbox_rx_callback_t rx_cb,
					      mbox_tx_callback_t tx_cb,
					      struct mbox_dev *cb_ctx)
{
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;

	if (!data->rx_cb && !data->tx_cb && !data->cb_ctx) {
		data->rx_cb = rx_cb;
		data->tx_cb = tx_cb;
		data->cb_ctx = cb_ctx;
		return TEE_SUCCESS;
	} else {
		return TEE_ERROR_BAD_STATE;
	}
}

static TEE_Result virt_mbox_enable(const struct mbox_desc *desc,
				   bool enable, unsigned int id)
{
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;
	uint32_t except = 0;
	TEE_Result res = TEE_SUCCESS;

	if (id >= VIRT_CHAN_NUM)
		return TEE_ERROR_BAD_PARAMETERS;
	except = cpu_spin_lock_xsave(&data->lock[id]);
	if (data->enabled[id] == (uint32_t)enable)
		res = TEE_ERROR_BAD_STATE;
	data->enabled[id] = (uint32_t)true;
	cpu_spin_unlock_xrestore(&data->lock[id], except);

	return res;
}

static TEE_Result virt_mbox_complete(const struct mbox_desc *desc,
				     unsigned int id)
{
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;
	uint32_t except = 0;
	TEE_Result res = TEE_SUCCESS;

	if (id >= VIRT_CHAN_NUM)
		return TEE_ERROR_BAD_PARAMETERS;
	except = cpu_spin_lock_xsave(&data->lock[id]);
	if (!atomic_load_int(&data->rx_full[id])) {
		LOG("complete : on rx empty ");
		res = TEE_ERROR_BAD_STATE;
	}
	LOG("complete : set rx empty ");
	atomic_store_int(&data->rx_full[id], 0);
	/*  clean data  */
	memset(data->rx_data[id], 0xff, sizeof(data->rx_data[id]));
	cpu_spin_unlock_xrestore(&data->lock[id], except);

	return res;
}

static const struct mbox_ops virt_mbox_ops = {
	.send = virt_mbox_send,
	.register_callback = virt_mbox_register_callback,
	.max_data_size = virt_mbox_get_mtu,
	.max_channel_count = virt_mbox_channel_count,
	.channel_interrupt = virt_mbox_enable,
	.complete = virt_mbox_complete,
};

static TEE_Result virt_mbox_init(struct mbox_desc **desc)
{
	TEE_Result res;
	/* Local structure provided to mailbox framework for content usage */
	struct virt_mbx_data *data = NULL;

	/*  allocate virt_mbx_data, init value is all 0 */
	data = calloc(1, sizeof(*data));
	if (!data)
		return TEE_ERROR_OUT_OF_MEMORY;

	/*  allocate desc  */
	*desc = calloc(1, sizeof(*desc));
	if (!desc) {
		free(data);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	(*desc)->ops = &virt_mbox_ops;
	(*desc)->priv = (void *)data;

	/* Register device to mailbox framework */
	res =  mbox_register(*desc);
	if (res) {
		free(*desc);
		free(data);
	}

	return res;
}

static TEE_Result test_recv_send(struct mbox_desc  *desc, unsigned int id,
				 int count)
{
	TEE_Result res = TEE_SUCCESS;
	uint8_t buffer[VIRT_CHAN_SIZE + 2] = {0};
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;
	struct mbox_chan *handle = data->handle[id];

	if (!handle) {
		/*  register handle without callback*/
		res = mbox_register_chan(NULL, NULL, NULL, desc, id,
					 &handle);
		if (res != TEE_SUCCESS) {
			LOG("Receive/send :failed to register channel");
			return res;
		}
		data->handle[id] = handle;
	}

	LOG("Receive/send : mailbox blocking api test: start");
	while (count > 0) {
		res = mbox_recv(handle, true, buffer, VIRT_CHAN_SIZE);

		if (res != TEE_SUCCESS) {
			LOG("Receive/send : failed to receive message");
			return res;
		}
		LOG("Receive/send : Receive message: OK");
		res = mbox_send(handle, true, buffer, VIRT_CHAN_SIZE);
		if (res != TEE_SUCCESS) {
			LOG("Receive/send : failed to send message");
			return res;
		}
		LOG("Receive/send : Send message: OK");
		count--;
	}

	return TEE_SUCCESS;
}

static TEE_Result test_send_rcv(struct mbox_desc *desc, unsigned int id,
				const uint8_t *info, int count)
{
	TEE_Result res = TEE_SUCCESS;
	uint8_t buffer[VIRT_CHAN_SIZE + 2] = {0};
	struct virt_mbx_data *data = (struct virt_mbx_data *)desc->priv;
	struct mbox_chan *handle = data->handle[id];

	if (!handle) {
		/*  register handle without callback*/
		res = mbox_register_chan(NULL, NULL, NULL, desc, id,
					 &handle);
		if (res != TEE_SUCCESS) {
			LOG("failed to register channel");
			return res;
		}
		data->handle[id] = handle;
	}

	LOG("Send/receive : mailbox blocking api test: start");
	while (count > 0) {
		res = mbox_send(handle, true, info, VIRT_CHAN_SIZE);
		if (res != TEE_SUCCESS) {
			LOG("Send/receive : failed to send message");
			return res;
		}
		LOG("Send/receive : Send message: OK");
		res = mbox_recv(handle, true, buffer, VIRT_CHAN_SIZE);

		if (res != TEE_SUCCESS) {
			LOG("Send/receive : failed to receive message");
			return res;
		}
		LOG("Send/receive :Receive message: OK");
		if (memcmp(info, buffer, sizeof(buffer) - 2)) {
			LOG("Send/receive: Receive message check KO");
			return TEE_ERROR_GENERIC;
		}
		LOG("Send/receive :Receive message check OK");
		count--;
	}

	return TEE_SUCCESS;
}

#define MBOX_CLIENT_COMPAT	"st,test-mbox-cli"

static struct mbox_chan *handle_receive_send;
static struct mbox_chan *handle_send_receive;

static TEE_Result test_ipcc_send_receive(int count)
{
	TEE_Result res = TEE_SUCCESS;

	LOG("Send/receive IPCC : start");
	if (!handle_send_receive) {
		LOG("Send/receive IPCC : handle invalid");
		return TEE_ERROR_BAD_STATE;
	}

	while (count > 0) {
		res = mbox_send(handle_send_receive, false, NULL, 0);
		if (res != TEE_SUCCESS) {
			LOG("Send/receive IPCC : send failed");
			return res;
		}
		LOG("Send/receive IPCC : send ok");
		res = mbox_recv(handle_send_receive, true, NULL, 0);
		if (res != TEE_SUCCESS)
			LOG("Send/receive IPCC : received failed");
		LOG("Send/receive IPCC : recv ok");
		count--;
	}
	LOG("Send/receive IPCC : Done");

	return TEE_SUCCESS;
}

static void receive_callback(const void *cookie __unused)
{
	if (!IS_ENABLED(CFG_CORE_ASYNC_NOTIF)) {
		TEE_Result res = mbox_recv(handle_receive_send, 0, NULL, 0);

		if (res != TEE_SUCCESS)
			panic();
		/*  send response*/
		res = mbox_send(handle_receive_send, 0, NULL, 0);
		if (res != TEE_SUCCESS)
			panic();
	} else {
		if (notif_async_is_started())
			notif_send_async(NOTIF_VALUE_DO_BOTTOM_HALF);
		else
			panic("no bottom half");
	}
}

static void yielding_mbox_notif(struct notif_driver *ndrv __unused,
				enum notif_event ev)
{
	TEE_Result res = TEE_SUCCESS;

	switch (ev) {
	case NOTIF_EVENT_DO_BOTTOM_HALF:
		MSG("Asynchronous notifications ongoing");

		res = mbox_recv(handle_receive_send, false, NULL, 0);
		if (res == TEE_ERROR_NO_DATA) {
			MSG("Asynchronous notificationsi: No Data");
			break;
		}
		if (res != TEE_SUCCESS)
			panic();
		/*  send response*/
		res = mbox_send(handle_receive_send, false, NULL, 0);
		if (res != TEE_SUCCESS)
			panic();
		MSG("Ack Done");

		break;
	case NOTIF_EVENT_STOPPED:
		MSG("Asynchronous notifications stopped");
		break;
	default:
		MSG("Unknown event %d", (int)ev);
	}
}

static void atomic_mbox_notif(struct notif_driver *ndrv __unused,
			      enum notif_event ev __maybe_unused)
{
	MSG("Asynchronous notifications started, event %d", (int)ev);
}

struct notif_driver mbox_notif = {
	.atomic_cb = atomic_mbox_notif,
	.yielding_cb = yielding_mbox_notif,
};

static TEE_Result ipcc_mailbox_test(void)
{
	TEE_Result res = TEE_SUCCESS;
	void *fdt = NULL;
	int node = 0;

	fdt = get_embedded_dt();
	node = fdt_node_offset_by_compatible(fdt, 0, MBOX_CLIENT_COMPAT);

	if (node < 0)
		/*  Test not activated in this dt configuration */
		return TEE_SUCCESS;
	MSG("TEST CLIENT MBOX");

	if (_fdt_get_status(fdt, node) == DT_STATUS_DISABLED)
		return TEE_SUCCESS;
	res = mbox_dt_register_chan(receive_callback, NULL, NULL, fdt, node,
				    &handle_receive_send);
	if (res != TEE_SUCCESS)
		MSG("chan handle_receive_send %d", res);

	notif_register_driver(&mbox_notif);

	res = mbox_dt_register_chan_by_name(NULL, NULL, NULL, fdt, node,
					    "test_client_m0",
					    &handle_send_receive);
	if (res != TEE_SUCCESS)
		MSG("chan handle_send_receive %d", res);

	return res;
}

driver_init_late(ipcc_mailbox_test);

struct mbox_desc *virt_mbox_desc;
#define CHAN_TX 0 /*  tx initiated by target */
#define CHAN_RX 1 /*  tx initited by copro   */
static const uint8_t tx_data[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,
	0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
static const uint8_t rx_data[] = {0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8, 0x7,
	0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0};

/* Exported entrypoint for mbox tests */
TEE_Result core_mbox_tests(uint32_t ptypes,
			   TEE_Param params[TEE_NUM_PARAMS])
{
	uint32_t test = 0;
	uint32_t count = 0;

	if (ptypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE,
				      TEE_PARAM_TYPE_NONE)) {
		DMSG("bad parameters types: 0x%" PRIx32, ptypes);
		return TEE_ERROR_BAD_PARAMETERS;
	}
	test = params[0].value.a;

	switch (test) {
	case PTA_MBOX_TEST_COPRO_INIT:
		if (virt_mbox_desc)
			return TEE_SUCCESS;
		return virt_mbox_init(&virt_mbox_desc);
	case PTA_MBOX_TEST_COPRO_SEND_WAIT:
		/*  Launch a thread sending and listening response  */
		count = params[0].value.b;
		if (!virt_mbox_desc)
			return TEE_ERROR_BAD_STATE;
		return thread_test_send(virt_mbox_desc, CHAN_RX, rx_data, true,
					count);
	case PTA_MBOX_TEST_COPRO_WAIT_SEND:
		/*  Launch a thread listening and response  */
		count = params[0].value.b;
		if (!virt_mbox_desc)
			return TEE_ERROR_BAD_STATE;
		return thread_test_recv(virt_mbox_desc, CHAN_TX, true,
					count);

	case PTA_MBOX_TEST_SEND_RECEIVE:
		count = params[0].value.b;
		if (!virt_mbox_desc)
			return TEE_ERROR_BAD_STATE;
		return test_send_rcv(virt_mbox_desc, CHAN_TX, tx_data, count);

	case  PTA_MBOX_TEST_RECEIVE_SEND:
		count = params[0].value.b;
		if (!virt_mbox_desc)
			return TEE_ERROR_BAD_STATE;
		return test_recv_send(virt_mbox_desc, CHAN_RX, count);

	case PTA_MBOX_TEST_IPCC_SEND_RECEIVE:
		count = params[0].value.b;
		return test_ipcc_send_receive(count);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}

	return TEE_ERROR_BAD_PARAMETERS;
}
