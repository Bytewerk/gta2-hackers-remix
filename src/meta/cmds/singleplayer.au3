; This command starts a single player GTA2 game.
; Syntax:
;	$cmd[1]: levelname (TODO)


Func cmd_singleplayer($cmd)

	; We'll need to change this (in the GTA2.exe file), so it doesn't
	; collide with normal installations of the game!
	$reg_base = "HKEY_CURRENT_USER\Software\DMA Design Ltd\GTA2\Debug"
	
	; Skip the menu, start the first map
	RegWrite($reg_base, "do_blood", 	"REG_DWORD",	1)
	RegWrite($reg_base, "mapname", 		"REG_SZ",		"wil.gmp")
	RegWrite($reg_base, "replaynum", 	"REG_DWORD",	2)
	RegWrite($reg_base, "savename",		"REG_DWORD",	0)
	RegWrite($reg_base, "scriptname",	"REG_SZ",		"wil.scr")
	RegWrite($reg_base, "skip_frontend","REG_BINARY",	1)
	RegWrite($reg_base, "stylename",	"REG_SZ",		"wil.sty")
	
	
	
	
	Run("GTA2/gta2.exe", "GTA2")
Endfunc
