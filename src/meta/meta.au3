Func OnAutoItExit()
   TCPShutdown()
EndFunc

; Connect to the menu
TCPStartup() ;
OnAutoItExitRegister("OnAutoItExit")
$sock = TCPConnect("127.0.0.1", 19990)
If @error Then Exit ConsoleWrite("Connection refused" & @CRLF)
ConsoleWrite("[meta] connected to the menu" & @CRLF)

$exit = 0
While Not $exit
   $data = BinaryToString(TCPRecv($sock,10,1))
   If StringLen($data) > 0 Then ConsoleWrite("[menu] "&$data&""&@CRLF)

   Switch $data
	  Case "CLEANUP"
		 $exit = 1
	  ; ...
   EndSwitch

   sleep(100) ; TCPRecv is non-blocking!
WEnd

TCPCloseSocket($sock)