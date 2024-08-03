// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2018-2024, STMicroelectronics - All Rights Reserved
 * Copyright (c) 2017-2021, ARM Limited and Contributors. All rights reserved.
 */

#include <arm32.h>
#include <boot_api.h>
#include <crypto/crypto.h>
#include <drivers/clk.h>
#include <drivers/gic.h>
#include <drivers/stm32_rng.h>
#include <drivers/stm32_rtc.h>
#include <drivers/stm32mp_dt_bindings.h>
#include <drivers/stm32mp1_ddrc.h>
#include <generated/context_asm_defines.h>
#include <initcall.h>
#include <io.h>
#include <kernel/cache_helpers.h>
#include <kernel/delay.h>
#include <keep.h>
#include <kernel/panic.h>
#include <kernel/pm.h>
#include <mm/core_memprot.h>
#include <mm/mobj.h>
#include <platform_config.h>
#include <stdlib.h>
#include <stm32_util.h>
#include <stm32mp_pm.h>
#include <string.h>

#include "context.h"
#include "power.h"

#define TRAINING_AREA_SIZE		64

/*
 * STANDBY_CONTEXT_MAGIC0:
 * Context provides magic, resume entry, zq0cr0 zdata and DDR training buffer.
 *
 * STANDBY_CONTEXT_MAGIC1:
 * Context provides MAGIC0 content and PLL1 dual OPP settings structure
 * (86 bytes).
 *
 * STANDBY_CONTEXT_MAGIC2:
 * Context provides MAGIC1 content, low power entry point, BL2 code start, end
 * and BL2_END (102 bytes). And, only for STM32MP13, add MCE master key seed
 * (4 bytes).
 */
#define STANDBY_CONTEXT_MAGIC0		(0x0001 << 16)
#define STANDBY_CONTEXT_MAGIC1		(0x0002 << 16)
#define STANDBY_CONTEXT_MAGIC2		(0x0003 << 16)

#if CFG_STM32MP1_PM_CONTEXT_VERSION == 1
#define STANDBY_CONTEXT_MAGIC	(STANDBY_CONTEXT_MAGIC0 | TRAINING_AREA_SIZE)
#elif CFG_STM32MP1_PM_CONTEXT_VERSION == 2
#define STANDBY_CONTEXT_MAGIC	(STANDBY_CONTEXT_MAGIC1 | TRAINING_AREA_SIZE)
#elif CFG_STM32MP1_PM_CONTEXT_VERSION == 3
#define STANDBY_CONTEXT_MAGIC	(STANDBY_CONTEXT_MAGIC2 | TRAINING_AREA_SIZE)
#else
#error Invalid value for CFG_STM32MP1_PM_CONTEXT_VERSION
#endif

#if CFG_STM32MP1_PM_CONTEXT_VERSION >= 2
#if (PLAT_MAX_OPP_NB != 2) || (PLAT_MAX_PLLCFG_NB != 6)
#error STANDBY_CONTEXT_MAGIC does not support expected PLL1 settings
#endif

/* pll_settings structure size definitions (reference to clock driver) */
#define PLL1_SETTINGS_SIZE		(((PLAT_MAX_OPP_NB * \
					  (PLAT_MAX_PLLCFG_NB + 3)) + 1) * \
					 sizeof(uint32_t))
#endif

#if CFG_STM32MP1_PM_CONTEXT_VERSION >= 3 && defined(CFG_STM32MP13)
#define MCE_SEED_SIZE_IN_BYTES		4
#endif

/*
 * Context saved in TEE RAM during lower power sequence.
 * Can be allocated if to big for static allocation.
 *
 * @stgen_cnt_h: Upper 32bit of the STGEN counter
 * @stgen_cnt_l: Lower 32bit of the STGEN counter
 * @rtc: RTC time read at suspend
 */
struct pm_context {
	uint32_t stgen_cnt_h;
	uint32_t stgen_cnt_l;
	struct stm32_rtc_calendar rtc;
};

static struct pm_context plat_ctx;

/*
 * Clocks used during PM sequences. Stores clock references
 * saved at boot time for clk.h API functions.
 */
struct pm_clocks {
	struct clk *rtcapb;
	struct clk *bkpsram;
	struct clk *cryp1;
	struct clk *rtc;
};

static struct pm_clocks pm_clocks;

