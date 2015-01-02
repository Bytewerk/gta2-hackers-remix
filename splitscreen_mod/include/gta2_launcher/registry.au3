; This function creates up to $player_count
; exe files, which are like GTA2.exe, but
; with a different registry path per player.
;
; The files get cached in AppData.
;

#include "..\screen_layout\screen_layout_calc.au3"
#include <Array.au3>

Func registry_exe_modify($player_count, $gta2_exe_path)
	Local $cache = @AppDataDir & "\GTA2 Hackers Remix\Modified GTA2.exe Cache"
	DirCreate($cache)

	; Copy the GTA2.exe to the cache folder, each time with a different
	; Registry path (so we can control the screen resolutions independently)
	For $i = 1 To $player_count
		If Not FileExists($cache & "\Player" & $i & ".exe") Then _
			RunWait(@ScriptDir & '\bin\registry_path_changer.exe ' &$i&' "' & $gta2_exe_path & '" "Player' & $i &'.exe"', $cache,@SW_HIDE)
	Next


	Local $gta2dir = StringMid($gta2_exe_path, 1, _
		StringInStr($gta2_exe_path, "\", 1, -1))

	; Copy the proxy_dll.dll to $cache/dmavideo.dll and the
	; real dmavideo.dll to $cache/original_dmavideo.dll
	If Not FileExists($cache & "\dmavideo.dll") Then _
		FileCopy(@ScriptDir & "\bin\proxy_dll.dll", $cache & "\dmavideo.dll")
	If Not FileExists( $cache & "\dmavideo_original.dll") Then _
		FileCopy($gta2dir & "\dmavideo.dll", $cache & "\dmavideo_original.dll")
EndFunc


Func regwrite_if_empty($keyname, $valuename, $type, $value)
	RegRead($keyname, $valuename)
	If @Error Then _
		RegWrite($keyname, $valuename, $type, $value)
EndFunc


Func registry_prepare_network($root)
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
	regwrite_if_empty($root&"\Network", "game_time_limit", "REG_DWORD", 0x0e)
	regwrite_if_empty($root&"\Network", "game_type", "REG_DWORD", 0x01)
	regwrite_if_empty($root&"\Network", "map_index", "REG_DWORD", 0x00)
	regwrite_if_empty($root&"\Network", "police", "REG_DWORD", 0x00)
	regwrite_if_empty($root&"\Network", "show_player_names", "REG_DWORD", 0x01)

EndFunc

; $config, $player_res, $geo: See arrays.txt
Func registry_prepare($config, $player_res)


	; Write the network settings to the normal registry path.
	; They seem to always get loaded from there, even if the path
	; was hacked like in Player1.exe to Player6.exe.
	$root = "HKEY_CURRENT_USER\Software\DMA Design Ltd\GTA2";
	RegWrite($root&"\Screen", "special_recognition", "REG_DWORD", 0x01)
	registry_prepare_network($root);


   $root = "HKEY_CURRENT_USER\Software\Aureal\A3D"
   RegWrite($root, "SplashAudio", "REG_DWORD", 0x00)
   RegWrite($root, "SplashScreen", "REG_DWORD", 0x00)


	; Player1.exe to Player6.exe have different registry paths
	; (that is the main purpose for the exe files!). Write all
	; registry settings for each instance here.
	For $i = 1 To $config[7]
		$root = "HKEY_CURRENT_USER\Software\GTA2HackersRemix\P" & $i

		; First set the stuff we care about:
		; resolution, windowed mode, player name,
		; no car-radio (imagine 6 players in cars ;) )
		Local $geo = $player_res[$i -1]
		RegWrite($root&"\Screen","full_width","REG_DWORD", $geo[2])
		RegWrite($root&"\Screen","full_height","REG_DWORD", $geo[3])
		RegWrite($root&"\Screen","window_width","REG_DWORD", $geo[2])
		RegWrite($root&"\Screen","window_height","REG_DWORD", $geo[3])
		RegWrite($root&"\Screen", "start_mode", "REG_DWORD", 0)
		RegWrite($root&"\Network", "PlayerName", "REG_SZ", "Player "&$i)
		RegWrite($root&"\Sound",  "CDVol", "REG_DWORD", 0x00)
		RegWrite($root&"\Sound",  "SFXVol", "REG_DWORD", 0x7f)

		; Set some default values
		; (without some of these, GTA2 won't start! see bug #6)
		registry_prepare_network($root);
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
	Next
EndFunc













