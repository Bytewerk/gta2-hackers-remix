run: ../out/sfx_test.bin
	cd ../../../ && src/menu/out/sfx_test.bin


../out/sfx_test.bin: *.c *.h
	mkdir -p ../out
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ *.c \
		../../common/fs/fs.c ../../common/cstr/cstr.c \
		`pkg-config --cflags --libs sdl2 SDL2_mixer`


valgrind: ../out/sfx_test.bin
	cd ../../../ && valgrind --leak-check=full \
		src/menu/out/sfx_test.bin



clean:
	rm ../out/sfx_test.bin


.PHONY: clean
