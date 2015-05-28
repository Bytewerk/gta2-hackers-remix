SUBCOMPONENT :=cstr
BIN_FILE := "../../../bin/debug_$(SUBCOMPONENT).bin"

run: $(BIN_FILE)
	$(BIN_FILE)


$(BIN_FILE): *.c *.h
	LANG=C gcc -std=c99 -Wall -g -Werror -o $@ \
		$(SUBCOMPONENT).c main_test.c


valgrind: $(BIN_FILE)
	valgrind --leak-check=full $(BIN_FILE)
