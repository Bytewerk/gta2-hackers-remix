# The menu code will run as native win32 application.
# However, SDL is crossplattform and some parts can at least be
# developed on Linux. So here's the Makefile.
#
# To develop the menu code on windows, see:
# http://git.io/g2hr-codeblocks-setup
#

output = menu.$(shell uname -m)

$(output): *.c */*.c */*.h
	c99 *.c */*.c -lSDL2 -lSDL2_image -lSDL2_net -o $(output)

clean:
	rm $(output)
