; Global variables
Global $global_sock
Global $global_game_instances_open = 0
Global $global_game_process_ids[6]


; Initialize Arrayss
For $i=0 To 5
	$global_game_process_ids[$i] = NULL
Next

; Zero-terminated string reply to the server
Func re($message)
	TCPSend($global_sock, $message & Chr(0))
Endfunc
