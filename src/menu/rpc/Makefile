COMPONENT := rpc


run: ../out/$(COMPONENT)_test.bin
	cd ../../../ && src/menu/out/$(COMPONENT)_test.bin $(INPUT_FILE)


../out/$(COMPONENT)_test.bin: *.c *.h
	mkdir -p ../out/
	LANG=C gcc -std=c99 -Wall -Werror -g  -o $@ *.c `pkg-config --cflags --libs sdl2`


valgrind: ../out/$(COMPONENT)_test.bin
	cd ../../../ && valgrind --leak-check=full src/menu/out/$(COMPONENT)_test.bin
