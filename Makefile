all: bin/menu.exe bin/meta.exe bin/native.bin bin/proxy.dll


# The four main components
bin/proxy.dll:
	$(MAKE) -C src/injected/

bin/meta.exe:
	$(MAKE) -C src/meta/

bin/menu.exe:
	$(MAKE) -C src/menu/ Windows

bin/menu.bin:
	$(MAKE) -C src/menu/ Linux

bin/native.bin:
	$(MAKE) -C src/native/ Linux

bin/native.exe:
	$(MAKE) -C src/native/ Windows



# Additional targets for easy debugging
run: bin/menu.exe bin/meta.exe bin/native.bin GTA2/dmavideo.dll
	bin/native.bin
	wineboot -e -f

GTA2/dmavideo.dll: bin/proxy.dll
	cp bin/proxy.dll GTA2/dmavideo.dll


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

clean:
	rm bin/*.exe bin/*.bin || true
	$(MAKE) -s -C src/menu clean
	
.PHONY: clean all bin/menu.exe bin/meta.exe bin/native.bin bin/proxy.dll GTA2/dmavideo.dll regsniff
