CC:=LANG=C gcc -Wall -Werror
BIN:=../../bin/g2hr_installer.bin




run: $(BIN)
	$(BIN)

$(BIN): installer.c packed_files.h obj/xz.o obj_installer
	$(CC) -o $@ installer.c obj_installer/*.o obj/xz.o


obj_installer:
	./pack.sh


obj/xz.o:
	$(CC) -c -o $@ ../../src-3rdparty/xz-embedded/xz.c


clean:
	rm -r obj/* obj_installer $(BIN) || true



.PHONY: clean run
