// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021, STMicroelectronics
 */

#include <assert.h>
#include <pta_bsec.h>
#include <pta_system.h>
#include <stdlib.h>
#include <string.h>
#include <string_ext.h>
#include <ta_stm32mp_nvmem.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <util.h>

#define TA_STM32MP_NVMEM_VERSION	0x02
#define OTP_UPDATE_REQ			BIT(31)
#define OTP_ERROR_DETECTED		BIT(0)

/* OTP Structure
 * uint32_t version
 * uint32_t bsec_state
 * struct OTP [OTP_MAX_SIZE]
 *	uint32_t otp_value
 *	uint32_t otp_lock
 */
#define OTP_EXCHANGE_MIN_SIZE	(sizeof(uint32_t) * 4)
#define OTP_EXCHANGE_ALIGN	(sizeof(uint32_t) * 2)
#define OTP_NB(size)		((((size) / sizeof(uint32_t)) - 2) / 2)

static size_t session_refcount;
static TEE_TASessionHandle pta_session = TEE_HANDLE_NULL;

static TEE_Result nvmem_get_state(uint32_t *state)
{
	TEE_Result res = TEE_SUCCESS;
	TEE_Param params[TEE_NUM_PARAMS] = { };
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);

	assert(state);

	res = TEE_InvokeTACommand(pta_session, TEE_TIMEOUT_INFINITE,
				  PTA_BSEC_GET_STATE,
				  exp_pt, params, NULL);
	if (!res)
		*state = params[0].value.a;

	return res;
}

static TEE_Result nvmem_read(uint32_t pt, TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
	uint32_t *out = (uint32_t *)params[1].memref.buffer;
	size_t out_size = params[1].memref.size;
	uint32_t otp_id = 0;
	uint32_t otp_nb = 0;
	TEE_Result res = TEE_SUCCESS;

	if (pt != exp_pt || !out || !out_size)
		return TEE_ERROR_BAD_PARAMETERS;

	if (params[0].value.a != STM32MP_NVMEM_OTP ||
	    out_size < OTP_EXCHANGE_MIN_SIZE ||
	    !IS_ALIGNED(out_size, OTP_EXCHANGE_ALIGN) ||
	    !IS_ALIGNED_WITH_TYPE(params[1].memref.buffer, uint32_t))
		return TEE_ERROR_BAD_PARAMETERS;

	/* Version */
	*out++ = TA_STM32MP_NVMEM_VERSION;

	/* Global status */
	res = nvmem_get_state(out++);
	if (res)
		return res;

	otp_nb = OTP_NB(out_size);
	for (otp_id = 0; otp_id < otp_nb; otp_id++, out++) {
		TEE_Param params_pta[TEE_NUM_PARAMS] = { };

		/* Read OTP */
		params_pta[0].value.a = otp_id * sizeof(uint32_t);
		params_pta[0].value.b = FUSE_ACCESS;
		params_pta[1].memref.buffer = out;
		params_pta[1].memref.size = sizeof(uint32_t);

		res = TEE_InvokeTACommand(pta_session, TEE_TIMEOUT_INFINITE,
					  PTA_BSEC_READ_MEM,
					  pt, params_pta, NULL);
		if (res) {
			*out++ = 0x0;
			*out = OTP_ERROR_DETECTED;
			continue;
		}

		out++;

		/* Read OTP state */
		params_pta[0].value.a = otp_id * sizeof(uint32_t);
		params_pta[0].value.b = LOCK_ACCESS;
		params_pta[1].memref.buffer = out;
		params_pta[1].memref.size = sizeof(uint32_t);
		res = TEE_InvokeTACommand(pta_session, TEE_TIMEOUT_INFINITE,
					  PTA_BSEC_READ_MEM, pt, params_pta,
					  NULL);
		if (res)
			*out = OTP_ERROR_DETECTED;
	}

	return TEE_SUCCESS;
}

