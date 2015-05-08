COMPONENT := pads
LIBS:=sdl2

run: ../out/$(COMPONENT)_test.bin
	../out/$(COMPONENT)_test.bin


../out/$(COMPONENT)_test.bin: *.c *.h
	mkdir -p ../out/
	LANG=C gcc -std=c99 -Wall -Werror -g \
		`pkg-config --cflags --libs $(LIBS)` -o $@ *.c


valgrind: ../out/$(COMPONENT)_test.bin
	valgrind --leak-check=full ../out/$(COMPONENT)_test.bin \
	$(INPUT_FILE)
