PATCH:=../../data/vike_patch.bsdiff
PATCH_XZ:=../../data/vike_patch.xz


# The patch file is bigger than gta2_vike.exe, because it contains
# LOTS of zeros. We compress it for github, so it becomes obvious that
# this is really only a patch and not a full exe file!


$(PATCH): $(PATCH_XZ)
	cp $(PATCH_XZ) $(PATCH).xz
	unxz $(PATCH).xz

$(PATCH_XZ): create_patch.bin gta2.exe gta2_vike.exe
	./create_patch.bin gta2.exe gta2_vike.exe gta2_patched.exe $(PATCH)
	md5sum gta2_vike.exe gta2_patched.exe
	xz $(PATCH)
	mv $(PATCH).xz $(PATCH_XZ)


create_patch.bin: create_patch.c
	gcc -o create_patch.bin -Wall create_patch.c \
		"../../src-3rdparty/bsdiff/bsdiff.c" \
		"../../src-3rdparty/bsdiff/bspatch.c" \
		"../../src/common/fs/fs.c"

clean:
	rm create_patch.bin $(PATCH) $(PATCH_XZ) gta2_patched.exe || true
	



.PHONY: clean
