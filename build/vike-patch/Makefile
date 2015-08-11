run: create_patch.bin
	./create_patch.bin gta2.exe gta2_vike.exe


create_patch.bin: create_patch.c
	gcc -o create_patch.bin -Wall create_patch.c \
		"../../src-3rdparty/bytestring-delta/bdelta.c" \
		"../../src/common/fs/fs.c"


.PHONY: run
