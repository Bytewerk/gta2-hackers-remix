INPUT_FILE := "../../../data/screen_layouts.cfg"
COMPONENT := screen_layout


run: ../out/$(COMPONENT)_test.bin
	../out/$(COMPONENT)_test.bin $(INPUT_FILE)


../out/$(COMPONENT)_test.bin: *.c *.h
	LANG=C gcc -std=c99 -Wall -Werror -g -o $@ *.c ../../common/io/io.c


valgrind: ../out/$(COMPONENT)_test.bin
	valgrind --leak-check=full --show-leak-kinds=all \
		../out/$(COMPONENT)_test.bin $(INPUT_FILE)
