COMPONENT := menu
SUBCOMPONENT := chk
LIBS:=SDL2_net

run: ../out/$(SUBCOMPONENT)_test.bin
	../out/$(SUBCOMPONENT)_test.bin


../out/$(SUBCOMPONENT)_test.bin: *.c *.h ../../common/cfg/cfg.*
	mkdir -p ../out/
	LANG=C gcc -std=c99 -Wall -Werror -g \
		`pkg-config --cflags --libs $(LIBS)` -o $@ *.c \
			../../common/ini/ini.c ../../common/io/io.c \
			../../common/cstr/cstr.c


valgrind: ../out/$(SUBCOMPONENT)_test.bin
	valgrind --leak-check=full --show-leak-kinds=all \
	 ../out/$(SUBCOMPONENT)_test.bin
