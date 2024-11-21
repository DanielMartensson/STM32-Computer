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
	.file	"context_asm_defines.c"
@ GNU C11 (GCC) version 12.3.0 (arm-ostl-linux-gnueabi)
@	compiled by GNU C version 12.3.0, GMP version 6.2.1, MPFR version 4.2.1, MPC version 1.3.1, isl version none
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed: -mcpu=cortex-a7 -mfloat-abi=soft -mthumb -mno-unaligned-access -mtls-dialect=gnu -march=armv7ve -g3 -Os -std=gnu11 -ffunction-sections -fdata-sections -fno-short-enums -fno-common
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c"
	.section	.text.__defines,"ax",%progbits
	.align	1
	.global	__defines
	.syntax unified
	.thumb
	.thumb_func
	.type	__defines, %function
__defines:
.LFB0:
	.loc 1 15 1 view -0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 16 2 view .LVU1
	.syntax unified
@ 16 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_RESUME_PA 0 OFFSET_OF_CTX_STRUCT(resume_pa)"	@
@ 0 "" 2
	.loc 1 16 59 view .LVU2
	.loc 1 17 2 view .LVU3
@ 17 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_TEERAM_BKP_PA 4 OFFSET_OF_CTX_STRUCT(teeram_bkp_pa)"	@
@ 0 "" 2
	.loc 1 17 67 view .LVU4
	.loc 1 18 2 view .LVU5
@ 18 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_CRYP1_BASE 8 OFFSET_OF_CTX_STRUCT(cryp1_base)"	@
@ 0 "" 2
	.loc 1 18 61 view .LVU6
	.loc 1 19 2 view .LVU7
@ 19 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_RCC_BASE 12 OFFSET_OF_CTX_STRUCT(rcc_base)"	@
@ 0 "" 2
	.loc 1 19 57 view .LVU8
	.loc 1 20 2 view .LVU9
@ 20 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_STGEN_BASE 16 OFFSET_OF_CTX_STRUCT(stgen_base)"	@
@ 0 "" 2
	.loc 1 20 61 view .LVU10
	.loc 1 21 2 view .LVU11
@ 21 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_STGEN_CNT 20 OFFSET_OF_CTX_STRUCT(stgen_cnt)"	@
@ 0 "" 2
	.loc 1 21 59 view .LVU12
	.loc 1 22 2 view .LVU13
@ 22 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_CCM_KEY 40 OFFSET_OF_CMM_CTX_STRUCT(key)"	@
@ 0 "" 2
	.loc 1 22 55 view .LVU14
	.loc 1 23 2 view .LVU15
@ 23 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_CCM_CTR1 72 OFFSET_OF_CMM_CTX_STRUCT(ctr1)"	@
@ 0 "" 2
	.loc 1 23 57 view .LVU16
	.loc 1 24 2 view .LVU17
@ 24 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_CCM_B0 88 OFFSET_OF_CMM_CTX_STRUCT(b0)"	@
@ 0 "" 2
	.loc 1 24 53 view .LVU18
	.loc 1 25 2 view .LVU19
@ 25 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_CCM_CTR0 104 OFFSET_OF_CMM_CTX_STRUCT(ctr0)"	@
@ 0 "" 2
	.loc 1 25 57 view .LVU20
	.loc 1 26 2 view .LVU21
@ 26 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_CCM_TAG 120 OFFSET_OF_CMM_CTX_STRUCT(tag)"	@
@ 0 "" 2
	.loc 1 26 55 view .LVU22
	.loc 1 27 2 view .LVU23
@ 27 "core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c" 1
	
.ascii "==>PM_CTX_CCM_REF_TAG 24 OFFSET_OF_CTX_STRUCT(ccm_ref_tag)"	@
@ 0 "" 2
	.loc 1 27 63 view .LVU24
@ core/arch/arm/plat-stm32mp1/pm/context_asm_defines.c:28: }
	.loc 1 28 1 is_stmt 0 view .LVU25
	.thumb
	.syntax unified
	bx	lr	@
	.cfi_endproc
