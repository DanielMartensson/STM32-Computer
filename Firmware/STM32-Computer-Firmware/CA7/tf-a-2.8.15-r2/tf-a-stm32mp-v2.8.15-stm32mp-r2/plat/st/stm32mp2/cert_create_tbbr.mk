#
# Copyright (c) 2023, STMicroelectronics - All Rights Reserved
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Override TBBR Cert to update generic certificate
$(eval $(call add_define,PLAT_DEF_OID))
$(eval $(call add_define,PDEF_CERTS))
$(eval $(call add_define,PDEF_EXTS))

PLAT_INCLUDE	+= -I${PLAT_DIR}include

src/stm32mp2_tbb_cert.o: ${PLAT_DIR}stm32mp2_tbb_cert.c
	${Q}${HOSTCC} -c ${HOSTCCFLAGS} ${INC_DIR} $< -o $@

PLAT_OBJECTS	= src/stm32mp2_tbb_cert.o

OBJECTS		+= $(PLAT_OBJECTS)