/*
 * BKPSRAM contains a mailbox used with early boot stages for resume sequence.
 * The mailbox content data that must be restored before OP-TEE is resumed.
 *
 * @magic: magic value read by early boot stage for consistency
 * @zq0cr0_zdata: DDRPHY configuration to be restored.
 * @ddr_training_backup: DDR area saved at suspend and backed up at resume
 */
struct pm_mailbox {
	uint32_t magic;
	uint32_t core0_resume_ep;
	uint32_t zq0cr0_zdata;
	uint8_t ddr_training_backup[TRAINING_AREA_SIZE];
#if CFG_STM32MP1_PM_CONTEXT_VERSION >= 2
	uint8_t pll1_settings[PLL1_SETTINGS_SIZE];
#endif
#if CFG_STM32MP1_PM_CONTEXT_VERSION >= 3
	uint32_t low_power_ep;
	uint32_t bl2_code_base;
	uint32_t bl2_code_end;
	uint32_t bl2_end;
#ifdef CFG_STM32MP13
	uint8_t mce_seed[MCE_SEED_SIZE_IN_BYTES];
#endif
#endif
};

/*
 * BKPSRAM contains OP-TEE resume instruction sequence which restores
 * TEE RAM content. The BKPSRAM contains restoration materials
 * (key, tag) and the resume entry point in restored TEE RAM.
 */
static struct retram_resume_ctx *get_retram_resume_ctx(void)
{
	vaddr_t bkpsram_base = stm32mp_bkpsram_base();
	vaddr_t context_base = bkpsram_base + BKPSRAM_PM_CONTEXT_OFFSET;

	return (struct retram_resume_ctx *)context_base;
}

static struct pm_mailbox *get_pm_mailbox(void)
{
	vaddr_t bkpsram_base = stm32mp_bkpsram_base();
	vaddr_t mailbox_base = bkpsram_base + BKPSRAM_PM_MAILBOX_OFFSET;

	return (struct pm_mailbox *)mailbox_base;
}

#if TRACE_LEVEL >= TRACE_DEBUG
static void __maybe_unused dump_context(void)
{
	struct pm_mailbox *mailbox = get_pm_mailbox();
	struct retram_resume_ctx *ctx = get_retram_resume_ctx();

	clk_enable(pm_clocks.rtcapb);

	DMSG("Backup registers: address 0x%" PRIx32 ", magic 0x%" PRIx32,
		*(uint32_t *)stm32mp_bkpreg(BCKR_CORE1_BRANCH_ADDRESS),
		*(uint32_t *)stm32mp_bkpreg(BCKR_CORE1_MAGIC_NUMBER));

	clk_disable(pm_clocks.rtcapb);

	clk_enable(pm_clocks.bkpsram);

	DMSG("BKPSRAM mailbox:  0x%" PRIx32 ", zd 0x%" PRIx32 ", ep 0x%" PRIx32,
		mailbox->magic, mailbox->zq0cr0_zdata,
		mailbox->core0_resume_ep);

	DMSG("BKPSRAM context:  teeram backup @%" PRIx32 ", resume @0x%" PRIx32,
		ctx->teeram_bkp_pa, ctx->resume_pa);

	clk_disable(pm_clocks.bkpsram);
}
#else
static void __maybe_unused dump_context(void)
{
}
#endif

/*
 * Save and restore functions
 */
static void save_time(void)
{
	vaddr_t stgen = stm32mp_stgen_base();

	plat_ctx.stgen_cnt_h = io_read32(stgen + CNTCVU_OFFSET);
	plat_ctx.stgen_cnt_l = io_read32(stgen + CNTCVL_OFFSET);
	if (plat_ctx.stgen_cnt_l < 10)
		plat_ctx.stgen_cnt_h = io_read32(stgen + CNTCVU_OFFSET);

	clk_enable(pm_clocks.rtc);

	if (stm32_rtc_get_calendar(&plat_ctx.rtc))
		panic();
}

#if TRACE_LEVEL >= TRACE_DEBUG
static void __maybe_unused print_ccm_decryption_duration(void)
{
	vaddr_t stgen = stm32mp_stgen_base();
	struct retram_resume_ctx *ctx = get_retram_resume_ctx();

	clk_enable(pm_clocks.bkpsram);

	DMSG("CCM decryption duration %llums",
		((unsigned long long)ctx->stgen_cnt * 1000) /
		io_read32(stgen + CNTFID_OFFSET));

	clk_disable(pm_clocks.bkpsram);
}
#else
static void __maybe_unused print_ccm_decryption_duration(void)
{
}
#endif

