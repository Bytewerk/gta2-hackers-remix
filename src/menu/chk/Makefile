COMPONENT := menu
SUBCOMPONENT := chk
LIBS:=SDL2_net

run: ../out/$(SUBCOMPONENT)_test.bin
	cd ../../../ &&  src/$(COMPONENT)/out/$(SUBCOMPONENT)_test.bin


../out/$(SUBCOMPONENT)_test.bin: *.c *.h ../../common/cfg/cfg.*
	mkdir -p ../out/
	LANG=C gcc -std=c99 -Wall -Werror -g \
		`pkg-config --cflags --libs $(LIBS)` -o $@ *.c \
			../../common/cfg/cfg.c ../../common/io/io.c


valgrind: ../out/$(SUBCOMPONENT)_test.bin
	cd ../../../ && valgrind --leak-check=full --show-leak-kinds=all \
	 src/$(COMPONENT)/out/$(SUBCOMPONENT)_test.bin
