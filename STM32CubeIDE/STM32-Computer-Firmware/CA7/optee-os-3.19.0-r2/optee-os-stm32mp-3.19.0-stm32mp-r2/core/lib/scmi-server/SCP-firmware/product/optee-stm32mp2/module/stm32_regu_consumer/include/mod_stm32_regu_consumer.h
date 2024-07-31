/*
 * Copyright (c) 2020, Arm Limited and Contributors. All rights reserved.
 * Copyright (c) 2022, STMicroelectronics
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MOD_STM32_REGU_CONSUMER_H
#define MOD_STM32_REGU_CONSUMER_H

#include <fwk_element.h>
#include <fwk_macros.h>
#include <stdint.h>
#include <stdbool.h>

struct scmi_server_regu_channel;
struct rdev;

/*!
 * \brief Platform regulator configuration.
 */
struct mod_stm32_regu_consumer_dev_config {
    struct rdev *rdev;
    bool default_enabled;
};

/*
 * Build SCMI server resource tables for the voltage domains exposed
 * by platform.
 *
 * @plat - reference to platform data
 * @plat_count - Number of elements in platform data
 */
void scmi_server_build_optee_regu_config(struct scmi_server_regu_channel *plat,
                                         size_t plat_count);

#endif /* MOD_STM32_REGU_CONSUMER_H */