static void restore_time(void)
{
	struct stm32_rtc_calendar current_calendar = { };
	unsigned long long stdby_time_in_ms = 0;
	unsigned long long cnt = 0;
	vaddr_t stgen = stm32mp_stgen_base();
	struct retram_resume_ctx __maybe_unused *ctx = get_retram_resume_ctx();

	if (stm32_rtc_get_calendar(&current_calendar))
		panic();

	stdby_time_in_ms = stm32_rtc_diff_calendar_ms(&current_calendar,
						      &plat_ctx.rtc);

	cnt = ((uint64_t)plat_ctx.stgen_cnt_h << 32) | plat_ctx.stgen_cnt_l;
	cnt += (stdby_time_in_ms * io_read32(stgen + CNTFID_OFFSET)) / 1000U;

	io_clrbits32(stgen + CNTCR_OFFSET, CNTCR_EN);
	io_write32(stgen + CNTCVL_OFFSET, (uint32_t)cnt);
	io_write32(stgen + CNTCVU_OFFSET, (uint32_t)(cnt >> 32));
	io_setbits32(stgen + CNTCR_OFFSET, CNTCR_EN);

	/* Balance clock enable(RTC) at save_time() */
	clk_disable(pm_clocks.rtc);

#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	print_ccm_decryption_duration();
#endif
}

static bool __maybe_unused pm_cb_is_valid(void (*cb)(enum pm_op op, void *hdl),
					  void *hdl)
{
	void *cb_voidp = (void *)(vaddr_t)cb;
	paddr_t cb_phy = virt_to_phys(cb_voidp);
	paddr_t hdl_phy = virt_to_phys(hdl);
	bool valid = false;

	valid = (phys_to_virt(cb_phy, MEM_AREA_TEE_RAM_RX, 1) == cb_voidp) &&
		((phys_to_virt(hdl_phy, MEM_AREA_TEE_RAM_RX, 1) == hdl) ||
		 (phys_to_virt(hdl_phy, MEM_AREA_TEE_RAM_RO, 1) == hdl) ||
		 (phys_to_virt(hdl_phy, MEM_AREA_TEE_RAM_RW, 1) == hdl));

	if (!valid)
		EMSG("pm_cb mandates unpaged arguments %p %p", cb_voidp, hdl);

	return valid;
}

uintptr_t stm32mp_pm_retram_resume_ep(void)
{
	struct retram_resume_ctx *ctx = get_retram_resume_ctx();

	return (uintptr_t)&ctx->resume_sequence;
}

/* Clear the content of the PM mailbox */
void stm32mp_pm_wipe_context(void)
{
	struct retram_resume_ctx *ctx = get_retram_resume_ctx();
	struct pm_mailbox __maybe_unused *mailbox = get_pm_mailbox();

	clk_enable(pm_clocks.bkpsram);

	memset(ctx, 0xa5, sizeof(*ctx));
#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	memset(mailbox, 0xa5, sizeof(*mailbox));
#endif

	clk_disable(pm_clocks.bkpsram);
}

static struct mobj *teeram_bkp_mobj;

static void init_retram_resume_resources(void)
{
	struct retram_resume_ctx *ctx = get_retram_resume_ctx();
	size_t __maybe_unused csize = 0;
	paddr_t __maybe_unused pa = 0;

	COMPILE_TIME_ASSERT(sizeof(struct pm_mailbox) <
			    BKPSRAM_PM_MAILBOX_SIZE);
	COMPILE_TIME_ASSERT(sizeof(struct retram_resume_ctx) <
			    BKPSRAM_PM_CONTEXT_SIZE);
	csize = (vaddr_t)stm32mp_bkpsram_image_end -
		(vaddr_t)stm32mp_bkpsram_resume;
	assert((sizeof(*ctx) + csize) < BKPSRAM_PM_CONTEXT_SIZE);

	teeram_bkp_mobj = mobj_mm_alloc(mobj_sec_ddr, TEE_RAM_PH_SIZE,
					&tee_mm_sec_ddr);
	if (!teeram_bkp_mobj)
		panic();

	assert((mobj_get_va(teeram_bkp_mobj, 0, TEE_RAM_PH_SIZE) != NULL) &&
	       (mobj_get_pa(teeram_bkp_mobj, 0, 0, &pa) == 0));

	clk_enable(pm_clocks.bkpsram);
	memset(ctx, 0, sizeof(*ctx));
	clk_disable(pm_clocks.bkpsram);
}

