/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2019-2021, Linaro Limited
 */
#ifndef PTA_SCMI_CLIENT_H
#define PTA_SCMI_CLIENT_H

#define PTA_SCMI_UUID { 0xa8cfe406, 0xd4f5, 0x4a2e, \
		{ 0x9f, 0x8d, 0xa2, 0x5d, 0xc7, 0x54, 0xc0, 0x99 } }

#define PTA_SCMI_NAME "PTA-SCMI"

/*
 * PTA_SCMI_CMD_CAPABILITIES - Get channel capabilities
 *
 * [out]    value[0].a: Capabilities bit mask (PTA_SCMI_CAPS_*)
 * [out]    value[0].b: Extended capabilities or 0
 */
#define PTA_SCMI_CMD_CAPABILITIES	0

/*
 * PTA_SCMI_CMD_PROCESS_SMT_CHANNEL - Process SCMI message in SMT buffer
 *
 * [in]     value[0].a: Channel handle
 *
 * Shared memory used for SCMI message/response exhange is expected
 * already identified and bound to channel handle in both SCMI agent
 * and SCMI server (OP-TEE) parts.
 * The memory uses SMT header to carry SCMI meta-data (protocol ID and
 * protocol message ID).
 */
#define PTA_SCMI_CMD_PROCESS_SMT_CHANNEL	1

/*
 * PTA_SCMI_CMD_PROCESS_SMT_CHANNEL_MESSAGE - Process SCMI message in
 *				SMT buffer pointed by memref parameters
 *
 * [in]     value[0].a: Channel handle
 * [in/out] memref[1]: Message/response buffer (SMT and SCMI payload)
 *
 * Shared memory used for SCMI message/response is a SMT buffer
 * referenced by param[1]. It shall be 128 bytes large to fit response
 * payload whatever message playload size.
 * The memory uses SMT header to carry SCMI meta-data (protocol ID and
 * protocol message ID).
 */
#define PTA_SCMI_CMD_PROCESS_SMT_CHANNEL_MESSAGE	2

/*
 * PTA_SCMI_CMD_GET_CHANNEL_HANDLE - Get handle for an SCMI channel
 *
 * Get a handle for the SCMI channel. This handle value is to be passed
 * as argument to some commands as PTA_SCMI_CMD_PROCESS_*.
 *
 * [in]     value[0].a: Channel identifier or 0 if no assigned ID
 * [in]     value[0].b: Requested capabilities mask (PTA_SCMI_CAPS_*)
 * [out]    value[0].a: Returned channel handle
 */
#define PTA_SCMI_CMD_GET_CHANNEL_HANDLE		3

/*
 * PTA_SCMI_CMD_PROCESS_MSG_CHANNEL - Process SCMI message in a MSG
 * buffer pointed by memref parameters
 *
 * [in]     value[0].a: Channel handle
 * [in]     memref[1]: Input message shared buffer
 * [out]    memref[2]: Output message shared buffer
 */
#define PTA_SCMI_CMD_PROCESS_MSG_CHANNEL	4

/*
 * PTA_SCMI_CMD_OCALL2_SMT_THREAD - Allocate a thread context using Ocall2
 * for processing of SMT messages.
 *
 * [in]     value[0].a: channel handle
 *
 * Use Ocall2 support to create a provisioned OP-TEE thread context for
 * an SMT based SCMI channel. Successful creation of the thread makes this
 * command to return with Ocall command PTA_SCMI_OCALL_CMD_THREAD_READY.
 */
#define PTA_SCMI_CMD_OCALL2_SMT_THREAD		2048

/*
 * PTA_SCMI_CMD_OCALL2_MSG_THREAD - Allocate an thread context using Ocall2
 * for processing of MSG messages.
 *
 * [in]     value[0].a: channel handle
 *
 * Use Ocall2 support to create a provisioned OP-TEE thread context for
 * an MSG based SCMI channel. Successful creation of the thread makes this
 * command to return with Ocall command PTA_SCMI_OCALL_CMD_THREAD_READY.
 */
#define PTA_SCMI_CMD_OCALL2_MSG_THREAD		2049

/*
 * Capabilities
 */

/* Channel supports shared memory using the SMT header protocol */
#define PTA_SCMI_CAPS_SMT_HEADER			BIT32(0)

