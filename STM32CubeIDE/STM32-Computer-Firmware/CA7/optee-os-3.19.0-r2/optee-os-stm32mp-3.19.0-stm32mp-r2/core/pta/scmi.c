// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2019-2021, Linaro Limited
 * Copyright (c) 2019-2021, STMicroelectronics
 */
#include <compiler.h>
#include <config.h>
#include <drivers/scmi-msg.h>
#include <scmi/scmi_server.h>
#include <kernel/pseudo_ta.h>
#include <kernel/thread_private_arch.h>
#include <optee_rpc_cmd.h>
#include <pta_scmi_client.h>
#include <stdint.h>
#include <string.h>
#include <util.h>

struct msg_shm {
	void *in_buf;
	size_t in_size;
	void *out_buf;
	size_t out_size;
	size_t out_max_size;
};

static bool valid_caps(unsigned int caps)
{
	return (caps & ~PTA_SCMI_CAPS_MASK) == 0;
}

static uint32_t supported_caps(void)
{
	uint32_t caps = 0;

	if (IS_ENABLED2(_CFG_SCMI_PTA_SMT_HEADER))
		caps |= PTA_SCMI_CAPS_SMT_HEADER | PTA_SCMI_CAPS_OCALL2_THREAD;

	if (IS_ENABLED2(_CFG_SCMI_PTA_MSG_HEADER))
		caps |= PTA_SCMI_CAPS_MSG_HEADER | PTA_SCMI_CAPS_OCALL2_THREAD;

	return caps;
}

static TEE_Result cmd_capabilities(uint32_t ptypes,
				   TEE_Param param[TEE_NUM_PARAMS])
{
	const uint32_t exp_ptypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE);

	if (ptypes != exp_ptypes)
		return TEE_ERROR_BAD_PARAMETERS;

	param[0].value.a = supported_caps();
	param[0].value.b = 0;

	return TEE_SUCCESS;
}

static TEE_Result cmd_process_smt_channel(uint32_t ptypes,
					  TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_ptypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE);
	unsigned int channel_id = params[0].value.a;

	if (ptypes != exp_ptypes)
		return TEE_ERROR_BAD_PARAMETERS;

	if (IS_ENABLED(CFG_SCMI_MSG_SMT)) {
		struct scmi_msg_channel *channel = NULL;

		channel = plat_scmi_get_channel(channel_id);
		if (!channel)
			return TEE_ERROR_BAD_PARAMETERS;

		scmi_smt_threaded_entry(channel_id);

		return TEE_SUCCESS;
	}

	if (IS_ENABLED(CFG_SCMI_SCPFW))
		return scmi_server_smt_process_thread(channel_id);

	return TEE_ERROR_NOT_SUPPORTED;
}

static TEE_Result cmd_process_smt_message(uint32_t ptypes,
					  TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_ptypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						    TEE_PARAM_TYPE_MEMREF_INOUT,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE);
	unsigned int channel_id = params[0].value.a;
	TEE_Param *param1 = params + 1;

	if (ptypes != exp_ptypes)
		return TEE_ERROR_BAD_PARAMETERS;

	if (IS_ENABLED(CFG_SCMI_MSG_SMT)) {
		struct scmi_msg_channel *channel = NULL;

		if (param1->memref.size < SMT_BUF_SLOT_SIZE)
			return TEE_ERROR_BAD_PARAMETERS;

		channel = plat_scmi_get_channel(channel_id);
		if (!channel)
			return TEE_ERROR_BAD_PARAMETERS;

		/*
		 * Caller provides the buffer, we bind channel to that buffer.
		 * Once message is processed, unbind the buffer since it is
		 * valid only for the current invocation.
		 */
		scmi_smt_set_shared_buffer(channel, param1->memref.buffer);
		scmi_smt_threaded_entry(channel_id);
		scmi_smt_set_shared_buffer(channel, NULL);

		return TEE_SUCCESS;
	}

	return TEE_ERROR_NOT_SUPPORTED;
}

static TEE_Result cmd_process_msg_channel(uint32_t ptypes,
					  TEE_Param params[TEE_NUM_PARAMS])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_MEMREF_INPUT,
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE);
	unsigned int channel_id = params[0].value.a;
	void *in_buf = params[1].memref.buffer;
	size_t in_size = params[1].memref.size;
	void *out_buf = params[2].memref.buffer;
	size_t out_size = params[2].memref.size;

	if (ptypes != exp_pt || !in_buf || !out_buf)
		return TEE_ERROR_BAD_PARAMETERS;

	if (IS_ENABLED(CFG_SCMI_MSG_DRIVERS)) {
		struct scmi_msg_channel *channel = NULL;

		if (!IS_ENABLED(CFG_SCMI_MSG_SHM_MSG))
			return TEE_ERROR_NOT_SUPPORTED;

		channel = plat_scmi_get_channel(channel_id);
		if (!channel)
			return TEE_ERROR_BAD_PARAMETERS;

		res = scmi_msg_threaded_entry(channel_id, in_buf, in_size,
					      out_buf, &out_size);
		if (!res)
			params[2].memref.size = out_size;

		return res;
	}

	if (IS_ENABLED(CFG_SCMI_SCPFW)) {
		if (!in_buf || !out_buf)
			return TEE_ERROR_BAD_PARAMETERS;

		res = scmi_server_msg_process_thread(channel_id, in_buf,
						     in_size, out_buf,
						     &out_size);
		if (!res)
			params[2].memref.size = (uint32_t)out_size;

		return res;
	}

	return TEE_ERROR_NOT_SUPPORTED;
}

