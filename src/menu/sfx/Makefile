MINGWFLAGS:=-static -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

run_Windows: ../out/sfx_test.exe
	cd ../../../ && wine src/menu/out/sfx_test.exe


run: ../out/sfx_test.bin
	cd ../../../ && src/menu/out/sfx_test.bin


../out/sfx_test.bin: *.c *.h
	mkdir -p ../out
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ *.c \
		../../common/fs/fs.c ../../common/cstr/cstr.c \
		`pkg-config --cflags --libs sdl2 SDL2_mixer`
		

../out/sfx_test.exe: *.c *.h
	mkdir -p ../out
	LANG=C i686-w64-mingw32-gcc $(MINGWFLAGS) -std=c99 -Wall -g \
		-Werror -o $@ *.c ../../common/fs/fs.c \
		../../common/cstr/cstr.c \
		`i686-w64-mingw32-pkg-config --cflags --libs sdl2 SDL2_mixer`


valgrind: ../out/sfx_test.bin
	cd ../../../ && valgrind --leak-check=full \
		src/menu/out/sfx_test.bin



clean:
	rm ../out/sfx_test.*


.PHONY: clean
