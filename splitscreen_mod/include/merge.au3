#include "screen_layout\screen_layout_calc.au3"

#cs
	- Prepare a player-number->resolution array
	- Create a fullscreen gui
	- Wait for the next gta2 instance
	- Rename it
	- Get the resolution
	- Move the window to the right position in the new window

#ce
Func merge($gameinfo, $config)
	; Prepare a player-number (0 to n) -> $geo array
	Local $player_to_geo[ $gameinfo[0] ]
	Local $screen_w = @DesktopWidth
	Local $screen_h = @DesktopHeight
	For $i = 0 To $gameinfo[0] -1
		$player_to_geo[$i] = screen_layout_calc($screen_w, $screen_h, 10, _
			$gameinfo[0] -1, $gameinfo[1], $i)
	Next


	; Wait for the next GTA2 instance
	WinWait("GTA2")

EndFunc