/*
 * When returning from STANDBY, the 64 first bytes of DDR will be overwritten
 * during DDR DQS training. This area must then be saved before going to
 * standby in the PM mailbox with the earlier boot stages.
 */
__maybe_unused static void save_ddr_training_area(void)
{
	struct pm_mailbox *mailbox = get_pm_mailbox();
	size_t size = sizeof(mailbox->ddr_training_backup);
	paddr_t pa = DDR_BASE;
	void *va = phys_to_virt(pa, MEM_AREA_RAM_NSEC, SMALL_PAGE_SIZE);

	assert(va);

	memcpy(&mailbox->ddr_training_backup[0], va, size);
}

/*
 * When returning from STANDBY, warm boot boot stage needs to access to PLL1
 * settings. This avoids to re-compute them and optimizes performances. This
 * structure must then be saved before going to STANDBY in the PM mailbox
 * shared with the warm boot boot stage.
 */
#if (CFG_STM32MP1_PM_CONTEXT_VERSION >= 2) && \
    defined(CFG_STM32MP1_OPTEE_IN_SYSRAM)
__maybe_unused static void save_pll1_settings(void)
{
	struct pm_mailbox *mailbox = get_pm_mailbox();
	size_t size = sizeof(mailbox->pll1_settings);
	uint8_t *data = &mailbox->pll1_settings[0];

	stm32mp1_clk_lp_save_opp_pll1_settings(data, size);
}
#endif

static void load_earlyboot_pm_mailbox(void)
{
	struct pm_mailbox *mailbox __maybe_unused = get_pm_mailbox();

	COMPILE_TIME_ASSERT(sizeof(struct pm_mailbox) <
			    BKPSRAM_PM_MAILBOX_SIZE);

	assert(clk_is_enabled(pm_clocks.bkpsram));

#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	memset(mailbox, 0, sizeof(*mailbox));

	mailbox->zq0cr0_zdata = get_ddrphy_calibration();

#if CFG_STM32MP1_PM_CONTEXT_VERSION >= 2
	save_pll1_settings();
#endif
#endif /* CFG_STM32MP1_OPTEE_IN_SYSRAM */

	save_ddr_training_area();
}

#if defined(CFG_STM32_CRYP) && defined(CFG_STM32MP1_OPTEE_IN_SYSRAM)
/*
 * CRYP relies on standard format for CCM IV/B0/CRT0 data. Our sequence uses
 * no AAD, 4 bytes to encode the payload byte size and a 11 byte nonce.
 */
#define PM_CCM_Q			4
#define PM_CCM_Q_FLAGS			(PM_CCM_Q - 1)
#define PM_CCM_TAG_LEN			16
#define PM_CCM_TAG_FLAGS		(((PM_CCM_TAG_LEN - 2) / 2) << 3)

