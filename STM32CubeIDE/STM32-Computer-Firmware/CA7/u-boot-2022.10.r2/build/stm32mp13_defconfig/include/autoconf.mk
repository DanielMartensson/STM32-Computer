CONFIG_SYS_MAX_NAND_DEVICE=y
CONFIG_EXTRA_ENV_SETTINGS="STM32MP_MEM_LAYOUT ST_STM32MP13_BOOTCMD BOOTENV STM32MP_EXTRA STM32MP_BOARD_EXTRA_ENV"
CONFIG_BOARDDIR="board/st/stm32mp1"
CONFIG_SYS_BAUDRATE_TABLE="{ 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 1000000, 2000000, 4000000}"
CONFIG_SYS_SDRAM_BASE="STM32_DDR_BASE"
CONFIG_SYS_MMC_MAX_DEVICE=2
CONFIG_SERVERIP="192.168.1.1"
CONFIG_SYS_BOOTMAPSZ="SZ_256M"
CONFIG_SYS_NONCACHED_MEMORY="(1 * SZ_1M)"