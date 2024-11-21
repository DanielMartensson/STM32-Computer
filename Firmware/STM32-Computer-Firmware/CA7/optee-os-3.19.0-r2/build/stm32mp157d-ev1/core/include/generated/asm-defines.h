#ifndef _home_lubuntu_Dokument_GitHub_STM32_Computer_Firmware_STM32_Computer_Firmware_CA7_optee_os_3_19_0_r2_optee_os_stm32mp_3_19_0_stm32mp_r2____build_stm32mp157d_ev1_core_include_generated_asm_defines_h
#define _home_lubuntu_Dokument_GitHub_STM32_Computer_Firmware_STM32_Computer_Firmware_CA7_optee_os_3_19_0_r2_optee_os_stm32mp_3_19_0_stm32mp_r2____build_stm32mp157d_ev1_core_include_generated_asm_defines_h
#define SM_NSEC_CTX_R0	92	/* offsetof(struct sm_nsec_ctx, r0)"	*/
#define SM_NSEC_CTX_R8	72	/* offsetof(struct sm_nsec_ctx, r8)"	*/
#define SM_SEC_CTX_R0	72	/* offsetof(struct sm_sec_ctx, r0)"	*/
#define SM_SEC_CTX_MON_LR	104	/* offsetof(struct sm_sec_ctx, mon_lr)"	*/
#define SM_CTX_SEC_SIZE	112	/* sizeof(struct sm_sec_ctx)"	*/
#define SM_CTX_SIZE	248	/* sizeof(struct sm_ctx)"	*/
#define SM_CTX_NSEC	116	/* offsetof(struct sm_ctx, nsec)"	*/
#define SM_CTX_SEC	0	/* offsetof(struct sm_ctx, sec)"	*/
#define THREAD_SVC_REG_R0	4	/* offsetof(struct thread_svc_regs, r0)"	*/
#define THREAD_SVC_REG_R5	24	/* offsetof(struct thread_svc_regs, r5)"	*/
#define THREAD_SVC_REG_R6	28	/* offsetof(struct thread_svc_regs, r6)"	*/
#define THREAD_CTX_STACK_VA_END	84	/* offsetof(struct thread_ctx, stack_va_end)"	*/
#define THREAD_CTX_REGS_USR_SP	52	/* offsetof(struct thread_ctx_regs, usr_sp)"	*/
#define THREAD_CTX_REGS_PC	72	/* offsetof(struct thread_ctx_regs, pc)"	*/
#define THREAD_CTX_REGS_CPSR	76	/* offsetof(struct thread_ctx_regs, cpsr)"	*/
#define THREAD_CORE_LOCAL_R0	0	/* offsetof(struct thread_core_local, r[0])"	*/
#define THREAD_CORE_LOCAL_SM_PM_CTX_PHYS	8	/* offsetof(struct thread_core_local, sm_pm_ctx_phys)"	*/
#define THREAD_CORE_LOCAL_SIZE	32	/* sizeof(struct thread_core_local)"	*/
#define SM_PM_CTX_SIZE	80	/* sizeof(struct sm_pm_ctx)"	*/
#define THREAD_CTX_SIZE	784	/* sizeof(struct thread_ctx)"	*/
#define THREAD_CORE_LOCAL_TMP_STACK_VA_END	12	/* offsetof(struct thread_core_local, tmp_stack_va_end)"	*/
#define THREAD_CORE_LOCAL_CURR_THREAD	20	/* offsetof(struct thread_core_local, curr_thread)"	*/
#define THREAD_CORE_LOCAL_FLAGS	24	/* offsetof(struct thread_core_local, flags)"	*/
#define THREAD_CORE_LOCAL_ABT_STACK_VA_END	28	/* offsetof(struct thread_core_local, abt_stack_va_end)"	*/
#define STACK_TMP_GUARD	256	/* STACK_CANARY_SIZE */
#define CORE_MMU_CONFIG_SIZE	20	/* sizeof(struct core_mmu_config)"	*/
#define CORE_MMU_CONFIG_LOAD_OFFSET	16	/* offsetof(struct core_mmu_config, load_offset)"	*/
#define BOOT_EMBDATA_HASHES_OFFSET	8	/* offsetof(struct boot_embdata, hashes_offset)"	*/
#define BOOT_EMBDATA_HASHES_LEN	12	/* offsetof(struct boot_embdata, hashes_len)"	*/
#define BOOT_EMBDATA_RELOC_OFFSET	16	/* offsetof(struct boot_embdata, reloc_offset)"	*/
#define BOOT_EMBDATA_RELOC_LEN	20	/* offsetof(struct boot_embdata, reloc_len)"	*/
#endif
