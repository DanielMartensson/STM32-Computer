/*
 * Copyright (c) 2024, STMicroelectronics and the Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mod_power_domain.h>
#include <scmi_agent_configuration.h>
#include <drivers/stm32mp_dt_bindings.h>

#include <fwk_id.h>
#include <fwk_log.h>
#include <fwk_mm.h>
#include <fwk_module.h>

struct mod_stm32_pd {
    const struct scmi_pd *config;
    unsigned int current_state;
    fwk_id_t requester_id;
    struct mod_pd_driver_input_api *pd_driver_input_api;
};

static struct mod_stm32_pd *mod_stm32_pd_ctx;

static int stm32_pd_get_state(fwk_id_t pd_id, unsigned int *state)
{
    struct mod_stm32_pd *ctx = mod_stm32_pd_ctx + fwk_id_get_element_idx(pd_id);

    *state = ctx->current_state;
    return FWK_SUCCESS;
}

static int stm32_pd_set_state(fwk_id_t pd_id, unsigned int state)
{
    struct mod_stm32_pd *ctx = mod_stm32_pd_ctx + fwk_id_get_element_idx(pd_id);

    ctx->current_state = state;
    return FWK_SUCCESS;
}

static int stm32_pd_reset(fwk_id_t pd_id)
{
    FWK_LOG_DEBUG("Power domain %s reset not supported",
                  fwk_module_get_element_name(pd_id));
    return FWK_E_SUPPORT;
}

static int stm32_pd_set_state_gpu(fwk_id_t pd_id, unsigned int state)
{
    struct mod_stm32_pd *ctx = mod_stm32_pd_ctx + fwk_id_get_element_idx(pd_id);
    struct clk *clk = ctx->config->clk;
    struct rdev *regu = ctx->config->regu;
    int status;

    fwk_assert(state != ctx->current_state);
    fwk_assert(ctx->pd_driver_input_api != NULL);
    fwk_assert(fwk_id_get_element_idx(pd_id) == PD_SCMI_GPU);

    switch (state) {
        case MOD_PD_STATE_ON:
            if (regu != NULL && regulator_enable(regu) != TEE_SUCCESS) {
                return FWK_E_DEVICE;
            }
            if (clk != NULL && clk_enable(clk) != TEE_SUCCESS) {
                if (regu != NULL)
                    regulator_disable(regu);
                return FWK_E_DEVICE;
            }
            break;
        case MOD_PD_STATE_OFF:
            if (clk != NULL) {
                clk_disable(clk);
            }
            if (regu != NULL && regulator_disable(regu) != TEE_SUCCESS) {
                if (clk != NULL) {
                    clk_enable(clk);
                }
                return FWK_E_DEVICE;
            }
            break;
        default:
            FWK_LOG_ERR("State %d not supported on power domain %s",
                        state,
                        fwk_module_get_element_name(pd_id));
            return FWK_E_SUPPORT;
    }

    status = ctx->pd_driver_input_api->report_power_state_transition(
        ctx->requester_id,
        state);
    assert(status == FWK_SUCCESS);

    FWK_LOG_DEBUG("Power domain %s change state to %s",
                  fwk_module_get_element_name(pd_id),
                  state == MOD_PD_STATE_ON ? "ON" : "OFF");

    ctx->current_state = state;
    return FWK_SUCCESS;
}

static int stm32_pd_bind(fwk_id_t id, unsigned int round)
{
    struct mod_stm32_pd *ctx;

    /* Nothing to do for the module */
    if (fwk_module_is_valid_module_id(id)) {
        return FWK_SUCCESS;
    }

    /* Element id is not valid */
    if (!fwk_module_is_valid_element_id(id)) {
        FWK_LOG_ERR("Cannot bind %s, element id is needed",
                    fwk_module_get_element_name(id));
        return FWK_E_SUPPORT;
    }

    ctx = mod_stm32_pd_ctx + fwk_id_get_element_idx(id);

    /* stm32_pd_process_bind_request probably isn't been called right now */
    if (!fwk_module_is_valid_element_id(ctx->requester_id)) {
        FWK_LOG_ERR("stm32_pd_process_bind_request hasn't been called yet");
        return FWK_E_ACCESS;
    }

    /* Only get an input API from power domain framework */
    if (fwk_id_get_module_idx(ctx->requester_id) !=
            FWK_MODULE_IDX_POWER_DOMAIN) {
        FWK_LOG_ERR("Requester is not power domain module");
        return FWK_E_SUPPORT;
    }

    return fwk_module_bind(ctx->requester_id,
                           mod_pd_api_id_driver_input,
                           &ctx->pd_driver_input_api);
}

static int stm32_pd_element_init(fwk_id_t element_id,
                                 unsigned int unused,
                                 const void *data)
{
    struct mod_stm32_pd *ctx =
                        mod_stm32_pd_ctx + fwk_id_get_element_idx(element_id);

    ctx->config = (const struct scmi_pd *)data;

    if (fwk_id_get_element_idx(element_id) == PD_SCMI_GPU) {
        ctx->current_state = MOD_PD_STATE_OFF;
        return FWK_SUCCESS;
    }

    ctx->current_state = MOD_PD_STATE_ON;
    return FWK_SUCCESS;
}

static const struct mod_pd_driver_api stm32_pd_api_gpu = {
    .set_state = stm32_pd_set_state_gpu,
    .get_state = stm32_pd_get_state,
    .reset = stm32_pd_reset,
};

static const struct mod_pd_driver_api stm32_pd_api = {
    .set_state = stm32_pd_set_state,
    .get_state = stm32_pd_get_state,
    .reset = stm32_pd_reset,
};

static int stm32_pd_process_bind_request(fwk_id_t requester_id,
                                         fwk_id_t target_id,
                                         fwk_id_t api_type,
                                         const void **api)
{
    struct mod_stm32_pd *ctx = NULL;

    ctx = mod_stm32_pd_ctx + fwk_id_get_element_idx(target_id);
    ctx->requester_id = requester_id;

    if (fwk_id_get_element_idx(target_id) == PD_SCMI_GPU) {
        *api = &stm32_pd_api_gpu;
        return FWK_SUCCESS;
    }

    *api = &stm32_pd_api;
    return FWK_SUCCESS;
}

static int stm32_pd_init(fwk_id_t module_id,
    unsigned int element_count, const void *data)
{
    mod_stm32_pd_ctx = fwk_mm_calloc(element_count, sizeof(*mod_stm32_pd_ctx));

    return FWK_SUCCESS;
}

const struct fwk_module module_stm32_pd = {
    .type = FWK_MODULE_TYPE_DRIVER,
    .api_count = 1,
    .init = stm32_pd_init,
    .element_init = stm32_pd_element_init,
    .bind = stm32_pd_bind,
    .process_bind_request = stm32_pd_process_bind_request,
};