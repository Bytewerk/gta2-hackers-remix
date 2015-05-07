#NoTrayIcon
#include "common.au3"
#include "cmds/singleplayer.au3"
#include <Array.au3>

Func OnAutoItExit()
   TCPShutdown()
EndFunc

If $CmdLine[0] < 1 Then _
	Exit Msgbox(16,"G2HR","Syntax: meta.exe menu_server_port")

; Connect to the menu
TCPStartup()
OnAutoItExitRegister("OnAutoItExit")
$global_sock = TCPConnect("127.0.0.1", $CmdLine[1])
If @ERROR Then Exit ConsoleWrite("[meta] connection refused" & @CRLF)
re("CONNECTED")

; Handle incoming commands. Format:
; 	COMMAND_NAME [PARAMETER1 [PARAMETER2] ... ]
$exit = 0
While Not $exit
	$data = BinaryToString(TCPRecv($global_sock,100,1))
	If StringLen($data) > 0 Then
		ConsoleWrite("[menu] " & $data & "" & @CRLF)
		
		$cmd = StringSplit($data," ",2)
		Switch $cmd[0]
			Case "CLEANUP"
				$exit = 1
			Case "SINGLEPLAYER"
				cmd_singleplayer($cmd)
				Sleep(500)
				re("HIDE GET READY SCREEN")
		EndSwitch
	EndIf
	
	
	; Check if the game is still open
	If $global_game_instances_open > 0 Then
		For $i = 0 To 5
			If $global_game_process_ids[$i] _
				And Not ProcessExists($global_game_process_ids[$i]) Then
				
				$global_game_instances_open -= 1
				$global_game_process_ids[$i] = NULL
				re("INSTANCE CLOSED " & $i)
				
			Endif
		Next
	Endif
	
	
	
	sleep(100) ; TCPRecv is non-blocking!
WEnd

; Clean up
TCPCloseSocket($global_sock)
