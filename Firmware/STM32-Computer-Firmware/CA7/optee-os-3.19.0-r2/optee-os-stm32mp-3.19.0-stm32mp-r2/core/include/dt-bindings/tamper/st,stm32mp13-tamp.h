/* SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-3-Clause) */
/*
 * Copyright (C) STMicroelectronics 2023 - All Rights Reserved
 * Author: Gatien Chevallier <gatien.chevallier@foss.st.com>
 */

/* Internal Tampers */
#define INT_TAMPER_BKUP_DOMAIN_THRESHOLD	1
#define INT_TAMPER_TEMPERATURE_MONITORING	2
#define INT_TAMPER_LSE_MONITORING		3
#define INT_TAMPER_HSE_MONITORING		4
#define INT_TAMPER_RTC_CALENDAR_OVERFLOW	5
#define INT_TAMPER_JTAG_SWD_ACCESS		6
#define INT_TAMPER_ADC2_AWD_MONITORING_1	7
#define INT_TAMPER_MONOTONIC_COUNTER_1		8
#define INT_TAMPER_CRYPTO_PERIPH_FAULT		9
#define INT_TAMPER_MONOTONIC_COUNTER_2		10
#define INT_TAMPER_IWDG1_RESET			11
#define INT_TAMPER_ADC2_AWD_MONITORING_2	12
#define INT_TAMPER_ADC2_AWD_MONITORING_3	13

/* External Tampers */
#define EXT_TAMPER_1				1
#define EXT_TAMPER_2				2
#define EXT_TAMPER_3				3
#define EXT_TAMPER_4				4
#define EXT_TAMPER_5				5
#define EXT_TAMPER_6				6
#define EXT_TAMPER_7				7
#define EXT_TAMPER_8				8

/* Tamper mode */
#define TAMPER_CONFIRMED_MODE			0
#define TAMPER_POTENTIAL_MODE			2
#define TAMPER_MASKED_MODE			4
