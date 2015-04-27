; This command starts a single player GTA2 game.
; Syntax:
;	$cmd[1]: levelpack ("GTA2", "MY_CUSTOM_PACK_1", ...)
;	$cmd[2]: type ("NORMAL", "BONUS")
;	$cmd[3]: id (eg. "1"-"3" or "A"-"I")
;
; Credit to Sektor for his GTA2 launcher, reading 'frmLauncher.frm' from
; its source code was very helpful while creating this function!
; 		http://gtamp.com/forum/viewtopic.php?f=4&t=456
;
; FIXME: reg_base should point somewhere else, but we'll need to adjust
; that in GTA2.exe, too!
;
Func cmd_singleplayer($cmd)
	Local $reg_base = "HKEY_CURRENT_USER\Software\DMA Design Ltd\GTA2\Debug"
	Local $levelpack = $cmd[1]
	Local $type		 = $cmd[2]
	Local $id		 = String($cmd[3])
	Local $gmp		 = ""
	Local $scr		 = ""
	Local $sty		 = ""
	
	If $levelpack == "GTA2" Then
		If $type == "NORMAL" Then
			If $id == "1" Then ; Downtown
				$sty = "wil"
			Elseif $id == "2" Then ; Residential
				$sty = "ste"
			Elseif $id == "3" Then ; Industrial
				$sty = "bil"
			Endif
		Elseif $type == "BONUS" Then
			If $id == "A" Then
				$sty = "wil"
				$gmp = "lorne2e"
			Elseif $id == "B" Then
				$sty = "wil"
				$gmp = "lorne2m"
			Elseif $id == "C" Then
				$sty = "wil"
				$gmp = "lorne2h"
			Elseif $id == "D" Then
				$sty = "ste"
				$gmp = "mike1e"
			Elseif $id == "E" Then
				$sty = "ste"
				$gmp = "mike1m"
			Elseif $id == "F" Then
				$sty = "ste"
				$gmp = "mike1h"
			Elseif $id == "G" Then
				$sty = "bil"
				$gmp = "mike2e"
			Elseif $id == "H" Then
				$sty = "bil"
				$gmp = "mike2m"
			Elseif $id == "I" Then
				$sty = "bil"
				$gmp = "mike2h"
			Endif
		Endif
	Endif
	
	if $sty == "" Then _
		Return Msgbox(16,"G2HR", "level not yet implemented:" & @crlf _
			& $levelpack & " " & $type & " " & $id)
	If $gmp == "" Then $gmp = $sty
	If $scr == "" Then $scr = $gmp
	
	; Write everything to the registry
	RegWrite($reg_base, "skip_frontend","REG_BINARY",	1)
	RegWrite($reg_base, "do_blood", 	"REG_DWORD",	1)
	RegWrite($reg_base, "replaynum", 	"REG_DWORD",	2)
	RegWrite($reg_base, "savename",		"REG_DWORD",	0)
	RegWrite($reg_base, "mapname", 		"REG_SZ",		$gmp&".gmp")
	RegWrite($reg_base, "scriptname",	"REG_SZ",		$scr&".scr")
	RegWrite($reg_base, "stylename",	"REG_SZ",		$sty&".sty")
	
	; Start the game
	Run("GTA2/gta2.exe", "GTA2")
	WinWait("GTA2")
	WinActivate("GTA2")
Endfunc
