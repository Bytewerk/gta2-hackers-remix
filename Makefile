all: bin/menu.exe bin/meta.exe bin/native.bin


run: bin/menu.exe bin/meta.exe bin/native.bin
	bin/native.bin
	wineboot -e -f

# Run native.bin with gdb and print a stack trace, when it
# fails. The very short "--debug-menu-with-gdb-on-linux"
# parameter makes the native component run the linux-binary
# (usually we run the windows binary!) with the same gdb
# options.
gdb: bin/menu.bin bin/meta.exe bin/native.bin
	gdb -batch -ex run -ex bt --args bin/native.bin \
		--debug-menu-with-gdb-on-linux

# Sniff registry changes performed by the GTA2 manager
# You should install the 'power patch' registry key, which will
# enable the debug mode ( http://gtamp.com/GTA2/powerpatch.zip )
regsniff: GTA2/gta2manager.exe
	cd GTA2 && WINEDEBUG=+reg wine gta2manager.exe


bin/menu.exe:
	$(MAKE) -C src/menu/ Windows

bin/menu.bin:
	$(MAKE) -C src/menu/ Linux

bin/meta.exe:
	$(MAKE) -C src/meta

bin/native.bin:
	$(MAKE) -C src/native/ Linux


clean:
	rm bin/*.exe bin/*.bin || true
	$(MAKE) -s -C src/menu clean
	
.PHONY: clean all bin/menu.exe bin/meta.exe bin/native.bin regsniff
