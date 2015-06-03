INPUT_FILE := "../../../GTA2/data/mp1-6p.mmp"
# INPUT_FILE := "test.mmp"
# INPUT_FILE := "../../../data/g2hr.cfg"
BIN_FILE := "../../../bin/debug_ini.bin"

run: $(BIN_FILE)
	$(BIN_FILE) $(INPUT_FILE)


$(BIN_FILE): *.c *.h
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ \
		main_test.c ini.c ../io/io.c ../cstr/cstr.c


valgrind: $(BIN_FILE)
	valgrind --leak-check=full $(BIN_FILE) \
	$(INPUT_FILE)
