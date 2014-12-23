#AutoIt3Wrapper_icon=g2hr.ico
#NoTrayIcon
#include <Array.au3>
#include "include\logging.au3"
#include "include\screen_layout\screen_layout_init.au3"
#include "include\screen_layout\screen_layout_calc.au3"
#include "alphagui\alphagui.au3"
#include "include\config.au3"
#include "include\gta2_launcher\registry.au3"
#include "include\gta2_launcher\lobby.au3"
#include "include\merge.au3"
#include "include\statuswin.au3"
#include "include\gamepad_glue.au3"
#include "include\taskbar.au3"
#include "include\other.au3"

Func die()
	Local $i
	l("Shutting down...")
	taskbar_hide(False)
	For $i = 0 To 6
		ProcessClose("Player"&$i&".exe")
	Next
	ProcessClose("sdl_controller_code.exe")
	Exit
EndFunc

; $config: See arrays.txt
$config = config_init()

If $config[9] And EnvGet("G2HR_WINE_SCRIPT") == "" Then _
	Exit MsgBox(16, "G2HR", "When running in wine, please execute the bundled splitscreen_wine.sh instead.")

screen_layout_init()

Global $statuswin = statuswin_create()
l("GTA2 Hackers Remix: Splitscreen Mod Alpha")
l("-------------------------------------------")
l("[F1]: Toggle this window")
l("[F2]: Quit")
l("[F3]: Force ingame windows to have sound")
l("-------------------------------------------")
HotKeySet("{F1}", "statuswin_toggle")
HotKeySet("{F2}","die")
HotKeySet("{F3}","fake_focus")


; Show the GUI until the user starts the game.
; See arrays.txt for the $gameinfo structure.
alphagui($config)
config_save($config)

registry_exe_modify($config[7], $config[0])
$player_res = screen_layout_calc_array($config)
registry_prepare($config, $player_res)
lobby_run($config)

; Avoid flickering
WinSetTrans($statuswin[0],"",255)

merge($config, $player_res)
gamepad_glue($config)


; When Gamepad Glue has quit (eg. because no players)
; were attached, keep the game open for screenshots.
l("Press [F2] to shut the mod down properly!")
While 1
	Sleep(1000)
	; fake_focus()
WEnd

