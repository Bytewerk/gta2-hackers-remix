#include <WindowsConstants.au3>
#include <GUIConstantsEx.au3>
#include <WinAPI.au3>
#include <Array.au3>
#include "screen_layout\screen_layout_calc.au3"
#include "logging.au3"


#cs
	We're comparing the outer window size
	(which includes the window border) with the
	size it should have, so we need some kind of
	tolerance.
#ce
Func roughly_same($a, $b, $tolerance)
	Return $b <= $a + $tolerance And $b >= $a - $tolerance
EndFunc

#cs
	- Create a fullscreen GUI
	- iterate through the player_res array
		find a matching GTA2 window (from WinList)
		merge that window
#ce
Func merge($gameinfo, $config, $player_res)

	; Create a Fullscreen GUI
	Local $full = GUICreate("Hackers Remix", $config[1] ,$config[2], _
		0, 0, $WS_POPUP);, BitOR($WS_EX_TOOLWINDOW, $WS_EX_TOPMOST))
	GUISetBkColor(0x000000)
	GUISetCursor(16,1)
	GUISetState(@SW_SHOW)


	; Iterate through player_res
	Local $i
	For $i = 0 To UBound($player_res) -1
		WinWait("GTA2")
		Local $geo_player = $player_res[$i]
		Local $list = WinList("GTA2")
		Local $j
		For $j = 1 To $list[0][0]
			Local $hwnd = $list[$j][1]
			Local $geo_win = WinGetPos($hwnd)

			If roughly_same($geo_win[2],$geo_player[2],20) _
				And roughly_same($geo_win[3],$geo_player[3],80) Then

				; TODO: remove window borders

				; If we're here, we found the right window.
				; Merge it now!
				_WinAPI_SetParent($hwnd,$full)
				_WinAPI_SetWindowPos($hwnd, $HWND_BOTTOM, _
					$geo_player[0], $geo_player[1], _
					$geo_player[2], $geo_player[3], _
					$SWP_NOSIZE)

				; Remove "GTA2" from the title, so it won't be
				; matched again
				WinSetTitle($hwnd,"", "Player "&($i+1))

				ExitLoop
			EndIf
		Next
	Next

	While 1
		Local $msg = GUIGetMsg()
		If $msg = $GUI_EVENT_CLOSE Then ExitLoop
	WEnd
EndFunc