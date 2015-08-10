# Build environment (see also env_win)
CC:=LANG=C gcc -std=c99 -Wall -g -Werror
PKGCONFIG:=pkg-config
LIBS:=sdl2 SDL2_image SDL2_net SDL2_mixer

# FIXME: get this with pkg-config?
MINGWFLAGS:=-static -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

# Automatic C dependency detection (just gcc, no make recursion etc)
DEPS=$(shell $(CC) -MM $(1) | cut -d ':' -f 2 | tr -d '\n\\')
OBJS=$(shell grep obj/ Makefile | grep 'call DEPS' | grep -v OBJS | cut -d':' -f 1 | tr '\n' ' ')


info:
	$(info ------------------------------------------------------------)
	$(info The menu can either be compiled for Windows or for Linux.)
	$(info In the release, the Windows version will be necessary, even)
	$(info when the mod runs on Linux - it needs to be inside the wine)
	$(info virtual desktop!)
	$(info )
	$(info Run one of these:)
	$(info ~  make Linux)
	$(info ~  make Windows)
	$(info )
	$(info ------------------------------------------------------------)


# High level stuff at the top
Linux: obj out out/menu.bin
run: Linux
	cd "../../" && src/menu/out/menu.bin
gdb: Linux
	cd "../../" && bin/native.bin --debug-menu-on-linux

# WINDOWS (don't mix Windows/Linux objects, run make clean!)
# Works on Arch Linux, install these packages from
# the AUR: mingw-w64-{sdl2,sdl2_image,libpng,zlib}
Windows: env_win obj ../../bin/g2hr_menu.exe
env_win:
	$(eval CC = $(shell echo "$(CC)" | sed 's/gcc/i686-w64-mingw32-gcc $(MINGWFLAGS)/'))
	$(eval PKGCONFIG = i686-w64-mingw32-pkg-config)
	$(eval LIBS = sdl2 SDL2_image SDL2_net SDL2_mixer libpng zlib)
res/menu.res: res menu.rc
	i686-w64-mingw32-windres menu.rc -O coff -o $@


# MENU
../../bin/g2hr_menu.exe: $(OBJS) res/menu.res
	$(CC) -o $@ obj/*.o res/menu.res \
		`$(PKGCONFIG) --cflags --libs $(LIBS)` -lws2_32 -liphlpapi
	
out/g2hr_menu.bin: $(OBJS)
	$(CC) -o $@ obj/*.o `$(PKGCONFIG) --cflags --libs $(LIBS)`

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
obj/screen_levels.o: $(call DEPS,ui/screens/levelselect.c)
	$(CC) -c -o $@ $<
obj/screen_ready.o: $(call DEPS,ui/screens/ready.c)
	$(CC) -c -o $@ $<
obj/screen_first_run.o: $(call DEPS,ui/screens/first_run.c)
	$(CC) -c -o $@ $<
obj/screen_options.o: $(call DEPS,ui/screens/options.c)
	$(CC) -c -o $@ $<
obj/screen_opt_audio.o: $(call DEPS,ui/screens/opt_audio.c)
	$(CC) -c -o $@ $<
obj/screen_opt_video.o: $(call DEPS,ui/screens/opt_video.c)
	$(CC) -c -o $@ $<

# UI
obj/ui.o: $(call DEPS,ui/ui.c)
	$(CC) -c -o $@ $<
obj/ui_callbacks.o: $(call DEPS,ui/ui_callbacks.c)
	$(CC) -c -o $@ $<
obj/ui_configs.o: $(call DEPS,ui/ui_configs.c)
	$(CC) -c -o $@ $<
obj/ui_send_cfg.o: $(call DEPS,ui/ui_send_game_config.c)
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
obj/sfx_sdt.o: $(call DEPS,sfx/sdt.c)
	$(CC) -c -o $@ $<

# GXT
obj/gxt.o: $(call DEPS,gxt/gxt.c)
	$(CC) -c -o $@ $<

# COMMON
obj/common_fs.o: $(call DEPS,../common/fs/fs.c)
	$(CC) -c -o $@ $<
obj/common_cstr.o: $(call DEPS,../common/cstr/cstr.c)
	$(CC) -c -o $@ $<
obj/common_ini.o: $(call DEPS,../common/ini/ini.c)
	$(CC) -c -o $@ $<

# CHK
obj/chk.o: $(call DEPS,chk/chk.c)
	$(CC) -c -o $@ $<

# MMP
obj/mmp.o: $(call DEPS,mmp/mmp.c)
	$(CC) -c -o $@ $<

# Network
obj/net.o: $(call DEPS,net/menu_net.c)
	$(CC) -c -o $@ $<

# SL
obj/sl.o: $(call DEPS,sl/sl.c)
	$(CC) -c -o $@ $<
obj/sl_calc.o: $(call DEPS,sl/calc.c)
	$(CC) -c -o $@ $<

# RPC
obj/rpc.o: $(call DEPS,rpc/rpc.c)
	$(CC) -c -o $@ $<

# VALGRIND: automatically create a rule for SDL2 init
valgrind: obj out out/valgrind.sup out/menu.bin
	cd "../../" && valgrind --suppressions=src/menu/out/valgrind.sup \
		src/menu/out/menu.bin

out/valgrind.sup: out/minimal_init.bin
	valgrind --gen-suppressions=all --quiet $< 2>&1 >/dev/null | grep "==" -v > $@
	sed -i -e 's/<insert_a_suppression_name_here>/SDL2 init/' $@

out/minimal_init.bin: minimal_init.c
	$(CC) `pkg-config --cflags --libs sdl2 SDL2_image SDL2_net` -o $@ $<

out/minimal.bin: minimal.c $(OBJS)
	mv obj/menu.o obj/menu.o_
	$(CC) `pkg-config --cflags --libs $(LIBS)` -o $@ obj/*.o $<
	mv obj/menu.o_ obj/menu.o

obj out res:
	mkdir -p $@
clean:
	rm ../../bin/g2hr_menu.exe || true
	rm -r out obj || true

.PHONY: clean win env_win run info Windows Linux
