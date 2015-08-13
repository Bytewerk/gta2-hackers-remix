BIN=../../bin/g2hr_installer.bin

$(BIN): installer.c packed_files.h obj/xz.o
	gcc -Wall -Werr -o $@ installer.c obj_installer/*.o obj/xz.o


obj/xz.o:
	gcc -c -o $@ ../../src-3rdparty/xz-embedded/xz.c