.LFE0:
	.size	__defines, .-__defines
	.text
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x8d
	.2byte	0x5
	.byte	0x1
	.byte	0x4
	.4byte	.Ldebug_abbrev0
	.uleb128 0x2
	.4byte	.LASF1278
	.byte	0x1d
	.4byte	.LASF1279
	.4byte	.LASF1280
	.4byte	.LLRL0
	.4byte	0
	.4byte	.Ldebug_line0
	.4byte	.Ldebug_macro0
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.4byte	.LASF1267
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.4byte	.LASF1268
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF1269
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1270
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.4byte	.LASF1271
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF1272
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF1273
	.uleb128 0x1
	.byte	0x4
	.byte	0x5
	.4byte	.LASF1274
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF1275
	.uleb128 0x1
	.byte	0x8
	.byte	0x4
	.4byte	.LASF1276
	.uleb128 0x1
	.byte	0x1
	.byte	0x2
	.4byte	.LASF1277
	.uleb128 0x4
	.4byte	.LASF1281
	.byte	0x1
	.byte	0xe
	.byte	0x1
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
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
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
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
	.4byte	.LFB0
	.uleb128 .LFE0-.LFB0
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
	.file 2 "/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmware/STM32-Computer-Firmware/CA7/optee-os-3.19.0-r2/optee-os-stm32mp-3.19.0-stm32mp-r2/../build/stm32mp157a-dk1/include/generated/conf.h"
	.byte	0x3
	.uleb128 0
	.uleb128 0x2
	.byte	0x7
	.4byte	.Ldebug_macro3
	.byte	0x4
	.file 3 "core/include/gen-asm-defines.h"
	.byte	0x3
	.uleb128 0x7
	.uleb128 0x3
	.byte	0x7
	.4byte	.Ldebug_macro4
	.byte	0x4
	.file 4 "core/arch/arm/plat-stm32mp1/pm/context.h"
	.byte	0x3
	.uleb128 0x9
	.uleb128 0x4
	.byte	0x5
	.uleb128 0x7
	.4byte	.LASF645
	.file 5 "lib/libutils/ext/include/compiler.h"
	.byte	0x3
	.uleb128 0xa
	.uleb128 0x5
	.byte	0x7
	.4byte	.Ldebug_macro5
	.byte	0x4
	.file 6 "/opt/STMicroelectronics/STM32MP1/SDK/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stdbool.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0x6
	.byte	0x7
	.4byte	.Ldebug_macro6
	.byte	0x4
	.file 7 "lib/libutils/isoc/include/stdint.h"
	.byte	0x3
	.uleb128 0xc
	.uleb128 0x7
	.file 8 "lib/libutils/isoc/include/limits.h"
	.byte	0x3
	.uleb128 0x6
	.uleb128 0x8
	.byte	0x7
	.4byte	.Ldebug_macro7
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro8
	.byte	0x4
	.file 9 "/opt/STMicroelectronics/STM32MP1/SDK/4.2.4-openstlinux-6.1-yocto-mickledore-mpu-v24.06.26/sysroots/x86_64-ostl_sdk-linux/usr/lib/arm-ostl-linux-gnueabi/gcc/arm-ostl-linux-gnueabi/12.3.0/include/stddef.h"
	.byte	0x3
	.uleb128 0xd
	.uleb128 0x9
	.byte	0x7
	.4byte	.Ldebug_macro9
	.byte	0x4
	.file 10 "core/arch/arm/plat-stm32mp1/./stm32mp_pm.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0xa
	.byte	0x7
	.4byte	.Ldebug_macro10
	.byte	0x4
	.file 11 "lib/libutee/include/tee_api_types.h"
	.byte	0x3
	.uleb128 0xf
	.uleb128 0xb
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF834
	.file 12 "lib/libutils/isoc/include/inttypes.h"
	.byte	0x3
	.uleb128 0xb
	.uleb128 0xc
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF835
	.byte	0x3
	.uleb128 0x8
	.uleb128 0x7
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro11
	.byte	0x4
	.byte	0x3
	.uleb128 0xd
	.uleb128 0x9
	.byte	0x4
	.file 13 "lib/libutee/include/tee_api_defines.h"
	.byte	0x3
	.uleb128 0xe
	.uleb128 0xd
	.byte	0x7
	.4byte	.Ldebug_macro12
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro13
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro14
	.byte	0x4
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF1265
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF1266
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
	.section	.debug_macro,"G",%progbits,wm4.conf.h.2.b79c4829439b8b8353eecabc46fb25ee,comdat
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
	.4byte	.LASF646
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF647
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF648
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF649
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF650
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF651
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF652
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF653
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF654
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF655
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF656
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF657
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF658
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF659
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF660
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF661
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF662
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF663
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF664
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF665
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF666
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF667
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF668
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF669
	.byte	0x5
	.uleb128 0x49
	.4byte	.LASF670
	.byte	0x5
	.uleb128 0x4a
	.4byte	.LASF671
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF672
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF673
	.byte	0x5
	.uleb128 0x4f
	.4byte	.LASF674
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF675
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF676
	.byte	0x5
	.uleb128 0x53
	.4byte	.LASF677
	.byte	0x5
	.uleb128 0x57
	.4byte	.LASF678
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF679
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF680
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF681
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF682
	.byte	0x5
	.uleb128 0x102
	.4byte	.LASF683
	.byte	0x5
	.uleb128 0x106
	.4byte	.LASF684
	.byte	0x5
	.uleb128 0x107
	.4byte	.LASF685
	.byte	0x5
	.uleb128 0x10a
	.4byte	.LASF686
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF687
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdbool.h.29.4a1c88fe569adb8d03217dd16982ca34,comdat
.Ldebug_macro6:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF688
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF689
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF690
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF691
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF692
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.limits.h.6.760e6357f019fe24b3c4097a4598e4c4,comdat
.Ldebug_macro7:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF693
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF694
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF695
	.byte	0x5
	.uleb128 0xc
	.4byte	.LASF696
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF697
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF698
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF699
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF700
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF701
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF702
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF703
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF704
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF705
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF706
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF707
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF708
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF709
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF710
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF711
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF712
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdint.h.14.746b19174aad304c7dcbde03ee5fd75a,comdat
.Ldebug_macro8:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xe
	.4byte	.LASF713
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF714
	.byte	0x5
	.uleb128 0x3a
	.4byte	.LASF715
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF716
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF717
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF718
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF719
	.byte	0x5
	.uleb128 0x6c
	.4byte	.LASF720
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF721
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF722
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF723
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF724
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF725
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF726
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF727
	.byte	0x5
	.uleb128 0x78
	.4byte	.LASF728
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF729
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF730
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF731
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF732
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF733
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF734
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF735
	.byte	0x5
	.uleb128 0x82
	.4byte	.LASF736
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF737
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF738
	.byte	0x5
	.uleb128 0x85
	.4byte	.LASF739
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF740
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF741
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF742
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF743
	.byte	0x5
	.uleb128 0x8e
	.4byte	.LASF744
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF745
	.byte	0x5
	.uleb128 0x90
	.4byte	.LASF746
	.byte	0x5
	.uleb128 0x91
	.4byte	.LASF747
	.byte	0x5
	.uleb128 0x93
	.4byte	.LASF748
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF749
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF750
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF751
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF752
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF753
	.byte	0x5
	.uleb128 0x9c
	.4byte	.LASF754
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF755
	.byte	0x5
	.uleb128 0xa0
	.4byte	.LASF756
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF757
	.byte	0x5
	.uleb128 0xa4
	.4byte	.LASF758
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF759
	.byte	0x5
	.uleb128 0xb2
	.4byte	.LASF760
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF761
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF762
	.byte	0x5
	.uleb128 0xb5
	.4byte	.LASF763
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF764
	.byte	0x5
	.uleb128 0xbb
	.4byte	.LASF765
	.byte	0x5
	.uleb128 0xbc
	.4byte	.LASF766
	.byte	0x5
	.uleb128 0xbd
	.4byte	.LASF767
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF768
	.byte	0x5
	.uleb128 0xbf
	.4byte	.LASF769
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF770
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF771
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF772
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF773
	.byte	0x5
	.uleb128 0xce
	.4byte	.LASF774
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stddef.h.39.3e0425629195acf083c9561151306b4d,comdat
.Ldebug_macro9:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF775
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF776
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF777
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF778
	.byte	0x5
	.uleb128 0x85
	.4byte	.LASF779
	.byte	0x5
	.uleb128 0x86
	.4byte	.LASF780
	.byte	0x5
	.uleb128 0x87
	.4byte	.LASF781
	.byte	0x5
	.uleb128 0x88
	.4byte	.LASF782
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF783
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF784
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF785
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF786
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF787
	.byte	0x6
	.uleb128 0x9e
	.4byte	.LASF788
	.byte	0x5
	.uleb128 0xb9
	.4byte	.LASF789
	.byte	0x5
	.uleb128 0xba
	.4byte	.LASF790
	.byte	0x5
	.uleb128 0xbb
	.4byte	.LASF791
	.byte	0x5
	.uleb128 0xbc
	.4byte	.LASF792
	.byte	0x5
	.uleb128 0xbd
	.4byte	.LASF793
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF794
	.byte	0x5
	.uleb128 0xbf
	.4byte	.LASF795
	.byte	0x5
	.uleb128 0xc0
	.4byte	.LASF796
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF797
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF798
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF799
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF800
	.byte	0x5
	.uleb128 0xc5
	.4byte	.LASF801
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF802
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF803
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF804
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF805
	.byte	0x5
	.uleb128 0xd0
	.4byte	.LASF806
	.byte	0x6
	.uleb128 0xed
	.4byte	.LASF807
	.byte	0x5
	.uleb128 0x10b
	.4byte	.LASF808
	.byte	0x5
	.uleb128 0x10c
	.4byte	.LASF809
	.byte	0x5
	.uleb128 0x10d
	.4byte	.LASF810
	.byte	0x5
	.uleb128 0x10e
	.4byte	.LASF811
	.byte	0x5
	.uleb128 0x10f
	.4byte	.LASF812
	.byte	0x5
	.uleb128 0x110
	.4byte	.LASF813
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF814
	.byte	0x5
	.uleb128 0x112
	.4byte	.LASF815
	.byte	0x5
	.uleb128 0x113
	.4byte	.LASF816
	.byte	0x5
	.uleb128 0x114
	.4byte	.LASF817
	.byte	0x5
	.uleb128 0x115
	.4byte	.LASF818
	.byte	0x5
	.uleb128 0x116
	.4byte	.LASF819
	.byte	0x5
	.uleb128 0x117
	.4byte	.LASF820
	.byte	0x5
	.uleb128 0x118
	.4byte	.LASF821
	.byte	0x5
	.uleb128 0x119
	.4byte	.LASF822
	.byte	0x5
	.uleb128 0x11a
	.4byte	.LASF823
	.byte	0x6
	.uleb128 0x127
	.4byte	.LASF824
	.byte	0x6
	.uleb128 0x15d
	.4byte	.LASF825
	.byte	0x6
	.uleb128 0x18f
	.4byte	.LASF826
	.byte	0x5
	.uleb128 0x194
	.4byte	.LASF827
	.byte	0x6
	.uleb128 0x19a
	.4byte	.LASF828
	.byte	0x5
	.uleb128 0x19f
	.4byte	.LASF829
	.byte	0x5
	.uleb128 0x1a4
	.4byte	.LASF830
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stm32mp_pm.h.6.36ad7ddda3a260320205ede981f251ef,comdat
.Ldebug_macro10:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF831
	.byte	0x5
	.uleb128 0x8
	.4byte	.LASF832
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF833
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.inttypes.h.11.b36a3d97cd4f39688ec32ad1fdb4feb1,comdat
.Ldebug_macro11:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF836
	.byte	0x5
	.uleb128 0x10
	.4byte	.LASF837
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF838
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF839
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF840
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF841
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF842
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF843
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF844
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF845
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF846
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF847
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF848
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF849
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF850
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF851
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF852
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF853
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF854
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF855
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF856
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF857
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF858
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF859
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF860
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF861
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF862
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF863
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF864
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF865
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF866
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF867
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_api_defines.h.9.ca569bf9e9d05e0bfcb305aaa2de33ca,comdat
.Ldebug_macro12:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x9
	.4byte	.LASF868
	.byte	0x5
	.uleb128 0xb
	.4byte	.LASF869
	.byte	0x5
	.uleb128 0xd
	.4byte	.LASF870
	.byte	0x5
	.uleb128 0xf
	.4byte	.LASF871
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF872
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF873
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF874
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF875
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF876
	.byte	0x5
	.uleb128 0x17
	.4byte	.LASF877
	.byte	0x5
	.uleb128 0x18
	.4byte	.LASF878
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF879
	.byte	0x5
	.uleb128 0x1a
	.4byte	.LASF880
	.byte	0x5
	.uleb128 0x1b
	.4byte	.LASF881
	.byte	0x5
	.uleb128 0x1c
	.4byte	.LASF882
	.byte	0x5
	.uleb128 0x1d
	.4byte	.LASF883
	.byte	0x5
	.uleb128 0x1e
	.4byte	.LASF884
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF885
	.byte	0x5
	.uleb128 0x20
	.4byte	.LASF886
	.byte	0x5
	.uleb128 0x21
	.4byte	.LASF887
	.byte	0x5
	.uleb128 0x22
	.4byte	.LASF888
	.byte	0x5
	.uleb128 0x23
	.4byte	.LASF889
	.byte	0x5
	.uleb128 0x24
	.4byte	.LASF890
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF891
	.byte	0x5
	.uleb128 0x26
	.4byte	.LASF892
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF893
	.byte	0x5
	.uleb128 0x28
	.4byte	.LASF894
	.byte	0x5
	.uleb128 0x29
	.4byte	.LASF895
	.byte	0x5
	.uleb128 0x2a
	.4byte	.LASF896
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF897
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF898
	.byte	0x5
	.uleb128 0x2d
	.4byte	.LASF899
	.byte	0x5
	.uleb128 0x2e
	.4byte	.LASF900
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF901
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF902
	.byte	0x5
	.uleb128 0x33
	.4byte	.LASF903
	.byte	0x5
	.uleb128 0x34
	.4byte	.LASF904
	.byte	0x5
	.uleb128 0x35
	.4byte	.LASF905
	.byte	0x5
	.uleb128 0x36
	.4byte	.LASF906
	.byte	0x5
	.uleb128 0x37
	.4byte	.LASF907
	.byte	0x5
	.uleb128 0x38
	.4byte	.LASF908
	.byte	0x5
	.uleb128 0x39
	.4byte	.LASF909
	.byte	0x5
	.uleb128 0x3c
	.4byte	.LASF910
	.byte	0x5
	.uleb128 0x3d
	.4byte	.LASF911
	.byte	0x5
	.uleb128 0x3e
	.4byte	.LASF912
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF913
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF914
	.byte	0x5
	.uleb128 0x41
	.4byte	.LASF915
	.byte	0x5
	.uleb128 0x42
	.4byte	.LASF916
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF917
	.byte	0x5
	.uleb128 0x46
	.4byte	.LASF918
	.byte	0x5
	.uleb128 0x47
	.4byte	.LASF919
	.byte	0x5
	.uleb128 0x48
	.4byte	.LASF920
	.byte	0x5
	.uleb128 0x4b
	.4byte	.LASF921
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF922
	.byte	0x5
	.uleb128 0x4d
	.4byte	.LASF923
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF924
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF925
	.byte	0x5
	.uleb128 0x52
	.4byte	.LASF926
	.byte	0x5
	.uleb128 0x55
	.4byte	.LASF927
	.byte	0x5
	.uleb128 0x58
	.4byte	.LASF928
	.byte	0x5
	.uleb128 0x5a
	.4byte	.LASF929
	.byte	0x5
	.uleb128 0x5b
	.4byte	.LASF930
	.byte	0x5
	.uleb128 0x5c
	.4byte	.LASF931
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF932
	.byte	0x5
	.uleb128 0x5e
	.4byte	.LASF933
	.byte	0x5
	.uleb128 0x5f
	.4byte	.LASF934
	.byte	0x5
	.uleb128 0x60
	.4byte	.LASF935
	.byte	0x5
	.uleb128 0x61
	.4byte	.LASF936
	.byte	0x5
	.uleb128 0x62
	.4byte	.LASF937
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF938
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF939
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF940
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF941
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF942
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF943
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF944
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF945
	.byte	0x5
	.uleb128 0x6b
	.4byte	.LASF946
	.byte	0x5
	.uleb128 0x6c
	.4byte	.LASF947
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF948
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF949
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF950
	.byte	0x5
	.uleb128 0x70
	.4byte	.LASF951
	.byte	0x5
	.uleb128 0x71
	.4byte	.LASF952
	.byte	0x5
	.uleb128 0x72
	.4byte	.LASF953
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF954
	.byte	0x5
	.uleb128 0x74
	.4byte	.LASF955
	.byte	0x5
	.uleb128 0x75
	.4byte	.LASF956
	.byte	0x5
	.uleb128 0x78
	.4byte	.LASF957
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF958
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF959
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF960
	.byte	0x5
	.uleb128 0x7c
	.4byte	.LASF961
	.byte	0x5
	.uleb128 0x7d
	.4byte	.LASF962
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF963
	.byte	0x5
	.uleb128 0x7f
	.4byte	.LASF964
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF965
	.byte	0x5
	.uleb128 0x81
	.4byte	.LASF966
	.byte	0x5
	.uleb128 0x82
	.4byte	.LASF967
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF968
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF969
	.byte	0x5
	.uleb128 0x85
	.4byte	.LASF970
	.byte	0x5
	.uleb128 0x86
	.4byte	.LASF971
	.byte	0x5
	.uleb128 0x87
	.4byte	.LASF972
	.byte	0x5
	.uleb128 0x88
	.4byte	.LASF973
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF974
	.byte	0x5
	.uleb128 0x8a
	.4byte	.LASF975
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF976
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF977
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF978
	.byte	0x5
	.uleb128 0x8e
	.4byte	.LASF979
	.byte	0x5
	.uleb128 0x8f
	.4byte	.LASF980
	.byte	0x5
	.uleb128 0x90
	.4byte	.LASF981
	.byte	0x5
	.uleb128 0x91
	.4byte	.LASF982
	.byte	0x5
	.uleb128 0x92
	.4byte	.LASF983
	.byte	0x5
	.uleb128 0x93
	.4byte	.LASF984
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF985
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF986
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF987
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF988
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF989
	.byte	0x5
	.uleb128 0x99
	.4byte	.LASF990
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF991
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF992
	.byte	0x5
	.uleb128 0x9c
	.4byte	.LASF993
	.byte	0x5
	.uleb128 0x9d
	.4byte	.LASF994
	.byte	0x5
	.uleb128 0x9e
	.4byte	.LASF995
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF996
	.byte	0x5
	.uleb128 0xa0
	.4byte	.LASF997
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF998
	.byte	0x5
	.uleb128 0xa2
	.4byte	.LASF999
	.byte	0x5
	.uleb128 0xa3
	.4byte	.LASF1000
	.byte	0x5
	.uleb128 0xa4
	.4byte	.LASF1001
	.byte	0x5
	.uleb128 0xa5
	.4byte	.LASF1002
	.byte	0x5
	.uleb128 0xa6
	.4byte	.LASF1003
	.byte	0x5
	.uleb128 0xa7
	.4byte	.LASF1004
	.byte	0x5
	.uleb128 0xa8
	.4byte	.LASF1005
	.byte	0x5
	.uleb128 0xa9
	.4byte	.LASF1006
	.byte	0x5
	.uleb128 0xaa
	.4byte	.LASF1007
	.byte	0x5
	.uleb128 0xab
	.4byte	.LASF1008
	.byte	0x5
	.uleb128 0xac
	.4byte	.LASF1009
	.byte	0x5
	.uleb128 0xad
	.4byte	.LASF1010
	.byte	0x5
	.uleb128 0xae
	.4byte	.LASF1011
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF1012
	.byte	0x5
	.uleb128 0xb0
	.4byte	.LASF1013
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF1014
	.byte	0x5
	.uleb128 0xb2
	.4byte	.LASF1015
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF1016
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF1017
	.byte	0x5
	.uleb128 0xb5
	.4byte	.LASF1018
	.byte	0x5
	.uleb128 0xb6
	.4byte	.LASF1019
	.byte	0x5
	.uleb128 0xb7
	.4byte	.LASF1020
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF1021
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF1022
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF1023
	.byte	0x5
	.uleb128 0xc5
	.4byte	.LASF1024
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF1025
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF1026
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF1027
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF1028
	.byte	0x5
	.uleb128 0xca
	.4byte	.LASF1029
	.byte	0x5
	.uleb128 0xcb
	.4byte	.LASF1030
	.byte	0x5
	.uleb128 0xcc
	.4byte	.LASF1031
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF1032
	.byte	0x5
	.uleb128 0xce
	.4byte	.LASF1033
	.byte	0x5
	.uleb128 0xcf
	.4byte	.LASF1034
	.byte	0x5
	.uleb128 0xd0
	.4byte	.LASF1035
	.byte	0x5
	.uleb128 0xd4
	.4byte	.LASF1036
	.byte	0x5
	.uleb128 0xd5
	.4byte	.LASF1037
	.byte	0x5
	.uleb128 0xd6
	.4byte	.LASF1038
	.byte	0x5
	.uleb128 0xd7
	.4byte	.LASF1039
	.byte	0x5
	.uleb128 0xd8
	.4byte	.LASF1040
	.byte	0x5
	.uleb128 0xd9
	.4byte	.LASF1041
	.byte	0x5
	.uleb128 0xda
	.4byte	.LASF1042
	.byte	0x5
	.uleb128 0xdb
	.4byte	.LASF1043
	.byte	0x5
	.uleb128 0xdc
	.4byte	.LASF1044
	.byte	0x5
	.uleb128 0xdd
	.4byte	.LASF1045
	.byte	0x5
	.uleb128 0xde
	.4byte	.LASF1046
	.byte	0x5
	.uleb128 0xdf
	.4byte	.LASF1047
	.byte	0x5
	.uleb128 0xe0
	.4byte	.LASF1048
	.byte	0x5
	.uleb128 0xe1
	.4byte	.LASF1049
	.byte	0x5
	.uleb128 0xe2
	.4byte	.LASF1050
	.byte	0x5
	.uleb128 0xe3
	.4byte	.LASF1051
	.byte	0x5
	.uleb128 0xe4
	.4byte	.LASF1052
	.byte	0x5
	.uleb128 0xe5
	.4byte	.LASF1053
	.byte	0x5
	.uleb128 0xe6
	.4byte	.LASF1054
	.byte	0x5
	.uleb128 0xe7
	.4byte	.LASF1055
	.byte	0x5
	.uleb128 0xe8
	.4byte	.LASF1056
	.byte	0x5
	.uleb128 0xe9
	.4byte	.LASF1057
	.byte	0x5
	.uleb128 0xea
	.4byte	.LASF1058
	.byte	0x5
	.uleb128 0xeb
	.4byte	.LASF1059
	.byte	0x5
	.uleb128 0xec
	.4byte	.LASF1060
	.byte	0x5
	.uleb128 0xed
	.4byte	.LASF1061
	.byte	0x5
	.uleb128 0xee
	.4byte	.LASF1062
	.byte	0x5
	.uleb128 0xef
	.4byte	.LASF1063
	.byte	0x5
	.uleb128 0xf0
	.4byte	.LASF1064
	.byte	0x5
	.uleb128 0xf1
	.4byte	.LASF1065
	.byte	0x5
	.uleb128 0xf2
	.4byte	.LASF1066
	.byte	0x5
	.uleb128 0xf3
	.4byte	.LASF1067
	.byte	0x5
	.uleb128 0xf4
	.4byte	.LASF1068
	.byte	0x5
	.uleb128 0xf8
	.4byte	.LASF1069
	.byte	0x5
	.uleb128 0xf9
	.4byte	.LASF1070
	.byte	0x5
	.uleb128 0xfa
	.4byte	.LASF1071
	.byte	0x5
	.uleb128 0xfb
	.4byte	.LASF1072
	.byte	0x5
	.uleb128 0xfc
	.4byte	.LASF1073
	.byte	0x5
	.uleb128 0xfd
	.4byte	.LASF1074
	.byte	0x5
	.uleb128 0xfe
	.4byte	.LASF1075
	.byte	0x5
	.uleb128 0xff
	.4byte	.LASF1076
	.byte	0x5
	.uleb128 0x100
	.4byte	.LASF1077
	.byte	0x5
	.uleb128 0x101
	.4byte	.LASF1078
	.byte	0x5
	.uleb128 0x102
	.4byte	.LASF1079
	.byte	0x5
	.uleb128 0x103
	.4byte	.LASF1080
	.byte	0x5
	.uleb128 0x104
	.4byte	.LASF1081
	.byte	0x5
	.uleb128 0x105
	.4byte	.LASF1082
	.byte	0x5
	.uleb128 0x106
	.4byte	.LASF1083
	.byte	0x5
	.uleb128 0x107
	.4byte	.LASF1084
	.byte	0x5
	.uleb128 0x108
	.4byte	.LASF1085
	.byte	0x5
	.uleb128 0x109
	.4byte	.LASF1086
	.byte	0x5
	.uleb128 0x10a
	.4byte	.LASF1087
	.byte	0x5
	.uleb128 0x10b
	.4byte	.LASF1088
	.byte	0x5
	.uleb128 0x10c
	.4byte	.LASF1089
	.byte	0x5
	.uleb128 0x10d
	.4byte	.LASF1090
	.byte	0x5
	.uleb128 0x10e
	.4byte	.LASF1091
	.byte	0x5
	.uleb128 0x10f
	.4byte	.LASF1092
	.byte	0x5
	.uleb128 0x110
	.4byte	.LASF1093
	.byte	0x5
	.uleb128 0x111
	.4byte	.LASF1094
	.byte	0x5
	.uleb128 0x112
	.4byte	.LASF1095
	.byte	0x5
	.uleb128 0x113
	.4byte	.LASF1096
	.byte	0x5
	.uleb128 0x114
	.4byte	.LASF1097
	.byte	0x5
	.uleb128 0x115
	.4byte	.LASF1098
	.byte	0x5
	.uleb128 0x116
	.4byte	.LASF1099
	.byte	0x5
	.uleb128 0x117
	.4byte	.LASF1100
	.byte	0x5
	.uleb128 0x118
	.4byte	.LASF1101
	.byte	0x5
	.uleb128 0x119
	.4byte	.LASF1102
	.byte	0x5
	.uleb128 0x11a
	.4byte	.LASF1103
	.byte	0x5
	.uleb128 0x11b
	.4byte	.LASF1104
	.byte	0x5
	.uleb128 0x11c
	.4byte	.LASF1105
	.byte	0x5
	.uleb128 0x11d
	.4byte	.LASF1106
	.byte	0x5
	.uleb128 0x11e
	.4byte	.LASF1107
	.byte	0x5
	.uleb128 0x120
	.4byte	.LASF1108
	.byte	0x5
	.uleb128 0x121
	.4byte	.LASF1109
	.byte	0x5
	.uleb128 0x129
	.4byte	.LASF1110
	.byte	0x5
	.uleb128 0x12a
	.4byte	.LASF1111
	.byte	0x5
	.uleb128 0x12d
	.4byte	.LASF1112
	.byte	0x5
	.uleb128 0x12e
	.4byte	.LASF1113
	.byte	0x5
	.uleb128 0x12f
	.4byte	.LASF1114
	.byte	0x5
	.uleb128 0x130
	.4byte	.LASF1115
	.byte	0x5
	.uleb128 0x131
	.4byte	.LASF1116
	.byte	0x5
	.uleb128 0x132
	.4byte	.LASF1117
	.byte	0x5
	.uleb128 0x133
	.4byte	.LASF1118
	.byte	0x5
	.uleb128 0x134
	.4byte	.LASF1119
	.byte	0x5
	.uleb128 0x138
	.4byte	.LASF1120
	.byte	0x5
	.uleb128 0x139
	.4byte	.LASF1121
	.byte	0x5
	.uleb128 0x13a
	.4byte	.LASF1122
	.byte	0x5
	.uleb128 0x13b
	.4byte	.LASF1123
	.byte	0x5
	.uleb128 0x13c
	.4byte	.LASF1124
	.byte	0x5
	.uleb128 0x13e
	.4byte	.LASF1125
	.byte	0x5
	.uleb128 0x13f
	.4byte	.LASF1126
	.byte	0x5
	.uleb128 0x140
	.4byte	.LASF1127
	.byte	0x5
	.uleb128 0x141
	.4byte	.LASF1128
	.byte	0x5
	.uleb128 0x142
	.4byte	.LASF1129
	.byte	0x5
	.uleb128 0x143
	.4byte	.LASF1130
	.byte	0x5
	.uleb128 0x144
	.4byte	.LASF1131
	.byte	0x5
	.uleb128 0x145
	.4byte	.LASF1132
	.byte	0x5
	.uleb128 0x146
	.4byte	.LASF1133
	.byte	0x5
	.uleb128 0x147
	.4byte	.LASF1134
	.byte	0x5
	.uleb128 0x148
	.4byte	.LASF1135
	.byte	0x5
	.uleb128 0x149
	.4byte	.LASF1136
	.byte	0x5
	.uleb128 0x14b
	.4byte	.LASF1137
	.byte	0x5
	.uleb128 0x14d
	.4byte	.LASF1138
	.byte	0x5
	.uleb128 0x14e
	.4byte	.LASF1139
	.byte	0x5
	.uleb128 0x14f
	.4byte	.LASF1140
	.byte	0x5
	.uleb128 0x151
	.4byte	.LASF1141
	.byte	0x5
	.uleb128 0x152
	.4byte	.LASF1142
	.byte	0x5
	.uleb128 0x153
	.4byte	.LASF1143
	.byte	0x5
	.uleb128 0x155
	.4byte	.LASF1144
	.byte	0x5
	.uleb128 0x156
	.4byte	.LASF1145
	.byte	0x5
	.uleb128 0x157
	.4byte	.LASF1146
	.byte	0x5
	.uleb128 0x158
	.4byte	.LASF1147
	.byte	0x5
	.uleb128 0x159
	.4byte	.LASF1148
	.byte	0x5
	.uleb128 0x15a
	.4byte	.LASF1149
	.byte	0x5
	.uleb128 0x15b
	.4byte	.LASF1150
	.byte	0x5
	.uleb128 0x15c
	.4byte	.LASF1151
	.byte	0x5
	.uleb128 0x15d
	.4byte	.LASF1152
	.byte	0x5
	.uleb128 0x15f
	.4byte	.LASF1153
	.byte	0x5
	.uleb128 0x160
	.4byte	.LASF1154
	.byte	0x5
	.uleb128 0x162
	.4byte	.LASF1155
	.byte	0x5
	.uleb128 0x163
	.4byte	.LASF1156
	.byte	0x5
	.uleb128 0x165
	.4byte	.LASF1157
	.byte	0x5
	.uleb128 0x166
	.4byte	.LASF1158
	.byte	0x5
	.uleb128 0x167
	.4byte	.LASF1159
	.byte	0x5
	.uleb128 0x169
	.4byte	.LASF1160
	.byte	0x5
	.uleb128 0x16b
	.4byte	.LASF1161
	.byte	0x5
	.uleb128 0x16c
	.4byte	.LASF1162
	.byte	0x5
	.uleb128 0x16d
	.4byte	.LASF1163
	.byte	0x5
	.uleb128 0x16e
	.4byte	.LASF1164
	.byte	0x5
	.uleb128 0x16f
	.4byte	.LASF1165
	.byte	0x5
	.uleb128 0x170
	.4byte	.LASF1166
	.byte	0x5
	.uleb128 0x171
	.4byte	.LASF1167
	.byte	0x5
	.uleb128 0x172
	.4byte	.LASF1168
	.byte	0x5
	.uleb128 0x175
	.4byte	.LASF1169
	.byte	0x5
	.uleb128 0x176
	.4byte	.LASF1170
	.byte	0x5
	.uleb128 0x177
	.4byte	.LASF1171
	.byte	0x5
	.uleb128 0x178
	.4byte	.LASF1172
	.byte	0x5
	.uleb128 0x179
	.4byte	.LASF1173
	.byte	0x5
	.uleb128 0x17b
	.4byte	.LASF1174
	.byte	0x5
	.uleb128 0x17c
	.4byte	.LASF1175
	.byte	0x5
	.uleb128 0x17d
	.4byte	.LASF1176
	.byte	0x5
	.uleb128 0x17e
	.4byte	.LASF1177
	.byte	0x5
	.uleb128 0x17f
	.4byte	.LASF1178
	.byte	0x5
	.uleb128 0x181
	.4byte	.LASF1179
	.byte	0x5
	.uleb128 0x182
	.4byte	.LASF1180
	.byte	0x5
	.uleb128 0x183
	.4byte	.LASF1181
	.byte	0x5
	.uleb128 0x184
	.4byte	.LASF1182
	.byte	0x5
	.uleb128 0x186
	.4byte	.LASF1183
	.byte	0x5
	.uleb128 0x187
	.4byte	.LASF1184
	.byte	0x5
	.uleb128 0x188
	.4byte	.LASF1185
	.byte	0x5
	.uleb128 0x189
	.4byte	.LASF1186
	.byte	0x5
	.uleb128 0x18a
	.4byte	.LASF1187
	.byte	0x5
	.uleb128 0x18b
	.4byte	.LASF1188
	.byte	0x5
	.uleb128 0x18c
	.4byte	.LASF1189
	.byte	0x5
	.uleb128 0x18d
	.4byte	.LASF1190
	.byte	0x5
	.uleb128 0x18f
	.4byte	.LASF1191
	.byte	0x5
	.uleb128 0x190
	.4byte	.LASF1192
	.byte	0x5
	.uleb128 0x192
	.4byte	.LASF1193
	.byte	0x5
	.uleb128 0x193
	.4byte	.LASF1194
	.byte	0x5
	.uleb128 0x194
	.4byte	.LASF1195
	.byte	0x5
	.uleb128 0x196
	.4byte	.LASF1196
	.byte	0x5
	.uleb128 0x197
	.4byte	.LASF1197
	.byte	0x5
	.uleb128 0x198
	.4byte	.LASF1198
	.byte	0x5
	.uleb128 0x199
	.4byte	.LASF1199
	.byte	0x5
	.uleb128 0x19b
	.4byte	.LASF1200
	.byte	0x5
	.uleb128 0x19c
	.4byte	.LASF1201
	.byte	0x5
	.uleb128 0x19d
	.4byte	.LASF1202
	.byte	0x5
	.uleb128 0x19e
	.4byte	.LASF1203
	.byte	0x5
	.uleb128 0x19f
	.4byte	.LASF1204
	.byte	0x5
	.uleb128 0x1a1
	.4byte	.LASF1205
	.byte	0x5
	.uleb128 0x1a2
	.4byte	.LASF1206
	.byte	0x5
	.uleb128 0x1a3
	.4byte	.LASF1207
	.byte	0x5
	.uleb128 0x1a4
	.4byte	.LASF1208
	.byte	0x5
	.uleb128 0x1a6
	.4byte	.LASF1209
	.byte	0x5
	.uleb128 0x1a8
	.4byte	.LASF1210
	.byte	0x5
	.uleb128 0x1aa
	.4byte	.LASF1211
	.byte	0x5
	.uleb128 0x1ab
	.4byte	.LASF1212
	.byte	0x5
	.uleb128 0x1ac
	.4byte	.LASF1213
	.byte	0x5
	.uleb128 0x1ad
	.4byte	.LASF1214
	.byte	0x5
	.uleb128 0x1ae
	.4byte	.LASF1215
	.byte	0x5
	.uleb128 0x1b0
	.4byte	.LASF1216
	.byte	0x5
	.uleb128 0x1b1
	.4byte	.LASF1217
	.byte	0x5
	.uleb128 0x1b3
	.4byte	.LASF1218
	.byte	0x5
	.uleb128 0x1b4
	.4byte	.LASF1219
	.byte	0x5
	.uleb128 0x1b5
	.4byte	.LASF1220
	.byte	0x5
	.uleb128 0x1b7
	.4byte	.LASF1221
	.byte	0x5
	.uleb128 0x1b8
	.4byte	.LASF1222
	.byte	0x5
	.uleb128 0x1b9
	.4byte	.LASF1223
	.byte	0x5
	.uleb128 0x1ba
	.4byte	.LASF1224
	.byte	0x5
	.uleb128 0x1bc
	.4byte	.LASF1225
	.byte	0x5
	.uleb128 0x1bd
	.4byte	.LASF1226
	.byte	0x5
	.uleb128 0x1be
	.4byte	.LASF1227
	.byte	0x5
	.uleb128 0x1bf
	.4byte	.LASF1228
	.byte	0x5
	.uleb128 0x1c0
	.4byte	.LASF1229
	.byte	0x5
	.uleb128 0x1c2
	.4byte	.LASF1230
	.byte	0x5
	.uleb128 0x1c3
	.4byte	.LASF1231
	.byte	0x5
	.uleb128 0x1c4
	.4byte	.LASF1232
	.byte	0x5
	.uleb128 0x1c5
	.4byte	.LASF1233
	.byte	0x5
	.uleb128 0x1c6
	.4byte	.LASF1234
	.byte	0x5
	.uleb128 0x1c7
	.4byte	.LASF1235
	.byte	0x5
	.uleb128 0x1c9
	.4byte	.LASF1236
	.byte	0x5
	.uleb128 0x1ca
	.4byte	.LASF1237
	.byte	0x5
	.uleb128 0x1cb
	.4byte	.LASF1238
	.byte	0x5
	.uleb128 0x1cc
	.4byte	.LASF1239
	.byte	0x5
	.uleb128 0x1cd
	.4byte	.LASF1240
	.byte	0x5
	.uleb128 0x1ce
	.4byte	.LASF1241
	.byte	0x5
	.uleb128 0x1d0
	.4byte	.LASF1242
	.byte	0x5
	.uleb128 0x1d1
	.4byte	.LASF1243
	.byte	0x5
	.uleb128 0x1d2
	.4byte	.LASF1244
	.byte	0x5
	.uleb128 0x1d4
	.4byte	.LASF1245
	.byte	0x5
	.uleb128 0x1d5
	.4byte	.LASF1246
	.byte	0x5
	.uleb128 0x1d6
	.4byte	.LASF1247
	.byte	0x5
	.uleb128 0x1e2
	.4byte	.LASF1248
	.byte	0x5
	.uleb128 0x1e9
	.4byte	.LASF1249
	.byte	0x5
	.uleb128 0x1ef
	.4byte	.LASF1250
	.byte	0x5
	.uleb128 0x1f2
	.4byte	.LASF1251
	.byte	0x5
	.uleb128 0x1f6
	.4byte	.LASF1252
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.tee_api_types.h.195.70f88af562e4de06db5cb96bde92609f,comdat
.Ldebug_macro13:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF1253
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF1254
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF1255
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF1256
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF1257
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF1258
	.byte	0x5
	.uleb128 0xcb
	.4byte	.LASF1259
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.context.h.18.d07f2b03542bed2e9b4714af8c01bfb6,comdat
.Ldebug_macro14:
	.2byte	0x5
	.byte	0
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF1260
	.byte	0x5
	.uleb128 0x13
	.4byte	.LASF1261
	.byte	0x5
	.uleb128 0x14
	.4byte	.LASF1262
	.byte	0x5
	.uleb128 0x15
	.4byte	.LASF1263
	.byte	0x5
	.uleb128 0x16
	.4byte	.LASF1264
	.byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF454:
	.ascii	"__linux__ 1\000"
