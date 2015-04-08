# The menu code will run as native win32 application.
# However, SDL is crossplattform and some parts can at least be
# developed on Linux. So here's the Makefile.
#
# FIXME: The Makefile isn't written very well, but make doesn't
# offer automatic dependency resolution and creating objects
# only works for one c file at a time (one folder would be handy).
# So we'll keep it like that for now. Compilation speed isn't really
# slow anyway.
#
# To develop the menu code on windows, see:
# http://git.io/g2hr-codeblocks-setup
#


out/menu.bin: main.c */*.c */*.h
	mkdir -p out
	gcc -Wall -g -std=c99 main.c */*.c -lSDL2 -lSDL2_image -lSDL2_net -o $@


clean:
	rm -r out || true
