run: ../out/gxt_test.bin
	../out/gxt_test.bin ../../../GTA2/data/e.gxt "quit1"


../out/gxt_test.bin: *.c *.h
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ *.c ../../common/fs/fs.c


valgrind: ../out/gxt_test.bin
	valgrind --leak-check=full ../out/gxt_test.bin \
		../../../GTA2/data/e.gxt "quit1"