.LASF187:
	.ascii	"__DECIMAL_DIG__ 17\000"
.LASF1134:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYNAME 0x0000020A\000"
.LASF354:
	.ascii	"__UHA_FBIT__ 8\000"
.LASF1161:
	.ascii	"TEE_PANIC_ID_TEE_COPYOBJECTATTRIBUTES 0x00000802\000"
.LASF403:
	.ascii	"__ARM_FEATURE_CLZ 1\000"
.LASF1177:
	.ascii	"TEE_PANIC_ID_TEE_RESETPERSISTENTOBJECTENUMERATOR 0x"
	.ascii	"00000A04\000"
.LASF1009:
	.ascii	"TEE_ALG_SHA3_224 0x50000008\000"
.LASF225:
	.ascii	"__FLT64_DENORM_MIN__ 4.9406564584124654e-324F64\000"
.LASF51:
	.ascii	"__UINT32_TYPE__ unsigned int\000"
.LASF374:
	.ascii	"__GCC_ATOMIC_WCHAR_T_LOCK_FREE 2\000"
.LASF648:
	.ascii	"__packed __attribute__((packed))\000"
.LASF365:
	.ascii	"__CHAR_UNSIGNED__ 1\000"
.LASF704:
	.ascii	"SCHAR_MAX __SCHAR_MAX__\000"
.LASF1013:
	.ascii	"TEE_ALG_MD5SHA1 0x5000000F\000"
.LASF409:
	.ascii	"__ARM_ARCH_PROFILE 65\000"
.LASF318:
	.ascii	"__LLACCUM_MIN__ (-0X1P31LLK-0X1P31LLK)\000"
.LASF380:
	.ascii	"__GCC_DESTRUCTIVE_SIZE 64\000"
.LASF310:
	.ascii	"__LACCUM_EPSILON__ 0x1P-31LK\000"
.LASF81:
	.ascii	"__PTRDIFF_MAX__ 0x7fffffff\000"
.LASF717:
	.ascii	"INT16_MIN (-0x7fff-1)\000"
.LASF578:
	.ascii	"CFG_STM32_FIREWALL 1\000"
.LASF1197:
	.ascii	"TEE_PANIC_ID_TEE_MACCOMPUTEFINAL 0x00000F02\000"
.LASF93:
	.ascii	"__INTMAX_C(c) c ## LL\000"
.LASF92:
	.ascii	"__INTMAX_MAX__ 0x7fffffffffffffffLL\000"
.LASF1033:
	.ascii	"TEE_ALG_SM3 0x50000007\000"
.LASF1188:
	.ascii	"TEE_PANIC_ID_TEE_SETOPERATIONKEY 0x00000C06\000"
.LASF335:
	.ascii	"__TQ_IBIT__ 0\000"
.LASF220:
	.ascii	"__FLT64_DECIMAL_DIG__ 17\000"
.LASF191:
	.ascii	"__LDBL_MIN__ 2.2250738585072014e-308L\000"
.LASF895:
	.ascii	"TEE_ERROR_EXTERNAL_CANCEL 0xFFFF0011\000"
.LASF577:
	.ascii	"CFG_STM32_ETZPC 1\000"
.LASF967:
	.ascii	"TEE_ALG_DES_ECB_NOPAD 0x10000011\000"
.LASF14:
	.ascii	"__ATOMIC_CONSUME 1\000"
.LASF311:
	.ascii	"__ULACCUM_FBIT__ 32\000"
.LASF77:
	.ascii	"__WCHAR_MAX__ 0xffffffffU\000"
.LASF1004:
	.ascii	"TEE_ALG_SHA1 0x50000002\000"
.LASF1252:
	.ascii	"TEE_BigIntSizeInU32(n) ((((n)+31)/32)+2)\000"
.LASF298:
	.ascii	"__ACCUM_MIN__ (-0X1P15K-0X1P15K)\000"
.LASF784:
	.ascii	"___int_ptrdiff_t_h \000"
.LASF1125:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATEPROPERTYENUMERATOR 0x00000"
	.ascii	"201\000"
.LASF500:
	.ascii	"CFG_DEBUG_INFO 1\000"
.LASF898:
	.ascii	"TEE_ERROR_STORAGE_NO_SPACE 0xFFFF3041\000"
.LASF870:
	.ascii	"TEE_HANDLE_NULL 0\000"
.LASF1097:
	.ascii	"TEE_ATTR_SM2_KEP_USER 0xF0000646\000"
.LASF20:
	.ascii	"__SIZEOF_LONG_LONG__ 8\000"
.LASF931:
	.ascii	"TEE_DATA_FLAG_ACCESS_WRITE_META 0x00000004\000"
.LASF170:
	.ascii	"__DBL_MAX_10_EXP__ 308\000"
.LASF1244:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTRELATIVEPRIME 0x00001B03\000"
.LASF258:
	.ascii	"__FRACT_MIN__ (-0.5R-0.5R)\000"
.LASF324:
	.ascii	"__ULLACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULLK\000"
.LASF746:
	.ascii	"INT_FAST32_MAX INT32_MAX\000"
.LASF679:
	.ascii	"__HAVE_SINGLE_ARGUMENT_STATIC_ASSERT 1\000"
.LASF294:
	.ascii	"__USACCUM_MAX__ 0XFFFFP-8UHK\000"
.LASF246:
	.ascii	"__SFRACT_FBIT__ 7\000"
.LASF96:
	.ascii	"__INTMAX_WIDTH__ 64\000"
.LASF448:
	.ascii	"__ARM_FEATURE_BF16_SCALAR_ARITHMETIC\000"
.LASF331:
	.ascii	"__SQ_IBIT__ 0\000"
.LASF30:
	.ascii	"__ORDER_PDP_ENDIAN__ 3412\000"
.LASF36:
	.ascii	"__SIZE_TYPE__ unsigned int\000"
.LASF587:
	.ascii	"CFG_STM32_RNG 1\000"
.LASF505:
	.ascii	"CFG_DRIVERS_CLK_DT 1\000"
.LASF161:
	.ascii	"__FLT_HAS_DENORM__ 1\000"
.LASF45:
	.ascii	"__INT8_TYPE__ signed char\000"
.LASF1027:
	.ascii	"TEE_ALG_ECDH_P192 0x80001042\000"
.LASF293:
	.ascii	"__USACCUM_MIN__ 0.0UHK\000"
.LASF204:
	.ascii	"__FLT32_DECIMAL_DIG__ 9\000"
.LASF848:
	.ascii	"PRIo8 \"o\"\000"
.LASF1246:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTFROMFMM 0x00001C02\000"
.LASF183:
	.ascii	"__LDBL_MIN_EXP__ (-1021)\000"
.LASF181:
	.ascii	"__LDBL_MANT_DIG__ 53\000"
.LASF1139:
	.ascii	"TEE_PANIC_ID_TEE_INVOKETACOMMAND 0x00000402\000"
.LASF659:
	.ascii	"__must_check __attribute__((warn_unused_result))\000"
.LASF121:
	.ascii	"__UINT8_C(c) c\000"
.LASF46:
	.ascii	"__INT16_TYPE__ short int\000"
.LASF455:
	.ascii	"linux 1\000"
.LASF221:
	.ascii	"__FLT64_MAX__ 1.7976931348623157e+308F64\000"
.LASF488:
	.ascii	"CFG_CORE_TZSRAM_EMUL_SIZE 458752\000"
.LASF1115:
	.ascii	"TEE_ECC_CURVE_NIST_P256 0x00000003\000"
.LASF971:
	.ascii	"TEE_ALG_DES3_ECB_NOPAD 0x10000013\000"
.LASF1045:
	.ascii	"TEE_TYPE_HMAC_SHA512 0xA0000006\000"
.LASF850:
	.ascii	"PRIo32 \"o\"\000"
.LASF49:
	.ascii	"__UINT8_TYPE__ unsigned char\000"
.LASF366:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1\000"
.LASF1230:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTADD 0x00001901\000"
.LASF613:
	.ascii	"CFG_TEE_FW_IMPL_VERSION FW_IMPL_UNDEF\000"
.LASF122:
	.ascii	"__UINT_LEAST16_MAX__ 0xffff\000"
.LASF4:
	.ascii	"__STDC_HOSTED__ 1\000"
.LASF281:
	.ascii	"__ULLFRACT_FBIT__ 64\000"
.LASF44:
	.ascii	"__SIG_ATOMIC_TYPE__ int\000"
.LASF904:
	.ascii	"TEE_PARAM_TYPE_VALUE_INPUT 1\000"
.LASF64:
	.ascii	"__INT_FAST64_TYPE__ long long int\000"
.LASF39:
	.ascii	"__WINT_TYPE__ unsigned int\000"
.LASF467:
	.ascii	"CFG_ARM32_core 1\000"
.LASF1075:
	.ascii	"TEE_ATTR_RSA_EXPONENT1 0xC0000630\000"
.LASF790:
	.ascii	"__SIZE_T__ \000"
.LASF1087:
	.ascii	"TEE_ATTR_DH_PUBLIC_VALUE 0xD0000132\000"
.LASF203:
	.ascii	"__FLT32_MAX_10_EXP__ 38\000"
.LASF1143:
	.ascii	"TEE_PANIC_ID_TEE_UNMASKCANCELLATION 0x00000503\000"
.LASF502:
	.ascii	"CFG_DRAM_BASE 0xc0000000\000"
.LASF1124:
	.ascii	"TEE_PANIC_ID_TA_OPENSESSIONENTRYPOINT 0x00000105\000"
.LASF944:
	.ascii	"TEE_HANDLE_FLAG_PERSISTENT 0x00010000\000"
.LASF254:
	.ascii	"__USFRACT_MAX__ 0XFFP-8UHR\000"
.LASF142:
	.ascii	"__UINTPTR_MAX__ 0xffffffffU\000"
.LASF912:
	.ascii	"TEE_LOGIN_GROUP 0x00000002\000"
.LASF825:
	.ascii	"__need_wchar_t\000"
.LASF200:
	.ascii	"__FLT32_MIN_EXP__ (-125)\000"
.LASF600:
	.ascii	"CFG_TA_ASLR_MIN_OFFSET_PAGES 0\000"
.LASF703:
	.ascii	"MB_LEN_MAX 1\000"
.LASF1248:
	.ascii	"TEE_PARAM_TYPES(t0,t1,t2,t3) ((t0) | ((t1) << 4) | "
	.ascii	"((t2) << 8) | ((t3) << 12))\000"
.LASF473:
	.ascii	"CFG_BSEC_PTA 1\000"
.LASF549:
	.ascii	"CFG_RPMB_FS_RD_ENTRIES 8\000"
.LASF783:
	.ascii	"_BSD_PTRDIFF_T_ \000"
.LASF712:
	.ascii	"ULLONG_MAX (LLONG_MAX * 2ULL + 1ULL)\000"
.LASF13:
	.ascii	"__ATOMIC_ACQ_REL 4\000"
.LASF271:
	.ascii	"__ULFRACT_FBIT__ 32\000"
.LASF566:
	.ascii	"CFG_STM32MP1_PM_CONTEXT_VERSION 3\000"
.LASF217:
	.ascii	"__FLT64_MIN_10_EXP__ (-307)\000"
.LASF34:
	.ascii	"__GNUC_EXECUTION_CHARSET_NAME \"UTF-8\"\000"
.LASF1057:
	.ascii	"TEE_TYPE_ED25519_KEYPAIR 0xA1000043\000"
.LASF135:
	.ascii	"__INT_FAST64_WIDTH__ 64\000"
.LASF533:
	.ascii	"CFG_MMAP_REGIONS 30\000"
.LASF1165:
	.ascii	"TEE_PANIC_ID_TEE_INITVALUEATTRIBUTE 0x00000806\000"
.LASF538:
	.ascii	"CFG_OS_REV_REPORTS_GIT_SHA1 1\000"
.LASF707:
	.ascii	"SHRT_MIN (-SHRT_MAX - 1)\000"
.LASF1245:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCOMPUTEFMM 0x00001C01\000"
.LASF558:
	.ascii	"CFG_SM_NO_CYCLE_COUNTING 1\000"
.LASF250:
	.ascii	"__SFRACT_EPSILON__ 0x1P-7HR\000"
.LASF545:
	.ascii	"CFG_REGULATOR_FIXED 1\000"
.LASF794:
	.ascii	"_T_SIZE \000"
.LASF548:
	.ascii	"CFG_RPMB_FS_DEV_ID 0\000"
.LASF29:
	.ascii	"__ORDER_BIG_ENDIAN__ 4321\000"
.LASF480:
	.ascii	"CFG_CORE_HAS_GENERIC_TIMER 1\000"
.LASF608:
	.ascii	"CFG_TEE_API_VERSION GPD-1.1-dev\000"
.LASF750:
	.ascii	"UINT_FAST32_MAX UINT32_MAX\000"
.LASF693:
	.ascii	"__LIMITS_H \000"
.LASF770:
	.ascii	"INT64_C(v) LL(v)\000"
.LASF709:
	.ascii	"USHRT_MAX (SHRT_MAX * 2 + 1)\000"
.LASF787:
	.ascii	"__DEFINED_ptrdiff_t \000"
.LASF734:
	.ascii	"INT_LEAST32_MAX INT32_MAX\000"
.LASF523:
	.ascii	"CFG_HWRNG_RATE 0\000"
.LASF216:
	.ascii	"__FLT64_MIN_EXP__ (-1021)\000"
.LASF535:
	.ascii	"CFG_NUM_THREADS 10\000"
.LASF90:
	.ascii	"__PTRDIFF_WIDTH__ 32\000"
.LASF644:
	.ascii	"DEFINE(def,val) asm volatile(\"\\n.ascii \\\"==>\" "
	.ascii	"#def \" %c0 \" #val \"\\\"\" : : \"i\" (val));\000"
.LASF522:
	.ascii	"CFG_HWRNG_QUALITY 1024\000"
.LASF1103:
	.ascii	"TEE_ATTR_ED25519_PUBLIC_VALUE 0xD0000743\000"
.LASF1142:
	.ascii	"TEE_PANIC_ID_TEE_MASKCANCELLATION 0x00000502\000"
.LASF922:
	.ascii	"TEE_PROPSET_CURRENT_CLIENT (TEE_PropSetHandle)0xFFF"
	.ascii	"FFFFE\000"
.LASF1130:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASIDENTITY 0x00000206\000"
.LASF826:
	.ascii	"NULL\000"
.LASF1096:
	.ascii	"TEE_ATTR_SM2_ID_RESPONDER 0xD0000546\000"
.LASF307:
	.ascii	"__LACCUM_IBIT__ 32\000"
.LASF799:
	.ascii	"_SIZE_T_DEFINED \000"
.LASF131:
	.ascii	"__INT_FAST16_WIDTH__ 32\000"
.LASF510:
	.ascii	"CFG_DT 1\000"
.LASF1039:
	.ascii	"TEE_TYPE_SM4 0xA0000014\000"
.LASF423:
	.ascii	"__VFP_FP__ 1\000"
.LASF511:
	.ascii	"CFG_DTB_MAX_SIZE (256 * 1024)\000"
.LASF278:
	.ascii	"__LLFRACT_MIN__ (-0.5LLR-0.5LLR)\000"
.LASF493:
	.ascii	"CFG_COUNTER_DRIVER 1\000"
.LASF1190:
	.ascii	"TEE_PANIC_ID_TEE_GETOPERATIONINFOMULTIPLE 0x00000C0"
	.ascii	"8\000"
.LASF963:
	.ascii	"TEE_ALG_AES_CBC_MAC_PKCS5 0x30000510\000"
.LASF575:
	.ascii	"CFG_STM32_CPU_OPP 1\000"
.LASF509:
	.ascii	"CFG_DRIVERS_RSTCTRL 1\000"
.LASF140:
	.ascii	"__INTPTR_MAX__ 0x7fffffff\000"
.LASF1151:
	.ascii	"TEE_PANIC_ID_TEE_REALLOC 0x00000608\000"
.LASF722:
	.ascii	"INT32_MAX 0x7fffffff\000"
.LASF986:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA224 0x70313930\000"
.LASF838:
	.ascii	"PRId8 \"d\"\000"
.LASF263:
	.ascii	"__UFRACT_MIN__ 0.0UR\000"
.LASF136:
	.ascii	"__UINT_FAST8_MAX__ 0xff\000"
.LASF669:
	.ascii	"__relrodata_unpaged(x) __rodata_unpaged(x)\000"
.LASF1162:
	.ascii	"TEE_PANIC_ID_TEE_FREETRANSIENTOBJECT 0x00000803\000"
.LASF243:
	.ascii	"__FLT32X_HAS_INFINITY__ 1\000"
.LASF792:
	.ascii	"_SYS_SIZE_T_H \000"
.LASF1088:
	.ascii	"TEE_ATTR_DH_PRIVATE_VALUE 0xC0000232\000"
.LASF602:
	.ascii	"CFG_TA_BIGNUM_MAX_BITS 2048\000"
.LASF57:
	.ascii	"__UINT_LEAST8_TYPE__ unsigned char\000"
.LASF681:
	.ascii	"__compiler_sub_overflow(a,b,res) __builtin_sub_over"
	.ascii	"flow((a), (b), (res))\000"
.LASF1170:
	.ascii	"TEE_PANIC_ID_TEE_CREATEPERSISTENTOBJECT 0x00000902\000"
.LASF668:
	.ascii	"__rodata_unpaged(x) __section(\".rodata.__unpaged.\""
	.ascii	" x __SECTION_FLAGS_RODATA)\000"
.LASF1279:
	.ascii	"core/arch/arm/plat-stm32mp1/pm/context_asm_defines."
	.ascii	"c\000"
.LASF302:
	.ascii	"__UACCUM_IBIT__ 16\000"
.LASF1274:
	.ascii	"long int\000"
.LASF867:
	.ascii	"PRIXPTR __PRIPTR_PREFIX \"X\"\000"
.LASF719:
	.ascii	"INT64_MIN (-0x7fffffffffffffffL-1)\000"
.LASF236:
	.ascii	"__FLT32X_DECIMAL_DIG__ 17\000"
.LASF1209:
	.ascii	"TEE_PANIC_ID_TEE_DERIVEKEY 0x00001201\000"
.LASF174:
	.ascii	"__DBL_MIN__ ((double)2.2250738585072014e-308L)\000"
