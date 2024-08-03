/*
 * Copyright (c) 2017-2019, Arm Limited and Contributors. All rights reserved.
 * Copyright (c) 2022, STMicroelectronics
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>

#include <fwk_log.h>
#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <mod_psu.h>
#include <mod_psu_optee_regulator.h>
#include <mod_scmi_std.h>

#include <assert.h>
#include <config.h>
#include <drivers/regulator.h>
#include <io.h>
#include <trace.h>

#define DEBUG_MSG(...)	FMSG(__VA_ARGS__)

/* Module context */
struct psu_optee_regulator_ctx {
    struct mod_psu_optee_regulator_dev_config *config;
    unsigned int dev_count;
};

/* A single instance handles all voltage regulators abstracted by regulator.h */
static struct psu_optee_regulator_ctx module_ctx;

static char __maybe_unused *regulator_name(struct rdev *rdev)
{
    if (rdev)
        return (char *)rdev->reg_name;

    return NULL;
}

static struct rdev *get_regulator(fwk_id_t id)
{
    unsigned int elt_index;

    elt_index = fwk_id_get_element_idx(id);
    if (elt_index >= module_ctx.dev_count)
        return NULL;

    return module_ctx.config[elt_index].rdev;
}

/*
 * Driver functions for the PSU API
 */
static int psu_optee_regulator_set_enabled(fwk_id_t id, bool enabled)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    struct rdev *regulator;

    regulator = get_regulator(id);
    if (!regulator) {
        return FWK_E_PARAM;
    }

    DEBUG_MSG("PSU set %s %s", regulator_name(regulator),
          enabled ? "ON" : "OFF");

    if (enabled) {
        res = regulator_enable(regulator);
    } else {
        res = regulator_disable(regulator);
    }

    if (res) {
        return FWK_E_HANDLER;
    } else {
        return FWK_SUCCESS;
    }
}

static int psu_optee_regulator_get_enabled(fwk_id_t id, bool *enabled)
{
    struct rdev *regulator;

   regulator = get_regulator(id);
   if (!regulator || (enabled == NULL)) {
        return FWK_E_PARAM;
    }

    *enabled = regulator_is_enabled((const struct rdev *)regulator);
    DEBUG_MSG("PSU get %s state: %s", regulator_name(regulator),
              enabled ? "ON" : "OFF");

    return FWK_SUCCESS;
}

static int psu_optee_regulator_set_voltage(fwk_id_t id, uint32_t voltage)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    struct rdev *regulator;
    uint16_t level_mv = voltage;

    regulator = get_regulator(id);
    if (!regulator) {
        return FWK_E_PARAM;
    }

    DEBUG_MSG("PSU set regulator %s level: %"PRIu32"mV",
              regulator_name(regulator), (uint32_t)level_mv);

    res = regulator_set_voltage(regulator, level_mv);

    if (res) {
        return FWK_E_HANDLER;
    } else {

        return FWK_SUCCESS;
    }
}

static int psu_optee_regulator_get_voltage(fwk_id_t id, uint32_t *voltage)
{
    TEE_Result res = TEE_ERROR_GENERIC;
    struct rdev *regulator;
    uint16_t level_mv;

    regulator = get_regulator(id);
    if (!regulator || (voltage == NULL)) {
        return FWK_E_PARAM;
    }

    res = regulator_get_voltage(regulator, &level_mv);

    DEBUG_MSG("PSU get regulator %s level: %"PRIu32"mV (res %#"PRIx32")",
              regulator_name(regulator), (uint32_t)level_mv, res);

    if (res) {
        return FWK_E_HANDLER;
    }

    *voltage = level_mv;

    return FWK_SUCCESS;
}

static struct mod_psu_driver_api psu_driver_api = {
    .set_enabled = psu_optee_regulator_set_enabled,
    .get_enabled = psu_optee_regulator_get_enabled,
    .set_voltage = psu_optee_regulator_set_voltage,
    .get_voltage = psu_optee_regulator_get_voltage,
};

static int psu_optee_regulator_init(fwk_id_t module_id,
                                    unsigned int element_count,
                                    const void *data)
{
    return FWK_SUCCESS;
}

static int psu_optee_regulator_element_init(fwk_id_t element_id,
                                            unsigned int sub_element_count,
                                            const void *data)
{
    fwk_assert(data != NULL);

    module_ctx.config = (struct mod_psu_optee_regulator_dev_config *)data;
    module_ctx.dev_count = sub_element_count;

    return FWK_SUCCESS;
}

static int psu_optee_regulator_process_bind_request(fwk_id_t source_id,
                                                    fwk_id_t target_id,
                                                    fwk_id_t api_id,
                                                    const void **api)
{
    *api = &psu_driver_api;

    return FWK_SUCCESS;
}

const struct fwk_module module_psu_optee_regulator = {
    .api_count = 1,
    .type = FWK_MODULE_TYPE_DRIVER,
    .init = psu_optee_regulator_init,
    .element_init = psu_optee_regulator_element_init,
    .process_bind_request = psu_optee_regulator_process_bind_request,
};
