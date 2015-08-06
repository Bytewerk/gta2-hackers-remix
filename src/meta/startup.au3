Func startup()
	If $CmdLine[0] < 1 And $WINE Then
		Exit Msgbox(16,"G2HR","This program gets opened internally on" _
			& @CRLF & "Linux. Start g2hr.sh instead!")
	Endif

	; Kill processes that are still open
	; todo: find a way to kill g2hr.exe - without killing itself?
	
	ProcessClose("g2hr_native.exe")
	ProcessClose("g2hr_menu.exe")
	For $i=1 To 6
		ProcessClose("G2HR_PLAYER" & $i & ".exe")
	Next

	
	; Start the other components
	Run("bin/g2hr_menu.exe")
	If Not $WINE Then Run("bin/g2hr_native.exe")

	
	TCPStartup()

	; Wait 5 seconds for the menu window to appear
	$HWND_SDL = WinWait($HWND_SDL_DESC, "", 5)
	If Not $HWND_SDL Then
		Exit MsgBox(16,"G2HR","Meta: Couldn't find the SDL menu window!" _
			& @CRLF & "Please report this here: http://git.io/g2hr-bugs")
	Endif
Endfunc
