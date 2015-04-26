all: bin/menu.exe bin/meta.exe


run: bin/menu.exe bin/meta.exe
	WINEDEBUG=-all wine explorer /desktop=G2HR,640x480 bin/menu.exe
	wineboot -e -f

bin/menu.exe:
	$(MAKE) -C src/menu/ Windows

bin/meta.exe:
	$(MAKE) -C src/meta

clean:
	rm bin/*.exe bin/*.bin || true
	$(MAKE) -s -C src/menu clean
	
.PHONY: clean all bin/menu.exe bin/meta.exe
