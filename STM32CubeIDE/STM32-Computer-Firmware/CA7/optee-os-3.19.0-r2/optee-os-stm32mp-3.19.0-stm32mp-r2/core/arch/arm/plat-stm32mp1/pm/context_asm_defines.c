// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2018, STMicroelectronics
 * Copyright (c) 2018, Linaro Limited
 */

#include <gen-asm-defines.h>

#include "context.h"

#define OFFSET_OF_CTX_STRUCT(_f)	offsetof(struct retram_resume_ctx, _f)
#define OFFSET_OF_CMM_CTX_STRUCT(_f)	(OFFSET_OF_CTX_STRUCT(ccm_ctx) + \
					 offsetof(struct ccm_unpg_ctx, _f))
DEFINES
{
	DEFINE(PM_CTX_RESUME_PA, OFFSET_OF_CTX_STRUCT(resume_pa));
	DEFINE(PM_CTX_TEERAM_BKP_PA, OFFSET_OF_CTX_STRUCT(teeram_bkp_pa));
	DEFINE(PM_CTX_CRYP1_BASE, OFFSET_OF_CTX_STRUCT(cryp1_base));
	DEFINE(PM_CTX_RCC_BASE, OFFSET_OF_CTX_STRUCT(rcc_base));
	DEFINE(PM_CTX_STGEN_BASE, OFFSET_OF_CTX_STRUCT(stgen_base));
	DEFINE(PM_CTX_STGEN_CNT, OFFSET_OF_CTX_STRUCT(stgen_cnt));
	DEFINE(PM_CTX_CCM_KEY, OFFSET_OF_CMM_CTX_STRUCT(key));
	DEFINE(PM_CTX_CCM_CTR1, OFFSET_OF_CMM_CTX_STRUCT(ctr1));
	DEFINE(PM_CTX_CCM_B0, OFFSET_OF_CMM_CTX_STRUCT(b0));
	DEFINE(PM_CTX_CCM_CTR0, OFFSET_OF_CMM_CTX_STRUCT(ctr0));
	DEFINE(PM_CTX_CCM_TAG, OFFSET_OF_CMM_CTX_STRUCT(tag));
	DEFINE(PM_CTX_CCM_REF_TAG, OFFSET_OF_CTX_STRUCT(ccm_ref_tag));
}
