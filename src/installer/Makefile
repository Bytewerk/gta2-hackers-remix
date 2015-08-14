CC:=LANG=C i686-w64-mingw32-gcc -static -Wall -Werror
BIN:=../../bin/g2hr_installer.exe


hash_test: $(BIN)
	sha512sum $(BIN)
	wine $(BIN) verify $(BIN)

run: $(BIN)
	wine $(BIN)

$(BIN): c/installer.h c/installer.c c/packed_files.h obj/xz.o obj/cstr.o obj_installer obj/sha512.o
	$(CC) -o $@ c/installer.c obj_installer/*.o obj/xz.o obj/cstr.o \
		obj/sha512.o
	du -h $(BIN)


obj_installer c/packed_files.h:
	./pack.sh


obj/xz.o:
	mkdir -p obj
	$(CC) -c -o $@ ../../src-3rdparty/xz-embedded/xz.c
	
obj/sha512.o:
	mkdir -p obj
	$(CC) -c -o $@ ../../src-3rdparty/mbedtls-sha512/sha512.c

obj/cstr.o: ../common/cstr/*.c ../common/cstr/*.h
	mkdir -p obj
	$(CC) -c -o $@ ../common/cstr/cstr.c


clean:
	rm -r obj/* obj_installer $(BIN) || true

force_repack:
	rm c/packed_files.h || true


.PHONY: clean run force_repack
