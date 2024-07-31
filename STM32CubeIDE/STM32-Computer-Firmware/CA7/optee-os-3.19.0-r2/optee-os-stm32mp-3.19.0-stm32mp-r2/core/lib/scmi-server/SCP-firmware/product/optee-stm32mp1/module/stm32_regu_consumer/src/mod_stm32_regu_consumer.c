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
#include <mod_scmi_std.h>
#include <mod_stm32_regu_consumer.h>
#include <mod_voltage_domain.h>

#include <assert.h>
#include <drivers/regulator.h>
#include <scmi_regulator_consumer.h>
#include <trace.h>

#define DEBUG_MSG(...)	FMSG(__VA_ARGS__)

/* Device context */
struct stm32_regu_consumer_dev_ctx {
	struct rdev *rdev;
	bool enabled;
};

/* Module context */
struct stm32_regu_consumer_ctx {
    struct stm32_regu_consumer_dev_ctx *dev_ctx_table;
    unsigned int dev_count;
};

/* A single instance handles all voltage regulators abstracted by regulator.h */
static struct stm32_regu_consumer_ctx module_ctx;

static char *regu_name(struct rdev *rdev)
{
	if (rdev)
		return (char *)rdev->reg_name;

	return NULL;
}

static int find_ctx(fwk_id_t dev_id,
		    struct stm32_regu_consumer_dev_ctx **out_ctx)
{
    struct stm32_regu_consumer_dev_ctx *ctx;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);

    if (!fwk_module_is_valid_element_id(dev_id))
        return FWK_E_PARAM;

    if (!ctx->rdev)
        return FWK_E_ACCESS;

    *out_ctx = ctx;

    return FWK_SUCCESS;
}

static int stm32_regu_consumer_get_config(fwk_id_t dev_id, uint8_t *mode_type,
					  uint8_t *mode_id)
{
    struct stm32_regu_consumer_dev_ctx *ctx;
    int ret;

    ret = find_ctx(dev_id, &ctx);
    if (ret)
        return ret;

    *mode_type = MOD_VOLTD_MODE_TYPE_ARCH;

    if (ctx->enabled)
        *mode_id = MOD_VOLTD_MODE_ID_ON;
    else
        *mode_id = MOD_VOLTD_MODE_ID_OFF;

    DEBUG_MSG("SCMI voltd %u: get config PMIC %s = %#"PRIx8", %#"PRIx8,
	      fwk_id_get_element_idx(dev_id), regu_name(ctx->rdev), *mode_type,
	      *mode_id);

    return FWK_SUCCESS;
}

static int stm32_regu_consumer_set_config(fwk_id_t dev_id, uint8_t mode_type,
					  uint8_t mode_id)
{
    struct stm32_regu_consumer_dev_ctx *ctx;
    int ret;

    ret = find_ctx(dev_id, &ctx);
    if (ret)
        return ret;

    if (mode_type != MOD_VOLTD_MODE_TYPE_ARCH) {
        return FWK_E_PARAM;
    }

    switch (mode_id) {
    case MOD_VOLTD_MODE_ID_ON:
        if (!ctx->enabled) {
            if (regulator_enable(ctx->rdev))
                return FWK_E_DEVICE;

            ctx->enabled = true;
        }
        break;

    case MOD_VOLTD_MODE_ID_OFF:
        if (ctx->enabled) {
            if (regulator_disable(ctx->rdev))
                return FWK_E_DEVICE;

            ctx->enabled = false;
        }
        break;

    default:
        return FWK_E_PARAM;
    }

    DEBUG_MSG("SCMI voltd %u: set config PMIC %s to type %#"PRIx8" / mode %#"PRIx8,
	      fwk_id_get_element_idx(dev_id), regu_name(ctx->rdev), mode_type,
	      mode_id);

    return FWK_SUCCESS;
}

static int stm32_regu_consumer_get_level(fwk_id_t dev_id, int *level_uv)
{
    struct stm32_regu_consumer_dev_ctx *ctx;
    uint16_t level_mv;
    int ret;

    ret = find_ctx(dev_id, &ctx);
    if (ret)
        return ret;

    if (regulator_get_voltage(ctx->rdev, &level_mv))
        return FWK_E_PANIC;

    *level_uv = (int)level_mv * 1000;

    DEBUG_MSG("SCMI voltd %u: get level PMIC %s = %d",
	      fwk_id_get_element_idx(dev_id), regu_name(ctx->rdev), *level_uv);

    return FWK_SUCCESS;
}

static int stm32_regu_consumer_set_level(fwk_id_t dev_id, int level_uv)
{
    struct stm32_regu_consumer_dev_ctx *ctx = NULL;
    int ret = FWK_E_PANIC;

    if (level_uv / 1000 > UINT16_MAX) {
        FWK_LOG_ERR("Volatge level too high (mV shall fit in 16 bits)");
        return FWK_E_PARAM;
    }

    ret = find_ctx(dev_id, &ctx);
    if (ret)
        return ret;

    if (regulator_set_voltage(ctx->rdev, level_uv / 1000))
        return FWK_E_DEVICE;

    DEBUG_MSG("SCMI voltd %u: set level PMIC %s to %d",
	      fwk_id_get_element_idx(dev_id), regu_name(ctx->rdev), level_uv);

    return FWK_SUCCESS;
}

