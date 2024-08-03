/* SPDX-License-Identifier: GPL-2.0 or BSD-3-Clause */
/*
 * Copyright (C) STMicroelectronics 2022 - All Rights Reserved
 * Author: Gatien Chevallier <gatien.chevallier@st.com> for STMicroelectronics.
 */

#ifndef _DT_BINDINGS_GPIO_STM32MP_GPIO_H
#define _DT_BINDINGS_GPIO_STM32MP_GPIO_H

#include <dt-bindings/gpio/gpio.h>

/* Macro to define the security for GPIO */
#define TZPROT(id) (1 << (id))

#endif
