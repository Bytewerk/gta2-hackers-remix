#include <WinAPI.au3>

; Syntax:
;	$cmd[1]: FULLSCREEN
;	$cmd[2]: ON | OFF
;
Func cmd_fullscreen($cmd)
	If $cmd[1] == "ON" Then
		$global_fullscreen = true
		hide_taskbar_when_menu_focused()
	Else
		$global_fullscreen = false
		taskbar_hide(false)
	Endif
Endfunc
