global-incdirs-y += .

srcs-y += main.c
srcs-$(CFG_SCMI_MSG_DRIVERS) += scmi_server.c
srcs-$(CFG_SCMI_SCPFW) += scmi_server_scpfw.c
srcs-$(CFG_DISPLAY) += display.c
subdirs-y += drivers
subdirs-y += pm
