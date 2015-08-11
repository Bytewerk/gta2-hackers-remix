#NoTrayIcon
#include "common.au3"
#include "handle_msg.au3"
#include <Array.au3>
#include "startup.au3"



startup()


Global $exit = 0
While Not $exit
	
	; Get new messages (they must have an "(END)"-suffix!)
	Global $data = ""
	Do
		Global $new = BinaryToString(TCPRecv($global_sock, 200, 1))
		If @Error == -1 Then
			ConsoleWrite("[meta] unexpected disconnect from menu," _
				& "shutting down!" & @CRLF)
			Exitloop 2
		Elseif StringLen($new) == 0 Then
			If StringLen($data) > 0 Then _
				ConsoleWrite("[meta]: unexpected end of message from" _
					& " menu: '" & $data & "'" & @CRLF)
			Exitloop
		Endif
		
		$data &= $new
		Sleep(10) ; ms
		
	Until StringRight($data, 5) == "(END)"
	
	
	; Handle all received messages, if any
	If StringLen($data) > 0 Then
		Global $messages = StringSplit($data,"(END)",1)
		For $i=1 To $messages[0]
			If handle_msg($messages[$i]) == False Then _
				Exitloop 2
		Next
	Endif
	
	
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

; Clean up (sends a message, in case the clean up is unexpected by menu)
re("CLEANING UP...")
ProcessClose("dplaysvr.exe")
For $i = 1 To $GTA2_PLAYER_COUNT
	ProcessClose("G2HR_PLAYER"&$i&".exe")
Next
TCPCloseSocket($global_sock)
TCPShutdown()
