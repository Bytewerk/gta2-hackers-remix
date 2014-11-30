#include "screen_layout\screen_layout_calc.au3"

#cs
	- Prepare a player-number->resolution array
	- Create a fullscreen gui
	- Wait for the next gta2 instance
	- Rename it
	- Get the resolution
	- Move the window to the right position in the new window
#ce
Func merge($gameinfo, $player_res)
	Local $i
	Local $j

	For $i = 0 To UBound($player_res) -1
		; TODO: use more specific filter than the title.
		WinWait("GTA2")
		Local $hwnd = WinGetHandle("GTA2")
		Local $pos = WinGetPos($hwnd)
		Local $width  = $pos[0]
		Local $height = $pos[1]

		; Try to find the matching resolution and rename
		; the window according to the player number
		For $j = 0 To UBound($player_res) -1
			Local $geo = $player_res[$j]
			If $geo[2] <> $width Or $geo[3] <> $height Then ContinueLoop

			WinSetTitle($hwnd, "", "GTA2: Player " & ($i+1))
			ExitLoop
		Next
	Next

	; Wait for the next GTA2 instance
	WinWait("GTA2")

EndFunc