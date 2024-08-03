/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2021, STMicroelectronics
 */

#ifndef __STM32MP1_SYSCFG_H
#define __STM32MP1_SYSCFG_H

/*
 * HSLV definitions
 */
#define SYSCFG_HSLV_IDX_TPIU			U(0)
#define SYSCFG_HSLV_IDX_QSPI			U(1)
#define SYSCFG_HSLV_IDX_ETH1			U(2)
#define SYSCFG_HSLV_IDX_ETH2			U(3)
#define SYSCFG_HSLV_IDX_SDMMC1			U(4)
#define SYSCFG_HSLV_IDX_SDMMC2			U(5)
#define SYSCFG_HSLV_IDX_SPI1			U(6)
#define SYSCFG_HSLV_IDX_SPI2			U(7)
#define SYSCFG_HSLV_IDX_SPI3			U(8)
#define SYSCFG_HSLV_IDX_SPI4			U(9)
#define SYSCFG_HSLV_IDX_SPI5			U(10)
#define SYSCFG_HSLV_IDX_LTDC			U(11)
#define SYSCFG_HSLV_NB_IDX			U(12)

#define SYSCFG_IO_COMP_IDX_SD1			U(0)
#define SYSCFG_IO_COMP_IDX_SD2			U(1)
#define SYSCFG_IO_COMP_NB_IDX			U(2)

void stm32mp_set_hslv_by_index(uint32_t index, bool state);

void stm32mp_set_io_comp_by_index(uint32_t index, bool state);

#endif /*__STM32MP1_SYSCFG_H*/
