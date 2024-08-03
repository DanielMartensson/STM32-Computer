/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2022, STMicroelectronics
 */

#ifndef __DRIVERS_STM32_ADC_CORE_H
#define __DRIVERS_STM32_ADC_CORE_H

#define STM32_ADCX_COMN_OFFSET		U(0x300)

/* STM32MP13 - common registers for all ADC instances */
#define STM32MP13_ADC_CSR		(STM32_ADCX_COMN_OFFSET + 0x00)
#define STM32MP13_ADC_CCR		(STM32_ADCX_COMN_OFFSET + 0x08)

/* STM32MP13_ADC_CSR - bit fields */
#define STM32MP13_AWD3			BIT(9)
#define STM32MP13_AWD2			BIT(8)
#define STM32MP13_AWD1			BIT(7)

/* STM32MP13_ADC_CCR - bit fields */
#define STM32MP13_VBATEN		BIT(24)
#define STM32MP13_VREFEN		BIT(22)
#define STM32MP13_PRESC_SHIFT		U(18)
#define STM32MP13_PRESC_MASK		GENMASK_32(21, 18)
#define STM32MP13_CKMODE_SHIFT		U(16)
#define STM32MP13_CKMODE_MASK		GENMASK_32(17, 16)

struct stm32_adc_core_device;

struct stm32_adc_common {
	struct stm32_adc_core_device *dev;
	vaddr_t regs;
	unsigned long rate;
	uint16_t vref_mv;
};

#endif /* __DRIVERS_STM32_ADC_CORE_H */
