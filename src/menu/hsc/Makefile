run: ../out/hsc_test.bin
	../out/hsc_test.bin ../player/hiscores.hsc


../out/hsc_test.bin: *.c *.h
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ ../../common/io/io.c *.c

