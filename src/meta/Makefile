# Compile as 'gui', so no terminal window gets opened when starting
# the application.
# See also: "Compiling Scripts" in the AutoIt3 help for command-
# line reference.

AUTOIT_DIR := c:\Program Files\AutoIt3
AUT2EXE := "$(AUTOIT_DIR)\Aut2Exe\Aut2exe.exe"
SCITE := "$(AUTOIT_DIR)\SciTE\SciTE.exe"
AU3INFO := "$(AUTOIT_DIR)\Au3Info.exe"
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


scite:
	$(WINE) $(SCITE) &

au3info:
	$(WINE) $(AU3INFO) &

.PHONY: clean wineprefix scite au3info
