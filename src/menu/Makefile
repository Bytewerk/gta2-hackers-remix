# The menu code will run as native win32 application.
# However, SDL is crossplattform and most parts can be
# developed on Linux. So here's the Makefile.
#
# To develop the menu code on windows, see:
# http://git.io/g2hr-codeblocks-setup
#

CC=gcc -std=c99 -Wall -g
DEPS=$(shell $(CC) -MM $(1) | cut -d ':' -f 2 | tr -d '\n\\') obj out
OBJS=$(shell grep obj/ Makefile | grep 'call DEPS' | grep -v OBJS | cut -d':' -f 1 | tr '\n' ' ')

# Menu - TODO: put libs in extra variable?
out/menu.bin: $(OBJS)
	$(CC) -lSDL2 -lSDL2_image -lSDL2_net -o $@ obj/*.o

obj/menu.o: $(call DEPS,main.c)
	$(CC) -o $@ -c $<

# STY
obj/sty_circle.o: $(call DEPS,sty/circle.c)
	$(CC) -o $@ -c $<

obj/sty_sprite.o: $(call DEPS,sty/sprite.c)
	$(CC) -o $@ -c $<

obj/sty_text.o: $(call DEPS,sty/text.c)
	$(CC) -o $@ -c $<

obj/sty.o: $(call DEPS,sty/sty.c)
	$(CC) -o $@ -c $<

# SFX
obj/sfx.o: $(call DEPS,sfx/sfx.c)
	$(CC) -o $@ -c $<

# UI
obj/ui.o: $(call DEPS,interface/ui.c)
	$(CC) -o $@ -c $<

# TK
obj/tk_ctrl.o: $(call DEPS,toolkit/controls.c)
	$(CC) -o $@ -c $<

obj/tk_bg.o: $(call DEPS,toolkit/background.c)
	$(CC) -o $@ -c $<

obj/tk_screen.o: $(call DEPS,toolkit/screen.c)
	$(CC) -o $@ -c $<
	
obj/tk.o: $(call DEPS,toolkit/toolkit.c)
	$(CC) -o $@ -c $<



# Other
obj out:
	mkdir -p $@
clean:
	rm -r out obj || true











