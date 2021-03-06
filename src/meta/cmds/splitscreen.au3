#include <WinAPI.au3>
#include <WinAPIConstants.au3>
#include <WindowsConstants.au3> 

; Move all network GUIs off-screen (they can't be hidden properly in
; wine). Moved windows get renamed, so they don't get catched again.
Func wine_hide_network_windows()
	If Not $WINE Then Return
	While True
		Local $ret = WinMove("Network GTA2","", _
			@DesktopWidth + 1, @DesktopHeight + 1)
		If Not $ret Then Return
		WinSetTitle($ret, "", "Hidden G2HR Network UI")
	Wend
Endfunc

Func wait_for_host_window($pid, $ctrl_id)
	While True
		Local $hwnd = find_hwnd_with_control( _
			get_all_hwnds_from_pid($pid), $ctrl_id)
		If $hwnd Then _
			Return $hwnd
		
		wine_hide_network_windows()
		Sleep(100)
	Wend
Endfunc

Func wait_for_player_count($hwnd, $ctrl_id, $count)

	Local $last = -1
	
	While True
		Local $current = ControlListView($hwnd, "", $ctrl_id, _
			"GetItemCount")
		
		If $current <> $last Then
			wine_hide_network_windows()
			$last = $current
			status($last & "/" & $count & " PLAYERS JOINED")
		Endif
		
		If $current == $count Then _
			Return
		
		Sleep(100)
	Wend
Endfunc


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
	
	regwrite_if_empty($root&"\Network", "f_limit", "REG_DWORD", 0x03)
	regwrite_if_empty($root&"\Network", "game_speed", "REG_DWORD", 0x01)
	regwrite_if_empty($root&"\Network", "game_time_limit", _
		"REG_DWORD", 0x0e)
	regwrite_if_empty($root&"\Network", "game_type", "REG_DWORD", 0x01)
	regwrite_if_empty($root&"\Network", "map_index", "REG_DWORD", 0x00)
	regwrite_if_empty($root&"\Network", "police", "REG_DWORD", 0x00)
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
	RegWrite($root&"\Screen", "gamma", "REG_DWORD", 0x0a)
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
	; FIXME: probably need t load more registry settings here!
	
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
	Local $ini = "data/g2hr.ini"
	Local $ini_hide_network_windows = IniRead($ini, "splitscreen", _
		"hide_network_windows", "true") == "true"
	Local $ini_merge_windows = IniRead($ini, "splitscreen", _
		"merge_windows", "true") == "true"
	Local $ini_sleep_before_merge_ms = IniRead($ini, "splitscreen", _
		"merge_windows", 0000)
	Local $ini_move_windows = IniRead($ini, "splitscreen", _
		"move_windows", "true") == "true"
	Local $ini_activate_last_player_window = IniRead($ini, _
		"splitscreen", "activate_last_player_window", "true") == true
	Local $player_count = $cmd[1] ; Zero based!
	
	; both methods don't work in wine!
	; If $WINE Then WinSetOnTop($HWND_SDL, "", 1)
	; If $WINE Then _WinAPI_SetWindowLong ($HWND_SDL, $GWL_EXSTYLE, _
	;	$WS_EX_TOPMOST)
	
	
	; Host settings (map id, game type etc.)
	status("PREPARING THE REGISTRY")
	prepare_host_registry($cmd)
	
	; Client settings (such as resultions etc.)
	For $i = 1 To ($player_count+1)
		prepare_common_registry_settings($i)
	Next
	
	status("STARTING ALL INSTANCES")
	
	; Launch all instances
	For $i=1 To ($player_count+1)
		Local $param = "-j 127.0.0.1"
		If $i == 1 Then $param = "-c"
		
		$global_game_process_ids[$i-1] =  Run($global_config_path _
			& "\G2HR_PLAYER" & $i & ".exe " _
			& $param, "GTA2", $ini_hide_network_windows ? @SW_HIDE _
			: @SW_SHOW)
	Next
	$global_game_instances_open = $player_count + 1
	send_pid_table()
	
	; Wait until all instances are connected to the host
	Local $host_pid = $global_game_process_ids[0]
	Local $hwnd = wait_for_host_window($host_pid, _
		$GTA2_LOBBY_CTRL_LIST)
	
	wait_for_player_count($hwnd, $GTA2_LOBBY_CTRL_LIST, _
		$player_count +1)
	
	
	; Press the start button!
	ControlClick($hwnd,"",$GTA2_LOBBY_CTRL_START)
	
	;
	; FIXME:
	; The code below needs better testing and fixing. It should react
	; when a GTA2 instance crashes and restart it for example! Also it
	; should find the right timing, when to move windows so that GTA2
	; won't crash. Patches welcome!
	;
	
	; Try to avoid directx errors, when we move the window too early
	Sleep($ini_sleep_before_merge_ms)
	
	; Wait for all windows and move them to the right place
	For $i = $player_count To 0 Step -1
		status(($player_count - $i + 1) & "/" & ($player_count+1) _
			& " GAME WINDOWS OPEN")
	
		$hwnd = wait_for_hwnd_with_desc($global_game_process_ids[$i], _
			$GTA2_GAME_WINDOW_DESC)
		
		If Not $WINE And $ini_merge_windows Then _
			_WinAPI_SetParent($hwnd, $HWND_SDL)
		
		Local $geo = $global_game_screen_layouts[$i]
		
		If $ini_move_windows Then
			move_until_it_works($hwnd, $geo) ; FIXME
		Endif
	Next
	
	If $ini_hide_network_windows Then
		
		status("SHOWING ALL WINDOWS")
		
		; Show all windows "at the same time"
		If $WINE Then
			; this doesn't work in wine.
			; WinSetOnTop($HWND_SDL, "", 0)
		Else	
			For $i = $player_count To 0 Step -1
				$hwnd = wait_for_hwnd_with_desc($global_game_process_ids[$i], _
					$GTA2_GAME_WINDOW_DESC)
				
				If Not $WINE Then WinSetState($hwnd, "", @SW_SHOW)
			Next
		Endif
	Endif
	
	If $ini_activate_last_player_window Then
		; Activate the last player's window, so it could be played with
		; the keyboard
		WinActivate(wait_for_hwnd_with_desc( _
			$global_game_process_ids[$player_count], _
			$GTA2_GAME_WINDOW_DESC))
	Endif
Endfunc
