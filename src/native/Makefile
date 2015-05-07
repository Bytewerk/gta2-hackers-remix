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
Linux: obj ../../bin ../../bin/native.bin
valgrind: Linux
	cd "../../" && valgrind bin/native.bin
run: Linux
	cd "../../" && bin/native.bin
gdb: Linux
	cd "../../" && gdb bin/native.bin

# WINDOWS (don't mix Windows/Linux objects, run make clean!)
# Works on Arch Linux, install these packages from
# the AUR: mingw-w64-{sdl2,sdl2_image,libpng,zlib}
Windows: env_win obj ../../bin/native.exe
env_win:
	$(eval CC = $(shell echo "$(CC)" | sed 's/gcc/i686-w64-mingw32-gcc $(MINGWFLAGS)/'))
	$(eval PKGCONFIG = i686-w64-mingw32-pkg-config)
	$(eval LIBS = sdl2 SDL2_image SDL2_net libpng zlib)


# NATIVE
../../bin/native.exe: $(OBJS)
	$(CC) -o $@ obj/*.o `$(PKGCONFIG) --cflags --libs $(LIBS)` -lws2_32 -liphlpapi
	
../../bin/native.bin: $(OBJS)
	$(CC) -o $@ obj/*.o `$(PKGCONFIG) --cflags --libs $(LIBS)`

obj/native.o: $(call DEPS,native.c)
	$(CC) -c -o $@ $<

# NET
obj/net.o: $(call DEPS,net/net.c)
	$(CC) -c -o $@ $<




# OTHER

obj ../../bin:
	mkdir -p $@
	
clean:
	rm ../../bin/native.exe || true
	rm ../../bin/native.bin || true
	rm -r obj || true

.PHONY: clean win env_win run info Windows Linux