static void find_bound_uv(const uint16_t *levels, size_t count,
			  int32_t *min, int32_t *max)
{
	size_t n = 0;

	*min = INT32_MAX;
	*max = INT32_MIN;

	for (n = 0; n < count; n++) {
		if (*min > levels[n])
			*min = levels[n];
		if (*max < levels[n])
			*max = levels[n];
	}

	/* Convert from mV to uV */
	*min *= 1000;
	*max *= 1000;
}

static int stm32_regu_consumer_get_info(fwk_id_t dev_id,
					struct mod_voltd_info *info)
{
    struct stm32_regu_consumer_dev_ctx *ctx;
    uint16_t *levels;
    size_t full_count;
    int ret;

    ret = find_ctx(dev_id, &ctx);
    if (ret == FWK_E_ACCESS) {
        static const char reserved[] = "reserved";

        memset(info, 0, sizeof(*info));
        info->name = reserved;
        info->level_range.level_type = MOD_VOLTD_VOLTAGE_LEVEL_DISCRETE;
        info->level_range.level_count = 1;
        info->level_range.min_uv = 0;
        info->level_range.max_uv = 0;

	return FWK_SUCCESS;
    } else if (ret != FWK_SUCCESS) {
        return ret;
    }

    if (regulator_list_voltages(ctx->rdev, &levels, &full_count)) {
        return FWK_E_SUPPORT;
    }

    memset(info, 0, sizeof(*info));
    info->name = regu_name(ctx->rdev);
    info->level_range.level_type = MOD_VOLTD_VOLTAGE_LEVEL_DISCRETE;
    info->level_range.level_count = full_count;
    find_bound_uv(levels, full_count,
		  &info->level_range.min_uv, &info->level_range.max_uv);

    DEBUG_MSG("SCMI voltd %u: get_info PMIC %s, range [%d %d]",
	      fwk_id_get_element_idx(dev_id), regu_name(ctx->rdev),
	      info->level_range.min_uv, info->level_range.max_uv);

    return FWK_SUCCESS;
}

static int stm32_regu_consumer_level_from_index(fwk_id_t dev_id,
						unsigned int index,
						int32_t *level_uv)
{
    struct stm32_regu_consumer_dev_ctx *ctx;
    uint16_t *levels;
    size_t full_count;
    int ret;

    ret = find_ctx(dev_id, &ctx);
    if (ret == FWK_E_ACCESS) {
        if (index > 0) {
            return FWK_E_RANGE;
        }
        *level_uv = 0;

	return FWK_SUCCESS;
    } else if (ret != FWK_SUCCESS) {
        return ret;
    }

    if (regulator_list_voltages(ctx->rdev, &levels, &full_count)) {
        return FWK_E_SUPPORT;
    }

    if (index >= full_count)
        return FWK_E_RANGE;

    *level_uv = (int32_t)levels[index] * 1000;

    DEBUG_MSG("SCMI voltd %u: get level PMIC %s = %"PRId32,
	      fwk_id_get_element_idx(dev_id), regu_name(ctx->rdev), *level_uv);

    return FWK_SUCCESS;
}

static const struct mod_voltd_drv_api api_optee_regu = {
    .get_level = stm32_regu_consumer_get_level,
    .set_level = stm32_regu_consumer_set_level,
    .set_config = stm32_regu_consumer_set_config,
    .get_config = stm32_regu_consumer_get_config,
    .get_info = stm32_regu_consumer_get_info,
    .get_level_from_index = stm32_regu_consumer_level_from_index,
};

/*
 * Framework handler functions
 */

static int stm32_regu_consumer_init(fwk_id_t module_id,
				    unsigned int element_count,
				    const void *data)
{
    module_ctx.dev_count = element_count;

    if (element_count)
        module_ctx.dev_ctx_table =
            fwk_mm_calloc(element_count, sizeof(*module_ctx.dev_ctx_table));

    return FWK_SUCCESS;
}

static int stm32_regu_consumer_element_init(fwk_id_t element_id,
					    unsigned int unused,
					    const void *data)
{
    const struct mod_stm32_regu_consumer_dev_config *dev_config = data;
    struct stm32_regu_consumer_dev_ctx *ctx;

    if (!fwk_module_is_valid_element_id(element_id))
        return FWK_E_PANIC;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(element_id);

    ctx->rdev = dev_config->rdev;
    ctx->enabled = dev_config->default_enabled;

    if (ctx->enabled) {
        if (!ctx->rdev)
            return FWK_E_PARAM;
        if (regulator_enable(ctx->rdev))
            return FWK_E_DEVICE;
    }

    return FWK_SUCCESS;
}

static int stm32_regu_consumer_process_bind_request(fwk_id_t requester_id,
						    fwk_id_t target_id,
						    fwk_id_t api_type,
						    const void **api)
{
    *api = &api_optee_regu;

    return FWK_SUCCESS;
}

const struct fwk_module module_stm32_regu_consumer = {
    .type = FWK_MODULE_TYPE_DRIVER,
    .api_count = 1,
    .init = stm32_regu_consumer_init,
    .element_init = stm32_regu_consumer_element_init,
    .process_bind_request = stm32_regu_consumer_process_bind_request,
};
