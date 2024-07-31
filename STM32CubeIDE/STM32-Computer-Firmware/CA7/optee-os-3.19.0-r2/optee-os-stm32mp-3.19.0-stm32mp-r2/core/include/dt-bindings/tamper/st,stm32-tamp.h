/* SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause) */
/*
 * Copyright (C) 2023, STMicroelectronics - All Rights Reserved
 * Author: Gatien Chevallier <gatien.chevallier@foss.st.com>
 */

/* Internal Tampers */
#define INT_TAMPER_RTC_VOLTAGE_MONITORING	1
#define INT_TAMPER_TEMPERATURE_MONITORING	2
#define INT_TAMPER_LSE_MONITORING		3
#define INT_TAMPER_HSE_MONITORING		4
#define INT_TAMPER_RTC_CALENDAR_OVERFLOW	5
/* Nothing for tampers 6-7*/
#define INT_TAMPER_6				6
#define INT_TAMPER_7				7
#define INT_TAMPER_MONOTONIC_COUNTER_		8

/* External Tampers */
#define EXT_TAMPER_1				1
#define EXT_TAMPER_2				2
#define EXT_TAMPER_3				3

/* Tamper mode */
#define TAMPER_CONFIRMED_MODE			0
#define TAMPER_POTENTIAL_MODE			2
#define TAMPER_MASKED_MODE			4
