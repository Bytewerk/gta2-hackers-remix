# It's probably smart to use an extra wine prefix for AutoIt3!
AUT2EXE := "c:\Program Files (x86)\AutoIt3\Aut2Exe\Aut2exe.exe"


# See also: "Compiling Scripts" in the AutoIt3 help for command-
# line reference.

# Compile as 'gui', so no terminal window gets opened when starting
# the application.
meta.exe: *.au3
	WINEDEBUG=-all wine $(AUT2EXE) \
		/in meta.au3 /out meta.exe /x86 /gui


clean:
	rm meta.exe
	
	
.PHONY: clean
