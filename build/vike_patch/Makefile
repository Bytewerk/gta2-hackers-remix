PATCH:=../../data/vike_patch/vike_patch.plainbsdiff
PATCH_XZ:=../../data/vike_patch/vike_patch.xz
HEADER:=../../src/common/headers/vike_patch.h


# The patch file is bigger than gta2_vike.exe, because it contains
# LOTS of zeros. We compress it for github, so it becomes obvious that
# this is really only a patch and not a full exe file!


all: $(PATCH)

$(PATCH):
	cp $(PATCH_XZ) $(PATCH).xz
	unxz $(PATCH).xz

$(PATCH_XZ) $(HEADER): create_patch.bin gta2.exe gta2_vike.exe
	./create_patch.bin gta2.exe gta2_vike.exe $(PATCH) \
		gta2_patched.exe ../../src/common/headers/vike_patch.h
	md5sum -b gta2_vike.exe | sed s/_vike/_patched/ | md5sum -c
	xz $(PATCH)
	mv $(PATCH).xz $(PATCH_XZ)


create_patch.bin: create_patch.c
	gcc -o create_patch.bin -Wall create_patch.c \
		"../../src-3rdparty/bsdiff/bsdiff.c" \
		"../../src-3rdparty/bsdiff/bspatch.c" \
		"../../src/common/fs/fs.c"

clean:
	rm create_patch.bin $(PATCH) $(PATCH_XZ) gta2_patched.exe $(HEADER) || true
	



.PHONY: clean all
