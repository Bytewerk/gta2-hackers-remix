INPUT_FILE := "../../../data/screen_layouts.cfg"
COMPONENT := screen_layout


run: ../out/$(COMPONENT)_test.bin
	../out/$(COMPONENT)_test.bin $(INPUT_FILE)


../out/$(COMPONENT)_test.bin: *.c *.h
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ *.c


valgrind: ../out/$(COMPONENT)_test.bin
	valgrind --leak-check=full ../out/$(COMPONENT)_test.bin \
	$(INPUT_FILE)
