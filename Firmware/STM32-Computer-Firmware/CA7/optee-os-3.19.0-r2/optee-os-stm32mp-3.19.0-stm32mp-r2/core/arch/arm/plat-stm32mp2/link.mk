include core/arch/arm/kernel/link.mk

#
# Add target 'all_dtbs' that builds all the dtb for current PLATFORM
#
ifneq ($(filter all_dtbs,$(MAKECMDGOALS)),)

.PHONY: all_dtbs

define add-dts-and-build
# DTS file name in $1
dts-file := $(if $(wildcard $(arch-dir)/dts/$1), \
	$(arch-dir)/dts/$1, \
	$(CFG_EXT_DTS)/$1)
dtb-file := $(out-dir)/$(arch-dir)/dts/$(1:.dts=.dtb)
$$(eval $$(call gen-dtb-file,$$(dts-file),$$(dtb-file)))

all_dtbs: $$(dtb-file)

endef

$(foreach dts, \
	$(filter-out $(CFG_EMBED_DTB_SOURCE_FILE),$(ALL_DTS)), \
	$(eval $(call add-dts-and-build,$(dts))) \
)

all_dtbs: $(out-dir)/$(arch-dir)/dts/$(CFG_EMBED_DTB_SOURCE_FILE:.dts=.dtb)

endif