.LASF244:
	.ascii	"__FLT32X_HAS_QUIET_NAN__ 1\000"
.LASF240:
	.ascii	"__FLT32X_EPSILON__ 2.2204460492503131e-16F32x\000"
.LASF938:
	.ascii	"TEE_USAGE_ENCRYPT 0x00000002\000"
.LASF1047:
	.ascii	"TEE_TYPE_RSA_PUBLIC_KEY 0xA0000030\000"
.LASF358:
	.ascii	"__UDA_FBIT__ 32\000"
.LASF701:
	.ascii	"LLONG_MAX __LONG_LONG_MAX__\000"
.LASF977:
	.ascii	"TEE_ALG_SM4_CTR 0x10000214\000"
.LASF981:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA256 0x70004830\000"
.LASF1157:
	.ascii	"TEE_PANIC_ID_TEE_RESTRICTOBJECTUSAGE 0x00000705\000"
.LASF95:
	.ascii	"__UINTMAX_C(c) c ## ULL\000"
.LASF1250:
	.ascii	"TEE_PARAM_TYPE_SET(t,i) (((uint32_t)(t) & 0xF) << ("
	.ascii	"(i)*4))\000"
.LASF33:
	.ascii	"__SIZEOF_POINTER__ 4\000"
.LASF53:
	.ascii	"__INT_LEAST8_TYPE__ signed char\000"
.LASF1256:
	.ascii	"TEE_MEMREF_1_USED 0x00000002\000"
.LASF1067:
	.ascii	"TEE_TYPE_X25519_PUBLIC_KEY 0xA0000044\000"
.LASF534:
	.ascii	"CFG_MSG_LONG_PREFIX_MASK 0x1a\000"
.LASF370:
	.ascii	"__GCC_ATOMIC_BOOL_LOCK_FREE 2\000"
.LASF714:
	.ascii	"__int8_t_defined \000"
.LASF834:
	.ascii	"TEE_API_TYPES_H \000"
.LASF432:
	.ascii	"__ARM_NEON__\000"
.LASF532:
	.ascii	"CFG_MAX_CACHE_LINE_SHIFT 6\000"
.LASF202:
	.ascii	"__FLT32_MAX_EXP__ 128\000"
.LASF435:
	.ascii	"__THUMB_INTERWORK__ 1\000"
.LASF284:
	.ascii	"__ULLFRACT_MAX__ 0XFFFFFFFFFFFFFFFFP-64ULLR\000"
.LASF8:
	.ascii	"__VERSION__ \"12.3.0\"\000"
.LASF35:
	.ascii	"__GNUC_WIDE_EXECUTION_CHARSET_NAME \"UTF-32LE\"\000"
.LASF1041:
	.ascii	"TEE_TYPE_HMAC_SHA1 0xA0000002\000"
.LASF233:
	.ascii	"__FLT32X_MIN_10_EXP__ (-307)\000"
.LASF801:
	.ascii	"_SIZE_T_DECLARED \000"
.LASF1131:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASSTRING 0x00000207\000"
.LASF429:
	.ascii	"__ARM_FEATURE_FP16_VECTOR_ARITHMETIC\000"
.LASF883:
	.ascii	"TEE_ERROR_BAD_FORMAT 0xFFFF0005\000"
.LASF180:
	.ascii	"__DBL_IS_IEC_60559__ 2\000"
.LASF461:
	.ascii	"ARM32 1\000"
.LASF147:
	.ascii	"__DEC_EVAL_METHOD__ 2\000"
.LASF153:
	.ascii	"__FLT_MAX_EXP__ 128\000"
.LASF19:
	.ascii	"__SIZEOF_LONG__ 4\000"
.LASF837:
	.ascii	"__PRIPTR_PREFIX \"l\"\000"
.LASF1235:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSUB 0x00001906\000"
.LASF946:
	.ascii	"TEE_HANDLE_FLAG_KEY_SET 0x00040000\000"
.LASF1086:
	.ascii	"TEE_ATTR_DH_X_BITS 0xF0001332\000"
.LASF924:
	.ascii	"TEE_MEMORY_ACCESS_READ 0x00000001\000"
.LASF23:
	.ascii	"__SIZEOF_DOUBLE__ 8\000"
.LASF116:
	.ascii	"__INT_LEAST32_WIDTH__ 32\000"
.LASF767:
	.ascii	"UINT16_C(v) v\000"
.LASF638:
	.ascii	"_CFG_CORE_LTC_SIZE_OPTIMIZATION 1\000"
.LASF738:
	.ascii	"UINT_LEAST32_MAX UINT32_MAX\000"
.LASF1184:
	.ascii	"TEE_PANIC_ID_TEE_COPYOPERATION 0x00000C02\000"
.LASF592:
	.ascii	"CFG_STM32_TAMP 1\000"
.LASF516:
	.ascii	"CFG_EXT_DTS core/arch/arm/dts/external-dt/optee\000"
.LASF1108:
	.ascii	"TEE_ATTR_FLAG_PUBLIC (1 << 28)\000"
.LASF1216:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTFMMCONTEXTSIZEINU32 0x000015"
	.ascii	"01\000"
.LASF1192:
	.ascii	"TEE_PANIC_ID_TEE_DIGESTUPDATE 0x00000D02\000"
.LASF360:
	.ascii	"__UTA_FBIT__ 64\000"
.LASF155:
	.ascii	"__FLT_DECIMAL_DIG__ 9\000"
.LASF572:
	.ascii	"CFG_STM32_BSEC 1\000"
.LASF706:
	.ascii	"SHRT_MAX __SHRT_MAX__\000"
.LASF725:
	.ascii	"UINT16_MAX 0xffff\000"
.LASF1267:
	.ascii	"signed char\000"
.LASF530:
	.ascii	"CFG_LOCKDEP_RECORD_STACK 1\000"
.LASF926:
	.ascii	"TEE_MEMORY_ACCESS_ANY_OWNER 0x00000004\000"
.LASF789:
	.ascii	"__size_t__ \000"
.LASF1046:
	.ascii	"TEE_TYPE_HMAC_SM3 0xA0000007\000"
.LASF1212:
	.ascii	"TEE_PANIC_ID_TEE_GETSYSTEMTIME 0x00001402\000"
.LASF364:
	.ascii	"__GNUC_STDC_INLINE__ 1\000"
.LASF764:
	.ascii	"INT8_C(v) v\000"
.LASF256:
	.ascii	"__FRACT_FBIT__ 15\000"
.LASF737:
	.ascii	"UINT_LEAST16_MAX UINT16_MAX\000"
.LASF320:
	.ascii	"__LLACCUM_EPSILON__ 0x1P-31LLK\000"
.LASF7:
	.ascii	"__GNUC_PATCHLEVEL__ 0\000"
.LASF672:
	.ascii	"__noprof __attribute__((no_instrument_function))\000"
.LASF373:
	.ascii	"__GCC_ATOMIC_CHAR32_T_LOCK_FREE 2\000"
.LASF1071:
	.ascii	"TEE_ATTR_RSA_PUBLIC_EXPONENT 0xD0000230\000"
.LASF176:
	.ascii	"__DBL_DENORM_MIN__ ((double)4.9406564584124654e-324"
	.ascii	"L)\000"
.LASF127:
	.ascii	"__UINT64_C(c) c ## ULL\000"
.LASF609:
	.ascii	"CFG_TEE_CORE_EMBED_INTERNAL_TESTS 1\000"
.LASF1156:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTVALUEATTRIBUTE 0x00000704"
	.ascii	"\000"
.LASF487:
	.ascii	"CFG_CORE_THREAD_SHIFT 0\000"
.LASF823:
	.ascii	"__DEFINED_wchar_t \000"
.LASF972:
	.ascii	"TEE_ALG_DES3_CBC_NOPAD 0x10000113\000"
.LASF259:
	.ascii	"__FRACT_MAX__ 0X7FFFP-15R\000"
.LASF330:
	.ascii	"__SQ_FBIT__ 31\000"
.LASF219:
	.ascii	"__FLT64_MAX_10_EXP__ 308\000"
.LASF413:
	.ascii	"__ARM_ARCH_ISA_ARM 1\000"
.LASF531:
	.ascii	"CFG_LPAE_ADDR_SPACE_BITS 32\000"
.LASF965:
	.ascii	"TEE_ALG_AES_CCM 0x40000710\000"
.LASF871:
	.ascii	"TEE_TIMEOUT_INFINITE 0xFFFFFFFF\000"
.LASF800:
	.ascii	"_BSD_SIZE_T_DEFINED_ \000"
.LASF67:
	.ascii	"__UINT_FAST32_TYPE__ unsigned int\000"
.LASF1270:
	.ascii	"unsigned char\000"
.LASF3:
	.ascii	"__STDC_UTF_32__ 1\000"
.LASF22:
	.ascii	"__SIZEOF_FLOAT__ 4\000"
.LASF241:
	.ascii	"__FLT32X_DENORM_MIN__ 4.9406564584124654e-324F32x\000"
.LASF858:
	.ascii	"PRIx8 \"x\"\000"
.LASF154:
	.ascii	"__FLT_MAX_10_EXP__ 38\000"
.LASF474:
	.ascii	"CFG_CC_OPT_LEVEL s\000"
.LASF758:
	.ascii	"SIZE_MAX ULONG_MAX\000"
.LASF106:
	.ascii	"__UINT32_MAX__ 0xffffffffU\000"
.LASF831:
	.ascii	"__STM32MP_PM_H__ \000"
.LASF1191:
	.ascii	"TEE_PANIC_ID_TEE_DIGESTDOFINAL 0x00000D01\000"
.LASF132:
	.ascii	"__INT_FAST32_MAX__ 0x7fffffff\000"
.LASF10:
	.ascii	"__ATOMIC_SEQ_CST 5\000"
.LASF940:
	.ascii	"TEE_USAGE_MAC 0x00000008\000"
.LASF802:
	.ascii	"__DEFINED_size_t \000"
.LASF616:
	.ascii	"CFG_TEE_MANUFACTURER LINARO\000"
.LASF1021:
	.ascii	"TEE_ALG_ECDSA_P192 0x70001041\000"
.LASF1101:
	.ascii	"TEE_ATTR_ECC_EPHEMERAL_PUBLIC_VALUE_Y 0xD0000A46\000"
.LASF942:
	.ascii	"TEE_USAGE_VERIFY 0x00000020\000"
.LASF1145:
	.ascii	"TEE_PANIC_ID_TEE_FREE 0x00000602\000"
.LASF105:
	.ascii	"__UINT16_MAX__ 0xffff\000"
.LASF1133:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASUUID 0x00000209\000"
.LASF334:
	.ascii	"__TQ_FBIT__ 127\000"
.LASF541:
	.ascii	"CFG_PM_STUBS 1\000"
.LASF845:
	.ascii	"PRIi32 \"i\"\000"
.LASF943:
	.ascii	"TEE_USAGE_DERIVE 0x00000040\000"
.LASF923:
	.ascii	"TEE_PROPSET_CURRENT_TA (TEE_PropSetHandle)0xFFFFFFF"
	.ascii	"F\000"
.LASF125:
	.ascii	"__UINT32_C(c) c ## U\000"
.LASF340:
	.ascii	"__USQ_FBIT__ 32\000"
.LASF508:
	.ascii	"CFG_DRIVERS_NVMEM 1\000"
.LASF1263:
	.ascii	"PM_CTX_CCM_CTR0_SIZE 16\000"
.LASF1174:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATEPERSISTENTOBJECTENUMERATOR"
	.ascii	" 0x00000A01\000"
.LASF1182:
	.ascii	"TEE_PANIC_ID_TEE_WRITEOBJECTDATA 0x00000B04\000"
.LASF21:
	.ascii	"__SIZEOF_SHORT__ 2\000"
.LASF321:
	.ascii	"__ULLACCUM_FBIT__ 32\000"
.LASF964:
	.ascii	"TEE_ALG_AES_CMAC 0x30000610\000"
.LASF866:
	.ascii	"PRIX64 __PRI64_PREFIX \"X\"\000"
.LASF1225:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCMP 0x00001801\000"
.LASF1277:
	.ascii	"_Bool\000"
.LASF1140:
	.ascii	"TEE_PANIC_ID_TEE_OPENTASESSION 0x00000403\000"
.LASF632:
	.ascii	"CFG_WITH_VFP 1\000"
.LASF995:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512 0x60610230\000"
.LASF798:
	.ascii	"_SIZE_T_DEFINED_ \000"
.LASF1053:
	.ascii	"TEE_TYPE_ECDSA_KEYPAIR 0xA1000041\000"
.LASF1255:
	.ascii	"TEE_MEMREF_0_USED 0x00000001\000"
.LASF855:
	.ascii	"PRIu32 \"u\"\000"
.LASF655:
	.ascii	"__attr_const __attribute__((__const__))\000"
.LASF24:
	.ascii	"__SIZEOF_LONG_DOUBLE__ 8\000"
.LASF475:
	.ascii	"CFG_COMPAT_GP10_DES 1\000"
.LASF465:
	.ascii	"__home_lubuntu_Dokument_GitHub_STM32_Computer_Firmw"
	.ascii	"are_STM32_Computer_Firmware_CA7_optee_os_3_19_0_r2_"
	.ascii	"optee_os_stm32mp_3_19_0_stm32mp_r2____build_stm32mp"
	.ascii	"157a_dk1_include_generated_conf_h_ \000"
.LASF385:
	.ascii	"__PRAGMA_REDEFINE_EXTNAME 1\000"
.LASF38:
	.ascii	"__WCHAR_TYPE__ unsigned int\000"
.LASF479:
	.ascii	"CFG_CORE_DYN_SHM 1\000"
.LASF357:
	.ascii	"__USA_IBIT__ 16\000"
.LASF498:
	.ascii	"CFG_CRYPTO_SIZE_OPTIMIZATION 1\000"
.LASF367:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1\000"
.LASF954:
	.ascii	"TEE_OPERATION_KEY_DERIVATION 8\000"
.LASF589:
	.ascii	"CFG_STM32_RTC 1\000"
.LASF68:
	.ascii	"__UINT_FAST64_TYPE__ long long unsigned int\000"
.LASF1137:
	.ascii	"TEE_PANIC_ID_TEE_PANIC 0x00000301\000"
.LASF1007:
	.ascii	"TEE_ALG_SHA384 0x50000005\000"
.LASF478:
	.ascii	"CFG_CORE_CLUSTER_SHIFT 2\000"
.LASF158:
	.ascii	"__FLT_MIN__ 1.1754943508222875e-38F\000"
.LASF346:
	.ascii	"__HA_FBIT__ 7\000"
.LASF439:
	.ascii	"__FDPIC__\000"
.LASF1082:
	.ascii	"TEE_ATTR_DSA_PRIVATE_VALUE 0xC0000231\000"
.LASF879:
	.ascii	"TEE_ERROR_ACCESS_DENIED 0xFFFF0001\000"
.LASF504:
	.ascii	"CFG_DRIVERS_CLK 1\000"
.LASF1093:
	.ascii	"TEE_ATTR_ECC_PRIVATE_VALUE 0xC0000341\000"
.LASF1231:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTDIV 0x00001902\000"
.LASF456:
	.ascii	"__unix 1\000"
.LASF1000:
	.ascii	"TEE_ALG_SM2_DSA_SM3 0x70006045\000"
.LASF252:
	.ascii	"__USFRACT_IBIT__ 0\000"
.LASF711:
	.ascii	"ULONG_MAX (LONG_MAX * 2UL + 1UL)\000"
.LASF1141:
	.ascii	"TEE_PANIC_ID_TEE_GETCANCELLATIONFLAG 0x00000501\000"
.LASF253:
	.ascii	"__USFRACT_MIN__ 0.0UHR\000"
.LASF658:
	.ascii	"__used __attribute__((__used__))\000"
.LASF760:
	.ascii	"UL(v) v ## UL\000"
.LASF433:
	.ascii	"__ARM_NEON\000"
.LASF754:
	.ascii	"UINTPTR_MAX ULONG_MAX\000"
.LASF104:
	.ascii	"__UINT8_MAX__ 0xff\000"
.LASF386:
	.ascii	"__SIZEOF_WCHAR_T__ 4\000"
.LASF185:
	.ascii	"__LDBL_MAX_EXP__ 1024\000"
.LASF1012:
	.ascii	"TEE_ALG_SHA3_512 0x5000000B\000"
.LASF1276:
	.ascii	"long double\000"
.LASF678:
	.ascii	"__HAVE_BUILTIN_OVERFLOW 1\000"
.LASF1102:
	.ascii	"TEE_ATTR_EDDSA_CTX 0xD0000643\000"
.LASF876:
	.ascii	"TEE_ERROR_STORAGE_NOT_AVAILABLE_2 0xF0100004\000"
.LASF177:
	.ascii	"__DBL_HAS_DENORM__ 1\000"
.LASF925:
	.ascii	"TEE_MEMORY_ACCESS_WRITE 0x00000002\000"
.LASF642:
	.ascii	"GEN_ASM_DEFINES_H \000"
.LASF350:
	.ascii	"__DA_FBIT__ 31\000"
.LASF988:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA384 0x70515930\000"
.LASF71:
	.ascii	"__GXX_ABI_VERSION 1017\000"
.LASF111:
	.ascii	"__INT_LEAST16_MAX__ 0x7fff\000"
.LASF811:
	.ascii	"_T_WCHAR_ \000"
.LASF1029:
	.ascii	"TEE_ALG_ECDH_P256 0x80003042\000"
.LASF160:
	.ascii	"__FLT_DENORM_MIN__ 1.4012984643248171e-45F\000"
.LASF515:
	.ascii	"CFG_ENABLE_EMBEDDED_TESTS 1\000"
.LASF598:
	.ascii	"CFG_TA_ASLR 1\000"
.LASF627:
	.ascii	"CFG_WITH_NSEC_GPIOS 1\000"
.LASF1017:
	.ascii	"TEE_ALG_HMAC_SHA256 0x30000004\000"
.LASF238:
	.ascii	"__FLT32X_NORM_MAX__ 1.7976931348623157e+308F32x\000"
.LASF978:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_MD5 0x70001830\000"
.LASF718:
	.ascii	"INT32_MIN (-0x7fffffff-1)\000"
.LASF460:
	.ascii	"__KERNEL__ 1\000"
.LASF303:
	.ascii	"__UACCUM_MIN__ 0.0UK\000"
.LASF933:
	.ascii	"TEE_DATA_FLAG_SHARE_WRITE 0x00000020\000"
.LASF1238:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTMOD 0x00001A03\000"
.LASF159:
	.ascii	"__FLT_EPSILON__ 1.1920928955078125e-7F\000"
.LASF816:
	.ascii	"_WCHAR_T_DEFINED_ \000"
.LASF591:
	.ascii	"CFG_STM32_SHARED_IO 1\000"
.LASF906:
	.ascii	"TEE_PARAM_TYPE_VALUE_INOUT 3\000"
.LASF61:
	.ascii	"__INT_FAST8_TYPE__ signed char\000"
.LASF917:
	.ascii	"TEE_ORIGIN_API 0x00000001\000"
.LASF976:
	.ascii	"TEE_ALG_SM4_CBC_NOPAD 0x10000114\000"
.LASF419:
	.ascii	"__ARM_ARCH_ISA_THUMB\000"
.LASF1094:
	.ascii	"TEE_ATTR_ECC_CURVE 0xF0000441\000"
.LASF835:
	.ascii	"__INTTYPES_H \000"
.LASF447:
	.ascii	"__ARM_FEATURE_MATMUL_INT8\000"
.LASF654:
	.ascii	"__noinline __attribute__((noinline))\000"
.LASF1072:
	.ascii	"TEE_ATTR_RSA_PRIVATE_EXPONENT 0xC0000330\000"
.LASF1081:
	.ascii	"TEE_ATTR_DSA_PUBLIC_VALUE 0xD0000131\000"
.LASF375:
	.ascii	"__GCC_ATOMIC_SHORT_LOCK_FREE 2\000"
.LASF637:
	.ascii	"_CFG_CORE_LTC_OPTEE_THREAD 1\000"
.LASF820:
	.ascii	"__INT_WCHAR_T_H \000"
.LASF726:
	.ascii	"UINT32_MAX 0xffffffffU\000"
.LASF913:
	.ascii	"TEE_LOGIN_APPLICATION 0x00000004\000"
.LASF291:
	.ascii	"__USACCUM_FBIT__ 8\000"
.LASF744:
	.ascii	"INT_FAST8_MAX INT8_MAX\000"
.LASF379:
	.ascii	"__GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1\000"
.LASF810:
	.ascii	"_WCHAR_T \000"