static void save_teeram_in_ddr(void)
{
	struct retram_resume_ctx *ctx = get_retram_resume_ctx();
	size_t __maybe_unused size = (vaddr_t)stm32mp_bkpsram_image_end -
				     (vaddr_t)stm32mp_bkpsram_resume;
	paddr_t pa = 0;
	struct ccm_unpg_ctx *ccm = &ctx->ccm_ctx;
	void *teeram = phys_to_virt(TEE_RAM_START, MEM_AREA_ROM_SEC, TEE_RAM_PH_SIZE);
	void *teeram_bkp = mobj_get_va(teeram_bkp_mobj, 0, TEE_RAM_PH_SIZE);

	COMPILE_TIME_ASSERT(PM_CTX_CCM_KEY_SIZE == sizeof(ccm->key));
	COMPILE_TIME_ASSERT(PM_CTX_CCM_CTR1_SIZE == sizeof(ccm->ctr1));
	COMPILE_TIME_ASSERT(PM_CTX_CCM_B0_SIZE == sizeof(ccm->b0));
	COMPILE_TIME_ASSERT(PM_CTX_CCM_CTR0_SIZE == sizeof(ccm->ctr0));
	COMPILE_TIME_ASSERT(PM_CTX_CCM_TAG_SIZE == sizeof(ccm->tag));

	if (!stm32mp_supports_hw_cryp())
		panic();

	if (IS_ENABLED(CFG_WITH_SOFTWARE_PRNG)) {
		/*
		 * Re-init STM32 RNG1 (including reset cycle) since non-secure
		 * world may have loaded a weak configuration.
		 */
		if (stm32_rng_init())
			panic();
	}

	assert(clk_is_enabled(pm_clocks.bkpsram) &&
	       clk_is_enabled(pm_clocks.cryp1));

	memcpy(ctx->resume_sequence,
	       (void *)(vaddr_t)stm32mp_bkpsram_resume, size);

	memset(ctx, 0, sizeof(*ctx));
	ctx->resume_pa = virt_to_phys((void *)(vaddr_t)stm32mp_sysram_resume);
	if (mobj_get_pa(teeram_bkp_mobj, 0, 0, &pa))
		panic();

	ctx->teeram_bkp_pa = (uint32_t)pa;
	ctx->cryp1_base = (uint32_t)phys_to_virt(CRYP1_BASE, MEM_AREA_IO_SEC, 1);
	ctx->rcc_base = (uint32_t)phys_to_virt(RCC_BASE, MEM_AREA_IO_SEC, 1);
	ctx->stgen_base = (uint32_t)phys_to_virt(STGEN_BASE, MEM_AREA_IO_SEC, 1);

	if (stm32_rng_read((uint8_t *)ccm->key, sizeof(ccm->key)))
		panic();

	assert(((PM_CCM_TAG_FLAGS & ~0x38U) | (PM_CCM_Q_FLAGS & ~0x07U)) == 0);
	COMPILE_TIME_ASSERT(PM_CCM_Q <= 4);
	COMPILE_TIME_ASSERT(TEE_RAM_PH_SIZE > UINT16_MAX);
	COMPILE_TIME_ASSERT(TEE_RAM_PH_SIZE < UINT32_MAX);

	if (stm32_rng_read((uint8_t *)ccm->ctr1, sizeof(ccm->ctr1)))
		panic();

	ccm->ctr1[0] &= GENMASK_32(24, 0);
	memcpy(ccm->b0, ccm->ctr1, sizeof(ccm->b0));
	memcpy(ccm->ctr0, ccm->ctr1, sizeof(ccm->ctr0));

	ccm->ctr0[0] |= PM_CCM_Q_FLAGS << 24;
	ccm->ctr0[3] = 0;
	ccm->ctr1[0] |= PM_CCM_Q_FLAGS << 24;
	ccm->ctr1[3] = 1;
	ccm->b0[0] |= (PM_CCM_Q_FLAGS | PM_CCM_TAG_FLAGS) << 24;
	ccm->b0[3] = TEE_RAM_PH_SIZE;

	stm32mp_ccm_encrypt_teeram(ctx, teeram_bkp, teeram, TEE_RAM_PH_SIZE);
	dcache_clean_range(teeram_bkp, TEE_RAM_PH_SIZE);

	memcpy(ctx->ccm_ref_tag, ccm->tag, sizeof(ctx->ccm_ref_tag));

	DMSG("CCM encryption duration %llums",
		((unsigned long long)ctx->stgen_cnt * 1000) /
		io_read32(ctx->stgen_base + CNTFID_OFFSET));
	ctx->stgen_cnt = 0;
}
#else
static void __maybe_unused save_teeram_in_ddr(void)
{
	panic("Mandates CRYP support");
}
#endif /* CFG_STM32_CRYP && CFG_STM32MP1_OPTEE_IN_SYSRAM */

/* Finalize the PM mailbox now that everything is loaded */
static void enable_pm_mailbox(unsigned int suspend)
{
	struct pm_mailbox *mailbox = get_pm_mailbox();
	uint32_t magic = 0;
	uint32_t hint = 0;

	assert(clk_is_enabled(pm_clocks.bkpsram) &&
	       clk_is_enabled(pm_clocks.rtcapb));

	if (suspend) {
		magic = BOOT_API_A7_CORE0_MAGIC_NUMBER;
		mailbox->magic = STANDBY_CONTEXT_MAGIC;

#ifndef CFG_STM32MP1_OPTEE_IN_SYSRAM
		hint = virt_to_phys(stm32mp_sysram_resume);
#else
		hint = virt_to_phys(&get_retram_resume_ctx()->resume_sequence);
#endif
	} else {
		mailbox->magic = 0;
	}

	io_write32(stm32mp_bkpreg(BCKR_CORE1_MAGIC_NUMBER), magic);
	io_write32(stm32mp_bkpreg(BCKR_CORE1_BRANCH_ADDRESS), hint);

	mailbox->core0_resume_ep = hint;
}

