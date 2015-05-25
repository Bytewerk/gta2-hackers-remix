; We have this in an extra function, because we also need to do this
; for the normal registry path (Software\DMA Design Ltd\GTA2\). Although
; it is replaced in the exe files where it was found, somehow the net-
; work settings still get loaded from there. So the easiest way is, just
; putting it there, too.
;
Func prepare_common_registry_network_settings($root)
	; Working network settings, when selecting TCP/IP and 127.0.0.1.
	; Simply dumped with: ConsoleWrite(RegRead(...))
	RegWrite($root&"\Network", "protocol_selected", "REG_DWORD", 0)
	RegWrite($root&"\Network", "TCPIPAddrStringd", "REG_BINARY", _
	   Binary("0x3100320037002E0030002E0030002E0031000000"))
	RegWrite($root&"\Network", "TCPIPAddrStrings", "REG_DWORD", 20)

	RegWrite($root&"\Network", "UseConnectiond", "REG_BINARY", _
		Binary("0x60F518132C91D0119DAA00A0C90A43CB0400000064000000C016D907AFE0CF119C4E00A0C905425E10000000E05EE9367785CF11960C0080C7534E82A03232E6BF9DD0119CC100A0C905425E140000003100320037002E0030002E0030002E0031000000"))
	RegWrite($root&"\Network", "UseConnections", "REG_DWORD", 100)

	RegWrite($root&"\Network", "UseProtocold", "REG_BINARY", _
		Binary("0xE05EE9367785CF11960C0080C7534E82"))
	RegWrite($root&"\Network", "UseProtocols", "REG_DWORD", 16)
Endfunc

Func prepare_common_registry_settings($i) ;$i: 1...6
	Local $geo = $global_game_screen_layouts[$i -1]
	Local $root = "HKEY_CURRENT_USER\Software\GTA2HackersRemix\P" & $i
	
	prepare_common_registry_network_settings($root)
	
	RegWrite($root&"\Screen","full_width","REG_DWORD", $geo[2])
	RegWrite($root&"\Screen","full_height","REG_DWORD", $geo[3])
	RegWrite($root&"\Screen","window_width","REG_DWORD", $geo[2])
	RegWrite($root&"\Screen","window_height","REG_DWORD", $geo[3])
	RegWrite($root&"\Screen", "start_mode", "REG_DWORD", 0)
	RegWrite($root&"\Network", "PlayerName", "REG_SZ", "Player "&$i)
	RegWrite($root&"\Sound",  "CDVol", "REG_DWORD", 0x00)
	RegWrite($root&"\Sound",  "SFXVol", "REG_DWORD", 0x7f)
	
	RegWrite($root&"\Control",  "0", "REG_DWORD", 0xc8)
	RegWrite($root&"\Control",  "1", "REG_DWORD", 0xd0)
	RegWrite($root&"\Control",  "2", "REG_DWORD", 0xcb)
	RegWrite($root&"\Control",  "3", "REG_DWORD", 0xcd)
	RegWrite($root&"\Control",  "4", "REG_DWORD", 0x1d)
	RegWrite($root&"\Control",  "5", "REG_DWORD", 0x1c)
	RegWrite($root&"\Control",  "6", "REG_DWORD", 0x39)
	RegWrite($root&"\Control",  "7", "REG_DWORD", 0x2c)
	RegWrite($root&"\Control",  "8", "REG_DWORD", 0x2d)
	RegWrite($root&"\Control",  "9", "REG_DWORD", 0x0f)
	RegWrite($root&"\Control", "10", "REG_DWORD", 0x38)
	RegWrite($root&"\Control", "11", "REG_DWORD", 0x36)
	RegWrite($root&"\Debug", "replaynum", "REG_DWORD", 0x00)
	RegWrite($root&"\Option", "text_speed", "REG_DWORD", 0x03)
	RegWrite($root&"\Screen", "exploding_on", "REG_DWORD", 0x01)
	RegWrite($root&"\Screen", "gamma", "REG_DWORD", 0x0a)
	RegWrite($root&"\Screen", "lighting", "REG_DWORD", 0x01)
	RegWrite($root&"\Screen", "max_frame_rate", "REG_DWORD", 0x01)
	RegWrite($root&"\Screen", "min_frame_rate", "REG_DWORD", 0x00)
	RegWrite($root&"\Screen", "renderdevice", "REG_DWORD", 0x01)
	RegWrite($root&"\Screen", "rendername", "REG_SZ", "d3ddll.dll")
	RegWrite($root&"\Screen", "start_mode", "REG_DWORD", 0x00)
	RegWrite($root&"\Screen", "tripple_buffer", "REG_DWORD", 0x00)
	RegWrite($root&"\Screen", "videodevice", "REG_DWORD", 0x01)
	RegWrite($root&"\Screen", "videoname", "REG_SZ", "dmavideo.dll")
	RegWrite($root&"\Screen", "special_recognition", "REG_DWORD", 0x01)
Endfunc


Func prepare_host_registry($cmd)
	Local $map_id = $cmd[2]
	; Local $game_type = $cmd[3]
	Local $time = $cmd[4]
	Local $cops_enabled = $cmd[5]
	
	; Host settings (they need to be in the original registry path,
	; couldn't change that in the exe files yet)
	Local $root_original = "HKCU\Software\DMA Design Ltd\GTA2"
	prepare_common_registry_network_settings($root_original)
	
	RegWrite($root_original&"\Network", "map_index", "REG_DWORD", _
		$map_id)
	RegWrite($root_original&"\Network", "police", "REG_DWORD", _
		$cops_enabled)
		
	; TODO: game type
	
	RegWrite($root_original&"\Network", "game_time_limit", _
		"REG_DWORD", $time)
Endfunc


; This command starts a splitscreeen game by launching multiple
; instances of GTA2. The first one hosts a network game, the others
; join it. After starting it, the windows get attached to the main
; window and moved in the right positions.
Func cmd_splitscreen($cmd)
	Local $player_count = $cmd[1] ; Zero based!
	
	; Host settings (map id, game type etc.)
	prepare_host_registry($cmd)
	
	; Client settings (such as resultions etc.)
	For $i = 1 To ($player_count+1)
		prepare_common_registry_settings($i)
	Next
	
	; Launch all instances
	For $i=1 To ($player_count+1)
		Local $param = "-j 127.0.0.1"
		If $i == 1 Then $param = "-c"
	
		Run($global_config_path & "\G2HR_PLAYER" & $i & ".exe " _
			& $param, @ScriptDir & "\..\GTA2", @SW_SHOW)
	Next
	
	; Wait until all instances are connected somehow
Endfunc
