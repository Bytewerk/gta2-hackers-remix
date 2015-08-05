# Build environment (see also env_win)
CC:=LANG=C gcc -std=c99 -Wall -g -Werror
PKGCONFIG:=pkg-config
LIBS:=sdl2 SDL2_net

# FIXME: get this with pkg-config?
MINGWFLAGS:=-static -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

# Automatic C dependency detection (just gcc, no make recursion etc)
DEPS=$(shell $(CC) -MM $(1) | cut -d ':' -f 2 | tr -d '\n\\')
OBJS=$(shell grep obj/ Makefile | grep 'call DEPS' | grep -v OBJS | cut -d':' -f 1 | tr '\n' ' ')


info:
	$(info ------------------------------------------------------------)
	$(info The native component can either be compiled for Windows or)
	$(info for Linux.)
	$(info )
	$(info Run one of these:)
	$(info ~  make Linux)
	$(info ~  make Windows)
	$(info )
	$(info ------------------------------------------------------------)


# High level stuff at the top
Linux: obj ../../bin ../../bin/g2hr.bin
valgrind: Linux
	cd "../../" && valgrind bin/g2hr.bin
run: Linux
	cd "../../" && bin/g2hr.bin
gdb: Linux
	cd "../../" && gdb bin/g2hr.bin

# WINDOWS (don't mix Windows/Linux objects, run make clean!)
# Works on Arch Linux, install these packages from
# the AUR: mingw-w64-{sdl2,sdl2_image,libpng,zlib}
Windows: env_win obj ../../bin/g2hr.exe
env_win:
	$(eval CC = $(shell echo "$(CC)" | sed 's/gcc/i686-w64-mingw32-gcc $(MINGWFLAGS)/'))
	$(eval PKGCONFIG = i686-w64-mingw32-pkg-config)
	$(eval LIBS = sdl2 SDL2_image SDL2_net libpng zlib)

res/native.res: res native.rc
	i686-w64-mingw32-windres native.rc -O coff -o $@


# NATIVE
../../bin/g2hr.exe: $(OBJS) res/native.res
	$(CC) -o $@ res/native.res obj/*.o \
		`$(PKGCONFIG) --cflags --libs $(LIBS)` -lws2_32 -liphlpapi
	
../../bin/g2hr.bin: $(OBJS)
	$(CC) -o $@ obj/*.o `$(PKGCONFIG) --cflags --libs $(LIBS)`

obj/native.o: $(call DEPS,native.c)
	$(CC) -c -o $@ $<

# Network
obj/net.o: $(call DEPS,net/native_net.c)
	$(CC) -c -o $@ $<
obj/net_frame.o: $(call DEPS,net/native_net_frame.c)
	$(CC) -c -o $@ $<
obj/net_msg.o: $(call DEPS,net/native_net_injected_msg.c)
	$(CC) -c -o $@ $<

# Game Controllers
obj/pads.o: $(call DEPS,pads/pads.c)
	$(CC) -c -o $@ $<

# inmenu
obj/inmenu.o: $(call DEPS,inmenu/inmenu.c)
	$(CC) -c -o $@ $<

# ingame
obj/ingame.o: $(call DEPS,ingame/ingame.c)
	$(CC) -c -o $@ $<

# Controller Mappings
obj/cmap.o:$(call DEPS,cmap/cmap.c)
	$(CC) -c -o $@ $<

# COMMON
obj/common_ini.o: $(call DEPS,../common/ini/ini.c)
	$(CC) -c -o $@ $<
obj/common_cstr.o: $(call DEPS,../common/cstr/cstr.c)
	$(CC) -c -o $@ $<
obj/common_fs.o: $(call DEPS,../common/fs/fs.c)
	$(CC) -c -o $@ $<

# OTHER

obj ../../bin res:
	mkdir -p $@
	
clean:
	rm ../../bin/g2hr.exe || true
	rm ../../bin/g2hr.bin || true
	rm -r obj out || true

.PHONY: clean win env_win run info Windows Linux
