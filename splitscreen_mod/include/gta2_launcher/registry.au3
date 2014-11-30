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

	For $i = 1 To $player_count
		If Not FileExists($cache & "\Player" & $i & ".exe") Then _
			Run(@ScriptDir & '\bin\registry_path_changer.exe ' &$i&' "' & $gta2_exe_path & '" "Player' & $i &'.exe"', $cache,@SW_HIDE)
	Next
EndFunc

; $gameinfo, $player_res, $geo: See arrays.txt
Func registry_prepare($gameinfo, $player_res)
	For $i = 1 To $gameinfo[0] ; iterate over players 1...n
		Local $root = "HKEY_CURRENT_USER\Software\GTA2HackersRemix\P" & $i

		; Set the resolution
		Local $geo = $player_res[$i -1]
		RegWrite($root&"\Screen","full_width","REG_DWORD", $geo[2])
		RegWrite($root&"\Screen","full_height","REG_DWORD", $geo[3])
		RegWrite($root&"\Screen","window_width","REG_DWORD", $geo[2])
		RegWrite($root&"\Screen","window_height","REG_DWORD", $geo[3])

		; Force windowed mode
		RegWrite($root&"\Screen", "start_mode", "REG_DWORD", 0)

		; Set the player names
		RegWrite($root&"\Network", "PlayerName", "REG_SZ", "Player "&$i)
	Next
EndFunc
