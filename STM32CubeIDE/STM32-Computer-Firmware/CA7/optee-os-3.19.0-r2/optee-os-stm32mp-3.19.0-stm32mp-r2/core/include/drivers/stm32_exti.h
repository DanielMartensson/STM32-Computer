/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021, STMicroelectronics
 */

#ifndef __STM32_EXTI_H
#define __STM32_EXTI_H

#include <tee_api_types.h>

#define EXTI_TYPE_RISING	1
#define EXTI_TYPE_FALLING	2
#define EXTI_TYPE_BOTH	(EXTI_TYPE_RISING | EXTI_TYPE_FALLING)

struct stm32_exti_pdata;

/*
 * Set EXTI type in RTSR and FTSR EXTI registers
 * @exti_line: EXTI line number
 * @type: type (rising, falling or both) to set
 * Return a TEE_Result compliant return value
 */
void stm32_exti_set_type(struct stm32_exti_pdata *exti, uint32_t exti_line,
			 uint32_t type);

/*
 * Mask EXTI Interrupt (IMR)
 * @exti_line: EXTI line number
 * Return a TEE_Result compliant return value
 */
void stm32_exti_mask(struct stm32_exti_pdata *exti, uint32_t exti_line);

/*
 * Unmask EXTI Interrupt (IMR)
 * @exti_line: EXTI line number
 * Return a TEE_Result compliant return value
 */
void stm32_exti_unmask(struct stm32_exti_pdata *exti, uint32_t exti_line);

/*
 * Enable EXTI line as wakeup interrupt
 * @exti_line: EXTI line number
 * Return a TEE_Result compliant return value
 */
void stm32_exti_enable_wake(struct stm32_exti_pdata *exti, uint32_t exti_line);

/*
 * Disable EXTI line as wakeup interrupt
 * @exti_line: EXTI line number
 * Return a TEE_Result compliant return value
 */
void stm32_exti_disable_wake(struct stm32_exti_pdata *exti, uint32_t exti_line);

/*
 * Clear pending EXTI interrupts
 * @exti_line: EXTI line number
 * Return a TEE_Result compliant return value
 */
void stm32_exti_clear(struct stm32_exti_pdata *exti, uint32_t exti_line);

/*
 * Configure EXTI mux for GPIO irq
 * @bank: GPIO bank id
 * @pin: GPIO number in the bank
 * Return a TEE_Result compliant return value
 */
void stm32_exti_set_gpio_port_sel(struct stm32_exti_pdata *exti, uint8_t bank,
				  uint8_t pin);

/*
 * Securize the EXTI line
 * @exti_line: EXTI line number
 * Return a TEE_Result compliant return value
 */
void stm32_exti_set_tz(struct stm32_exti_pdata *exti, uint32_t exti_line);

#endif /*__STM32_EXTI_H*/