.LASF554:
	.ascii	"CFG_SCMI_MSG_VOLTAGE_DOMAIN 1\000"
.LASF483:
	.ascii	"CFG_CORE_MAX_SYSCALL_RECURSION 4\000"
.LASF306:
	.ascii	"__LACCUM_FBIT__ 31\000"
.LASF484:
	.ascii	"CFG_CORE_MBEDTLS_MPI 1\000"
.LASF621:
	.ascii	"CFG_WARN_DECL_AFTER_STATEMENT 1\000"
.LASF212:
	.ascii	"__FLT32_HAS_QUIET_NAN__ 1\000"
.LASF1166:
	.ascii	"TEE_PANIC_ID_TEE_POPULATETRANSIENTOBJECT 0x00000807"
	.ascii	"\000"
.LASF805:
	.ascii	"_SIZET_ \000"
.LASF894:
	.ascii	"TEE_ERROR_SHORT_BUFFER 0xFFFF0010\000"
.LASF195:
	.ascii	"__LDBL_HAS_INFINITY__ 1\000"
.LASF890:
	.ascii	"TEE_ERROR_OUT_OF_MEMORY 0xFFFF000C\000"
.LASF352:
	.ascii	"__TA_FBIT__ 63\000"
.LASF526:
	.ascii	"CFG_INIT_CNTVOFF 1\000"
.LASF579:
	.ascii	"CFG_STM32_GPIO 1\000"
.LASF920:
	.ascii	"TEE_ORIGIN_TRUSTED_APP 0x00000004\000"
.LASF469:
	.ascii	"CFG_ARM32_ta_arm32 1\000"
.LASF1247:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTTOFMM 0x00001C03\000"
.LASF437:
	.ascii	"__ARM_PCS 1\000"
.LASF551:
	.ascii	"CFG_SCMI_MSG_DRIVERS 1\000"
.LASF1014:
	.ascii	"TEE_ALG_HMAC_MD5 0x30000001\000"
.LASF689:
	.ascii	"bool _Bool\000"
.LASF514:
	.ascii	"CFG_EMBED_DTB_SOURCE_FILE stm32mp157a-dk1.dts\000"
.LASF1249:
	.ascii	"TEE_PARAM_TYPE_GET(t,i) ((((uint32_t)t) >> ((i)*4))"
	.ascii	" & 0xF)\000"
.LASF98:
	.ascii	"__SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)\000"
.LASF670:
	.ascii	"__nex_bss \000"
.LASF120:
	.ascii	"__UINT_LEAST8_MAX__ 0xff\000"
.LASF762:
	.ascii	"L(v) v ## L\000"
.LASF815:
	.ascii	"_BSD_WCHAR_T_ \000"
.LASF936:
	.ascii	"TEE_OBJECT_ID_MAX_LEN 64\000"
.LASF1262:
	.ascii	"PM_CTX_CCM_B0_SIZE 16\000"
.LASF490:
	.ascii	"CFG_CORE_WORKAROUND_NSITR_CACHE_PRIME 1\000"
.LASF966:
	.ascii	"TEE_ALG_AES_GCM 0x40000810\000"
.LASF959:
	.ascii	"TEE_ALG_AES_CTR 0x10000210\000"
.LASF245:
	.ascii	"__FLT32X_IS_IEC_60559__ 2\000"
.LASF1059:
	.ascii	"TEE_TYPE_SM2_DSA_KEYPAIR 0xA1000045\000"
.LASF983:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA512 0x70006830\000"
.LASF665:
	.ascii	"__SECTION_FLAGS_RODATA \",\\\"a\\\",%progbits \" CO"
	.ascii	"MMENT_CHAR\000"
.LASF773:
	.ascii	"INTMAX_C(v) INT64_C(v)\000"
.LASF113:
	.ascii	"__INT_LEAST16_WIDTH__ 16\000"
.LASF476:
	.ascii	"CFG_CORE_ASYNC_NOTIF_GIC_INTID 0\000"
.LASF1200:
	.ascii	"TEE_PANIC_ID_TEE_AEDECRYPTFINAL 0x00001001\000"
.LASF430:
	.ascii	"__ARM_FEATURE_FP16_FML\000"
.LASF950:
	.ascii	"TEE_OPERATION_AE 4\000"
.LASF563:
	.ascii	"CFG_STM32MP15 1\000"
.LASF809:
	.ascii	"__WCHAR_T__ \000"
.LASF255:
	.ascii	"__USFRACT_EPSILON__ 0x1P-8UHR\000"
.LASF827:
	.ascii	"NULL ((void *)0)\000"
.LASF729:
	.ascii	"INT_LEAST16_MIN INT16_MIN\000"
.LASF1015:
	.ascii	"TEE_ALG_HMAC_SHA1 0x30000002\000"
.LASF1066:
	.ascii	"TEE_TYPE_DATA 0xA00000BF\000"
.LASF537:
	.ascii	"CFG_OPTEE_REVISION_MINOR 19\000"
.LASF251:
	.ascii	"__USFRACT_FBIT__ 8\000"
.LASF724:
	.ascii	"UINT8_MAX 0xff\000"
.LASF319:
	.ascii	"__LLACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LLK\000"
.LASF841:
	.ascii	"PRId64 __PRI64_PREFIX \"d\"\000"
.LASF828:
	.ascii	"__need_NULL\000"
.LASF847:
	.ascii	"PRIiPTR __PRIPTR_PREFIX \"i\"\000"
.LASF452:
	.ascii	"__gnu_linux__ 1\000"
.LASF1062:
	.ascii	"TEE_TYPE_SM2_PKE_PUBLIC_KEY 0xA0000047\000"
.LASF363:
	.ascii	"__USER_LABEL_PREFIX__ \000"
.LASF107:
	.ascii	"__UINT64_MAX__ 0xffffffffffffffffULL\000"
.LASF561:
	.ascii	"CFG_STACK_THREAD_EXTRA 0\000"
.LASF624:
	.ascii	"CFG_WDT_SM_HANDLER 1\000"
.LASF114:
	.ascii	"__INT_LEAST32_MAX__ 0x7fffffff\000"
.LASF854:
	.ascii	"PRIu16 \"u\"\000"
.LASF1168:
	.ascii	"TEE_PANIC_ID_TEE_COPYOBJECTATTRIBUTES1 0x00000809\000"
.LASF626:
	.ascii	"CFG_WITH_LPAE 1\000"
.LASF1:
	.ascii	"__STDC_VERSION__ 201112L\000"
.LASF1144:
	.ascii	"TEE_PANIC_ID_TEE_CHECKMEMORYACCESSRIGHTS 0x00000601"
	.ascii	"\000"
.LASF108:
	.ascii	"__INT_LEAST8_MAX__ 0x7f\000"
.LASF1117:
	.ascii	"TEE_ECC_CURVE_NIST_P521 0x00000005\000"
.LASF382:
	.ascii	"__GCC_ATOMIC_POINTER_LOCK_FREE 2\000"
.LASF390:
	.ascii	"__ARM_FEATURE_QBIT 1\000"
.LASF314:
	.ascii	"__ULACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULK\000"
.LASF11:
	.ascii	"__ATOMIC_ACQUIRE 2\000"
.LASF397:
	.ascii	"__ARM_FEATURE_COMPLEX\000"
.LASF1052:
	.ascii	"TEE_TYPE_ECDSA_PUBLIC_KEY 0xA0000041\000"
.LASF934:
	.ascii	"TEE_DATA_FLAG_OVERWRITE 0x00000400\000"
.LASF1163:
	.ascii	"TEE_PANIC_ID_TEE_GENERATEKEY 0x00000804\000"
.LASF407:
	.ascii	"__ARM_SIZEOF_WCHAR_T 4\000"
.LASF234:
	.ascii	"__FLT32X_MAX_EXP__ 1024\000"
.LASF1146:
	.ascii	"TEE_PANIC_ID_TEE_GETINSTANCEDATA 0x00000603\000"
.LASF660:
	.ascii	"__cold __attribute__((__cold__))\000"
.LASF1060:
	.ascii	"TEE_TYPE_SM2_KEP_PUBLIC_KEY 0xA0000046\000"
.LASF1026:
	.ascii	"TEE_ALG_ED25519 0x70006043\000"
.LASF175:
	.ascii	"__DBL_EPSILON__ ((double)2.2204460492503131e-16L)\000"
.LASF1043:
	.ascii	"TEE_TYPE_HMAC_SHA256 0xA0000004\000"
.LASF268:
	.ascii	"__LFRACT_MIN__ (-0.5LR-0.5LR)\000"
.LASF119:
	.ascii	"__INT_LEAST64_WIDTH__ 64\000"
.LASF162:
	.ascii	"__FLT_HAS_INFINITY__ 1\000"
.LASF937:
	.ascii	"TEE_USAGE_EXTRACTABLE 0x00000001\000"
.LASF844:
	.ascii	"PRIi16 \"i\"\000"
.LASF299:
	.ascii	"__ACCUM_MAX__ 0X7FFFFFFFP-15K\000"
.LASF1258:
	.ascii	"TEE_MEMREF_3_USED 0x00000008\000"
.LASF100:
	.ascii	"__INT8_MAX__ 0x7f\000"
.LASF1217:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTFMMSIZEINU32 0x00001502\000"
.LASF675:
	.ascii	"__compiler_bswap32(x) __builtin_bswap32((x))\000"
.LASF62:
	.ascii	"__INT_FAST16_TYPE__ int\000"
.LASF384:
	.ascii	"__GCC_HAVE_DWARF2_CFI_ASM 1\000"
.LASF395:
	.ascii	"__ARM_FEATURE_CRC32\000"
.LASF248:
	.ascii	"__SFRACT_MIN__ (-0.5HR-0.5HR)\000"
.LASF1275:
	.ascii	"long unsigned int\000"
.LASF349:
	.ascii	"__SA_IBIT__ 16\000"
.LASF393:
	.ascii	"__ARM_FEATURE_UNALIGNED\000"
.LASF584:
	.ascii	"CFG_STM32_LPTIMER 1\000"
.LASF144:
	.ascii	"__GCC_IEC_559_COMPLEX 0\000"
.LASF399:
	.ascii	"__ARM_FEATURE_MVE\000"
.LASF115:
	.ascii	"__INT32_C(c) c\000"
.LASF1176:
	.ascii	"TEE_PANIC_ID_TEE_GETNEXTPERSISTENTOBJECT 0x00000A03"
	.ascii	"\000"
.LASF425:
	.ascii	"__ARM_FP16_FORMAT_IEEE\000"
.LASF50:
	.ascii	"__UINT16_TYPE__ short unsigned int\000"
.LASF594:
	.ascii	"CFG_STM32_TIM 1\000"
.LASF909:
	.ascii	"TEE_PARAM_TYPE_MEMREF_INOUT 7\000"
.LASF1024:
	.ascii	"TEE_ALG_ECDSA_P384 0x70004041\000"
.LASF422:
	.ascii	"__SOFTFP__ 1\000"
.LASF1068:
	.ascii	"TEE_TYPE_X25519_KEYPAIR 0xA1000044\000"
.LASF1099:
	.ascii	"TEE_ATTR_SM2_KEP_CONFIRMATION_OUT 0xD0000846\000"
.LASF745:
	.ascii	"INT_FAST16_MAX INT16_MAX\000"
.LASF146:
	.ascii	"__FLT_EVAL_METHOD_TS_18661_3__ 0\000"
.LASF83:
	.ascii	"__SCHAR_WIDTH__ 8\000"
.LASF903:
	.ascii	"TEE_PARAM_TYPE_NONE 0\000"
.LASF832:
	.ascii	"PSCI_MODE_SYSTEM_SUSPEND 0\000"
.LASF1181:
	.ascii	"TEE_PANIC_ID_TEE_TRUNCATEOBJECTDATA 0x00000B03\000"
.LASF846:
	.ascii	"PRIi64 __PRI64_PREFIX \"i\"\000"
.LASF1023:
	.ascii	"TEE_ALG_ECDSA_P256 0x70003041\000"
.LASF317:
	.ascii	"__LLACCUM_IBIT__ 32\000"
.LASF1074:
	.ascii	"TEE_ATTR_RSA_PRIME2 0xC0000530\000"
.LASF260:
	.ascii	"__FRACT_EPSILON__ 0x1P-15R\000"
.LASF1158:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTINFO1 0x00000706\000"
.LASF1204:
	.ascii	"TEE_PANIC_ID_TEE_AEUPDATEAAD 0x00001005\000"
.LASF436:
	.ascii	"__ARM_ARCH_7A__ 1\000"
.LASF970:
	.ascii	"TEE_ALG_DES_CBC_MAC_PKCS5 0x30000511\000"
.LASF1006:
	.ascii	"TEE_ALG_SHA256 0x50000004\000"
.LASF129:
	.ascii	"__INT_FAST8_WIDTH__ 8\000"
.LASF463:
	.ascii	"TRACE_LEVEL 2\000"
.LASF682:
	.ascii	"__compiler_mul_overflow(a,b,res) __builtin_mul_over"
	.ascii	"flow((a), (b), (res))\000"
.LASF117:
	.ascii	"__INT_LEAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF449:
	.ascii	"__ARM_FEATURE_BF16_VECTOR_ARITHMETIC\000"
.LASF753:
	.ascii	"INTPTR_MAX LONG_MAX\000"
.LASF1254:
	.ascii	"TEE_MEM_OUTPUT 0x00000002\000"
.LASF1104:
	.ascii	"TEE_ATTR_ED25519_PRIVATE_VALUE 0xC0000843\000"
.LASF198:
	.ascii	"__FLT32_MANT_DIG__ 24\000"
.LASF1178:
	.ascii	"TEE_PANIC_ID_TEE_STARTPERSISTENTOBJECTENUMERATOR 0x"
	.ascii	"00000A05\000"
.LASF1135:
	.ascii	"TEE_PANIC_ID_TEE_RESETPROPERTYENUMERATOR 0x0000020B"
	.ascii	"\000"
.LASF209:
	.ascii	"__FLT32_DENORM_MIN__ 1.4012984643248171e-45F32\000"
.LASF829:
	.ascii	"offsetof(TYPE,MEMBER) __builtin_offsetof (TYPE, MEM"
	.ascii	"BER)\000"
.LASF653:
	.ascii	"__printf(a,b) __attribute__((format(printf, a, b)))"
	.ascii	"\000"
.LASF60:
	.ascii	"__UINT_LEAST64_TYPE__ long long unsigned int\000"
.LASF1257:
	.ascii	"TEE_MEMREF_2_USED 0x00000004\000"
.LASF1179:
	.ascii	"TEE_PANIC_ID_TEE_READOBJECTDATA 0x00000B01\000"
.LASF1234:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSQUARE 0x00001905\000"
.LASF192:
	.ascii	"__LDBL_EPSILON__ 2.2204460492503131e-16L\000"
.LASF1010:
	.ascii	"TEE_ALG_SHA3_256 0x50000009\000"
.LASF957:
	.ascii	"TEE_ALG_AES_ECB_NOPAD 0x10000010\000"
.LASF372:
	.ascii	"__GCC_ATOMIC_CHAR16_T_LOCK_FREE 2\000"
.LASF576:
	.ascii	"CFG_STM32_EARLY_CONSOLE_UART 4\000"
.LASF169:
	.ascii	"__DBL_MAX_EXP__ 1024\000"
.LASF771:
	.ascii	"UINT64_C(v) ULL(v)\000"
.LASF12:
	.ascii	"__ATOMIC_RELEASE 3\000"
.LASF1092:
	.ascii	"TEE_ATTR_ECC_PUBLIC_VALUE_Y 0xD0000241\000"
.LASF242:
	.ascii	"__FLT32X_HAS_DENORM__ 1\000"
.LASF149:
	.ascii	"__FLT_MANT_DIG__ 24\000"
.LASF763:
	.ascii	"LL(v) v ## LL\000"
.LASF343:
	.ascii	"__UDQ_IBIT__ 0\000"
.LASF15:
	.ascii	"__OPTIMIZE_SIZE__ 1\000"
.LASF16:
	.ascii	"__OPTIMIZE__ 1\000"
.LASF635:
	.ascii	"_CFG_CORE_LTC_BIGNUM_MAX_BITS 4096\000"
.LASF304:
	.ascii	"__UACCUM_MAX__ 0XFFFFFFFFP-16UK\000"
.LASF222:
	.ascii	"__FLT64_NORM_MAX__ 1.7976931348623157e+308F64\000"
.LASF1120:
	.ascii	"TEE_PANIC_ID_TA_CLOSESESSIONENTRYPOINT 0x00000101\000"
.LASF564:
	.ascii	"CFG_STM32MP15_CLK 1\000"
.LASF779:
	.ascii	"_T_PTRDIFF_ \000"
.LASF921:
	.ascii	"TEE_PROPSET_TEE_IMPLEMENTATION (TEE_PropSetHandle)0"
	.ascii	"xFFFFFFFD\000"
.LASF731:
	.ascii	"INT_LEAST64_MIN INT64_MIN\000"
.LASF193:
	.ascii	"__LDBL_DENORM_MIN__ 4.9406564584124654e-324L\000"
.LASF103:
	.ascii	"__INT64_MAX__ 0x7fffffffffffffffLL\000"
.LASF391:
	.ascii	"__ARM_FEATURE_SAT 1\000"
.LASF560:
	.ascii	"CFG_SP_INIT_INFO_MAX_SIZE 0x1000\000"
.LASF536:
	.ascii	"CFG_OPTEE_REVISION_MAJOR 3\000"
.LASF282:
	.ascii	"__ULLFRACT_IBIT__ 0\000"
.LASF416:
	.ascii	"__thumb__ 1\000"
.LASF752:
	.ascii	"INTPTR_MIN LONG_MIN\000"
.LASF777:
	.ascii	"_ANSI_STDDEF_H \000"
.LASF571:
	.ascii	"CFG_STM32MP_PROVISIONING 1\000"
.LASF1090:
	.ascii	"TEE_ATTR_RSA_PSS_SALT_LENGTH 0xF0000A30\000"
.LASF705:
	.ascii	"SCHAR_MIN (-SCHAR_MAX - 1)\000"
.LASF585:
	.ascii	"CFG_STM32_PWR_SIP 1\000"
.LASF1214:
	.ascii	"TEE_PANIC_ID_TEE_SETTAPERSISTENTTIME 0x00001404\000"
.LASF5:
	.ascii	"__GNUC__ 12\000"
.LASF369:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1\000"
.LASF1198:
	.ascii	"TEE_PANIC_ID_TEE_MACINIT 0x00000F03\000"
.LASF1173:
	.ascii	"TEE_PANIC_ID_TEE_CLOSEANDDELETEPERSISTENTOBJECT1 0x"
	.ascii	"00000905\000"
.LASF573:
	.ascii	"CFG_STM32_BSEC_WRITE 1\000"
.LASF540:
	.ascii	"CFG_PM_ARM32 1\000"
.LASF640:
	.ascii	"_CFG_FTRACE_BUF_WHEN_FULL_shift 1\000"
.LASF156:
	.ascii	"__FLT_MAX__ 3.4028234663852886e+38F\000"
.LASF1085:
	.ascii	"TEE_ATTR_DH_BASE 0xD0001232\000"
.LASF301:
	.ascii	"__UACCUM_FBIT__ 16\000"
.LASF265:
	.ascii	"__UFRACT_EPSILON__ 0x1P-16UR\000"
.LASF78:
	.ascii	"__WCHAR_MIN__ 0U\000"
.LASF235:
	.ascii	"__FLT32X_MAX_10_EXP__ 308\000"
.LASF337:
	.ascii	"__UQQ_IBIT__ 0\000"
.LASF529:
	.ascii	"CFG_LIBUTILS_WITH_ISOC 1\000"
.LASF900:
	.ascii	"TEE_ERROR_SIGNATURE_INVALID 0xFFFF3072\000"
.LASF377:
	.ascii	"__GCC_ATOMIC_LONG_LOCK_FREE 2\000"
.LASF313:
	.ascii	"__ULACCUM_MIN__ 0.0ULK\000"
.LASF629:
	.ascii	"CFG_WITH_STACK_CANARIES 1\000"
.LASF807:
	.ascii	"__need_size_t\000"
.LASF189:
	.ascii	"__LDBL_MAX__ 1.7976931348623157e+308L\000"
.LASF412:
	.ascii	"__ARM_ARCH 7\000"
.LASF1083:
	.ascii	"TEE_ATTR_DH_PRIME 0xD0001032\000"
.LASF1227:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTGETBIT 0x00001803\000"
.LASF796:
	.ascii	"_SIZE_T_ \000"
.LASF148:
	.ascii	"__FLT_RADIX__ 2\000"
