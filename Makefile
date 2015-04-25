all: bin/menu.exe bin/meta.exe


run: bin/menu.exe bin/meta.exe
	WINEDEBUG=-all wine bin/menu.exe
	sleep 1 && wineboot -e -f

bin/menu.exe:
	$(MAKE) -C src/menu/ Windows

bin/meta.exe:
	$(MAKE) -C src/meta

clean:
	rm bin/*.exe bin/*.bin || true
	$(MAKE) -s -C src/menu clean
	
.PHONY: clean all bin/menu.exe bin/meta.exe
