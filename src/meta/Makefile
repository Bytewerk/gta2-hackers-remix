# Compile as 'gui', so no terminal window gets opened when starting
# the application. See also: "Compiling Scripts" in the AutoIt3 help
# for commandline reference and the au3check reference:
#   https://www.autoitscript.com/autoit3/docs/intro/au3check.htm


AUTOIT_DIR := c:\Program Files\AutoIt3
AUT2EXE := "$(AUTOIT_DIR)\Aut2Exe\Aut2exe.exe"
SCITE := "$(AUTOIT_DIR)\SciTE\SciTE.exe"
AU3INFO := "$(AUTOIT_DIR)\Au3Info.exe"
AU3CHECK := "$(AUTOIT_DIR)\Au3Check.exe"
WINEPREFIX := $(shell realpath ../../build/wineprefix)
WINE := WINEPREFIX=$(WINEPREFIX) WINEDEBUG=-all wine


../../bin/g2hr.exe: *.au3 */*.au3
	$(WINE) $(AU3CHECK) -q -d -w 1 -w 2 -w 3 -w 4 -w 5 -w 6 -w 7 \
		meta.au3
	$(WINE) $(AUT2EXE) \
		/in meta.au3 /out $@ /icon ../../data/g2hr.ico /x86 /gui

wineprefix: $(WINEPREFIX)

$(WINEPREFIX):
	cd ../../build/ && ./setup_wineprefix.sh

clean:
	rm ../../bin/g2hr.exe || true


scite:
	$(WINE) $(SCITE) &

au3info:
	$(WINE) $(AU3INFO) &

.PHONY: clean wineprefix scite au3info
