CC:=LANG=C i686-w64-mingw32-gcc -Wall -g -Werror -shared -std=c99



proxy.dll: *.c *.h
	$(CC) -o $@ proxy.c

PHONY: pexports
