# Compile as 'gui', so no terminal window gets opened when starting
# the application.
# See also: "Compiling Scripts" in the AutoIt3 help for command-
# line reference.

AUT2EXE := "c:\Program Files\AutoIt3\Aut2Exe\Aut2exe.exe"
WINEPREFIX := $(shell realpath ../../build/wineprefix)
WINE := WINEPREFIX=$(WINEPREFIX) WINEDEBUG=-all wine

../../bin/meta.exe: *.au3 */*.au3
	$(WINE) $(AUT2EXE) \
		/in meta.au3 /out $@ /x86 /gui

wineprefix: $(WINEPREFIX)

$(WINEPREFIX):
	cd ../../build/ && ./setup_wineprefix.sh

clean:
	rm ../../bin/meta.exe || true
	
.PHONY: clean wineprefix
