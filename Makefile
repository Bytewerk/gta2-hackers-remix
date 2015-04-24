bin/menu.exe:
	$(MAKE) -C src/menu/ win


clean:
	rm bin/*.exe bin/*.bin || true
	$(MAKE) -C src/menu clean


.PHONY: clean