/* Process an OCALL RPC to client and report status */
static enum optee_scmi_ocall_reply pta_scmi_ocall(uint32_t channel_id,
						  struct msg_shm *dyn_shm)
{
	uint32_t ocall2_params[2] = {
		PTA_SCMI_OCALL_CMD_THREAD_READY,
	};

	if (dyn_shm)
		ocall2_params[1] = dyn_shm->out_size;

	if (thread_rpc_ocall2_cmd(ocall2_params)) {
		DMSG("Error on channel %u"PRIu32, channel_id);
		return PTA_SCMI_OCALL_ERROR;
	}

	switch (ocall2_params[0]) {
	case PTA_SCMI_OCALL_PROCESS_SMT_CHANNEL:
		FMSG("Posting SMT message on channel %u"PRIu32, channel_id);
		if (!IS_ENABLED(CFG_SCMI_MSG_SMT) || dyn_shm)
			return PTA_SCMI_OCALL_ERROR;

		scmi_smt_threaded_entry(channel_id);
		break;
	case PTA_SCMI_OCALL_PROCESS_MSG_CHANNEL:
		FMSG("Posting MSG message on channel %u"PRIu32, channel_id);
		if (!dyn_shm)
			return PTA_SCMI_OCALL_ERROR;

		dyn_shm->in_size = ocall2_params[1];
		dyn_shm->out_size = dyn_shm->out_max_size;

		if (IS_ENABLED(CFG_SCMI_MSG_SHM_MSG)) {
			if (scmi_msg_threaded_entry(channel_id, dyn_shm->in_buf,
						    dyn_shm->in_size,
						    dyn_shm->out_buf,
						    &dyn_shm->out_size))
				return PTA_SCMI_OCALL_ERROR;
		}

		if (IS_ENABLED(CFG_SCMI_SCPFW)) {
			if (scmi_server_msg_process_thread(channel_id,
							   dyn_shm->in_buf,
							   dyn_shm->in_size,
							   dyn_shm->out_buf,
							   &dyn_shm->out_size))
				return PTA_SCMI_OCALL_ERROR;
		}

		break;
	case PTA_SCMI_OCALL_CLOSE_THREAD:
		FMSG("Closing channel %u"PRIu32, channel_id);
		break;
	case PTA_SCMI_OCALL_ERROR:
		FMSG("Error on channel %u"PRIu32, channel_id);
		break;
	default:
		DMSG("Invalid Ocall cmd %#x on channel %u"PRIu32,
		     ocall2_params[1], channel_id);
		return PTA_SCMI_OCALL_ERROR;
	}

	return ocall2_params[0];
}

static TEE_Result cmd_scmi_ocall_smt_thread(uint32_t ptypes,
					    TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_ptypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE);

	uint32_t channel_id = (int)params[0].value.a;
	struct scmi_msg_channel *channel = NULL;
	struct optee_msg_arg *rpc_arg = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (ptypes != exp_ptypes)
		return TEE_ERROR_BAD_PARAMETERS;

	if (!IS_ENABLED(CFG_SCMI_MSG_SMT))
		return TEE_ERROR_NOT_SUPPORTED;

	channel = plat_scmi_get_channel(channel_id);
	if (!channel)
		return TEE_ERROR_BAD_PARAMETERS;

	res = thread_rpc_ocall2_prepare(&rpc_arg);
	if (res)
		return res;

	FMSG("Enter Ocall thread on channel %u"PRIu32, channel_id);
	while (1) {
		switch (pta_scmi_ocall(channel_id, NULL)) {
		case PTA_SCMI_OCALL_PROCESS_SMT_CHANNEL:
			continue;
		case PTA_SCMI_OCALL_CLOSE_THREAD:
			thread_rpc_ocall2_unprepare(rpc_arg);
			return TEE_SUCCESS;
		default:
			thread_rpc_ocall2_unprepare(rpc_arg);
			return TEE_ERROR_GENERIC;
		}
	}
}

