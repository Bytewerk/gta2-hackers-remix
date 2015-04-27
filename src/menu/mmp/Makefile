# INPUT_FILE := "../../../GTA2/data/mp1-6p.mmp"
# INPUT_FILE := "test.mmp"
INPUT_FILE := "../../../GTA2/data"


run: ../out/mmp_test.bin
	../out/mmp_test.bin $(INPUT_FILE)


../out/mmp_test.bin: *.c *.h
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ *.c


valgrind: ../out/mmp_test.bin
	valgrind --leak-check=full ../out/mmp_test.bin \
	$(INPUT_FILE)
