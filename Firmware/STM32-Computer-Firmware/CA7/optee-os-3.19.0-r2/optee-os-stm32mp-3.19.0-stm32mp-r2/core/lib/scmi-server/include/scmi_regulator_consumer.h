/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2020-2023, STMicroelectronics
 */

#ifndef SCMI_SERVER_REGULATOR_CONSUMER_H
#define SCMI_SERVER_REGULATOR_CONSUMER_H

#include <drivers/regulator.h>
#include <stdbool.h>
#include <stdint.h>
#include <tee_api_types.h>

struct scmi_server_regu {
	uint32_t domain_id;
	struct rdev *rdev;
	bool enabled;
};

struct scmi_server_regu_channel {
	unsigned int channel_id;
	struct scmi_server_regu *regu;
	size_t regu_count;
};

#ifdef CFG_SCMI_SERVER_REGULATOR_CONSUMER
/*
 * Initialize SCMI regulator from regulator consumer information from DT.
 * Returns a TEE_Result compliant value
 */
TEE_Result scmi_regulator_consumer_init(void);

#else
static inline TEE_Result scmi_regulator_consumer_init(void)
{
	return TEE_SUCCESS;
}
#endif /* CFG_SCMI_SERVER_REGULATOR_CONSUMER */
#endif /* SCMI_SERVER_REGULATOR_CONSUMER_H */