.LASF997:
	.ascii	"TEE_ALG_DSA_SHA1 0x70002131\000"
.LASF1269:
	.ascii	"long long int\000"
.LASF400:
	.ascii	"__ARM_FEATURE_CMSE\000"
.LASF86:
	.ascii	"__LONG_WIDTH__ 32\000"
.LASF899:
	.ascii	"TEE_ERROR_MAC_INVALID 0xFFFF3071\000"
.LASF43:
	.ascii	"__CHAR32_TYPE__ unsigned int\000"
.LASF656:
	.ascii	"__unused __attribute__((unused))\000"
.LASF808:
	.ascii	"__wchar_t__ \000"
.LASF196:
	.ascii	"__LDBL_HAS_QUIET_NAN__ 1\000"
.LASF1111:
	.ascii	"TEE_ATTR_BIT_VALUE TEE_ATTR_FLAG_VALUE\000"
.LASF945:
	.ascii	"TEE_HANDLE_FLAG_INITIALIZED 0x00020000\000"
.LASF87:
	.ascii	"__LONG_LONG_WIDTH__ 64\000"
.LASF1159:
	.ascii	"TEE_PANIC_ID_TEE_RESTRICTOBJECTUSAGE1 0x00000707\000"
.LASF778:
	.ascii	"_PTRDIFF_T \000"
.LASF518:
	.ascii	"CFG_FTRACE_US_MS 10000\000"
.LASF1019:
	.ascii	"TEE_ALG_HMAC_SHA512 0x30000006\000"
.LASF139:
	.ascii	"__UINT_FAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF424:
	.ascii	"__ARM_FP\000"
.LASF580:
	.ascii	"CFG_STM32_HUK_FROM_DT 1\000"
.LASF347:
	.ascii	"__HA_IBIT__ 8\000"
.LASF1063:
	.ascii	"TEE_TYPE_SM2_PKE_KEYPAIR 0xA1000047\000"
.LASF141:
	.ascii	"__INTPTR_WIDTH__ 32\000"
.LASF1121:
	.ascii	"TEE_PANIC_ID_TA_CREATEENTRYPOINT 0x00000102\000"
.LASF378:
	.ascii	"__GCC_ATOMIC_LLONG_LOCK_FREE 2\000"
.LASF137:
	.ascii	"__UINT_FAST16_MAX__ 0xffffffffU\000"
.LASF239:
	.ascii	"__FLT32X_MIN__ 2.2250738585072014e-308F32x\000"
.LASF747:
	.ascii	"INT_FAST64_MAX INT64_MAX\000"
.LASF218:
	.ascii	"__FLT64_MAX_EXP__ 1024\000"
.LASF1050:
	.ascii	"TEE_TYPE_DSA_KEYPAIR 0xA1000031\000"
.LASF813:
	.ascii	"__WCHAR_T \000"
.LASF865:
	.ascii	"PRIX32 \"X\"\000"
.LASF623:
	.ascii	"CFG_WDT 1\000"
.LASF223:
	.ascii	"__FLT64_MIN__ 2.2250738585072014e-308F64\000"
.LASF521:
	.ascii	"CFG_HWRNG_PTA 1\000"
.LASF1153:
	.ascii	"TEE_PANIC_ID_TEE_CLOSEOBJECT 0x00000701\000"
.LASF869:
	.ascii	"TEE_INT_CORE_API_SPEC_VERSION 0x0000000A\000"
.LASF408:
	.ascii	"__ARM_ARCH_PROFILE\000"
.LASF48:
	.ascii	"__INT64_TYPE__ long long int\000"
.LASF962:
	.ascii	"TEE_ALG_AES_CBC_MAC_NOPAD 0x30000110\000"
.LASF266:
	.ascii	"__LFRACT_FBIT__ 31\000"
.LASF26:
	.ascii	"__CHAR_BIT__ 8\000"
.LASF547:
	.ascii	"CFG_RPMB_FS_CACHE_ENTRIES 0\000"
.LASF128:
	.ascii	"__INT_FAST8_MAX__ 0x7f\000"
.LASF527:
	.ascii	"CFG_KERN_LINKER_ARCH arm\000"
.LASF935:
	.ascii	"TEE_DATA_MAX_POSITION 0xFFFFFFFF\000"
.LASF647:
	.ascii	"__deprecated __attribute__((deprecated))\000"
.LASF525:
	.ascii	"CFG_HWSUPP_MEM_PERM_WXN 1\000"
.LASF716:
	.ascii	"INT8_MIN (-0x7f-1)\000"
.LASF477:
	.ascii	"CFG_CORE_BIGNUM_MAX_BITS 4096\000"
.LASF264:
	.ascii	"__UFRACT_MAX__ 0XFFFFP-16UR\000"
.LASF915:
	.ascii	"TEE_LOGIN_APPLICATION_GROUP 0x00000006\000"
.LASF989:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA512 0x70616930\000"
.LASF768:
	.ascii	"INT32_C(v) v\000"
.LASF836:
	.ascii	"__PRI64_PREFIX \"ll\"\000"
.LASF31:
	.ascii	"__BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF208:
	.ascii	"__FLT32_EPSILON__ 1.1920928955078125e-7F32\000"
.LASF434:
	.ascii	"__ARM_NEON_FP\000"
.LASF1232:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTMUL 0x00001903\000"
.LASF902:
	.ascii	"TEE_ERROR_TIME_NEEDS_RESET 0xFFFF5001\000"
.LASF528:
	.ascii	"CFG_KERN_LINKER_FORMAT elf32-littlearm\000"
.LASF1058:
	.ascii	"TEE_TYPE_SM2_DSA_PUBLIC_KEY 0xA0000045\000"
.LASF968:
	.ascii	"TEE_ALG_DES_CBC_NOPAD 0x10000111\000"
.LASF261:
	.ascii	"__UFRACT_FBIT__ 16\000"
.LASF342:
	.ascii	"__UDQ_FBIT__ 64\000"
.LASF173:
	.ascii	"__DBL_NORM_MAX__ ((double)1.7976931348623157e+308L)"
	.ascii	"\000"
.LASF1265:
	.ascii	"OFFSET_OF_CTX_STRUCT(_f) offsetof(struct retram_res"
	.ascii	"ume_ctx, _f)\000"
.LASF186:
	.ascii	"__LDBL_MAX_10_EXP__ 308\000"
.LASF874:
	.ascii	"TEE_ERROR_CORRUPT_OBJECT_2 0xF0100002\000"
.LASF1152:
	.ascii	"TEE_PANIC_ID_TEE_SETINSTANCEDATA 0x00000609\000"
.LASF1221:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTFROMOCTETSTRING 0x000"
	.ascii	"01701\000"
.LASF905:
	.ascii	"TEE_PARAM_TYPE_VALUE_OUTPUT 2\000"
.LASF570:
	.ascii	"CFG_STM32MP_PROFILE system_services\000"
.LASF603:
	.ascii	"CFG_TA_FLOAT_SUPPORT 1\000"
.LASF59:
	.ascii	"__UINT_LEAST32_TYPE__ unsigned int\000"
.LASF1272:
	.ascii	"unsigned int\000"
.LASF1251:
	.ascii	"TEE_NUM_PARAMS 4\000"
.LASF728:
	.ascii	"INT_LEAST8_MIN INT8_MIN\000"
.LASF1229:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSHIFTRIGHT 0x00001805\000"
.LASF740:
	.ascii	"INT_FAST8_MIN INT8_MIN\000"
.LASF151:
	.ascii	"__FLT_MIN_EXP__ (-125)\000"
.LASF228:
	.ascii	"__FLT64_HAS_QUIET_NAN__ 1\000"
.LASF1105:
	.ascii	"TEE_ATTR_X25519_PUBLIC_VALUE 0xD0000944\000"
.LASF292:
	.ascii	"__USACCUM_IBIT__ 8\000"
.LASF878:
	.ascii	"TEE_ERROR_GENERIC 0xFFFF0000\000"
.LASF226:
	.ascii	"__FLT64_HAS_DENORM__ 1\000"
.LASF150:
	.ascii	"__FLT_DIG__ 6\000"
.LASF305:
	.ascii	"__UACCUM_EPSILON__ 0x1P-16UK\000"
.LASF822:
	.ascii	"_WCHAR_T_DECLARED \000"
.LASF552:
	.ascii	"CFG_SCMI_MSG_RESET_DOMAIN 1\000"
.LASF1100:
	.ascii	"TEE_ATTR_ECC_EPHEMERAL_PUBLIC_VALUE_X 0xD0000946\000"
.LASF1037:
	.ascii	"TEE_TYPE_DES 0xA0000011\000"
.LASF145:
	.ascii	"__FLT_EVAL_METHOD__ 0\000"
.LASF1169:
	.ascii	"TEE_PANIC_ID_TEE_CLOSEANDDELETEPERSISTENTOBJECT 0x0"
	.ascii	"0000901\000"
.LASF667:
	.ascii	"__rodata_dummy __section(\".rodata.dummy\" __SECTIO"
	.ascii	"N_FLAGS_RODATA)\000"
.LASF974:
	.ascii	"TEE_ALG_DES3_CBC_MAC_PKCS5 0x30000513\000"
.LASF72:
	.ascii	"__SCHAR_MAX__ 0x7f\000"
.LASF634:
	.ascii	"PLATFORM_stm32mp1 1\000"
.LASF702:
	.ascii	"LLONG_MIN (-LLONG_MAX - 1LL)\000"
.LASF401:
	.ascii	"__ARM_FEATURE_LDREX\000"
.LASF780:
	.ascii	"_T_PTRDIFF \000"
.LASF336:
	.ascii	"__UQQ_FBIT__ 8\000"
.LASF1160:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATETRANSIENTOBJECT 0x00000801"
	.ascii	"\000"
.LASF1224:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTTOS32 0x00001704\000"
.LASF501:
	.ascii	"CFG_DEVICE_ENUM_PTA 1\000"
.LASF427:
	.ascii	"__ARM_FP16_ARGS\000"
.LASF143:
	.ascii	"__GCC_IEC_559 0\000"
.LASF1195:
	.ascii	"TEE_PANIC_ID_TEE_CIPHERUPDATE 0x00000E03\000"
.LASF1107:
	.ascii	"TEE_ATTR_EDDSA_PREHASH 0xF0000004\000"
.LASF1222:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTFROMS32 0x00001702\000"
.LASF269:
	.ascii	"__LFRACT_MAX__ 0X7FFFFFFFP-31LR\000"
.LASF388:
	.ascii	"__SIZEOF_PTRDIFF_T__ 4\000"
.LASF0:
	.ascii	"__STDC__ 1\000"
.LASF853:
	.ascii	"PRIu8 \"u\"\000"
.LASF1136:
	.ascii	"TEE_PANIC_ID_TEE_STARTPROPERTYENUMERATOR 0x0000020C"
	.ascii	"\000"
.LASF1236:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTADDMOD 0x00001A01\000"
.LASF441:
	.ascii	"__ARM_FEATURE_IDIV 1\000"
.LASF916:
	.ascii	"TEE_LOGIN_TRUSTED_APP 0xF0000000\000"
.LASF32:
	.ascii	"__FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF1147:
	.ascii	"TEE_PANIC_ID_TEE_MALLOC 0x00000604\000"
.LASF720:
	.ascii	"INT8_MAX 0x7f\000"
.LASF657:
	.ascii	"__maybe_unused __attribute__((unused))\000"
.LASF686:
	.ascii	"barrier() asm volatile (\"\" : : : \"memory\")\000"
.LASF975:
	.ascii	"TEE_ALG_SM4_ECB_NOPAD 0x10000014\000"
.LASF444:
	.ascii	"__ARM_FEATURE_COPROC 15\000"
.LASF1208:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICVERIFYDIGEST 0x00001104\000"
.LASF772:
	.ascii	"UINTPTR_C(v) UL(v)\000"
.LASF691:
	.ascii	"false 0\000"
.LASF996:
	.ascii	"TEE_ALG_RSA_NOPAD 0x60000030\000"
.LASF84:
	.ascii	"__SHRT_WIDTH__ 16\000"
.LASF991:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA1 0x60210230\000"
.LASF649:
	.ascii	"__weak __attribute__((weak))\000"
.LASF690:
	.ascii	"true 1\000"
.LASF786:
	.ascii	"_PTRDIFF_T_DECLARED \000"
.LASF356:
	.ascii	"__USA_FBIT__ 16\000"
.LASF949:
	.ascii	"TEE_OPERATION_MAC 3\000"
.LASF1154:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTBUFFERATTRIBUTE 0x0000070"
	.ascii	"2\000"
.LASF63:
	.ascii	"__INT_FAST32_TYPE__ int\000"
.LASF1122:
	.ascii	"TEE_PANIC_ID_TA_DESTROYENTRYPOINT 0x00000103\000"
.LASF232:
	.ascii	"__FLT32X_MIN_EXP__ (-1021)\000"
.LASF908:
	.ascii	"TEE_PARAM_TYPE_MEMREF_OUTPUT 6\000"
.LASF163:
	.ascii	"__FLT_HAS_QUIET_NAN__ 1\000"
.LASF371:
	.ascii	"__GCC_ATOMIC_CHAR_LOCK_FREE 2\000"
.LASF1185:
	.ascii	"TEE_PANIC_ID_TEE_FREEOPERATION 0x00000C03\000"
.LASF1261:
	.ascii	"PM_CTX_CCM_CTR1_SIZE 16\000"
.LASF628:
	.ascii	"CFG_WITH_NSEC_UARTS 1\000"
.LASF270:
	.ascii	"__LFRACT_EPSILON__ 0x1P-31LR\000"
.LASF567:
	.ascii	"CFG_STM32MP1_REGULATOR_IOD 1\000"
.LASF102:
	.ascii	"__INT32_MAX__ 0x7fffffff\000"
.LASF882:
	.ascii	"TEE_ERROR_EXCESS_DATA 0xFFFF0004\000"
.LASF919:
	.ascii	"TEE_ORIGIN_TEE 0x00000003\000"
.LASF406:
	.ascii	"__ARM_SIZEOF_MINIMAL_ENUM 4\000"
.LASF956:
	.ascii	"TEE_OPERATION_STATE_ACTIVE 0x00000001\000"
.LASF410:
	.ascii	"__arm__ 1\000"
.LASF499:
	.ascii	"CFG_DDR_LOWPOWER 1\000"
.LASF1240:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSQUAREMOD 0x00001A05\000"
.LASF1114:
	.ascii	"TEE_ECC_CURVE_NIST_P224 0x00000002\000"
.LASF1030:
	.ascii	"TEE_ALG_ECDH_P384 0x80004042\000"
.LASF201:
	.ascii	"__FLT32_MIN_10_EXP__ (-37)\000"
.LASF574:
	.ascii	"CFG_STM32_CLKCALIB 1\000"
.LASF631:
	.ascii	"CFG_WITH_TRNG 1\000"
.LASF426:
	.ascii	"__ARM_FP16_FORMAT_ALTERNATIVE\000"
.LASF1266:
	.ascii	"OFFSET_OF_CMM_CTX_STRUCT(_f) (OFFSET_OF_CTX_STRUCT("
	.ascii	"ccm_ctx) + offsetof(struct ccm_unpg_ctx, _f))\000"
.LASF842:
	.ascii	"PRIdPTR __PRIPTR_PREFIX \"d\"\000"
.LASF190:
	.ascii	"__LDBL_NORM_MAX__ 1.7976931348623157e+308L\000"
.LASF2:
	.ascii	"__STDC_UTF_16__ 1\000"
.LASF1259:
	.ascii	"TEE_SE_READER_NAME_MAX 20\000"
.LASF512:
	.ascii	"CFG_EARLY_TA_COMPRESS 1\000"
.LASF1218:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINIT 0x00001601\000"
.LASF785:
	.ascii	"_GCC_PTRDIFF_T \000"
.LASF1118:
	.ascii	"TEE_ECC_CURVE_25519 0x00000300\000"
.LASF873:
	.ascii	"TEE_ERROR_CORRUPT_OBJECT 0xF0100001\000"
.LASF353:
	.ascii	"__TA_IBIT__ 64\000"
.LASF520:
	.ascii	"CFG_GP_SOCKETS 1\000"
.LASF955:
	.ascii	"TEE_OPERATION_STATE_INITIAL 0x00000000\000"
.LASF973:
	.ascii	"TEE_ALG_DES3_CBC_MAC_NOPAD 0x30000113\000"
.LASF891:
	.ascii	"TEE_ERROR_BUSY 0xFFFF000D\000"
.LASF1164:
	.ascii	"TEE_PANIC_ID_TEE_INITREFATTRIBUTE 0x00000805\000"
.LASF791:
	.ascii	"_SIZE_T \000"
.LASF818:
	.ascii	"_WCHAR_T_H \000"
.LASF697:
	.ascii	"INT_MAX __INT_MAX__\000"
.LASF394:
	.ascii	"__ARM_FEATURE_QRDMX\000"
.LASF715:
	.ascii	"__uint32_t_defined \000"
.LASF1201:
	.ascii	"TEE_PANIC_ID_TEE_AEENCRYPTFINAL 0x00001002\000"
.LASF766:
	.ascii	"INT16_C(v) v\000"
.LASF953:
	.ascii	"TEE_OPERATION_ASYMMETRIC_SIGNATURE 7\000"
.LASF420:
	.ascii	"__ARM_ARCH_ISA_THUMB 2\000"
.LASF76:
	.ascii	"__LONG_LONG_MAX__ 0x7fffffffffffffffLL\000"
.LASF1213:
	.ascii	"TEE_PANIC_ID_TEE_GETTAPERSISTENTTIME 0x00001403\000"
.LASF491:
	.ascii	"CFG_CORE_WORKAROUND_SPECTRE_BP 1\000"
.LASF550:
	.ascii	"CFG_SCMI_MSG_CLOCK 1\000"
.LASF606:
	.ascii	"CFG_TA_MBEDTLS_SELF_TEST 1\000"
.LASF683:
	.ascii	"__compiler_compare_and_swap(p,oval,nval) __atomic_c"
	.ascii	"ompare_exchange_n((p), (oval), (nval), true, __ATOM"
	.ascii	"IC_ACQUIRE, __ATOMIC_RELAXED)\000"
.LASF262:
	.ascii	"__UFRACT_IBIT__ 0\000"
.LASF398:
	.ascii	"__ARM_32BIT_STATE 1\000"
.LASF109:
	.ascii	"__INT8_C(c) c\000"
.LASF267:
	.ascii	"__LFRACT_IBIT__ 0\000"
.LASF661:
	.ascii	"__section(x) __attribute__((section(x)))\000"
.LASF18:
	.ascii	"__SIZEOF_INT__ 4\000"
.LASF167:
	.ascii	"__DBL_MIN_EXP__ (-1021)\000"
.LASF795:
	.ascii	"__SIZE_T \000"
.LASF839:
	.ascii	"PRId16 \"d\"\000"
.LASF47:
	.ascii	"__INT32_TYPE__ int\000"
.LASF546:
	.ascii	"CFG_RESERVED_VASPACE_SIZE (1024 * 1024 * 10)\000"
.LASF1036:
	.ascii	"TEE_TYPE_AES 0xA0000010\000"
.LASF199:
	.ascii	"__FLT32_DIG__ 6\000"
.LASF742:
	.ascii	"INT_FAST32_MIN INT32_MIN\000"
.LASF864:
	.ascii	"PRIX16 \"X\"\000"
.LASF751:
	.ascii	"UINT_FAST64_MAX UINT64_MAX\000"
.LASF674:
	.ascii	"__compiler_bswap64(x) __builtin_bswap64((x))\000"
.LASF633:
	.ascii	"PLATFORM_FLAVOR_ 1\000"
.LASF652:
	.ascii	"__aligned(x) __attribute__((aligned(x)))\000"
.LASF862:
	.ascii	"PRIxPTR __PRIPTR_PREFIX \"x\"\000"
.LASF914:
	.ascii	"TEE_LOGIN_APPLICATION_USER 0x00000005\000"
.LASF194:
	.ascii	"__LDBL_HAS_DENORM__ 1\000"
.LASF383:
	.ascii	"__HAVE_SPECULATION_SAFE_VALUE 1\000"
.LASF286:
	.ascii	"__SACCUM_FBIT__ 7\000"
.LASF710:
	.ascii	"UINT_MAX (INT_MAX * 2U + 1U)\000"
.LASF619:
	.ascii	"CFG_TZDRAM_SIZE 0x02000000\000"
.LASF1056:
	.ascii	"TEE_TYPE_ED25519_PUBLIC_KEY 0xA0000043\000"
.LASF1078:
	.ascii	"TEE_ATTR_DSA_PRIME 0xD0001031\000"
