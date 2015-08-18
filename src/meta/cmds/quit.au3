Func cmd_quit()
	For $i = 1 To $GTA2_PLAYER_COUNT
		ProcessClose($global_game_process_ids[$i-1])
	Next
	
	; Just to make sure
	ProcessClose("dplaysvr.exe")
	For $i = 1 To $GTA2_PLAYER_COUNT
		ProcessClose("G2HR_PLAYER"&$i&".exe")
	Next
Endfunc
