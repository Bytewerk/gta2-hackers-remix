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
			Run(@ScriptDir & '\bin\registry_path_changer.exe ' &$i&' "' & $gta2_exe_path & '" "Player' & $i &'.exe"', $cache,@SW_HIDE)
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

; $config, $player_res, $geo: See arrays.txt
Func registry_prepare($config, $player_res)
	; Maybe this helps to fix some crashes?
	; RegDelete("HKEY_CURRENT_USER\Software\GTA2HackersRemix")

	For $i = 1 To $config[7] ; iterate over players 1...n
		Local $root = "HKEY_CURRENT_USER\Software\GTA2HackersRemix\P" & $i

		; Set the resolution
		Local $geo = $player_res[$i -1]
		RegWrite($root&"\Screen","full_width","REG_DWORD", $geo[2])
		RegWrite($root&"\Screen","full_height","REG_DWORD", $geo[3])
		RegWrite($root&"\Screen","window_width","REG_DWORD", $geo[2])
		RegWrite($root&"\Screen","window_height","REG_DWORD", $geo[3])

		; Force windowed mode
		RegWrite($root&"\Screen", "start_mode", "REG_DWORD", 0)

		; Set the player name
		RegWrite($root&"\Network", "PlayerName", "REG_SZ", "Player "&$i)

		; Working network settings, when selecting TCP/IP and 127.0.0.1.
		; Simply dumped with: ConsoleWrite(RegRead(...))
		; This might help with issue #6 - but it currently doesn't solve it.
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
	Next
EndFunc