.LASF821:
	.ascii	"_GCC_WCHAR_T \000"
.LASF392:
	.ascii	"__ARM_FEATURE_CRYPTO\000"
.LASF582:
	.ascii	"CFG_STM32_IWDG 1\000"
.LASF486:
	.ascii	"CFG_CORE_RWDATA_NOEXEC 1\000"
.LASF999:
	.ascii	"TEE_ALG_DSA_SHA256 0x70004131\000"
.LASF1210:
	.ascii	"TEE_PANIC_ID_TEE_GENERATERANDOM 0x00001301\000"
.LASF485:
	.ascii	"CFG_CORE_NEX_HEAP_SIZE 16384\000"
.LASF930:
	.ascii	"TEE_DATA_FLAG_ACCESS_WRITE 0x00000002\000"
.LASF126:
	.ascii	"__UINT_LEAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF171:
	.ascii	"__DBL_DECIMAL_DIG__ 17\000"
.LASF257:
	.ascii	"__FRACT_IBIT__ 0\000"
.LASF1028:
	.ascii	"TEE_ALG_ECDH_P224 0x80002042\000"
.LASF1008:
	.ascii	"TEE_ALG_SHA512 0x50000006\000"
.LASF229:
	.ascii	"__FLT64_IS_IEC_60559__ 2\000"
.LASF769:
	.ascii	"UINT32_C(v) U(v)\000"
.LASF557:
	.ascii	"CFG_SECURE_TIME_SOURCE_CNTPCT 1\000"
.LASF1127:
	.ascii	"TEE_PANIC_ID_TEE_GETNEXTPROPERTY 0x00000203\000"
.LASF662:
	.ascii	"__data __section(\".data\")\000"
.LASF892:
	.ascii	"TEE_ERROR_COMMUNICATION 0xFFFF000E\000"
.LASF1196:
	.ascii	"TEE_PANIC_ID_TEE_MACCOMPAREFINAL 0x00000F01\000"
.LASF157:
	.ascii	"__FLT_NORM_MAX__ 3.4028234663852886e+38F\000"
.LASF1273:
	.ascii	"long long unsigned int\000"
.LASF152:
	.ascii	"__FLT_MIN_10_EXP__ (-37)\000"
.LASF732:
	.ascii	"INT_LEAST8_MAX INT8_MAX\000"
.LASF312:
	.ascii	"__ULACCUM_IBIT__ 32\000"
.LASF565:
	.ascii	"CFG_STM32MP15_RSTCTRL 1\000"
.LASF73:
	.ascii	"__SHRT_MAX__ 0x7fff\000"
.LASF197:
	.ascii	"__LDBL_IS_IEC_60559__ 2\000"
.LASF37:
	.ascii	"__PTRDIFF_TYPE__ int\000"
.LASF414:
	.ascii	"__APCS_32__ 1\000"
.LASF79:
	.ascii	"__WINT_MAX__ 0xffffffffU\000"
.LASF1193:
	.ascii	"TEE_PANIC_ID_TEE_CIPHERDOFINAL 0x00000E01\000"
.LASF482:
	.ascii	"CFG_CORE_HUK_SUBKEY_COMPAT 1\000"
.LASF618:
	.ascii	"CFG_TZC400 1\000"
.LASF287:
	.ascii	"__SACCUM_IBIT__ 8\000"
.LASF332:
	.ascii	"__DQ_FBIT__ 63\000"
.LASF680:
	.ascii	"__compiler_add_overflow(a,b,res) __builtin_add_over"
	.ascii	"flow((a), (b), (res))\000"
.LASF781:
	.ascii	"__PTRDIFF_T \000"
.LASF339:
	.ascii	"__UHQ_IBIT__ 0\000"
.LASF601:
	.ascii	"CFG_TA_BGET_TEST 1\000"
.LASF849:
	.ascii	"PRIo16 \"o\"\000"
.LASF615:
	.ascii	"CFG_TEE_IMPL_DESCR OPTEE\000"
.LASF1049:
	.ascii	"TEE_TYPE_DSA_PUBLIC_KEY 0xA0000031\000"
.LASF513:
	.ascii	"CFG_EMBED_DTB 1\000"
.LASF368:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1\000"
.LASF614:
	.ascii	"CFG_TEE_FW_MANUFACTURER FW_MAN_UNDEF\000"
.LASF1040:
	.ascii	"TEE_TYPE_HMAC_MD5 0xA0000001\000"
.LASF896:
	.ascii	"TEE_ERROR_OVERFLOW 0xFFFF300F\000"
.LASF288:
	.ascii	"__SACCUM_MIN__ (-0X1P7HK-0X1P7HK)\000"
.LASF274:
	.ascii	"__ULFRACT_MAX__ 0XFFFFFFFFP-32ULR\000"
.LASF1150:
	.ascii	"TEE_PANIC_ID_TEE_MEMMOVE 0x00000607\000"
.LASF58:
	.ascii	"__UINT_LEAST16_TYPE__ short unsigned int\000"
.LASF431:
	.ascii	"__ARM_FEATURE_FMA\000"
.LASF309:
	.ascii	"__LACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LK\000"
.LASF42:
	.ascii	"__CHAR16_TYPE__ short unsigned int\000"
.LASF1219:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINITFMM 0x00001602\000"
.LASF503:
	.ascii	"CFG_DRAM_SIZE 0x20000000\000"
.LASF231:
	.ascii	"__FLT32X_DIG__ 15\000"
.LASF685:
	.ascii	"__compiler_atomic_store(p,val) __atomic_store_n((p)"
	.ascii	", (val), __ATOMIC_RELAXED)\000"
.LASF344:
	.ascii	"__UTQ_FBIT__ 128\000"
.LASF910:
	.ascii	"TEE_LOGIN_PUBLIC 0x00000000\000"
.LASF982:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA384 0x70005830\000"
.LASF1220:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINITFMMCONTEXT 0x00001603\000"
.LASF17:
	.ascii	"__FINITE_MATH_ONLY__ 0\000"
.LASF130:
	.ascii	"__INT_FAST16_MAX__ 0x7fffffff\000"
.LASF928:
	.ascii	"TEE_STORAGE_PRIVATE 0x00000001\000"
.LASF1202:
	.ascii	"TEE_PANIC_ID_TEE_AEINIT 0x00001003\000"
.LASF462:
	.ascii	"__ILP32__ 1\000"
.LASF1148:
	.ascii	"TEE_PANIC_ID_TEE_MEMCOMPARE 0x00000605\000"
.LASF1018:
	.ascii	"TEE_ALG_HMAC_SHA384 0x30000005\000"
.LASF984:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_MD5SHA1 0x7000F830\000"
.LASF620:
	.ascii	"CFG_TZDRAM_START (0xc0000000 + 0x20000000 - 0x02000"
	.ascii	"000)\000"
.LASF806:
	.ascii	"__size_t \000"
.LASF723:
	.ascii	"INT64_MAX 0x7fffffffffffffffL\000"
.LASF741:
	.ascii	"INT_FAST16_MIN INT16_MIN\000"
.LASF569:
	.ascii	"CFG_STM32MP_CLK_CORE 1\000"
.LASF134:
	.ascii	"__INT_FAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF1106:
	.ascii	"TEE_ATTR_X25519_PRIVATE_VALUE 0xC0000A44\000"
.LASF676:
	.ascii	"__compiler_bswap16(x) __builtin_bswap16((x))\000"
.LASF273:
	.ascii	"__ULFRACT_MIN__ 0.0ULR\000"
.LASF852:
	.ascii	"PRIoPTR __PRIPTR_PREFIX \"o\"\000"
.LASF893:
	.ascii	"TEE_ERROR_SECURITY 0xFFFF000F\000"
.LASF333:
	.ascii	"__DQ_IBIT__ 0\000"
.LASF645:
	.ascii	"__STM32MP_PM_CONTEXT_H__ \000"
.LASF450:
	.ascii	"__ARM_BF16_FORMAT_ALTERNATIVE\000"
.LASF998:
	.ascii	"TEE_ALG_DSA_SHA224 0x70003131\000"
.LASF1199:
	.ascii	"TEE_PANIC_ID_TEE_MACUPDATE 0x00000F04\000"
.LASF1183:
	.ascii	"TEE_PANIC_ID_TEE_ALLOCATEOPERATION 0x00000C01\000"
.LASF761:
	.ascii	"ULL(v) v ## ULL\000"
.LASF519:
	.ascii	"CFG_GIC 1\000"
.LASF75:
	.ascii	"__LONG_MAX__ 0x7fffffffL\000"
.LASF797:
	.ascii	"_BSD_SIZE_T_ \000"
.LASF859:
	.ascii	"PRIx16 \"x\"\000"
.LASF402:
	.ascii	"__ARM_FEATURE_LDREX 15\000"
.LASF599:
	.ascii	"CFG_TA_ASLR_MAX_OFFSET_PAGES 128\000"
.LASF812:
	.ascii	"_T_WCHAR \000"
.LASF932:
	.ascii	"TEE_DATA_FLAG_SHARE_READ 0x00000010\000"
.LASF739:
	.ascii	"UINT_LEAST64_MAX UINT64_MAX\000"
.LASF543:
	.ascii	"CFG_PSCI_ARM32 1\000"
.LASF471:
	.ascii	"CFG_ATTESTATION_PTA_KEY_SIZE 3072\000"
.LASF888:
	.ascii	"TEE_ERROR_NOT_SUPPORTED 0xFFFF000A\000"
.LASF699:
	.ascii	"LONG_MAX __LONG_MAX__\000"
.LASF1242:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCOMPUTEEXTENDEDGCD 0x00001B0"
	.ascii	"1\000"
.LASF851:
	.ascii	"PRIo64 __PRI64_PREFIX \"o\"\000"
.LASF345:
	.ascii	"__UTQ_IBIT__ 0\000"
.LASF1207:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICSIGNDIGEST 0x00001103\000"
.LASF348:
	.ascii	"__SA_FBIT__ 15\000"
.LASF542:
	.ascii	"CFG_PREALLOC_RPC_CACHE 1\000"
.LASF237:
	.ascii	"__FLT32X_MAX__ 1.7976931348623157e+308F32x\000"
.LASF207:
	.ascii	"__FLT32_MIN__ 1.1754943508222875e-38F32\000"
.LASF1051:
	.ascii	"TEE_TYPE_DH_KEYPAIR 0xA1000032\000"
.LASF733:
	.ascii	"INT_LEAST16_MAX INT16_MAX\000"
.LASF749:
	.ascii	"UINT_FAST16_MAX UINT16_MAX\000"
.LASF459:
	.ascii	"__ELF__ 1\000"
.LASF164:
	.ascii	"__FLT_IS_IEC_60559__ 2\000"
.LASF418:
	.ascii	"__THUMBEL__ 1\000"
.LASF1167:
	.ascii	"TEE_PANIC_ID_TEE_RESETTRANSIENTOBJECT 0x00000808\000"
.LASF389:
	.ascii	"__ARM_FEATURE_DSP 1\000"
.LASF327:
	.ascii	"__QQ_IBIT__ 0\000"
.LASF468:
	.ascii	"CFG_ARM32_ldelf 1\000"
.LASF666:
	.ascii	"__rodata __section(\".rodata\" __SECTION_FLAGS_RODA"
	.ascii	"TA)\000"
.LASF595:
	.ascii	"CFG_STM32_UART 1\000"
.LASF646:
	.ascii	"COMPILER_H \000"
.LASF1038:
	.ascii	"TEE_TYPE_DES3 0xA0000013\000"
.LASF316:
	.ascii	"__LLACCUM_FBIT__ 31\000"
.LASF55:
	.ascii	"__INT_LEAST32_TYPE__ int\000"
.LASF41:
	.ascii	"__UINTMAX_TYPE__ long long unsigned int\000"
.LASF889:
	.ascii	"TEE_ERROR_NO_DATA 0xFFFF000B\000"
.LASF664:
	.ascii	"COMMENT_CHAR \"//\"\000"
.LASF341:
	.ascii	"__USQ_IBIT__ 0\000"
.LASF6:
	.ascii	"__GNUC_MINOR__ 3\000"
.LASF727:
	.ascii	"UINT64_MAX 0xffffffffffffffffUL\000"
.LASF404:
	.ascii	"__ARM_FEATURE_NUMERIC_MAXMIN\000"
.LASF40:
	.ascii	"__INTMAX_TYPE__ long long int\000"
.LASF376:
	.ascii	"__GCC_ATOMIC_INT_LOCK_FREE 2\000"
.LASF1187:
	.ascii	"TEE_PANIC_ID_TEE_RESETOPERATION 0x00000C05\000"
.LASF466:
	.ascii	"CFG_AES_GCM_TABLE_BASED 1\000"
.LASF568:
	.ascii	"CFG_STM32MP1_SHARED_RESOURCES 1\000"
.LASF593:
	.ascii	"CFG_STM32_TAMP_NVRAM 1\000"
.LASF694:
	.ascii	"CHAR_BIT __CHAR_BIT__\000"
.LASF857:
	.ascii	"PRIuPTR __PRIPTR_PREFIX \"u\"\000"
.LASF494:
	.ascii	"CFG_CRYPTOLIB_DIR core/lib/libtomcrypt\000"
.LASF684:
	.ascii	"__compiler_atomic_load(p) __atomic_load_n((p), __AT"
	.ascii	"OMIC_RELAXED)\000"
.LASF428:
	.ascii	"__ARM_FEATURE_FP16_SCALAR_ARITHMETIC\000"
.LASF622:
	.ascii	"CFG_WARN_INSECURE 1\000"
.LASF295:
	.ascii	"__USACCUM_EPSILON__ 0x1P-8UHK\000"
.LASF179:
	.ascii	"__DBL_HAS_QUIET_NAN__ 1\000"
.LASF227:
	.ascii	"__FLT64_HAS_INFINITY__ 1\000"
.LASF1280:
	.ascii	"/home/lubuntu/Dokument/GitHub/STM32-Computer/Firmwa"
	.ascii	"re/STM32-Computer-Firmware/CA7/optee-os-3.19.0-r2/o"
	.ascii	"ptee-os-stm32mp-3.19.0-stm32mp-r2\000"
.LASF814:
	.ascii	"_WCHAR_T_ \000"
.LASF279:
	.ascii	"__LLFRACT_MAX__ 0X7FFFFFFFFFFFFFFFP-63LLR\000"
.LASF1211:
	.ascii	"TEE_PANIC_ID_TEE_GETREETIME 0x00001401\000"
.LASF929:
	.ascii	"TEE_DATA_FLAG_ACCESS_READ 0x00000001\000"
.LASF1110:
	.ascii	"TEE_ATTR_BIT_PROTECTED TEE_ATTR_FLAG_PUBLIC\000"
.LASF596:
	.ascii	"CFG_STPMIC1 1\000"
.LASF961:
	.ascii	"TEE_ALG_AES_XTS 0x10000410\000"
.LASF700:
	.ascii	"LONG_MIN (-LONG_MAX - 1L)\000"
.LASF289:
	.ascii	"__SACCUM_MAX__ 0X7FFFP-7HK\000"
.LASF69:
	.ascii	"__INTPTR_TYPE__ int\000"
.LASF539:
	.ascii	"CFG_PM 1\000"
.LASF70:
	.ascii	"__UINTPTR_TYPE__ unsigned int\000"
.LASF1016:
	.ascii	"TEE_ALG_HMAC_SHA224 0x30000003\000"
.LASF948:
	.ascii	"TEE_OPERATION_CIPHER 1\000"
.LASF1080:
	.ascii	"TEE_ATTR_DSA_BASE 0xD0001231\000"
.LASF362:
	.ascii	"__REGISTER_PREFIX__ \000"
.LASF205:
	.ascii	"__FLT32_MAX__ 3.4028234663852886e+38F32\000"
.LASF166:
	.ascii	"__DBL_DIG__ 15\000"
.LASF275:
	.ascii	"__ULFRACT_EPSILON__ 0x1P-32ULR\000"
.LASF877:
	.ascii	"TEE_ERROR_CIPHERTEXT_INVALID 0xF0100006\000"
.LASF1034:
	.ascii	"TEE_ALG_X25519 0x80000044\000"
.LASF25:
	.ascii	"__SIZEOF_SIZE_T__ 4\000"
.LASF52:
	.ascii	"__UINT64_TYPE__ long long unsigned int\000"
.LASF897:
	.ascii	"TEE_ERROR_TARGET_DEAD 0xFFFF3024\000"
.LASF118:
	.ascii	"__INT64_C(c) c ## LL\000"
.LASF1237:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTINVMOD 0x00001A02\000"
.LASF184:
	.ascii	"__LDBL_MIN_10_EXP__ (-307)\000"
.LASF497:
	.ascii	"CFG_CRYPTO_RSASSA_NA1 1\000"
.LASF1079:
	.ascii	"TEE_ATTR_DSA_SUBPRIME 0xD0001131\000"
.LASF1005:
	.ascii	"TEE_ALG_SHA224 0x50000003\000"
.LASF544:
	.ascii	"CFG_REGULATOR_DRIVERS 1\000"
.LASF1180:
	.ascii	"TEE_PANIC_ID_TEE_SEEKOBJECTDATA 0x00000B02\000"
.LASF445:
	.ascii	"__ARM_FEATURE_CDE\000"
.LASF297:
	.ascii	"__ACCUM_IBIT__ 16\000"
.LASF27:
	.ascii	"__BIGGEST_ALIGNMENT__ 8\000"
.LASF1155:
	.ascii	"TEE_PANIC_ID_TEE_GETOBJECTINFO 0x00000703\000"
.LASF65:
	.ascii	"__UINT_FAST8_TYPE__ unsigned char\000"
.LASF907:
	.ascii	"TEE_PARAM_TYPE_MEMREF_INPUT 5\000"
.LASF1116:
	.ascii	"TEE_ECC_CURVE_NIST_P384 0x00000004\000"
.LASF1260:
	.ascii	"PM_CTX_CCM_KEY_SIZE 32\000"
.LASF743:
	.ascii	"INT_FAST64_MIN INT64_MIN\000"
.LASF833:
	.ascii	"PSCI_MODE_SYSTEM_OFF 1\000"
.LASF1132:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASU32 0x00000208\000"
.LASF1123:
	.ascii	"TEE_PANIC_ID_TA_INVOKECOMMANDENTRYPOINT 0x00000104\000"
.LASF1268:
	.ascii	"short int\000"
.LASF775:
	.ascii	"_STDDEF_H \000"
.LASF586:
	.ascii	"CFG_STM32_REGULATOR_GPIO 1\000"
.LASF213:
	.ascii	"__FLT32_IS_IEC_60559__ 2\000"
.LASF958:
	.ascii	"TEE_ALG_AES_CBC_NOPAD 0x10000110\000"
.LASF123:
	.ascii	"__UINT16_C(c) c\000"
.LASF803:
	.ascii	"___int_size_t_h \000"
.LASF980:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA224 0x70003830\000"
.LASF604:
	.ascii	"CFG_TA_MBEDTLS 1\000"
.LASF359:
	.ascii	"__UDA_IBIT__ 32\000"
.LASF1112:
	.ascii	"TEE_CRYPTO_ELEMENT_NONE 0x00000000\000"
.LASF979:
	.ascii	"TEE_ALG_RSASSA_PKCS1_V1_5_SHA1 0x70002830\000"
.LASF9:
	.ascii	"__ATOMIC_RELAXED 0\000"
.LASF338:
	.ascii	"__UHQ_FBIT__ 16\000"
.LASF443:
	.ascii	"__ARM_FEATURE_COPROC\000"
.LASF875:
	.ascii	"TEE_ERROR_STORAGE_NOT_AVAILABLE 0xF0100003\000"
.LASF178:
	.ascii	"__DBL_HAS_INFINITY__ 1\000"
.LASF97:
	.ascii	"__SIG_ATOMIC_MAX__ 0x7fffffff\000"
.LASF1243:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTISPROBABLEPRIME 0x00001B02\000"
.LASF214:
	.ascii	"__FLT64_MANT_DIG__ 53\000"
.LASF524:
	.ascii	"CFG_HWSUPP_MEM_PERM_PXN 1\000"
.LASF843:
	.ascii	"PRIi8 \"i\"\000"
.LASF677:
	.ascii	"__GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * "
	.ascii	"100 + __GNUC_PATCHLEVEL__)\000"
.LASF698:
	.ascii	"INT_MIN (-INT_MAX - 1)\000"
.LASF440:
	.ascii	"__ARM_ARCH_EXT_IDIV__ 1\000"
.LASF472:
	.ascii	"CFG_BOOT_SECONDARY_REQUEST 1\000"
.LASF1031:
	.ascii	"TEE_ALG_ECDH_P521 0x80005042\000"
