#NoTrayIcon
#include "common.au3"
#include "cmds/singleplayer.au3"
#include <Array.au3>

If $CmdLine[0] <> 1 Then
	Exit Msgbox(16,"G2HR","Syntax: meta.exe menu_port")
Endif


; Connect to the menu
TCPStartup()
$global_sock = TCPConnect("127.0.0.1", $CmdLine[1])
If @ERROR Then Exit ConsoleWrite("[meta] connection refused" & @CRLF)
re("CONNECTED! CAN YOU GIVE ME THE CONFIG PATH?")


; Handle incoming commands. Format:
; 	COMMAND_NAME [PARAMETER1 [PARAMETER2] ... ]
$exit = 0
While Not $exit
	$data = BinaryToString(TCPRecv($global_sock,200,1))
	If @Error == -1 Then
		ConsoleWrite("[meta] unexpected disconnect from menu, " _
			& "shutting down..." & @CRLF)
		ExitLoop
	Endif
	
	If StringLen($data) > 0 Then
		ConsoleWrite("[menu] " & $data & "" & @CRLF)
		
		; The first message from the menu is always the config path
		If Not $global_config_path Then
			$global_config_path = $data
			If Not FileExists($global_config_path) Then
				Exit Msgbox(16, "G2HR", "ERROR: Config path doesn't" _
					& "exist: " & @CRLF & $global_config_path & @CRLF _
					& "This shouldn't have happened, please report a " _
					& "bug at:" & @CRLF &  "http://git.io/g2hr-bugs")
			Endif
			re("LOOKS VALID")
		Endif
		
		$cmd = StringSplit($data," ",2)
		Switch $cmd[0]
			Case "CLEANUP"
				$exit = 1
			Case "SINGLEPLAYER"
				cmd_singleplayer($cmd)
				Sleep(500)
				re("HIDE GET READY SCREEN")
			Case "SCREENLAYOUT"
				Local $geo[4] = [$cmd[2], $cmd[3], $cmd[4], $cmd[5]]
				$global_game_screen_layouts[$cmd[1]] = $geo
			Case "SPLITSCREEN"
				re("SPLITSCREEN: STUB!")
			Case "QUIT"
				ProcessClose($global_game_process_ids[$cmd[1]])
		EndSwitch
	EndIf
	
	
	; Check if the game is still open
	If $global_game_instances_open > 0 Then
		For $i = 0 To $GTA2_PLAYER_COUNT - 1
			If $global_game_process_ids[$i] _
				And Not ProcessExists($global_game_process_ids[$i]) Then
				
				$global_game_instances_open -= 1
				$global_game_process_ids[$i] = 0
				re("INSTANCE CLOSED " & $i)
				
			Endif
		Next
	Endif
	
	
	
	sleep(100) ; TCPRecv is non-blocking!
WEnd

; Clean up
TCPCloseSocket($global_sock)
TCPShutdown()
