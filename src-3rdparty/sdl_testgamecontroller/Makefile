../../bin/sdl_testgamecontroller.bin: sdl_testgamecontroller.c
	LANG=C gcc `pkg-config --cflags --libs sdl2` -Wall -Werror -o $@ $<
	cp *.bmp ../../bin/


.PHONY: clean

clean:
	rm ../../bin/sdl_testgamecontroller.bin ../../bin/*.bmp || true