.LASF1172:
	.ascii	"TEE_PANIC_ID_TEE_RENAMEPERSISTENTOBJECT 0x00000904\000"
.LASF788:
	.ascii	"__need_ptrdiff_t\000"
.LASF506:
	.ascii	"CFG_DRIVERS_CLK_EARLY_PROBE 1\000"
.LASF496:
	.ascii	"CFG_CRYPTOLIB_NAME_tomcrypt 1\000"
.LASF1011:
	.ascii	"TEE_ALG_SHA3_384 0x5000000A\000"
.LASF782:
	.ascii	"_PTRDIFF_T_ \000"
.LASF1003:
	.ascii	"TEE_ALG_MD5 0x50000001\000"
.LASF911:
	.ascii	"TEE_LOGIN_USER 0x00000001\000"
.LASF1128:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASBINARYBLOCK 0x0000020"
	.ascii	"4\000"
.LASF735:
	.ascii	"INT_LEAST64_MAX INT64_MAX\000"
.LASF581:
	.ascii	"CFG_STM32_I2C 1\000"
.LASF817:
	.ascii	"_WCHAR_T_DEFINED \000"
.LASF881:
	.ascii	"TEE_ERROR_ACCESS_CONFLICT 0xFFFF0003\000"
.LASF101:
	.ascii	"__INT16_MAX__ 0x7fff\000"
.LASF421:
	.ascii	"__ARMEL__ 1\000"
.LASF610:
	.ascii	"CFG_TEE_CORE_LOG_LEVEL 2\000"
.LASF1002:
	.ascii	"TEE_ALG_SM2_KEP 0x60000045\000"
.LASF721:
	.ascii	"INT16_MAX 0x7fff\000"
.LASF85:
	.ascii	"__INT_WIDTH__ 32\000"
.LASF405:
	.ascii	"__ARM_FEATURE_SIMD32 1\000"
.LASF562:
	.ascii	"CFG_STACK_TMP_EXTRA 0\000"
.LASF1020:
	.ascii	"TEE_ALG_HMAC_SM3 0x30000007\000"
.LASF759:
	.ascii	"U(v) v ## U\000"
.LASF901:
	.ascii	"TEE_ERROR_TIME_NOT_SET 0xFFFF5000\000"
.LASF326:
	.ascii	"__QQ_FBIT__ 7\000"
.LASF556:
	.ascii	"CFG_SECONDARY_INIT_CNTFRQ 1\000"
.LASF296:
	.ascii	"__ACCUM_FBIT__ 15\000"
.LASF99:
	.ascii	"__SIG_ATOMIC_WIDTH__ 32\000"
.LASF224:
	.ascii	"__FLT64_EPSILON__ 2.2204460492503131e-16F64\000"
.LASF1077:
	.ascii	"TEE_ATTR_RSA_COEFFICIENT 0xC0000830\000"
.LASF951:
	.ascii	"TEE_OPERATION_DIGEST 5\000"
.LASF507:
	.ascii	"CFG_DRIVERS_CLK_FIXED 1\000"
.LASF1189:
	.ascii	"TEE_PANIC_ID_TEE_SETOPERATIONKEY2 0x00000C07\000"
.LASF361:
	.ascii	"__UTA_IBIT__ 64\000"
.LASF322:
	.ascii	"__ULLACCUM_IBIT__ 32\000"
.LASF1126:
	.ascii	"TEE_PANIC_ID_TEE_FREEPROPERTYENUMERATOR 0x00000202\000"
.LASF643:
	.ascii	"DEFINES void __defines(void); void __defines(void)\000"
.LASF285:
	.ascii	"__ULLFRACT_EPSILON__ 0x1P-64ULLR\000"
.LASF1022:
	.ascii	"TEE_ALG_ECDSA_P224 0x70002041\000"
.LASF663:
	.ascii	"__bss __section(\".bss\")\000"
.LASF1001:
	.ascii	"TEE_ALG_DH_DERIVE_SHARED_SECRET 0x80000032\000"
.LASF863:
	.ascii	"PRIX8 \"X\"\000"
.LASF387:
	.ascii	"__SIZEOF_WINT_T__ 4\000"
.LASF28:
	.ascii	"__ORDER_LITTLE_ENDIAN__ 1234\000"
.LASF451:
	.ascii	"__GXX_TYPEINFO_EQUALITY_INLINE 0\000"
.LASF188:
	.ascii	"__LDBL_DECIMAL_DIG__ 17\000"
.LASF308:
	.ascii	"__LACCUM_MIN__ (-0X1P31LK-0X1P31LK)\000"
.LASF774:
	.ascii	"UINTMAX_C(v) UINT64_C(v)\000"
.LASF438:
	.ascii	"__ARM_EABI__ 1\000"
.LASF210:
	.ascii	"__FLT32_HAS_DENORM__ 1\000"
.LASF555:
	.ascii	"CFG_SCMI_PTA 1\000"
.LASF1278:
	.ascii	"GNU C11 12.3.0 -mcpu=cortex-a7 -mfloat-abi=soft -mt"
	.ascii	"humb -mno-unaligned-access -mtls-dialect=gnu -march"
	.ascii	"=armv7ve -g3 -Os -std=gnu11 -ffunction-sections -fd"
	.ascii	"ata-sections -fno-short-enums -fno-common\000"
.LASF323:
	.ascii	"__ULLACCUM_MIN__ 0.0ULLK\000"
.LASF133:
	.ascii	"__INT_FAST32_WIDTH__ 32\000"
.LASF819:
	.ascii	"___int_wchar_t_h \000"
.LASF860:
	.ascii	"PRIx32 \"x\"\000"
.LASF1113:
	.ascii	"TEE_ECC_CURVE_NIST_P192 0x00000001\000"
.LASF1032:
	.ascii	"TEE_ALG_SM2_PKE 0x80000045\000"
.LASF442:
	.ascii	"__ARM_ASM_SYNTAX_UNIFIED__ 1\000"
.LASF1253:
	.ascii	"TEE_MEM_INPUT 0x00000001\000"
.LASF1226:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCMPS32 0x00001802\000"
.LASF481:
	.ascii	"CFG_CORE_HEAP_SIZE 65536\000"
.LASF757:
	.ascii	"UINTMAX_MAX UINT64_MAX\000"
.LASF985:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA1 0x70212930\000"
.LASF396:
	.ascii	"__ARM_FEATURE_DOTPROD\000"
.LASF1061:
	.ascii	"TEE_TYPE_SM2_KEP_KEYPAIR 0xA1000046\000"
.LASF417:
	.ascii	"__thumb2__ 1\000"
.LASF639:
	.ascii	"_CFG_CORE_LTC_VFP 1\000"
.LASF612:
	.ascii	"CFG_TEE_CORE_TA_TRACE 1\000"
.LASF748:
	.ascii	"UINT_FAST8_MAX UINT8_MAX\000"
.LASF1044:
	.ascii	"TEE_TYPE_HMAC_SHA384 0xA0000005\000"
.LASF1206:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICENCRYPT 0x00001102\000"
.LASF1025:
	.ascii	"TEE_ALG_ECDSA_P521 0x70005041\000"
.LASF804:
	.ascii	"_GCC_SIZE_T \000"
.LASF89:
	.ascii	"__WINT_WIDTH__ 32\000"
.LASF283:
	.ascii	"__ULLFRACT_MIN__ 0.0ULLR\000"
.LASF688:
	.ascii	"_STDBOOL_H \000"
.LASF756:
	.ascii	"INTMAX_MIN INT64_MIN\000"
.LASF765:
	.ascii	"UINT8_C(v) v\000"
.LASF381:
	.ascii	"__GCC_CONSTRUCTIVE_SIZE 64\000"
.LASF824:
	.ascii	"_BSD_WCHAR_T_\000"
.LASF880:
	.ascii	"TEE_ERROR_CANCEL 0xFFFF0002\000"
.LASF277:
	.ascii	"__LLFRACT_IBIT__ 0\000"
.LASF884:
	.ascii	"TEE_ERROR_BAD_PARAMETERS 0xFFFF0006\000"
.LASF1064:
	.ascii	"TEE_TYPE_GENERIC_SECRET 0xA0000000\000"
.LASF1149:
	.ascii	"TEE_PANIC_ID_TEE_MEMFILL 0x00000606\000"
.LASF947:
	.ascii	"TEE_HANDLE_FLAG_EXPECT_TWO_KEYS 0x00080000\000"
.LASF588:
	.ascii	"CFG_STM32_RSTCTRL 1\000"
.LASF597:
	.ascii	"CFG_SYSCFG 1\000"
.LASF290:
	.ascii	"__SACCUM_EPSILON__ 0x1P-7HK\000"
.LASF941:
	.ascii	"TEE_USAGE_SIGN 0x00000010\000"
.LASF415:
	.ascii	"__GCC_ASM_FLAG_OUTPUTS__ 1\000"
.LASF1091:
	.ascii	"TEE_ATTR_ECC_PUBLIC_VALUE_X 0xD0000141\000"
.LASF695:
	.ascii	"CHAR_MAX UCHAR_MAX\000"
.LASF886:
	.ascii	"TEE_ERROR_ITEM_NOT_FOUND 0xFFFF0008\000"
.LASF457:
	.ascii	"__unix__ 1\000"
.LASF66:
	.ascii	"__UINT_FAST16_TYPE__ unsigned int\000"
.LASF355:
	.ascii	"__UHA_IBIT__ 8\000"
.LASF992:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA224 0x60310230\000"
.LASF300:
	.ascii	"__ACCUM_EPSILON__ 0x1P-15K\000"
.LASF315:
	.ascii	"__ULACCUM_EPSILON__ 0x1P-32ULK\000"
.LASF918:
	.ascii	"TEE_ORIGIN_COMMS 0x00000002\000"
.LASF182:
	.ascii	"__LDBL_DIG__ 15\000"
.LASF1109:
	.ascii	"TEE_ATTR_FLAG_VALUE (1 << 29)\000"
.LASF91:
	.ascii	"__SIZE_WIDTH__ 32\000"
.LASF673:
	.ascii	"__nostackcheck __attribute__((no_instrument_functio"
	.ascii	"n))\000"
.LASF651:
	.ascii	"__pure __attribute__((pure))\000"
.LASF80:
	.ascii	"__WINT_MIN__ 0U\000"
.LASF1138:
	.ascii	"TEE_PANIC_ID_TEE_CLOSETASESSION 0x00000401\000"
.LASF215:
	.ascii	"__FLT64_DIG__ 15\000"
.LASF1228:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTGETBITCOUNT 0x00001804\000"
.LASF868:
	.ascii	"TEE_API_DEFINES_H \000"
.LASF470:
	.ascii	"CFG_ARM_GIC_PM 1\000"
.LASF110:
	.ascii	"__INT_LEAST8_WIDTH__ 8\000"
.LASF885:
	.ascii	"TEE_ERROR_BAD_STATE 0xFFFF0007\000"
.LASF54:
	.ascii	"__INT_LEAST16_TYPE__ short int\000"
.LASF325:
	.ascii	"__ULLACCUM_EPSILON__ 0x1P-32ULLK\000"
.LASF1281:
	.ascii	"__defines\000"
.LASF172:
	.ascii	"__DBL_MAX__ ((double)1.7976931348623157e+308L)\000"
.LASF1271:
	.ascii	"short unsigned int\000"
.LASF276:
	.ascii	"__LLFRACT_FBIT__ 63\000"
.LASF211:
	.ascii	"__FLT32_HAS_INFINITY__ 1\000"
.LASF1175:
	.ascii	"TEE_PANIC_ID_TEE_FREEPERSISTENTOBJECTENUMERATOR 0x0"
	.ascii	"0000A02\000"
.LASF517:
	.ascii	"CFG_FTRACE_BUF_WHEN_FULL shift\000"
.LASF1233:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTNEG 0x00001904\000"
.LASF583:
	.ascii	"CFG_STM32_LOWPOWER_SIP 1\000"
.LASF872:
	.ascii	"TEE_SUCCESS 0x00000000\000"
.LASF553:
	.ascii	"CFG_SCMI_MSG_SHM_MSG 1\000"
.LASF840:
	.ascii	"PRId32 \"d\"\000"
.LASF492:
	.ascii	"CFG_CORE_WORKAROUND_SPECTRE_BP_SEC 1\000"
.LASF1205:
	.ascii	"TEE_PANIC_ID_TEE_ASYMMETRICDECRYPT 0x00001101\000"
.LASF328:
	.ascii	"__HQ_FBIT__ 15\000"
.LASF692:
	.ascii	"__bool_true_false_are_defined 1\000"
.LASF1215:
	.ascii	"TEE_PANIC_ID_TEE_WAIT 0x00001405\000"
.LASF861:
	.ascii	"PRIx64 __PRI64_PREFIX \"x\"\000"
.LASF607:
	.ascii	"CFG_TA_STRICT_ANNOTATION_CHECKS 1\000"
.LASF489:
	.ascii	"CFG_CORE_UNMAP_CORE_AT_EL0 1\000"
.LASF82:
	.ascii	"__SIZE_MAX__ 0xffffffffU\000"
.LASF124:
	.ascii	"__UINT_LEAST32_MAX__ 0xffffffffU\000"
.LASF464:
	.ascii	"NDEBUG 1\000"
.LASF1239:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTMULMOD 0x00001A04\000"
.LASF830:
	.ascii	"_GCC_MAX_ALIGN_T \000"
.LASF1194:
	.ascii	"TEE_PANIC_ID_TEE_CIPHERINIT 0x00000E02\000"
.LASF411:
	.ascii	"__ARM_ARCH\000"
.LASF458:
	.ascii	"unix 1\000"
.LASF1264:
	.ascii	"PM_CTX_CCM_TAG_SIZE 16\000"
.LASF247:
	.ascii	"__SFRACT_IBIT__ 0\000"
.LASF1095:
	.ascii	"TEE_ATTR_SM2_ID_INITIATOR 0xD0000446\000"
.LASF590:
	.ascii	"CFG_STM32_SAES_SW_FALLBACK 1\000"
.LASF1241:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTSUBMOD 0x00001A06\000"
.LASF927:
	.ascii	"TEE_MALLOC_FILL_ZERO 0x00000000\000"
.LASF1203:
	.ascii	"TEE_PANIC_ID_TEE_AEUPDATE 0x00001004\000"
.LASF617:
	.ascii	"CFG_TEE_TA_LOG_LEVEL 1\000"
.LASF625:
	.ascii	"CFG_WERROR 1\000"
.LASF1186:
	.ascii	"TEE_PANIC_ID_TEE_GETOPERATIONINFO 0x00000C04\000"
.LASF650:
	.ascii	"__noreturn __attribute__((__noreturn__))\000"
.LASF636:
	.ascii	"_CFG_CORE_LTC_MPI 1\000"
.LASF495:
	.ascii	"CFG_CRYPTOLIB_NAME tomcrypt\000"
.LASF856:
	.ascii	"PRIu64 __PRI64_PREFIX \"u\"\000"
.LASF793:
	.ascii	"_T_SIZE_ \000"
.LASF1129:
	.ascii	"TEE_PANIC_ID_TEE_GETPROPERTYASBOOL 0x00000205\000"
.LASF776:
	.ascii	"_STDDEF_H_ \000"
.LASF280:
	.ascii	"__LLFRACT_EPSILON__ 0x1P-63LLR\000"
.LASF1055:
	.ascii	"TEE_TYPE_ECDH_KEYPAIR 0xA1000042\000"
.LASF249:
	.ascii	"__SFRACT_MAX__ 0X7FP-7HR\000"
.LASF230:
	.ascii	"__FLT32X_MANT_DIG__ 53\000"
.LASF88:
	.ascii	"__WCHAR_WIDTH__ 32\000"
.LASF1084:
	.ascii	"TEE_ATTR_DH_SUBPRIME 0xD0001132\000"
.LASF939:
	.ascii	"TEE_USAGE_DECRYPT 0x00000004\000"
.LASF713:
	.ascii	"__STDINT_H \000"
.LASF112:
	.ascii	"__INT16_C(c) c\000"
.LASF708:
	.ascii	"UCHAR_MAX (SCHAR_MAX * 2 + 1)\000"
.LASF952:
	.ascii	"TEE_OPERATION_ASYMMETRIC_CIPHER 6\000"
.LASF990:
	.ascii	"TEE_ALG_RSAES_PKCS1_V1_5 0x60000130\000"
.LASF351:
	.ascii	"__DA_IBIT__ 32\000"
.LASF605:
	.ascii	"CFG_TA_MBEDTLS_MPI 1\000"
.LASF1076:
	.ascii	"TEE_ATTR_RSA_EXPONENT2 0xC0000730\000"
.LASF1098:
	.ascii	"TEE_ATTR_SM2_KEP_CONFIRMATION_IN 0xD0000746\000"
.LASF993:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA256 0x60410230\000"
.LASF1048:
	.ascii	"TEE_TYPE_RSA_KEYPAIR 0xA1000030\000"
.LASF138:
	.ascii	"__UINT_FAST32_MAX__ 0xffffffffU\000"
.LASF329:
	.ascii	"__HQ_IBIT__ 0\000"
.LASF453:
	.ascii	"__linux 1\000"
.LASF168:
	.ascii	"__DBL_MIN_10_EXP__ (-307)\000"
.LASF1073:
	.ascii	"TEE_ATTR_RSA_PRIME1 0xC0000430\000"
.LASF559:
	.ascii	"CFG_SM_PLATFORM_HANDLER 1\000"
.LASF887:
	.ascii	"TEE_ERROR_NOT_IMPLEMENTED 0xFFFF0009\000"
.LASF987:
	.ascii	"TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256 0x70414930\000"
.LASF1070:
	.ascii	"TEE_ATTR_RSA_MODULUS 0xD0000130\000"
.LASF1089:
	.ascii	"TEE_ATTR_RSA_OAEP_LABEL 0xD0000930\000"
.LASF1119:
	.ascii	"TEE_ECC_CURVE_SM2 0x00000400\000"
.LASF1223:
	.ascii	"TEE_PANIC_ID_TEE_BIGINTCONVERTTOOCTETSTRING 0x00001"
	.ascii	"703\000"
.LASF696:
	.ascii	"CHAR_MIN 0\000"
.LASF1171:
	.ascii	"TEE_PANIC_ID_TEE_OPENPERSISTENTOBJECT 0x00000903\000"
.LASF206:
	.ascii	"__FLT32_NORM_MAX__ 3.4028234663852886e+38F32\000"
.LASF969:
	.ascii	"TEE_ALG_DES_CBC_MAC_NOPAD 0x30000111\000"
.LASF1069:
	.ascii	"TEE_ATTR_SECRET_VALUE 0xC0000000\000"
.LASF641:
	.ascii	"_CFG_SCMI_PTA_MSG_HEADER 1\000"
.LASF755:
	.ascii	"INTMAX_MAX INT64_MAX\000"
.LASF94:
	.ascii	"__UINTMAX_MAX__ 0xffffffffffffffffULL\000"
.LASF165:
	.ascii	"__DBL_MANT_DIG__ 53\000"
.LASF1042:
	.ascii	"TEE_TYPE_HMAC_SHA224 0xA0000003\000"
.LASF272:
	.ascii	"__ULFRACT_IBIT__ 0\000"
.LASF671:
	.ascii	"__nex_data \000"
.LASF687:
	.ascii	"fallthrough __attribute__((__fallthrough__))\000"
.LASF1065:
	.ascii	"TEE_TYPE_CORRUPTED_OBJECT 0xA00000BE\000"
.LASF611:
	.ascii	"CFG_TEE_CORE_NB_CORE 2\000"
.LASF630:
	.ascii	"CFG_WITH_STATS 1\000"
.LASF730:
	.ascii	"INT_LEAST32_MIN INT32_MIN\000"
.LASF74:
	.ascii	"__INT_MAX__ 0x7fffffff\000"
.LASF56:
	.ascii	"__INT_LEAST64_TYPE__ long long int\000"
.LASF960:
	.ascii	"TEE_ALG_AES_CTS 0x10000310\000"
.LASF1035:
	.ascii	"TEE_ALG_ILLEGAL_VALUE 0xEFFFFFFF\000"
.LASF1054:
	.ascii	"TEE_TYPE_ECDH_PUBLIC_KEY 0xA0000042\000"
.LASF446:
	.ascii	"__ARM_FEATURE_CDE_COPROC\000"
.LASF994:
	.ascii	"TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA384 0x60510230\000"
.LASF736:
	.ascii	"UINT_LEAST8_MAX UINT8_MAX\000"
	.ident	"GCC: (GNU) 12.3.0"
	.section	.note.GNU-stack,"",%progbits
