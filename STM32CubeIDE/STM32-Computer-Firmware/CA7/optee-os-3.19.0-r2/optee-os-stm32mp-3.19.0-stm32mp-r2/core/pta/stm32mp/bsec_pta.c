// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (C) 2021, STMicroelectronics - All Rights Reserved
 */

#include <config.h>
#include <drivers/stm32_bsec.h>
#include <kernel/pseudo_ta.h>
#include <kernel/user_access.h>
#include <kernel/user_mode_ctx.h>
#include <kernel/user_ta.h>
#include <mm/vm.h>
#include <pta_bsec.h>
#include <string.h>
#include <util.h>

#define TA_STM32MP_NVMEM_UUID { 0x1a8342cc, 0x81a5, 0x4512, \
		{ 0x99, 0xfe, 0x9e, 0x2b, 0x3e, 0x37, 0xd6, 0x26 } }

#define PTA_NAME "bsec.pta"

static TEE_Result bsec_check_access(uint32_t otp_id)
{
	struct ts_session *ts = ts_get_current_session();
	struct tee_ta_session *ta_session = to_ta_session(ts);

	/* REE kernel is allowed to access non secure OTP */
	if (ta_session->clnt_id.login == TEE_LOGIN_REE_KERNEL) {
		if (!stm32_bsec_nsec_can_access_otp(otp_id))
			return TEE_ERROR_ACCESS_DENIED;

		return TEE_SUCCESS;
	}

	if (!stm32_bsec_can_access_otp(otp_id))
		return TEE_ERROR_ACCESS_DENIED;

	/* All TA have access to fuses */
	return TEE_SUCCESS;
}

static TEE_Result bsec_read_mem(uint32_t pt, TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);

	uint32_t *buf = (uint32_t *)params[1].memref.buffer;
	uint32_t otp_start = 0;
	size_t otp_length = 0;
	uint32_t otp_id = 0;
	TEE_Result res = TEE_ERROR_GENERIC;
	size_t size = params[1].memref.size;
	bool locked = false;
	unsigned int otp_base_offset = 0;
	unsigned int bsec_command = 0;

	if (pt != exp_pt || !buf || !size ||
	    !IS_ALIGNED_WITH_TYPE(params[1].memref.buffer, uint32_t))
		return TEE_ERROR_BAD_PARAMETERS;

	otp_base_offset = params[0].value.a;
	bsec_command = params[0].value.b;

	/* check 32bits alignment */
	if (otp_base_offset % BSEC_BYTES_PER_WORD ||
	    size % BSEC_BYTES_PER_WORD)
		return TEE_ERROR_BAD_PARAMETERS;

	otp_start = otp_base_offset / BSEC_BYTES_PER_WORD;
	otp_length = size / BSEC_BYTES_PER_WORD;

	for (otp_id = otp_start; otp_id < otp_start + otp_length;
	     otp_id++, buf++) {
		res = bsec_check_access(otp_id);
		switch (bsec_command) {
		case SHADOW_ACCESS:
			if (res) {
				/* Force 0 when access is not allowed */
				*buf = 0x0;
				continue;
			}
			/* Read shadow register */
			res = stm32_bsec_read_otp(buf, otp_id);
			FMSG("Read shadow %"PRIu32" val: %#"PRIx32, otp_id,
			     *buf);
			break;

		case FUSE_ACCESS:
			/* check access */
			if (res)
				goto out;
			/* Read fuse value */
			res = stm32_bsec_shadow_read_otp(buf, otp_id);
			FMSG("Read fuse %"PRIu32" val: %#"PRIx32, otp_id, *buf);
			break;

		case LOCK_ACCESS:
			if (res) {
				/* Force error when access is not allowed */
				*buf = LOCK_ERROR;
				continue;
			}
			*buf = 0;
			/* Read lock value */
			res = stm32_bsec_read_permanent_lock(otp_id, &locked);
			if (res)
				goto out;

			if (locked)
				*buf |= LOCK_PERM;

			res = stm32_bsec_read_sr_lock(otp_id, &locked);
			if (res)
				goto out;

			if (locked)
				*buf |= LOCK_SHADOW_R;

			res = stm32_bsec_read_sw_lock(otp_id, &locked);
			if (res)
				goto out;

			if (locked)
				*buf |= LOCK_SHADOW_W;

			res = stm32_bsec_read_sp_lock(otp_id, &locked);
			if (res)
				goto out;

			if (locked)
				*buf |= LOCK_SHADOW_P;

			FMSG("Read lock %"PRIu32" val: %#"PRIx32, otp_id, *buf);
			break;

		default:
			FMSG("%"PRIu32" invalid operation: %"PRIu32, otp_id,
			     bsec_command);
			res = TEE_ERROR_BAD_PARAMETERS;
		}

		if (res)
			goto out;
	}

	FMSG("Buffer orig %p, size %zu", buf, size);

	res = TEE_SUCCESS;
out:
	return res;
}

