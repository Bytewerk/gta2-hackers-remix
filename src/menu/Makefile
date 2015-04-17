# Automatic C dependency detection (just gcc, no make recursion etc)
CC:=LANG=C gcc -std=c99 -Wall -g -Werror
DEPS=$(shell $(CC) -MM $(1) | cut -d ':' -f 2 | tr -d '\n\\')
OBJS=$(shell grep obj/ Makefile | grep 'call DEPS' | grep -v OBJS | cut -d':' -f 1 | tr '\n' ' ')

all: obj out out/menu.bin


# MENU
out/menu.bin: $(OBJS)
	$(CC) `pkg-config --cflags --libs sdl2 SDL2_image SDL2_net` -o $@ obj/*.o

obj/menu.o: $(call DEPS,main.c)
	$(CC) -c -o $@ $<

# STY
obj/sty_sprite.o: $(call DEPS,sty/sprite.c)
	$(CC) -c -o $@ $<

obj/sty_text.o: $(call DEPS,sty/text.c)
	$(CC) -c -o $@ $<

obj/sty.o: $(call DEPS,sty/sty.c)
	$(CC) -c -o $@ $<

# SFX
obj/sfx.o: $(call DEPS,sfx/sfx.c)
	$(CC) -c -o $@ $<

# UI
obj/ui.o: $(call DEPS,ui/interface.c)
	$(CC) -c -o $@ $<

# BG
obj/bg.o: $(call DEPS,bg/background.c)
	$(CC) -c -o $@ $<

# TK
obj/tk_el.o: $(call DEPS,tk/elements.c)
	$(CC) -c -o $@ $<

obj/tk_captions.o: $(call DEPS,tk/captions.c)
	$(CC) -c -o $@ $<

obj/tk_ctrl.o: $(call DEPS,tk/controls.c)
	$(CC) -c -o $@ $<

obj/tk_screen.o: $(call DEPS,tk/screen.c)
	$(CC) -c -o $@ $<

obj/tk_actions.o: $(call DEPS,tk/actions.c)
	$(CC) -c -o $@ $<

obj/tk_screen_actions.o: $(call DEPS,tk/screen_actions.c)
	$(CC) -c -o $@ $<

obj/tk.o: $(call DEPS,tk/toolkit.c)
	$(CC) -c -o $@ $<


# Other
obj out:
	mkdir -p $@
clean:
	rm -r out obj || true
.PHONY: all clean
