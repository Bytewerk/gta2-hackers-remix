BIN_FILE := "../../../bin/debug_io.bin"

run: $(BIN_FILE)
	$(BIN_FILE)


$(BIN_FILE): *.c *.h
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ *.c


valgrind: $(BIN_FILE)
	valgrind --leak-check=full $(BIN_FILE)