static TEE_Result cmd_scmi_ocall_msg_thread(uint32_t ptypes,
					    TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_MEMREF_INPUT,
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE);
	struct msg_shm dyn_shm = {
		.in_buf = params[1].memref.buffer,
		.in_size = params[1].memref.size,
		.out_buf = params[2].memref.buffer,
		.out_max_size = params[2].memref.size,
	};
	unsigned int channel_id = params[0].value.a;
	struct optee_msg_arg *rpc_arg = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (ptypes != exp_pt || !dyn_shm.in_buf || !dyn_shm.out_buf)
		return TEE_ERROR_BAD_PARAMETERS;

	if (IS_ENABLED(CFG_SCMI_MSG_SHM_MSG) &&
	    !plat_scmi_get_channel(channel_id))
		return TEE_ERROR_BAD_PARAMETERS;

	res = thread_rpc_ocall2_prepare(&rpc_arg);
	if (res)
		return res;

	while (1) {
		switch (pta_scmi_ocall(channel_id, &dyn_shm)) {
		case PTA_SCMI_OCALL_PROCESS_MSG_CHANNEL:
			continue;
		case PTA_SCMI_OCALL_CLOSE_THREAD:
			thread_rpc_ocall2_unprepare(rpc_arg);
			params[2].memref.size =  dyn_shm.out_size;
			return TEE_SUCCESS;
		default:
			thread_rpc_ocall2_unprepare(rpc_arg);
			return TEE_ERROR_GENERIC;
		}
	}
}

static TEE_Result cmd_get_channel_handle(uint32_t ptypes,
					 TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_ptypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE,
						    TEE_PARAM_TYPE_NONE);
	unsigned int channel_id = params[0].value.a;
	unsigned int caps = params[0].value.b;

	if (ptypes != exp_ptypes || !valid_caps(caps))
		return TEE_ERROR_BAD_PARAMETERS;

	if (!(caps & supported_caps()))
		return TEE_ERROR_NOT_SUPPORTED;

	if (IS_ENABLED(CFG_SCMI_MSG_DRIVERS)) {
		struct scmi_msg_channel *channel = NULL;

		channel = plat_scmi_get_channel(channel_id);
		if (!channel)
			return TEE_ERROR_BAD_PARAMETERS;

		channel->threaded = true;
		params[0].value.a = scmi_smt_channel_handle(channel_id);

		return TEE_SUCCESS;
	}

	if (IS_ENABLED(CFG_SCMI_SCPFW)) {
		/* Only check the channel ID is known from SCP-firwmare */
		return scmi_server_get_channel(channel_id, NULL);
	}

	return TEE_ERROR_NOT_SUPPORTED;
}

static TEE_Result pta_scmi_open_session(uint32_t ptypes __unused,
					TEE_Param par[TEE_NUM_PARAMS] __unused,
					void **session __unused)
{
	struct ts_session *ts = ts_get_current_session();
	struct tee_ta_session *ta_session = to_ta_session(ts);

	/* Only REE kernel is allowed to access SCMI resources */
	if (ta_session->clnt_id.login != TEE_LOGIN_REE_KERNEL) {
		DMSG("Expecting TEE_LOGIN_REE_KERNEL");
		return TEE_ERROR_ACCESS_DENIED;
	}

	if (IS_ENABLED(CFG_SCMI_MSG_DRIVERS) || IS_ENABLED(CFG_SCMI_SCPFW))
		return TEE_SUCCESS;

	return TEE_ERROR_NOT_SUPPORTED;
}

static TEE_Result pta_scmi_invoke_command(void *session __unused, uint32_t cmd,
					  uint32_t ptypes,
					  TEE_Param params[TEE_NUM_PARAMS])
{
	FMSG("SCMI command %#"PRIx32" ptypes %#"PRIx32, cmd, ptypes);

	switch (cmd) {
	case PTA_SCMI_CMD_CAPABILITIES:
		return cmd_capabilities(ptypes, params);
	case PTA_SCMI_CMD_PROCESS_SMT_CHANNEL:
		return cmd_process_smt_channel(ptypes, params);
	case PTA_SCMI_CMD_PROCESS_SMT_CHANNEL_MESSAGE:
		return cmd_process_smt_message(ptypes, params);
	case PTA_SCMI_CMD_PROCESS_MSG_CHANNEL:
		return cmd_process_msg_channel(ptypes, params);
	case PTA_SCMI_CMD_GET_CHANNEL_HANDLE:
		return cmd_get_channel_handle(ptypes, params);
	case PTA_SCMI_CMD_OCALL2_SMT_THREAD:
		return cmd_scmi_ocall_smt_thread(ptypes, params);
	case PTA_SCMI_CMD_OCALL2_MSG_THREAD:
		return cmd_scmi_ocall_msg_thread(ptypes, params);
	default:
		return TEE_ERROR_NOT_SUPPORTED;
	}
}

pseudo_ta_register(.uuid = PTA_SCMI_UUID, .name = PTA_SCMI_NAME,
		   .flags = PTA_DEFAULT_FLAGS | TA_FLAG_CONCURRENT |
			    TA_FLAG_DEVICE_ENUM,
		   .open_session_entry_point = pta_scmi_open_session,
		   .invoke_command_entry_point = pta_scmi_invoke_command);
