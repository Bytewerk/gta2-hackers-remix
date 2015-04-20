# Automatic C dependency detection (just gcc, no make recursion etc)
# High level at the top
CC:=LANG=C gcc -std=c99 -Wall -g -Werror
DEPS=$(shell $(CC) -MM $(1) | cut -d ':' -f 2 | tr -d '\n\\')
OBJS=$(shell grep obj/ Makefile | grep 'call DEPS' | grep -v OBJS | cut -d':' -f 1 | tr '\n' ' ')

all: obj out out/menu.bin


# MENU
out/menu.bin: $(OBJS)
	$(CC) `pkg-config --cflags --libs sdl2 SDL2_image SDL2_net` -o $@ obj/*.o

obj/menu.o: $(call DEPS,main.c)
	$(CC) -c -o $@ $<

# UI: SCREENS
obj/screen_main.o: $(call DEPS,ui/screens/main_menu.c)
	$(CC) -c -o $@ $<

obj/screen_play.o: $(call DEPS,ui/screens/play.c)
	$(CC) -c -o $@ $<

obj/screen_credits.o: $(call DEPS,ui/screens/credits.c)
	$(CC) -c -o $@ $<

obj/screen_scores.o: $(call DEPS,ui/screens/scores.c)
	$(CC) -c -o $@ $<

obj/screen_splitscreen.o: $(call DEPS,ui/screens/splitscreen.c)
	$(CC) -c -o $@ $<
	
# UI
obj/ui.o: $(call DEPS,ui/interface.c)
	$(CC) -c -o $@ $<

# TK: CTRL
obj/ctrl_arrow.o: $(call DEPS,tk/ctrl/arrow.c)
	$(CC) -c -o $@ $<

obj/ctrl_arrowtext.o: $(call DEPS,tk/ctrl/arrowtext.c)
	$(CC) -c -o $@ $<
	
obj/ctrl_button.o: $(call DEPS,tk/ctrl/button.c)
	$(CC) -c -o $@ $<

obj/ctrl_circle.o: $(call DEPS,tk/ctrl/circle.c)
	$(CC) -c -o $@ $<

obj/ctrl_boolean.o: $(call DEPS,tk/ctrl/boolean.c)
	$(CC) -c -o $@ $<

# TK
obj/tk_el.o: $(call DEPS,tk/elements.c)
	$(CC) -c -o $@ $<

obj/tk_captions.o: $(call DEPS,tk/captions.c)
	$(CC) -c -o $@ $<

obj/tk_screen.o: $(call DEPS,tk/screen.c)
	$(CC) -c -o $@ $<

obj/tk_actions.o: $(call DEPS,tk/actions.c)
	$(CC) -c -o $@ $<

obj/tk_screen_actions.o: $(call DEPS,tk/screen_actions.c)
	$(CC) -c -o $@ $<

obj/tk.o: $(call DEPS,tk/toolkit.c)
	$(CC) -c -o $@ $<

# BG
obj/bg.o: $(call DEPS,bg/background.c)
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



# VALGRIND: automatically create a rule for SDL2 init
valgrind: obj out out/valgrind.sup out/menu.bin
	valgrind --suppressions=out/valgrind.sup out/menu.bin

out/valgrind.sup: out/minimal_init.bin
	valgrind --gen-suppressions=all --quiet $< 2>&1 >/dev/null | grep "==" -v > $@
	sed -i -e 's/<insert_a_suppression_name_here>/SDL2 init/' $@

out/minimal_init.bin: minimal_init.c
	$(CC) `pkg-config --cflags --libs sdl2 SDL2_image SDL2_net` -o $@ $<


# Other
out/minimal.bin: minimal.c $(OBJS)
	mv obj/menu.o obj/menu.o_
	$(CC) `pkg-config --cflags --libs sdl2 SDL2_image SDL2_net` -o $@ obj/*.o $<
	mv obj/menu.o_ obj/menu.o

obj out:
	mkdir -p $@
clean:
	rm -r out obj || true
.PHONY: all clean
