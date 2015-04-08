# The menu code will run as native win32 application.
# However, SDL is crossplattform and some parts can at least be
# developed on Linux. So here's the Makefile.
#
# To develop the menu code on windows, see:
# http://git.io/g2hr-codeblocks-setup
#

output = menu.$(shell uname -m)


# FIXME: add -Wall and then fix all warnings!

$(output): main.c */*.c */*.h
	gcc -g -std=c99 main.c */*.c -lSDL2 -lSDL2_image -o $(output) -Wall #-lSDL2_net

sprite_test.bin: sprite_test.c sty/*.c sty/*.h
	gcc -g -std=c99 sprite_test.c sty/*.c -lSDL2 -o sprite_test.bin -Wfatal-errors


clean:
	rm $(output)
