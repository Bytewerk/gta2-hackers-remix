Func startup()
	If $CmdLine[0] < 1 And $WINE Then
		Exit Msgbox(16,"G2HR","This program gets opened internally on" _
			& @CRLF & "Linux. Start g2hr.sh instead!")
	Endif
	
	taskbar_hide(false)
	
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

	
	; Try to connect to the menu for 7 seconds
	TCPStartup()
	For $i = 0 To 140
		Sleep(50)
		$global_sock = TCPConnect("127.0.0.1", 20150)
		If $global_sock <> -1 And $global_sock <> 0 Then Exitloop
	Next

	If $global_sock == -1 Or $global_sock == 0 Then _
		Exit MsgBox(16,"G2HR", "Meta: Can't connect to the " _
			& "'menu' component!" & @CRLF _
			& "GTA2: Hacker's Remix is divided into multiple" & @CRLF _
			& " components, which need to connect to each other over" _
			& "	TCP" & @CRLF & "via localhost in order to do their unholy" _
			& " vodoo magic." & @CRLF & "Check your firewall settings!" _
			& @CRLF & @CRLF & "More info: http://git.io/g2hr-firewall")

	re("CONNECTED! CAN YOU GIVE ME THE CONFIG PATH?")
	
Endfunc
