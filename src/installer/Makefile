CC:=LANG=C i686-w64-mingw32-gcc -static -Wall -Werror
BIN:=../../bin/g2hr_installer.exe




run: $(BIN)
	wine $(BIN)

$(BIN): c/installer.c c/packed_files.h obj/xz.o obj_installer
	$(CC) -o $@ c/installer.c obj_installer/*.o obj/xz.o
	du -h $(BIN)


obj_installer:
	./pack.sh


obj/xz.o:
	$(CC) -c -o $@ ../../src-3rdparty/xz-embedded/xz.c


clean:
	rm -r obj/* obj_installer $(BIN) || true



.PHONY: clean run
