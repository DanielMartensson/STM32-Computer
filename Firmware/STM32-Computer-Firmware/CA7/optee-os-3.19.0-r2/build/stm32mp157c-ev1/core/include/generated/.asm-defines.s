	.cpu cortex-a7
	.arch armv7-a
	.arch_extension virt
	.arch_extension idiv
	.arch_extension sec
	.arch_extension mp
	.fpu softvfp
	.eabi_attribute 20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute 21, 1	@ Tag_ABI_FP_exceptions
	.eabi_attribute 23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute 24, 1	@ Tag_ABI_align8_needed
	.eabi_attribute 25, 1	@ Tag_ABI_align8_preserved
	.eabi_attribute 26, 2	@ Tag_ABI_enum_size
	.eabi_attribute 30, 4	@ Tag_ABI_optimization_goals
	.eabi_attribute 34, 0	@ Tag_CPU_unaligned_access
	.eabi_attribute 18, 4	@ Tag_ABI_PCS_wchar_t
	.file	"asm-defines.c"
@ GNU C11 (GCC) version 12.3.0 (arm-ostl-linux-gnueabi)
@	compiled by GNU C version 12.3.0, GMP version 6.2.1, MPFR version 4.2.1, MPC version 1.3.1, isl version none
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed: -mcpu=cortex-a7 -mfloat-abi=soft -mthumb -mno-unaligned-access -mtls-dialect=gnu -march=armv7ve -g3 -Os -std=gnu11 -ffunction-sections -fdata-sections -fno-short-enums -fno-common
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "core/arch/arm/kernel/asm-defines.c"
	.section	.text.__defines,"ax",%progbits
	.align	1
	.global	__defines
	.syntax unified
	.thumb
	.thumb_func
	.type	__defines, %function
__defines:
.LFB255:
	.loc 1 15 1 view -0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 17 2 view .LVU1
	.syntax unified
@ 17 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_NSEC_CTX_R0 92 offsetof(struct sm_nsec_ctx, r0)"	@
@ 0 "" 2
	.loc 1 17 58 view .LVU2
	.loc 1 18 2 view .LVU3
@ 18 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_NSEC_CTX_R8 72 offsetof(struct sm_nsec_ctx, r8)"	@
@ 0 "" 2
	.loc 1 18 58 view .LVU4
	.loc 1 19 2 view .LVU5
@ 19 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_SEC_CTX_R0 72 offsetof(struct sm_sec_ctx, r0)"	@
@ 0 "" 2
	.loc 1 19 56 view .LVU6
	.loc 1 20 2 view .LVU7
@ 20 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_SEC_CTX_MON_LR 104 offsetof(struct sm_sec_ctx, mon_lr)"	@
@ 0 "" 2
	.loc 1 20 64 view .LVU8
	.loc 1 21 2 view .LVU9
@ 21 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_CTX_SEC_SIZE 112 sizeof(struct sm_sec_ctx)"	@
@ 0 "" 2
	.loc 1 21 52 view .LVU10
	.loc 1 22 2 view .LVU11
@ 22 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_CTX_SIZE 248 sizeof(struct sm_ctx)"	@
@ 0 "" 2
	.loc 1 22 44 view .LVU12
	.loc 1 23 2 view .LVU13
@ 23 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_CTX_NSEC 116 offsetof(struct sm_ctx, nsec)"	@
@ 0 "" 2
	.loc 1 23 52 view .LVU14
	.loc 1 24 2 view .LVU15
@ 24 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_CTX_SEC 0 offsetof(struct sm_ctx, sec)"	@
@ 0 "" 2
	.loc 1 24 50 view .LVU16
	.loc 1 26 2 view .LVU17
@ 26 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_SVC_REG_R0 4 offsetof(struct thread_svc_regs, r0)"	@
@ 0 "" 2
	.loc 1 26 65 view .LVU18
	.loc 1 27 2 view .LVU19
@ 27 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_SVC_REG_R5 24 offsetof(struct thread_svc_regs, r5)"	@
@ 0 "" 2
	.loc 1 27 65 view .LVU20
	.loc 1 28 2 view .LVU21
@ 28 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_SVC_REG_R6 28 offsetof(struct thread_svc_regs, r6)"	@
@ 0 "" 2
	.loc 1 28 65 view .LVU22
	.loc 1 31 2 view .LVU23
@ 31 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CTX_STACK_VA_END 84 offsetof(struct thread_ctx, stack_va_end)"	@
@ 0 "" 2
	.loc 1 32 22 view .LVU24
	.loc 1 35 2 view .LVU25
@ 35 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CTX_REGS_USR_SP 52 offsetof(struct thread_ctx_regs, usr_sp)"	@
@ 0 "" 2
	.loc 1 36 50 view .LVU26
	.loc 1 37 2 view .LVU27
@ 37 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CTX_REGS_PC 72 offsetof(struct thread_ctx_regs, pc)"	@
@ 0 "" 2
	.loc 1 37 66 view .LVU28
	.loc 1 38 2 view .LVU29
@ 38 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CTX_REGS_CPSR 76 offsetof(struct thread_ctx_regs, cpsr)"	@
@ 0 "" 2
	.loc 1 38 70 view .LVU30
	.loc 1 41 2 view .LVU31
@ 41 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CORE_LOCAL_R0 0 offsetof(struct thread_core_local, r[0])"	@
@ 0 "" 2
	.loc 1 41 72 view .LVU32
	.loc 1 42 2 view .LVU33
@ 42 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CORE_LOCAL_SM_PM_CTX_PHYS 8 offsetof(struct thread_core_local, sm_pm_ctx_phys)"	@
@ 0 "" 2
	.loc 1 43 60 view .LVU34
	.loc 1 44 2 view .LVU35
@ 44 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CORE_LOCAL_SIZE 32 sizeof(struct thread_core_local)"	@
@ 0 "" 2
	.loc 1 44 66 view .LVU36
	.loc 1 46 2 view .LVU37
@ 46 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>SM_PM_CTX_SIZE 80 sizeof(struct sm_pm_ctx)"	@
@ 0 "" 2
	.loc 1 46 50 view .LVU38
	.loc 1 125 2 view .LVU39
@ 125 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CTX_SIZE 784 sizeof(struct thread_ctx)"	@
@ 0 "" 2
	.loc 1 125 52 view .LVU40
	.loc 1 132 2 view .LVU41
@ 132 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CORE_LOCAL_TMP_STACK_VA_END 12 offsetof(struct thread_core_local, tmp_stack_va_end)"	@
@ 0 "" 2
	.loc 1 133 56 view .LVU42
	.loc 1 134 2 view .LVU43
@ 134 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CORE_LOCAL_CURR_THREAD 20 offsetof(struct thread_core_local, curr_thread)"	@
@ 0 "" 2
	.loc 1 135 51 view .LVU44
	.loc 1 136 2 view .LVU45
@ 136 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CORE_LOCAL_FLAGS 24 offsetof(struct thread_core_local, flags)"	@
@ 0 "" 2
	.loc 1 137 45 view .LVU46
	.loc 1 138 2 view .LVU47
@ 138 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>THREAD_CORE_LOCAL_ABT_STACK_VA_END 28 offsetof(struct thread_core_local, abt_stack_va_end)"	@
@ 0 "" 2
	.loc 1 139 56 view .LVU48
	.loc 1 141 2 view .LVU49
@ 141 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>STACK_TMP_GUARD 256 STACK_CANARY_SIZE / 2 + STACK_TMP_OFFS"	@
@ 0 "" 2
	.loc 1 141 65 view .LVU50
	.loc 1 144 2 view .LVU51
@ 144 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>CORE_MMU_CONFIG_SIZE 20 sizeof(struct core_mmu_config)"	@
@ 0 "" 2
	.loc 1 144 62 view .LVU52
	.loc 1 145 2 view .LVU53
@ 145 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>CORE_MMU_CONFIG_LOAD_OFFSET 16 offsetof(struct core_mmu_config, load_offset)"	@
@ 0 "" 2
	.loc 1 146 55 view .LVU54
	.loc 1 149 2 view .LVU55
@ 149 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>BOOT_EMBDATA_HASHES_OFFSET 8 offsetof(struct boot_embdata, hashes_offset)"	@
@ 0 "" 2
	.loc 1 150 54 view .LVU56
	.loc 1 151 2 view .LVU57
@ 151 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>BOOT_EMBDATA_HASHES_LEN 12 offsetof(struct boot_embdata, hashes_len)"	@
@ 0 "" 2
	.loc 1 152 51 view .LVU58
	.loc 1 153 2 view .LVU59
@ 153 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>BOOT_EMBDATA_RELOC_OFFSET 16 offsetof(struct boot_embdata, reloc_offset)"	@
@ 0 "" 2
	.loc 1 154 53 view .LVU60
	.loc 1 155 2 view .LVU61
@ 155 "core/arch/arm/kernel/asm-defines.c" 1
	
.ascii "==>BOOT_EMBDATA_RELOC_LEN 20 offsetof(struct boot_embdata, reloc_len)"	@
@ 0 "" 2
	.loc 1 156 50 view .LVU62
@ core/arch/arm/kernel/asm-defines.c:157: }
	.loc 1 157 1 is_stmt 0 view .LVU63
	.thumb
	.syntax unified
	bx	lr	@
	.cfi_endproc
.LFE255:
	.size	__defines, .-__defines
	.text
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x94
	.2byte	0x5
	.byte	0x1
	.byte	0x4
	.4byte	.Ldebug_abbrev0
	.uleb128 0x2
	.4byte	.LASF2199
	.byte	0x1d
	.4byte	.LASF2200
	.4byte	.LASF2201
	.4byte	.LLRL0
	.4byte	0
	.4byte	.Ldebug_line0
	.4byte	.Ldebug_macro0
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.4byte	.LASF2187
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2188
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF2189
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF2190
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.4byte	.LASF2191
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF2192
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF2193
	.uleb128 0x1
	.byte	0x4
	.byte	0x5
	.4byte	.LASF2194
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF2195
	.uleb128 0x1
	.byte	0x8
	.byte	0x4
	.4byte	.LASF2196
	.uleb128 0x1
	.byte	0x1
	.byte	0x2
	.4byte	.LASF2197
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF2198
	.uleb128 0x4
	.4byte	.LASF2202
	.byte	0x1
	.byte	0xe
	.byte	0x1
	.4byte	.LFB255
	.4byte	.LFE255-.LFB255
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.uleb128 0x79
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB255
	.4byte	.LFE255-.LFB255
	.4byte	0
	.4byte	0
	.section	.debug_rnglists,"",%progbits
.Ldebug_ranges0:
	.4byte	.Ldebug_ranges3-.Ldebug_ranges2
.Ldebug_ranges2:
	.2byte	0x5
	.byte	0x4
	.byte	0
	.4byte	0
.LLRL0:
	.byte	0x7
	.4byte	.LFB255
	.uleb128 .LFE255-.LFB255
	.byte	0
.Ldebug_ranges3:
	.section	.debug_macro,"",%progbits
.Ldebug_macro0:
	.2byte	0x5
	.byte	0x2
	.4byte	.Ldebug_line0
	.byte	0x7
	.4byte	.Ldebug_macro2
	.byte	0x3
	.uleb128 0
	.uleb128 0x1
	.file 2 "/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/optee-os-3.19.0-r2/optee-os-stm32mp-3.19.0-stm32mp-r2/../build/stm32mp157c-ev1/include/generated/conf.h"
	.byte	0x3
	.uleb128 0
	.uleb128 0x2
	.byte	0x7
	.4byte	.Ldebug_macro3
	.byte	0x4
	.file 3 "core/include/gen-asm-defines.h"
	.byte	0x3
	.uleb128 0x6
	.uleb128 0x3
	.byte	0x7
	.4byte	.Ldebug_macro4
	.byte	0x4
	.file 4 "core/include/kernel/boot.h"
	.byte	0x3
	.uleb128 0x7
	.uleb128 0x4
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF645
	.file 5 "core/include/initcall.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x5
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF646
	.file 6 "core/include/scattered_array.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x6
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF647
	.file 7 "lib/libutils/ext/include/compiler.h"
	.byte	0x3
	.uleb128 0x8
	.uleb128 0x7
	.byte	0x7
	.4byte	.Ldebug_macro5
	.byte	0x4
	.file 8 "core/include/keep.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x8
	.byte	0x7
	.4byte	.Ldebug_macro6
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro7
	.byte	0x4
	.file 9 "lib/libutee/include/tee_api_types.h"
	.byte	0x3
	.uleb128 0xa
	.uleb128 0x9
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF710
	.file 10 "lib/libutils/isoc/include/inttypes.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0xa
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF711
	.file 11 "lib/libutils/isoc/include/stdint.h"
	.byte	0x3
	.uleb128 0x8
	.uleb128 0xb
	.file 12 "lib/libutils/isoc/include/limits.h"
	.byte	0x3
	.uleb128 0x6
	.uleb128 0xc
	.byte	0x7
	.4byte	.Ldebug_macro8
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro9
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro10
	.byte	0x4
	.file 13 "/home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stdbool.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0xd
	.byte	0x7
	.4byte	.Ldebug_macro11
	.byte	0x4
	.file 14 "/home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stddef.h"
	.byte	0x3
	.uleb128 0xd
	.uleb128 0xe
	.byte	0x7
	.4byte	.Ldebug_macro12
	.byte	0x4
	.file 15 "lib/libutee/include/tee_api_defines.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0xf
	.byte	0x7
	.4byte	.Ldebug_macro13
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro14
	.byte	0x4
	.file 16 "lib/libutils/ext/include/trace.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x10
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1279
	.file 17 "/home/lubuntu/Program/STM32/SDK/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stdarg.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x11
	.byte	0x7
	.4byte	.Ldebug_macro15
	.byte	0x4
	.byte	0x3
	.uleb128 0xb
	.uleb128 0xe
	.byte	0x4
	.file 18 "lib/libutils/ext/include/trace_levels.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0x12
	.byte	0x7
	.4byte	.Ldebug_macro16
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro17
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro18
	.byte	0x4
	.file 19 "lib/libutils/ext/include/types_ext.h"
	.byte	0x3
	.uleb128 0xa
	.uleb128 0x13
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1339
	.byte	0x3
	.uleb128 0x9
	.uleb128 0xe
	.byte	0x4
	.file 20 "lib/libutils/isoc/include/unistd.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0x14
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1340
	.byte	0x3
	.uleb128 0x8
	.uleb128 0xb
	.byte	0x4
	.byte	0x3
	.uleb128 0x9
	.uleb128 0xe
	.byte	0x4
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF1341
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro19
	.byte	0x4
	.byte	0x4
	.file 21 "core/include/kernel/thread.h"
	.byte	0x3
	.uleb128 0x8
	.uleb128 0x15
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF1349
	.file 22 "core/include/mm/pgt_cache.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0x16
	.byte	0x7
	.4byte	.Ldebug_macro20
	.file 23 "lib/libutils/isoc/include/assert.h"
	.byte	0x3
	.uleb128 0x10
	.uleb128 0x17
	.byte	0x7
	.4byte	.Ldebug_macro21
	.byte	0x4
	.file 24 "core/include/kernel/tee_ta_manager.h"
	.byte	0x3
	.uleb128 0x11
	.uleb128 0x18
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF1357
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x17
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1356
	.byte	0x4
	.file 25 "core/include/kernel/mutex.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0x19
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1358
	.file 26 "core/include/kernel/refcount.h"
	.byte	0x3
	.uleb128 0x8
	.uleb128 0x1a
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1359
	.file 27 "lib/libutils/ext/include/atomic.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x1b
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1360
	.byte	0x4
	.byte	0x4
	.file 28 "core/include/kernel/wait_queue.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x1c
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1361
	.file 29 "lib/libutils/isoc/include/sys/queue.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x1d
	.byte	0x7
	.4byte	.Ldebug_macro22
	.byte	0x4
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF1469
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro23
	.byte	0x4
	.file 30 "core/include/kernel/tee_common.h"
	.byte	0x3
	.uleb128 0xd
	.uleb128 0x1e
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1473
	.file 31 "lib/libutils/isoc/include/stdlib.h"
	.byte	0x3
	.uleb128 0x8
	.uleb128 0x1f
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF1474
	.byte	0x3
	.uleb128 0xf
	.uleb128 0xe
	.byte	0x4
	.file 32 "lib/libutils/isoc/include/malloc.h"
	.byte	0x3
	.uleb128 0x10
	.uleb128 0x20
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1475
	.byte	0x3
	.uleb128 0x8
	.uleb128 0xe
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro24
	.byte	0x4
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF1484
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro25
	.byte	0x4
	.file 33 "core/include/kernel/ts_manager.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0x21
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF1489
	.byte	0x4
	.file 34 "core/include/mm/tee_mmu_types.h"
	.byte	0x3
	.uleb128 0xf
	.uleb128 0x22
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1490
	.byte	0x3
	.uleb128 0xa
	.uleb128 0xb
	.byte	0x4
	.file 35 "lib/libutils/ext/include/util.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0x23
	.byte	0x7
	.4byte	.Ldebug_macro26
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro27
	.byte	0x4
	.file 36 "lib/libutee/include/user_ta_header.h"
	.byte	0x3
	.uleb128 0x13
	.uleb128 0x24
	.byte	0x7
	.4byte	.Ldebug_macro28
	.byte	0x4
	.file 37 "lib/libutee/include/utee_types.h"
	.byte	0x3
	.uleb128 0x14
	.uleb128 0x25
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1579
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro29
	.byte	0x4
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1582
	.byte	0x4
	.file 38 "core/arch/arm/include/kernel/thread_arch.h"
	.byte	0x3
	.uleb128 0x11
	.uleb128 0x26
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF1583
	.file 39 "core/arch/arm/include/arm.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0x27
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1584
	.byte	0x3
	.uleb128 0xa
	.uleb128 0xb
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro30
	.file 40 "core/arch/arm/include/arm32.h"
	.byte	0x3
	.uleb128 0x7d
	.uleb128 0x28
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1674
	.file 41 "lib/libutils/isoc/include/sys/cdefs.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x29
	.byte	0x7
	.4byte	.Ldebug_macro31
	.byte	0x4
	.byte	0x3
	.uleb128 0xc
	.uleb128 0xb
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro32
	.file 42 "/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/optee-os-3.19.0-r2/optee-os-stm32mp-3.19.0-stm32mp-r2/../build/stm32mp157c-ev1/core/include/generated/arm32_sysreg.h"
	.byte	0x3
	.uleb128 0xb0
	.uleb128 0x2a
	.byte	0x5
	.uleb128 0x3
	.4byte	.LASF1792
	.byte	0x4
	.byte	0x4
	.byte	0x4
	.file 43 "core/arch/arm/include/kernel/vfp.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0x2b
	.byte	0x7
	.4byte	.Ldebug_macro33
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro34
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro35
	.byte	0x4
	.file 44 "core/include/kernel/thread_private.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x2c
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1807
	.file 45 "core/arch/arm/include/kernel/thread_private_arch.h"
	.byte	0x3
	.uleb128 0xf
	.uleb128 0x2d
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1808
	.file 46 "core/arch/arm/include/sm/sm.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0x2e
	.byte	0x7
	.4byte	.Ldebug_macro36
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro37
	.byte	0x4
	.file 47 "core/include/mm/core_mmu.h"
	.byte	0x3
	.uleb128 0x10
	.uleb128 0x2f
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1819
	.byte	0x3
	.uleb128 0xa
	.uleb128 0x17
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1356
	.byte	0x4
	.file 48 "core/include/kernel/user_ta.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0x30
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1820
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x17
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1356
	.byte	0x4
	.file 49 "core/include/kernel/user_mode_ctx_struct.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x31
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1821
	.byte	0x4
	.file 50 "core/include/mm/file.h"
	.byte	0x3
	.uleb128 0xd
	.uleb128 0x32
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1822
	.file 51 "lib/libutee/include/utee_defines.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x33
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1823
	.file 52 "lib/libutee/include/tee_api_defines_extensions.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x34
	.byte	0x7
	.4byte	.Ldebug_macro38
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro39
	.byte	0x4
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF1944
	.byte	0x4
	.file 53 "core/include/mm/tee_mm.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0x35
	.byte	0x7
	.4byte	.Ldebug_macro40
	.byte	0x4
	.byte	0x4
	.file 54 "core/arch/arm/include/mm/core_mmu_arch.h"
	.byte	0x3
	.uleb128 0x12
	.uleb128 0x36
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1949
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x17
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1356
	.byte	0x4
	.file 55 "lib/libutils/ext/include/config.h"
	.byte	0x3
	.uleb128 0xd
	.uleb128 0x37
	.byte	0x7
	.4byte	.Ldebug_macro41
	.byte	0x4
	.file 56 "core/arch/arm/plat-stm32mp1/./platform_config.h"
	.byte	0x3
	.uleb128 0x14
	.uleb128 0x38
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1957
	.file 57 "core/arch/arm/include/mm/generic_ram_layout.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x39
	.byte	0x7
	.4byte	.Ldebug_macro42
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro43
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro44
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro45
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro46
	.byte	0x4
	.file 58 "core/arch/arm/include/sm/pm.h"
	.byte	0x3
	.uleb128 0xa
	.uleb128 0x3a
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF2186
	.byte	0x3
	.uleb128 0x1f
	.uleb128 0xb
	.byte	0x4
	.byte	0x4
	.byte	0x4
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.0.430fb3e4b70e492b6686f7eca0c26a65,comdat
.Ldebug_macro2:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF1
	.byte	0x5
	.uleb128 0
	.4byte	.LASF2
	.byte	0x5
	.uleb128 0
	.4byte	.LASF3
	.byte	0x5
	.uleb128 0
	.4byte	.LASF4
	.byte	0x5
	.uleb128 0
	.4byte	.LASF5
	.byte	0x5
	.uleb128 0
	.4byte	.LASF6
	.byte	0x5
	.uleb128 0
	.4byte	.LASF7
	.byte	0x5
	.uleb128 0
	.4byte	.LASF8
	.byte	0x5
	.uleb128 0
	.4byte	.LASF9
	.byte	0x5
	.uleb128 0
	.4byte	.LASF10
	.byte	0x5
	.uleb128 0
	.4byte	.LASF11
	.byte	0x5
	.uleb128 0
	.4byte	.LASF12
	.byte	0x5
	.uleb128 0
	.4byte	.LASF13
	.byte	0x5
	.uleb128 0
	.4byte	.LASF14
	.byte	0x5
	.uleb128 0
	.4byte	.LASF15
	.byte	0x5
	.uleb128 0
	.4byte	.LASF16
	.byte	0x5
	.uleb128 0
	.4byte	.LASF17
	.byte	0x5
	.uleb128 0
	.4byte	.LASF18
	.byte	0x5
	.uleb128 0
	.4byte	.LASF19
	.byte	0x5
	.uleb128 0
	.4byte	.LASF20
	.byte	0x5
	.uleb128 0
	.4byte	.LASF21
	.byte	0x5
	.uleb128 0
	.4byte	.LASF22
	.byte	0x5
	.uleb128 0
	.4byte	.LASF23
	.byte	0x5
	.uleb128 0
	.4byte	.LASF24
	.byte	0x5
	.uleb128 0
	.4byte	.LASF25
	.byte	0x5
	.uleb128 0
	.4byte	.LASF26
	.byte	0x5
	.uleb128 0
	.4byte	.LASF27
	.byte	0x5
	.uleb128 0
	.4byte	.LASF28
	.byte	0x5
	.uleb128 0
	.4byte	.LASF29
	.byte	0x5
	.uleb128 0
	.4byte	.LASF30
	.byte	0x5
	.uleb128 0
	.4byte	.LASF31
	.byte	0x5
	.uleb128 0
	.4byte	.LASF32
	.byte	0x5
	.uleb128 0
	.4byte	.LASF33
	.byte	0x5
	.uleb128 0
	.4byte	.LASF34
	.byte	0x5
	.uleb128 0
	.4byte	.LASF35
	.byte	0x5
	.uleb128 0
	.4byte	.LASF36
	.byte	0x5
	.uleb128 0
	.4byte	.LASF37
	.byte	0x5
	.uleb128 0
	.4byte	.LASF38
	.byte	0x5
	.uleb128 0
	.4byte	.LASF39
	.byte	0x5
	.uleb128 0
	.4byte	.LASF40
	.byte	0x5
	.uleb128 0
	.4byte	.LASF41
	.byte	0x5
	.uleb128 0
	.4byte	.LASF42
	.byte	0x5
	.uleb128 0
	.4byte	.LASF43
	.byte	0x5
	.uleb128 0
	.4byte	.LASF44
	.byte	0x5
	.uleb128 0
	.4byte	.LASF45
	.byte	0x5
	.uleb128 0
	.4byte	.LASF46
	.byte	0x5
	.uleb128 0
	.4byte	.LASF47
	.byte	0x5
	.uleb128 0
	.4byte	.LASF48
	.byte	0x5
	.uleb128 0
	.4byte	.LASF49
	.byte	0x5
	.uleb128 0
	.4byte	.LASF50
	.byte	0x5
	.uleb128 0
	.4byte	.LASF51
	.byte	0x5
	.uleb128 0
	.4byte	.LASF52
	.byte	0x5
	.uleb128 0
	.4byte	.LASF53
	.byte	0x5
	.uleb128 0
	.4byte	.LASF54
	.byte	0x5
	.uleb128 0
	.4byte	.LASF55
	.byte	0x5
	.uleb128 0
	.4byte	.LASF56
	.byte	0x5
	.uleb128 0
	.4byte	.LASF57
	.byte	0x5
	.uleb128 0
	.4byte	.LASF58
	.byte	0x5
	.uleb128 0
	.4byte	.LASF59
	.byte	0x5
	.uleb128 0
	.4byte	.LASF60
	.byte	0x5
	.uleb128 0
	.4byte	.LASF61
	.byte	0x5
	.uleb128 0
	.4byte	.LASF62
	.byte	0x5
	.uleb128 0
	.4byte	.LASF63
	.byte	0x5
	.uleb128 0
	.4byte	.LASF64
	.byte	0x5
	.uleb128 0
	.4byte	.LASF65
	.byte	0x5
	.uleb128 0
	.4byte	.LASF66
	.byte	0x5
	.uleb128 0
	.4byte	.LASF67
	.byte	0x5
	.uleb128 0
	.4byte	.LASF68
	.byte	0x5
	.uleb128 0
	.4byte	.LASF69
	.byte	0x5
	.uleb128 0
	.4byte	.LASF70
	.byte	0x5
	.uleb128 0
	.4byte	.LASF71
	.byte	0x5
	.uleb128 0
	.4byte	.LASF72
	.byte	0x5
	.uleb128 0
	.4byte	.LASF73
	.byte	0x5
	.uleb128 0
	.4byte	.LASF74
	.byte	0x5
	.uleb128 0
	.4byte	.LASF75
	.byte	0x5
	.uleb128 0
	.4byte	.LASF76
	.byte	0x5
	.uleb128 0
	.4byte	.LASF77
	.byte	0x5
	.uleb128 0
	.4byte	.LASF78
	.byte	0x5
	.uleb128 0
	.4byte	.LASF79
	.byte	0x5
	.uleb128 0
	.4byte	.LASF80
	.byte	0x5
	.uleb128 0
	.4byte	.LASF81
	.byte	0x5
	.uleb128 0
	.4byte	.LASF82
	.byte	0x5
	.uleb128 0
	.4byte	.LASF83
	.byte	0x5
	.uleb128 0
	.4byte	.LASF84
	.byte	0x5
	.uleb128 0
	.4byte	.LASF85
	.byte	0x5
	.uleb128 0
	.4byte	.LASF86
	.byte	0x5
	.uleb128 0
	.4byte	.LASF87
	.byte	0x5
	.uleb128 0
	.4byte	.LASF88
	.byte	0x5
	.uleb128 0
	.4byte	.LASF89
	.byte	0x5
	.uleb128 0
	.4byte	.LASF90
	.byte	0x5
	.uleb128 0
	.4byte	.LASF91
	.byte	0x5
	.uleb128 0
	.4byte	.LASF92
	.byte	0x5
	.uleb128 0
	.4byte	.LASF93
	.byte	0x5
	.uleb128 0
	.4byte	.LASF94
	.byte	0x5
	.uleb128 0
	.4byte	.LASF95
	.byte	0x5
	.uleb128 0
	.4byte	.LASF96
	.byte	0x5
	.uleb128 0
	.4byte	.LASF97
	.byte	0x5
	.uleb128 0
	.4byte	.LASF98
	.byte	0x5
	.uleb128 0
	.4byte	.LASF99
	.byte	0x5
	.uleb128 0
	.4byte	.LASF100
	.byte	0x5
	.uleb128 0
	.4byte	.LASF101
	.byte	0x5
	.uleb128 0
	.4byte	.LASF102
	.byte	0x5
	.uleb128 0
	.4byte	.LASF103
	.byte	0x5
	.uleb128 0
	.4byte	.LASF104
	.byte	0x5
	.uleb128 0
	.4byte	.LASF105
	.byte	0x5
	.uleb128 0
	.4byte	.LASF106
	.byte	0x5
	.uleb128 0
	.4byte	.LASF107
	.byte	0x5
	.uleb128 0
	.4byte	.LASF108
	.byte	0x5
	.uleb128 0
	.4byte	.LASF109
	.byte	0x5
	.uleb128 0
	.4byte	.LASF110
	.byte	0x5
	.uleb128 0
	.4byte	.LASF111
	.byte	0x5
	.uleb128 0
	.4byte	.LASF112
	.byte	0x5
	.uleb128 0
	.4byte	.LASF113
	.byte	0x5
	.uleb128 0
	.4byte	.LASF114
	.byte	0x5
	.uleb128 0
	.4byte	.LASF115
	.byte	0x5
	.uleb128 0
	.4byte	.LASF116
	.byte	0x5
	.uleb128 0
	.4byte	.LASF117
	.byte	0x5
	.uleb128 0
	.4byte	.LASF118
	.byte	0x5
	.uleb128 0
	.4byte	.LASF119
	.byte	0x5
	.uleb128 0
	.4byte	.LASF120
	.byte	0x5
	.uleb128 0
	.4byte	.LASF121
	.byte	0x5
	.uleb128 0
	.4byte	.LASF122
	.byte	0x5
	.uleb128 0
	.4byte	.LASF123
	.byte	0x5
	.uleb128 0
	.4byte	.LASF124
	.byte	0x5
	.uleb128 0
	.4byte	.LASF125
	.byte	0x5
	.uleb128 0
	.4byte	.LASF126
	.byte	0x5
	.uleb128 0
	.4byte	.LASF127
	.byte	0x5
	.uleb128 0
	.4byte	.LASF128
	.byte	0x5
	.uleb128 0
	.4byte	.LASF129
	.byte	0x5
	.uleb128 0
	.4byte	.LASF130
	.byte	0x5
	.uleb128 0
	.4byte	.LASF131
	.byte	0x5
	.uleb128 0
	.4byte	.LASF132
	.byte	0x5
	.uleb128 0
	.4byte	.LASF133
	.byte	0x5
	.uleb128 0
	.4byte	.LASF134
	.byte	0x5
	.uleb128 0
	.4byte	.LASF135
	.byte	0x5
	.uleb128 0
	.4byte	.LASF136
	.byte	0x5
	.uleb128 0
	.4byte	.LASF137
	.byte	0x5
	.uleb128 0
	.4byte	.LASF138
	.byte	0x5
	.uleb128 0
	.4byte	.LASF139
	.byte	0x5
	.uleb128 0
	.4byte	.LASF140
	.byte	0x5
	.uleb128 0
	.4byte	.LASF141
	.byte	0x5
	.uleb128 0
	.4byte	.LASF142
	.byte	0x5
	.uleb128 0
	.4byte	.LASF143
	.byte	0x5
	.uleb128 0
	.4byte	.LASF144
	.byte	0x5
	.uleb128 0
	.4byte	.LASF145
	.byte	0x5
	.uleb128 0
	.4byte	.LASF146
	.byte	0x5
	.uleb128 0
	.4byte	.LASF147
	.byte	0x5
	.uleb128 0
	.4byte	.LASF148
	.byte	0x5
	.uleb128 0
	.4byte	.LASF149
	.byte	0x5
	.uleb128 0
	.4byte	.LASF150
	.byte	0x5
	.uleb128 0
	.4byte	.LASF151
	.byte	0x5
	.uleb128 0
	.4byte	.LASF152
	.byte	0x5
	.uleb128 0
	.4byte	.LASF153
	.byte	0x5
	.uleb128 0
	.4byte	.LASF154
	.byte	0x5
	.uleb128 0
	.4byte	.LASF155
	.byte	0x5
	.uleb128 0
	.4byte	.LASF156
	.byte	0x5
	.uleb128 0
	.4byte	.LASF157
	.byte	0x5
	.uleb128 0
	.4byte	.LASF158
	.byte	0x5
	.uleb128 0
	.4byte	.LASF159
	.byte	0x5
	.uleb128 0
	.4byte	.LASF160
	.byte	0x5
	.uleb128 0
	.4byte	.LASF161
	.byte	0x5
	.uleb128 0
	.4byte	.LASF162
	.byte	0x5
	.uleb128 0
	.4byte	.LASF163
	.byte	0x5
	.uleb128 0
	.4byte	.LASF164
	.byte	0x5
	.uleb128 0
	.4byte	.LASF165
	.byte	0x5
	.uleb128 0
	.4byte	.LASF166
	.byte	0x5
	.uleb128 0
	.4byte	.LASF167
	.byte	0x5
	.uleb128 0
	.4byte	.LASF168
	.byte	0x5
	.uleb128 0
	.4byte	.LASF169
	.byte	0x5
	.uleb128 0
	.4byte	.LASF170
	.byte	0x5
	.uleb128 0
	.4byte	.LASF171
	.byte	0x5
	.uleb128 0
	.4byte	.LASF172
	.byte	0x5
	.uleb128 0
	.4byte	.LASF173
	.byte	0x5
	.uleb128 0
	.4byte	.LASF174
	.byte	0x5
	.uleb128 0
	.4byte	.LASF175
	.byte	0x5
	.uleb128 0
	.4byte	.LASF176
	.byte	0x5
	.uleb128 0
	.4byte	.LASF177
	.byte	0x5
	.uleb128 0
	.4byte	.LASF178
	.byte	0x5
	.uleb128 0
	.4byte	.LASF179
	.byte	0x5
	.uleb128 0
	.4byte	.LASF180
	.byte	0x5
	.uleb128 0
	.4byte	.LASF181
	.byte	0x5
	.uleb128 0
	.4byte	.LASF182
	.byte	0x5
	.uleb128 0
	.4byte	.LASF183
	.byte	0x5
	.uleb128 0
	.4byte	.LASF184
	.byte	0x5
	.uleb128 0
	.4byte	.LASF185
	.byte	0x5
	.uleb128 0
	.4byte	.LASF186
	.byte	0x5
	.uleb128 0
	.4byte	.LASF187
	.byte	0x5
	.uleb128 0
	.4byte	.LASF188
	.byte	0x5
	.uleb128 0
	.4byte	.LASF189
	.byte	0x5
	.uleb128 0
	.4byte	.LASF190
	.byte	0x5
	.uleb128 0
	.4byte	.LASF191
	.byte	0x5
	.uleb128 0
	.4byte	.LASF192
	.byte	0x5
	.uleb128 0
	.4byte	.LASF193
	.byte	0x5
	.uleb128 0
	.4byte	.LASF194
	.byte	0x5
	.uleb128 0
	.4byte	.LASF195
	.byte	0x5
	.uleb128 0
	.4byte	.LASF196
	.byte	0x5
	.uleb128 0
	.4byte	.LASF197
	.byte	0x5
	.uleb128 0
	.4byte	.LASF198
	.byte	0x5
	.uleb128 0
	.4byte	.LASF199
	.byte	0x5
	.uleb128 0
	.4byte	.LASF200
	.byte	0x5
	.uleb128 0
	.4byte	.LASF201
	.byte	0x5
	.uleb128 0
	.4byte	.LASF202
	.byte	0x5
	.uleb128 0
	.4byte	.LASF203
	.byte	0x5
	.uleb128 0
	.4byte	.LASF204
	.byte	0x5
	.uleb128 0
	.4byte	.LASF205
	.byte	0x5
	.uleb128 0
	.4byte	.LASF206
	.byte	0x5
	.uleb128 0
	.4byte	.LASF207
	.byte	0x5
	.uleb128 0
	.4byte	.LASF208
	.byte	0x5
	.uleb128 0
	.4byte	.LASF209
	.byte	0x5
	.uleb128 0
	.4byte	.LASF210
	.byte	0x5
	.uleb128 0
	.4byte	.LASF211
	.byte	0x5
	.uleb128 0
	.4byte	.LASF212
	.byte	0x5
	.uleb128 0
	.4byte	.LASF213
	.byte	0x5
	.uleb128 0
	.4byte	.LASF214
	.byte	0x5
	.uleb128 0
	.4byte	.LASF215
	.byte	0x5
	.uleb128 0
	.4byte	.LASF216
	.byte	0x5
	.uleb128 0
	.4byte	.LASF217
	.byte	0x5
	.uleb128 0
	.4byte	.LASF218
	.byte	0x5
	.uleb128 0
	.4byte	.LASF219
	.byte	0x5
	.uleb128 0
	.4byte	.LASF220
	.byte	0x5
	.uleb128 0
	.4byte	.LASF221
	.byte	0x5
	.uleb128 0
	.4byte	.LASF222
	.byte	0x5
	.uleb128 0
	.4byte	.LASF223
	.byte	0x5
	.uleb128 0
	.4byte	.LASF224
	.byte	0x5
	.uleb128 0
	.4byte	.LASF225
	.byte	0x5
	.uleb128 0
	.4byte	.LASF226
	.byte	0x5
	.uleb128 0
	.4byte	.LASF227
	.byte	0x5
	.uleb128 0
	.4byte	.LASF228
	.byte	0x5
	.uleb128 0
	.4byte	.LASF229
	.byte	0x5
	.uleb128 0
	.4byte	.LASF230
	.byte	0x5
	.uleb128 0
	.4byte	.LASF231
	.byte	0x5
	.uleb128 0
	.4byte	.LASF232
	.byte	0x5
	.uleb128 0
	.4byte	.LASF233
	.byte	0x5
	.uleb128 0
	.4byte	.LASF234
	.byte	0x5
	.uleb128 0
	.4byte	.LASF235
	.byte	0x5
	.uleb128 0
	.4byte	.LASF236
	.byte	0x5
	.uleb128 0
	.4byte	.LASF237
	.byte	0x5
	.uleb128 0
	.4byte	.LASF238
	.byte	0x5
	.uleb128 0
	.4byte	.LASF239
	.byte	0x5
	.uleb128 0
	.4byte	.LASF240
	.byte	0x5
	.uleb128 0
	.4byte	.LASF241
	.byte	0x5
	.uleb128 0
	.4byte	.LASF242
	.byte	0x5
	.uleb128 0
	.4byte	.LASF243
	.byte	0x5
	.uleb128 0
	.4byte	.LASF244
	.byte	0x5
	.uleb128 0
	.4byte	.LASF245
	.byte	0x5
	.uleb128 0
	.4byte	.LASF246
	.byte	0x5
	.uleb128 0
	.4byte	.LASF247
	.byte	0x5
	.uleb128 0
	.4byte	.LASF248
	.byte	0x5
	.uleb128 0
	.4byte	.LASF249
	.byte	0x5
	.uleb128 0
	.4byte	.LASF250
	.byte	0x5
	.uleb128 0
	.4byte	.LASF251
	.byte	0x5
	.uleb128 0
	.4byte	.LASF252
	.byte	0x5
	.uleb128 0
	.4byte	.LASF253
	.byte	0x5
	.uleb128 0
	.4byte	.LASF254
	.byte	0x5
	.uleb128 0
	.4byte	.LASF255
	.byte	0x5
	.uleb128 0
	.4byte	.LASF256
	.byte	0x5
	.uleb128 0
	.4byte	.LASF257
	.byte	0x5
	.uleb128 0
	.4byte	.LASF258
	.byte	0x5
	.uleb128 0
	.4byte	.LASF259
	.byte	0x5
	.uleb128 0
	.4byte	.LASF260
	.byte	0x5
	.uleb128 0
	.4byte	.LASF261
	.byte	0x5
	.uleb128 0
	.4byte	.LASF262
	.byte	0x5
	.uleb128 0
	.4byte	.LASF263
	.byte	0x5
	.uleb128 0
	.4byte	.LASF264
	.byte	0x5
	.uleb128 0
	.4byte	.LASF265
	.byte	0x5
	.uleb128 0
	.4byte	.LASF266
	.byte	0x5
	.uleb128 0
	.4byte	.LASF267
	.byte	0x5
	.uleb128 0
	.4byte	.LASF268
	.byte	0x5
	.uleb128 0
	.4byte	.LASF269
	.byte	0x5
	.uleb128 0
	.4byte	.LASF270
	.byte	0x5
	.uleb128 0
	.4byte	.LASF271
	.byte	0x5
	.uleb128 0
	.4byte	.LASF272
	.byte	0x5
	.uleb128 0
	.4byte	.LASF273
	.byte	0x5
	.uleb128 0
	.4byte	.LASF274
	.byte	0x5
	.uleb128 0
	.4byte	.LASF275
	.byte	0x5
	.uleb128 0
	.4byte	.LASF276
	.byte	0x5
	.uleb128 0
	.4byte	.LASF277
	.byte	0x5
	.uleb128 0
	.4byte	.LASF278
	.byte	0x5
	.uleb128 0
	.4byte	.LASF279
	.byte	0x5
	.uleb128 0
	.4byte	.LASF280
	.byte	0x5
	.uleb128 0
	.4byte	.LASF281
	.byte	0x5
	.uleb128 0
	.4byte	.LASF282
	.byte	0x5
	.uleb128 0
	.4byte	.LASF283
	.byte	0x5
	.uleb128 0
	.4byte	.LASF284
	.byte	0x5
	.uleb128 0
	.4byte	.LASF285
	.byte	0x5
	.uleb128 0
	.4byte	.LASF286
	.byte	0x5
	.uleb128 0
	.4byte	.LASF287
	.byte	0x5
	.uleb128 0
	.4byte	.LASF288
	.byte	0x5
	.uleb128 0
	.4byte	.LASF289
	.byte	0x5
	.uleb128 0
	.4byte	.LASF290
	.byte	0x5
	.uleb128 0
	.4byte	.LASF291
	.byte	0x5
	.uleb128 0
	.4byte	.LASF292
	.byte	0x5
	.uleb128 0
	.4byte	.LASF293
	.byte	0x5
	.uleb128 0
	.4byte	.LASF294
	.byte	0x5
	.uleb128 0
	.4byte	.LASF295
	.byte	0x5
	.uleb128 0
	.4byte	.LASF296
	.byte	0x5
	.uleb128 0
	.4byte	.LASF297
	.byte	0x5
	.uleb128 0
	.4byte	.LASF298
	.byte	0x5
	.uleb128 0
	.4byte	.LASF299
	.byte	0x5
	.uleb128 0
	.4byte	.LASF300
	.byte	0x5
	.uleb128 0
	.4byte	.LASF301
	.byte	0x5
	.uleb128 0
	.4byte	.LASF302
	.byte	0x5
	.uleb128 0
	.4byte	.LASF303
	.byte	0x5
	.uleb128 0
	.4byte	.LASF304
	.byte	0x5
	.uleb128 0
	.4byte	.LASF305
	.byte	0x5
	.uleb128 0
	.4byte	.LASF306
	.byte	0x5
	.uleb128 0
	.4byte	.LASF307
	.byte	0x5
	.uleb128 0
	.4byte	.LASF308
	.byte	0x5
	.uleb128 0
	.4byte	.LASF309
	.byte	0x5
	.uleb128 0
	.4byte	.LASF310
	.byte	0x5
	.uleb128 0
	.4byte	.LASF311
	.byte	0x5
	.uleb128 0
	.4byte	.LASF312
	.byte	0x5
	.uleb128 0
	.4byte	.LASF313
	.byte	0x5
	.uleb128 0
	.4byte	.LASF314
	.byte	0x5
	.uleb128 0
	.4byte	.LASF315
	.byte	0x5
	.uleb128 0
	.4byte	.LASF316
	.byte	0x5
	.uleb128 0
	.4byte	.LASF317
	.byte	0x5
	.uleb128 0
	.4byte	.LASF318
	.byte	0x5
	.uleb128 0
	.4byte	.LASF319
	.byte	0x5
	.uleb128 0
	.4byte	.LASF320
	.byte	0x5
	.uleb128 0
	.4byte	.LASF321
	.byte	0x5
	.uleb128 0
	.4byte	.LASF322
	.byte	0x5
	.uleb128 0
	.4byte	.LASF323
	.byte	0x5
	.uleb128 0
	.4byte	.LASF324
	.byte	0x5
	.uleb128 0
	.4byte	.LASF325
	.byte	0x5
	.uleb128 0
	.4byte	.LASF326
	.byte	0x5
	.uleb128 0
	.4byte	.LASF327
	.byte	0x5
	.uleb128 0
	.4byte	.LASF328
	.byte	0x5
	.uleb128 0
	.4byte	.LASF329
	.byte	0x5
	.uleb128 0
	.4byte	.LASF330
	.byte	0x5
	.uleb128 0
	.4byte	.LASF331
	.byte	0x5
	.uleb128 0
	.4byte	.LASF332
	.byte	0x5
	.uleb128 0
	.4byte	.LASF333
	.byte	0x5
	.uleb128 0
	.4byte	.LASF334
	.byte	0x5
	.uleb128 0
	.4byte	.LASF335
	.byte	0x5
	.uleb128 0
	.4byte	.LASF336
	.byte	0x5
	.uleb128 0
	.4byte	.LASF337
	.byte	0x5
	.uleb128 0
	.4byte	.LASF338
	.byte	0x5
	.uleb128 0
	.4byte	.LASF339
	.byte	0x5
	.uleb128 0
	.4byte	.LASF340
	.byte	0x5
	.uleb128 0
	.4byte	.LASF341
	.byte	0x5
	.uleb128 0
	.4byte	.LASF342
	.byte	0x5
	.uleb128 0
	.4byte	.LASF343
	.byte	0x5
	.uleb128 0
	.4byte	.LASF344
	.byte	0x5
	.uleb128 0
	.4byte	.LASF345
	.byte	0x5
	.uleb128 0
	.4byte	.LASF346
	.byte	0x5
	.uleb128 0
	.4byte	.LASF347
	.byte	0x5
	.uleb128 0
	.4byte	.LASF348
	.byte	0x5
	.uleb128 0
	.4byte	.LASF349
	.byte	0x5
	.uleb128 0
	.4byte	.LASF350
	.byte	0x5
	.uleb128 0
	.4byte	.LASF351
	.byte	0x5
	.uleb128 0
	.4byte	.LASF352
	.byte	0x5
	.uleb128 0
	.4byte	.LASF353
	.byte	0x5
	.uleb128 0
	.4byte	.LASF354
	.byte	0x5
	.uleb128 0
	.4byte	.LASF355
	.byte	0x5
	.uleb128 0
	.4byte	.LASF356
	.byte	0x5
	.uleb128 0
	.4byte	.LASF357
	.byte	0x5
	.uleb128 0
	.4byte	.LASF358
	.byte	0x5
	.uleb128 0
	.4byte	.LASF359
	.byte	0x5
	.uleb128 0
	.4byte	.LASF360
	.byte	0x5
	.uleb128 0
	.4byte	.LASF361
	.byte	0x5
	.uleb128 0
	.4byte	.LASF362
	.byte	0x5
	.uleb128 0
	.4byte	.LASF363
	.byte	0x5
	.uleb128 0
	.4byte	.LASF364
	.byte	0x5
	.uleb128 0
	.4byte	.LASF365
	.byte	0x5
	.uleb128 0
	.4byte	.LASF366
	.byte	0x5
	.uleb128 0
	.4byte	.LASF367
	.byte	0x5
	.uleb128 0
	.4byte	.LASF368
	.byte	0x5
	.uleb128 0
	.4byte	.LASF369
	.byte	0x5
	.uleb128 0
	.4byte	.LASF370
	.byte	0x5
	.uleb128 0
	.4byte	.LASF371
	.byte	0x5
	.uleb128 0
	.4byte	.LASF372
	.byte	0x5
	.uleb128 0
	.4byte	.LASF373
	.byte	0x5
	.uleb128 0
	.4byte	.LASF374
	.byte	0x5
	.uleb128 0
	.4byte	.LASF375
	.byte	0x5
	.uleb128 0
	.4byte	.LASF376
	.byte	0x5
	.uleb128 0
	.4byte	.LASF377
	.byte	0x5
	.uleb128 0
	.4byte	.LASF378
	.byte	0x5
	.uleb128 0
	.4byte	.LASF379
	.byte	0x5
	.uleb128 0
	.4byte	.LASF380
	.byte	0x5
	.uleb128 0
	.4byte	.LASF381
	.byte	0x5
	.uleb128 0
	.4byte	.LASF382
	.byte	0x5
	.uleb128 0
	.4byte	.LASF383
	.byte	0x5
	.uleb128 0
	.4byte	.LASF384
	.byte	0x5
	.uleb128 0
	.4byte	.LASF385
	.byte	0x5
	.uleb128 0
	.4byte	.LASF386
	.byte	0x5
	.uleb128 0
	.4byte	.LASF387
	.byte	0x5
	.uleb128 0
	.4byte	.LASF388
	.byte	0x5
	.uleb128 0
	.4byte	.LASF389
	.byte	0x5
	.uleb128 0
	.4byte	.LASF390
	.byte	0x5
	.uleb128 0
	.4byte	.LASF391
	.byte	0x6
	.uleb128 0
	.4byte	.LASF392
	.byte	0x6
	.uleb128 0
	.4byte	.LASF393
	.byte	0x6
	.uleb128 0
	.4byte	.LASF394
	.byte	0x6
	.uleb128 0
	.4byte	.LASF395
	.byte	0x6
	.uleb128 0
	.4byte	.LASF396
	.byte	0x6
	.uleb128 0
	.4byte	.LASF397
	.byte	0x5
	.uleb128 0
	.4byte	.LASF398
	.byte	0x6
	.uleb128 0
	.4byte	.LASF399
	.byte	0x6
	.uleb128 0
	.4byte	.LASF400
	.byte	0x6
	.uleb128 0
	.4byte	.LASF401
	.byte	0x5
	.uleb128 0
	.4byte	.LASF402
	.byte	0x5
	.uleb128 0
	.4byte	.LASF403
	.byte	0x6
	.uleb128 0
	.4byte	.LASF404
	.byte	0x5
	.uleb128 0
	.4byte	.LASF405
	.byte	0x5
	.uleb128 0
	.4byte	.LASF406
	.byte	0x5
	.uleb128 0
	.4byte	.LASF407
	.byte	0x6
	.uleb128 0
	.4byte	.LASF408
	.byte	0x5
	.uleb128 0
	.4byte	.LASF409
	.byte	0x5
	.uleb128 0
	.4byte	.LASF410
	.byte	0x6
	.uleb128 0
	.4byte	.LASF411
	.byte	0x5
	.uleb128 0
	.4byte	.LASF412
	.byte	0x5
	.uleb128 0
	.4byte	.LASF413
	.byte	0x5
	.uleb128 0
	.4byte	.LASF414
	.byte	0x5
	.uleb128 0
	.4byte	.LASF415
	.byte	0x5
	.uleb128 0
	.4byte	.LASF416
	.byte	0x5
	.uleb128 0
	.4byte	.LASF417
	.byte	0x5
	.uleb128 0
	.4byte	.LASF418
	.byte	0x6
	.uleb128 0
	.4byte	.LASF419
	.byte	0x5
	.uleb128 0
	.4byte	.LASF420
	.byte	0x5
	.uleb128 0
	.4byte	.LASF421
	.byte	0x5
	.uleb128 0
	.4byte	.LASF422
	.byte	0x5
	.uleb128 0
	.4byte	.LASF423
	.byte	0x6
	.uleb128 0
	.4byte	.LASF424
	.byte	0x6
	.uleb128 0
	.4byte	.LASF425
	.byte	0x6
	.uleb128 0
	.4byte	.LASF426
	.byte	0x6
	.uleb128 0
	.4byte	.LASF427
	.byte	0x6
	.uleb128 0
	.4byte	.LASF428
	.byte	0x6
	.uleb128 0
	.4byte	.LASF429
	.byte	0x6
	.uleb128 0
	.4byte	.LASF430
	.byte	0x6
	.uleb128 0
	.4byte	.LASF431
	.byte	0x6
	.uleb128 0
	.4byte	.LASF432
	.byte	0x6
	.uleb128 0
	.4byte	.LASF433
	.byte	0x6
	.uleb128 0
	.4byte	.LASF434
	.byte	0x5
	.uleb128 0
	.4byte	.LASF435
	.byte	0x5
	.uleb128 0
	.4byte	.LASF436
	.byte	0x5
	.uleb128 0
	.4byte	.LASF437
	.byte	0x5
	.uleb128 0
	.4byte	.LASF438
	.byte	0x6
	.uleb128 0
	.4byte	.LASF439
	.byte	0x5
	.uleb128 0
	.4byte	.LASF440
	.byte	0x5
	.uleb128 0
	.4byte	.LASF441
	.byte	0x5
	.uleb128 0
	.4byte	.LASF442
	.byte	0x6
	.uleb128 0
	.4byte	.LASF443
	.byte	0x5
	.uleb128 0
	.4byte	.LASF444
	.byte	0x6
	.uleb128 0
	.4byte	.LASF445
	.byte	0x6
	.uleb128 0
	.4byte	.LASF446
	.byte	0x6
	.uleb128 0
	.4byte	.LASF447
	.byte	0x6
	.uleb128 0
	.4byte	.LASF448
	.byte	0x6
	.uleb128 0
	.4byte	.LASF449
	.byte	0x6
	.uleb128 0
	.4byte	.LASF450
	.byte	0x5
	.uleb128 0
	.4byte	.LASF451
	.byte	0x5
	.uleb128 0
	.4byte	.LASF452
	.byte	0x5
	.uleb128 0
	.4byte	.LASF453
	.byte	0x5
	.uleb128 0
	.4byte	.LASF454
	.byte	0x5
	.uleb128 0
	.4byte	.LASF455
	.byte	0x5
	.uleb128 0
	.4byte	.LASF456
	.byte	0x5
	.uleb128 0
	.4byte	.LASF457
	.byte	0x5
	.uleb128 0
	.4byte	.LASF458
	.byte	0x5
	.uleb128 0
	.4byte	.LASF459
	.byte	0x5
	.uleb128 0
	.4byte	.LASF460
	.byte	0x5
	.uleb128 0
	.4byte	.LASF461
	.byte	0x5
	.uleb128 0
	.4byte	.LASF462
	.byte	0x5
	.uleb128 0
	.4byte	.LASF463
	.byte	0x5
	.uleb128 0
	.4byte	.LASF464
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.conf.h.2.ed4ce403b74b65a7e79992cab9b49542,comdat
.Ldebug_macro3:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x2
	.4byte	.LASF465
	.byte	0x5
	.uleb128 0x3
	.4byte	.LASF466
	.byte	0x5
	.uleb128 0x5
	.4byte	.LASF467
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF468
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF469
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF470
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF471
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF472
	.byte	0x5
	.uleb128 0xd
	.4byte	.LASF473
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF474
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF475
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF476
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF477
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF478
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF479
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF480
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF481
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF482
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF483
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF484
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF485
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF486
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF487
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF488
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF489
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF490
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF491
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF492
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF493
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF494
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF495
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF496
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF497
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF498
	.byte	0x5
	.uleb128 0x38
	.4byte	.LASF499
	.byte	0x5
	.uleb128 0x39
	.4byte	.LASF500
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF501
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF502
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF503
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF504
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF505
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF506
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF507
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF508
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF509
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF510
	.byte	0x5
	.uleb128 0x49
	.4byte	.LASF511
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF512
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF513
	.byte	0x5
	.uleb128 0x4e
	.4byte	.LASF514
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF515
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF516
	.byte	0x5
	.uleb128 0x54
	.4byte	.LASF517
	.byte	0x5
	.uleb128 0x56
	.4byte	.LASF518
	.byte	0x5
	.uleb128 0x58
	.4byte	.LASF519
	.byte	0x5
	.uleb128 0x59
	.4byte	.LASF520
	.byte	0x5
	.uleb128 0x5a
	.4byte	.LASF521
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF522
	.byte	0x5
	.uleb128 0x5c
	.4byte	.LASF523
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF524
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF525
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF526
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF527
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF528
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF529
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF530
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF531
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF532
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF533
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF534
	.byte	0x5
	.uleb128 0x6b
	.4byte	.LASF535
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF536
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF537
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF538
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF539
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF540
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF541
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF542
	.byte	0x5
	.uleb128 0x76
	.4byte	.LASF543
	.byte	0x5
	.uleb128 0x7c
	.4byte	.LASF544
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF545
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF546
	.byte	0x5
	.uleb128 0x81
	.4byte	.LASF547
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF548
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF549
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF550
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF551
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF552
	.byte	0x5
	.uleb128 0x8e
	.4byte	.LASF553
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF554
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF555
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF556
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF557
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF558
	.byte	0x5
	.uleb128 0xa2
	.4byte	.LASF559
	.byte	0x5
	.uleb128 0xa4
	.4byte	.LASF560
	.byte	0x5
	.uleb128 0xa5
	.4byte	.LASF561
	.byte	0x5
	.uleb128 0xa6
	.4byte	.LASF562
	.byte	0x5
	.uleb128 0xa8
	.4byte	.LASF563
	.byte	0x5
	.uleb128 0xa9
	.4byte	.LASF564
	.byte	0x5
	.uleb128 0xab
	.4byte	.LASF565
	.byte	0x5
	.uleb128 0xae
	.4byte	.LASF566
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF567
	.byte	0x5
	.uleb128 0xb0
	.4byte	.LASF568
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF569
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF570
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF571
	.byte	0x5
	.uleb128 0xb7
	.4byte	.LASF572
	.byte	0x5
	.uleb128 0xb8
	.4byte	.LASF573
	.byte	0x5
	.uleb128 0xb9
	.4byte	.LASF574
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF575
	.byte	0x5
	.uleb128 0xbc
	.4byte	.LASF576
	.byte	0x5
	.uleb128 0xbd
	.4byte	.LASF577
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF578
	.byte	0x5
	.uleb128 0xbf
	.4byte	.LASF579
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF580
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF581
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF582
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF583
	.byte	0x5
	.uleb128 0xc5
	.4byte	.LASF584
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF585
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF586
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF587
	.byte	0x5
	.uleb128 0xca
	.4byte	.LASF588
	.byte	0x5
	.uleb128 0xcb
	.4byte	.LASF589
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF590
	.byte	0x5
	.uleb128 0xce
	.4byte	.LASF591
	.byte	0x5
	.uleb128 0xcf
	.4byte	.LASF592
	.byte	0x5
	.uleb128 0xd0
	.4byte	.LASF593
	.byte	0x5
	.uleb128 0xd1
	.4byte	.LASF594
	.byte	0x5
	.uleb128 0xd2
	.4byte	.LASF595
	.byte	0x5
	.uleb128 0xd4
	.4byte	.LASF596
	.byte	0x5
	.uleb128 0xd7
	.4byte	.LASF597
	.byte	0x5
	.uleb128 0xd9
	.4byte	.LASF598
	.byte	0x5
	.uleb128 0xda
	.4byte	.LASF599
	.byte	0x5
	.uleb128 0xdb
	.4byte	.LASF600
	.byte	0x5
	.uleb128 0xdc
	.4byte	.LASF601
	.byte	0x5
	.uleb128 0xdd
	.4byte	.LASF602
	.byte	0x5
	.uleb128 0xdf
	.4byte	.LASF603
	.byte	0x5
	.uleb128 0xe1
	.4byte	.LASF604
	.byte	0x5
	.uleb128 0xe2
	.4byte	.LASF605
	.byte	0x5
	.uleb128 0xe3
	.4byte	.LASF606
	.byte	0x5
	.uleb128 0xe6
	.4byte	.LASF607
	.byte	0x5
	.uleb128 0xe7
	.4byte	.LASF608
	.byte	0x5
	.uleb128 0xea
	.4byte	.LASF609
	.byte	0x5
	.uleb128 0xeb
	.4byte	.LASF610
	.byte	0x5
	.uleb128 0xed
	.4byte	.LASF611
	.byte	0x5
	.uleb128 0xee
	.4byte	.LASF612
	.byte	0x5
	.uleb128 0xef
	.4byte	.LASF613
	.byte	0x5
	.uleb128 0xf0
	.4byte	.LASF614
	.byte	0x5
	.uleb128 0xf1
	.4byte	.LASF615
	.byte	0x5
	.uleb128 0xf2
	.4byte	.LASF616
	.byte	0x5
	.uleb128 0xf3
	.4byte	.LASF617
	.byte	0x5
	.uleb128 0xf5
	.4byte	.LASF618
	.byte	0x5
	.uleb128 0xf6
	.4byte	.LASF619
	.byte	0x5
	.uleb128 0xf7
	.4byte	.LASF620
	.byte	0x5
	.uleb128 0xfb
	.4byte	.LASF621
	.byte	0x5
	.uleb128 0xfc
	.4byte	.LASF622
	.byte	0x5
	.uleb128 0xfd
	.4byte	.LASF623
	.byte	0x5
	.uleb128 0xfe
	.4byte	.LASF624
	.byte	0x5
	.uleb128 0xff
	.4byte	.LASF625
	.byte	0x5
	.uleb128 0x100
	.4byte	.LASF626
	.byte	0x5
	.uleb128 0x101
	.4byte	.LASF627
	.byte	0x5
	.uleb128 0x102
	.4byte	.LASF628
	.byte	0x5
	.uleb128 0x105
	.4byte	.LASF629
	.byte	0x5
	.uleb128 0x106
	.4byte	.LASF630
	.byte	0x5
	.uleb128 0x108
	.4byte	.LASF631
	.byte	0x5
	.uleb128 0x10b
	.4byte	.LASF632
	.byte	0x5
	.uleb128 0x10c
	.4byte	.LASF633
	.byte	0x5
	.uleb128 0x10d
	.4byte	.LASF634
	.byte	0x5
	.uleb128 0x114
	.4byte	.LASF635
	.byte	0x5
	.uleb128 0x129
	.4byte	.LASF636
	.byte	0x5
	.uleb128 0x12a
	.4byte	.LASF637
	.byte	0x5
	.uleb128 0x138
	.4byte	.LASF638
	.byte	0x5
	.uleb128 0x13c
	.4byte	.LASF639
	.byte	0x5
	.uleb128 0x13f
	.4byte	.LASF640
	.byte	0x5
	.uleb128 0x140
	.4byte	.LASF641
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.genasmdefines.h.5.631b683b88ac0b74cc2616367630af6f,comdat
.Ldebug_macro4:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x5
	.4byte	.LASF642
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF643
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF644
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.compiler.h.7.6225c6e70dcaa0d4336423d5b1a471e8,comdat
.Ldebug_macro5:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF648
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF649
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF650
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF651
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF652
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF653
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF654
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF655
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF656
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF657
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF658
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF659
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF660
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF661
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF662
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF663
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF664
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF665
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF666
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF667
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF668
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF669
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF670
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF671
	.byte	0x5
	.uleb128 0x49
	.4byte	.LASF672
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF673
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF674
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF675
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF676
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF677
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF678
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF679
	.byte	0x5
	.uleb128 0x57
	.4byte	.LASF680
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF681
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF682
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF683
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF684
	.byte	0x5
	.uleb128 0x102
	.4byte	.LASF685
	.byte	0x5
	.uleb128 0x106
	.4byte	.LASF686
	.byte	0x5
	.uleb128 0x107
	.4byte	.LASF687
	.byte	0x5
	.uleb128 0x10a
	.4byte	.LASF688
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF689
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.keep.h.6.9f0c7528dcb60415bbe8e7882a4c8945,comdat
.Ldebug_macro6:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF690
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF691
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF692
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF693
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF694
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF695
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF696
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.scattered_array.h.20.e2ff31aa6fa6a1de51e87efdcb9174b6,comdat
.Ldebug_macro7:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF697
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF698
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF699
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF700
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF701
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF702
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF703
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF704
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF705
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF706
	.byte	0x5
	.uleb128 0x55
	.4byte	.LASF707
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF708
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF709
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.limits.h.6.760e6357f019fe24b3c4097a4598e4c4,comdat
.Ldebug_macro8:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF712
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF713
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF714
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF715
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF716
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF717
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF718
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF719
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF720
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF721
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF722
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF723
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF724
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF725
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF726
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF727
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF728
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF729
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF730
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF731
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdint.h.14.746b19174aad304c7dcbde03ee5fd75a,comdat
.Ldebug_macro9:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF732
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF733
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF734
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF735
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF736
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF737
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF738
	.byte	0x5
	.uleb128 0x6c
	.4byte	.LASF739
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF740
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF741
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF742
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF743
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF744
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF745
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF746
	.byte	0x5
	.uleb128 0x78
	.4byte	.LASF747
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF748
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF749
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF750
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF751
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF752
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF753
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF754
	.byte	0x5
	.uleb128 0x82
	.4byte	.LASF755
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF756
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF757
	.byte	0x5
	.uleb128 0x85
	.4byte	.LASF758
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF759
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF760
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF761
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF762
	.byte	0x5
	.uleb128 0x8e
	.4byte	.LASF763
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF764
	.byte	0x5
	.uleb128 0x90
	.4byte	.LASF765
	.byte	0x5
	.uleb128 0x91
	.4byte	.LASF766
	.byte	0x5
	.uleb128 0x93
	.4byte	.LASF767
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF768
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF769
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF770
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF771
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF772
	.byte	0x5
	.uleb128 0x9c
	.4byte	.LASF773
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF774
	.byte	0x5
	.uleb128 0xa0
	.4byte	.LASF775
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF776
	.byte	0x5
	.uleb128 0xa4
	.4byte	.LASF777
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF778
	.byte	0x5
	.uleb128 0xb2
	.4byte	.LASF779
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF780
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF781
	.byte	0x5
	.uleb128 0xb5
	.4byte	.LASF782
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF783
	.byte	0x5
	.uleb128 0xbb
	.4byte	.LASF784
	.byte	0x5
	.uleb128 0xbc
	.4byte	.LASF785
	.byte	0x5
	.uleb128 0xbd
	.4byte	.LASF786
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF787
	.byte	0x5
	.uleb128 0xbf
	.4byte	.LASF788
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF789
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF790
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF791
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF792
	.byte	0x5
	.uleb128 0xce
	.4byte	.LASF793
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.inttypes.h.11.b36a3d97cd4f39688ec32ad1fdb4feb1,comdat
.Ldebug_macro10:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF794
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF795
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF796
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF797
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF798
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF799
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF800
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF801
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF802
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF803
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF804
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF805
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF806
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF807
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF808
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF809
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF810
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF811
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF812
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF813
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF814
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF815
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF816
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF817
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF818
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF819
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF820
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF821
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF822
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF823
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF824
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF825
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdbool.h.29.4a1c88fe569adb8d03217dd16982ca34,comdat
.Ldebug_macro11:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF826
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF827
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF828
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF829
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF830
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stddef.h.39.3e0425629195acf083c9561151306b4d,comdat
.Ldebug_macro12:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF831
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF832
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF833
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF834
	.byte	0x5
	.uleb128 0x85
	.4byte	.LASF835
	.byte	0x5
	.uleb128 0x86
	.4byte	.LASF836
	.byte	0x5
	.uleb128 0x87
	.4byte	.LASF837
	.byte	0x5
	.uleb128 0x88
	.4byte	.LASF838
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF839
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF840
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF841
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF842
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF843
	.byte	0x6
	.uleb128 0x9e
	.4byte	.LASF844
	.byte	0x5
	.uleb128 0xb9
	.4byte	.LASF845
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF846
	.byte	0x5
	.uleb128 0xbb
	.4byte	.LASF847
	.byte	0x5
	.uleb128 0xbc
	.4byte	.LASF848
	.byte	0x5
	.uleb128 0xbd
	.4byte	.LASF849
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF850
	.byte	0x5
	.uleb128 0xbf
	.4byte	.LASF851
	.byte	0x5
	.uleb128 0xc0
	.4byte	.LASF852
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF853
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF854
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF855
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF856
	.byte	0x5
	.uleb128 0xc5
	.4byte	.LASF857
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF858
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF859
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF860
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF861
	.byte	0x5
	.uleb128 0xd0
	.4byte	.LASF862
	.byte	0x6
	.uleb128 0xed
	.4byte	.LASF863
	.byte	0x5
	.uleb128 0x10b
	.4byte	.LASF864
	.byte	0x5
	.uleb128 0x10c
	.4byte	.LASF865
	.byte	0x5
	.uleb128 0x10d
	.4byte	.LASF866
	.byte	0x5
	.uleb128 0x10e
	.4byte	.LASF867
	.byte	0x5
	.uleb128 0x10f
	.4byte	.LASF868
	.byte	0x5
	.uleb128 0x110
	.4byte	.LASF869
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF870
	.byte	0x5
	.uleb128 0x112
	.4byte	.LASF871
	.byte	0x5
	.uleb128 0x113
	.4byte	.LASF872
	.byte	0x5
	.uleb128 0x114
	.4byte	.LASF873
	.byte	0x5
	.uleb128 0x115
	.4byte	.LASF874
	.byte	0x5
	.uleb128 0x116
	.4byte	.LASF875
	.byte	0x5
	.uleb128 0x117
	.4byte	.LASF876
	.byte	0x5
	.uleb128 0x118
	.4byte	.LASF877
	.byte	0x5
	.uleb128 0x119
	.4byte	.LASF878
	.byte	0x5
	.uleb128 0x11a
	.4byte	.LASF879
	.byte	0x6
	.uleb128 0x127
	.4byte	.LASF880
	.byte	0x6
	.uleb128 0x15d
	.4byte	.LASF881
	.byte	0x6
	.uleb128 0x18f
	.4byte	.LASF882
	.byte	0x5
	.uleb128 0x194
	.4byte	.LASF883
	.byte	0x6
	.uleb128 0x19a
	.4byte	.LASF884
	.byte	0x5
	.uleb128 0x19f
	.4byte	.LASF885
	.byte	0x5
	.uleb128 0x1a4
	.4byte	.LASF886
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_api_defines.h.9.ca569bf9e9d05e0bfcb305aaa2de33ca,comdat
.Ldebug_macro13:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF887
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF888
	.byte	0x5
	.uleb128 0xd
	.4byte	.LASF889
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF890
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF891
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF892
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF893
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF894
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF895
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF896
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF897
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF898
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF899
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF900
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF901
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF902
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF903
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF904
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF905
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF906
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF907
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF908
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF909
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF910
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF911
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF912
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF913
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF914
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF915
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF916
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF917
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF918
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF919
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF920
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF921
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF922
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF923
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF924
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF925
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF926
	.byte	0x5
	.uleb128 0x38
	.4byte	.LASF927
	.byte	0x5
	.uleb128 0x39
	.4byte	.LASF928
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF929
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF930
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF931
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF932
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF933
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF934
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF935
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF936
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF937
	.byte	0x5
	.uleb128 0x47
	.4byte	.LASF938
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF939
	.byte	0x5
	.uleb128 0x4b
	.4byte	.LASF940
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF941
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF942
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF943
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF944
	.byte	0x5
	.uleb128 0x52
	.4byte	.LASF945
	.byte	0x5
	.uleb128 0x55
	.4byte	.LASF946
	.byte	0x5
	.uleb128 0x58
	.4byte	.LASF947
	.byte	0x5
	.uleb128 0x5a
	.4byte	.LASF948
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF949
	.byte	0x5
	.uleb128 0x5c
	.4byte	.LASF950
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF951
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF952
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF953
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF954
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF955
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF956
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF957
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF958
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF959
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF960
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF961
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF962
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF963
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF964
	.byte	0x5
	.uleb128 0x6b
	.4byte	.LASF965
	.byte	0x5
	.uleb128 0x6c
	.4byte	.LASF966
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF967
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF968
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF969
	.byte	0x5
	.uleb128 0x70
	.4byte	.LASF970
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF971
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF972
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF973
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF974
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF975
	.byte	0x5
	.uleb128 0x78
	.4byte	.LASF976
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF977
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF978
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF979
	.byte	0x5
	.uleb128 0x7c
	.4byte	.LASF980
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF981
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF982
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF983
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF984
	.byte	0x5
	.uleb128 0x81
	.4byte	.LASF985
	.byte	0x5
	.uleb128 0x82
	.4byte	.LASF986
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF987
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF988
	.byte	0x5
	.uleb128 0x85
	.4byte	.LASF989
	.byte	0x5
	.uleb128 0x86
	.4byte	.LASF990
	.byte	0x5
	.uleb128 0x87
	.4byte	.LASF991
	.byte	0x5
	.uleb128 0x88
	.4byte	.LASF992
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF993
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF994
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF995
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF996
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF997
	.byte	0x5
	.uleb128 0x8e
	.4byte	.LASF998
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF999
	.byte	0x5
	.uleb128 0x90
	.4byte	.LASF1000
	.byte	0x5
	.uleb128 0x91
	.4byte	.LASF1001
	.byte	0x5
	.uleb128 0x92
	.4byte	.LASF1002
	.byte	0x5
	.uleb128 0x93
	.4byte	.LASF1003
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF1004
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF1005
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF1006
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF1007
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF1008
	.byte	0x5
	.uleb128 0x99
	.4byte	.LASF1009
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF1010
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF1011
	.byte	0x5
	.uleb128 0x9c
	.4byte	.LASF1012
	.byte	0x5
	.uleb128 0x9d
	.4byte	.LASF1013
	.byte	0x5
	.uleb128 0x9e
	.4byte	.LASF1014
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF1015
	.byte	0x5
	.uleb128 0xa0
	.4byte	.LASF1016
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF1017
	.byte	0x5
	.uleb128 0xa2
	.4byte	.LASF1018
	.byte	0x5
	.uleb128 0xa3
	.4byte	.LASF1019
	.byte	0x5
	.uleb128 0xa4
	.4byte	.LASF1020
	.byte	0x5
	.uleb128 0xa5
	.4byte	.LASF1021
	.byte	0x5
	.uleb128 0xa6
	.4byte	.LASF1022
	.byte	0x5
	.uleb128 0xa7
	.4byte	.LASF1023
	.byte	0x5
	.uleb128 0xa8
	.4byte	.LASF1024
	.byte	0x5
	.uleb128 0xa9
	.4byte	.LASF1025
	.byte	0x5
	.uleb128 0xaa
	.4byte	.LASF1026
	.byte	0x5
	.uleb128 0xab
	.4byte	.LASF1027
	.byte	0x5
	.uleb128 0xac
	.4byte	.LASF1028
	.byte	0x5
	.uleb128 0xad
	.4byte	.LASF1029
	.byte	0x5
	.uleb128 0xae
	.4byte	.LASF1030
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF1031
	.byte	0x5
	.uleb128 0xb0
	.4byte	.LASF1032
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF1033
	.byte	0x5
	.uleb128 0xb2
	.4byte	.LASF1034
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF1035
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF1036
	.byte	0x5
	.uleb128 0xb5
	.4byte	.LASF1037
	.byte	0x5
	.uleb128 0xb6
	.4byte	.LASF1038
	.byte	0x5
	.uleb128 0xb7
	.4byte	.LASF1039
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF1040
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF1041
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF1042
	.byte	0x5
	.uleb128 0xc5
	.4byte	.LASF1043
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF1044
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF1045
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF1046
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF1047
	.byte	0x5
	.uleb128 0xca
	.4byte	.LASF1048
	.byte	0x5
	.uleb128 0xcb
	.4byte	.LASF1049
	.byte	0x5
	.uleb128 0xcc
	.4byte	.LASF1050
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF1051
	.byte	0x5
	.uleb128 0xce
	.4byte	.LASF1052
	.byte	0x5
	.uleb128 0xcf
	.4byte	.LASF1053
	.byte	0x5
	.uleb128 0xd0
	.4byte	.LASF1054
	.byte	0x5
	.uleb128 0xd4
	.4byte	.LASF1055
	.byte	0x5
	.uleb128 0xd5
	.4byte	.LASF1056
	.byte	0x5
	.uleb128 0xd6
	.4byte	.LASF1057
	.byte	0x5
	.uleb128 0xd7
	.4byte	.LASF1058
	.byte	0x5
	.uleb128 0xd8
	.4byte	.LASF1059
	.byte	0x5
	.uleb128 0xd9
	.4byte	.LASF1060
	.byte	0x5
	.uleb128 0xda
	.4byte	.LASF1061
	.byte	0x5
	.uleb128 0xdb
	.4byte	.LASF1062
	.byte	0x5
	.uleb128 0xdc
	.4byte	.LASF1063
	.byte	0x5
	.uleb128 0xdd
	.4byte	.LASF1064
	.byte	0x5
	.uleb128 0xde
	.4byte	.LASF1065
	.byte	0x5
	.uleb128 0xdf
	.4byte	.LASF1066
	.byte	0x5
	.uleb128 0xe0
	.4byte	.LASF1067
	.byte	0x5
	.uleb128 0xe1
	.4byte	.LASF1068
	.byte	0x5
	.uleb128 0xe2
	.4byte	.LASF1069
	.byte	0x5
	.uleb128 0xe3
	.4byte	.LASF1070
	.byte	0x5
	.uleb128 0xe4
	.4byte	.LASF1071
	.byte	0x5
	.uleb128 0xe5
	.4byte	.LASF1072
	.byte	0x5
	.uleb128 0xe6
	.4byte	.LASF1073
	.byte	0x5
	.uleb128 0xe7
	.4byte	.LASF1074
	.byte	0x5
	.uleb128 0xe8
	.4byte	.LASF1075
	.byte	0x5
	.uleb128 0xe9
	.4byte	.LASF1076
	.byte	0x5
	.uleb128 0xea
	.4byte	.LASF1077
	.byte	0x5
	.uleb128 0xeb
	.4byte	.LASF1078
	.byte	0x5
	.uleb128 0xec
	.4byte	.LASF1079
	.byte	0x5
	.uleb128 0xed
	.4byte	.LASF1080
	.byte	0x5
	.uleb128 0xee
	.4byte	.LASF1081
	.byte	0x5
	.uleb128 0xef
	.4byte	.LASF1082
	.byte	0x5
	.uleb128 0xf0
	.4byte	.LASF1083
	.byte	0x5
	.uleb128 0xf1
	.4byte	.LASF1084
	.byte	0x5
	.uleb128 0xf2
	.4byte	.LASF1085
	.byte	0x5
	.uleb128 0xf3
	.4byte	.LASF1086
	.byte	0x5
	.uleb128 0xf4
	.4byte	.LASF1087
	.byte	0x5
	.uleb128 0xf8
	.4byte	.LASF1088
	.byte	0x5
	.uleb128 0xf9
	.4byte	.LASF1089
	.byte	0x5
	.uleb128 0xfa
	.4byte	.LASF1090
	.byte	0x5
	.uleb128 0xfb
	.4byte	.LASF1091
	.byte	0x5
	.uleb128 0xfc
	.4byte	.LASF1092
	.byte	0x5
	.uleb128 0xfd
	.4byte	.LASF1093
	.byte	0x5
	.uleb128 0xfe
	.4byte	.LASF1094
	.byte	0x5
	.uleb128 0xff
	.4byte	.LASF1095
	.byte	0x5
	.uleb128 0x100
	.4byte	.LASF1096
	.byte	0x5
	.uleb128 0x101
	.4byte	.LASF1097
	.byte	0x5
	.uleb128 0x102
	.4byte	.LASF1098
	.byte	0x5
	.uleb128 0x103
	.4byte	.LASF1099
	.byte	0x5
	.uleb128 0x104
	.4byte	.LASF1100
	.byte	0x5
	.uleb128 0x105
	.4byte	.LASF1101
	.byte	0x5
	.uleb128 0x106
	.4byte	.LASF1102
	.byte	0x5
	.uleb128 0x107
	.4byte	.LASF1103
	.byte	0x5
	.uleb128 0x108
	.4byte	.LASF1104
	.byte	0x5
	.uleb128 0x109
	.4byte	.LASF1105
	.byte	0x5
	.uleb128 0x10a
	.4byte	.LASF1106
	.byte	0x5
	.uleb128 0x10b
	.4byte	.LASF1107
	.byte	0x5
	.uleb128 0x10c
	.4byte	.LASF1108
	.byte	0x5
	.uleb128 0x10d
	.4byte	.LASF1109
	.byte	0x5
	.uleb128 0x10e
	.4byte	.LASF1110
	.byte	0x5
	.uleb128 0x10f
	.4byte	.LASF1111
	.byte	0x5
	.uleb128 0x110
	.4byte	.LASF1112
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF1113
	.byte	0x5
	.uleb128 0x112
	.4byte	.LASF1114
	.byte	0x5
	.uleb128 0x113
	.4byte	.LASF1115
	.byte	0x5
	.uleb128 0x114
	.4byte	.LASF1116
	.byte	0x5
	.uleb128 0x115
	.4byte	.LASF1117
	.byte	0x5
	.uleb128 0x116
	.4byte	.LASF1118
	.byte	0x5
	.uleb128 0x117
	.4byte	.LASF1119
	.byte	0x5
	.uleb128 0x118
	.4byte	.LASF1120
	.byte	0x5
	.uleb128 0x119
	.4byte	.LASF1121
	.byte	0x5
	.uleb128 0x11a
	.4byte	.LASF1122
	.byte	0x5
	.uleb128 0x11b
	.4byte	.LASF1123
	.byte	0x5
	.uleb128 0x11c
	.4byte	.LASF1124
	.byte	0x5
	.uleb128 0x11d
	.4byte	.LASF1125
	.byte	0x5
	.uleb128 0x11e
	.4byte	.LASF1126
	.byte	0x5
	.uleb128 0x120
	.4byte	.LASF1127
	.byte	0x5
	.uleb128 0x121
	.4byte	.LASF1128
	.byte	0x5
	.uleb128 0x129
	.4byte	.LASF1129
	.byte	0x5
	.uleb128 0x12a
	.4byte	.LASF1130
	.byte	0x5
	.uleb128 0x12d
	.4byte	.LASF1131
	.byte	0x5
	.uleb128 0x12e
	.4byte	.LASF1132
	.byte	0x5
	.uleb128 0x12f
	.4byte	.LASF1133
	.byte	0x5
	.uleb128 0x130
	.4byte	.LASF1134
	.byte	0x5
	.uleb128 0x131
	.4byte	.LASF1135
	.byte	0x5
	.uleb128 0x132
	.4byte	.LASF1136
	.byte	0x5
	.uleb128 0x133
	.4byte	.LASF1137
	.byte	0x5
	.uleb128 0x134
	.4byte	.LASF1138
	.byte	0x5
	.uleb128 0x138
	.4byte	.LASF1139
	.byte	0x5
	.uleb128 0x139
	.4byte	.LASF1140
	.byte	0x5
	.uleb128 0x13a
	.4byte	.LASF1141
	.byte	0x5
	.uleb128 0x13b
	.4byte	.LASF1142
	.byte	0x5
	.uleb128 0x13c
	.4byte	.LASF1143
	.byte	0x5
	.uleb128 0x13e
	.4byte	.LASF1144
	.byte	0x5
	.uleb128 0x13f
	.4byte	.LASF1145
	.byte	0x5
	.uleb128 0x140
	.4byte	.LASF1146
	.byte	0x5
	.uleb128 0x141
	.4byte	.LASF1147
	.byte	0x5
	.uleb128 0x142
	.4byte	.LASF1148
	.byte	0x5
	.uleb128 0x143
	.4byte	.LASF1149
	.byte	0x5
	.uleb128 0x144
	.4byte	.LASF1150
	.byte	0x5
	.uleb128 0x145
	.4byte	.LASF1151
	.byte	0x5
	.uleb128 0x146
	.4byte	.LASF1152
	.byte	0x5
	.uleb128 0x147
	.4byte	.LASF1153
	.byte	0x5
	.uleb128 0x148
	.4byte	.LASF1154
	.byte	0x5
	.uleb128 0x149
	.4byte	.LASF1155
	.byte	0x5
	.uleb128 0x14b
	.4byte	.LASF1156
	.byte	0x5
	.uleb128 0x14d
	.4byte	.LASF1157
	.byte	0x5
	.uleb128 0x14e
	.4byte	.LASF1158
	.byte	0x5
	.uleb128 0x14f
	.4byte	.LASF1159
	.byte	0x5
	.uleb128 0x151
	.4byte	.LASF1160
	.byte	0x5
	.uleb128 0x152
	.4byte	.LASF1161
	.byte	0x5
	.uleb128 0x153
	.4byte	.LASF1162
	.byte	0x5
	.uleb128 0x155
	.4byte	.LASF1163
	.byte	0x5
	.uleb128 0x156
	.4byte	.LASF1164
	.byte	0x5
	.uleb128 0x157
	.4byte	.LASF1165
	.byte	0x5
	.uleb128 0x158
	.4byte	.LASF1166
	.byte	0x5
	.uleb128 0x159
	.4byte	.LASF1167
	.byte	0x5
	.uleb128 0x15a
	.4byte	.LASF1168
	.byte	0x5
	.uleb128 0x15b
	.4byte	.LASF1169
	.byte	0x5
	.uleb128 0x15c
	.4byte	.LASF1170
	.byte	0x5
	.uleb128 0x15d
	.4byte	.LASF1171
	.byte	0x5
	.uleb128 0x15f
	.4byte	.LASF1172
	.byte	0x5
	.uleb128 0x160
	.4byte	.LASF1173
	.byte	0x5
	.uleb128 0x162
	.4byte	.LASF1174
	.byte	0x5
	.uleb128 0x163
	.4byte	.LASF1175
	.byte	0x5
	.uleb128 0x165
	.4byte	.LASF1176
	.byte	0x5
	.uleb128 0x166
	.4byte	.LASF1177
	.byte	0x5
	.uleb128 0x167
	.4byte	.LASF1178
	.byte	0x5
	.uleb128 0x169
	.4byte	.LASF1179
	.byte	0x5
	.uleb128 0x16b
	.4byte	.LASF1180
	.byte	0x5
	.uleb128 0x16c
	.4byte	.LASF1181
	.byte	0x5
	.uleb128 0x16d
	.4byte	.LASF1182
	.byte	0x5
	.uleb128 0x16e
	.4byte	.LASF1183
	.byte	0x5
	.uleb128 0x16f
	.4byte	.LASF1184
	.byte	0x5
	.uleb128 0x170
	.4byte	.LASF1185
	.byte	0x5
	.uleb128 0x171
	.4byte	.LASF1186
	.byte	0x5
	.uleb128 0x172
	.4byte	.LASF1187
	.byte	0x5
	.uleb128 0x175
	.4byte	.LASF1188
	.byte	0x5
	.uleb128 0x176
	.4byte	.LASF1189
	.byte	0x5
	.uleb128 0x177
	.4byte	.LASF1190
	.byte	0x5
	.uleb128 0x178
	.4byte	.LASF1191
	.byte	0x5
	.uleb128 0x179
	.4byte	.LASF1192
	.byte	0x5
	.uleb128 0x17b
	.4byte	.LASF1193
	.byte	0x5
	.uleb128 0x17c
	.4byte	.LASF1194
	.byte	0x5
	.uleb128 0x17d
	.4byte	.LASF1195
	.byte	0x5
	.uleb128 0x17e
	.4byte	.LASF1196
	.byte	0x5
	.uleb128 0x17f
	.4byte	.LASF1197
	.byte	0x5
	.uleb128 0x181
	.4byte	.LASF1198
	.byte	0x5
	.uleb128 0x182
	.4byte	.LASF1199
	.byte	0x5
	.uleb128 0x183
	.4byte	.LASF1200
	.byte	0x5
	.uleb128 0x184
	.4byte	.LASF1201
	.byte	0x5
	.uleb128 0x186
	.4byte	.LASF1202
	.byte	0x5
	.uleb128 0x187
	.4byte	.LASF1203
	.byte	0x5
	.uleb128 0x188
	.4byte	.LASF1204
	.byte	0x5
	.uleb128 0x189
	.4byte	.LASF1205
	.byte	0x5
	.uleb128 0x18a
	.4byte	.LASF1206
	.byte	0x5
	.uleb128 0x18b
	.4byte	.LASF1207
	.byte	0x5
	.uleb128 0x18c
	.4byte	.LASF1208
	.byte	0x5
	.uleb128 0x18d
	.4byte	.LASF1209
	.byte	0x5
	.uleb128 0x18f
	.4byte	.LASF1210
	.byte	0x5
	.uleb128 0x190
	.4byte	.LASF1211
	.byte	0x5
	.uleb128 0x192
	.4byte	.LASF1212
	.byte	0x5
	.uleb128 0x193
	.4byte	.LASF1213
	.byte	0x5
	.uleb128 0x194
	.4byte	.LASF1214
	.byte	0x5
	.uleb128 0x196
	.4byte	.LASF1215
	.byte	0x5
	.uleb128 0x197
	.4byte	.LASF1216
	.byte	0x5
	.uleb128 0x198
	.4byte	.LASF1217
	.byte	0x5
	.uleb128 0x199
	.4byte	.LASF1218
	.byte	0x5
	.uleb128 0x19b
	.4byte	.LASF1219
	.byte	0x5
	.uleb128 0x19c
	.4byte	.LASF1220
	.byte	0x5
	.uleb128 0x19d
	.4byte	.LASF1221
	.byte	0x5
	.uleb128 0x19e
	.4byte	.LASF1222
	.byte	0x5
	.uleb128 0x19f
	.4byte	.LASF1223
	.byte	0x5
	.uleb128 0x1a1
	.4byte	.LASF1224
	.byte	0x5
	.uleb128 0x1a2
	.4byte	.LASF1225
	.byte	0x5
	.uleb128 0x1a3
	.4byte	.LASF1226
	.byte	0x5
	.uleb128 0x1a4
	.4byte	.LASF1227
	.byte	0x5
	.uleb128 0x1a6
	.4byte	.LASF1228
	.byte	0x5
	.uleb128 0x1a8
	.4byte	.LASF1229
	.byte	0x5
	.uleb128 0x1aa
	.4byte	.LASF1230
	.byte	0x5
	.uleb128 0x1ab
	.4byte	.LASF1231
	.byte	0x5
	.uleb128 0x1ac
	.4byte	.LASF1232
	.byte	0x5
	.uleb128 0x1ad
	.4byte	.LASF1233
	.byte	0x5
	.uleb128 0x1ae
	.4byte	.LASF1234
	.byte	0x5
	.uleb128 0x1b0
	.4byte	.LASF1235
	.byte	0x5
	.uleb128 0x1b1
	.4byte	.LASF1236
	.byte	0x5
	.uleb128 0x1b3
	.4byte	.LASF1237
	.byte	0x5
	.uleb128 0x1b4
	.4byte	.LASF1238
	.byte	0x5
	.uleb128 0x1b5
	.4byte	.LASF1239
	.byte	0x5
	.uleb128 0x1b7
	.4byte	.LASF1240
	.byte	0x5
	.uleb128 0x1b8
	.4byte	.LASF1241
	.byte	0x5
	.uleb128 0x1b9
	.4byte	.LASF1242
	.byte	0x5
	.uleb128 0x1ba
	.4byte	.LASF1243
	.byte	0x5
	.uleb128 0x1bc
	.4byte	.LASF1244
	.byte	0x5
	.uleb128 0x1bd
	.4byte	.LASF1245
	.byte	0x5
	.uleb128 0x1be
	.4byte	.LASF1246
	.byte	0x5
	.uleb128 0x1bf
	.4byte	.LASF1247
	.byte	0x5
	.uleb128 0x1c0
	.4byte	.LASF1248
	.byte	0x5
	.uleb128 0x1c2
	.4byte	.LASF1249
	.byte	0x5
	.uleb128 0x1c3
	.4byte	.LASF1250
	.byte	0x5
	.uleb128 0x1c4
	.4byte	.LASF1251
	.byte	0x5
	.uleb128 0x1c5
	.4byte	.LASF1252
	.byte	0x5
	.uleb128 0x1c6
	.4byte	.LASF1253
	.byte	0x5
	.uleb128 0x1c7
	.4byte	.LASF1254
	.byte	0x5
	.uleb128 0x1c9
	.4byte	.LASF1255
	.byte	0x5
	.uleb128 0x1ca
	.4byte	.LASF1256
	.byte	0x5
	.uleb128 0x1cb
	.4byte	.LASF1257
	.byte	0x5
	.uleb128 0x1cc
	.4byte	.LASF1258
	.byte	0x5
	.uleb128 0x1cd
	.4byte	.LASF1259
	.byte	0x5
	.uleb128 0x1ce
	.4byte	.LASF1260
	.byte	0x5
	.uleb128 0x1d0
	.4byte	.LASF1261
	.byte	0x5
	.uleb128 0x1d1
	.4byte	.LASF1262
	.byte	0x5
	.uleb128 0x1d2
	.4byte	.LASF1263
	.byte	0x5
	.uleb128 0x1d4
	.4byte	.LASF1264
	.byte	0x5
	.uleb128 0x1d5
	.4byte	.LASF1265
	.byte	0x5
	.uleb128 0x1d6
	.4byte	.LASF1266
	.byte	0x5
	.uleb128 0x1e2
	.4byte	.LASF1267
	.byte	0x5
	.uleb128 0x1e9
	.4byte	.LASF1268
	.byte	0x5
	.uleb128 0x1ef
	.4byte	.LASF1269
	.byte	0x5
	.uleb128 0x1f2
	.4byte	.LASF1270
	.byte	0x5
	.uleb128 0x1f6
	.4byte	.LASF1271
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_api_types.h.195.70f88af562e4de06db5cb96bde92609f,comdat
.Ldebug_macro14:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF1272
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF1273
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF1274
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF1275
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF1276
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF1277
	.byte	0x5
	.uleb128 0xcb
	.4byte	.LASF1278
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdarg.h.31.b55da1089056868966f25de5dbfc7d3c,comdat
.Ldebug_macro15:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1280
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1281
	.byte	0x6
	.uleb128 0x22
	.4byte	.LASF1282
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF1283
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF1284
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF1285
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF1286
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1287
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF1288
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF1289
	.byte	0x5
	.uleb128 0x6c
	.4byte	.LASF1290
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF1291
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF1292
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF1293
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.trace_levels.h.6.505d35da271c0597dd2f84103d342d6b,comdat
.Ldebug_macro16:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1294
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF1295
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1296
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF1297
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF1298
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF1299
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1300
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF1301
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.trace.h.14.774127cca5e28477b863cb29f11d1107,comdat
.Ldebug_macro17:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF1302
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1303
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF1304
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF1305
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF1306
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF1307
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF1308
	.byte	0x5
	.uleb128 0x49
	.4byte	.LASF1309
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF1310
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF1311
	.byte	0x5
	.uleb128 0x54
	.4byte	.LASF1312
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF1313
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF1314
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF1315
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF1316
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF1317
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF1318
	.byte	0x5
	.uleb128 0x87
	.4byte	.LASF1319
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF1320
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.initcall.h.27.5a57e9f7bfd36d3e729e225feac662a2,comdat
.Ldebug_macro18:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1321
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF1322
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1323
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF1324
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF1325
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1326
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF1327
	.byte	0x5
	.uleb128 0x5a
	.4byte	.LASF1328
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF1329
	.byte	0x5
	.uleb128 0x5c
	.4byte	.LASF1330
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF1331
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF1332
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF1333
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF1334
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF1335
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF1336
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF1337
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF1338
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.types_ext.h.15.e3b47cfc410272ddfab11b3a34a626a6,comdat
.Ldebug_macro19:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1342
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1343
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF1344
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF1345
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1346
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF1347
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1348
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.pgt_cache.h.6.c8088feaa7aee519ace6e2089734334e,comdat
.Ldebug_macro20:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1350
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF1351
	.byte	0x5
	.uleb128 0xa
	.4byte	.LASF1352
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.assert.h.6.59ad21a61f9718ed363a6f369c9758cf,comdat
.Ldebug_macro21:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1353
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF1354
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1355
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1356
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.queue.h.36.ab6be41b80585ec9d579ac4c6beceb1a,comdat
.Ldebug_macro22:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF1362
	.byte	0x5
	.uleb128 0x59
	.4byte	.LASF1363
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF1364
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF1365
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF1366
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF1367
	.byte	0x5
	.uleb128 0x7c
	.4byte	.LASF1368
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF1369
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF1370
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF1371
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF1372
	.byte	0x5
	.uleb128 0x9c
	.4byte	.LASF1373
	.byte	0x5
	.uleb128 0xa5
	.4byte	.LASF1374
	.byte	0x5
	.uleb128 0xad
	.4byte	.LASF1375
	.byte	0x5
	.uleb128 0xae
	.4byte	.LASF1376
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF1377
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF1378
	.byte	0x5
	.uleb128 0xb9
	.4byte	.LASF1379
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF1380
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF1381
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF1382
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF1383
	.byte	0x5
	.uleb128 0xd2
	.4byte	.LASF1384
	.byte	0x5
	.uleb128 0xd7
	.4byte	.LASF1385
	.byte	0x5
	.uleb128 0xdb
	.4byte	.LASF1386
	.byte	0x5
	.uleb128 0xe8
	.4byte	.LASF1387
	.byte	0x5
	.uleb128 0xed
	.4byte	.LASF1388
	.byte	0x5
	.uleb128 0xf0
	.4byte	.LASF1389
	.byte	0x5
	.uleb128 0xf8
	.4byte	.LASF1390
	.byte	0x5
	.uleb128 0xf9
	.4byte	.LASF1391
	.byte	0x5
	.uleb128 0xfa
	.4byte	.LASF1392
	.byte	0x5
	.uleb128 0xff
	.4byte	.LASF1393
	.byte	0x5
	.uleb128 0x105
	.4byte	.LASF1394
	.byte	0x5
	.uleb128 0x108
	.4byte	.LASF1395
	.byte	0x5
	.uleb128 0x110
	.4byte	.LASF1396
	.byte	0x5
	.uleb128 0x115
	.4byte	.LASF1397
	.byte	0x5
	.uleb128 0x11b
	.4byte	.LASF1398
	.byte	0x5
	.uleb128 0x121
	.4byte	.LASF1399
	.byte	0x5
	.uleb128 0x127
	.4byte	.LASF1400
	.byte	0x5
	.uleb128 0x12c
	.4byte	.LASF1401
	.byte	0x5
	.uleb128 0x139
	.4byte	.LASF1402
	.byte	0x5
	.uleb128 0x13f
	.4byte	.LASF1403
	.byte	0x5
	.uleb128 0x144
	.4byte	.LASF1404
	.byte	0x5
	.uleb128 0x149
	.4byte	.LASF1405
	.byte	0x5
	.uleb128 0x154
	.4byte	.LASF1406
	.byte	0x5
	.uleb128 0x155
	.4byte	.LASF1407
	.byte	0x5
	.uleb128 0x156
	.4byte	.LASF1408
	.byte	0x5
	.uleb128 0x15b
	.4byte	.LASF1409
	.byte	0x5
	.uleb128 0x161
	.4byte	.LASF1410
	.byte	0x5
	.uleb128 0x164
	.4byte	.LASF1411
	.byte	0x5
	.uleb128 0x16c
	.4byte	.LASF1412
	.byte	0x5
	.uleb128 0x171
	.4byte	.LASF1413
	.byte	0x5
	.uleb128 0x177
	.4byte	.LASF1414
	.byte	0x5
	.uleb128 0x17d
	.4byte	.LASF1415
	.byte	0x5
	.uleb128 0x183
	.4byte	.LASF1416
	.byte	0x5
	.uleb128 0x188
	.4byte	.LASF1417
	.byte	0x5
	.uleb128 0x195
	.4byte	.LASF1418
	.byte	0x5
	.uleb128 0x19d
	.4byte	.LASF1419
	.byte	0x5
	.uleb128 0x19e
	.4byte	.LASF1420
	.byte	0x5
	.uleb128 0x19f
	.4byte	.LASF1421
	.byte	0x5
	.uleb128 0x1a4
	.4byte	.LASF1422
	.byte	0x5
	.uleb128 0x1a9
	.4byte	.LASF1423
	.byte	0x5
	.uleb128 0x1ab
	.4byte	.LASF1424
	.byte	0x5
	.uleb128 0x1ae
	.4byte	.LASF1425
	.byte	0x5
	.uleb128 0x1b3
	.4byte	.LASF1426
	.byte	0x5
	.uleb128 0x1d0
	.4byte	.LASF1427
	.byte	0x5
	.uleb128 0x1d1
	.4byte	.LASF1428
	.byte	0x5
	.uleb128 0x1d2
	.4byte	.LASF1429
	.byte	0x5
	.uleb128 0x1d3
	.4byte	.LASF1430
	.byte	0x5
	.uleb128 0x1d4
	.4byte	.LASF1431
	.byte	0x5
	.uleb128 0x1d7
	.4byte	.LASF1432
	.byte	0x5
	.uleb128 0x1dc
	.4byte	.LASF1433
	.byte	0x5
	.uleb128 0x1e7
	.4byte	.LASF1434
	.byte	0x5
	.uleb128 0x1ef
	.4byte	.LASF1435
	.byte	0x5
	.uleb128 0x1fa
	.4byte	.LASF1436
	.byte	0x5
	.uleb128 0x202
	.4byte	.LASF1437
	.byte	0x5
	.uleb128 0x20e
	.4byte	.LASF1438
	.byte	0x5
	.uleb128 0x213
	.4byte	.LASF1439
	.byte	0x5
	.uleb128 0x218
	.4byte	.LASF1440
	.byte	0x5
	.uleb128 0x21d
	.4byte	.LASF1441
	.byte	0x5
	.uleb128 0x222
	.4byte	.LASF1442
	.byte	0x5
	.uleb128 0x22e
	.4byte	.LASF1443
	.byte	0x5
	.uleb128 0x22f
	.4byte	.LASF1444
	.byte	0x5
	.uleb128 0x230
	.4byte	.LASF1445
	.byte	0x5
	.uleb128 0x232
	.4byte	.LASF1446
	.byte	0x5
	.uleb128 0x234
	.4byte	.LASF1447
	.byte	0x5
	.uleb128 0x25b
	.4byte	.LASF1448
	.byte	0x5
	.uleb128 0x25c
	.4byte	.LASF1449
	.byte	0x5
	.uleb128 0x25d
	.4byte	.LASF1450
	.byte	0x5
	.uleb128 0x260
	.4byte	.LASF1451
	.byte	0x5
	.uleb128 0x266
	.4byte	.LASF1452
	.byte	0x5
	.uleb128 0x269
	.4byte	.LASF1453
	.byte	0x5
	.uleb128 0x272
	.4byte	.LASF1454
	.byte	0x5
	.uleb128 0x277
	.4byte	.LASF1455
	.byte	0x5
	.uleb128 0x283
	.4byte	.LASF1456
	.byte	0x5
	.uleb128 0x28f
	.4byte	.LASF1457
	.byte	0x5
	.uleb128 0x29a
	.4byte	.LASF1458
	.byte	0x5
	.uleb128 0x2a5
	.4byte	.LASF1459
	.byte	0x5
	.uleb128 0x2b5
	.4byte	.LASF1460
	.byte	0x5
	.uleb128 0x2ba
	.4byte	.LASF1461
	.byte	0x5
	.uleb128 0x2c2
	.4byte	.LASF1462
	.byte	0x5
	.uleb128 0x2c3
	.4byte	.LASF1463
	.byte	0x5
	.uleb128 0x2c4
	.4byte	.LASF1464
	.byte	0x5
	.uleb128 0x2c5
	.4byte	.LASF1465
	.byte	0x5
	.uleb128 0x2c6
	.4byte	.LASF1466
	.byte	0x5
	.uleb128 0x2c8
	.4byte	.LASF1467
	.byte	0x5
	.uleb128 0x2cc
	.4byte	.LASF1468
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.mutex.h.19.84c9e3da130860eb4fcd62ac4b5ced1c,comdat
.Ldebug_macro23:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1470
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1471
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF1472
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.malloc.h.15.1a961020f2b904cd14df059f9a4e1062,comdat
.Ldebug_macro24:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1476
	.byte	0x5
	.uleb128 0x39
	.4byte	.LASF1477
	.byte	0x5
	.uleb128 0x57
	.4byte	.LASF1478
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF1479
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF1480
	.byte	0x5
	.uleb128 0x99
	.4byte	.LASF1481
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF1482
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF1483
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_common.h.32.e3e16e2e9a1728f6eed2b7aa0525ca57,comdat
.Ldebug_macro25:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1485
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF1486
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF1487
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1488
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.util.h.6.47b426b7fb0b49a5af30a0a0aaf8ca95,comdat
.Ldebug_macro26:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1491
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF1492
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF1493
	.byte	0x5
	.uleb128 0xd
	.4byte	.LASF1494
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF1495
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1496
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF1497
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1498
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF1499
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1500
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF1501
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF1502
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF1503
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1504
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF1505
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF1506
	.byte	0x5
	.uleb128 0x52
	.4byte	.LASF1507
	.byte	0x5
	.uleb128 0x55
	.4byte	.LASF1508
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF1509
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF1510
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF1511
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF1512
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF1513
	.byte	0x5
	.uleb128 0x6c
	.4byte	.LASF1514
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF1515
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF1516
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF1517
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF1518
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF1519
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF1520
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF1521
	.byte	0x5
	.uleb128 0x82
	.4byte	.LASF1522
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF1523
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF1524
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF1525
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF1526
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF1527
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF1528
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_mmu_types.h.14.10626e8b0e7dae65b47a24ff553fcc0d,comdat
.Ldebug_macro27:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF1529
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1530
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF1531
	.byte	0x5
	.uleb128 0x11
	.4byte	.LASF1532
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1533
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1534
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1535
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF1536
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF1537
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF1538
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF1539
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF1540
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF1541
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1542
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF1543
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1544
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1545
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF1546
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1547
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF1548
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF1549
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF1550
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF1551
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF1552
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF1553
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF1554
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF1555
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF1556
	.byte	0x5
	.uleb128 0x49
	.4byte	.LASF1557
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF1558
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.user_ta_header.h.8.3a4ff74d5df5bf852dbc28541e19c80d,comdat
.Ldebug_macro28:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1559
	.byte	0x5
	.uleb128 0xd
	.4byte	.LASF1560
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF1561
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1562
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF1563
	.byte	0x5
	.uleb128 0x11
	.4byte	.LASF1564
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1565
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1566
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1567
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF1568
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1569
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF1570
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1571
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF1572
	.byte	0x5
	.uleb128 0x70
	.4byte	.LASF1573
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF1574
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF1575
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF1576
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF1577
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF1578
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_ta_manager.h.23.ef4eba23024ac593234d709a7038a9e7,comdat
.Ldebug_macro29:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF1580
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF1581
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.arm.h.14.28aed03834b3682f40ff3d6c9aaf7b32,comdat
.Ldebug_macro30:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF1585
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1586
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF1587
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1588
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1589
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1590
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF1591
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF1592
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF1593
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF1594
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1595
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF1596
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF1597
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1598
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1599
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF1600
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF1601
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1602
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF1603
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF1604
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF1605
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF1606
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1607
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF1608
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF1609
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF1610
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF1611
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF1612
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF1613
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF1614
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF1615
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF1616
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF1617
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF1618
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1619
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF1620
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF1621
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF1622
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF1623
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF1624
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF1625
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF1626
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF1627
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF1628
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF1629
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF1630
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF1631
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF1632
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF1633
	.byte	0x5
	.uleb128 0x47
	.4byte	.LASF1634
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF1635
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF1636
	.byte	0x5
	.uleb128 0x4e
	.4byte	.LASF1637
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF1638
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF1639
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF1640
	.byte	0x5
	.uleb128 0x56
	.4byte	.LASF1641
	.byte	0x5
	.uleb128 0x57
	.4byte	.LASF1642
	.byte	0x5
	.uleb128 0x58
	.4byte	.LASF1643
	.byte	0x5
	.uleb128 0x59
	.4byte	.LASF1644
	.byte	0x5
	.uleb128 0x5a
	.4byte	.LASF1645
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF1646
	.byte	0x5
	.uleb128 0x5c
	.4byte	.LASF1647
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF1648
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF1649
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF1650
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF1651
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF1652
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF1653
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF1654
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF1655
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF1656
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF1657
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF1658
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF1659
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF1660
	.byte	0x5
	.uleb128 0x6b
	.4byte	.LASF1661
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF1662
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF1663
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF1664
	.byte	0x5
	.uleb128 0x70
	.4byte	.LASF1665
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF1666
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF1667
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF1668
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF1669
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF1670
	.byte	0x5
	.uleb128 0x76
	.4byte	.LASF1671
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF1672
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF1673
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.cdefs.h.6.216810ff45474094d4f2557fb832b9f7,comdat
.Ldebug_macro31:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF1675
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1676
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF1677
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.arm32.h.15.9ddfabfb2811785b396fdc903a52cfd9,comdat
.Ldebug_macro32:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1678
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF1679
	.byte	0x5
	.uleb128 0x11
	.4byte	.LASF1680
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1681
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1682
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1683
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF1684
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF1685
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF1686
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF1687
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF1688
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1689
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF1690
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF1691
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF1692
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1693
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1694
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF1695
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1696
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF1697
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF1698
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF1699
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1700
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF1701
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF1702
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF1703
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF1704
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF1705
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF1706
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF1707
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF1708
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF1709
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF1710
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1711
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF1712
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF1713
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF1714
	.byte	0x5
	.uleb128 0x38
	.4byte	.LASF1715
	.byte	0x5
	.uleb128 0x39
	.4byte	.LASF1716
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF1717
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF1718
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF1719
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF1720
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF1721
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF1722
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF1723
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF1724
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF1725
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF1726
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF1727
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF1728
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF1729
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF1730
	.byte	0x5
	.uleb128 0x4e
	.4byte	.LASF1731
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF1732
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF1733
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF1734
	.byte	0x5
	.uleb128 0x52
	.4byte	.LASF1735
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF1736
	.byte	0x5
	.uleb128 0x55
	.4byte	.LASF1737
	.byte	0x5
	.uleb128 0x56
	.4byte	.LASF1738
	.byte	0x5
	.uleb128 0x57
	.4byte	.LASF1739
	.byte	0x5
	.uleb128 0x58
	.4byte	.LASF1740
	.byte	0x5
	.uleb128 0x59
	.4byte	.LASF1741
	.byte	0x5
	.uleb128 0x5a
	.4byte	.LASF1742
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF1743
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF1744
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF1745
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF1746
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF1747
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF1748
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF1749
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF1750
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF1751
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF1752
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF1753
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF1754
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF1755
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF1756
	.byte	0x5
	.uleb128 0x76
	.4byte	.LASF1757
	.byte	0x5
	.uleb128 0x77
	.4byte	.LASF1758
	.byte	0x5
	.uleb128 0x78
	.4byte	.LASF1759
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF1760
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF1761
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF1762
	.byte	0x5
	.uleb128 0x7c
	.4byte	.LASF1763
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF1764
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF1765
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF1766
	.byte	0x5
	.uleb128 0x82
	.4byte	.LASF1767
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF1768
	.byte	0x5
	.uleb128 0x86
	.4byte	.LASF1769
	.byte	0x5
	.uleb128 0x88
	.4byte	.LASF1770
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF1771
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF1772
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF1773
	.byte	0x5
	.uleb128 0x91
	.4byte	.LASF1774
	.byte	0x5
	.uleb128 0x92
	.4byte	.LASF1775
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF1776
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF1777
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF1778
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF1779
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF1780
	.byte	0x5
	.uleb128 0x9e
	.4byte	.LASF1781
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF1782
	.byte	0x5
	.uleb128 0xa2
	.4byte	.LASF1783
	.byte	0x5
	.uleb128 0xa3
	.4byte	.LASF1784
	.byte	0x5
	.uleb128 0xa4
	.4byte	.LASF1785
	.byte	0x5
	.uleb128 0xa7
	.4byte	.LASF1786
	.byte	0x5
	.uleb128 0xa8
	.4byte	.LASF1787
	.byte	0x5
	.uleb128 0xa9
	.4byte	.LASF1788
	.byte	0x5
	.uleb128 0xaa
	.4byte	.LASF1789
	.byte	0x5
	.uleb128 0xab
	.4byte	.LASF1790
	.byte	0x5
	.uleb128 0xad
	.4byte	.LASF1791
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.vfp.h.7.465fbcfc035103dd8779f8e70461126e,comdat
.Ldebug_macro33:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1793
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1794
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.thread_arch.h.27.e71628e9b7c1c895ebeaa5544ff07d3d,comdat
.Ldebug_macro34:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1795
	.byte	0x5
	.uleb128 0x12d
	.4byte	.LASF1796
	.byte	0x5
	.uleb128 0x12e
	.4byte	.LASF1797
	.byte	0x5
	.uleb128 0x130
	.4byte	.LASF1798
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.thread.h.19.0d4dd1120e1e7cb46be3176cdbb17453,comdat
.Ldebug_macro35:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1799
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1800
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF1801
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF1802
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF1803
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF1804
	.byte	0x5
	.uleb128 0x139
	.4byte	.LASF1805
	.byte	0x5
	.uleb128 0x13f
	.4byte	.LASF1806
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.sm.h.8.c3f12fbe8e7d86a062ceb31393d911f4,comdat
.Ldebug_macro36:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1809
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF1810
	.byte	0x5
	.uleb128 0x8e
	.4byte	.LASF1811
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF1812
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.thread_private_arch.h.19.1ca33e49c31dac42764770141789dfe1,comdat
.Ldebug_macro37:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1813
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF1814
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF1815
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1816
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF1817
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF1818
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_api_defines_extensions.h.8.cfe33d5e762c808c2c91a4d26c53769f,comdat
.Ldebug_macro38:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF1824
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF1825
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1826
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF1827
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF1828
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1829
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF1830
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF1831
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF1832
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1833
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF1834
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF1835
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF1836
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF1837
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1838
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF1839
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF1840
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF1841
	.byte	0x5
	.uleb128 0x38
	.4byte	.LASF1842
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF1843
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF1844
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF1845
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF1846
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF1847
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF1848
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF1849
	.byte	0x5
	.uleb128 0x4b
	.4byte	.LASF1850
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF1851
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF1852
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF1853
	.byte	0x5
	.uleb128 0x58
	.4byte	.LASF1854
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF1855
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF1856
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF1857
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF1858
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF1859
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF1860
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.utee_defines.h.18.6f4eacd906097fb5b8a9295cfc5d01c1,comdat
.Ldebug_macro39:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1861
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1862
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1863
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF1864
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF1865
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF1866
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF1867
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF1868
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF1869
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF1870
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF1871
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF1872
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF1873
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF1874
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF1875
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF1876
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF1877
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1878
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF1879
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF1880
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF1881
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF1882
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1883
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF1884
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF1885
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF1886
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF1887
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF1888
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF1889
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF1890
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF1891
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF1892
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1893
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF1894
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF1895
	.byte	0x5
	.uleb128 0x47
	.4byte	.LASF1896
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF1897
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF1898
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF1899
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF1900
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF1901
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF1902
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF1903
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF1904
	.byte	0x5
	.uleb128 0x93
	.4byte	.LASF1905
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF1906
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF1907
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF1908
	.byte	0x5
	.uleb128 0x9c
	.4byte	.LASF1909
	.byte	0x5
	.uleb128 0x9e
	.4byte	.LASF1910
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF1911
	.byte	0x5
	.uleb128 0xb0
	.4byte	.LASF1912
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF1913
	.byte	0x5
	.uleb128 0xb2
	.4byte	.LASF1914
	.byte	0x5
	.uleb128 0xdf
	.4byte	.LASF1915
	.byte	0x5
	.uleb128 0xe9
	.4byte	.LASF1916
	.byte	0x5
	.uleb128 0xf7
	.4byte	.LASF1917
	.byte	0x5
	.uleb128 0x101
	.4byte	.LASF1918
	.byte	0x5
	.uleb128 0x109
	.4byte	.LASF1919
	.byte	0x5
	.uleb128 0x10a
	.4byte	.LASF1920
	.byte	0x5
	.uleb128 0x10b
	.4byte	.LASF1921
	.byte	0x5
	.uleb128 0x10c
	.4byte	.LASF1922
	.byte	0x5
	.uleb128 0x10d
	.4byte	.LASF1923
	.byte	0x5
	.uleb128 0x10e
	.4byte	.LASF1924
	.byte	0x5
	.uleb128 0x10f
	.4byte	.LASF1925
	.byte	0x5
	.uleb128 0x110
	.4byte	.LASF1926
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF1927
	.byte	0x5
	.uleb128 0x112
	.4byte	.LASF1928
	.byte	0x5
	.uleb128 0x113
	.4byte	.LASF1929
	.byte	0x5
	.uleb128 0x114
	.4byte	.LASF1930
	.byte	0x5
	.uleb128 0x116
	.4byte	.LASF1931
	.byte	0x5
	.uleb128 0x118
	.4byte	.LASF1932
	.byte	0x5
	.uleb128 0x11d
	.4byte	.LASF1933
	.byte	0x5
	.uleb128 0x122
	.4byte	.LASF1934
	.byte	0x5
	.uleb128 0x12b
	.4byte	.LASF1935
	.byte	0x5
	.uleb128 0x138
	.4byte	.LASF1936
	.byte	0x5
	.uleb128 0x139
	.4byte	.LASF1937
	.byte	0x5
	.uleb128 0x13a
	.4byte	.LASF1938
	.byte	0x5
	.uleb128 0x13b
	.4byte	.LASF1939
	.byte	0x5
	.uleb128 0x13c
	.4byte	.LASF1940
	.byte	0x5
	.uleb128 0x13e
	.4byte	.LASF1941
	.byte	0x5
	.uleb128 0x13f
	.4byte	.LASF1942
	.byte	0x5
	.uleb128 0x140
	.4byte	.LASF1943
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_mm.h.7.c53beb94045541f300207b891a1fd448,comdat
.Ldebug_macro40:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1945
	.byte	0x5
	.uleb128 0xd
	.4byte	.LASF1946
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF1947
	.byte	0x5
	.uleb128 0x11
	.4byte	.LASF1948
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.config.h.7.f4b52b790ceaf643628f91a459235078,comdat
.Ldebug_macro41:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1950
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF1951
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF1952
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF1953
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF1954
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF1955
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF1956
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.generic_ram_layout.h.7.6b600de3f6952b170946c6e42c270083,comdat
.Ldebug_macro42:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF1958
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF1959
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF1960
	.byte	0x5
	.uleb128 0x90
	.4byte	.LASF1961
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF1962
	.byte	0x5
	.uleb128 0xa0
	.4byte	.LASF1963
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF1964
	.byte	0x5
	.uleb128 0xa5
	.4byte	.LASF1965
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF1966
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.platform_config.h.12.1eb17658e5d0e5573d298c5d4d4323cf,comdat
.Ldebug_macro43:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF1967
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF1968
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF1969
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF1970
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF1971
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF1972
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF1973
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF1974
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF1975
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF1976
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF1977
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF1978
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF1979
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF1980
	.byte	0x5
	.uleb128 0x38
	.4byte	.LASF1981
	.byte	0x5
	.uleb128 0x39
	.4byte	.LASF1982
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF1983
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF1984
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF1985
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF1986
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF1987
	.byte	0x5
	.uleb128 0x47
	.4byte	.LASF1988
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF1989
	.byte	0x5
	.uleb128 0x49
	.4byte	.LASF1990
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF1991
	.byte	0x5
	.uleb128 0x4b
	.4byte	.LASF1992
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF1993
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF1994
	.byte	0x5
	.uleb128 0x4e
	.4byte	.LASF1995
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF1996
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF1997
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF1998
	.byte	0x5
	.uleb128 0x52
	.4byte	.LASF1999
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF2000
	.byte	0x5
	.uleb128 0x54
	.4byte	.LASF2001
	.byte	0x5
	.uleb128 0x55
	.4byte	.LASF2002
	.byte	0x5
	.uleb128 0x56
	.4byte	.LASF2003
	.byte	0x5
	.uleb128 0x57
	.4byte	.LASF2004
	.byte	0x5
	.uleb128 0x58
	.4byte	.LASF2005
	.byte	0x5
	.uleb128 0x59
	.4byte	.LASF2006
	.byte	0x5
	.uleb128 0x5a
	.4byte	.LASF2007
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF2008
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF2009
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF2010
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF2011
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF2012
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF2013
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF2014
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF2015
	.byte	0x5
	.uleb128 0x6b
	.4byte	.LASF2016
	.byte	0x5
	.uleb128 0x6c
	.4byte	.LASF2017
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF2018
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF2019
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF2020
	.byte	0x5
	.uleb128 0x70
	.4byte	.LASF2021
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF2022
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF2023
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF2024
	.byte	0x5
	.uleb128 0x78
	.4byte	.LASF2025
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF2026
	.byte	0x5
	.uleb128 0x7c
	.4byte	.LASF2027
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF2028
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF2029
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF2030
	.byte	0x5
	.uleb128 0x86
	.4byte	.LASF2031
	.byte	0x5
	.uleb128 0x87
	.4byte	.LASF2032
	.byte	0x5
	.uleb128 0x88
	.4byte	.LASF2033
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF2034
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF2035
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF2036
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF2037
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF2038
	.byte	0x5
	.uleb128 0x8e
	.4byte	.LASF2039
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF2040
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF2041
	.byte	0x5
	.uleb128 0x9c
	.4byte	.LASF2042
	.byte	0x5
	.uleb128 0x9d
	.4byte	.LASF2043
	.byte	0x5
	.uleb128 0x9e
	.4byte	.LASF2044
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF2045
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF2046
	.byte	0x5
	.uleb128 0xa2
	.4byte	.LASF2047
	.byte	0x5
	.uleb128 0xa8
	.4byte	.LASF2048
	.byte	0x5
	.uleb128 0xa9
	.4byte	.LASF2049
	.byte	0x5
	.uleb128 0xab
	.4byte	.LASF2050
	.byte	0x5
	.uleb128 0xac
	.4byte	.LASF2051
	.byte	0x5
	.uleb128 0xad
	.4byte	.LASF2052
	.byte	0x5
	.uleb128 0xae
	.4byte	.LASF2053
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF2054
	.byte	0x5
	.uleb128 0xb0
	.4byte	.LASF2055
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF2056
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF2057
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF2058
	.byte	0x5
	.uleb128 0xb7
	.4byte	.LASF2059
	.byte	0x5
	.uleb128 0xb8
	.4byte	.LASF2060
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF2061
	.byte	0x5
	.uleb128 0xbb
	.4byte	.LASF2062
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF2063
	.byte	0x5
	.uleb128 0xbf
	.4byte	.LASF2064
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF2065
	.byte	0x5
	.uleb128 0xcf
	.4byte	.LASF2066
	.byte	0x5
	.uleb128 0xd0
	.4byte	.LASF2067
	.byte	0x5
	.uleb128 0xd1
	.4byte	.LASF2068
	.byte	0x5
	.uleb128 0xd5
	.4byte	.LASF2069
	.byte	0x5
	.uleb128 0xd8
	.4byte	.LASF2070
	.byte	0x5
	.uleb128 0xd9
	.4byte	.LASF2071
	.byte	0x5
	.uleb128 0xda
	.4byte	.LASF2072
	.byte	0x5
	.uleb128 0xdc
	.4byte	.LASF2073
	.byte	0x5
	.uleb128 0xdd
	.4byte	.LASF2074
	.byte	0x5
	.uleb128 0xde
	.4byte	.LASF2075
	.byte	0x5
	.uleb128 0xe0
	.4byte	.LASF2076
	.byte	0x5
	.uleb128 0xe2
	.4byte	.LASF2077
	.byte	0x5
	.uleb128 0xe3
	.4byte	.LASF2078
	.byte	0x5
	.uleb128 0xe4
	.4byte	.LASF2079
	.byte	0x5
	.uleb128 0xe6
	.4byte	.LASF2080
	.byte	0x5
	.uleb128 0xee
	.4byte	.LASF2081
	.byte	0x5
	.uleb128 0xf2
	.4byte	.LASF2082
	.byte	0x5
	.uleb128 0xf3
	.4byte	.LASF2083
	.byte	0x5
	.uleb128 0xf5
	.4byte	.LASF2084
	.byte	0x5
	.uleb128 0xf6
	.4byte	.LASF2085
	.byte	0x5
	.uleb128 0xf7
	.4byte	.LASF2086
	.byte	0x5
	.uleb128 0xff
	.4byte	.LASF2087
	.byte	0x5
	.uleb128 0x100
	.4byte	.LASF2088
	.byte	0x5
	.uleb128 0x101
	.4byte	.LASF2089
	.byte	0x5
	.uleb128 0x102
	.4byte	.LASF2090
	.byte	0x5
	.uleb128 0x105
	.4byte	.LASF2091
	.byte	0x5
	.uleb128 0x106
	.4byte	.LASF2092
	.byte	0x5
	.uleb128 0x107
	.4byte	.LASF2093
	.byte	0x5
	.uleb128 0x108
	.4byte	.LASF2094
	.byte	0x5
	.uleb128 0x10e
	.4byte	.LASF2095
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF2096
	.byte	0x5
	.uleb128 0x112
	.4byte	.LASF2097
	.byte	0x5
	.uleb128 0x115
	.4byte	.LASF2098
	.byte	0x5
	.uleb128 0x116
	.4byte	.LASF2099
	.byte	0x5
	.uleb128 0x117
	.4byte	.LASF2100
	.byte	0x5
	.uleb128 0x119
	.4byte	.LASF2101
	.byte	0x5
	.uleb128 0x11d
	.4byte	.LASF2102
	.byte	0x5
	.uleb128 0x124
	.4byte	.LASF2103
	.byte	0x5
	.uleb128 0x127
	.4byte	.LASF2104
	.byte	0x5
	.uleb128 0x12a
	.4byte	.LASF2105
	.byte	0x5
	.uleb128 0x12b
	.4byte	.LASF2106
	.byte	0x5
	.uleb128 0x12c
	.4byte	.LASF2107
	.byte	0x5
	.uleb128 0x12d
	.4byte	.LASF2108
	.byte	0x5
	.uleb128 0x12e
	.4byte	.LASF2109
	.byte	0x5
	.uleb128 0x12f
	.4byte	.LASF2110
	.byte	0x5
	.uleb128 0x130
	.4byte	.LASF2111
	.byte	0x5
	.uleb128 0x131
	.4byte	.LASF2112
	.byte	0x5
	.uleb128 0x132
	.4byte	.LASF2113
	.byte	0x5
	.uleb128 0x133
	.4byte	.LASF2114
	.byte	0x5
	.uleb128 0x134
	.4byte	.LASF2115
	.byte	0x5
	.uleb128 0x135
	.4byte	.LASF2116
	.byte	0x5
	.uleb128 0x137
	.4byte	.LASF2117
	.byte	0x5
	.uleb128 0x138
	.4byte	.LASF2118
	.byte	0x5
	.uleb128 0x139
	.4byte	.LASF2119
	.byte	0x5
	.uleb128 0x13a
	.4byte	.LASF2120
	.byte	0x5
	.uleb128 0x13b
	.4byte	.LASF2121
	.byte	0x5
	.uleb128 0x13c
	.4byte	.LASF2122
	.byte	0x5
	.uleb128 0x13d
	.4byte	.LASF2123
	.byte	0x5
	.uleb128 0x13e
	.4byte	.LASF2124
	.byte	0x5
	.uleb128 0x13f
	.4byte	.LASF2125
	.byte	0x5
	.uleb128 0x140
	.4byte	.LASF2126
	.byte	0x5
	.uleb128 0x141
	.4byte	.LASF2127
	.byte	0x5
	.uleb128 0x142
	.4byte	.LASF2128
	.byte	0x5
	.uleb128 0x14b
	.4byte	.LASF2129
	.byte	0x5
	.uleb128 0x14c
	.4byte	.LASF2130
	.byte	0x5
	.uleb128 0x14d
	.4byte	.LASF2131
	.byte	0x5
	.uleb128 0x14e
	.4byte	.LASF2132
	.byte	0x5
	.uleb128 0x151
	.4byte	.LASF2133
	.byte	0x5
	.uleb128 0x154
	.4byte	.LASF2134
	.byte	0x5
	.uleb128 0x155
	.4byte	.LASF2135
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.core_mmu_arch.h.39.e9e7d1555ae8239de2401e105050641e,comdat
.Ldebug_macro44:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF2136
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF2137
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF2138
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF2139
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF2140
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF2141
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF2142
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF2143
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF2144
	.byte	0x5
	.uleb128 0x4e
	.4byte	.LASF2145
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF2146
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF2147
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF2148
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.core_mmu.h.22.e94cbd64879abb2f2bedbf42b0ae21ec,comdat
.Ldebug_macro45:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF2149
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF2150
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF2151
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF2152
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF2153
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF2154
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF2155
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF2156
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF2147
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF2148
	.byte	0x5
	.uleb128 0x9e
	.4byte	.LASF2157
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF2158
	.byte	0x5
	.uleb128 0xc5
	.4byte	.LASF2159
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF2160
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF2161
	.byte	0x5
	.uleb128 0xd2
	.4byte	.LASF2162
	.byte	0x5
	.uleb128 0xdc
	.4byte	.LASF2163
	.byte	0x5
	.uleb128 0xe2
	.4byte	.LASF2164
	.byte	0x5
	.uleb128 0xf5
	.4byte	.LASF2165
	.byte	0x5
	.uleb128 0xf9
	.4byte	.LASF2166
	.byte	0x5
	.uleb128 0xfc
	.4byte	.LASF2167
	.byte	0x5
	.uleb128 0xff
	.4byte	.LASF2168
	.byte	0x5
	.uleb128 0x102
	.4byte	.LASF2169
	.byte	0x5
	.uleb128 0x105
	.4byte	.LASF2170
	.byte	0x5
	.uleb128 0x108
	.4byte	.LASF2171
	.byte	0x5
	.uleb128 0x10b
	.4byte	.LASF2172
	.byte	0x5
	.uleb128 0x10e
	.4byte	.LASF2173
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.thread_private.h.59.b710a0370676ee44133195e3b04a6acb,comdat
.Ldebug_macro46:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF2174
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF2175
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF2176
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF2177
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF2178
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF2179
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF2180
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF2181
	.byte	0x5
	.uleb128 0x44
	.4byte	.LASF2182
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF2183
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF2184
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF2185
	.byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF888:
	.ascii	"TEE_INT_CORE_API_SPEC_VERSION 0x0000000A\000"
.LASF310:
	.ascii	"__LACCUM_EPSILON__ 0x1P-31LK\000"
.LASF416:
	.ascii	"__thumb__ 1\000"
.LASF1830:
	.ascii	"TEE_ALG_HKDF_SHA256_DERIVE_KEY 0x800040C0\000"
.LASF1386:
	.ascii	"SLIST_REMOVE(head,elm,type,field) do { if ((head)->"
	.ascii	"slh_first == (elm)) { SLIST_REMOVE_HEAD((head), fie"
	.ascii	"ld); } else { struct type *curelm = (head)->slh_fir"
	.ascii	"st; while(curelm->field.sle_next != (elm)) curelm ="
	.ascii	" curelm->field.sle_next; curelm->field.sle_next = c"
	.ascii	"urelm->field.sle_next->field.sle_next; } } while ( "
	.ascii	"0)\000"
.LASF1209:
	.ascii	"TEE_PANIC_ID_TEE_GETOPERATIONINFOMULTIPLE 0x00000C0"
	.ascii	"8\000"
.LASF414:
	.ascii	"__APCS_32__ 1\000"
.LASF1327:
	.ascii	"finalcall_end SCATTERED_ARRAY_END(finalcall, struct"
	.ascii	" initcall)\000"
.LASF1625:
	.ascii	"MPIDR_AFF0_SHIFT U(0)\000"
.LASF270:
	.ascii	"__LFRACT_EPSILON__ 0x1P-31LR\000"
.LASF32:
	.ascii	"__FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF2192:
	.ascii	"unsigned int\000"
.LASF1449:
	.ascii	"QUEUEDEBUG_CIRCLEQ_ELM(head,elm,field) \000"
.LASF1418:
	.ascii	"SIMPLEQ_FOREACH(var,head,field) for ((var) = ((head"
	.ascii	")->sqh_first); (var); (var) = ((var)->field.sqe_nex"
	.ascii	"t))\000"
.LASF1056:
	.ascii	"TEE_TYPE_DES 0xA0000011\000"
.LASF528:
	.ascii	"CFG_KERN_LINKER_FORMAT elf32-littlearm\000"
.LASF157:
	.ascii	"__FLT_NORM_MAX__ 3.4028234663852886e+38F\000"
.LASF2053:
	.ascii	"UART6_BASE 0x44003000\000"
.LASF1564:
	.ascii	"TA_FLAG_INSTANCE_KEEP_ALIVE (1 << 4)\000"
.LASF293:
	.ascii	"__USACCUM_MIN__ 0.0UHK\000"
.LASF447:
	.ascii	"__ARM_FEATURE_MATMUL_INT8\000"
.LASF1077:
	.ascii	"TEE_TYPE_SM2_DSA_PUBLIC_KEY 0xA0000045\000"
.LASF1423:
	.ascii	"TAILQ_HEAD(name,type) _TAILQ_HEAD(name, struct type"
	.ascii	",)\000"
.LASF1951:
	.ascii	"IS_ENABLED(config_macro) Z_IS_ENABLED1(config_macro"
	.ascii	")\000"
.LASF199:
	.ascii	"__FLT32_DIG__ 6\000"
.LASF175:
	.ascii	"__DBL_EPSILON__ ((double)2.2204460492503131e-16L)\000"
.LASF589:
	.ascii	"CFG_STM32_RTC 1\000"
.LASF688:
	.ascii	"barrier() asm volatile (\"\" : : : \"memory\")\000"
.LASF302:
	.ascii	"__UACCUM_IBIT__ 16\000"
.LASF606:
	.ascii	"CFG_TA_MBEDTLS_SELF_TEST 1\000"
.LASF725:
	.ascii	"SHRT_MAX __SHRT_MAX__\000"
.LASF1341:
	.ascii	"__ssize_t_defined \000"
.LASF130:
	.ascii	"__INT_FAST16_MAX__ 0x7fffffff\000"
.LASF615:
	.ascii	"CFG_TEE_IMPL_DESCR OPTEE\000"
.LASF21:
	.ascii	"__SIZEOF_SHORT__ 2\000"
.LASF1531:
	.ascii	"TEE_MATTR_PR BIT(4)\000"
.LASF214:
	.ascii	"__FLT64_MANT_DIG__ 53\000"
.LASF1367:
	.ascii	"QUEUEDEBUG_LIST_OP(elm,field) \000"
.LASF799:
	.ascii	"PRId64 __PRI64_PREFIX \"d\"\000"
.LASF1968:
	.ascii	"MAX_XLAT_TABLES 10\000"
.LASF2013:
	.ascii	"I2C4_BASE 0x5c002000\000"
.LASF154:
	.ascii	"__FLT_MAX_10_EXP__ 38\000"
.LASF1788:
	.ascii	"CNTCVL_OFFSET 0x008\000"
.LASF97:
	.ascii	"__SIG_ATOMIC_MAX__ 0x7fffffff\000"
.LASF1388:
	.ascii	"SLIST_FOREACH(var,head,field) for((var) = (head)->s"
	.ascii	"lh_first; (var); (var) = (var)->field.sle_next)\000"
.LASF1378:
	.ascii	"LIST_FOREACH_SAFE(var,head,field,tvar) for ((var) ="
	.ascii	" LIST_FIRST((head)); (var) && ((tvar) = LIST_NEXT(("
	.ascii	"var), field), 1); (var) = (tvar))\000"
.LASF1499:
	.ascii	"MIN(a,b) (__extension__({ __typeof__(a) _a = (a); _"
	.ascii	"_typeof__(b) _b = (b); _a < _b ? _a : _b; }))\000"
.LASF645:
	.ascii	"__KERNEL_BOOT_H \000"
.LASF1079:
	.ascii	"TEE_TYPE_SM2_KEP_PUBLIC_KEY 0xA0000046\000"
.LASF1339:
	.ascii	"__TYPES_EXT_H \000"
.LASF619:
	.ascii	"CFG_TZDRAM_SIZE 0x02000000\000"
.LASF1734:
	.ascii	"NSACR_NSASEDIS BIT32(15)\000"
.LASF2080:
	.ascii	"STM32MP_GIC_PRIORITY_CSTOP U(0xC0)\000"
.LASF211:
	.ascii	"__FLT32_HAS_INFINITY__ 1\000"
.LASF1540:
	.ascii	"TEE_MATTR_URW (TEE_MATTR_UR | TEE_MATTR_UW)\000"
.LASF341:
	.ascii	"__USQ_IBIT__ 0\000"
.LASF1849:
	.ascii	"TEE_ATTR_PBKDF2_PASSWORD 0xC00001C2\000"
.LASF1098:
	.ascii	"TEE_ATTR_DSA_SUBPRIME 0xD0001131\000"
.LASF81:
	.ascii	"__PTRDIFF_MAX__ 0x7fffffff\000"
.LASF788:
	.ascii	"UINT32_C(v) U(v)\000"
.LASF1518:
	.ascii	"BIT32(nr) (UINT32_C(1) << (nr))\000"
.LASF217:
	.ascii	"__FLT64_MIN_10_EXP__ (-307)\000"
.LASF756:
	.ascii	"UINT_LEAST16_MAX UINT16_MAX\000"
.LASF1145:
	.ascii	"TEE_PANIC_ID_TEE_FREEPROPERTYENUMERATOR 0x00000202\000"
.LASF542:
	.ascii	"CFG_PREALLOC_RPC_CACHE 1\000"
.LASF1427:
	.ascii	"QUEUEDEBUG_TAILQ_INSERT_HEAD(head,elm,field) \000"
.LASF1269:
	.ascii	"TEE_PARAM_TYPE_SET(t,i) (((uint32_t)(t) & 0xF) << ("
	.ascii	"(i)*4))\000"
.LASF1646:
	.ascii	"CTR_DMINLINE_WIDTH U(4)\000"
.LASF1642:
	.ascii	"CTR_CWG_MASK U(0xf)\000"
.LASF2127:
	.ascii	"STM32MP151F_PART_NB U(0x050000AE)\000"
.LASF0:
	.ascii	"__STDC__ 1\000"
.LASF1432:
	.ascii	"TAILQ_INIT(head) do { (head)->tqh_first = NULL; (he"
	.ascii	"ad)->tqh_last = &(head)->tqh_first; } while ( 0)\000"
.LASF2197:
	.ascii	"_Bool\000"
.LASF1606:
	.ascii	"CORTEX_A73_PART_NUM U(0xD09)\000"
.LASF1202:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATEOPERATION 0x00000C01\000"
.LASF1029:
	.ascii	"TEE_ALG_SHA3_256 0x50000009\000"
.LASF282:
	.ascii	"__ULLFRACT_IBIT__ 0\000"
.LASF703:
	.ascii	"SCATTERED_ARRAY_DEFINE_ITEM_ORDERED(array_name,orde"
	.ascii	"r,element_type) __SCT_ARRAY_DEF_ITEM1(array_name, o"
	.ascii	"rder, __COUNTER__, element_type)\000"
.LASF1644:
	.ascii	"CTR_ERG_MASK U(0xf)\000"
.LASF202:
	.ascii	"__FLT32_MAX_EXP__ 128\000"
.LASF348:
	.ascii	"__SA_FBIT__ 15\000"
.LASF1844:
	.ascii	"TEE_ATTR_CONCAT_KDF_Z 0xC00001C1\000"
.LASF467:
	.ascii	"CFG_ARM32_core 1\000"
.LASF358:
	.ascii	"__UDA_FBIT__ 32\000"
.LASF1134:
	.ascii	"TEE_ECC_CURVE_NIST_P256 0x00000003\000"
.LASF1758:
	.ascii	"TTBCR_IRGN0_SHIFT U(8)\000"
.LASF1612:
	.ascii	"CORTEX_A77_PART_NUM U(0xD0D)\000"
.LASF1604:
	.ascii	"CORTEX_A57_PART_NUM U(0xD07)\000"
.LASF52:
	.ascii	"__UINT64_TYPE__ long long unsigned int\000"
.LASF170:
	.ascii	"__DBL_MAX_10_EXP__ 308\000"
.LASF646:
	.ascii	"INITCALL_H \000"
.LASF2130:
	.ascii	"STM32MP1_REV_A U(0x00001000)\000"
.LASF139:
	.ascii	"__UINT_FAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF1825:
	.ascii	"TEE_ERROR_DEFER_DRIVER_INIT 0x80000000\000"
.LASF1008:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA512 0x70616930\000"
.LASF1883:
	.ascii	"TEE_MAIN_ALGO_CONCAT_KDF 0xC1\000"
.LASF38:
	.ascii	"__WCHAR_TYPE__ unsigned int\000"
.LASF349:
	.ascii	"__SA_IBIT__ 16\000"
.LASF359:
	.ascii	"__UDA_IBIT__ 32\000"
.LASF1991:
	.ascii	"DCMIPP_BASE 0x5a000000\000"
.LASF1861:
	.ascii	"TEE_MAIN_ALGO_MD5 0x01\000"
.LASF459:
	.ascii	"__ELF__ 1\000"
.LASF19:
	.ascii	"__SIZEOF_LONG__ 4\000"
.LASF1978:
	.ascii	"APB5_SIZE 0x0000b000\000"
.LASF412:
	.ascii	"__ARM_ARCH 7\000"
.LASF990:
	.ascii	"TEE_ALG_DES3_ECB_NOPAD 0x10000013\000"
.LASF624:
	.ascii	"CFG_WDT_SM_HANDLER 1\000"
.LASF1488:
	.ascii	"tee_mtime_perftest() do { } while (0)\000"
.LASF2070:
	.ascii	"GIC_SIZE 0x2000\000"
.LASF1557:
	.ascii	"VM_FLAG_READONLY BIT(4)\000"
.LASF494:
	.ascii	"CFG_CRYPTOLIB_DIR core/lib/libtomcrypt\000"
.LASF1337:
	.ascii	"release_init_resource(fn) __define_initcall(init, 7"
	.ascii	", fn)\000"
.LASF2006:
	.ascii	"GPIOG_BASE 0x50008000\000"
.LASF1990:
	.ascii	"DBGMCU_BASE 0x50081000\000"
.LASF1492:
	.ascii	"SIZE_4K UINTPTR_C(0x1000)\000"
.LASF1162:
	.ascii	"TEE_PANIC_ID_TEE_UNMASKCANCELLATION 0x00000503\000"
.LASF865:
	.ascii	"__WCHAR_T__ \000"
.LASF1266:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTTOFMM 0x00001C03\000"
.LASF654:
	.ascii	"__aligned(x) __attribute__((aligned(x)))\000"
.LASF2169:
	.ascii	"phys_ddr_overall_compat_end SCATTERED_ARRAY_END(phy"
	.ascii	"s_ddr_overall_compat, struct core_mmu_phys_mem)\000"
.LASF1478:
	.ascii	"TEE_ALLOCATOR_DESC_LENGTH 32\000"
.LASF1282:
	.ascii	"__need___va_list\000"
.LASF1511:
	.ascii	"IS_ALIGNED_WITH_TYPE(x,type) (__extension__({ type "
	.ascii	"__is_aligned_y; IS_ALIGNED((uintptr_t)(x), __aligno"
	.ascii	"f__(__is_aligned_y)); }))\000"
.LASF2158:
	.ascii	"__register_memory(_name,_type,_addr,_size,_section)"
	.ascii	" SCATTERED_ARRAY_DEFINE_ITEM(_section, struct core_"
	.ascii	"mmu_phys_mem) = { .name = (_name), .type = (_type),"
	.ascii	" .addr = (_addr), .size = (_size) }\000"
.LASF1502:
	.ascii	"ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))\000"
.LASF1387:
	.ascii	"SLIST_REMOVE_AFTER(slistelm,field) do { (slistelm)-"
	.ascii	">field.sle_next = SLIST_NEXT(SLIST_NEXT((slistelm),"
	.ascii	" field), field); } while ( 0)\000"
.LASF1956:
	.ascii	"Z_IS_ENABLED3(ignore_this,val,...) val\000"
.LASF826:
	.ascii	"_STDBOOL_H \000"
.LASF723:
	.ascii	"SCHAR_MAX __SCHAR_MAX__\000"
.LASF1582:
	.ascii	"PGT_CACHE_SIZE ROUNDUP(CFG_NUM_THREADS * 2, PGT_NUM"
	.ascii	"_PGT_PER_PAGE)\000"
.LASF1116:
	.ascii	"TEE_ATTR_SM2_KEP_USER 0xF0000646\000"
.LASF1558:
	.ascii	"VM_FLAGS_NONPRIV (VM_FLAG_EPHEMERAL | VM_FLAG_PERMA"
	.ascii	"NENT | VM_FLAG_SHAREABLE)\000"
.LASF774:
	.ascii	"INTMAX_MAX INT64_MAX\000"
.LASF634:
	.ascii	"PLATFORM_stm32mp1 1\000"
.LASF69:
	.ascii	"__INTPTR_TYPE__ int\000"
.LASF332:
	.ascii	"__DQ_FBIT__ 63\000"
.LASF749:
	.ascii	"INT_LEAST32_MIN INT32_MIN\000"
.LASF305:
	.ascii	"__UACCUM_EPSILON__ 0x1P-16UK\000"
.LASF992:
	.ascii	"TEE_ALG_DES3_CBC_MAC_NOPAD 0x30000113\000"
.LASF1917:
	.ascii	"TEE_U32_BSWAP(x) __compiler_bswap32((x))\000"
.LASF131:
	.ascii	"__INT_FAST16_WIDTH__ 32\000"
.LASF762:
	.ascii	"INT_FAST64_MIN INT64_MIN\000"
.LASF1694:
	.ascii	"CPSR_IT_MASK1 ARM32_CPSR_IT_MASK1\000"
.LASF758:
	.ascii	"UINT_LEAST64_MAX UINT64_MAX\000"
.LASF162:
	.ascii	"__FLT_HAS_INFINITY__ 1\000"
.LASF1211:
	.ascii	"TEE_PANIC_ID_TEE_DIGESTUPDATE 0x00000D02\000"
.LASF1666:
	.ascii	"ARM32_CPSR_A BIT(8)\000"
.LASF1109:
	.ascii	"TEE_ATTR_RSA_PSS_SALT_LENGTH 0xF0000A30\000"
.LASF1399:
	.ascii	"STAILQ_INSERT_AFTER(head,listelm,elm,field) do { if"
	.ascii	" (((elm)->field.stqe_next = (listelm)->field.stqe_n"
	.ascii	"ext) == NULL) (head)->stqh_last = &(elm)->field.stq"
	.ascii	"e_next; (listelm)->field.stqe_next = (elm); } while"
	.ascii	" ( 0)\000"
.LASF1696:
	.ascii	"PMCR_DP BIT32(5)\000"
.LASF455:
	.ascii	"linux 1\000"
.LASF1158:
	.ascii	"TEE_PANIC_ID_TEE_INVOKETACOMMAND 0x00000402\000"
.LASF1998:
	.ascii	"FMC_BASE 0x58002000\000"
.LASF717:
	.ascii	"INT_MIN (-INT_MAX - 1)\000"
.LASF1546:
	.ascii	"TEE_MATTR_MEM_TYPE_MASK U(0x7)\000"
.LASF1160:
	.ascii	"TEE_PANIC_ID_TEE_GETCANCELLATIONFLAG 0x00000501\000"
.LASF1374:
	.ascii	"LIST_FOREACH(var,head,field) for ((var) = ((head)->"
	.ascii	"lh_first); (var); (var) = ((var)->field.le_next))\000"
.LASF2115:
	.ascii	"STM32MP131F_PART_NB U(0x05010EC8)\000"
.LASF1538:
	.ascii	"TEE_MATTR_UW BIT(8)\000"
.LASF1748:
	.ascii	"PAR_SS BIT32(1)\000"
.LASF1361:
	.ascii	"KERNEL_WAIT_QUEUE_H \000"
.LASF1946:
	.ascii	"TEE_MM_POOL_NO_FLAGS 0\000"
.LASF794:
	.ascii	"__PRI64_PREFIX \"ll\"\000"
.LASF523:
	.ascii	"CFG_HWRNG_RATE 0\000"
.LASF776:
	.ascii	"UINTMAX_MAX UINT64_MAX\000"
.LASF8:
	.ascii	"__VERSION__ \"12.3.0\"\000"
.LASF2108:
	.ascii	"STM32MP133A_PART_NB U(0x050100C1)\000"
.LASF1206:
	.ascii	"TEE_PANIC_ID_TEE_RESETOPERATION 0x00000C05\000"
.LASF420:
	.ascii	"__ARM_ARCH_ISA_THUMB 2\000"
.LASF824:
	.ascii	"PRIX64 __PRI64_PREFIX \"X\"\000"
.LASF1349:
	.ascii	"KERNEL_THREAD_H \000"
.LASF2120:
	.ascii	"STM32MP153A_PART_NB U(0x05000025)\000"
.LASF835:
	.ascii	"_T_PTRDIFF_ \000"
.LASF1512:
	.ascii	"TO_STR(x) _TO_STR(x)\000"
.LASF1182:
	.ascii	"TEE_PANIC_ID_TEE_GENERATEKEY 0x00000804\000"
.LASF1663:
	.ascii	"ARM32_CPSR_F_SHIFT U(6)\000"
.LASF531:
	.ascii	"CFG_LPAE_ADDR_SPACE_BITS 32\000"
.LASF602:
	.ascii	"CFG_TA_BIGNUM_MAX_BITS 2048\000"
.LASF1105:
	.ascii	"TEE_ATTR_DH_X_BITS 0xF0001332\000"
.LASF1207:
	.ascii	"TEE_PANIC_ID_TEE_SETOPERATIONKEY 0x00000C06\000"
.LASF765:
	.ascii	"INT_FAST32_MAX INT32_MAX\000"
.LASF1632:
	.ascii	"MPIDR_MT_MASK BIT(MPIDR_MT_SHIFT)\000"
.LASF1396:
	.ascii	"STAILQ_INIT(head) do { (head)->stqh_first = NULL; ("
	.ascii	"head)->stqh_last = &(head)->stqh_first; } while ( 0"
	.ascii	")\000"
.LASF1461:
	.ascii	"CIRCLEQ_FOREACH_REVERSE(var,head,field) for ((var) "
	.ascii	"= ((head)->cqh_last); (var) != (const void *)(head)"
	.ascii	"; (var) = ((var)->field.cqe_prev))\000"
.LASF113:
	.ascii	"__INT_LEAST16_WIDTH__ 16\000"
.LASF253:
	.ascii	"__USFRACT_MIN__ 0.0UHR\000"
.LASF534:
	.ascii	"CFG_MSG_LONG_PREFIX_MASK 0x1a\000"
.LASF2149:
	.ascii	"SMALL_PAGE_SIZE BIT(SMALL_PAGE_SHIFT)\000"
.LASF178:
	.ascii	"__DBL_HAS_INFINITY__ 1\000"
.LASF896:
	.ascii	"TEE_ERROR_CIPHERTEXT_INVALID 0xF0100006\000"
.LASF1618:
	.ascii	"CORTEX_X2_PART_NUM U(0xD48)\000"
.LASF1764:
	.ascii	"TTBCR_IRGN1_SHIFT U(24)\000"
.LASF1913:
	.ascii	"TEE_MAC_SIZE_AES_CMAC \000"
.LASF1291:
	.ascii	"_VA_LIST_DEFINED \000"
.LASF1107:
	.ascii	"TEE_ATTR_DH_PRIVATE_VALUE 0xC0000232\000"
.LASF789:
	.ascii	"INT64_C(v) LL(v)\000"
.LASF73:
	.ascii	"__SHRT_MAX__ 0x7fff\000"
.LASF938:
	.ascii	"TEE_ORIGIN_TEE 0x00000003\000"
.LASF986:
	.ascii	"TEE_ALG_DES_ECB_NOPAD 0x10000011\000"
.LASF469:
	.ascii	"CFG_ARM32_ta_arm32 1\000"
.LASF864:
	.ascii	"__wchar_t__ \000"
.LASF610:
	.ascii	"CFG_TEE_CORE_LOG_LEVEL 2\000"
.LASF1247:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTGETBITCOUNT 0x00001804\000"
.LASF1648:
	.ascii	"CTR_L1IP_SHIFT U(14)\000"
.LASF1026:
	.ascii	"TEE_ALG_SHA384 0x50000005\000"
.LASF502:
	.ascii	"CFG_DRAM_BASE 0xc0000000\000"
.LASF1718:
	.ascii	"SCTLR_UWXN BIT32(20)\000"
.LASF741:
	.ascii	"INT32_MAX 0x7fffffff\000"
.LASF1069:
	.ascii	"TEE_TYPE_DSA_KEYPAIR 0xA1000031\000"
.LASF681:
	.ascii	"__HAVE_SINGLE_ARGUMENT_STATIC_ASSERT 1\000"
.LASF1082:
	.ascii	"TEE_TYPE_SM2_PKE_KEYPAIR 0xA1000047\000"
.LASF102:
	.ascii	"__INT32_MAX__ 0x7fffffff\000"
.LASF1876:
	.ascii	"TEE_MAIN_ALGO_ECDSA 0x41\000"
.LASF1203:
	.ascii	"TEE_PANIC_ID_TEE_COPYOPERATION 0x00000C02\000"
.LASF1024:
	.ascii	"TEE_ALG_SHA224 0x50000003\000"
.LASF134:
	.ascii	"__INT_FAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF1653:
	.ascii	"ARM32_CPSR_MODE_MASK U(0x1f)\000"
.LASF318:
	.ascii	"__LLACCUM_MIN__ (-0X1P31LLK-0X1P31LLK)\000"
.LASF1970:
	.ascii	"APB1_SIZE 0x0001d000\000"
.LASF371:
	.ascii	"__GCC_ATOMIC_CHAR_LOCK_FREE 2\000"
.LASF66:
	.ascii	"__UINT_FAST16_TYPE__ unsigned int\000"
.LASF1032:
	.ascii	"TEE_ALG_MD5SHA1 0x5000000F\000"
.LASF1874:
	.ascii	"TEE_MAIN_ALGO_DSA 0x31\000"
.LASF2124:
	.ascii	"STM32MP157D_PART_NB U(0x05000081)\000"
.LASF1773:
	.ascii	"TTBCR_SHX_ISH U(0x3)\000"
.LASF1795:
	.ascii	"THREAD_CORE_LOCAL_ALIGNED __aligned(8)\000"
.LASF1076:
	.ascii	"TEE_TYPE_ED25519_KEYPAIR 0xA1000043\000"
.LASF107:
	.ascii	"__UINT64_MAX__ 0xffffffffffffffffULL\000"
.LASF1022:
	.ascii	"TEE_ALG_MD5 0x50000001\000"
.LASF18:
	.ascii	"__SIZEOF_INT__ 4\000"
.LASF622:
	.ascii	"CFG_WARN_INSECURE 1\000"
.LASF2140:
	.ascii	"CORE_MMU_PGDIR_LEVEL U(3)\000"
.LASF235:
	.ascii	"__FLT32X_MAX_10_EXP__ 308\000"
.LASF1111:
	.ascii	"TEE_ATTR_ECC_PUBLIC_VALUE_Y 0xD0000241\000"
.LASF2156:
	.ascii	"CORE_MMU_USER_PARAM_MASK ((paddr_t)CORE_MMU_USER_PA"
	.ascii	"RAM_SIZE - 1)\000"
.LASF1576:
	.ascii	"TA_PROP_STR_STACK_SIZE \"gpd.ta.stackSize\"\000"
.LASF1925:
	.ascii	"TEE_U64_FROM_BIG_ENDIAN(x) TEE_U64_BSWAP(x)\000"
.LASF917:
	.ascii	"TEE_ERROR_STORAGE_NO_SPACE 0xFFFF3041\000"
.LASF44:
	.ascii	"__SIG_ATOMIC_TYPE__ int\000"
.LASF856:
	.ascii	"_BSD_SIZE_T_DEFINED_ \000"
.LASF623:
	.ascii	"CFG_WDT 1\000"
.LASF1569:
	.ascii	"TA_FLAG_DEVICE_ENUM (1 << 9)\000"
.LASF1153:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYNAME 0x0000020A\000"
.LASF1774:
	.ascii	"TTBR_ASID_MASK U(0xff)\000"
.LASF237:
	.ascii	"__FLT32X_MAX__ 1.7976931348623157e+308F32x\000"
.LASF1905:
	.ascii	"TEE_DIGEST_HASH_TO_ALGO(algo) TEE_ALG_HASH_ALGO(TEE"
	.ascii	"_ALG_GET_DIGEST_HASH(algo))\000"
.LASF1735:
	.ascii	"NSACR_NS_L2ERR BIT32(17)\000"
.LASF1651:
	.ascii	"CTR_IMINLINE_MASK U(0xf)\000"
.LASF607:
	.ascii	"CFG_TA_STRICT_ANNOTATION_CHECKS 1\000"
.LASF123:
	.ascii	"__UINT16_C(c) c\000"
.LASF385:
	.ascii	"__PRAGMA_REDEFINE_EXTNAME 1\000"
.LASF56:
	.ascii	"__INT_LEAST64_TYPE__ long long int\000"
.LASF1765:
	.ascii	"TTBCR_ORGN1_SHIFT U(26)\000"
.LASF1862:
	.ascii	"TEE_MAIN_ALGO_SHA1 0x02\000"
.LASF695:
	.ascii	"__DECLARE_KEEP_INIT1(sym,file_id) __DECLARE_KEEP_IN"
	.ascii	"IT2(sym, file_id)\000"
.LASF1731:
	.ascii	"NSACR_CP10 BIT32(10)\000"
.LASF902:
	.ascii	"TEE_ERROR_BAD_FORMAT 0xFFFF0005\000"
.LASF2064:
	.ascii	"STM32MP1_UPPER_OTP_START 0x20U\000"
.LASF1021:
	.ascii	"TEE_ALG_SM2_KEP 0x60000045\000"
.LASF2090:
	.ascii	"USART6_BASE UART6_BASE\000"
.LASF2152:
	.ascii	"CORE_MMU_PGDIR_MASK ((paddr_t)CORE_MMU_PGDIR_SIZE -"
	.ascii	" 1)\000"
.LASF639:
	.ascii	"_CFG_CORE_LTC_VFP 1\000"
.LASF1267:
	.ascii	"TEE_PARAM_TYPES(t0,t1,t2,t3) ((t0) | ((t1) << 4) | "
	.ascii	"((t2) << 8) | ((t3) << 12))\000"
.LASF1286:
	.ascii	"va_arg(v,l) __builtin_va_arg(v,l)\000"
.LASF383:
	.ascii	"__HAVE_SPECULATION_SAFE_VALUE 1\000"
.LASF2046:
	.ascii	"TSC_BASE 0x5000B000\000"
.LASF421:
	.ascii	"__ARMEL__ 1\000"
.LASF1664:
	.ascii	"ARM32_CPSR_F BIT(6)\000"
.LASF2190:
	.ascii	"unsigned char\000"
.LASF1685:
	.ascii	"CPSR_MODE_UND ARM32_CPSR_MODE_UND\000"
.LASF779:
	.ascii	"UL(v) v ## UL\000"
.LASF326:
	.ascii	"__QQ_FBIT__ 7\000"
.LASF188:
	.ascii	"__LDBL_DECIMAL_DIG__ 17\000"
.LASF2183:
	.ascii	"THREAD_CLF_IRQ BIT(THREAD_CLF_IRQ_SHIFT)\000"
.LASF1931:
	.ascii	"TEE_TIME_MILLIS_BASE 1000\000"
.LASF241:
	.ascii	"__FLT32X_DENORM_MIN__ 4.9406564584124654e-324F32x\000"
.LASF1667:
	.ascii	"ARM32_CPSR_E BIT(9)\000"
.LASF1384:
	.ascii	"SLIST_INSERT_HEAD(head,elm,field) do { (elm)->field"
	.ascii	".sle_next = (head)->slh_first; (head)->slh_first = "
	.ascii	"(elm); } while ( 0)\000"
.LASF971:
	.ascii	"TEE_OPERATION_ASYMMETRIC_CIPHER 6\000"
.LASF1195:
	.ascii	"TEE_PANIC_ID_TEE_GETNEXTPERSISTENTOBJECT 0x00000A03"
	.ascii	"\000"
.LASF1293:
	.ascii	"__va_list__ \000"
.LASF1370:
	.ascii	"LIST_INSERT_AFTER(listelm,elm,field) do { QUEUEDEBU"
	.ascii	"G_LIST_OP((listelm), field) if (((elm)->field.le_ne"
	.ascii	"xt = (listelm)->field.le_next) != NULL) (listelm)->"
	.ascii	"field.le_next->field.le_prev = &(elm)->field.le_nex"
	.ascii	"t; (listelm)->field.le_next = (elm); (elm)->field.l"
	.ascii	"e_prev = &(listelm)->field.le_next; } while ( 0)\000"
.LASF27:
	.ascii	"__BIGGEST_ALIGNMENT__ 8\000"
.LASF1626:
	.ascii	"MPIDR_AFF0_MASK (MPIDR_AFFLVL_MASK << MPIDR_AFF0_SH"
	.ascii	"IFT)\000"
.LASF778:
	.ascii	"U(v) v ## U\000"
.LASF49:
	.ascii	"__UINT8_TYPE__ unsigned char\000"
.LASF1599:
	.ascii	"CORTEX_A7_PART_NUM U(0xC07)\000"
.LASF1073:
	.ascii	"TEE_TYPE_ECDH_PUBLIC_KEY 0xA0000042\000"
.LASF1759:
	.ascii	"TTBCR_ORGN0_SHIFT U(10)\000"
.LASF1230:
	.ascii	"TEE_PANIC_ID_TEE_GETREETIME 0x00001401\000"
.LASF201:
	.ascii	"__FLT32_MIN_10_EXP__ (-37)\000"
.LASF1263:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTRELATIVEPRIME 0x00001B03\000"
.LASF1690:
	.ascii	"CPSR_I ARM32_CPSR_I\000"
.LASF1898:
	.ascii	"TEE_ALG_GET_CHAIN_MODE(algo) (((algo) >> 8) & 0xF)\000"
.LASF729:
	.ascii	"UINT_MAX (INT_MAX * 2U + 1U)\000"
.LASF1952:
	.ascii	"IS_ENABLED2(config_macro) IS_ENABLED(config_macro)\000"
.LASF324:
	.ascii	"__ULLACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULLK\000"
.LASF1424:
	.ascii	"TAILQ_HEAD_INITIALIZER(head) { NULL, &(head).tqh_fi"
	.ascii	"rst }\000"
.LASF1435:
	.ascii	"TAILQ_INSERT_AFTER(head,listelm,elm,field) do { QUE"
	.ascii	"UEDEBUG_TAILQ_OP((listelm), field) if (((elm)->fiel"
	.ascii	"d.tqe_next = (listelm)->field.tqe_next) != NULL) (e"
	.ascii	"lm)->field.tqe_next->field.tqe_prev = &(elm)->field"
	.ascii	".tqe_next; else (head)->tqh_last = &(elm)->field.tq"
	.ascii	"e_next; (listelm)->field.tqe_next = (elm); (elm)->f"
	.ascii	"ield.tqe_prev = &(listelm)->field.tqe_next; } while"
	.ascii	" ( 0)\000"
.LASF411:
	.ascii	"__ARM_ARCH\000"
.LASF3:
	.ascii	"__STDC_UTF_32__ 1\000"
.LASF111:
	.ascii	"__INT_LEAST16_MAX__ 0x7fff\000"
.LASF904:
	.ascii	"TEE_ERROR_BAD_STATE 0xFFFF0007\000"
.LASF1237:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINIT 0x00001601\000"
.LASF1987:
	.ascii	"BKPSRAM_BASE 0x54000000\000"
.LASF1161:
	.ascii	"TEE_PANIC_ID_TEE_MASKCANCELLATION 0x00000502\000"
.LASF548:
	.ascii	"CFG_RPMB_FS_DEV_ID 0\000"
.LASF1746:
	.ascii	"DACR_DOMAIN_PERM_MANAGER U(0x3)\000"
.LASF608:
	.ascii	"CFG_TEE_API_VERSION GPD-1.1-dev\000"
.LASF1587:
	.ascii	"MIDR_PRIMARY_PART_NUM_MASK (BIT(MIDR_PRIMARY_PART_N"
	.ascii	"UM_WIDTH) - 1)\000"
.LASF1708:
	.ascii	"SCTLR_A BIT32(1)\000"
.LASF2180:
	.ascii	"THREAD_CLF_FIQ_SHIFT 3\000"
.LASF1072:
	.ascii	"TEE_TYPE_ECDSA_KEYPAIR 0xA1000041\000"
.LASF1469:
	.ascii	"WAIT_QUEUE_INITIALIZER { .slh_first = NULL }\000"
.LASF1885:
	.ascii	"TEE_MAIN_ALGO_X25519 0x44\000"
.LASF2016:
	.ascii	"IWDG2_BASE 0x5a002000\000"
.LASF792:
	.ascii	"INTMAX_C(v) INT64_C(v)\000"
.LASF287:
	.ascii	"__SACCUM_IBIT__ 8\000"
.LASF1900:
	.ascii	"TEE_ALG_GET_DIGEST_HASH(algo) __tee_alg_get_digest_"
	.ascii	"hash(algo)\000"
.LASF50:
	.ascii	"__UINT16_TYPE__ short unsigned int\000"
.LASF1513:
	.ascii	"_TO_STR(x) #x\000"
.LASF1278:
	.ascii	"TEE_SE_READER_NAME_MAX 20\000"
.LASF1920:
	.ascii	"TEE_U32_FROM_LITTLE_ENDIAN(x) ((uint32_t)(x))\000"
.LASF705:
	.ascii	"SCATTERED_ARRAY_DEFINE_ITEM(array_name,element_type"
	.ascii	") __SCT_ARRAY_DEF_ITEM1(array_name, 0, __COUNTER__,"
	.ascii	" element_type)\000"
.LASF1215:
	.ascii	"TEE_PANIC_ID_TEE_MACCOMPAREFINAL 0x00000F01\000"
.LASF585:
	.ascii	"CFG_STM32_PWR_SIP 1\000"
.LASF2139:
	.ascii	"CORE_MMU_PGDIR_SHIFT U(21)\000"
.LASF1655:
	.ascii	"ARM32_CPSR_MODE_FIQ U(0x11)\000"
.LASF2023:
	.ascii	"RCC_BASE 0x50000000\000"
.LASF721:
	.ascii	"LLONG_MIN (-LLONG_MAX - 1LL)\000"
.LASF281:
	.ascii	"__ULLFRACT_FBIT__ 64\000"
.LASF86:
	.ascii	"__LONG_WIDTH__ 32\000"
.LASF206:
	.ascii	"__FLT32_NORM_MAX__ 3.4028234663852886e+38F32\000"
.LASF1565:
	.ascii	"TA_FLAG_SECURE_DATA_PATH (1 << 5)\000"
.LASF941:
	.ascii	"TEE_PROPSET_CURRENT_CLIENT (TEE_PropSetHandle)0xFFF"
	.ascii	"FFFFE\000"
.LASF1377:
	.ascii	"LIST_NEXT(elm,field) ((elm)->field.le_next)\000"
.LASF243:
	.ascii	"__FLT32X_HAS_INFINITY__ 1\000"
.LASF811:
	.ascii	"PRIu8 \"u\"\000"
.LASF2194:
	.ascii	"long int\000"
.LASF887:
	.ascii	"TEE_API_DEFINES_H \000"
.LASF1712:
	.ascii	"SCTLR_Z BIT32(11)\000"
.LASF1619:
	.ascii	"NEOVERSE_E1_PART_NUM U(0xD4A)\000"
.LASF1841:
	.ascii	"TEE_ALG_CONCAT_KDF_SHA384_DERIVE_KEY 0x800050C1\000"
.LASF126:
	.ascii	"__UINT_LEAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF1033:
	.ascii	"TEE_ALG_HMAC_MD5 0x30000001\000"
.LASF754:
	.ascii	"INT_LEAST64_MAX INT64_MAX\000"
.LASF918:
	.ascii	"TEE_ERROR_MAC_INVALID 0xFFFF3071\000"
.LASF2081:
	.ascii	"RCC_WAKEUP_IT U(177)\000"
.LASF1369:
	.ascii	"LIST_INIT(head) do { (head)->lh_first = NULL; } whi"
	.ascii	"le ( 0)\000"
.LASF438:
	.ascii	"__ARM_EABI__ 1\000"
.LASF1112:
	.ascii	"TEE_ATTR_ECC_PRIVATE_VALUE 0xC0000341\000"
.LASF1412:
	.ascii	"SIMPLEQ_INIT(head) do { (head)->sqh_first = NULL; ("
	.ascii	"head)->sqh_last = &(head)->sqh_first; } while ( 0)\000"
.LASF1368:
	.ascii	"QUEUEDEBUG_LIST_POSTREMOVE(elm,field) \000"
.LASF1572:
	.ascii	"TA_PROP_STR_SINGLE_INSTANCE \"gpd.ta.singleInstance"
	.ascii	"\"\000"
.LASF2157:
	.ascii	"MEM_AREA_TEE_RAM_RW_DATA MEM_AREA_TEE_RAM_RW\000"
.LASF208:
	.ascii	"__FLT32_EPSILON__ 1.1920928955078125e-7F32\000"
.LASF1229:
	.ascii	"TEE_PANIC_ID_TEE_GENERATERANDOM 0x00001301\000"
.LASF1366:
	.ascii	"QUEUEDEBUG_LIST_INSERT_HEAD(head,elm,field) \000"
.LASF1168:
	.ascii	"TEE_PANIC_ID_TEE_MEMFILL 0x00000606\000"
.LASF829:
	.ascii	"false 0\000"
.LASF153:
	.ascii	"__FLT_MAX_EXP__ 128\000"
.LASF103:
	.ascii	"__INT64_MAX__ 0x7fffffffffffffffLL\000"
.LASF2077:
	.ascii	"TARGET_CPU0_GIC_MASK BIT(0)\000"
.LASF10:
	.ascii	"__ATOMIC_SEQ_CST 5\000"
.LASF860:
	.ascii	"_GCC_SIZE_T \000"
.LASF1294:
	.ascii	"TRACE_LEVELS_H \000"
.LASF1163:
	.ascii	"TEE_PANIC_ID_TEE_CHECKMEMORYACCESSRIGHTS 0x00000601"
	.ascii	"\000"
.LASF1389:
	.ascii	"SLIST_FOREACH_SAFE(var,head,field,tvar) for ((var) "
	.ascii	"= SLIST_FIRST((head)); (var) && ((tvar) = SLIST_NEX"
	.ascii	"T((var), field), 1); (var) = (tvar))\000"
.LASF2042:
	.ascii	"TIM14_BASE 0x40008000\000"
.LASF643:
	.ascii	"DEFINES void __defines(void); void __defines(void)\000"
.LASF234:
	.ascii	"__FLT32X_MAX_EXP__ 1024\000"
.LASF198:
	.ascii	"__FLT32_MANT_DIG__ 24\000"
.LASF988:
	.ascii	"TEE_ALG_DES_CBC_MAC_NOPAD 0x30000111\000"
.LASF1552:
	.ascii	"TEE_MATTR_GUARDED BIT(15)\000"
.LASF2052:
	.ascii	"UART5_BASE 0x40011000\000"
.LASF2151:
	.ascii	"CORE_MMU_PGDIR_SIZE BIT(CORE_MMU_PGDIR_SHIFT)\000"
.LASF360:
	.ascii	"__UTA_FBIT__ 64\000"
.LASF1687:
	.ascii	"CPSR_T ARM32_CPSR_T\000"
.LASF166:
	.ascii	"__DBL_DIG__ 15\000"
.LASF964:
	.ascii	"TEE_HANDLE_FLAG_INITIALIZED 0x00020000\000"
.LASF583:
	.ascii	"CFG_STM32_LOWPOWER_SIP 1\000"
.LASF795:
	.ascii	"__PRIPTR_PREFIX \"l\"\000"
.LASF559:
	.ascii	"CFG_SM_PLATFORM_HANDLER 1\000"
.LASF1297:
	.ascii	"TRACE_INFO 2\000"
.LASF187:
	.ascii	"__DECIMAL_DIG__ 17\000"
.LASF337:
	.ascii	"__UQQ_IBIT__ 0\000"
.LASF698:
	.ascii	"__SCT_ARRAY_DEF_PG_ITEM3(element_type,element_name,"
	.ascii	"section_name) static const element_type element_nam"
	.ascii	"e __used __section(section_name)\000"
.LASF2147:
	.ascii	"TEE_RAM_VA_START TEE_RAM_START\000"
.LASF1465:
	.ascii	"CIRCLEQ_NEXT(elm,field) ((elm)->field.cqe_next)\000"
.LASF1981:
	.ascii	"AHB4_BASE 0x50000000\000"
.LASF2062:
	.ascii	"CONSOLE_UART_SIZE 1024\000"
.LASF2003:
	.ascii	"GPIOD_BASE 0x50005000\000"
.LASF2059:
	.ascii	"STM32MP1_DEBUG_USART_BASE UART4_BASE\000"
.LASF1520:
	.ascii	"SHIFT_U32(v,shift) ((uint32_t)(v) << (shift))\000"
.LASF1471:
	.ascii	"RECURSIVE_MUTEX_INITIALIZER { .m = MUTEX_INITIALIZE"
	.ascii	"R, .owner = THREAD_ID_INVALID }\000"
.LASF1121:
	.ascii	"TEE_ATTR_EDDSA_CTX 0xD0000643\000"
.LASF815:
	.ascii	"PRIuPTR __PRIPTR_PREFIX \"u\"\000"
.LASF1798:
	.ascii	"THREAD_EXCP_ALL (THREAD_EXCP_FOREIGN_INTR | THREAD_"
	.ascii	"EXCP_NATIVE_INTR | (ARM32_CPSR_A >> ARM32_CPSR_F_SH"
	.ascii	"IFT))\000"
.LASF1219:
	.ascii	"TEE_PANIC_ID_TEE_AEDECRYPTFINAL 0x00001001\000"
.LASF1191:
	.ascii	"TEE_PANIC_ID_TEE_RENAMEPERSISTENTOBJECT 0x00000904\000"
.LASF361:
	.ascii	"__UTA_IBIT__ 64\000"
.LASF144:
	.ascii	"__GCC_IEC_559_COMPLEX 0\000"
.LASF1240:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTFROMOCTETSTRING 0x000"
	.ascii	"01701\000"
.LASF1894:
	.ascii	"TEE_CHAIN_MODE_GCM 0x8\000"
.LASF635:
	.ascii	"_CFG_CORE_LTC_BIGNUM_MAX_BITS 4096\000"
.LASF1573:
	.ascii	"TA_PROP_STR_MULTI_SESSION \"gpd.ta.multiSession\"\000"
.LASF1762:
	.ascii	"TTBCR_A1 BIT32(22)\000"
.LASF817:
	.ascii	"PRIx16 \"x\"\000"
.LASF1314:
	.ascii	"trace_printf_helper_raw(level,level_ok,...) trace_p"
	.ascii	"rintf(NULL, 0, (level), (level_ok), __VA_ARGS__)\000"
.LASF2010:
	.ascii	"GPIOK_BASE 0x5000c000\000"
.LASF1504:
	.ascii	"ROUNDUP_OVERFLOW(v,size,res) (__extension__({ typeo"
	.ascii	"f(*(res)) __roundup_tmp = 0; typeof(v) __roundup_ma"
	.ascii	"sk = (typeof(v))(size) - 1; ADD_OVERFLOW((v), __rou"
	.ascii	"ndup_mask, &__roundup_tmp) ? 1 : (void)(*(res) = __"
	.ascii	"roundup_tmp & ~__roundup_mask), 0; }))\000"
.LASF1908:
	.ascii	"TEE_DES_BLOCK_SIZE 8UL\000"
.LASF1271:
	.ascii	"TEE_BigIntSizeInU32(n) ((((n)+31)/32)+2)\000"
.LASF363:
	.ascii	"__USER_LABEL_PREFIX__ \000"
.LASF2096:
	.ascii	"BKPSRAM_PM_OFFSET U(0x0000)\000"
.LASF857:
	.ascii	"_SIZE_T_DECLARED \000"
.LASF2041:
	.ascii	"TIM13_BASE 0x40007000\000"
.LASF1463:
	.ascii	"CIRCLEQ_FIRST(head) ((head)->cqh_first)\000"
.LASF1189:
	.ascii	"TEE_PANIC_ID_TEE_CREATEPERSISTENTOBJECT 0x00000902\000"
.LASF1645:
	.ascii	"CTR_DMINLINE_SHIFT U(16)\000"
.LASF124:
	.ascii	"__UINT_LEAST32_MAX__ 0xffffffffU\000"
.LASF1799:
	.ascii	"THREAD_FLAGS_COPY_ARGS_ON_RETURN BIT(0)\000"
.LASF2051:
	.ascii	"UART4_BASE 0x40010000\000"
.LASF1879:
	.ascii	"TEE_MAIN_ALGO_SM2_DSA_SM3 0x45\000"
.LASF537:
	.ascii	"CFG_OPTEE_REVISION_MINOR 19\000"
.LASF2083:
	.ascii	"TAMP_BKP_REGISTER_COUNT U(32)\000"
.LASF1822:
	.ascii	"__MM_FILE_H \000"
.LASF1790:
	.ascii	"CNTFID_OFFSET 0x020\000"
.LASF1585:
	.ascii	"MIDR_PRIMARY_PART_NUM_SHIFT U(4)\000"
.LASF1351:
	.ascii	"PGT_SIZE (4 * 1024)\000"
.LASF2024:
	.ascii	"RETRAM_BASE 0x38000000ul\000"
.LASF174:
	.ascii	"__DBL_MIN__ ((double)2.2250738585072014e-308L)\000"
.LASF672:
	.ascii	"__nex_bss \000"
.LASF1865:
	.ascii	"TEE_MAIN_ALGO_SHA384 0x05\000"
.LASF369:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1\000"
.LASF1170:
	.ascii	"TEE_PANIC_ID_TEE_REALLOC 0x00000608\000"
.LASF866:
	.ascii	"_WCHAR_T \000"
.LASF1490:
	.ascii	"TEE_MMU_TYPES_H \000"
.LASF1279:
	.ascii	"TRACE_H \000"
.LASF1673:
	.ascii	"CNTKCTL_PL0VCTEN BIT(1)\000"
.LASF1174:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTINFO 0x00000703\000"
.LASF910:
	.ascii	"TEE_ERROR_BUSY 0xFFFF000D\000"
.LASF1241:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTFROMS32 0x00001702\000"
.LASF679:
	.ascii	"__GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * "
	.ascii	"100 + __GNUC_PATCHLEVEL__)\000"
.LASF189:
	.ascii	"__LDBL_MAX__ 1.7976931348623157e+308L\000"
.LASF540:
	.ascii	"CFG_PM_ARM32 1\000"
.LASF427:
	.ascii	"__ARM_FP16_ARGS\000"
.LASF200:
	.ascii	"__FLT32_MIN_EXP__ (-125)\000"
.LASF1120:
	.ascii	"TEE_ATTR_ECC_EPHEMERAL_PUBLIC_VALUE_Y 0xD0000A46\000"
.LASF1856:
	.ascii	"TEE_STORAGE_PRIVATE_RPMB 0x80000100\000"
.LASF2067:
	.ascii	"CFG0_OTP_SECURED_POS U(6)\000"
.LASF2039:
	.ascii	"TIM8_BASE 0x44001000\000"
.LASF183:
	.ascii	"__LDBL_MIN_EXP__ (-1021)\000"
.LASF769:
	.ascii	"UINT_FAST32_MAX UINT32_MAX\000"
.LASF168:
	.ascii	"__DBL_MIN_10_EXP__ (-307)\000"
.LASF1833:
	.ascii	"TEE_TYPE_HKDF_IKM 0xA10000C0\000"
.LASF907:
	.ascii	"TEE_ERROR_NOT_SUPPORTED 0xFFFF000A\000"
.LASF536:
	.ascii	"CFG_OPTEE_REVISION_MAJOR 3\000"
.LASF1208:
	.ascii	"TEE_PANIC_ID_TEE_SETOPERATIONKEY2 0x00000C07\000"
.LASF1665:
	.ascii	"ARM32_CPSR_I BIT(7)\000"
.LASF263:
	.ascii	"__UFRACT_MIN__ 0.0UR\000"
.LASF642:
	.ascii	"GEN_ASM_DEFINES_H \000"
.LASF1331:
	.ascii	"early_init(fn) __define_initcall(init, 1, fn)\000"
.LASF737:
	.ascii	"INT32_MIN (-0x7fffffff-1)\000"
.LASF849:
	.ascii	"_T_SIZE_ \000"
.LASF152:
	.ascii	"__FLT_MIN_10_EXP__ (-37)\000"
.LASF1954:
	.ascii	"_XXXX1 _YYYY,\000"
.LASF569:
	.ascii	"CFG_STM32MP_CLK_CORE 1\000"
.LASF1938:
	.ascii	"HW_UNIQUE_KEY_WORD2 (HW_UNIQUE_KEY_WORD1 + 1)\000"
.LASF1421:
	.ascii	"SIMPLEQ_NEXT(elm,field) ((elm)->field.sqe_next)\000"
.LASF903:
	.ascii	"TEE_ERROR_BAD_PARAMETERS 0xFFFF0006\000"
.LASF670:
	.ascii	"__rodata_unpaged(x) __section(\".rodata.__unpaged.\""
	.ascii	" x __SECTION_FLAGS_RODATA)\000"
.LASF915:
	.ascii	"TEE_ERROR_OVERFLOW 0xFFFF300F\000"
.LASF343:
	.ascii	"__UDQ_IBIT__ 0\000"
.LASF165:
	.ascii	"__DBL_MANT_DIG__ 53\000"
.LASF288:
	.ascii	"__SACCUM_MIN__ (-0X1P7HK-0X1P7HK)\000"
.LASF90:
	.ascii	"__PTRDIFF_WIDTH__ 32\000"
.LASF981:
	.ascii	"TEE_ALG_AES_CBC_MAC_NOPAD 0x30000110\000"
.LASF323:
	.ascii	"__ULLACCUM_MIN__ 0.0ULLK\000"
.LASF876:
	.ascii	"__INT_WCHAR_T_H \000"
.LASF1753:
	.ascii	"TTBCR_EAE BIT32(31)\000"
.LASF1210:
	.ascii	"TEE_PANIC_ID_TEE_DIGESTDOFINAL 0x00000D01\000"
.LASF1631:
	.ascii	"MPIDR_MT_SHIFT U(24)\000"
.LASF1639:
	.ascii	"CLIDR_FIELD_WIDTH U(3)\000"
.LASF706:
	.ascii	"SCATTERED_ARRAY_DEFINE_PG_ITEM(array_name,element_t"
	.ascii	"ype) __SCT_ARRAY_DEF_PG_ITEM1(array_name, 0, __COUN"
	.ascii	"TER__, element_type)\000"
.LASF2167:
	.ascii	"phys_ddr_overall_end SCATTERED_ARRAY_END(phys_ddr_o"
	.ascii	"verall, struct core_mmu_phys_mem)\000"
.LASF989:
	.ascii	"TEE_ALG_DES_CBC_MAC_PKCS5 0x30000511\000"
.LASF873:
	.ascii	"_WCHAR_T_DEFINED \000"
.LASF595:
	.ascii	"CFG_STM32_UART 1\000"
.LASF1043:
	.ascii	"TEE_ALG_ECDSA_P384 0x70004041\000"
.LASF667:
	.ascii	"__SECTION_FLAGS_RODATA \",\\\"a\\\",%progbits \" CO"
	.ascii	"MMENT_CHAR\000"
.LASF1812:
	.ascii	"SM_EXIT_TO_SECURE 1\000"
.LASF2049:
	.ascii	"UART2_BASE 0x4000e000\000"
.LASF1881:
	.ascii	"TEE_MAIN_ALGO_SM2_PKE 0x47\000"
.LASF862:
	.ascii	"__size_t \000"
.LASF1984:
	.ascii	"AHB5_SIZE 0x00005000\000"
.LASF79:
	.ascii	"__WINT_MAX__ 0xffffffffU\000"
.LASF1581:
	.ascii	"NSAPP_IDENTITY (NULL)\000"
.LASF563:
	.ascii	"CFG_STM32MP15 1\000"
.LASF934:
	.ascii	"TEE_LOGIN_APPLICATION_GROUP 0x00000006\000"
.LASF426:
	.ascii	"__ARM_FP16_FORMAT_ALTERNATIVE\000"
.LASF410:
	.ascii	"__arm__ 1\000"
.LASF1151:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASU32 0x00000208\000"
.LASF2148:
	.ascii	"TEE_TEXT_VA_START (TEE_RAM_VA_START + (TEE_LOAD_ADD"
	.ascii	"R - TEE_RAM_START))\000"
.LASF909:
	.ascii	"TEE_ERROR_OUT_OF_MEMORY 0xFFFF000C\000"
.LASF291:
	.ascii	"__USACCUM_FBIT__ 8\000"
.LASF1873:
	.ascii	"TEE_MAIN_ALGO_RSA 0x30\000"
.LASF985:
	.ascii	"TEE_ALG_AES_GCM 0x40000810\000"
.LASF1180:
	.ascii	"TEE_PANIC_ID_TEE_COPYOBJECTATTRIBUTES 0x00000802\000"
.LASF379:
	.ascii	"__GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1\000"
.LASF1982:
	.ascii	"AHB4_SIZE 0x00020000\000"
.LASF1070:
	.ascii	"TEE_TYPE_DH_KEYPAIR 0xA1000032\000"
.LASF159:
	.ascii	"__FLT_EPSILON__ 1.1920928955078125e-7F\000"
.LASF1448:
	.ascii	"QUEUEDEBUG_CIRCLEQ_HEAD(head,field) \000"
.LASF2138:
	.ascii	"SMALL_PAGE_SHIFT U(12)\000"
.LASF2072:
	.ascii	"GICD_OFFSET 0x0000\000"
.LASF1485:
	.ascii	"tee_mtime_init() do { } while (0)\000"
.LASF121:
	.ascii	"__UINT8_C(c) c\000"
.LASF1949:
	.ascii	"__CORE_MMU_ARCH_H \000"
.LASF1110:
	.ascii	"TEE_ATTR_ECC_PUBLIC_VALUE_X 0xD0000141\000"
.LASF1965:
	.ascii	"TA_RAM_SIZE (ROUNDDOWN(TZDRAM_BASE + (TZDRAM_SIZE -"
	.ascii	" TEE_SDP_TEST_MEM_SIZE), SMALL_PAGE_SIZE) - TA_RAM_"
	.ascii	"START)\000"
.LASF1256:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINVMOD 0x00001A02\000"
.LASF1315:
	.ascii	"MSG_RAW(...) trace_printf_helper_raw(0, false, __VA"
	.ascii	"_ARGS__)\000"
.LASF404:
	.ascii	"__ARM_FEATURE_NUMERIC_MAXMIN\000"
.LASF2084:
	.ascii	"TAMP_BKP_REGISTER_ZONE1_COUNT U(10)\000"
.LASF1834:
	.ascii	"TEE_ATTR_HKDF_IKM 0xC00001C0\000"
.LASF274:
	.ascii	"__ULFRACT_MAX__ 0XFFFFFFFFP-32ULR\000"
.LASF1535:
	.ascii	"TEE_MATTR_PRX (TEE_MATTR_PR | TEE_MATTR_PX)\000"
.LASF1536:
	.ascii	"TEE_MATTR_PRWX (TEE_MATTR_PRW | TEE_MATTR_PX)\000"
.LASF1816:
	.ascii	"STACK_ABT_SIZE 3072\000"
.LASF947:
	.ascii	"TEE_STORAGE_PRIVATE 0x00000001\000"
.LASF1031:
	.ascii	"TEE_ALG_SHA3_512 0x5000000B\000"
.LASF629:
	.ascii	"CFG_WITH_STACK_CANARIES 1\000"
.LASF761:
	.ascii	"INT_FAST32_MIN INT32_MIN\000"
.LASF1049:
	.ascii	"TEE_ALG_ECDH_P384 0x80004042\000"
.LASF268:
	.ascii	"__LFRACT_MIN__ (-0.5LR-0.5LR)\000"
.LASF468:
	.ascii	"CFG_ARM32_ldelf 1\000"
.LASF1330:
	.ascii	"preinit_late(fn) __define_initcall(preinit, 3, fn)\000"
.LASF712:
	.ascii	"__LIMITS_H \000"
.LASF2065:
	.ascii	"OTP_MAX_SIZE (STM32MP1_OTP_MAX_ID + 1U)\000"
.LASF496:
	.ascii	"CFG_CRYPTOLIB_NAME_tomcrypt 1\000"
.LASF1166:
	.ascii	"TEE_PANIC_ID_TEE_MALLOC 0x00000604\000"
.LASF422:
	.ascii	"__SOFTFP__ 1\000"
.LASF570:
	.ascii	"CFG_STM32MP_PROFILE system_services\000"
.LASF247:
	.ascii	"__SFRACT_IBIT__ 0\000"
.LASF1213:
	.ascii	"TEE_PANIC_ID_TEE_CIPHERINIT 0x00000E02\000"
.LASF28:
	.ascii	"__ORDER_LITTLE_ENDIAN__ 1234\000"
.LASF842:
	.ascii	"_PTRDIFF_T_DECLARED \000"
.LASF1467:
	.ascii	"CIRCLEQ_LOOP_NEXT(head,elm,field) (((elm)->field.cq"
	.ascii	"e_next == (void *)(head)) ? ((head)->cqh_first) : ("
	.ascii	"elm->field.cqe_next))\000"
.LASF1365:
	.ascii	"LIST_ENTRY(type) struct { struct type *le_next; str"
	.ascii	"uct type **le_prev; }\000"
.LASF921:
	.ascii	"TEE_ERROR_TIME_NEEDS_RESET 0xFFFF5001\000"
.LASF555:
	.ascii	"CFG_SCMI_PTA 1\000"
.LASF1686:
	.ascii	"CPSR_MODE_SYS ARM32_CPSR_MODE_SYS\000"
.LASF2174:
	.ascii	"THREAD_CLF_SAVED_SHIFT 4\000"
.LASF1408:
	.ascii	"STAILQ_NEXT(elm,field) ((elm)->field.stqe_next)\000"
.LASF150:
	.ascii	"__FLT_DIG__ 6\000"
.LASF1259:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSQUAREMOD 0x00001A05\000"
.LASF1739:
	.ascii	"CPACR_CP(co_proc,access) SHIFT_U32((access), ((co_p"
	.ascii	"roc) * 2))\000"
.LASF1838:
	.ascii	"TEE_ALG_CONCAT_KDF_SHA1_DERIVE_KEY 0x800020C1\000"
.LASF644:
	.ascii	"DEFINE(def,val) asm volatile(\"\\n.ascii \\\"==>\" "
	.ascii	"#def \" %c0 \" #val \"\\\"\" : : \"i\" (val));\000"
.LASF149:
	.ascii	"__FLT_MANT_DIG__ 24\000"
.LASF1248:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSHIFTRIGHT 0x00001805\000"
.LASF2155:
	.ascii	"CORE_MMU_USER_PARAM_SIZE BIT(CORE_MMU_USER_PARAM_SH"
	.ascii	"IFT)\000"
.LASF169:
	.ascii	"__DBL_MAX_EXP__ 1024\000"
.LASF1090:
	.ascii	"TEE_ATTR_RSA_PUBLIC_EXPONENT 0xD0000230\000"
.LASF1922:
	.ascii	"TEE_U64_TO_LITTLE_ENDIAN(x) ((uint64_t)(x))\000"
.LASF314:
	.ascii	"__ULACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULK\000"
.LASF1129:
	.ascii	"TEE_ATTR_BIT_PROTECTED TEE_ATTR_FLAG_PUBLIC\000"
.LASF122:
	.ascii	"__UINT_LEAST16_MAX__ 0xffff\000"
.LASF1414:
	.ascii	"SIMPLEQ_INSERT_TAIL(head,elm,field) do { (elm)->fie"
	.ascii	"ld.sqe_next = NULL; *(head)->sqh_last = (elm); (hea"
	.ascii	"d)->sqh_last = &(elm)->field.sqe_next; } while ( 0)"
	.ascii	"\000"
.LASF1633:
	.ascii	"MPIDR_CPU_MASK MPIDR_AFF0_MASK\000"
.LASF1574:
	.ascii	"TA_PROP_STR_KEEP_ALIVE \"gpd.ta.instanceKeepAlive\""
	.ascii	"\000"
.LASF875:
	.ascii	"___int_wchar_t_h \000"
.LASF936:
	.ascii	"TEE_ORIGIN_API 0x00000001\000"
.LASF1888:
	.ascii	"TEE_CHAIN_MODE_CTR 0x2\000"
.LASF701:
	.ascii	"__SCT_ARRAY_DEF_ITEM1(array_name,order,id,element_t"
	.ascii	"ype) __SCT_ARRAY_DEF_ITEM2(array_name, order, id, e"
	.ascii	"lement_type)\000"
.LASF665:
	.ascii	"__bss __section(\".bss\")\000"
.LASF25:
	.ascii	"__SIZEOF_SIZE_T__ 4\000"
.LASF1309:
	.ascii	"FMSG(...) (void)0\000"
.LASF481:
	.ascii	"CFG_CORE_HEAP_SIZE 65536\000"
.LASF2036:
	.ascii	"TIM5_BASE 0x40003000\000"
.LASF1775:
	.ascii	"TTBR_ASID_SHIFT U(48)\000"
.LASF1551:
	.ascii	"TEE_MATTR_MEM_TYPE_TAGGED U(3)\000"
.LASF796:
	.ascii	"PRId8 \"d\"\000"
.LASF62:
	.ascii	"__INT_FAST16_TYPE__ int\000"
.LASF2145:
	.ascii	"CORE_MMU_BASE_TABLE_OFFSET (CFG_TEE_CORE_NB_CORE * "
	.ascii	"BIT(CFG_LPAE_ADDR_SPACE_BITS - CORE_MMU_BASE_TABLE_"
	.ascii	"SHIFT) * U(8))\000"
.LASF834:
	.ascii	"_PTRDIFF_T \000"
.LASF1853:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5 0xF0000830\000"
.LASF2131:
	.ascii	"STM32MP1_REV_B U(0x00002000)\000"
.LASF588:
	.ascii	"CFG_STM32_RSTCTRL 1\000"
.LASF1554:
	.ascii	"VM_FLAG_PERMANENT BIT(1)\000"
.LASF1591:
	.ascii	"MIDR_IMPLEMENTER_ARM U(0x41)\000"
.LASF726:
	.ascii	"SHRT_MIN (-SHRT_MAX - 1)\000"
.LASF1101:
	.ascii	"TEE_ATTR_DSA_PRIVATE_VALUE 0xC0000231\000"
.LASF647:
	.ascii	"__SCATTERED_ARRAY_H \000"
.LASF1918:
	.ascii	"TEE_U16_BSWAP(x) __compiler_bswap16((x))\000"
.LASF1462:
	.ascii	"CIRCLEQ_EMPTY(head) ((head)->cqh_first == (void *)("
	.ascii	"head))\000"
.LASF457:
	.ascii	"__unix__ 1\000"
.LASF982:
	.ascii	"TEE_ALG_AES_CBC_MAC_PKCS5 0x30000510\000"
.LASF2182:
	.ascii	"THREAD_CLF_ABORT BIT(THREAD_CLF_ABORT_SHIFT)\000"
.LASF1380:
	.ascii	"SLIST_HEAD_INITIALIZER(head) { NULL }\000"
.LASF772:
	.ascii	"INTPTR_MAX LONG_MAX\000"
.LASF505:
	.ascii	"CFG_DRIVERS_CLK_DT 1\000"
.LASF740:
	.ascii	"INT16_MAX 0x7fff\000"
.LASF1060:
	.ascii	"TEE_TYPE_HMAC_SHA1 0xA0000002\000"
.LASF246:
	.ascii	"__SFRACT_FBIT__ 7\000"
.LASF456:
	.ascii	"__unix 1\000"
.LASF1086:
	.ascii	"TEE_TYPE_X25519_PUBLIC_KEY 0xA0000044\000"
.LASF320:
	.ascii	"__LLACCUM_EPSILON__ 0x1P-31LLK\000"
.LASF1927:
	.ascii	"TEE_U16_FROM_BIG_ENDIAN(x) TEE_U16_BSWAP(x)\000"
.LASF631:
	.ascii	"CFG_WITH_TRNG 1\000"
.LASF1912:
	.ascii	"TEE_MAC_SIZE_AES_CBC_MAC_PKCS5 \000"
.LASF1405:
	.ascii	"STAILQ_CONCAT(head1,head2) do { if (!STAILQ_EMPTY(("
	.ascii	"head2))) { *(head1)->stqh_last = (head2)->stqh_firs"
	.ascii	"t; (head1)->stqh_last = (head2)->stqh_last; STAILQ_"
	.ascii	"INIT((head2)); } } while ( 0)\000"
.LASF1486:
	.ascii	"tee_mtime_stamp(descr) do { } while (0)\000"
.LASF1623:
	.ascii	"MPIDR_AFFINITY_BITS U(8)\000"
.LASF1699:
	.ascii	"SCR_FIQ BIT32(2)\000"
.LASF1014:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512 0x60610230\000"
.LASF1258:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTMULMOD 0x00001A04\000"
.LASF171:
	.ascii	"__DBL_DECIMAL_DIG__ 17\000"
.LASF20:
	.ascii	"__SIZEOF_LONG_LONG__ 8\000"
.LASF1924:
	.ascii	"TEE_U16_TO_LITTLE_ENDIAN(x) ((uint16_t)(x))\000"
.LASF575:
	.ascii	"CFG_STM32_CPU_OPP 1\000"
.LASF807:
	.ascii	"PRIo16 \"o\"\000"
.LASF1847:
	.ascii	"TEE_ALG_PBKDF2_HMAC_SHA1_DERIVE_KEY 0x800020C2\000"
.LASF1745:
	.ascii	"DACR_DOMAIN_PERM_CLIENT U(0x1)\000"
.LASF1238:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINITFMM 0x00001602\000"
.LASF204:
	.ascii	"__FLT32_DECIMAL_DIG__ 9\000"
.LASF994:
	.ascii	"TEE_ALG_SM4_ECB_NOPAD 0x10000014\000"
.LASF1096:
	.ascii	"TEE_ATTR_RSA_COEFFICIENT 0xC0000830\000"
.LASF823:
	.ascii	"PRIX32 \"X\"\000"
.LASF1299:
	.ascii	"TRACE_FLOW 4\000"
.LASF1318:
	.ascii	"DMSG_RAW(...) (void)0\000"
.LASF1878:
	.ascii	"TEE_MAIN_ALGO_ED25519 0x43\000"
.LASF409:
	.ascii	"__ARM_ARCH_PROFILE 65\000"
.LASF872:
	.ascii	"_WCHAR_T_DEFINED_ \000"
.LASF478:
	.ascii	"CFG_CORE_CLUSTER_SHIFT 2\000"
.LASF1523:
	.ascii	"GENMASK_32(h,l) (((~UINT32_C(0)) << (l)) & (~UINT32"
	.ascii	"_C(0) >> (32 - 1 - (h))))\000"
.LASF39:
	.ascii	"__WINT_TYPE__ unsigned int\000"
.LASF933:
	.ascii	"TEE_LOGIN_APPLICATION_USER 0x00000005\000"
.LASF1332:
	.ascii	"early_init_late(fn) __define_initcall(init, 2, fn)\000"
.LASF1037:
	.ascii	"TEE_ALG_HMAC_SHA384 0x30000005\000"
.LASF1688:
	.ascii	"CPSR_F_SHIFT ARM32_CPSR_F_SHIFT\000"
.LASF1122:
	.ascii	"TEE_ATTR_ED25519_PUBLIC_VALUE 0xD0000743\000"
.LASF2176:
	.ascii	"THREAD_CLF_MASK 0xf\000"
.LASF893:
	.ascii	"TEE_ERROR_CORRUPT_OBJECT_2 0xF0100002\000"
.LASF1172:
	.ascii	"TEE_PANIC_ID_TEE_CLOSEOBJECT 0x00000701\000"
.LASF1672:
	.ascii	"CNTKCTL_PL0PCTEN BIT(0)\000"
.LASF1866:
	.ascii	"TEE_MAIN_ALGO_SHA512 0x06\000"
.LASF1173:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTBUFFERATTRIBUTE 0x0000070"
	.ascii	"2\000"
.LASF605:
	.ascii	"CFG_TA_MBEDTLS_MPI 1\000"
.LASF513:
	.ascii	"CFG_EMBED_DTB 1\000"
.LASF969:
	.ascii	"TEE_OPERATION_AE 4\000"
.LASF1472:
	.ascii	"CONDVAR_INITIALIZER { .m = NULL }\000"
.LASF713:
	.ascii	"CHAR_BIT __CHAR_BIT__\000"
.LASF975:
	.ascii	"TEE_OPERATION_STATE_ACTIVE 0x00000001\000"
.LASF567:
	.ascii	"CFG_STM32MP1_REGULATOR_IOD 1\000"
.LASF697:
	.ascii	"__SCT_ARRAY_DEF_ITEM3(element_type,element_name,sec"
	.ascii	"tion_name) static const element_type element_name; "
	.ascii	"DECLARE_KEEP_INIT(element_name); static const eleme"
	.ascii	"nt_type element_name __used __section(section_name)"
	.ascii	"\000"
.LASF311:
	.ascii	"__ULACCUM_FBIT__ 32\000"
.LASF2141:
	.ascii	"CORE_MMU_USER_CODE_SHIFT SMALL_PAGE_SHIFT\000"
.LASF1288:
	.ascii	"__va_copy(d,s) __builtin_va_copy(d,s)\000"
.LASF1131:
	.ascii	"TEE_CRYPTO_ELEMENT_NONE 0x00000000\000"
.LASF1429:
	.ascii	"QUEUEDEBUG_TAILQ_OP(elm,field) \000"
.LASF228:
	.ascii	"__FLT64_HAS_QUIET_NAN__ 1\000"
.LASF592:
	.ascii	"CFG_STM32_TAMP 1\000"
.LASF810:
	.ascii	"PRIoPTR __PRIPTR_PREFIX \"o\"\000"
.LASF1285:
	.ascii	"va_end(v) __builtin_va_end(v)\000"
.LASF576:
	.ascii	"CFG_STM32_EARLY_CONSOLE_UART 4\000"
.LASF594:
	.ascii	"CFG_STM32_TIM 1\000"
.LASF503:
	.ascii	"CFG_DRAM_SIZE 0x40000000\000"
.LASF1036:
	.ascii	"TEE_ALG_HMAC_SHA256 0x30000004\000"
.LASF1127:
	.ascii	"TEE_ATTR_FLAG_PUBLIC (1 << 28)\000"
.LASF1909:
	.ascii	"TEE_SM4_BLOCK_SIZE 16UL\000"
.LASF1617:
	.ascii	"CORTEX_X1_PART_NUM U(0xD44)\000"
.LASF1372:
	.ascii	"LIST_INSERT_HEAD(head,elm,field) do { QUEUEDEBUG_LI"
	.ascii	"ST_INSERT_HEAD((head), (elm), field) if (((elm)->fi"
	.ascii	"eld.le_next = (head)->lh_first) != NULL) (head)->lh"
	.ascii	"_first->field.le_prev = &(elm)->field.le_next; (hea"
	.ascii	"d)->lh_first = (elm); (elm)->field.le_prev = &(head"
	.ascii	")->lh_first; } while ( 0)\000"
.LASF1422:
	.ascii	"_TAILQ_HEAD(name,type,qual) struct name { qual type"
	.ascii	" *tqh_first; qual type *qual *tqh_last; }\000"
.LASF694:
	.ascii	"__DECLARE_KEEP_INIT2(sym,file_id) extern const unsi"
	.ascii	"gned long ____keep_init_ ##sym ##file_id; const uns"
	.ascii	"igned long ____keep_init_ ##sym ##_ ##file_id __sec"
	.ascii	"tion(\"__keep_meta_vars_init\") = (unsigned long)&("
	.ascii	"sym)\000"
.LASF312:
	.ascii	"__ULACCUM_IBIT__ 32\000"
.LASF491:
	.ascii	"CFG_CORE_WORKAROUND_SPECTRE_BP 1\000"
.LASF1204:
	.ascii	"TEE_PANIC_ID_TEE_FREEOPERATION 0x00000C03\000"
.LASF444:
	.ascii	"__ARM_FEATURE_COPROC 15\000"
.LASF2172:
	.ascii	"phys_mem_map_begin SCATTERED_ARRAY_BEGIN(phys_mem_m"
	.ascii	"ap, struct core_mmu_phys_mem)\000"
.LASF1346:
	.ascii	"__SIZEOF_PADDR__ __SIZEOF_POINTER__\000"
.LASF510:
	.ascii	"CFG_DT 1\000"
.LASF578:
	.ascii	"CFG_STM32_FIREWALL 1\000"
.LASF1784:
	.ascii	"IDPFR1_GENTIMER_SHIFT U(16)\000"
.LASF874:
	.ascii	"_WCHAR_T_H \000"
.LASF108:
	.ascii	"__INT_LEAST8_MAX__ 0x7f\000"
.LASF1409:
	.ascii	"SIMPLEQ_HEAD(name,type) struct name { struct type *"
	.ascii	"sqh_first; struct type **sqh_last; }\000"
.LASF1052:
	.ascii	"TEE_ALG_SM3 0x50000007\000"
.LASF2022:
	.ascii	"QSPI_BASE 0x58003000\000"
.LASF31:
	.ascii	"__BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF1620:
	.ascii	"NEOVERSE_N1_PART_NUM U(0xD0C)\000"
.LASF127:
	.ascii	"__UINT64_C(c) c ## ULL\000"
.LASF1434:
	.ascii	"TAILQ_INSERT_TAIL(head,elm,field) do { QUEUEDEBUG_T"
	.ascii	"AILQ_INSERT_TAIL((head), (elm), field) (elm)->field"
	.ascii	".tqe_next = NULL; (elm)->field.tqe_prev = (head)->t"
	.ascii	"qh_last; *(head)->tqh_last = (elm); (head)->tqh_las"
	.ascii	"t = &(elm)->field.tqe_next; } while ( 0)\000"
.LASF257:
	.ascii	"__FRACT_IBIT__ 0\000"
.LASF1270:
	.ascii	"TEE_NUM_PARAMS 4\000"
.LASF148:
	.ascii	"__FLT_RADIX__ 2\000"
.LASF115:
	.ascii	"__INT32_C(c) c\000"
.LASF356:
	.ascii	"__USA_FBIT__ 16\000"
.LASF1080:
	.ascii	"TEE_TYPE_SM2_KEP_KEYPAIR 0xA1000046\000"
.LASF1281:
	.ascii	"_ANSI_STDARG_H_ \000"
.LASF197:
	.ascii	"__LDBL_IS_IEC_60559__ 2\000"
.LASF386:
	.ascii	"__SIZEOF_WCHAR_T__ 4\000"
.LASF231:
	.ascii	"__FLT32X_DIG__ 15\000"
.LASF1715:
	.ascii	"SCTLR_RR BIT32(14)\000"
.LASF1637:
	.ascii	"CLIDR_LOUIS_SHIFT U(21)\000"
.LASF192:
	.ascii	"__LDBL_EPSILON__ 2.2204460492503131e-16L\000"
.LASF750:
	.ascii	"INT_LEAST64_MIN INT64_MIN\000"
.LASF450:
	.ascii	"__ARM_BF16_FORMAT_ALTERNATIVE\000"
.LASF1671:
	.ascii	"ARM32_CPSR_IT_MASK2 U(0x0000fc00)\000"
.LASF666:
	.ascii	"COMMENT_CHAR \"//\"\000"
.LASF146:
	.ascii	"__FLT_EVAL_METHOD_TS_18661_3__ 0\000"
.LASF659:
	.ascii	"__maybe_unused __attribute__((unused))\000"
.LASF330:
	.ascii	"__SQ_FBIT__ 31\000"
.LASF94:
	.ascii	"__UINTMAX_MAX__ 0xffffffffffffffffULL\000"
.LASF1342:
	.ascii	"PRIxUA PRIxPTR\000"
.LASF512:
	.ascii	"CFG_EARLY_TA_COMPRESS 1\000"
.LASF357:
	.ascii	"__USA_IBIT__ 16\000"
.LASF300:
	.ascii	"__ACCUM_EPSILON__ 0x1P-15K\000"
.LASF1141:
	.ascii	"TEE_PANIC_ID_TA_DESTROYENTRYPOINT 0x00000103\000"
.LASF980:
	.ascii	"TEE_ALG_AES_XTS 0x10000410\000"
.LASF1899:
	.ascii	"__TEE_MAIN_HASH_SM3 0x7\000"
.LASF2106:
	.ascii	"STM32MP135A_PART_NB U(0x05010001)\000"
.LASF1680:
	.ascii	"CPSR_MODE_FIQ ARM32_CPSR_MODE_FIQ\000"
.LASF309:
	.ascii	"__LACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LK\000"
.LASF1200:
	.ascii	"TEE_PANIC_ID_TEE_TRUNCATEOBJECTDATA 0x00000B03\000"
.LASF715:
	.ascii	"CHAR_MIN 0\000"
.LASF950:
	.ascii	"TEE_DATA_FLAG_ACCESS_WRITE_META 0x00000004\000"
.LASF671:
	.ascii	"__relrodata_unpaged(x) __rodata_unpaged(x)\000"
.LASF1401:
	.ascii	"STAILQ_REMOVE(head,elm,type,field) do { if ((head)-"
	.ascii	">stqh_first == (elm)) { STAILQ_REMOVE_HEAD((head), "
	.ascii	"field); } else { struct type *curelm = (head)->stqh"
	.ascii	"_first; while (curelm->field.stqe_next != (elm)) cu"
	.ascii	"relm = curelm->field.stqe_next; if ((curelm->field."
	.ascii	"stqe_next = curelm->field.stqe_next->field.stqe_nex"
	.ascii	"t) == NULL) (head)->stqh_last = &(curelm)->field.st"
	.ascii	"qe_next; } } while ( 0)\000"
.LASF1183:
	.ascii	"TEE_PANIC_ID_TEE_INITREFATTRIBUTE 0x00000805\000"
.LASF1130:
	.ascii	"TEE_ATTR_BIT_VALUE TEE_ATTR_FLAG_VALUE\000"
.LASF669:
	.ascii	"__rodata_dummy __section(\".rodata.dummy\" __SECTIO"
	.ascii	"N_FLAGS_RODATA)\000"
.LASF2102:
	.ascii	"SYSRAM_SIZE 0x40000\000"
.LASF1011:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA224 0x60310230\000"
.LASF260:
	.ascii	"__FRACT_EPSILON__ 0x1P-15R\000"
.LASF230:
	.ascii	"__FLT32X_MANT_DIG__ 53\000"
.LASF1470:
	.ascii	"MUTEX_INITIALIZER { .wq = WAIT_QUEUE_INITIALIZER }\000"
.LASF1826:
	.ascii	"TEE_ERROR_NODE_DISABLED 0x80000001\000"
.LASF65:
	.ascii	"__UINT_FAST8_TYPE__ unsigned char\000"
.LASF783:
	.ascii	"INT8_C(v) v\000"
.LASF43:
	.ascii	"__CHAR32_TYPE__ unsigned int\000"
.LASF722:
	.ascii	"MB_LEN_MAX 1\000"
.LASF85:
	.ascii	"__INT_WIDTH__ 32\000"
.LASF1188:
	.ascii	"TEE_PANIC_ID_TEE_CLOSEANDDELETEPERSISTENTOBJECT 0x0"
	.ascii	"0000901\000"
.LASF2068:
	.ascii	"CFG0_CLOSED_MASK BIT(CFG0_OTP_SECURED_POS)\000"
.LASF1660:
	.ascii	"ARM32_CPSR_MODE_UND U(0x1b)\000"
.LASF730:
	.ascii	"ULONG_MAX (LONG_MAX * 2UL + 1UL)\000"
.LASF2044:
	.ascii	"TIM16_BASE 0x44007000\000"
.LASF753:
	.ascii	"INT_LEAST32_MAX INT32_MAX\000"
.LASF1835:
	.ascii	"TEE_ATTR_HKDF_SALT 0xD00002C0\000"
.LASF766:
	.ascii	"INT_FAST64_MAX INT64_MAX\000"
.LASF213:
	.ascii	"__FLT32_IS_IEC_60559__ 2\000"
.LASF342:
	.ascii	"__UDQ_FBIT__ 64\000"
.LASF922:
	.ascii	"TEE_PARAM_TYPE_NONE 0\000"
.LASF1960:
	.ascii	"TZDRAM_BASE CFG_TZDRAM_START\000"
.LASF932:
	.ascii	"TEE_LOGIN_APPLICATION 0x00000004\000"
.LASF963:
	.ascii	"TEE_HANDLE_FLAG_PERSISTENT 0x00010000\000"
.LASF1778:
	.ascii	"FSR_LPAE BIT32(9)\000"
.LASF1192:
	.ascii	"TEE_PANIC_ID_TEE_CLOSEANDDELETEPERSISTENTOBJECT1 0x"
	.ascii	"00000905\000"
.LASF1494:
	.ascii	"SIZE_2M UINTPTR_C(0x200000)\000"
.LASF1474:
	.ascii	"__STDLIB_H \000"
.LASF1081:
	.ascii	"TEE_TYPE_SM2_PKE_PUBLIC_KEY 0xA0000047\000"
.LASF1596:
	.ascii	"MIDR_REVISION_WIDTH U(4)\000"
.LASF739:
	.ascii	"INT8_MAX 0x7f\000"
.LASF78:
	.ascii	"__WCHAR_MIN__ 0U\000"
.LASF960:
	.ascii	"TEE_USAGE_SIGN 0x00000010\000"
.LASF1065:
	.ascii	"TEE_TYPE_HMAC_SM3 0xA0000007\000"
.LASF1727:
	.ascii	"ACTLR_CA15_ENABLE_INVALIDATE_BTB BIT(0)\000"
.LASF236:
	.ascii	"__FLT32X_DECIMAL_DIG__ 17\000"
.LASF924:
	.ascii	"TEE_PARAM_TYPE_VALUE_OUTPUT 2\000"
.LASF1103:
	.ascii	"TEE_ATTR_DH_SUBPRIME 0xD0001132\000"
.LASF373:
	.ascii	"__GCC_ATOMIC_CHAR32_T_LOCK_FREE 2\000"
.LASF2038:
	.ascii	"TIM7_BASE 0x40005000\000"
.LASF1323:
	.ascii	"preinitcall_end SCATTERED_ARRAY_END(preinitcall, st"
	.ascii	"ruct initcall)\000"
.LASF46:
	.ascii	"__INT16_TYPE__ short int\000"
.LASF2137:
	.ascii	"TRUSTED_DRAM_SIZE TZDRAM_SIZE\000"
.LASF614:
	.ascii	"CFG_TEE_FW_MANUFACTURER FW_MAN_UNDEF\000"
.LASF1962:
	.ascii	"TEE_RAM_START TZDRAM_BASE\000"
.LASF2109:
	.ascii	"STM32MP131C_PART_NB U(0x050106C8)\000"
.LASF1407:
	.ascii	"STAILQ_FIRST(head) ((head)->stqh_first)\000"
.LASF967:
	.ascii	"TEE_OPERATION_CIPHER 1\000"
.LASF258:
	.ascii	"__FRACT_MIN__ (-0.5R-0.5R)\000"
.LASF271:
	.ascii	"__ULFRACT_FBIT__ 32\000"
.LASF746:
	.ascii	"UINT64_MAX 0xffffffffffffffffUL\000"
.LASF859:
	.ascii	"___int_size_t_h \000"
.LASF471:
	.ascii	"CFG_ATTESTATION_PTA_KEY_SIZE 3072\000"
.LASF59:
	.ascii	"__UINT_LEAST32_TYPE__ unsigned int\000"
.LASF590:
	.ascii	"CFG_STM32_SAES_SW_FALLBACK 1\000"
.LASF1441:
	.ascii	"TAILQ_FOREACH_REVERSE_SAFE(var,head,headname,field,"
	.ascii	"prev) for ((var) = TAILQ_LAST((head), headname); (v"
	.ascii	"ar) && ((prev) = TAILQ_PREV((var), headname, field)"
	.ascii	", 1); (var) = (prev))\000"
.LASF854:
	.ascii	"_SIZE_T_DEFINED_ \000"
.LASF479:
	.ascii	"CFG_CORE_DYN_SHM 1\000"
.LASF1193:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATEPERSISTENTOBJECTENUMERATOR"
	.ascii	" 0x00000A01\000"
.LASF377:
	.ascii	"__GCC_ATOMIC_LONG_LOCK_FREE 2\000"
.LASF1425:
	.ascii	"_TAILQ_ENTRY(type,qual) struct { qual type *tqe_nex"
	.ascii	"t; qual type *qual *tqe_prev; }\000"
.LASF2116:
	.ascii	"STM32MP131D_PART_NB U(0x05010EC9)\000"
.LASF451:
	.ascii	"__GXX_TYPEINFO_EQUALITY_INLINE 0\000"
.LASF771:
	.ascii	"INTPTR_MIN LONG_MIN\000"
.LASF338:
	.ascii	"__UHQ_FBIT__ 16\000"
.LASF80:
	.ascii	"__WINT_MIN__ 0U\000"
.LASF663:
	.ascii	"__section(x) __attribute__((section(x)))\000"
.LASF1719:
	.ascii	"SCTLR_FI BIT32(21)\000"
.LASF1167:
	.ascii	"TEE_PANIC_ID_TEE_MEMCOMPARE 0x00000605\000"
.LASF2089:
	.ascii	"USART3_BASE UART3_BASE\000"
.LASF1018:
	.ascii	"TEE_ALG_DSA_SHA256 0x70004131\000"
.LASF760:
	.ascii	"INT_FAST16_MIN INT16_MIN\000"
.LASF1907:
	.ascii	"TEE_AES_BLOCK_SIZE 16UL\000"
.LASF1766:
	.ascii	"TTBCR_SH1_SHIFT U(28)\000"
.LASF552:
	.ascii	"CFG_SCMI_MSG_RESET_DOMAIN 1\000"
.LASF292:
	.ascii	"__USACCUM_IBIT__ 8\000"
.LASF882:
	.ascii	"NULL\000"
.LASF1650:
	.ascii	"CTR_IMINLINE_SHIFT U(0)\000"
.LASF1403:
	.ascii	"STAILQ_FOREACH(var,head,field) for ((var) = ((head)"
	.ascii	"->stqh_first); (var); (var) = ((var)->field.stqe_ne"
	.ascii	"xt))\000"
.LASF1302:
	.ascii	"MAX_PRINT_SIZE 256\000"
.LASF1457:
	.ascii	"CIRCLEQ_INSERT_HEAD(head,elm,field) do { QUEUEDEBUG"
	.ascii	"_CIRCLEQ_HEAD((head), field) (elm)->field.cqe_next "
	.ascii	"= (head)->cqh_first; (elm)->field.cqe_prev = (void "
	.ascii	"*)(head); if ((head)->cqh_last == (void *)(head)) ("
	.ascii	"head)->cqh_last = (elm); else (head)->cqh_first->fi"
	.ascii	"eld.cqe_prev = (elm); (head)->cqh_first = (elm); } "
	.ascii	"while ( 0)\000"
.LASF674:
	.ascii	"__noprof __attribute__((no_instrument_function))\000"
.LASF2035:
	.ascii	"TIM4_BASE 0x40002000\000"
.LASF916:
	.ascii	"TEE_ERROR_TARGET_DEAD 0xFFFF3024\000"
.LASF895:
	.ascii	"TEE_ERROR_STORAGE_NOT_AVAILABLE_2 0xF0100004\000"
.LASF2037:
	.ascii	"TIM6_BASE 0x40004000\000"
.LASF1198:
	.ascii	"TEE_PANIC_ID_TEE_READOBJECTDATA 0x00000B01\000"
.LASF1149:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASIDENTITY 0x00000206\000"
.LASF686:
	.ascii	"__compiler_atomic_load(p) __atomic_load_n((p), __AT"
	.ascii	"OMIC_RELAXED)\000"
.LASF625:
	.ascii	"CFG_WERROR 1\000"
.LASF1100:
	.ascii	"TEE_ATTR_DSA_PUBLIC_VALUE 0xD0000131\000"
.LASF791:
	.ascii	"UINTPTR_C(v) UL(v)\000"
.LASF1818:
	.ascii	"THREAD_RPC_NUM_ARGS 4\000"
.LASF1150:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASSTRING 0x00000207\000"
.LASF2058:
	.ascii	"ROM_SIZE 0x20000\000"
.LASF877:
	.ascii	"_GCC_WCHAR_T \000"
.LASF1483:
	.ascii	"nex_memalign(alignment,size) memalign(alignment, si"
	.ascii	"ze)\000"
.LASF1044:
	.ascii	"TEE_ALG_ECDSA_P521 0x70005041\000"
.LASF1737:
	.ascii	"CPACR_ASEDIS BIT32(31)\000"
.LASF1950:
	.ascii	"__CONFIG_H_ \000"
.LASF734:
	.ascii	"__uint32_t_defined \000"
.LASF1047:
	.ascii	"TEE_ALG_ECDH_P224 0x80002042\000"
.LASF1222:
	.ascii	"TEE_PANIC_ID_TEE_AEUPDATE 0x00001004\000"
.LASF1273:
	.ascii	"TEE_MEM_OUTPUT 0x00000002\000"
.LASF135:
	.ascii	"__INT_FAST64_WIDTH__ 64\000"
.LASF256:
	.ascii	"__FRACT_FBIT__ 15\000"
.LASF1911:
	.ascii	"TEE_MAC_SIZE_AES_CBC_MAC_NOPAD \000"
.LASF1482:
	.ascii	"nex_realloc(ptr,size) realloc(ptr, size)\000"
.LASF1669:
	.ascii	"ARM32_CPSR_IT_MASK (ARM32_CPSR_IT_MASK1 | ARM32_CPS"
	.ascii	"R_IT_MASK2)\000"
.LASF1756:
	.ascii	"TTBCR_T0SZ_SHIFT U(0)\000"
.LASF984:
	.ascii	"TEE_ALG_AES_CCM 0x40000710\000"
.LASF1284:
	.ascii	"va_start(v,l) __builtin_va_start(v,l)\000"
.LASF2098:
	.ascii	"BKPSRAM_PM_MAILBOX_OFFSET BKPSRAM_PM_OFFSET\000"
.LASF616:
	.ascii	"CFG_TEE_MANUFACTURER LINARO\000"
.LASF365:
	.ascii	"__CHAR_UNSIGNED__ 1\000"
.LASF883:
	.ascii	"NULL ((void *)0)\000"
.LASF2076:
	.ascii	"GIC_MPU_WAKEUP_PIN 157\000"
.LASF1635:
	.ascii	"MPIDR_CLUSTER_MASK MPIDR_AFF1_MASK\000"
.LASF1352:
	.ascii	"PGT_NUM_PGT_PER_PAGE 1\000"
.LASF880:
	.ascii	"_BSD_WCHAR_T_\000"
.LASF463:
	.ascii	"TRACE_LEVEL 2\000"
.LASF72:
	.ascii	"__SCHAR_MAX__ 0x7f\000"
.LASF527:
	.ascii	"CFG_KERN_LINKER_ARCH arm\000"
.LASF384:
	.ascii	"__GCC_HAVE_DWARF2_CFI_ASM 1\000"
.LASF1532:
	.ascii	"TEE_MATTR_PW BIT(5)\000"
.LASF1940:
	.ascii	"HW_UNIQUE_KEY_WORD4 (HW_UNIQUE_KEY_WORD1 + 3)\000"
.LASF2045:
	.ascii	"TIM17_BASE 0x44008000\000"
.LASF852:
	.ascii	"_SIZE_T_ \000"
.LASF1058:
	.ascii	"TEE_TYPE_SM4 0xA0000014\000"
.LASF327:
	.ascii	"__QQ_IBIT__ 0\000"
.LASF290:
	.ascii	"__SACCUM_EPSILON__ 0x1P-7HK\000"
.LASF1035:
	.ascii	"TEE_ALG_HMAC_SHA224 0x30000003\000"
.LASF1689:
	.ascii	"CPSR_F ARM32_CPSR_F\000"
.LASF601:
	.ascii	"CFG_TA_BGET_TEST 1\000"
.LASF949:
	.ascii	"TEE_DATA_FLAG_ACCESS_WRITE 0x00000002\000"
.LASF863:
	.ascii	"__need_size_t\000"
.LASF1563:
	.ascii	"TA_FLAG_MULTI_SESSION (1 << 3)\000"
.LASF517:
	.ascii	"CFG_FTRACE_BUF_WHEN_FULL shift\000"
.LASF284:
	.ascii	"__ULLFRACT_MAX__ 0XFFFFFFFFFFFFFFFFP-64ULLR\000"
.LASF1793:
	.ascii	"KERNEL_VFP_H \000"
.LASF844:
	.ascii	"__need_ptrdiff_t\000"
.LASF1473:
	.ascii	"TEE_COMMON_H \000"
.LASF2057:
	.ascii	"ROM_BASE 0\000"
.LASF101:
	.ascii	"__INT16_MAX__ 0x7fff\000"
.LASF333:
	.ascii	"__DQ_IBIT__ 0\000"
.LASF1553:
	.ascii	"VM_FLAG_EPHEMERAL BIT(0)\000"
.LASF1489:
	.ascii	"__KERNEL_TS_MANAGER_H \000"
.LASF346:
	.ascii	"__HA_FBIT__ 7\000"
.LASF716:
	.ascii	"INT_MAX __INT_MAX__\000"
.LASF2055:
	.ascii	"UART8_BASE 0x40019000\000"
.LASF303:
	.ascii	"__UACCUM_MIN__ 0.0UK\000"
.LASF1006:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256 0x70414930\000"
.LASF1097:
	.ascii	"TEE_ATTR_DSA_PRIME 0xD0001031\000"
.LASF2025:
	.ascii	"RNG1_BASE 0x54003000\000"
.LASF2095:
	.ascii	"BKPSRAM_SIZE U(0x1000)\000"
.LASF820:
	.ascii	"PRIxPTR __PRIPTR_PREFIX \"x\"\000"
.LASF650:
	.ascii	"__packed __attribute__((packed))\000"
.LASF598:
	.ascii	"CFG_TA_ASLR 1\000"
.LASF1199:
	.ascii	"TEE_PANIC_ID_TEE_SEEKOBJECTDATA 0x00000B02\000"
.LASF627:
	.ascii	"CFG_WITH_NSEC_GPIOS 1\000"
.LASF819:
	.ascii	"PRIx64 __PRI64_PREFIX \"x\"\000"
.LASF163:
	.ascii	"__FLT_HAS_QUIET_NAN__ 1\000"
.LASF1363:
	.ascii	"LIST_HEAD(name,type) struct name { struct type *lh_"
	.ascii	"first; }\000"
.LASF2114:
	.ascii	"STM32MP133D_PART_NB U(0x050108C1)\000"
.LASF1529:
	.ascii	"TEE_MATTR_VALID_BLOCK BIT(0)\000"
.LASF1041:
	.ascii	"TEE_ALG_ECDSA_P224 0x70002041\000"
.LASF1597:
	.ascii	"MIDR_REVISION_MASK (BIT(MIDR_REVISION_WIDTH) - 1)\000"
.LASF628:
	.ascii	"CFG_WITH_NSEC_UARTS 1\000"
.LASF1974:
	.ascii	"APB3_SIZE 0x0000b000\000"
.LASF1713:
	.ascii	"SCTLR_I BIT32(12)\000"
.LASF1257:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTMOD 0x00001A03\000"
.LASF238:
	.ascii	"__FLT32X_NORM_MAX__ 1.7976931348623157e+308F32x\000"
.LASF1491:
	.ascii	"UTIL_H \000"
.LASF521:
	.ascii	"CFG_HWRNG_PTA 1\000"
.LASF1809:
	.ascii	"SM_SM_H \000"
.LASF1578:
	.ascii	"TA_PROP_STR_DESCRIPTION \"gpd.ta.description\"\000"
.LASF1692:
	.ascii	"CPSR_FIA ARM32_CPSR_FIA\000"
.LASF891:
	.ascii	"TEE_SUCCESS 0x00000000\000"
.LASF1603:
	.ascii	"CORTEX_A17_PART_NUM U(0xC0E)\000"
.LASF1385:
	.ascii	"SLIST_REMOVE_HEAD(head,field) do { (head)->slh_firs"
	.ascii	"t = (head)->slh_first->field.sle_next; } while ( 0)"
	.ascii	"\000"
.LASF2185:
	.ascii	"STACK_CANARY_SIZE (4 * sizeof(long))\000"
.LASF1085:
	.ascii	"TEE_TYPE_DATA 0xA00000BF\000"
.LASF708:
	.ascii	"SCATTERED_ARRAY_END(array_name,element_type) (__ext"
	.ascii	"ension__({ static const element_type __scattered_ar"
	.ascii	"ray_end[0] __unused __section(\".scattered_array_\""
	.ascii	" #array_name \"_2\"); __scattered_array_end; }))\000"
.LASF83:
	.ascii	"__SCHAR_WIDTH__ 8\000"
.LASF1493:
	.ascii	"SIZE_1M UINTPTR_C(0x100000)\000"
.LASF430:
	.ascii	"__ARM_FEATURE_FP16_FML\000"
.LASF526:
	.ascii	"CFG_INIT_CNTVOFF 1\000"
.LASF898:
	.ascii	"TEE_ERROR_ACCESS_DENIED 0xFFFF0001\000"
.LASF30:
	.ascii	"__ORDER_PDP_ENDIAN__ 3412\000"
.LASF579:
	.ascii	"CFG_STM32_GPIO 1\000"
.LASF11:
	.ascii	"__ATOMIC_ACQUIRE 2\000"
.LASF2092:
	.ascii	"DBGMCU_IDC_DEV_ID_MASK GENMASK_32(11, 0)\000"
.LASF1106:
	.ascii	"TEE_ATTR_DH_PUBLIC_VALUE 0xD0000132\000"
.LASF133:
	.ascii	"__INT_FAST32_WIDTH__ 32\000"
.LASF388:
	.ascii	"__SIZEOF_PTRDIFF_T__ 4\000"
.LASF2099:
	.ascii	"BKPSRAM_PM_MAILBOX_SIZE U(0x100)\000"
.LASF64:
	.ascii	"__INT_FAST64_TYPE__ long long int\000"
.LASF1780:
	.ascii	"FSR_STATUS_MASK (BIT32(6) - 1)\000"
.LASF296:
	.ascii	"__ACCUM_FBIT__ 15\000"
.LASF1741:
	.ascii	"CPACR_CP_ACCESS_PL1_ONLY U(0x1)\000"
.LASF1250:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTDIV 0x00001902\000"
.LASF117:
	.ascii	"__INT_LEAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF1921:
	.ascii	"TEE_U16_FROM_LITTLE_ENDIAN(x) ((uint16_t)(x))\000"
.LASF390:
	.ascii	"__ARM_FEATURE_QBIT 1\000"
.LASF539:
	.ascii	"CFG_PM 1\000"
.LASF943:
	.ascii	"TEE_MEMORY_ACCESS_READ 0x00000001\000"
.LASF892:
	.ascii	"TEE_ERROR_CORRUPT_OBJECT 0xF0100001\000"
.LASF939:
	.ascii	"TEE_ORIGIN_TRUSTED_APP 0x00000004\000"
.LASF289:
	.ascii	"__SACCUM_MAX__ 0X7FFFP-7HK\000"
.LASF1045:
	.ascii	"TEE_ALG_ED25519 0x70006043\000"
.LASF1276:
	.ascii	"TEE_MEMREF_2_USED 0x00000004\000"
.LASF1501:
	.ascii	"MIN_UNSAFE(a,b) (((a) < (b)) ? (a) : (b))\000"
.LASF805:
	.ascii	"PRIiPTR __PRIPTR_PREFIX \"i\"\000"
.LASF1828:
	.ascii	"TEE_ALG_HKDF_SHA1_DERIVE_KEY 0x800020C0\000"
.LASF1325:
	.ascii	"initcall_end SCATTERED_ARRAY_END(initcall, struct i"
	.ascii	"nitcall)\000"
.LASF2014:
	.ascii	"I2C6_BASE 0x5c009000\000"
.LASF1801:
	.ascii	"THREAD_FLAGS_EXIT_ON_FOREIGN_INTR BIT(2)\000"
.LASF785:
	.ascii	"INT16_C(v) v\000"
.LASF2054:
	.ascii	"UART7_BASE 0x40018000\000"
.LASF96:
	.ascii	"__INTMAX_WIDTH__ 64\000"
.LASF1176:
	.ascii	"TEE_PANIC_ID_TEE_RESTRICTOBJECTUSAGE 0x00000705\000"
.LASF1869:
	.ascii	"TEE_MAIN_ALGO_DES 0x11\000"
.LASF1283:
	.ascii	"__GNUC_VA_LIST \000"
.LASF70:
	.ascii	"__UINTPTR_TYPE__ unsigned int\000"
.LASF626:
	.ascii	"CFG_WITH_LPAE 1\000"
.LASF958:
	.ascii	"TEE_USAGE_DECRYPT 0x00000004\000"
.LASF1859:
	.ascii	"TEE_MEMORY_ACCESS_SECURE 0x20000000\000"
.LASF959:
	.ascii	"TEE_USAGE_MAC 0x00000008\000"
.LASF119:
	.ascii	"__INT_LEAST64_WIDTH__ 64\000"
.LASF2164:
	.ascii	"register_dynamic_shm(addr,size) __register_memory(#"
	.ascii	"addr, MEM_AREA_DDR_OVERALL, (addr), (size), phys_dd"
	.ascii	"r_overall_compat)\000"
.LASF1725:
	.ascii	"SCTLR_AFE BIT32(29)\000"
.LASF1973:
	.ascii	"APB3_BASE 0x50020000\000"
.LASF1997:
	.ascii	"ETZPC_BASE 0x5c007000\000"
.LASF398:
	.ascii	"__ARM_32BIT_STATE 1\000"
.LASF9:
	.ascii	"__ATOMIC_RELAXED 0\000"
.LASF1994:
	.ascii	"DDRPHYC_BASE 0x5a004000\000"
.LASF1986:
	.ascii	"ADC2_BASE 0x48004000\000"
.LASF269:
	.ascii	"__LFRACT_MAX__ 0X7FFFFFFFP-31LR\000"
.LASF191:
	.ascii	"__LDBL_MIN__ 2.2250738585072014e-308L\000"
.LASF661:
	.ascii	"__must_check __attribute__((warn_unused_result))\000"
.LASF549:
	.ascii	"CFG_RPMB_FS_RD_ENTRIES 8\000"
.LASF593:
	.ascii	"CFG_STM32_TAMP_NVRAM 1\000"
.LASF1711:
	.ascii	"SCTLR_SW BIT32(10)\000"
.LASF1177:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTINFO1 0x00000706\000"
.LASF937:
	.ascii	"TEE_ORIGIN_COMMS 0x00000002\000"
.LASF435:
	.ascii	"__THUMB_INTERWORK__ 1\000"
.LASF1577:
	.ascii	"TA_PROP_STR_VERSION \"gpd.ta.version\"\000"
.LASF1406:
	.ascii	"STAILQ_EMPTY(head) ((head)->stqh_first == NULL)\000"
.LASF1893:
	.ascii	"TEE_CHAIN_MODE_CCM 0x7\000"
.LASF1939:
	.ascii	"HW_UNIQUE_KEY_WORD3 (HW_UNIQUE_KEY_WORD1 + 2)\000"
.LASF1867:
	.ascii	"TEE_MAIN_ALGO_SM3 0x07\000"
.LASF649:
	.ascii	"__deprecated __attribute__((deprecated))\000"
.LASF176:
	.ascii	"__DBL_DENORM_MIN__ ((double)4.9406564584124654e-324"
	.ascii	"L)\000"
.LASF1872:
	.ascii	"TEE_MAIN_ALGO_SM4 0x14\000"
.LASF736:
	.ascii	"INT16_MIN (-0x7fff-1)\000"
.LASF1624:
	.ascii	"MPIDR_AFFLVL_MASK U(0xff)\000"
.LASF259:
	.ascii	"__FRACT_MAX__ 0X7FFFP-15R\000"
.LASF1524:
	.ascii	"GENMASK_64(h,l) (((~UINT64_C(0)) << (l)) & (~UINT64"
	.ascii	"_C(0) >> (64 - 1 - (h))))\000"
.LASF143:
	.ascii	"__GCC_IEC_559 0\000"
.LASF164:
	.ascii	"__FLT_IS_IEC_60559__ 2\000"
.LASF573:
	.ascii	"CFG_STM32_BSEC_WRITE 1\000"
.LASF240:
	.ascii	"__FLT32X_EPSILON__ 2.2204460492503131e-16F32x\000"
.LASF1436:
	.ascii	"TAILQ_INSERT_BEFORE(listelm,elm,field) do { QUEUEDE"
	.ascii	"BUG_TAILQ_OP((listelm), field) (elm)->field.tqe_pre"
	.ascii	"v = (listelm)->field.tqe_prev; (elm)->field.tqe_nex"
	.ascii	"t = (listelm); *(listelm)->field.tqe_prev = (elm); "
	.ascii	"(listelm)->field.tqe_prev = &(elm)->field.tqe_next;"
	.ascii	" } while ( 0)\000"
.LASF1840:
	.ascii	"TEE_ALG_CONCAT_KDF_SHA256_DERIVE_KEY 0x800040C1\000"
.LASF636:
	.ascii	"_CFG_CORE_LTC_MPI 1\000"
.LASF262:
	.ascii	"__UFRACT_IBIT__ 0\000"
.LASF535:
	.ascii	"CFG_NUM_THREADS 10\000"
.LASF1164:
	.ascii	"TEE_PANIC_ID_TEE_FREE 0x00000602\000"
.LASF2033:
	.ascii	"TIM2_BASE 0x40000000\000"
.LASF742:
	.ascii	"INT64_MAX 0x7fffffffffffffffL\000"
.LASF2121:
	.ascii	"STM32MP151C_PART_NB U(0x0500002E)\000"
.LASF2162:
	.ascii	"register_phys_mem_pgdir(type,addr,size) register_ph"
	.ascii	"ys_mem(type, ROUNDDOWN(addr, CORE_MMU_PGDIR_SIZE), "
	.ascii	"ROUNDUP(size + addr - ROUNDDOWN(addr, CORE_MMU_PGDI"
	.ascii	"R_SIZE), CORE_MMU_PGDIR_SIZE))\000"
.LASF1381:
	.ascii	"SLIST_ENTRY(type) struct { struct type *sle_next; }"
	.ascii	"\000"
.LASF2170:
	.ascii	"phys_sdp_mem_begin SCATTERED_ARRAY_BEGIN(phys_sdp_m"
	.ascii	"em, struct core_mmu_phys_mem)\000"
.LASF1334:
	.ascii	"service_init_late(fn) __define_initcall(init, 4, fn"
	.ascii	")\000"
.LASF1770:
	.ascii	"TTBCR_XRGNX_WBWA U(0x3)\000"
.LASF1498:
	.ascii	"MAX(a,b) (__extension__({ __typeof__(a) _a = (a); _"
	.ascii	"_typeof__(b) _b = (b); _a > _b ? _a : _b; }))\000"
.LASF1364:
	.ascii	"LIST_HEAD_INITIALIZER(head) { NULL }\000"
.LASF1539:
	.ascii	"TEE_MATTR_UX BIT(9)\000"
.LASF464:
	.ascii	"NDEBUG 1\000"
.LASF752:
	.ascii	"INT_LEAST16_MAX INT16_MAX\000"
.LASF267:
	.ascii	"__LFRACT_IBIT__ 0\000"
.LASF504:
	.ascii	"CFG_DRIVERS_CLK 1\000"
.LASF648:
	.ascii	"COMPILER_H \000"
.LASF1628:
	.ascii	"MPIDR_AFF1_MASK (MPIDR_AFFLVL_MASK << MPIDR_AFF1_SH"
	.ascii	"IFT)\000"
.LASF1223:
	.ascii	"TEE_PANIC_ID_TEE_AEUPDATEAAD 0x00001005\000"
.LASF1615:
	.ascii	"CORTEX_A78C_PART_NUM U(0xD4B)\000"
.LASF1761:
	.ascii	"TTBCR_T1SZ_SHIFT U(16)\000"
.LASF413:
	.ascii	"__ARM_ARCH_ISA_ARM 1\000"
.LASF878:
	.ascii	"_WCHAR_T_DECLARED \000"
.LASF492:
	.ascii	"CFG_CORE_WORKAROUND_SPECTRE_BP_SEC 1\000"
.LASF12:
	.ascii	"__ATOMIC_RELEASE 3\000"
.LASF1934:
	.ascii	"TEE_TIME_ADD(t1,t2,dst) do { (dst).seconds = (t1).s"
	.ascii	"econds + (t2).seconds; (dst).millis = (t1).millis +"
	.ascii	" (t2).millis; if ((dst).millis >= TEE_TIME_MILLIS_B"
	.ascii	"ASE) { (dst).seconds++; (dst).millis -= TEE_TIME_MI"
	.ascii	"LLIS_BASE; } } while (0)\000"
.LASF1354:
	.ascii	"assert(expr) ((void)0)\000"
.LASF1397:
	.ascii	"STAILQ_INSERT_HEAD(head,elm,field) do { if (((elm)-"
	.ascii	">field.stqe_next = (head)->stqh_first) == NULL) (he"
	.ascii	"ad)->stqh_last = &(elm)->field.stqe_next; (head)->s"
	.ascii	"tqh_first = (elm); } while ( 0)\000"
.LASF110:
	.ascii	"__INT_LEAST8_WIDTH__ 8\000"
.LASF1340:
	.ascii	"__UNISTD_H \000"
.LASF495:
	.ascii	"CFG_CRYPTOLIB_NAME tomcrypt\000"
.LASF944:
	.ascii	"TEE_MEMORY_ACCESS_WRITE 0x00000002\000"
.LASF5:
	.ascii	"__GNUC__ 12\000"
.LASF1053:
	.ascii	"TEE_ALG_X25519 0x80000044\000"
.LASF733:
	.ascii	"__int8_t_defined \000"
.LASF547:
	.ascii	"CFG_RPMB_FS_CACHE_ENTRIES 0\000"
.LASF1322:
	.ascii	"preinitcall_begin SCATTERED_ARRAY_BEGIN(preinitcall"
	.ascii	", struct initcall)\000"
.LASF1428:
	.ascii	"QUEUEDEBUG_TAILQ_INSERT_TAIL(head,elm,field) \000"
.LASF446:
	.ascii	"__ARM_FEATURE_CDE_COPROC\000"
.LASF116:
	.ascii	"__INT_LEAST32_WIDTH__ 32\000"
.LASF345:
	.ascii	"__UTQ_IBIT__ 0\000"
.LASF443:
	.ascii	"__ARM_FEATURE_COPROC\000"
.LASF180:
	.ascii	"__DBL_IS_IEC_60559__ 2\000"
.LASF1592:
	.ascii	"MIDR_VARIANT_SHIFT U(20)\000"
.LASF1186:
	.ascii	"TEE_PANIC_ID_TEE_RESETTRANSIENTOBJECT 0x00000808\000"
.LASF1254:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSUB 0x00001906\000"
.LASF1860:
	.ascii	"TEE_LOGIN_REE_KERNEL 0x80000000\000"
.LASF554:
	.ascii	"CFG_SCMI_MSG_VOLTAGE_DOMAIN 1\000"
.LASF700:
	.ascii	"__SCT_ARRAY_DEF_PG_ITEM2(array_name,order,id,elemen"
	.ascii	"t_type) __SCT_ARRAY_DEF_PG_ITEM3(element_type, __sc"
	.ascii	"attered_array_ ## id ## array_name, \".scattered_ar"
	.ascii	"ray_\" #array_name \"_1_\" #order)\000"
.LASF965:
	.ascii	"TEE_HANDLE_FLAG_KEY_SET 0x00040000\000"
.LASF2146:
	.ascii	"TEE_LOAD_ADDR TEE_RAM_START\000"
.LASF1020:
	.ascii	"TEE_ALG_DH_DERIVE_SHARED_SECRET 0x80000032\000"
.LASF391:
	.ascii	"__ARM_FEATURE_SAT 1\000"
.LASF798:
	.ascii	"PRId32 \"d\"\000"
.LASF74:
	.ascii	"__INT_MAX__ 0x7fffffff\000"
.LASF1007:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA384 0x70515930\000"
.LASF156:
	.ascii	"__FLT_MAX__ 3.4028234663852886e+38F\000"
.LASF53:
	.ascii	"__INT_LEAST8_TYPE__ signed char\000"
.LASF71:
	.ascii	"__GXX_ABI_VERSION 1017\000"
.LASF1445:
	.ascii	"TAILQ_NEXT(elm,field) ((elm)->field.tqe_next)\000"
.LASF1464:
	.ascii	"CIRCLEQ_LAST(head) ((head)->cqh_last)\000"
.LASF2056:
	.ascii	"VREFBUF_BASE 0x50025000\000"
.LASF1815:
	.ascii	"STACK_THREAD_SIZE (8192 + CFG_STACK_THREAD_EXTRA)\000"
.LASF2021:
	.ascii	"PWR_BASE 0x50001000\000"
.LASF976:
	.ascii	"TEE_ALG_AES_ECB_NOPAD 0x10000010\000"
.LASF532:
	.ascii	"CFG_MAX_CACHE_LINE_SHIFT 6\000"
.LASF266:
	.ascii	"__LFRACT_FBIT__ 31\000"
.LASF574:
	.ascii	"CFG_STM32_CLKCALIB 1\000"
.LASF419:
	.ascii	"__ARM_ARCH_ISA_THUMB\000"
.LASF229:
	.ascii	"__FLT64_IS_IEC_60559__ 2\000"
.LASF546:
	.ascii	"CFG_RESERVED_VASPACE_SIZE (1024 * 1024 * 10)\000"
.LASF1159:
	.ascii	"TEE_PANIC_ID_TEE_OPENTASESSION 0x00000403\000"
.LASF929:
	.ascii	"TEE_LOGIN_PUBLIC 0x00000000\000"
.LASF1583:
	.ascii	"__KERNEL_THREAD_ARCH_H \000"
.LASF294:
	.ascii	"__USACCUM_MAX__ 0XFFFFP-8UHK\000"
.LASF515:
	.ascii	"CFG_ENABLE_EMBEDDED_TESTS 1\000"
.LASF1691:
	.ascii	"CPSR_A ARM32_CPSR_A\000"
.LASF100:
	.ascii	"__INT8_MAX__ 0x7f\000"
.LASF1413:
	.ascii	"SIMPLEQ_INSERT_HEAD(head,elm,field) do { if (((elm)"
	.ascii	"->field.sqe_next = (head)->sqh_first) == NULL) (hea"
	.ascii	"d)->sqh_last = &(elm)->field.sqe_next; (head)->sqh_"
	.ascii	"first = (elm); } while ( 0)\000"
.LASF1275:
	.ascii	"TEE_MEMREF_1_USED 0x00000002\000"
.LASF870:
	.ascii	"_WCHAR_T_ \000"
.LASF1589:
	.ascii	"MIDR_IMPLEMENTER_WIDTH U(8)\000"
.LASF735:
	.ascii	"INT8_MIN (-0x7f-1)\000"
.LASF1614:
	.ascii	"CORTEX_A78AE_PART_NUM U(0xD42)\000"
.LASF1480:
	.ascii	"nex_malloc(size) malloc(size)\000"
.LASF1850:
	.ascii	"TEE_ATTR_PBKDF2_SALT 0xD00002C2\000"
.LASF2136:
	.ascii	"TRUSTED_DRAM_BASE TZDRAM_BASE\000"
.LASF603:
	.ascii	"CFG_TA_FLOAT_SUPPORT 1\000"
.LASF1091:
	.ascii	"TEE_ATTR_RSA_PRIVATE_EXPONENT 0xC0000330\000"
.LASF295:
	.ascii	"__USACCUM_EPSILON__ 0x1P-8UHK\000"
.LASF1450:
	.ascii	"QUEUEDEBUG_CIRCLEQ_POSTREMOVE(elm,field) \000"
.LASF2165:
	.ascii	"register_ddr(addr,size) __register_memory(#addr, ME"
	.ascii	"M_AREA_DDR_OVERALL, (addr), (size), phys_ddr_overal"
	.ascii	"l)\000"
.LASF1496:
	.ascii	"SIZE_8M UINTPTR_C(0x800000)\000"
.LASF691:
	.ascii	"__DECLARE_KEEP_PAGER2(sym,file_id) extern const uns"
	.ascii	"igned long ____keep_pager_ ##sym; const unsigned lo"
	.ascii	"ng ____keep_pager_ ##sym ##_ ##file_id __section(\""
	.ascii	"__keep_meta_vars_pager\") = (unsigned long)&(sym)\000"
.LASF1530:
	.ascii	"TEE_MATTR_TABLE BIT(3)\000"
.LASF848:
	.ascii	"_SYS_SIZE_T_H \000"
.LASF93:
	.ascii	"__INTMAX_C(c) c ## LL\000"
.LASF1321:
	.ascii	"__define_initcall(type,lvl,fn) SCATTERED_ARRAY_DEFI"
	.ascii	"NE_PG_ITEM_ORDERED(type ## call, lvl, struct initca"
	.ascii	"ll) = { .func = (fn), }\000"
.LASF137:
	.ascii	"__UINT_FAST16_MAX__ 0xffffffffU\000"
.LASF1447:
	.ascii	"TAILQ_PREV(elm,headname,field) (*(((struct headname"
	.ascii	" *)((elm)->field.tqe_prev))->tqh_last))\000"
.LASF68:
	.ascii	"__UINT_FAST64_TYPE__ long long unsigned int\000"
.LASF1317:
	.ascii	"IMSG_RAW(...) trace_printf_helper_raw(TRACE_INFO, t"
	.ascii	"rue, __VA_ARGS__)\000"
.LASF1356:
	.ascii	"static_assert _Static_assert\000"
.LASF272:
	.ascii	"__ULFRACT_IBIT__ 0\000"
.LASF1335:
	.ascii	"driver_init(fn) __define_initcall(init, 5, fn)\000"
.LASF571:
	.ascii	"CFG_STM32MP_PROVISIONING 1\000"
.LASF718:
	.ascii	"LONG_MAX __LONG_MAX__\000"
.LASF1391:
	.ascii	"SLIST_FIRST(head) ((head)->slh_first)\000"
.LASF912:
	.ascii	"TEE_ERROR_SECURITY 0xFFFF000F\000"
.LASF768:
	.ascii	"UINT_FAST16_MAX UINT16_MAX\000"
.LASF265:
	.ascii	"__UFRACT_EPSILON__ 0x1P-16UR\000"
.LASF818:
	.ascii	"PRIx32 \"x\"\000"
.LASF1343:
	.ascii	"PRIxVA PRIxPTR\000"
.LASF558:
	.ascii	"CFG_SM_NO_CYCLE_COUNTING 1\000"
.LASF1239:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINITFMMCONTEXT 0x00001603\000"
.LASF82:
	.ascii	"__SIZE_MAX__ 0xffffffffU\000"
.LASF1726:
	.ascii	"SCTLR_TE BIT32(30)\000"
.LASF1415:
	.ascii	"SIMPLEQ_INSERT_AFTER(head,listelm,elm,field) do { i"
	.ascii	"f (((elm)->field.sqe_next = (listelm)->field.sqe_ne"
	.ascii	"xt) == NULL) (head)->sqh_last = &(elm)->field.sqe_n"
	.ascii	"ext; (listelm)->field.sqe_next = (elm); } while ( 0"
	.ascii	")\000"
.LASF2111:
	.ascii	"STM32MP135F_PART_NB U(0x05010800)\000"
.LASF587:
	.ascii	"CFG_STM32_RNG 1\000"
.LASF1598:
	.ascii	"CORTEX_A5_PART_NUM U(0xC05)\000"
.LASF543:
	.ascii	"CFG_PSCI_ARM32 1\000"
.LASF906:
	.ascii	"TEE_ERROR_NOT_IMPLEMENTED 0xFFFF0009\000"
.LASF498:
	.ascii	"CFG_CRYPTO_SIZE_OPTIMIZATION 1\000"
.LASF1038:
	.ascii	"TEE_ALG_HMAC_SHA512 0x30000006\000"
.LASF821:
	.ascii	"PRIX8 \"X\"\000"
.LASF2154:
	.ascii	"CORE_MMU_USER_CODE_MASK ((paddr_t)CORE_MMU_USER_COD"
	.ascii	"E_SIZE - 1)\000"
.LASF1443:
	.ascii	"TAILQ_EMPTY(head) ((head)->tqh_first == NULL)\000"
.LASF1697:
	.ascii	"SCR_NS BIT32(0)\000"
.LASF2126:
	.ascii	"STM32MP153D_PART_NB U(0x050000A5)\000"
.LASF331:
	.ascii	"__SQ_IBIT__ 0\000"
.LASF1659:
	.ascii	"ARM32_CPSR_MODE_ABT U(0x17)\000"
.LASF405:
	.ascii	"__ARM_FEATURE_SIMD32 1\000"
.LASF884:
	.ascii	"__need_NULL\000"
.LASF885:
	.ascii	"offsetof(TYPE,MEMBER) __builtin_offsetof (TYPE, MEM"
	.ascii	"BER)\000"
.LASF889:
	.ascii	"TEE_HANDLE_NULL 0\000"
.LASF339:
	.ascii	"__UHQ_IBIT__ 0\000"
.LASF1094:
	.ascii	"TEE_ATTR_RSA_EXPONENT1 0xC0000630\000"
.LASF556:
	.ascii	"CFG_SECONDARY_INIT_CNTFRQ 1\000"
.LASF2000:
	.ascii	"GPIOA_BASE 0x50002000\000"
.LASF1063:
	.ascii	"TEE_TYPE_HMAC_SHA384 0xA0000005\000"
.LASF1534:
	.ascii	"TEE_MATTR_PRW (TEE_MATTR_PR | TEE_MATTR_PW)\000"
.LASF1556:
	.ascii	"VM_FLAG_LDELF BIT(3)\000"
.LASF1509:
	.ascii	"IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & (~(x) + 1"
	.ascii	")) == (x)))\000"
.LASF801:
	.ascii	"PRIi8 \"i\"\000"
.LASF2087:
	.ascii	"USART1_BASE UART1_BASE\000"
.LASF2144:
	.ascii	"CORE_MMU_BASE_TABLE_LEVEL U(1)\000"
.LASF1709:
	.ascii	"SCTLR_C BIT32(2)\000"
.LASF248:
	.ascii	"__SFRACT_MIN__ (-0.5HR-0.5HR)\000"
.LASF417:
	.ascii	"__thumb2__ 1\000"
.LASF366:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1\000"
.LASF306:
	.ascii	"__LACCUM_FBIT__ 31\000"
.LASF1961:
	.ascii	"TZDRAM_SIZE CFG_TZDRAM_SIZE\000"
.LASF983:
	.ascii	"TEE_ALG_AES_CMAC 0x30000610\000"
.LASF1959:
	.ascii	"TEE_RAM_VA_SIZE CORE_MMU_PGDIR_SIZE\000"
.LASF313:
	.ascii	"__ULACCUM_MIN__ 0.0ULK\000"
.LASF1588:
	.ascii	"MIDR_IMPLEMENTER_SHIFT U(24)\000"
.LASF974:
	.ascii	"TEE_OPERATION_STATE_INITIAL 0x00000000\000"
.LASF696:
	.ascii	"DECLARE_KEEP_INIT(sym) __DECLARE_KEEP_INIT1(sym, __"
	.ascii	"FILE_ID__)\000"
.LASF1265:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTFROMFMM 0x00001C02\000"
.LASF51:
	.ascii	"__UINT32_TYPE__ unsigned int\000"
.LASF751:
	.ascii	"INT_LEAST8_MAX INT8_MAX\000"
.LASF2202:
	.ascii	"__defines\000"
.LASF773:
	.ascii	"UINTPTR_MAX ULONG_MAX\000"
.LASF1071:
	.ascii	"TEE_TYPE_ECDSA_PUBLIC_KEY 0xA0000041\000"
.LASF480:
	.ascii	"CFG_CORE_HAS_GENERIC_TIMER 1\000"
.LASF508:
	.ascii	"CFG_DRIVERS_NVMEM 1\000"
.LASF2091:
	.ascii	"DBGMCU_IDC U(0x0)\000"
.LASF75:
	.ascii	"__LONG_MAX__ 0x7fffffffL\000"
.LASF935:
	.ascii	"TEE_LOGIN_TRUSTED_APP 0xF0000000\000"
.LASF1475:
	.ascii	"__MALLOC_H \000"
.LASF307:
	.ascii	"__LACCUM_IBIT__ 32\000"
.LASF2007:
	.ascii	"GPIOH_BASE 0x50009000\000"
.LASF222:
	.ascii	"__FLT64_NORM_MAX__ 1.7976931348623157e+308F64\000"
.LASF1904:
	.ascii	"TEE_INTERNAL_HASH_TO_ALGO(algo) TEE_ALG_HASH_ALGO(T"
	.ascii	"EE_ALG_GET_INTERNAL_HASH(algo))\000"
.LASF1638:
	.ascii	"CLIDR_LOC_SHIFT U(24)\000"
.LASF1560:
	.ascii	"TA_FLAG_USER_MODE 0\000"
.LASF1846:
	.ascii	"TEE_ATTR_CONCAT_KDF_DKM_LENGTH 0xF00003C1\000"
.LASF770:
	.ascii	"UINT_FAST64_MAX UINT64_MAX\000"
.LASF245:
	.ascii	"__FLT32X_IS_IEC_60559__ 2\000"
.LASF1497:
	.ascii	"SIZE_2G UINTPTR_C(0x80000000)\000"
.LASF2128:
	.ascii	"STM32MP151D_PART_NB U(0x050000AF)\000"
.LASF485:
	.ascii	"CFG_CORE_NEX_HEAP_SIZE 16384\000"
.LASF1440:
	.ascii	"TAILQ_FOREACH_REVERSE(var,head,headname,field) for "
	.ascii	"((var) = (*(((struct headname *)((head)->tqh_last))"
	.ascii	"->tqh_last)); (var); (var) = (*(((struct headname *"
	.ascii	")((var)->field.tqe_prev))->tqh_last)))\000"
.LASF1611:
	.ascii	"CORTEX_A76AE_PART_NUM U(0xD0E)\000"
.LASF483:
	.ascii	"CFG_CORE_MAX_SYSCALL_RECURSION 4\000"
.LASF927:
	.ascii	"TEE_PARAM_TYPE_MEMREF_OUTPUT 6\000"
.LASF2191:
	.ascii	"short unsigned int\000"
.LASF2187:
	.ascii	"signed char\000"
.LASF1852:
	.ascii	"TEE_ATTR_PBKDF2_DKM_LENGTH 0xF00004C2\000"
.LASF2186:
	.ascii	"SM_PM_H \000"
.LASF1360:
	.ascii	"__ATOMIC_H \000"
.LASF1541:
	.ascii	"TEE_MATTR_URX (TEE_MATTR_UR | TEE_MATTR_UX)\000"
.LASF1419:
	.ascii	"SIMPLEQ_EMPTY(head) ((head)->sqh_first == NULL)\000"
.LASF2160:
	.ascii	"register_phys_mem(type,addr,size) __register_memory"
	.ascii	"(#addr, (type), (addr), (size), phys_mem_map)\000"
.LASF845:
	.ascii	"__size_t__ \000"
.LASF1698:
	.ascii	"SCR_IRQ BIT32(1)\000"
.LASF763:
	.ascii	"INT_FAST8_MAX INT8_MAX\000"
.LASF2112:
	.ascii	"STM32MP135D_PART_NB U(0x05010801)\000"
.LASF1549:
	.ascii	"TEE_MATTR_MEM_TYPE_CACHED U(1)\000"
.LASF1348:
	.ascii	"PRIxPA_WIDTH ((int)(sizeof(paddr_t) * 2))\000"
.LASF966:
	.ascii	"TEE_HANDLE_FLAG_EXPECT_TWO_KEYS 0x00080000\000"
.LASF1227:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICVERIFYDIGEST 0x00001104\000"
.LASF325:
	.ascii	"__ULLACCUM_EPSILON__ 0x1P-32ULLK\000"
.LASF1791:
	.ascii	"CNTCR_EN BIT(0)\000"
.LASF26:
	.ascii	"__CHAR_BIT__ 8\000"
.LASF993:
	.ascii	"TEE_ALG_DES3_CBC_MAC_PKCS5 0x30000513\000"
.LASF1055:
	.ascii	"TEE_TYPE_AES 0xA0000010\000"
.LASF1274:
	.ascii	"TEE_MEMREF_0_USED 0x00000001\000"
.LASF1362:
	.ascii	"_SYS_QUEUE_H_ \000"
.LASF2034:
	.ascii	"TIM3_BASE 0x40001000\000"
.LASF1503:
	.ascii	"ROUNDUP(v,size) (((v) + ((__typeof__(v))(size) - 1)"
	.ascii	") & ~((__typeof__(v))(size) - 1))\000"
.LASF1996:
	.ascii	"ETH2_BASE 0x5800E000\000"
.LASF675:
	.ascii	"__nostackcheck __attribute__((no_instrument_functio"
	.ascii	"n))\000"
.LASF1622:
	.ascii	"NEOVERSE_V1_PART_NUM U(0xD40)\000"
.LASF1839:
	.ascii	"TEE_ALG_CONCAT_KDF_SHA224_DERIVE_KEY 0x800030C1\000"
.LASF396:
	.ascii	"__ARM_FEATURE_DOTPROD\000"
.LASF2117:
	.ascii	"STM32MP157C_PART_NB U(0x05000000)\000"
.LASF1057:
	.ascii	"TEE_TYPE_DES3 0xA0000013\000"
.LASF604:
	.ascii	"CFG_TA_MBEDTLS 1\000"
.LASF853:
	.ascii	"_BSD_SIZE_T_ \000"
.LASF1868:
	.ascii	"TEE_MAIN_ALGO_AES 0x10\000"
.LASF2093:
	.ascii	"DBGMCU_IDC_REV_ID_MASK GENMASK_32(31, 16)\000"
.LASF1393:
	.ascii	"STAILQ_HEAD(name,type) struct name { struct type *s"
	.ascii	"tqh_first; struct type **stqh_last; }\000"
.LASF1829:
	.ascii	"TEE_ALG_HKDF_SHA224_DERIVE_KEY 0x800030C0\000"
.LASF970:
	.ascii	"TEE_OPERATION_DIGEST 5\000"
.LASF945:
	.ascii	"TEE_MEMORY_ACCESS_ANY_OWNER 0x00000004\000"
.LASF87:
	.ascii	"__LONG_LONG_WIDTH__ 64\000"
.LASF223:
	.ascii	"__FLT64_MIN__ 2.2250738585072014e-308F64\000"
.LASF140:
	.ascii	"__INTPTR_MAX__ 0x7fffffff\000"
.LASF172:
	.ascii	"__DBL_MAX__ ((double)1.7976931348623157e+308L)\000"
.LASF995:
	.ascii	"TEE_ALG_SM4_CBC_NOPAD 0x10000114\000"
.LASF1132:
	.ascii	"TEE_ECC_CURVE_NIST_P192 0x00000001\000"
.LASF1454:
	.ascii	"CIRCLEQ_INIT(head) do { (head)->cqh_first = (void *"
	.ascii	")(head); (head)->cqh_last = (void *)(head); } while"
	.ascii	" ( 0)\000"
.LASF652:
	.ascii	"__noreturn __attribute__((__noreturn__))\000"
.LASF382:
	.ascii	"__GCC_ATOMIC_POINTER_LOCK_FREE 2\000"
.LASF1811:
	.ascii	"SM_EXIT_TO_NON_SECURE 0\000"
.LASF1102:
	.ascii	"TEE_ATTR_DH_PRIME 0xD0001032\000"
.LASF1636:
	.ascii	"MPIDR_AARCH32_AFF_MASK (MPIDR_AFF0_MASK | MPIDR_AFF"
	.ascii	"1_MASK | MPIDR_AFF2_MASK)\000"
.LASF1703:
	.ascii	"SCR_NET BIT32(6)\000"
.LASF822:
	.ascii	"PRIX16 \"X\"\000"
.LASF1757:
	.ascii	"TTBCR_EPD0 BIT32(7)\000"
.LASF1506:
	.ascii	"ROUNDDOWN(v,size) ((v) & ~((__typeof__(v))(size) - "
	.ascii	"1))\000"
.LASF1821:
	.ascii	"__KERNEL_USER_MODE_CTX_STRUCT_H \000"
.LASF668:
	.ascii	"__rodata __section(\".rodata\" __SECTION_FLAGS_RODA"
	.ascii	"TA)\000"
.LASF955:
	.ascii	"TEE_OBJECT_ID_MAX_LEN 64\000"
.LASF428:
	.ascii	"__ARM_FEATURE_FP16_SCALAR_ARITHMETIC\000"
.LASF203:
	.ascii	"__FLT32_MAX_10_EXP__ 38\000"
.LASF1935:
	.ascii	"TEE_TIME_SUB(t1,t2,dst) do { (dst).seconds = (t1).s"
	.ascii	"econds - (t2).seconds; if ((t1).millis < (t2).milli"
	.ascii	"s) { (dst).seconds--; (dst).millis = (t1).millis + "
	.ascii	"TEE_TIME_MILLIS_BASE - (t2).millis; } else { (dst)."
	.ascii	"millis = (t1).millis - (t2).millis; } } while (0)\000"
.LASF476:
	.ascii	"CFG_CORE_ASYNC_NOTIF_GIC_INTID 0\000"
.LASF1187:
	.ascii	"TEE_PANIC_ID_TEE_COPYOBJECTATTRIBUTES1 0x00000809\000"
.LASF951:
	.ascii	"TEE_DATA_FLAG_SHARE_READ 0x00000010\000"
.LASF1148:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASBOOL 0x00000205\000"
.LASF1724:
	.ascii	"SCTLR_TRE BIT32(28)\000"
.LASF782:
	.ascii	"LL(v) v ## LL\000"
.LASF1561:
	.ascii	"TA_FLAG_EXEC_DDR 0\000"
.LASF1544:
	.ascii	"TEE_MATTR_GLOBAL BIT(10)\000"
.LASF522:
	.ascii	"CFG_HWRNG_QUALITY 1024\000"
.LASF89:
	.ascii	"__WINT_WIDTH__ 32\000"
.LASF1661:
	.ascii	"ARM32_CPSR_MODE_SYS U(0x1f)\000"
.LASF744:
	.ascii	"UINT16_MAX 0xffff\000"
.LASF562:
	.ascii	"CFG_STACK_TMP_EXTRA 0\000"
.LASF194:
	.ascii	"__LDBL_HAS_DENORM__ 1\000"
.LASF833:
	.ascii	"_ANSI_STDDEF_H \000"
.LASF780:
	.ascii	"ULL(v) v ## ULL\000"
.LASF1877:
	.ascii	"TEE_MAIN_ALGO_ECDH 0x42\000"
.LASF1234:
	.ascii	"TEE_PANIC_ID_TEE_WAIT 0x00001405\000"
.LASF961:
	.ascii	"TEE_USAGE_VERIFY 0x00000020\000"
.LASF1338:
	.ascii	"boot_final(fn) __define_initcall(final, 1, fn)\000"
.LASF425:
	.ascii	"__ARM_FP16_FORMAT_IEEE\000"
.LASF1658:
	.ascii	"ARM32_CPSR_MODE_MON U(0x16)\000"
.LASF621:
	.ascii	"CFG_WARN_DECL_AFTER_STATEMENT 1\000"
.LASF2143:
	.ascii	"CORE_MMU_BASE_TABLE_SHIFT U(30)\000"
.LASF1890:
	.ascii	"TEE_CHAIN_MODE_XTS 0x4\000"
.LASF1832:
	.ascii	"TEE_ALG_HKDF_SHA512_DERIVE_KEY 0x800060C0\000"
.LASF1923:
	.ascii	"TEE_U32_TO_LITTLE_ENDIAN(x) ((uint32_t)(x))\000"
.LASF1721:
	.ascii	"SCTLR_VE BIT32(24)\000"
.LASF376:
	.ascii	"__GCC_ATOMIC_INT_LOCK_FREE 2\000"
.LASF125:
	.ascii	"__UINT32_C(c) c ## U\000"
.LASF218:
	.ascii	"__FLT64_MAX_EXP__ 1024\000"
.LASF1030:
	.ascii	"TEE_ALG_SHA3_384 0x5000000A\000"
.LASF677:
	.ascii	"__compiler_bswap32(x) __builtin_bswap32((x))\000"
.LASF362:
	.ascii	"__REGISTER_PREFIX__ \000"
.LASF1135:
	.ascii	"TEE_ECC_CURVE_NIST_P384 0x00000004\000"
.LASF1243:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTTOS32 0x00001704\000"
.LASF1522:
	.ascii	"BIT(nr) BIT32(nr)\000"
.LASF254:
	.ascii	"__USFRACT_MAX__ 0XFFP-8UHR\000"
.LASF1456:
	.ascii	"CIRCLEQ_INSERT_BEFORE(head,listelm,elm,field) do { "
	.ascii	"QUEUEDEBUG_CIRCLEQ_HEAD((head), field) QUEUEDEBUG_C"
	.ascii	"IRCLEQ_ELM((head), (listelm), field) (elm)->field.c"
	.ascii	"qe_next = (listelm); (elm)->field.cqe_prev = (liste"
	.ascii	"lm)->field.cqe_prev; if ((listelm)->field.cqe_prev "
	.ascii	"== (void *)(head)) (head)->cqh_first = (elm); else "
	.ascii	"(listelm)->field.cqe_prev->field.cqe_next = (elm); "
	.ascii	"(listelm)->field.cqe_prev = (elm); } while ( 0)\000"
.LASF1481:
	.ascii	"nex_calloc(nmemb,size) calloc(nmemb, size)\000"
.LASF490:
	.ascii	"CFG_CORE_WORKAROUND_NSITR_CACHE_PRIME 1\000"
.LASF2097:
	.ascii	"BKPSRAM_PM_SIZE (BKPSRAM_PM_MAILBOX_SIZE + BKPSRAM_"
	.ascii	"PM_CONTEXT_SIZE)\000"
.LASF1763:
	.ascii	"TTBCR_EPD1 BIT32(23)\000"
.LASF940:
	.ascii	"TEE_PROPSET_TEE_IMPLEMENTATION (TEE_PropSetHandle)0"
	.ascii	"xFFFFFFFD\000"
.LASF1061:
	.ascii	"TEE_TYPE_HMAC_SHA224 0xA0000003\000"
.LASF632:
	.ascii	"CFG_WITH_VFP 1\000"
.LASF205:
	.ascii	"__FLT32_MAX__ 3.4028234663852886e+38F32\000"
.LASF972:
	.ascii	"TEE_OPERATION_ASYMMETRIC_SIGNATURE 7\000"
.LASF2073:
	.ascii	"GIC_NON_SEC_SGI_0 0\000"
.LASF719:
	.ascii	"LONG_MIN (-LONG_MAX - 1L)\000"
.LASF1710:
	.ascii	"SCTLR_CP15BEN BIT32(5)\000"
.LASF1104:
	.ascii	"TEE_ATTR_DH_BASE 0xD0001232\000"
.LASF1272:
	.ascii	"TEE_MEM_INPUT 0x00000001\000"
.LASF2123:
	.ascii	"STM32MP157F_PART_NB U(0x05000080)\000"
.LASF1955:
	.ascii	"Z_IS_ENABLED2(one_or_two_args) Z_IS_ENABLED3(one_or"
	.ascii	"_two_args true, false)\000"
.LASF1125:
	.ascii	"TEE_ATTR_X25519_PRIVATE_VALUE 0xC0000A44\000"
.LASF1743:
	.ascii	"DACR_DOMAIN(num,perm) SHIFT_U32((perm), ((num) * 2)"
	.ascii	")\000"
.LASF727:
	.ascii	"UCHAR_MAX (SCHAR_MAX * 2 + 1)\000"
.LASF1643:
	.ascii	"CTR_ERG_SHIFT U(20)\000"
.LASF1197:
	.ascii	"TEE_PANIC_ID_TEE_STARTPERSISTENTOBJECTENUMERATOR 0x"
	.ascii	"00000A05\000"
.LASF340:
	.ascii	"__USQ_FBIT__ 32\000"
.LASF454:
	.ascii	"__linux__ 1\000"
.LASF1566:
	.ascii	"TA_FLAG_REMAP_SUPPORT 0\000"
.LASF2031:
	.ascii	"TAMP_BASE 0x5c00a000\000"
.LASF1420:
	.ascii	"SIMPLEQ_FIRST(head) ((head)->sqh_first)\000"
.LASF838:
	.ascii	"_PTRDIFF_T_ \000"
.LASF448:
	.ascii	"__ARM_FEATURE_BF16_SCALAR_ARITHMETIC\000"
.LASF1009:
	.ascii	"TEE_ALG_RSAES_PKCS1_V1_5 0x60000130\000"
.LASF1607:
	.ascii	"CORTEX_A75_PART_NUM U(0xD0A)\000"
.LASF2026:
	.ascii	"RTC_BASE 0x5c004000\000"
.LASF460:
	.ascii	"__KERNEL__ 1\000"
.LASF1484:
	.ascii	"RAND_MAX __INT_MAX__\000"
.LASF1212:
	.ascii	"TEE_PANIC_ID_TEE_CIPHERDOFINAL 0x00000E01\000"
.LASF618:
	.ascii	"CFG_TZC400 1\000"
.LASF1813:
	.ascii	"STACK_TMP_OFFS SM_STACK_TMP_RESERVE_SIZE\000"
.LASF4:
	.ascii	"__STDC_HOSTED__ 1\000"
.LASF899:
	.ascii	"TEE_ERROR_CANCEL 0xFFFF0002\000"
.LASF95:
	.ascii	"__UINTMAX_C(c) c ## ULL\000"
.LASF1851:
	.ascii	"TEE_ATTR_PBKDF2_ITERATION_COUNT 0xF00003C2\000"
.LASF1641:
	.ascii	"CTR_CWG_SHIFT U(24)\000"
.LASF925:
	.ascii	"TEE_PARAM_TYPE_VALUE_INOUT 3\000"
.LASF1347:
	.ascii	"PRIxVA_WIDTH ((int)(sizeof(vaddr_t) * 2))\000"
.LASF2020:
	.ascii	"OTG_BASE 0x49000000\000"
.LASF1706:
	.ascii	"SCR_SIF BIT32(9)\000"
.LASF711:
	.ascii	"__INTTYPES_H \000"
.LASF304:
	.ascii	"__UACCUM_MAX__ 0XFFFFFFFFP-16UK\000"
.LASF1732:
	.ascii	"NSACR_CP11 BIT32(11)\000"
.LASF584:
	.ascii	"CFG_STM32_LPTIMER 1\000"
.LASF114:
	.ascii	"__INT_LEAST32_MAX__ 0x7fffffff\000"
.LASF1306:
	.ascii	"EMSG(...) trace_printf_helper(TRACE_ERROR, true, __"
	.ascii	"VA_ARGS__)\000"
.LASF1915:
	.ascii	"TEE_ALG_GET_DIGEST_SIZE(algo) __tee_alg_get_digest_"
	.ascii	"size(algo)\000"
.LASF2100:
	.ascii	"BKPSRAM_PM_CONTEXT_OFFSET (BKPSRAM_PM_MAILBOX_OFFSE"
	.ascii	"T + BKPSRAM_PM_MAILBOX_SIZE)\000"
.LASF806:
	.ascii	"PRIo8 \"o\"\000"
.LASF1411:
	.ascii	"SIMPLEQ_ENTRY(type) struct { struct type *sqe_next;"
	.ascii	" }\000"
.LASF1517:
	.ascii	"MEMBER_SIZE(type,member) sizeof(((type *)0)->member"
	.ascii	")\000"
.LASF1233:
	.ascii	"TEE_PANIC_ID_TEE_SETTAPERSISTENTTIME 0x00001404\000"
.LASF173:
	.ascii	"__DBL_NORM_MAX__ ((double)1.7976931348623157e+308L)"
	.ascii	"\000"
.LASF1226:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICSIGNDIGEST 0x00001103\000"
.LASF177:
	.ascii	"__DBL_HAS_DENORM__ 1\000"
.LASF748:
	.ascii	"INT_LEAST16_MIN INT16_MIN\000"
.LASF431:
	.ascii	"__ARM_FEATURE_FMA\000"
.LASF106:
	.ascii	"__UINT32_MAX__ 0xffffffffU\000"
.LASF219:
	.ascii	"__FLT64_MAX_10_EXP__ 308\000"
.LASF704:
	.ascii	"SCATTERED_ARRAY_DEFINE_PG_ITEM_ORDERED(array_name,o"
	.ascii	"rder,element_type) __SCT_ARRAY_DEF_PG_ITEM1(array_n"
	.ascii	"ame, order, __COUNTER__, element_type)\000"
.LASF1147:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASBINARYBLOCK 0x0000020"
	.ascii	"4\000"
.LASF1681:
	.ascii	"CPSR_MODE_IRQ ARM32_CPSR_MODE_IRQ\000"
.LASF1395:
	.ascii	"STAILQ_ENTRY(type) struct { struct type *stqe_next;"
	.ascii	" }\000"
.LASF1882:
	.ascii	"TEE_MAIN_ALGO_HKDF 0xC0\000"
.LASF1500:
	.ascii	"MAX_UNSAFE(a,b) (((a) > (b)) ? (a) : (b))\000"
.LASF37:
	.ascii	"__PTRDIFF_TYPE__ int\000"
.LASF1093:
	.ascii	"TEE_ATTR_RSA_PRIME2 0xC0000530\000"
.LASF372:
	.ascii	"__GCC_ATOMIC_CHAR16_T_LOCK_FREE 2\000"
.LASF1668:
	.ascii	"ARM32_CPSR_FIA (ARM32_CPSR_F | ARM32_CPSR_I | ARM32"
	.ascii	"_CPSR_A)\000"
.LASF1845:
	.ascii	"TEE_ATTR_CONCAT_KDF_OTHER_INFO 0xD00002C1\000"
.LASF1252:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTNEG 0x00001904\000"
.LASF364:
	.ascii	"__GNUC_STDC_INLINE__ 1\000"
.LASF803:
	.ascii	"PRIi32 \"i\"\000"
.LASF1559:
	.ascii	"USER_TA_HEADER_H \000"
.LASF424:
	.ascii	"__ARM_FP\000"
.LASF1460:
	.ascii	"CIRCLEQ_FOREACH(var,head,field) for ((var) = ((head"
	.ascii	")->cqh_first); (var) != (const void *)(head); (var)"
	.ascii	" = ((var)->field.cqe_next))\000"
.LASF1034:
	.ascii	"TEE_ALG_HMAC_SHA1 0x30000002\000"
.LASF1831:
	.ascii	"TEE_ALG_HKDF_SHA384_DERIVE_KEY 0x800050C0\000"
.LASF900:
	.ascii	"TEE_ERROR_ACCESS_CONFLICT 0xFFFF0003\000"
.LASF2078:
	.ascii	"TARGET_CPU1_GIC_MASK BIT(1)\000"
.LASF1155:
	.ascii	"TEE_PANIC_ID_TEE_STARTPROPERTYENUMERATOR 0x0000020C"
	.ascii	"\000"
.LASF1196:
	.ascii	"TEE_PANIC_ID_TEE_RESETPERSISTENTOBJECTENUMERATOR 0x"
	.ascii	"00000A04\000"
.LASF88:
	.ascii	"__WCHAR_WIDTH__ 32\000"
.LASF1350:
	.ascii	"MM_PGT_CACHE_H \000"
.LASF1545:
	.ascii	"TEE_MATTR_SECURE BIT(11)\000"
.LASF344:
	.ascii	"__UTQ_FBIT__ 128\000"
.LASF1870:
	.ascii	"TEE_MAIN_ALGO_DES2 0x12\000"
.LASF1353:
	.ascii	"__ASSERT_H \000"
.LASF837:
	.ascii	"__PTRDIFF_T \000"
.LASF1296:
	.ascii	"TRACE_ERROR 1\000"
.LASF1051:
	.ascii	"TEE_ALG_SM2_PKE 0x80000045\000"
.LASF657:
	.ascii	"__attr_const __attribute__((__const__))\000"
.LASF732:
	.ascii	"__STDINT_H \000"
.LASF1510:
	.ascii	"IS_ALIGNED(x,a) (((x) & ((a) - 1)) == 0)\000"
.LASF1783:
	.ascii	"IDPFR1_VIRT_MASK SHIFT_U32(0xF, IDPFR1_VIRT_SHIFT)\000"
.LASF814:
	.ascii	"PRIu64 __PRI64_PREFIX \"u\"\000"
.LASF392:
	.ascii	"__ARM_FEATURE_CRYPTO\000"
.LASF1857:
	.ascii	"TEE_STORAGE_PRIVATE_SQL_RESERVED 0x80000200\000"
.LASF764:
	.ascii	"INT_FAST16_MAX INT16_MAX\000"
.LASF1433:
	.ascii	"TAILQ_INSERT_HEAD(head,elm,field) do { QUEUEDEBUG_T"
	.ascii	"AILQ_INSERT_HEAD((head), (elm), field) if (((elm)->"
	.ascii	"field.tqe_next = (head)->tqh_first) != NULL) (head)"
	.ascii	"->tqh_first->field.tqe_prev = &(elm)->field.tqe_nex"
	.ascii	"t; else (head)->tqh_last = &(elm)->field.tqe_next; "
	.ascii	"(head)->tqh_first = (elm); (elm)->field.tqe_prev = "
	.ascii	"&(head)->tqh_first; } while ( 0)\000"
.LASF277:
	.ascii	"__LLFRACT_IBIT__ 0\000"
.LASF210:
	.ascii	"__FLT32_HAS_DENORM__ 1\000"
.LASF1930:
	.ascii	"TEE_U16_TO_BIG_ENDIAN(x) TEE_U16_BSWAP(x)\000"
.LASF1740:
	.ascii	"CPACR_CP_ACCESS_DENIED U(0x0)\000"
.LASF1390:
	.ascii	"SLIST_EMPTY(head) ((head)->slh_first == NULL)\000"
.LASF1013:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA384 0x60510230\000"
.LASF1797:
	.ascii	"THREAD_EXCP_NATIVE_INTR (ARM32_CPSR_F >> ARM32_CPSR"
	.ascii	"_F_SHIFT)\000"
.LASF1398:
	.ascii	"STAILQ_INSERT_TAIL(head,elm,field) do { (elm)->fiel"
	.ascii	"d.stqe_next = NULL; *(head)->stqh_last = (elm); (he"
	.ascii	"ad)->stqh_last = &(elm)->field.stqe_next; } while ("
	.ascii	" 0)\000"
.LASF336:
	.ascii	"__UQQ_FBIT__ 8\000"
.LASF1005:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA224 0x70313930\000"
.LASF286:
	.ascii	"__SACCUM_FBIT__ 7\000"
.LASF1268:
	.ascii	"TEE_PARAM_TYPE_GET(t,i) ((((uint32_t)t) >> ((i)*4))"
	.ascii	" & 0xF)\000"
.LASF1906:
	.ascii	"TEE_ALG_HMAC_ALGO(main_hash) (TEE_OPERATION_MAC << "
	.ascii	"28 | (main_hash))\000"
.LASF317:
	.ascii	"__LLACCUM_IBIT__ 32\000"
.LASF1895:
	.ascii	"TEE_CHAIN_MODE_PKCS1_PSS_MGF1 0x9\000"
.LASF297:
	.ascii	"__ACCUM_IBIT__ 16\000"
.LASF1969:
	.ascii	"APB1_BASE 0x40000000\000"
.LASF1848:
	.ascii	"TEE_TYPE_PBKDF2_PASSWORD 0xA10000C2\000"
.LASF851:
	.ascii	"__SIZE_T \000"
.LASF151:
	.ascii	"__FLT_MIN_EXP__ (-125)\000"
.LASF276:
	.ascii	"__LLFRACT_FBIT__ 63\000"
.LASF84:
	.ascii	"__SHRT_WIDTH__ 16\000"
.LASF182:
	.ascii	"__LDBL_DIG__ 15\000"
.LASF1754:
	.ascii	"TTBCR_PD0 BIT32(4)\000"
.LASF1602:
	.ascii	"CORTEX_A15_PART_NUM U(0xC0F)\000"
.LASF1683:
	.ascii	"CPSR_MODE_MON ARM32_CPSR_MODE_MON\000"
.LASF1400:
	.ascii	"STAILQ_REMOVE_HEAD(head,field) do { if (((head)->st"
	.ascii	"qh_first = (head)->stqh_first->field.stqe_next) == "
	.ascii	"NULL) (head)->stqh_last = &(head)->stqh_first; } wh"
	.ascii	"ile ( 0)\000"
.LASF1324:
	.ascii	"initcall_begin SCATTERED_ARRAY_BEGIN(initcall, stru"
	.ascii	"ct initcall)\000"
.LASF1179:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATETRANSIENTOBJECT 0x00000801"
	.ascii	"\000"
.LASF787:
	.ascii	"INT32_C(v) v\000"
.LASF1767:
	.ascii	"TTBCR_XRGNX_NC U(0x0)\000"
.LASF1466:
	.ascii	"CIRCLEQ_PREV(elm,field) ((elm)->field.cqe_prev)\000"
.LASF1886:
	.ascii	"TEE_CHAIN_MODE_ECB_NOPAD 0x0\000"
.LASF33:
	.ascii	"__SIZEOF_POINTER__ 4\000"
.LASF1750:
	.ascii	"PAR_PA_SHIFT U(12)\000"
.LASF415:
	.ascii	"__GCC_ASM_FLAG_OUTPUTS__ 1\000"
.LASF1178:
	.ascii	"TEE_PANIC_ID_TEE_RESTRICTOBJECTUSAGE1 0x00000707\000"
.LASF328:
	.ascii	"__HQ_FBIT__ 15\000"
.LASF928:
	.ascii	"TEE_PARAM_TYPE_MEMREF_INOUT 7\000"
.LASF2017:
	.ascii	"LTDC_BASE 0x5a001000\000"
.LASF2196:
	.ascii	"long double\000"
.LASF709:
	.ascii	"SCATTERED_ARRAY_FOREACH(elem,array_name,element_typ"
	.ascii	"e) for ((elem) = SCATTERED_ARRAY_BEGIN(array_name, "
	.ascii	"element_type); (elem) < SCATTERED_ARRAY_END(array_n"
	.ascii	"ame, element_type); (elem)++)\000"
.LASF41:
	.ascii	"__UINTMAX_TYPE__ long long unsigned int\000"
.LASF850:
	.ascii	"_T_SIZE \000"
.LASF1119:
	.ascii	"TEE_ATTR_ECC_EPHEMERAL_PUBLIC_VALUE_X 0xD0000946\000"
.LASF1217:
	.ascii	"TEE_PANIC_ID_TEE_MACINIT 0x00000F03\000"
.LASF1117:
	.ascii	"TEE_ATTR_SM2_KEP_CONFIRMATION_IN 0xD0000746\000"
.LASF1333:
	.ascii	"service_init(fn) __define_initcall(init, 3, fn)\000"
.LASF141:
	.ascii	"__INTPTR_WIDTH__ 32\000"
.LASF812:
	.ascii	"PRIu16 \"u\"\000"
.LASF1942:
	.ascii	"UTEE_SE_READER_TEE_ONLY (1 << 1)\000"
.LASF879:
	.ascii	"__DEFINED_wchar_t \000"
.LASF1896:
	.ascii	"TEE_ALG_GET_CLASS(algo) __tee_alg_get_class(algo)\000"
.LASF1947:
	.ascii	"TEE_MM_POOL_HI_ALLOC (1u << 0)\000"
.LASF2200:
	.ascii	"core/arch/arm/kernel/asm-defines.c\000"
.LASF1629:
	.ascii	"MPIDR_AFF2_SHIFT U(16)\000"
.LASF1000:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA256 0x70004830\000"
.LASF809:
	.ascii	"PRIo64 __PRI64_PREFIX \"o\"\000"
.LASF227:
	.ascii	"__FLT64_HAS_INFINITY__ 1\000"
.LASF380:
	.ascii	"__GCC_DESTRUCTIVE_SIZE 64\000"
.LASF493:
	.ascii	"CFG_COUNTER_DRIVER 1\000"
.LASF2133:
	.ascii	"STM32MP1_CHIP_DEFAULT_VERSION U(0)\000"
.LASF687:
	.ascii	"__compiler_atomic_store(p,val) __atomic_store_n((p)"
	.ascii	", (val), __ATOMIC_RELAXED)\000"
.LASF1608:
	.ascii	"CORTEX_A65_PART_NUM U(0xD06)\000"
.LASF641:
	.ascii	"_CFG_SCMI_PTA_MSG_HEADER 1\000"
.LASF954:
	.ascii	"TEE_DATA_MAX_POSITION 0xFFFFFFFF\000"
.LASF76:
	.ascii	"__LONG_LONG_MAX__ 0x7fffffffffffffffLL\000"
.LASF1012:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA256 0x60410230\000"
.LASF1430:
	.ascii	"QUEUEDEBUG_TAILQ_PREREMOVE(head,elm,field) \000"
.LASF1027:
	.ascii	"TEE_ALG_SHA512 0x50000006\000"
.LASF867:
	.ascii	"_T_WCHAR_ \000"
.LASF1768:
	.ascii	"TTBCR_XRGNX_WB U(0x1)\000"
.LASF35:
	.ascii	"__GNUC_WIDE_EXECUTION_CHARSET_NAME \"UTF-32LE\"\000"
.LASF2066:
	.ascii	"CFG0_OTP_BIT_LENGTH U(8)\000"
.LASF2166:
	.ascii	"phys_ddr_overall_begin SCATTERED_ARRAY_BEGIN(phys_d"
	.ascii	"dr_overall, struct core_mmu_phys_mem)\000"
.LASF1358:
	.ascii	"KERNEL_MUTEX_H \000"
.LASF278:
	.ascii	"__LLFRACT_MIN__ (-0.5LLR-0.5LLR)\000"
.LASF1218:
	.ascii	"TEE_PANIC_ID_TEE_MACUPDATE 0x00000F04\000"
.LASF1677:
	.ascii	"__always_inline __attribute__((always_inline)) inli"
	.ascii	"ne\000"
.LASF564:
	.ascii	"CFG_STM32MP15_CLK 1\000"
.LASF1957:
	.ascii	"PLATFORM_CONFIG_H \000"
.LASF1593:
	.ascii	"MIDR_VARIANT_WIDTH U(4)\000"
.LASF1303:
	.ascii	"MAX_FUNC_PRINT_SIZE 32\000"
.LASF315:
	.ascii	"__ULACCUM_EPSILON__ 0x1P-32ULK\000"
.LASF1298:
	.ascii	"TRACE_DEBUG 3\000"
.LASF524:
	.ascii	"CFG_HWSUPP_MEM_PERM_PXN 1\000"
.LASF275:
	.ascii	"__ULFRACT_EPSILON__ 0x1P-32ULR\000"
.LASF1084:
	.ascii	"TEE_TYPE_CORRUPTED_OBJECT 0xA00000BE\000"
.LASF978:
	.ascii	"TEE_ALG_AES_CTR 0x10000210\000"
.LASF1837:
	.ascii	"TEE_ATTR_HKDF_OKM_LENGTH 0xF00004C0\000"
.LASF1543:
	.ascii	"TEE_MATTR_PROT_MASK (TEE_MATTR_PRWX | TEE_MATTR_URW"
	.ascii	"X | TEE_MATTR_GUARDED)\000"
.LASF1444:
	.ascii	"TAILQ_FIRST(head) ((head)->tqh_first)\000"
.LASF1128:
	.ascii	"TEE_ATTR_FLAG_VALUE (1 << 29)\000"
.LASF550:
	.ascii	"CFG_SCMI_MSG_CLOCK 1\000"
.LASF2047:
	.ascii	"TZC_BASE 0x5c006000\000"
.LASF1610:
	.ascii	"CORTEX_A76_PART_NUM U(0xD0B)\000"
.LASF1966:
	.ascii	"TEE_SDP_TEST_MEM_SIZE 0\000"
.LASF2198:
	.ascii	"char\000"
.LASF2063:
	.ascii	"STM32MP1_OTP_MAX_ID 0x5FU\000"
.LASF1525:
	.ascii	"ADD_OVERFLOW(a,b,res) __compiler_add_overflow((a), "
	.ascii	"(b), (res))\000"
.LASF224:
	.ascii	"__FLT64_EPSILON__ 2.2204460492503131e-16F64\000"
.LASF1181:
	.ascii	"TEE_PANIC_ID_TEE_FREETRANSIENTOBJECT 0x00000803\000"
.LASF1028:
	.ascii	"TEE_ALG_SHA3_224 0x50000008\000"
.LASF2159:
	.ascii	"__register_memory_ul(_name,_type,_addr,_size,_secti"
	.ascii	"on) __register_memory(_name, _type, _addr, _size, _"
	.ascii	"section)\000"
.LASF440:
	.ascii	"__ARM_ARCH_EXT_IDIV__ 1\000"
.LASF1114:
	.ascii	"TEE_ATTR_SM2_ID_INITIATOR 0xD0000446\000"
.LASF1621:
	.ascii	"NEOVERSE_N2_PART_NUM U(0xD49)\000"
.LASF1228:
	.ascii	"TEE_PANIC_ID_TEE_DERIVEKEY 0x00001201\000"
.LASF1442:
	.ascii	"TAILQ_CONCAT(head1,head2,field) do { if (!TAILQ_EMP"
	.ascii	"TY(head2)) { *(head1)->tqh_last = (head2)->tqh_firs"
	.ascii	"t; (head2)->tqh_first->field.tqe_prev = (head1)->tq"
	.ascii	"h_last; (head1)->tqh_last = (head2)->tqh_last; TAIL"
	.ascii	"Q_INIT((head2)); } } while ( 0)\000"
.LASF1133:
	.ascii	"TEE_ECC_CURVE_NIST_P224 0x00000002\000"
.LASF466:
	.ascii	"CFG_AES_GCM_TABLE_BASED 1\000"
.LASF1656:
	.ascii	"ARM32_CPSR_MODE_IRQ U(0x12)\000"
.LASF1594:
	.ascii	"MIDR_VARIANT_MASK (BIT(MIDR_VARIANT_WIDTH) - 1)\000"
.LASF793:
	.ascii	"UINTMAX_C(v) UINT64_C(v)\000"
.LASF613:
	.ascii	"CFG_TEE_FW_IMPL_VERSION FW_IMPL_UNDEF\000"
.LASF462:
	.ascii	"__ILP32__ 1\000"
.LASF321:
	.ascii	"__ULLACCUM_FBIT__ 32\000"
.LASF689:
	.ascii	"fallthrough __attribute__((__fallthrough__))\000"
.LASF196:
	.ascii	"__LDBL_HAS_QUIET_NAN__ 1\000"
.LASF389:
	.ascii	"__ARM_FEATURE_DSP 1\000"
.LASF600:
	.ascii	"CFG_TA_ASLR_MIN_OFFSET_PAGES 0\000"
.LASF1533:
	.ascii	"TEE_MATTR_PX BIT(6)\000"
.LASF77:
	.ascii	"__WCHAR_MAX__ 0xffffffffU\000"
.LASF577:
	.ascii	"CFG_STM32_ETZPC 1\000"
.LASF804:
	.ascii	"PRIi64 __PRI64_PREFIX \"i\"\000"
.LASF847:
	.ascii	"_SIZE_T \000"
.LASF1144:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATEPROPERTYENUMERATOR 0x00000"
	.ascii	"201\000"
.LASF1571:
	.ascii	"TA_FLAGS_MASK GENMASK_32(10, 0)\000"
.LASF551:
	.ascii	"CFG_SCMI_MSG_DRIVERS 1\000"
.LASF433:
	.ascii	"__ARM_NEON\000"
.LASF1319:
	.ascii	"FMSG_RAW(...) (void)0\000"
.LASF956:
	.ascii	"TEE_USAGE_EXTRACTABLE 0x00000001\000"
.LASF1001:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA384 0x70005830\000"
.LASF1260:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSUBMOD 0x00001A06\000"
.LASF911:
	.ascii	"TEE_ERROR_COMMUNICATION 0xFFFF000E\000"
.LASF673:
	.ascii	"__nex_data \000"
.LASF586:
	.ascii	"CFG_STM32_REGULATOR_GPIO 1\000"
.LASF322:
	.ascii	"__ULLACCUM_IBIT__ 32\000"
.LASF2029:
	.ascii	"SYSCFG_BASE 0x50020000\000"
.LASF1729:
	.ascii	"ACTLR_CA9_WFLZ BIT(3)\000"
.LASF533:
	.ascii	"CFG_MMAP_REGIONS 30\000"
.LASF181:
	.ascii	"__LDBL_MANT_DIG__ 53\000"
.LASF2002:
	.ascii	"GPIOC_BASE 0x50004000\000"
.LASF828:
	.ascii	"true 1\000"
.LASF2086:
	.ascii	"TAMP_BKP_REGISTER_ZONE3_COUNT U(17)\000"
.LASF676:
	.ascii	"__compiler_bswap64(x) __builtin_bswap64((x))\000"
.LASF560:
	.ascii	"CFG_SP_INIT_INFO_MAX_SIZE 0x1000\000"
.LASF184:
	.ascii	"__LDBL_MIN_10_EXP__ (-307)\000"
.LASF1059:
	.ascii	"TEE_TYPE_HMAC_MD5 0xA0000001\000"
.LASF1980:
	.ascii	"AHB2_SIZE 0x01040000\000"
.LASF221:
	.ascii	"__FLT64_MAX__ 1.7976931348623157e+308F64\000"
.LASF855:
	.ascii	"_SIZE_T_DEFINED \000"
.LASF541:
	.ascii	"CFG_PM_STUBS 1\000"
.LASF1431:
	.ascii	"QUEUEDEBUG_TAILQ_POSTREMOVE(elm,field) \000"
.LASF692:
	.ascii	"__DECLARE_KEEP_PAGER1(sym,file_id) __DECLARE_KEEP_P"
	.ascii	"AGER2(sym, file_id)\000"
.LASF591:
	.ascii	"CFG_STM32_SHARED_IO 1\000"
.LASF1123:
	.ascii	"TEE_ATTR_ED25519_PRIVATE_VALUE 0xC0000843\000"
.LASF1605:
	.ascii	"CORTEX_A72_PART_NUM U(0xD08)\000"
.LASF1682:
	.ascii	"CPSR_MODE_SVC ARM32_CPSR_MODE_SVC\000"
.LASF1657:
	.ascii	"ARM32_CPSR_MODE_SVC U(0x13)\000"
.LASF99:
	.ascii	"__SIG_ATOMIC_WIDTH__ 32\000"
.LASF1977:
	.ascii	"APB5_BASE 0x5c000000\000"
.LASF1937:
	.ascii	"HW_UNIQUE_KEY_LENGTH (16)\000"
.LASF908:
	.ascii	"TEE_ERROR_NO_DATA 0xFFFF000B\000"
.LASF2048:
	.ascii	"UART1_BASE 0x5c000000\000"
.LASF1232:
	.ascii	"TEE_PANIC_ID_TEE_GETTAPERSISTENTTIME 0x00001403\000"
.LASF830:
	.ascii	"__bool_true_false_are_defined 1\000"
.LASF1609:
	.ascii	"CORTEX_A65AE_PART_NUM U(0xD43)\000"
.LASF1394:
	.ascii	"STAILQ_HEAD_INITIALIZER(head) { NULL, &(head).stqh_"
	.ascii	"first }\000"
.LASF1118:
	.ascii	"TEE_ATTR_SM2_KEP_CONFIRMATION_OUT 0xD0000846\000"
.LASF651:
	.ascii	"__weak __attribute__((weak))\000"
.LASF1328:
	.ascii	"preinit_early(fn) __define_initcall(preinit, 1, fn)"
	.ascii	"\000"
.LASF1720:
	.ascii	"SCTLR_SPAN BIT32(23)\000"
.LASF401:
	.ascii	"__ARM_FEATURE_LDREX\000"
.LASF1115:
	.ascii	"TEE_ATTR_SM2_ID_RESPONDER 0xD0000546\000"
.LASF436:
	.ascii	"__ARM_ARCH_7A__ 1\000"
.LASF1785:
	.ascii	"IDPFR1_GENTIMER_MASK SHIFT_U32(0xF, IDPFR1_GENTIMER"
	.ascii	"_SHIFT)\000"
.LASF1253:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSQUARE 0x00001905\000"
.LASF1616:
	.ascii	"CORTEX_A710_PART_NUM U(0xD47)\000"
.LASF1451:
	.ascii	"CIRCLEQ_HEAD(name,type) struct name { struct type *"
	.ascii	"cqh_first; struct type *cqh_last; }\000"
.LASF2094:
	.ascii	"DBGMCU_IDC_REV_ID_SHIFT U(16)\000"
.LASF2110:
	.ascii	"STM32MP131A_PART_NB U(0x050106C9)\000"
.LASF298:
	.ascii	"__ACCUM_MIN__ (-0X1P15K-0X1P15K)\000"
.LASF255:
	.ascii	"__USFRACT_EPSILON__ 0x1P-8UHR\000"
.LASF942:
	.ascii	"TEE_PROPSET_CURRENT_TA (TEE_PropSetHandle)0xFFFFFFF"
	.ascii	"F\000"
.LASF2153:
	.ascii	"CORE_MMU_USER_CODE_SIZE BIT(CORE_MMU_USER_CODE_SHIF"
	.ascii	"T)\000"
.LASF29:
	.ascii	"__ORDER_BIG_ENDIAN__ 4321\000"
.LASF1992:
	.ascii	"DDR_BASE 0xc0000000ul\000"
.LASF400:
	.ascii	"__ARM_FEATURE_CMSE\000"
.LASF452:
	.ascii	"__gnu_linux__ 1\000"
.LASF759:
	.ascii	"INT_FAST8_MIN INT8_MIN\000"
.LASF264:
	.ascii	"__UFRACT_MAX__ 0XFFFFP-16UR\000"
.LASF1802:
	.ascii	"THREAD_ID_0 0\000"
.LASF367:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1\000"
.LASF477:
	.ascii	"CFG_CORE_BIGNUM_MAX_BITS 4096\000"
.LASF67:
	.ascii	"__UINT_FAST32_TYPE__ unsigned int\000"
.LASF518:
	.ascii	"CFG_FTRACE_US_MS 10000\000"
.LASF1547:
	.ascii	"TEE_MATTR_MEM_TYPE_SHIFT U(12)\000"
.LASF98:
	.ascii	"__SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)\000"
.LASF545:
	.ascii	"CFG_REGULATOR_FIXED 1\000"
.LASF2113:
	.ascii	"STM32MP133F_PART_NB U(0x050108C0)\000"
.LASF1214:
	.ascii	"TEE_PANIC_ID_TEE_CIPHERUPDATE 0x00000E03\000"
.LASF2005:
	.ascii	"GPIOF_BASE 0x50007000\000"
.LASF2027:
	.ascii	"SPI6_BASE 0x5c001000\000"
.LASF790:
	.ascii	"UINT64_C(v) ULL(v)\000"
.LASF996:
	.ascii	"TEE_ALG_SM4_CTR 0x10000214\000"
.LASF475:
	.ascii	"CFG_COMPAT_GP10_DES 1\000"
.LASF1999:
	.ascii	"GIC_BASE 0xa0021000ul\000"
.LASF1312:
	.ascii	"OUTRMSG(r) do { OUTMSG(\"r=[%x]\", r); return r; } "
	.ascii	"while (0)\000"
.LASF1301:
	.ascii	"TRACE_PRINTF_LEVEL TRACE_ERROR\000"
.LASF497:
	.ascii	"CFG_CRYPTO_RSASSA_NA1 1\000"
.LASF1919:
	.ascii	"TEE_U64_FROM_LITTLE_ENDIAN(x) ((uint64_t)(x))\000"
.LASF1875:
	.ascii	"TEE_MAIN_ALGO_DH 0x32\000"
.LASF682:
	.ascii	"__compiler_add_overflow(a,b,res) __builtin_add_over"
	.ascii	"flow((a), (b), (res))\000"
.LASF881:
	.ascii	"__need_wchar_t\000"
.LASF155:
	.ascii	"__FLT_DECIMAL_DIG__ 9\000"
.LASF638:
	.ascii	"_CFG_CORE_LTC_SIZE_OPTIMIZATION 1\000"
.LASF1948:
	.ascii	"TEE_MM_POOL_NEX_MALLOC (1u << 1)\000"
.LASF836:
	.ascii	"_T_PTRDIFF \000"
.LASF1308:
	.ascii	"DMSG(...) (void)0\000"
.LASF968:
	.ascii	"TEE_OPERATION_MAC 3\000"
.LASF1064:
	.ascii	"TEE_TYPE_HMAC_SHA512 0xA0000006\000"
.LASF757:
	.ascii	"UINT_LEAST32_MAX UINT32_MAX\000"
.LASF1453:
	.ascii	"CIRCLEQ_ENTRY(type) struct { struct type *cqe_next;"
	.ascii	" struct type *cqe_prev; }\000"
.LASF1156:
	.ascii	"TEE_PANIC_ID_TEE_PANIC 0x00000301\000"
.LASF747:
	.ascii	"INT_LEAST8_MIN INT8_MIN\000"
.LASF470:
	.ascii	"CFG_ARM_GIC_PM 1\000"
.LASF1738:
	.ascii	"CPACR_D32DIS BIT32(30)\000"
.LASF1675:
	.ascii	"SYS_CDEFS_H \000"
.LASF273:
	.ascii	"__ULFRACT_MIN__ 0.0ULR\000"
.LASF841:
	.ascii	"_GCC_PTRDIFF_T \000"
.LASF977:
	.ascii	"TEE_ALG_AES_CBC_NOPAD 0x10000110\000"
.LASF620:
	.ascii	"CFG_TZDRAM_START (0xc0000000 + 0x40000000 - 0x02000"
	.ascii	"000)\000"
.LASF453:
	.ascii	"__linux 1\000"
.LASF745:
	.ascii	"UINT32_MAX 0xffffffffU\000"
.LASF1887:
	.ascii	"TEE_CHAIN_MODE_CBC_NOPAD 0x1\000"
.LASF1836:
	.ascii	"TEE_ATTR_HKDF_INFO 0xD00003C0\000"
.LASF445:
	.ascii	"__ARM_FEATURE_CDE\000"
.LASF1383:
	.ascii	"SLIST_INSERT_AFTER(slistelm,elm,field) do { (elm)->"
	.ascii	"field.sle_next = (slistelm)->field.sle_next; (slist"
	.ascii	"elm)->field.sle_next = (elm); } while ( 0)\000"
.LASF1805:
	.ascii	"THREAD_PARAM_MEMREF(_direction,_mobj,_offs,_size) ("
	.ascii	"struct thread_param){ .attr = THREAD_PARAM_ATTR_MEM"
	.ascii	"REF_ ## _direction, .u.memref = { .mobj = (_mobj), "
	.ascii	".offs = (_offs), .size = (_size) } }\000"
.LASF17:
	.ascii	"__FINITE_MATH_ONLY__ 0\000"
.LASF656:
	.ascii	"__noinline __attribute__((noinline))\000"
.LASF2085:
	.ascii	"TAMP_BKP_REGISTER_ZONE2_COUNT U(5)\000"
.LASF393:
	.ascii	"__ARM_FEATURE_UNALIGNED\000"
.LASF1796:
	.ascii	"THREAD_EXCP_FOREIGN_INTR (ARM32_CPSR_I >> ARM32_CPS"
	.ascii	"R_F_SHIFT)\000"
.LASF1864:
	.ascii	"TEE_MAIN_ALGO_SHA256 0x04\000"
.LASF1723:
	.ascii	"SCTLR_NMFI BIT32(26)\000"
.LASF2004:
	.ascii	"GPIOE_BASE 0x50006000\000"
.LASF449:
	.ascii	"__ARM_FEATURE_BF16_VECTOR_ARITHMETIC\000"
.LASF472:
	.ascii	"CFG_BOOT_SECONDARY_REQUEST 1\000"
.LASF530:
	.ascii	"CFG_LOCKDEP_RECORD_STACK 1\000"
.LASF1092:
	.ascii	"TEE_ATTR_RSA_PRIME1 0xC0000430\000"
.LASF2032:
	.ascii	"TIM1_BASE 0x44000000\000"
.LASF1329:
	.ascii	"preinit(fn) __define_initcall(preinit, 2, fn)\000"
.LASF1154:
	.ascii	"TEE_PANIC_ID_TEE_RESETPROPERTYENUMERATOR 0x0000020B"
	.ascii	"\000"
.LASF1580:
	.ascii	"KERN_IDENTITY ((TEE_Identity *)-1)\000"
.LASF557:
	.ascii	"CFG_SECURE_TIME_SOURCE_CNTPCT 1\000"
.LASF1245:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCMPS32 0x00001802\000"
.LASF1772:
	.ascii	"TTBCR_SHX_OSH U(0x2)\000"
.LASF1568:
	.ascii	"TA_FLAG_CONCURRENT (1 << 8)\000"
.LASF1255:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTADDMOD 0x00001A01\000"
.LASF519:
	.ascii	"CFG_GIC 1\000"
.LASF406:
	.ascii	"__ARM_SIZEOF_MINIMAL_ENUM 4\000"
.LASF1916:
	.ascii	"TEE_U64_BSWAP(x) __compiler_bswap64((x))\000"
.LASF1075:
	.ascii	"TEE_TYPE_ED25519_PUBLIC_KEY 0xA0000043\000"
.LASF486:
	.ascii	"CFG_CORE_RWDATA_NOEXEC 1\000"
.LASF301:
	.ascii	"__UACCUM_FBIT__ 16\000"
.LASF47:
	.ascii	"__INT32_TYPE__ int\000"
.LASF1817:
	.ascii	"STACK_CHECK_EXTRA 0\000"
.LASF2142:
	.ascii	"CORE_MMU_USER_PARAM_SHIFT SMALL_PAGE_SHIFT\000"
.LASF1042:
	.ascii	"TEE_ALG_ECDSA_P256 0x70003041\000"
.LASF1601:
	.ascii	"CORTEX_A9_PART_NUM U(0xC09)\000"
.LASF350:
	.ascii	"__DA_FBIT__ 31\000"
.LASF207:
	.ascii	"__FLT32_MIN__ 1.1754943508222875e-38F32\000"
.LASF1945:
	.ascii	"TEE_MM_H \000"
.LASF2178:
	.ascii	"THREAD_CLF_ABORT_SHIFT 1\000"
.LASF2061:
	.ascii	"CONSOLE_UART_BASE STM32MP1_DEBUG_USART_BASE\000"
.LASF1139:
	.ascii	"TEE_PANIC_ID_TA_CLOSESESSIONENTRYPOINT 0x00000101\000"
.LASF215:
	.ascii	"__FLT64_DIG__ 15\000"
.LASF1704:
	.ascii	"SCR_SCD BIT32(7)\000"
.LASF370:
	.ascii	"__GCC_ATOMIC_BOOL_LOCK_FREE 2\000"
.LASF132:
	.ascii	"__INT_FAST32_MAX__ 0x7fffffff\000"
.LASF2018:
	.ascii	"MCUSRAM_BASE 0x30000000ul\000"
.LASF2161:
	.ascii	"register_phys_mem_ul(type,addr,size) __register_mem"
	.ascii	"ory_ul(#addr, (type), (addr), (size), phys_mem_map)"
	.ascii	"\000"
.LASF894:
	.ascii	"TEE_ERROR_STORAGE_NOT_AVAILABLE 0xF0100003\000"
.LASF1458:
	.ascii	"CIRCLEQ_INSERT_TAIL(head,elm,field) do { QUEUEDEBUG"
	.ascii	"_CIRCLEQ_HEAD((head), field) (elm)->field.cqe_next "
	.ascii	"= (void *)(head); (elm)->field.cqe_prev = (head)->c"
	.ascii	"qh_last; if ((head)->cqh_first == (void *)(head)) ("
	.ascii	"head)->cqh_first = (elm); else (head)->cqh_last->fi"
	.ascii	"eld.cqe_next = (elm); (head)->cqh_last = (elm); } w"
	.ascii	"hile ( 0)\000"
.LASF1455:
	.ascii	"CIRCLEQ_INSERT_AFTER(head,listelm,elm,field) do { Q"
	.ascii	"UEUEDEBUG_CIRCLEQ_HEAD((head), field) QUEUEDEBUG_CI"
	.ascii	"RCLEQ_ELM((head), (listelm), field) (elm)->field.cq"
	.ascii	"e_next = (listelm)->field.cqe_next; (elm)->field.cq"
	.ascii	"e_prev = (listelm); if ((listelm)->field.cqe_next ="
	.ascii	"= (void *)(head)) (head)->cqh_last = (elm); else (l"
	.ascii	"istelm)->field.cqe_next->field.cqe_prev = (elm); (l"
	.ascii	"istelm)->field.cqe_next = (elm); } while ( 0)\000"
.LASF351:
	.ascii	"__DA_IBIT__ 32\000"
.LASF1550:
	.ascii	"TEE_MATTR_MEM_TYPE_STRONGLY_O U(2)\000"
.LASF334:
	.ascii	"__TQ_FBIT__ 127\000"
.LASF423:
	.ascii	"__VFP_FP__ 1\000"
.LASF2009:
	.ascii	"GPIOJ_BASE 0x5000b000\000"
.LASF1495:
	.ascii	"SIZE_4M UINTPTR_C(0x400000)\000"
.LASF442:
	.ascii	"__ARM_ASM_SYNTAX_UNIFIED__ 1\000"
.LASF777:
	.ascii	"SIZE_MAX ULONG_MAX\000"
.LASF2060:
	.ascii	"GIC_SPI_UART4 84\000"
.LASF1345:
	.ascii	"PRIxPASZ PRIxPTR\000"
.LASF1695:
	.ascii	"CPSR_IT_MASK2 ARM32_CPSR_IT_MASK2\000"
.LASF699:
	.ascii	"__SCT_ARRAY_DEF_ITEM2(array_name,order,id,element_t"
	.ascii	"ype) __SCT_ARRAY_DEF_ITEM3(element_type, __scattere"
	.ascii	"d_array_ ## id ## array_name, \".scattered_array_\""
	.ascii	" #array_name \"_1_\" #order)\000"
.LASF1733:
	.ascii	"NSACR_NSD32DIS BIT32(14)\000"
.LASF1897:
	.ascii	"TEE_ALG_GET_MAIN_ALG(algo) __tee_alg_get_main_alg(a"
	.ascii	"lgo)\000"
.LASF2040:
	.ascii	"TIM12_BASE 0x40006000\000"
.LASF1016:
	.ascii	"TEE_ALG_DSA_SHA1 0x70002131\000"
.LASF2107:
	.ascii	"STM32MP133C_PART_NB U(0x050100C0)\000"
.LASF1205:
	.ascii	"TEE_PANIC_ID_TEE_GETOPERATIONINFO 0x00000C04\000"
.LASF1048:
	.ascii	"TEE_ALG_ECDH_P256 0x80003042\000"
.LASF1944:
	.ascii	"FILE_TAG_SIZE TEE_SHA256_HASH_SIZE\000"
.LASF566:
	.ascii	"CFG_STM32MP1_PM_CONTEXT_VERSION 3\000"
.LASF501:
	.ascii	"CFG_DEVICE_ENUM_PTA 1\000"
.LASF1843:
	.ascii	"TEE_TYPE_CONCAT_KDF_Z 0xA10000C1\000"
.LASF1138:
	.ascii	"TEE_ECC_CURVE_SM2 0x00000400\000"
.LASF1113:
	.ascii	"TEE_ATTR_ECC_CURVE 0xF0000441\000"
.LASF1941:
	.ascii	"UTEE_SE_READER_PRESENT (1 << 0)\000"
.LASF1175:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTVALUEATTRIBUTE 0x00000704"
	.ascii	"\000"
.LASF1236:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTFMMSIZEINU32 0x00001502\000"
.LASF381:
	.ascii	"__GCC_CONSTRUCTIVE_SIZE 64\000"
.LASF407:
	.ascii	"__ARM_SIZEOF_WCHAR_T 4\000"
.LASF1654:
	.ascii	"ARM32_CPSR_MODE_USR U(0x10)\000"
.LASF23:
	.ascii	"__SIZEOF_DOUBLE__ 8\000"
.LASF138:
	.ascii	"__UINT_FAST32_MAX__ 0xffffffffU\000"
.LASF185:
	.ascii	"__LDBL_MAX_EXP__ 1024\000"
.LASF1679:
	.ascii	"CPSR_MODE_USR ARM32_CPSR_MODE_USR\000"
.LASF2118:
	.ascii	"STM32MP157A_PART_NB U(0x05000001)\000"
.LASF1402:
	.ascii	"STAILQ_REMOVE_AFTER(head,elm,field) do { if ((STAIL"
	.ascii	"Q_NEXT(elm, field) = STAILQ_NEXT(STAILQ_NEXT(elm, f"
	.ascii	"ield), field)) == NULL) (head)->stqh_last = &STAILQ"
	.ascii	"_NEXT((elm), field); } while (0)\000"
.LASF775:
	.ascii	"INTMAX_MIN INT64_MIN\000"
.LASF1087:
	.ascii	"TEE_TYPE_X25519_KEYPAIR 0xA1000044\000"
.LASF1376:
	.ascii	"LIST_FIRST(head) ((head)->lh_first)\000"
.LASF2011:
	.ascii	"GPIOZ_BASE 0x54004000\000"
.LASF1975:
	.ascii	"APB4_BASE 0x5a000000\000"
.LASF871:
	.ascii	"_BSD_WCHAR_T_ \000"
.LASF429:
	.ascii	"__ARM_FEATURE_FP16_VECTOR_ARITHMETIC\000"
.LASF1262:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTISPROBABLEPRIME 0x00001B02\000"
.LASF637:
	.ascii	"_CFG_CORE_LTC_OPTEE_THREAD 1\000"
.LASF1983:
	.ascii	"AHB5_BASE 0x54000000\000"
.LASF831:
	.ascii	"_STDDEF_H \000"
.LASF707:
	.ascii	"SCATTERED_ARRAY_BEGIN(array_name,element_type) (__e"
	.ascii	"xtension__({ static const element_type __scattered_"
	.ascii	"array_begin[0] __unused __section(\".scattered_arra"
	.ascii	"y_\" #array_name \"_0\"); (const element_type *)sca"
	.ascii	"ttered_array_relax_ptr( __scattered_array_begin); }"
	.ascii	"))\000"
.LASF1705:
	.ascii	"SCR_HCE BIT32(8)\000"
.LASF1929:
	.ascii	"TEE_U32_TO_BIG_ENDIAN(x) TEE_U32_BSWAP(x)\000"
.LASF1375:
	.ascii	"LIST_EMPTY(head) ((head)->lh_first == NULL)\000"
.LASF683:
	.ascii	"__compiler_sub_overflow(a,b,res) __builtin_sub_over"
	.ascii	"flow((a), (b), (res))\000"
.LASF1751:
	.ascii	"PAR32_PA_MASK (BIT32(20) - 1)\000"
.LASF2179:
	.ascii	"THREAD_CLF_IRQ_SHIFT 2\000"
.LASF378:
	.ascii	"__GCC_ATOMIC_LLONG_LOCK_FREE 2\000"
.LASF1157:
	.ascii	"TEE_PANIC_ID_TEE_CLOSETASESSION 0x00000401\000"
.LASF1943:
	.ascii	"UTEE_SE_READER_SELECT_RESPONE_ENABLE (1 << 2)\000"
.LASF92:
	.ascii	"__INTMAX_MAX__ 0x7fffffffffffffffLL\000"
.LASF1542:
	.ascii	"TEE_MATTR_URWX (TEE_MATTR_URW | TEE_MATTR_UX)\000"
.LASF2177:
	.ascii	"THREAD_CLF_TMP_SHIFT 0\000"
.LASF1933:
	.ascii	"TEE_TIME_LE(t1,t2) (((t1).seconds == (t2).seconds) "
	.ascii	"? ((t1).millis <= (t2).millis) : ((t1).seconds <= ("
	.ascii	"t2).seconds))\000"
.LASF1562:
	.ascii	"TA_FLAG_SINGLE_INSTANCE (1 << 2)\000"
.LASF901:
	.ascii	"TEE_ERROR_EXCESS_DATA 0xFFFF0004\000"
.LASF1630:
	.ascii	"MPIDR_AFF2_MASK (MPIDR_AFFLVL_MASK << MPIDR_AFF2_SH"
	.ascii	"IFT)\000"
.LASF1184:
	.ascii	"TEE_PANIC_ID_TEE_INITVALUEATTRIBUTE 0x00000806\000"
.LASF16:
	.ascii	"__OPTIMIZE__ 1\000"
.LASF957:
	.ascii	"TEE_USAGE_ENCRYPT 0x00000002\000"
.LASF581:
	.ascii	"CFG_STM32_I2C 1\000"
.LASF611:
	.ascii	"CFG_TEE_CORE_NB_CORE 2\000"
.LASF1716:
	.ascii	"SCTLR_HA BIT32(17)\000"
.LASF538:
	.ascii	"CFG_OS_REV_REPORTS_GIT_SHA1 1\000"
.LASF1736:
	.ascii	"NSACR_NS_SMP BIT32(18)\000"
.LASF1676:
	.ascii	"__restrict restrict\000"
.LASF1526:
	.ascii	"SUB_OVERFLOW(a,b,res) __compiler_sub_overflow((a), "
	.ascii	"(b), (res))\000"
.LASF1652:
	.ascii	"CTR_WORD_SIZE U(4)\000"
.LASF1039:
	.ascii	"TEE_ALG_HMAC_SM3 0x30000007\000"
.LASF1373:
	.ascii	"LIST_REMOVE(elm,field) do { QUEUEDEBUG_LIST_OP((elm"
	.ascii	"), field) if ((elm)->field.le_next != NULL) (elm)->"
	.ascii	"field.le_next->field.le_prev = (elm)->field.le_prev"
	.ascii	"; *(elm)->field.le_prev = (elm)->field.le_next; QUE"
	.ascii	"UEDEBUG_LIST_POSTREMOVE((elm), field) } while ( 0)\000"
.LASF15:
	.ascii	"__OPTIMIZE_SIZE__ 1\000"
.LASF1476:
	.ascii	"MALLOC_INITIAL_POOL_MIN_SIZE 1024\000"
.LASF1404:
	.ascii	"STAILQ_FOREACH_SAFE(var,head,field,tvar) for ((var)"
	.ascii	" = STAILQ_FIRST((head)); (var) && ((tvar) = STAILQ_"
	.ascii	"NEXT((var), field), 1); (var) = (tvar))\000"
.LASF1810:
	.ascii	"SM_STACK_TMP_RESERVE_SIZE sizeof(struct sm_ctx)\000"
.LASF561:
	.ascii	"CFG_STACK_THREAD_EXTRA 0\000"
.LASF1437:
	.ascii	"TAILQ_REMOVE(head,elm,field) do { QUEUEDEBUG_TAILQ_"
	.ascii	"PREREMOVE((head), (elm), field) QUEUEDEBUG_TAILQ_OP"
	.ascii	"((elm), field) if (((elm)->field.tqe_next) != NULL)"
	.ascii	" (elm)->field.tqe_next->field.tqe_prev = (elm)->fie"
	.ascii	"ld.tqe_prev; else (head)->tqh_last = (elm)->field.t"
	.ascii	"qe_prev; *(elm)->field.tqe_prev = (elm)->field.tqe_"
	.ascii	"next; QUEUEDEBUG_TAILQ_POSTREMOVE((elm), field); } "
	.ascii	"while ( 0)\000"
.LASF565:
	.ascii	"CFG_STM32MP15_RSTCTRL 1\000"
.LASF1137:
	.ascii	"TEE_ECC_CURVE_25519 0x00000300\000"
.LASF653:
	.ascii	"__pure __attribute__((pure))\000"
.LASF1099:
	.ascii	"TEE_ATTR_DSA_BASE 0xD0001231\000"
.LASF13:
	.ascii	"__ATOMIC_ACQ_REL 4\000"
.LASF868:
	.ascii	"_T_WCHAR \000"
.LASF2019:
	.ascii	"MDMA_BASE 0x58000000\000"
.LASF861:
	.ascii	"_SIZET_ \000"
.LASF693:
	.ascii	"DECLARE_KEEP_PAGER(sym) __DECLARE_KEEP_PAGER1(sym, "
	.ascii	"__FILE_ID__)\000"
.LASF45:
	.ascii	"__INT8_TYPE__ signed char\000"
.LASF193:
	.ascii	"__LDBL_DENORM_MIN__ 4.9406564584124654e-324L\000"
.LASF702:
	.ascii	"__SCT_ARRAY_DEF_PG_ITEM1(array_name,order,id,elemen"
	.ascii	"t_type) __SCT_ARRAY_DEF_PG_ITEM2(array_name, order,"
	.ascii	" id, element_type)\000"
.LASF617:
	.ascii	"CFG_TEE_TA_LOG_LEVEL 1\000"
.LASF368:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1\000"
.LASF1824:
	.ascii	"TEE_API_DEFINES_EXTENSIONS_H \000"
.LASF1670:
	.ascii	"ARM32_CPSR_IT_MASK1 U(0x06000000)\000"
.LASF1979:
	.ascii	"AHB2_BASE 0x48000000\000"
.LASF832:
	.ascii	"_STDDEF_H_ \000"
.LASF1993:
	.ascii	"DDRCTRL_BASE 0x5a003000\000"
.LASF813:
	.ascii	"PRIu32 \"u\"\000"
.LASF920:
	.ascii	"TEE_ERROR_TIME_NOT_SET 0xFFFF5000\000"
.LASF710:
	.ascii	"TEE_API_TYPES_H \000"
.LASF299:
	.ascii	"__ACCUM_MAX__ 0X7FFFFFFFP-15K\000"
.LASF1649:
	.ascii	"CTR_L1IP_MASK U(0x3)\000"
.LASF1807:
	.ascii	"__KERNEL_THREAD_PRIVATE_H \000"
.LASF1747:
	.ascii	"PAR_F BIT32(0)\000"
.LASF437:
	.ascii	"__ARM_PCS 1\000"
.LASF1438:
	.ascii	"TAILQ_FOREACH(var,head,field) for ((var) = ((head)-"
	.ascii	">tqh_first); (var); (var) = ((var)->field.tqe_next)"
	.ascii	")\000"
.LASF216:
	.ascii	"__FLT64_MIN_EXP__ (-1021)\000"
.LASF63:
	.ascii	"__INT_FAST32_TYPE__ int\000"
.LASF2008:
	.ascii	"GPIOI_BASE 0x5000a000\000"
.LASF1953:
	.ascii	"Z_IS_ENABLED1(config_macro) Z_IS_ENABLED2(_XXXX ##c"
	.ascii	"onfig_macro)\000"
.LASF2135:
	.ascii	"PLAT_MAX_PLLCFG_NB 6U\000"
.LASF568:
	.ascii	"CFG_STM32MP1_SHARED_RESOURCES 1\000"
.LASF2105:
	.ascii	"STM32MP135C_PART_NB U(0x05010000)\000"
.LASF2074:
	.ascii	"GIC_SEC_SGI_0 8\000"
.LASF1300:
	.ascii	"TRACE_MAX TRACE_FLOW\000"
.LASF1515:
	.ascii	"_CONCAT(x,y) x ##y\000"
.LASF1142:
	.ascii	"TEE_PANIC_ID_TA_INVOKECOMMANDENTRYPOINT 0x00000104\000"
.LASF432:
	.ascii	"__ARM_NEON__\000"
.LASF2075:
	.ascii	"GIC_SEC_SGI_1 9\000"
.LASF2119:
	.ascii	"STM32MP153C_PART_NB U(0x05000024)\000"
.LASF109:
	.ascii	"__INT8_C(c) c\000"
.LASF1814:
	.ascii	"STACK_TMP_SIZE (2048 + STACK_TMP_OFFS + CFG_STACK_T"
	.ascii	"MP_EXTRA)\000"
.LASF1702:
	.ascii	"SCR_AW BIT32(5)\000"
.LASF1310:
	.ascii	"INMSG(...) FMSG(\"> \" __VA_ARGS__)\000"
.LASF61:
	.ascii	"__INT_FAST8_TYPE__ signed char\000"
.LASF1806:
	.ascii	"THREAD_PARAM_VALUE(_direction,_a,_b,_c) (struct thr"
	.ascii	"ead_param){ .attr = THREAD_PARAM_ATTR_VALUE_ ## _di"
	.ascii	"rection, .u.value = { .a = (_a), .b = (_b), .c = (_"
	.ascii	"c) } }\000"
.LASF612:
	.ascii	"CFG_TEE_CORE_TA_TRACE 1\000"
.LASF640:
	.ascii	"_CFG_FTRACE_BUF_WHEN_FULL_shift 1\000"
.LASF1066:
	.ascii	"TEE_TYPE_RSA_PUBLIC_KEY 0xA0000030\000"
.LASF1777:
	.ascii	"TLBI_ASID_MASK U(0xff)\000"
.LASF308:
	.ascii	"__LACCUM_MIN__ (-0X1P31LK-0X1P31LK)\000"
.LASF582:
	.ascii	"CFG_STM32_IWDG 1\000"
.LASF1251:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTMUL 0x00001903\000"
.LASF2175:
	.ascii	"THREAD_CLF_CURR_SHIFT 0\000"
.LASF1789:
	.ascii	"CNTCVU_OFFSET 0x00C\000"
.LASF232:
	.ascii	"__FLT32X_MIN_EXP__ (-1021)\000"
.LASF999:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA224 0x70003830\000"
.LASF580:
	.ascii	"CFG_STM32_HUK_FROM_DT 1\000"
.LASF839:
	.ascii	"_BSD_PTRDIFF_T_ \000"
.LASF869:
	.ascii	"__WCHAR_T \000"
.LASF1792:
	.ascii	"__ARM32_SYSREG_H \000"
.LASF1787:
	.ascii	"CNTSR_OFFSET 0x004\000"
.LASF991:
	.ascii	"TEE_ALG_DES3_CBC_NOPAD 0x10000113\000"
.LASF1002:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA512 0x70006830\000"
.LASF408:
	.ascii	"__ARM_ARCH_PROFILE\000"
.LASF1769:
	.ascii	"TTBCR_XRGNX_WT U(0x2)\000"
.LASF1555:
	.ascii	"VM_FLAG_SHAREABLE BIT(2)\000"
.LASF2173:
	.ascii	"phys_mem_map_end SCATTERED_ARRAY_END(phys_mem_map, "
	.ascii	"struct core_mmu_phys_mem)\000"
.LASF1261:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCOMPUTEEXTENDEDGCD 0x00001B0"
	.ascii	"1\000"
.LASF1452:
	.ascii	"CIRCLEQ_HEAD_INITIALIZER(head) { (void *)&head, (vo"
	.ascii	"id *)&head }\000"
.LASF2171:
	.ascii	"phys_sdp_mem_end SCATTERED_ARRAY_END(phys_sdp_mem, "
	.ascii	"struct core_mmu_phys_mem)\000"
.LASF2012:
	.ascii	"HASH1_BASE 0x54002000\000"
.LASF1600:
	.ascii	"CORTEX_A8_PART_NUM U(0xC08)\000"
.LASF112:
	.ascii	"__INT16_C(c) c\000"
.LASF840:
	.ascii	"___int_ptrdiff_t_h \000"
.LASF914:
	.ascii	"TEE_ERROR_EXTERNAL_CANCEL 0xFFFF0011\000"
.LASF1244:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCMP 0x00001801\000"
.LASF128:
	.ascii	"__INT_FAST8_MAX__ 0x7f\000"
.LASF946:
	.ascii	"TEE_MALLOC_FILL_ZERO 0x00000000\000"
.LASF458:
	.ascii	"unix 1\000"
.LASF1527:
	.ascii	"MUL_OVERFLOW(a,b,res) __compiler_mul_overflow((a), "
	.ascii	"(b), (res))\000"
.LASF1693:
	.ascii	"CPSR_IT_MASK ARM32_CPSR_IT_MASK\000"
.LASF2193:
	.ascii	"long long unsigned int\000"
.LASF658:
	.ascii	"__unused __attribute__((unused))\000"
.LASF1289:
	.ascii	"_VA_LIST_ \000"
.LASF1892:
	.ascii	"TEE_CHAIN_MODE_CMAC 0x6\000"
.LASF781:
	.ascii	"L(v) v ## L\000"
.LASF1755:
	.ascii	"TTBCR_PD1 BIT32(5)\000"
.LASF1889:
	.ascii	"TEE_CHAIN_MODE_CTS 0x3\000"
.LASF2201:
	.ascii	"/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmwa"
	.ascii	"re/STM32-Computer-Firmware/CA7/optee-os-3.19.0-r2/o"
	.ascii	"ptee-os-stm32mp-3.19.0-stm32mp-r2\000"
.LASF755:
	.ascii	"UINT_LEAST8_MAX UINT8_MAX\000"
.LASF1586:
	.ascii	"MIDR_PRIMARY_PART_NUM_WIDTH U(12)\000"
.LASF1165:
	.ascii	"TEE_PANIC_ID_TEE_GETINSTANCEDATA 0x00000603\000"
.LASF1074:
	.ascii	"TEE_TYPE_ECDH_KEYPAIR 0xA1000042\000"
.LASF375:
	.ascii	"__GCC_ATOMIC_SHORT_LOCK_FREE 2\000"
.LASF104:
	.ascii	"__UINT8_MAX__ 0xff\000"
.LASF714:
	.ascii	"CHAR_MAX UCHAR_MAX\000"
.LASF998:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA1 0x70002830\000"
.LASF1171:
	.ascii	"TEE_PANIC_ID_TEE_SETINSTANCEDATA 0x00000609\000"
.LASF678:
	.ascii	"__compiler_bswap16(x) __builtin_bswap16((x))\000"
.LASF2101:
	.ascii	"BKPSRAM_PM_CONTEXT_SIZE U(0xF00)\000"
.LASF1794:
	.ascii	"VFP_NUM_REGS U(32)\000"
.LASF905:
	.ascii	"TEE_ERROR_ITEM_NOT_FOUND 0xFFFF0008\000"
.LASF1819:
	.ascii	"CORE_MMU_H \000"
.LASF474:
	.ascii	"CFG_CC_OPT_LEVEL s\000"
.LASF511:
	.ascii	"CFG_DTB_MAX_SIZE (256 * 1024)\000"
.LASF1235:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTFMMCONTEXTSIZEINU32 0x000015"
	.ascii	"01\000"
.LASF609:
	.ascii	"CFG_TEE_CORE_EMBED_INTERNAL_TESTS 1\000"
.LASF136:
	.ascii	"__UINT_FAST8_MAX__ 0xff\000"
.LASF1089:
	.ascii	"TEE_ATTR_RSA_MODULUS 0xD0000130\000"
.LASF1548:
	.ascii	"TEE_MATTR_MEM_TYPE_DEV U(0)\000"
.LASF2028:
	.ascii	"STGEN_BASE 0x5c008000\000"
.LASF387:
	.ascii	"__SIZEOF_WINT_T__ 4\000"
.LASF731:
	.ascii	"ULLONG_MAX (LLONG_MAX * 2ULL + 1ULL)\000"
.LASF1216:
	.ascii	"TEE_PANIC_ID_TEE_MACCOMPUTEFINAL 0x00000F02\000"
.LASF220:
	.ascii	"__FLT64_DECIMAL_DIG__ 17\000"
.LASF142:
	.ascii	"__UINTPTR_MAX__ 0xffffffffU\000"
.LASF1040:
	.ascii	"TEE_ALG_ECDSA_P192 0x70001041\000"
.LASF473:
	.ascii	"CFG_BSEC_PTA 1\000"
.LASF1290:
	.ascii	"_VA_LIST \000"
.LASF1344:
	.ascii	"PRIxPA PRIxPTR\000"
.LASF529:
	.ascii	"CFG_LIBUTILS_WITH_ISOC 1\000"
.LASF1295:
	.ascii	"TRACE_MIN 0\000"
.LASF597:
	.ascii	"CFG_SYSCFG 1\000"
.LASF1662:
	.ascii	"ARM32_CPSR_T BIT(5)\000"
.LASF1521:
	.ascii	"SHIFT_U64(v,shift) ((uint64_t)(v) << (shift))\000"
.LASF1674:
	.ascii	"ARM32_H \000"
.LASF250:
	.ascii	"__SFRACT_EPSILON__ 0x1P-7HR\000"
.LASF14:
	.ascii	"__ATOMIC_CONSUME 1\000"
.LASF158:
	.ascii	"__FLT_MIN__ 1.1754943508222875e-38F\000"
.LASF54:
	.ascii	"__INT_LEAST16_TYPE__ short int\000"
.LASF520:
	.ascii	"CFG_GP_SOCKETS 1\000"
.LASF60:
	.ascii	"__UINT_LEAST64_TYPE__ long long unsigned int\000"
.LASF2125:
	.ascii	"STM32MP153F_PART_NB U(0x050000A4)\000"
.LASF251:
	.ascii	"__USFRACT_FBIT__ 8\000"
.LASF482:
	.ascii	"CFG_CORE_HUK_SUBKEY_COMPAT 1\000"
.LASF1382:
	.ascii	"SLIST_INIT(head) do { (head)->slh_first = NULL; } w"
	.ascii	"hile ( 0)\000"
.LASF1508:
	.ascii	"UDIV_ROUND_NEAREST(x,y) (__extension__ ({ __typeof_"
	.ascii	"_(x) _x = (x); __typeof__(y) _y = (y); (_x + (_y / "
	.ascii	"2)) / _y; }))\000"
.LASF952:
	.ascii	"TEE_DATA_FLAG_SHARE_WRITE 0x00000020\000"
.LASF1884:
	.ascii	"TEE_MAIN_ALGO_PBKDF2 0xC2\000"
.LASF374:
	.ascii	"__GCC_ATOMIC_WCHAR_T_LOCK_FREE 2\000"
.LASF525:
	.ascii	"CFG_HWSUPP_MEM_PERM_WXN 1\000"
.LASF1019:
	.ascii	"TEE_ALG_SM2_DSA_SM3 0x70006045\000"
.LASF55:
	.ascii	"__INT_LEAST32_TYPE__ int\000"
.LASF319:
	.ascii	"__LLACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LLK\000"
.LASF1700:
	.ascii	"SCR_EA BIT32(3)\000"
.LASF1246:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTGETBIT 0x00001803\000"
.LASF1050:
	.ascii	"TEE_ALG_ECDH_P521 0x80005042\000"
.LASF1078:
	.ascii	"TEE_TYPE_SM2_DSA_KEYPAIR 0xA1000045\000"
.LASF1479:
	.ascii	"nex_free(ptr) free(ptr)\000"
.LASF1932:
	.ascii	"TEE_TIME_LT(t1,t2) (((t1).seconds == (t2).seconds) "
	.ascii	"? ((t1).millis < (t2).millis) : ((t1).seconds < (t2"
	.ascii	").seconds))\000"
.LASF2122:
	.ascii	"STM32MP151A_PART_NB U(0x0500002F)\000"
.LASF784:
	.ascii	"UINT8_C(v) v\000"
.LASF987:
	.ascii	"TEE_ALG_DES_CBC_NOPAD 0x10000111\000"
.LASF1068:
	.ascii	"TEE_TYPE_DSA_PUBLIC_KEY 0xA0000031\000"
.LASF738:
	.ascii	"INT64_MIN (-0x7fffffffffffffffL-1)\000"
.LASF397:
	.ascii	"__ARM_FEATURE_COMPLEX\000"
.LASF516:
	.ascii	"CFG_EXT_DTS core/arch/arm/dts/external-dt/optee\000"
.LASF1292:
	.ascii	"_VA_LIST_T_H \000"
.LASF1046:
	.ascii	"TEE_ALG_ECDH_P192 0x80001042\000"
.LASF244:
	.ascii	"__FLT32X_HAS_QUIET_NAN__ 1\000"
.LASF1781:
	.ascii	"FSR_FS_MASK (BIT32(10) | (BIT32(4) - 1))\000"
.LASF34:
	.ascii	"__GNUC_EXECUTION_CHARSET_NAME \"UTF-8\"\000"
.LASF1169:
	.ascii	"TEE_PANIC_ID_TEE_MEMMOVE 0x00000607\000"
.LASF1010:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA1 0x60210230\000"
.LASF118:
	.ascii	"__INT64_C(c) c ## LL\000"
.LASF1570:
	.ascii	"TA_FLAG_DEVICE_ENUM_SUPP (1 << 10)\000"
.LASF1985:
	.ascii	"ADC1_BASE 0x48003000\000"
.LASF167:
	.ascii	"__DBL_MIN_EXP__ (-1021)\000"
.LASF2071:
	.ascii	"GICC_OFFSET 0x1000\000"
.LASF1242:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTTOOCTETSTRING 0x00001"
	.ascii	"703\000"
.LASF1516:
	.ascii	"container_of(ptr,type,member) (__extension__({ cons"
	.ascii	"t typeof(((type *)0)->member) *__ptr = (ptr); (type"
	.ascii	" *)((unsigned long)(__ptr) - offsetof(type, member)"
	.ascii	"); }))\000"
.LASF1017:
	.ascii	"TEE_ALG_DSA_SHA224 0x70003131\000"
.LASF913:
	.ascii	"TEE_ERROR_SHORT_BUFFER 0xFFFF0010\000"
.LASF1871:
	.ascii	"TEE_MAIN_ALGO_DES3 0x13\000"
.LASF1355:
	.ascii	"COMPILE_TIME_ASSERT(x) do { switch (0) { case 0: ca"
	.ascii	"se ((x) ? 1: 0): default : break; } } while (0)\000"
.LASF1:
	.ascii	"__STDC_VERSION__ 201112L\000"
.LASF1972:
	.ascii	"APB2_SIZE 0x00014000\000"
.LASF186:
	.ascii	"__LDBL_MAX_10_EXP__ 308\000"
.LASF1477:
	.ascii	"mdbg_check(x) do { } while (0)\000"
.LASF979:
	.ascii	"TEE_ALG_AES_CTS 0x10000310\000"
.LASF1124:
	.ascii	"TEE_ATTR_X25519_PUBLIC_VALUE 0xD0000944\000"
.LASF2199:
	.ascii	"GNU C11 12.3.0 -mcpu=cortex-a7 -mfloat-abi=soft -mt"
	.ascii	"humb -mno-unaligned-access -mtls-dialect=gnu -march"
	.ascii	"=armv7ve -g3 -Os -std=gnu11 -ffunction-sections -fd"
	.ascii	"ata-sections -fno-short-enums -fno-common\000"
.LASF1701:
	.ascii	"SCR_FW BIT32(4)\000"
.LASF1507:
	.ascii	"DIV_ROUND_UP(x,y) (((x) + (y) - 1) / (y))\000"
.LASF1595:
	.ascii	"MIDR_REVISION_SHIFT U(0)\000"
.LASF797:
	.ascii	"PRId16 \"d\"\000"
.LASF2184:
	.ascii	"THREAD_CLF_FIQ BIT(THREAD_CLF_FIQ_SHIFT)\000"
.LASF48:
	.ascii	"__INT64_TYPE__ long long int\000"
.LASF2132:
	.ascii	"STM32MP1_REV_Z U(0x00002001)\000"
.LASF395:
	.ascii	"__ARM_FEATURE_CRC32\000"
.LASF352:
	.ascii	"__TA_FBIT__ 63\000"
.LASF190:
	.ascii	"__LDBL_NORM_MAX__ 1.7976931348623157e+308L\000"
.LASF1221:
	.ascii	"TEE_PANIC_ID_TEE_AEINIT 0x00001003\000"
.LASF827:
	.ascii	"bool _Bool\000"
.LASF225:
	.ascii	"__FLT64_DENORM_MIN__ 4.9406564584124654e-324F64\000"
.LASF2088:
	.ascii	"USART2_BASE UART2_BASE\000"
.LASF1647:
	.ascii	"CTR_DMINLINE_MASK (BIT(4) - 1)\000"
.LASF1863:
	.ascii	"TEE_MAIN_ALGO_SHA224 0x03\000"
.LASF962:
	.ascii	"TEE_USAGE_DERIVE 0x00000040\000"
.LASF572:
	.ascii	"CFG_STM32_BSEC 1\000"
.LASF633:
	.ascii	"PLATFORM_FLAVOR_ 1\000"
.LASF2069:
	.ascii	"HW2_OTP_PRODUCT_BELOW_2V5 BIT(13)\000"
.LASF1326:
	.ascii	"finalcall_begin SCATTERED_ARRAY_BEGIN(finalcall, st"
	.ascii	"ruct initcall)\000"
.LASF660:
	.ascii	"__used __attribute__((__used__))\000"
.LASF1025:
	.ascii	"TEE_ALG_SHA256 0x50000004\000"
.LASF2163:
	.ascii	"register_sdp_mem(addr,size) static int CONCAT(__reg"
	.ascii	"ister_sdp_mem_unused, __COUNTER__) __unused\000"
.LASF1313:
	.ascii	"DHEXDUMP(buf,len) (void)0\000"
.LASF1827:
	.ascii	"TEE_ALG_HKDF_MD5_DERIVE_KEY 0x800010C0\000"
.LASF347:
	.ascii	"__HA_IBIT__ 8\000"
.LASF1143:
	.ascii	"TEE_PANIC_ID_TA_OPENSESSIONENTRYPOINT 0x00000105\000"
.LASF353:
	.ascii	"__TA_IBIT__ 64\000"
.LASF1989:
	.ascii	"CRYP1_BASE 0x54001000\000"
.LASF1627:
	.ascii	"MPIDR_AFF1_SHIFT U(8)\000"
.LASF2181:
	.ascii	"THREAD_CLF_TMP BIT(THREAD_CLF_TMP_SHIFT)\000"
.LASF242:
	.ascii	"__FLT32X_HAS_DENORM__ 1\000"
.LASF1842:
	.ascii	"TEE_ALG_CONCAT_KDF_SHA512_DERIVE_KEY 0x800060C1\000"
.LASF212:
	.ascii	"__FLT32_HAS_QUIET_NAN__ 1\000"
.LASF441:
	.ascii	"__ARM_FEATURE_IDIV 1\000"
.LASF1590:
	.ascii	"MIDR_IMPLEMENTER_MASK (BIT(MIDR_IMPLEMENTER_WIDTH) "
	.ascii	"- 1)\000"
.LASF329:
	.ascii	"__HQ_IBIT__ 0\000"
.LASF802:
	.ascii	"PRIi16 \"i\"\000"
.LASF1967:
	.ascii	"STACK_ALIGNMENT 32\000"
.LASF2189:
	.ascii	"long long int\000"
.LASF145:
	.ascii	"__FLT_EVAL_METHOD__ 0\000"
.LASF1613:
	.ascii	"CORTEX_A78_PART_NUM U(0xD41)\000"
.LASF1575:
	.ascii	"TA_PROP_STR_DATA_SIZE \"gpd.ta.dataSize\"\000"
.LASF484:
	.ascii	"CFG_CORE_MBEDTLS_MPI 1\000"
.LASF1023:
	.ascii	"TEE_ALG_SHA1 0x50000002\000"
.LASF1584:
	.ascii	"ARM_H \000"
.LASF1468:
	.ascii	"CIRCLEQ_LOOP_PREV(head,elm,field) (((elm)->field.cq"
	.ascii	"e_prev == (void *)(head)) ? ((head)->cqh_last) : (e"
	.ascii	"lm->field.cqe_prev))\000"
.LASF354:
	.ascii	"__UHA_FBIT__ 8\000"
.LASF1304:
	.ascii	"trace_printf_helper(level,level_ok,...) trace_print"
	.ascii	"f(__func__, __LINE__, (level), (level_ok), __VA_ARG"
	.ascii	"S__)\000"
.LASF1803:
	.ascii	"THREAD_ID_INVALID -1\000"
.LASF1714:
	.ascii	"SCTLR_V BIT32(13)\000"
.LASF1379:
	.ascii	"SLIST_HEAD(name,type) struct name { struct type *sl"
	.ascii	"h_first; }\000"
.LASF1914:
	.ascii	"TEE_MAC_SIZE_DES_CBC_MAC_PKCS5 \000"
.LASF1307:
	.ascii	"IMSG(...) trace_printf_helper(TRACE_INFO, true, __V"
	.ascii	"A_ARGS__)\000"
.LASF403:
	.ascii	"__ARM_FEATURE_CLZ 1\000"
.LASF1359:
	.ascii	"__KERNEL_REFCOUNT_H \000"
.LASF1730:
	.ascii	"ACTLR_SMP BIT32(6)\000"
.LASF1514:
	.ascii	"CONCAT(x,y) _CONCAT(x, y)\000"
.LASF1976:
	.ascii	"APB4_SIZE 0x00008000\000"
.LASF402:
	.ascii	"__ARM_FEATURE_LDREX 15\000"
.LASF1728:
	.ascii	"ACTLR_CA8_ENABLE_INVALIDATE_BTB BIT(6)\000"
.LASF544:
	.ascii	"CFG_REGULATOR_DRIVERS 1\000"
.LASF1015:
	.ascii	"TEE_ALG_RSA_NOPAD 0x60000030\000"
.LASF489:
	.ascii	"CFG_CORE_UNMAP_CORE_AT_EL0 1\000"
.LASF858:
	.ascii	"__DEFINED_size_t \000"
.LASF1910:
	.ascii	"TEE_AES_MAX_KEY_SIZE 32UL\000"
.LASF1800:
	.ascii	"THREAD_FLAGS_FOREIGN_INTR_ENABLE BIT(1)\000"
.LASF434:
	.ascii	"__ARM_NEON_FP\000"
.LASF948:
	.ascii	"TEE_DATA_FLAG_ACCESS_READ 0x00000001\000"
.LASF91:
	.ascii	"__SIZE_WIDTH__ 32\000"
.LASF439:
	.ascii	"__FDPIC__\000"
.LASF1305:
	.ascii	"MSG(...) trace_printf_helper(0, false, __VA_ARGS__)"
	.ascii	"\000"
.LASF680:
	.ascii	"__HAVE_BUILTIN_OVERFLOW 1\000"
.LASF105:
	.ascii	"__UINT16_MAX__ 0xffff\000"
.LASF1459:
	.ascii	"CIRCLEQ_REMOVE(head,elm,field) do { QUEUEDEBUG_CIRC"
	.ascii	"LEQ_HEAD((head), field) QUEUEDEBUG_CIRCLEQ_ELM((hea"
	.ascii	"d), (elm), field) if ((elm)->field.cqe_next == (voi"
	.ascii	"d *)(head)) (head)->cqh_last = (elm)->field.cqe_pre"
	.ascii	"v; else (elm)->field.cqe_next->field.cqe_prev = (el"
	.ascii	"m)->field.cqe_prev; if ((elm)->field.cqe_prev == (v"
	.ascii	"oid *)(head)) (head)->cqh_first = (elm)->field.cqe_"
	.ascii	"next; else (elm)->field.cqe_prev->field.cqe_next = "
	.ascii	"(elm)->field.cqe_next; QUEUEDEBUG_CIRCLEQ_POSTREMOV"
	.ascii	"E((elm), field) } while ( 0)\000"
.LASF2104:
	.ascii	"MCUSRAM_SIZE U(0x60000)\000"
.LASF743:
	.ascii	"UINT8_MAX 0xff\000"
.LASF1277:
	.ascii	"TEE_MEMREF_3_USED 0x00000008\000"
.LASF923:
	.ascii	"TEE_PARAM_TYPE_VALUE_INPUT 1\000"
.LASF1958:
	.ascii	"__MM_GENERIC_RAM_LAYOUT_H \000"
.LASF720:
	.ascii	"LLONG_MAX __LONG_LONG_MAX__\000"
.LASF1152:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASUUID 0x00000209\000"
.LASF662:
	.ascii	"__cold __attribute__((__cold__))\000"
.LASF1928:
	.ascii	"TEE_U64_TO_BIG_ENDIAN(x) TEE_U64_BSWAP(x)\000"
.LASF1858:
	.ascii	"TEE_MEMORY_ACCESS_NONSECURE 0x10000000\000"
.LASF1891:
	.ascii	"TEE_CHAIN_MODE_CBC_MAC_PKCS5 0x5\000"
.LASF1371:
	.ascii	"LIST_INSERT_BEFORE(listelm,elm,field) do { QUEUEDEB"
	.ascii	"UG_LIST_OP((listelm), field) (elm)->field.le_prev ="
	.ascii	" (listelm)->field.le_prev; (elm)->field.le_next = ("
	.ascii	"listelm); *(listelm)->field.le_prev = (elm); (liste"
	.ascii	"lm)->field.le_prev = &(elm)->field.le_next; } while"
	.ascii	" ( 0)\000"
.LASF179:
	.ascii	"__DBL_HAS_QUIET_NAN__ 1\000"
.LASF461:
	.ascii	"ARM32 1\000"
.LASF1854:
	.ascii	"TEE_ALG_DES3_CMAC 0xF0000613\000"
.LASF1108:
	.ascii	"TEE_ATTR_RSA_OAEP_LABEL 0xD0000930\000"
.LASF1820:
	.ascii	"KERNEL_USER_TA_H \000"
.LASF1316:
	.ascii	"EMSG_RAW(...) trace_printf_helper_raw(TRACE_ERROR, "
	.ascii	"true, __VA_ARGS__)\000"
.LASF930:
	.ascii	"TEE_LOGIN_USER 0x00000001\000"
.LASF1336:
	.ascii	"driver_init_late(fn) __define_initcall(init, 6, fn)"
	.ascii	"\000"
.LASF252:
	.ascii	"__USFRACT_IBIT__ 0\000"
.LASF233:
	.ascii	"__FLT32X_MIN_10_EXP__ (-307)\000"
.LASF2103:
	.ascii	"RETRAM_SIZE U(0x10000)\000"
.LASF724:
	.ascii	"SCHAR_MIN (-SCHAR_MAX - 1)\000"
.LASF147:
	.ascii	"__DEC_EVAL_METHOD__ 2\000"
.LASF506:
	.ascii	"CFG_DRIVERS_CLK_EARLY_PROBE 1\000"
.LASF553:
	.ascii	"CFG_SCMI_MSG_SHM_MSG 1\000"
.LASF195:
	.ascii	"__LDBL_HAS_INFINITY__ 1\000"
.LASF2001:
	.ascii	"GPIOB_BASE 0x50003000\000"
.LASF2015:
	.ascii	"IWDG1_BASE 0x5c003000\000"
.LASF1004:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA1 0x70212930\000"
.LASF2195:
	.ascii	"long unsigned int\000"
.LASF1903:
	.ascii	"TEE_ALG_HASH_ALGO(main_hash) __tee_alg_hash_algo(ma"
	.ascii	"in_hash)\000"
.LASF1776:
	.ascii	"TLBI_MVA_SHIFT U(12)\000"
.LASF684:
	.ascii	"__compiler_mul_overflow(a,b,res) __builtin_mul_over"
	.ascii	"flow((a), (b), (res))\000"
.LASF1190:
	.ascii	"TEE_PANIC_ID_TEE_OPENPERSISTENTOBJECT 0x00000903\000"
.LASF1855:
	.ascii	"TEE_STORAGE_PRIVATE_REE 0x80000000\000"
.LASF1136:
	.ascii	"TEE_ECC_CURVE_NIST_P521 0x00000005\000"
.LASF249:
	.ascii	"__SFRACT_MAX__ 0X7FP-7HR\000"
.LASF1231:
	.ascii	"TEE_PANIC_ID_TEE_GETSYSTEMTIME 0x00001402\000"
.LASF897:
	.ascii	"TEE_ERROR_GENERIC 0xFFFF0000\000"
.LASF890:
	.ascii	"TEE_TIMEOUT_INFINITE 0xFFFFFFFF\000"
.LASF1417:
	.ascii	"SIMPLEQ_REMOVE(head,elm,type,field) do { if ((head)"
	.ascii	"->sqh_first == (elm)) { SIMPLEQ_REMOVE_HEAD((head),"
	.ascii	" field); } else { struct type *curelm = (head)->sqh"
	.ascii	"_first; while (curelm->field.sqe_next != (elm)) cur"
	.ascii	"elm = curelm->field.sqe_next; if ((curelm->field.sq"
	.ascii	"e_next = curelm->field.sqe_next->field.sqe_next) =="
	.ascii	" NULL) (head)->sqh_last = &(curelm)->field.sqe_next"
	.ascii	"; } } while ( 0)\000"
.LASF1988:
	.ascii	"BSEC_BASE 0x5c005000\000"
.LASF509:
	.ascii	"CFG_DRIVERS_RSTCTRL 1\000"
.LASF465:
	.ascii	"__home_lubuntu_Dokument_GitHub_STM32_Computer_Firmw"
	.ascii	"are_STM32_Computer_Firmware_CA7_optee_os_3_19_0_r2_"
	.ascii	"optee_os_stm32mp_3_19_0_stm32mp_r2____build_stm32mp"
	.ascii	"157c_ev1_include_generated_conf_h_ \000"
.LASF1936:
	.ascii	"HW_UNIQUE_KEY_WORD1 (8)\000"
.LASF767:
	.ascii	"UINT_FAST8_MAX UINT8_MAX\000"
.LASF1410:
	.ascii	"SIMPLEQ_HEAD_INITIALIZER(head) { NULL, &(head).sqh_"
	.ascii	"first }\000"
.LASF2082:
	.ascii	"TAMP_BKP_REGISTER_OFF 0x100\000"
.LASF816:
	.ascii	"PRIx8 \"x\"\000"
.LASF1505:
	.ascii	"ROUNDUP_DIV(x,y) (__extension__({ typeof(x) __round"
	.ascii	"up_x = (x); typeof(y) __roundup_mask = (typeof(x))("
	.ascii	"y) - 1; (__roundup_x / (y)) + (__roundup_x & __roun"
	.ascii	"dup_mask ? 1 : 0); }))\000"
.LASF1126:
	.ascii	"TEE_ATTR_EDDSA_PREHASH 0xF0000004\000"
.LASF24:
	.ascii	"__SIZEOF_LONG_DOUBLE__ 8\000"
.LASF1678:
	.ascii	"CPSR_MODE_MASK ARM32_CPSR_MODE_MASK\000"
.LASF1752:
	.ascii	"PAR64_PA_MASK (BIT64(28) - 1)\000"
.LASF280:
	.ascii	"__LLFRACT_EPSILON__ 0x1P-63LLR\000"
.LASF1707:
	.ascii	"SCTLR_M BIT32(0)\000"
.LASF226:
	.ascii	"__FLT64_HAS_DENORM__ 1\000"
.LASF1320:
	.ascii	"SMSG(...) trace_printf(__func__, __LINE__, TRACE_ER"
	.ascii	"ROR, true, __VA_ARGS__)\000"
.LASF1201:
	.ascii	"TEE_PANIC_ID_TEE_WRITEOBJECTDATA 0x00000B04\000"
.LASF161:
	.ascii	"__FLT_HAS_DENORM__ 1\000"
.LASF316:
	.ascii	"__LLACCUM_FBIT__ 31\000"
.LASF1782:
	.ascii	"IDPFR1_VIRT_SHIFT U(12)\000"
.LASF1902:
	.ascii	"TEE_ALG_GET_KEY_TYPE(algo,with_private_key) __tee_a"
	.ascii	"lg_get_key_type(algo, with_private_key)\000"
.LASF418:
	.ascii	"__THUMBEL__ 1\000"
.LASF1249:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTADD 0x00001901\000"
.LASF599:
	.ascii	"CFG_TA_ASLR_MAX_OFFSET_PAGES 128\000"
.LASF1095:
	.ascii	"TEE_ATTR_RSA_EXPONENT2 0xC0000730\000"
.LASF1264:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCOMPUTEFMM 0x00001C01\000"
.LASF886:
	.ascii	"_GCC_MAX_ALIGN_T \000"
.LASF1823:
	.ascii	"UTEE_DEFINES_H \000"
.LASF335:
	.ascii	"__TQ_IBIT__ 0\000"
.LASF1416:
	.ascii	"SIMPLEQ_REMOVE_HEAD(head,field) do { if (((head)->s"
	.ascii	"qh_first = (head)->sqh_first->field.sqe_next) == NU"
	.ascii	"LL) (head)->sqh_last = &(head)->sqh_first; } while "
	.ascii	"( 0)\000"
.LASF1771:
	.ascii	"TTBCR_SHX_NSH U(0x0)\000"
.LASF239:
	.ascii	"__FLT32X_MIN__ 2.2250738585072014e-308F32x\000"
.LASF1964:
	.ascii	"TA_RAM_START ROUNDUP(TZDRAM_BASE + TEE_RAM_VA_SIZE,"
	.ascii	" SMALL_PAGE_SIZE)\000"
.LASF2168:
	.ascii	"phys_ddr_overall_compat_begin SCATTERED_ARRAY_BEGIN"
	.ascii	"(phys_ddr_overall_compat, struct core_mmu_phys_mem)"
	.ascii	"\000"
.LASF685:
	.ascii	"__compiler_compare_and_swap(p,oval,nval) __atomic_c"
	.ascii	"ompare_exchange_n((p), (oval), (nval), true, __ATOM"
	.ascii	"IC_ACQUIRE, __ATOMIC_RELAXED)\000"
.LASF22:
	.ascii	"__SIZEOF_FLOAT__ 4\000"
.LASF500:
	.ascii	"CFG_DEBUG_INFO 1\000"
.LASF1194:
	.ascii	"TEE_PANIC_ID_TEE_FREEPERSISTENTOBJECTENUMERATOR 0x0"
	.ascii	"0000A02\000"
.LASF846:
	.ascii	"__SIZE_T__ \000"
.LASF36:
	.ascii	"__SIZE_TYPE__ unsigned int\000"
.LASF2150:
	.ascii	"SMALL_PAGE_MASK ((paddr_t)SMALL_PAGE_SIZE - 1)\000"
.LASF57:
	.ascii	"__UINT_LEAST8_TYPE__ unsigned char\000"
.LASF1426:
	.ascii	"TAILQ_ENTRY(type) _TAILQ_ENTRY(struct type,)\000"
.LASF931:
	.ascii	"TEE_LOGIN_GROUP 0x00000002\000"
.LASF2129:
	.ascii	"STM32MP1_CHIP_ID U(0x500)\000"
.LASF1220:
	.ascii	"TEE_PANIC_ID_TEE_AEENCRYPTFINAL 0x00001002\000"
.LASF1185:
	.ascii	"TEE_PANIC_ID_TEE_POPULATETRANSIENTOBJECT 0x00000807"
	.ascii	"\000"
.LASF283:
	.ascii	"__ULLFRACT_MIN__ 0.0ULLR\000"
.LASF800:
	.ascii	"PRIdPTR __PRIPTR_PREFIX \"d\"\000"
.LASF1808:
	.ascii	"__KERNEL_THREAD_PRIVATE_ARCH_H \000"
.LASF42:
	.ascii	"__CHAR16_TYPE__ short unsigned int\000"
.LASF1901:
	.ascii	"TEE_ALG_GET_INTERNAL_HASH(algo) (((algo) >> 20) & 0"
	.ascii	"x7)\000"
.LASF926:
	.ascii	"TEE_PARAM_TYPE_MEMREF_INPUT 5\000"
.LASF514:
	.ascii	"CFG_EMBED_DTB_SOURCE_FILE stm32mp157c-ev1.dts\000"
.LASF1062:
	.ascii	"TEE_TYPE_HMAC_SHA256 0xA0000004\000"
.LASF261:
	.ascii	"__UFRACT_FBIT__ 16\000"
.LASF973:
	.ascii	"TEE_OPERATION_KEY_DERIVATION 8\000"
.LASF808:
	.ascii	"PRIo32 \"o\"\000"
.LASF825:
	.ascii	"PRIXPTR __PRIPTR_PREFIX \"X\"\000"
.LASF919:
	.ascii	"TEE_ERROR_SIGNATURE_INVALID 0xFFFF3072\000"
.LASF664:
	.ascii	"__data __section(\".data\")\000"
.LASF1684:
	.ascii	"CPSR_MODE_ABT ARM32_CPSR_MODE_ABT\000"
.LASF1439:
	.ascii	"TAILQ_FOREACH_SAFE(var,head,field,next) for ((var) "
	.ascii	"= ((head)->tqh_first); (var) != NULL && ((next) = T"
	.ascii	"AILQ_NEXT(var, field), 1); (var) = (next))\000"
.LASF399:
	.ascii	"__ARM_FEATURE_MVE\000"
.LASF209:
	.ascii	"__FLT32_DENORM_MIN__ 1.4012984643248171e-45F32\000"
.LASF728:
	.ascii	"USHRT_MAX (SHRT_MAX * 2 + 1)\000"
.LASF507:
	.ascii	"CFG_DRIVERS_CLK_FIXED 1\000"
.LASF499:
	.ascii	"CFG_DDR_LOWPOWER 1\000"
.LASF1786:
	.ascii	"CNTCR_OFFSET 0x000\000"
.LASF1579:
	.ascii	"UTEE_TYPES_H \000"
.LASF596:
	.ascii	"CFG_STPMIC1 1\000"
.LASF953:
	.ascii	"TEE_DATA_FLAG_OVERWRITE 0x00000400\000"
.LASF1311:
	.ascii	"OUTMSG(...) FMSG(\"< \" __VA_ARGS__)\000"
.LASF355:
	.ascii	"__UHA_IBIT__ 8\000"
.LASF1003:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_MD5SHA1 0x7000F830\000"
.LASF279:
	.ascii	"__LLFRACT_MAX__ 0X7FFFFFFFFFFFFFFFP-63LLR\000"
.LASF630:
	.ascii	"CFG_WITH_STATS 1\000"
.LASF1357:
	.ascii	"TEE_TA_MANAGER_H \000"
.LASF1537:
	.ascii	"TEE_MATTR_UR BIT(7)\000"
.LASF1722:
	.ascii	"SCTLR_EE BIT32(25)\000"
.LASF2188:
	.ascii	"short int\000"
.LASF690:
	.ascii	"KEEP_H \000"
.LASF1640:
	.ascii	"CSSELR_LEVEL_SHIFT U(1)\000"
.LASF6:
	.ascii	"__GNUC_MINOR__ 3\000"
.LASF2:
	.ascii	"__STDC_UTF_16__ 1\000"
.LASF1287:
	.ascii	"va_copy(d,s) __builtin_va_copy(d,s)\000"
.LASF1963:
	.ascii	"TEE_RAM_PH_SIZE TEE_RAM_VA_SIZE\000"
.LASF1088:
	.ascii	"TEE_ATTR_SECRET_VALUE 0xC0000000\000"
.LASF1146:
	.ascii	"TEE_PANIC_ID_TEE_GETNEXTPROPERTY 0x00000203\000"
.LASF40:
	.ascii	"__INTMAX_TYPE__ long long int\000"
.LASF2050:
	.ascii	"UART3_BASE 0x4000f000\000"
.LASF58:
	.ascii	"__UINT_LEAST16_TYPE__ short unsigned int\000"
.LASF285:
	.ascii	"__ULLFRACT_EPSILON__ 0x1P-64ULLR\000"
.LASF1880:
	.ascii	"TEE_MAIN_ALGO_SM2_KEP 0x46\000"
.LASF786:
	.ascii	"UINT16_C(v) v\000"
.LASF2079:
	.ascii	"TARGET_CPUS_GIC_MASK GENMASK_32(CFG_TEE_CORE_NB_COR"
	.ascii	"E - 1, 0)\000"
.LASF997:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_MD5 0x70001830\000"
.LASF1742:
	.ascii	"CPACR_CP_ACCESS_FULL U(0x3)\000"
.LASF1567:
	.ascii	"TA_FLAG_CACHE_MAINTENANCE (1 << 7)\000"
.LASF394:
	.ascii	"__ARM_FEATURE_QRDMX\000"
.LASF2030:
	.ascii	"SYSRAM_BASE 0x2ffc0000\000"
.LASF2043:
	.ascii	"TIM15_BASE 0x44006000\000"
.LASF1926:
	.ascii	"TEE_U32_FROM_BIG_ENDIAN(x) TEE_U32_BSWAP(x)\000"
.LASF1224:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICDECRYPT 0x00001101\000"
.LASF1083:
	.ascii	"TEE_TYPE_GENERIC_SECRET 0xA0000000\000"
.LASF129:
	.ascii	"__INT_FAST8_WIDTH__ 8\000"
.LASF1054:
	.ascii	"TEE_ALG_ILLEGAL_VALUE 0xEFFFFFFF\000"
.LASF160:
	.ascii	"__FLT_DENORM_MIN__ 1.4012984643248171e-45F\000"
.LASF1995:
	.ascii	"ETH1_BASE 0x5800A000\000"
.LASF1971:
	.ascii	"APB2_BASE 0x44000000\000"
.LASF1140:
	.ascii	"TEE_PANIC_ID_TA_CREATEENTRYPOINT 0x00000102\000"
.LASF1446:
	.ascii	"TAILQ_LAST(head,headname) (*(((struct headname *)(("
	.ascii	"head)->tqh_last))->tqh_last))\000"
.LASF1280:
	.ascii	"_STDARG_H \000"
.LASF1760:
	.ascii	"TTBCR_SH0_SHIFT U(12)\000"
.LASF1392:
	.ascii	"SLIST_NEXT(elm,field) ((elm)->field.sle_next)\000"
.LASF1749:
	.ascii	"PAR_LPAE BIT32(11)\000"
.LASF487:
	.ascii	"CFG_CORE_THREAD_SHIFT 0\000"
.LASF1779:
	.ascii	"FSR_WNR BIT32(11)\000"
.LASF843:
	.ascii	"__DEFINED_ptrdiff_t \000"
.LASF120:
	.ascii	"__UINT_LEAST8_MAX__ 0xff\000"
.LASF1225:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICENCRYPT 0x00001102\000"
.LASF1519:
	.ascii	"BIT64(nr) (UINT64_C(1) << (nr))\000"
.LASF1634:
	.ascii	"MPIDR_CLUSTER_SHIFT MPIDR_AFF1_SHIFT\000"
.LASF2134:
	.ascii	"PLAT_MAX_OPP_NB 2U\000"
.LASF1528:
	.ascii	"CMP_TRILEAN(a,b) (__extension__({ __typeof__(a) _a "
	.ascii	"= (a); __typeof__(b) _b = (b); _a > _b ? 1 : _a < _"
	.ascii	"b ? -1 : 0; }))\000"
.LASF1717:
	.ascii	"SCTLR_WXN BIT32(19)\000"
.LASF1804:
	.ascii	"THREAD_RPC_MAX_NUM_PARAMS U(4)\000"
.LASF488:
	.ascii	"CFG_CORE_TZSRAM_EMUL_SIZE 458752\000"
.LASF1067:
	.ascii	"TEE_TYPE_RSA_KEYPAIR 0xA1000030\000"
.LASF7:
	.ascii	"__GNUC_PATCHLEVEL__ 0\000"
.LASF1487:
	.ascii	"tee_mtime_report() do { } while (0)\000"
.LASF1744:
	.ascii	"DACR_DOMAIN_PERM_NO_ACCESS U(0x0)\000"
.LASF655:
	.ascii	"__printf(a,b) __attribute__((format(printf, a, b)))"
	.ascii	"\000"
	.ident	"GCC: (GNU) 12.3.0"
	.section	.note.GNU-stack,"",%progbits
