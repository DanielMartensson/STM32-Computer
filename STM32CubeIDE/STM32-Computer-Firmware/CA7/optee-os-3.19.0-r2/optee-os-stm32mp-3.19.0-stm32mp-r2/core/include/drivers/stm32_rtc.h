/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2017-2018, STMicroelectronics - All Rights Reserved
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 */

#ifndef __PLAT_RTC_H__
#define __PLAT_RTC_H__

#include <stdbool.h>
#include <tee_api_types.h>

struct stm32_rtc_calendar {
	uint32_t ssr;
	uint32_t tr;
	uint32_t dr;
};

enum months {
	JANUARY = 1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER,
	NB_MONTHS = 12
};

struct stm32_rtc_time {
	uint32_t hour;
	uint32_t min;
	uint32_t sec;
	uint32_t wday;
	uint32_t day;
	enum months month;
	uint32_t year;
};

/*
 * Get calendar formatted time from RTC device
 * Returns TEE_Success on success, TEE_ERROR_* otherwise.
 */
TEE_Result stm32_rtc_get_calendar(struct stm32_rtc_calendar *calendar);

/*
 * Return time diff in milliseconds between current and reference time
 * System will panic if stm32_rtc_calendar "cur" is older than "ref".
 */
unsigned long long stm32_rtc_diff_calendar_ms(struct stm32_rtc_calendar *cur,
					      struct stm32_rtc_calendar *ref);

/*
 * Return time diff in tick count between current and reference time
 * System will panic if stm32_rtc_calendar "cur" is older than "ref".
 */
unsigned long long stm32_rtc_diff_calendar_tick(struct stm32_rtc_calendar *cur,
						struct stm32_rtc_calendar *ref,
						unsigned long tick_rate);

/*
 * Enable tamper and secure timestamp access in RTC
 * Returns TEE_Success on success, TEE_ERROR_* otherwise.
 */

TEE_Result stm32_rtc_set_tamper_timestamp(void);

/*
 * *ret equals true if and only if RTC timestamp is enable
 * Returns TEE_Success on a successful read.
 */
TEE_Result stm32_rtc_is_timestamp_enable(bool *ret);

/*
 * Get RTC timestamp for current time
 * Returns TEE_Success on success, TEE_ERROR_* otherwise.
 */
TEE_Result stm32_rtc_get_timestamp(struct stm32_rtc_time *tamp_ts);

TEE_Result stm32_rtc_driver_is_initialized(void);

#endif /* __PLAT_RTC_H__ */