static TEE_Result nvmem_write(uint32_t pt, TEE_Param params[TEE_NUM_PARAMS])
{
	const uint32_t exp_pt = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						TEE_PARAM_TYPE_MEMREF_INPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
	uint32_t *in = (uint32_t *)params[1].memref.buffer;
	size_t in_size = params[1].memref.size;
	uint32_t otp_id = 0;
	uint32_t otp_nb = 0;

	if (pt != exp_pt || !in || !in_size)
		return TEE_ERROR_BAD_PARAMETERS;

	if (params[0].value.a != STM32MP_NVMEM_OTP ||
	    in_size < OTP_EXCHANGE_MIN_SIZE ||
	    !IS_ALIGNED(in_size, OTP_EXCHANGE_ALIGN) ||
	    !IS_ALIGNED_WITH_TYPE(params[1].memref.buffer, uint32_t))
		return TEE_ERROR_BAD_PARAMETERS;

	/* Check version */
	if (*in++ != TA_STM32MP_NVMEM_VERSION)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Skip global status */
	in++;

	otp_nb = OTP_NB(in_size);
	for (otp_id = 0; otp_id < otp_nb; otp_id++) {
		TEE_Param params_pta[TEE_NUM_PARAMS] = { };
		uint32_t value = *in++;
		uint32_t status = *in++;
		TEE_Result res = TEE_SUCCESS;

		if (!(status & OTP_UPDATE_REQ))
			continue;

		/* Write OTP if value not NULL*/
		if (value) {
			params_pta[0].value.a = otp_id * sizeof(uint32_t);
			params_pta[0].value.b = FUSE_ACCESS;
			params_pta[1].memref.buffer = &value;
			params_pta[1].memref.size = sizeof(uint32_t);
			res = TEE_InvokeTACommand(pta_session,
						  TEE_TIMEOUT_INFINITE,
						  PTA_BSEC_WRITE_MEM,
						  pt, params_pta, NULL);
			if (res)
				return res;
		}

		/* Only set the permanent lock, other are not relevant here */
		if (status & LOCK_PERM) {
			uint32_t lock = LOCK_PERM;

			DMSG("Perm lock detected %i", otp_id);
			params_pta[0].value.a = otp_id * sizeof(uint32_t);
			params_pta[0].value.b = LOCK_ACCESS;
			params_pta[1].memref.buffer = &lock;
			params_pta[1].memref.size = sizeof(uint32_t);
			res = TEE_InvokeTACommand(pta_session,
						  TEE_TIMEOUT_INFINITE,
						  PTA_BSEC_WRITE_MEM,
						  pt, params_pta, NULL);
			if (res)
				return res;
		}
	}

	return TEE_SUCCESS;
}

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t pt __unused,
				    TEE_Param params[TEE_NUM_PARAMS] __unused,
				    void **session __unused)
{
	static const TEE_UUID uuid = PTA_BSEC_UUID;
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_PropSetHandle h = TEE_HANDLE_NULL;
	TEE_Identity id = { };

	res = TEE_AllocatePropertyEnumerator(&h);
	if (res)
		goto out;

	TEE_StartPropertyEnumerator(h, TEE_PROPSET_CURRENT_CLIENT);

	res = TEE_GetPropertyAsIdentity(h, NULL, &id);
	if (res)
		goto out;

	if (id.login == TEE_LOGIN_REE_KERNEL) {
		res = TEE_ERROR_ACCESS_DENIED;
		goto out;
	}

	if (!session_refcount) {
		res = TEE_OpenTASession(&uuid, TEE_TIMEOUT_INFINITE, 0, NULL,
					&pta_session, NULL);
		if (res)
			goto out;
	}

	session_refcount++;

out:
	if (h != TEE_HANDLE_NULL)
		TEE_FreePropertyEnumerator(h);

	return res;
}

void TA_CloseSessionEntryPoint(void *sess __unused)
{
	session_refcount--;

	if (!session_refcount)
		TEE_CloseTASession(pta_session);
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess __unused, uint32_t cmd,
				      uint32_t pt,
				      TEE_Param params[TEE_NUM_PARAMS])
{
	switch (cmd) {
	case TA_STM32MP_NVMEM_READ:
		return nvmem_read(pt, params);
	case TA_STM32MP_NVMEM_WRITE:
		return nvmem_write(pt, params);
	default:
		EMSG("Command ID %#"PRIx32" is not supported", cmd);
		return TEE_ERROR_NOT_SUPPORTED;
	}
}