static void gate_pm_context_clocks(bool enable)
{
	static bool clocks_enabled;

	if (enable) {
		assert(!clocks_enabled);
		clk_enable(pm_clocks.bkpsram);
		clk_enable(pm_clocks.rtcapb);
		clk_enable(pm_clocks.cryp1);
		clocks_enabled = true;
		return;
	}

	/* Suspended TEE RAM state left the clocks enabled */
	if (clocks_enabled) {
		clk_disable(pm_clocks.bkpsram);
		clk_disable(pm_clocks.rtcapb);
		clk_disable(pm_clocks.cryp1);
		clocks_enabled = false;
	}
}

/*
 * Context (TEE RAM content + peripherals) must be restored
 * only if system may reach STANDBY state.
 */
TEE_Result stm32mp_pm_save_context(unsigned int soc_mode)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	save_time();

	res = pm_change_state(PM_OP_SUSPEND, soc_mode);
	if (res)
		return res;

	if (!need_to_backup_cpu_context(soc_mode)) {
#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
		if (need_to_backup_stop_context(soc_mode))
			stm32mp1_clk_save_context_for_stop();
#endif

		return TEE_SUCCESS;
	}

	gate_pm_context_clocks(true);
	load_earlyboot_pm_mailbox();

#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	save_teeram_in_ddr();
#endif
	enable_pm_mailbox(1);

	return TEE_SUCCESS;
}

void stm32mp_pm_restore_context(unsigned int soc_mode)
{
	if (need_to_backup_cpu_context(soc_mode))
		gate_pm_context_clocks(false);
#ifdef CFG_STM32MP1_OPTEE_IN_SYSRAM
	else if (need_to_backup_stop_context(soc_mode))
		stm32mp1_clk_restore_context_for_stop();
#endif

	if (pm_change_state(PM_OP_RESUME, 0))
		panic();

	restore_time();
}

void stm32mp_pm_shutdown_context(void)
{
	gate_pm_context_clocks(true);
	load_earlyboot_pm_mailbox();
	enable_pm_mailbox(0);
	gate_pm_context_clocks(false);
}

#ifndef CFG_STM32MP1_OPTEE_IN_SYSRAM
TEE_Result stm32mp_pm_call_bl2_lp_entry(unsigned int soc_mode)
{
	struct pm_mailbox *mailbox = NULL;
	void (*stm32_pwr_down_wfi)(bool is_cstop, unsigned int soc_mode) = NULL;

	clk_enable(pm_clocks.bkpsram);

	mailbox = get_pm_mailbox();

	FMSG("BL2 low_power_ep %#"PRIx32, mailbox->low_power_ep);

	stm32_pwr_down_wfi = (void *)mailbox->low_power_ep;

	dcache_op_all(DCACHE_OP_CLEAN_INV);

	/* Disable Cache & MMU before calling low_power section */
	write_sctlr(read_sctlr() & ~(SCTLR_C | SCTLR_M));

	dsb();
	isb();

	(*stm32_pwr_down_wfi)(true, soc_mode);

	/* Enable Cache & MMU */
	write_sctlr(read_sctlr() | SCTLR_C | SCTLR_M);

	clk_disable(pm_clocks.bkpsram);

	return TEE_SUCCESS;
}
#endif /* CFG_STM32MP1_OPTEE_IN_SYSRAM */

static TEE_Result init_pm_support(void)
{
	pm_clocks.rtcapb = stm32mp_rcc_clock_id_to_clk(RTCAPB);
	pm_clocks.bkpsram = stm32mp_rcc_clock_id_to_clk(BKPSRAM);
	pm_clocks.cryp1 = stm32mp_rcc_clock_id_to_clk(CRYP1);
	pm_clocks.rtc = stm32mp_rcc_clock_id_to_clk(RTC);

	if (!pm_clocks.rtcapb || !pm_clocks.bkpsram ||
	    !pm_clocks.cryp1 || !pm_clocks.rtc)
		panic();

	init_retram_resume_resources();

	stm32mp_pm_wipe_context();

	return TEE_SUCCESS;
}
driver_init(init_pm_support);
