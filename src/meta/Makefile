# Compile as 'gui', so no terminal window gets opened when starting
# the application.
# See also: "Compiling Scripts" in the AutoIt3 help for command-
# line reference.

# It's probably smart to use an extra wine prefix for AutoIt3!
AUT2EXE := "c:\Program Files (x86)\AutoIt3\Aut2Exe\Aut2exe.exe"

../../bin/meta.exe: meta.au3 */*.au3
	WINEDEBUG=-all wine $(AUT2EXE) \
		/in meta.au3 /out $@ /x86 /gui

clean:
	rm ../../bin/meta.exe || true
	
.PHONY: clean
