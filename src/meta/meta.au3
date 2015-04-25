#include "cmds/singleplayer.au3"

Func OnAutoItExit()
   TCPShutdown()
EndFunc

; Connect to the menu
TCPStartup()
OnAutoItExitRegister("OnAutoItExit")
$sock = TCPConnect("127.0.0.1", 19990)
If @ERROR Then Exit ConsoleWrite("[meta] connection refused" & @CRLF)
ConsoleWrite("[meta] connected to the menu" & @CRLF)


; Handle incoming commands. Format:
; 	COMMAND_NAME [PARAMETER1 [PARAMETER2] ... ]
$exit = 0
While Not $exit
	$data = BinaryToString(TCPRecv($sock,10,1))
	If StringLen($data) > 0 Then
		ConsoleWrite("[menu] " & $data & "" & @CRLF)
		
		$cmd = StringSplit($data," ")
		Switch $cmd[0]
			Case "CLEANUP"
				$exit = 1
			Case "SINGLEPLAYER"
				cmd_singleplayer($cmd)
				
				
		EndSwitch
		
	EndIf

	sleep(100) ; TCPRecv is non-blocking!
WEnd

; Clean up
TCPCloseSocket($sock)