/* Channel supports shared memory using the MSG header protocol */
#define PTA_SCMI_CAPS_MSG_HEADER			BIT32(1)

/*
 * Channel can use command PTA_SCMI_CMD_OCALL2_SMT_THREAD to provision a
 * TEE thread for SCMI message passing.
 */
#define PTA_SCMI_CAPS_OCALL2_THREAD			BIT32(31)

#define PTA_SCMI_CAPS_MASK		(PTA_SCMI_CAPS_SMT_HEADER | \
					 PTA_SCMI_CAPS_MSG_HEADER | \
					 PTA_SCMI_CAPS_OCALL2_THREAD)
/*
 * enum optee_scmi_ocall_cmd
 * enum optee_scmi_ocall_reply
 *
 * These enumerates define the IDs used by REE/TEE to communicate in the
 * established REE/TEE Ocall thread context.
 *
 * At channel setup, we start from the REE: caller requests an Ocall context.
 *
 * 0. REE client opens a session toward PTA SCMI. Client invokes PTA command
 *    PTA_SCMI_CMD_GET_CHANNEL_HANDLE to get a channel handler and passes
 *    capabilities for querying which of SMT or MSG shared memory protocol is
 *    to be used and whether OCall can be used to suspend SCMI task until next
 *    incoming SCMI message.
 *
 * 1. When client and OP-TEE core both support Ocall context management,
 *    REE invokes command PTA_SCMI_CMD_OCALL2_SMT_THREAD (or respectively
 *    PTA_SCMI_CMD_OCALL2_MSG_THREAD) with an Ocall context.
 *    This is the initial invocation of the Ocall thread context. Any further
 *    error in the thread communication make the Ocall to return from REE to
 *    TEE with an error status (Ocall2 out_param1 == 0) upon which SCMI PTA
 *    will return from initial command PTA_SCMI_CMD_OCALL2_SMT_THREAD
 *    (or resp. PTA_SCMI_CMD_OCALL2_MSG_THREAD) with an error result.
 *    Whether SMT or MSG shared memory protocol is used depends on the
 *    capabilities exchanged at step 0.
 *
 * 2. Upon support of Ocall the PTA creates an Ocall context and returns to
 *    REE with Ocall command PTA_SCMI_OCALL_CMD_THREAD_READY.
 *
 * 3. When an SCMI message is posted, REE returns to the PTA, from the Ocall,
 *    with output out_param1 set to PTA_SCMI_OCALL_PROCESS_SMT_CHANNEL
 *    (or respectively PTA_SCMI_CMD_OCALL2_MSG_THREAD) to post an SCMI message.
 *    In such case, OP-TEE processes the message and returns to REE with
 *    Ocall command PTA_SCMI_OCALL_CMD_THREAD_READY. The SCMI response is in
 *    the shared memory buffer. Whether SMT or MSG shared memory protocol is
 *    used depends on the capabilities exchanged at step 0.
 *
 * 4. Alternatively REE can return from the Ocall with out_param1 set to
 *    PTA_SCMI_OCALL_CLOSE_THREAD. This requests OP-TEE to terminate the
 *    Ocall, release resources and return from initial command invocation at
 *    step 1. as if REE closes the SCMI communication.
 *
 * At anytime if an error is reported by Ocall command replies, SCMI PTA
 * releases the Ocall thread context and returns from initial invocation
 * at step 1. PTA_SCMI_OCALL_ERROR is used in Ocall return to force an error
 * report.
 *
 * REE channel initialization completes when returning from step 2.
 * REE agent posts an SCMI message through step 3.
 * At channel release, REE driver executes step 4.
 */

enum optee_scmi_ocall_cmd {
	PTA_SCMI_OCALL_CMD_THREAD_READY = 0,
};

enum optee_scmi_ocall_reply {
	PTA_SCMI_OCALL_ERROR = 0, /* OPTEE_RPC_OCALL2_OUT_PARAM1_ERROR */
	PTA_SCMI_OCALL_CLOSE_THREAD = 1,
	PTA_SCMI_OCALL_PROCESS_SMT_CHANNEL = 2,
	PTA_SCMI_OCALL_PROCESS_MSG_CHANNEL = 3,
};
#endif /* SCMI_PTA_SCMI_CLIENT_H */
