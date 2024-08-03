srcs-y += tee_tui.c
srcs-y += tui_pta.c
srcs-y += tui_widget.c
srcs-y += tui_keyboard.c
srcs-y += tui_entry.c
srcs-y += utf8.c
srcs-y += image.c
srcs-y += image_png.c
srcs-y += font.c

# Need to locate font.h from generated sources
incdirs-y += .

render_font:=scripts/render_font.py

gensrcs-y += default_bold
force-gensrc-default_bold = y
produce-additional-default_bold = default_bold.h
produce-default_bold = default_bold.c
depends-default_bold := $(render_font) \
		$(sub-dir)/fonts/amble/Amble-Bold.ttf
recipe-default_bold := $(PYTHON3) $(render_font) \
		--font_file $(sub-dir)/fonts/amble/Amble-Bold.ttf \
		--font_size 20 --font_name default_bold \
		--out_dir $(sub-dir-out)
cleanfiles += $(sub-dir-out)/default_bold.c $(sub-dir-out)/default_bold.h

gensrcs-y += default_regular
force-gensrc-default_regular = y
produce-additional-default_regular = default_regular.h
produce-default_regular = default_regular.c
depends-default_regular := $(render_font) \
		$(sub-dir)/fonts/amble/Amble-Regular.ttf
recipe-default_regular := $(PYTHON3) $(render_font) \
		--font_file $(sub-dir)/fonts/amble/Amble-Regular.ttf \
		--font_size 20 --font_name default_regular \
		--out_dir $(sub-dir-out)
cleanfiles += $(sub-dir-out)/default_regular.c $(sub-dir-out)/default_regular.h