static TEE_Result bsec_write_mem(uint32_t pt, TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_MEMREF_INPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);

	uint32_t *buf = (uint32_t *)params[1].memref.buffer;
	size_t size = params[1].memref.size;
	uint32_t otp_start = 0;
	size_t otp_length = 0;
	uint32_t otp_id = 0;
	TEE_Result res = TEE_ERROR_GENERIC;
	unsigned int otp_base_offset = 0;
	unsigned int bsec_command = 0;

	if (pt != exp_pt || !buf || !size ||
	    !IS_ALIGNED_WITH_TYPE(params[1].memref.buffer, uint32_t))
		return TEE_ERROR_BAD_PARAMETERS;

	otp_base_offset = params[0].value.a;
	bsec_command = params[0].value.b;

	/* check 32bits alignment */
	if (params[0].value.a % BSEC_BYTES_PER_WORD ||
	    size % BSEC_BYTES_PER_WORD)
		return TEE_ERROR_BAD_PARAMETERS;

	otp_start = otp_base_offset / BSEC_BYTES_PER_WORD;
	otp_length = size / BSEC_BYTES_PER_WORD;

	/* Initial check to ensure that all BSEC words are available */
	for (otp_id = otp_start; otp_id < otp_start + otp_length; otp_id++) {
		res = bsec_check_access(otp_id);
		if (res)
			return res;
	}

	for (otp_id = otp_start; otp_id < otp_start + otp_length;
	     otp_id++, buf++) {
		switch (bsec_command) {
		case SHADOW_ACCESS:
			/* Write shadow register */
			FMSG("Write shadow %"PRIx32" : %"PRIx32,
			     otp_id, *buf);
			res = stm32_bsec_write_otp(*buf, otp_id);
			break;

		case FUSE_ACCESS:
			/* Write fuse value */
			FMSG("Write fuse %"PRIx32" : %08"PRIx32,
			     otp_id, *buf);
			res = stm32_bsec_program_otp(*buf, otp_id);
			break;

		case LOCK_ACCESS:
			if (*buf & LOCK_PERM) {
				FMSG("Perm lock access");
				res = stm32_bsec_permanent_lock_otp(otp_id);
				if (res)
					break;
			}

			if (*buf & LOCK_SHADOW_R) {
				FMSG("Shadow read lock detected");
				res = stm32_bsec_set_sr_lock(otp_id);
				if (res)
					break;
			}

			if (*buf & LOCK_SHADOW_W) {
				FMSG("Shadow write lock detected");
				res = stm32_bsec_set_sw_lock(otp_id);
				if (res)
					break;
			}

			if (*buf & LOCK_SHADOW_P) {
				FMSG("Shadow programming lock detected");
				res = stm32_bsec_set_sp_lock(otp_id);
			}

			break;

		default:
			FMSG("OTP %"PRIx32" invalid operation: %"PRIx32,
			     otp_id, bsec_command);
			res = TEE_ERROR_BAD_PARAMETERS;
		}

		if (res)
			return res;
	}

	return TEE_SUCCESS;
}

static TEE_Result bsec_pta_state(uint32_t pt, TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
	TEE_Result res = TEE_ERROR_GENERIC;
	uint32_t state = BSEC_STATE_INVALID;

	if (pt != exp_pt)
		return TEE_ERROR_BAD_PARAMETERS;

	res = stm32_bsec_get_state(&state);
	if (res)
		return res;

	params[0].value.a = state;

	return TEE_SUCCESS;
}

static TEE_Result bsec_pta_invoke_command(void *pSessionContext __unused,
					  uint32_t cmd_id,
					  uint32_t param_types,
					  TEE_Param params[TEE_NUM_PARAMS])
{
	FMSG(PTA_NAME" command %#"PRIx32" ptypes %#"PRIx32,
	     cmd_id, param_types);

	switch (cmd_id) {
	case PTA_BSEC_READ_MEM:
		return bsec_read_mem(param_types, params);
	case PTA_BSEC_WRITE_MEM:
		return bsec_write_mem(param_types, params);
	case PTA_BSEC_GET_STATE:
		return bsec_pta_state(param_types, params);
	default:
		break;
	}

	return TEE_ERROR_NOT_IMPLEMENTED;
}

static TEE_Result pta_bsec_open_session(uint32_t ptypes __unused,
					TEE_Param par[TEE_NUM_PARAMS] __unused,
					void **session __unused)
{
	uint32_t login = to_ta_session(ts_get_current_session())->clnt_id.login;
	struct ts_session *caller_ts = ts_get_calling_session();
	static const TEE_UUID ta_uuid = TA_STM32MP_NVMEM_UUID;
	uint32_t state = BSEC_STATE_INVALID;
	TEE_Result res = TEE_ERROR_GENERIC;

	if (!IS_ENABLED(CFG_STM32_BSEC) && !IS_ENABLED(CFG_STM32_BSEC3))
		return TEE_ERROR_NOT_SUPPORTED;

	if (login == TEE_LOGIN_TRUSTED_APP &&
	    !memcmp((char *)&caller_ts->ctx->uuid, (char *)&ta_uuid,
		    sizeof(TEE_UUID))) {
		assert(is_user_ta_ctx(caller_ts->ctx));

		res = stm32_bsec_get_state(&state);
		if (res || (state & BSEC_STATE_MASK) != BSEC_STATE_SEC_OPEN)
			return TEE_ERROR_ACCESS_DENIED;

		return TEE_SUCCESS;
	}

	if (login == TEE_LOGIN_REE_KERNEL)
		return TEE_SUCCESS;

	return TEE_ERROR_ACCESS_DENIED;
}

pseudo_ta_register(.uuid = PTA_BSEC_UUID, .name = PTA_NAME,
		   .flags = PTA_DEFAULT_FLAGS | TA_FLAG_CONCURRENT |
			    TA_FLAG_DEVICE_ENUM,
		   . open_session_entry_point = pta_bsec_open_session,
		   .invoke_command_entry_point = bsec_pta_invoke_command);
