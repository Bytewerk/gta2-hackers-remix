output = sdl_controller_code.$(shell uname -m)

$(output): *.c *.h
	c99 *.c -lSDL2 -lSDL2_net -o $(output)
	cp $(output) ../../splitscreen_mod/bin/
clean:
	rm $(output)
