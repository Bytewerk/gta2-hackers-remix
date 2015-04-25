all: bin/menu.exe bin/meta.exe


bin/menu.exe:
	$(MAKE) -C src/menu/ Windows

bin/meta.exe:
	$(MAKE) -C src/meta

clean:
	rm bin/*.exe bin/*.bin || true
	$(MAKE) -C src/menu clean


.PHONY: clean all
