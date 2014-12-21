#include <WindowsConstants.au3>
#include <GUIConstantsEx.au3>
#include <WinAPI.au3>
#include <Array.au3>
#include "screen_layout\screen_layout_calc.au3"
#include "logging.au3"
#include "taskbar.au3"


#cs
	We're comparing the outer window size
	(which includes the window border) with the
	size it should have, so we need some kind of
	tolerance.
#ce
Func roughly_same($a, $b, $tolerance)
	Return $b <= $a + $tolerance And $b >= $a - $tolerance
EndFunc

Func remove_style($hwnd, $style)
	_WinAPI_SetWindowLong($hwnd, $GWL_STYLE, _
		BitXOr($style, _WinAPI_GetWindowLong($hwnd,$GWL_STYLE)))
EndFunc

#cs
	- Create a fullscreen GUI
	- iterate through the player_res array
		find a matching GTA2 window (from WinList)
		merge that window
#ce
Func merge($config, $player_res)

	If $config[6] Then taskbar_hide(True)

	If $config[4] Then
		l("Creating fullscreen GUI")
		Local $full = GUICreate("G2HR", $config[1] ,$config[2], _
			0, 0, $WS_POPUP)
		GUISetBkColor(0x000000)
		GUISetCursor(16,1)
		GUISetState(@SW_SHOW)
		WinActivate($full)

		; If we do the merge stuff directly, the games will have a
		; black screen and crash - not sure about that anymore.
		l("Waiting for games to initialize")
		sleep($config[5])
	Else
		l("Fullscreen (merge) GUI is disabled")
	EndIf


	; Iterate backwards through player_res, so the window title
	; of the lower windows get covered by the higher ones.
	; TODO: add a real splitscreen border
	Local $i
	For $i = UBound($player_res) -1 To 0 Step -1
		WinWait("GTA2")
		Local $geo_player = $player_res[$i]
		Local $list = WinList("GTA2")
		Local $j
		For $j = 1 To $list[0][0]
			Local $hwnd = $list[$j][1]
			Local $geo_win = WinGetPos($hwnd)

			If roughly_same($geo_win[2],$geo_player[2],20) _
				And roughly_same($geo_win[3],$geo_player[3],80) Then

				; Removing the window border doesn't work as good as
				; just placing the borders off-screen. We assume that
				; the bottom border has the same size as the one on the
				; left and right.
				Local $offset_x = ($geo_win[2] - $geo_player[2]) / 2
				Local $offset_y = ($geo_win[3] - $geo_player[3]) - $offset_x
				Local $pos_x	= $geo_player[0] - $offset_x
				Local $pos_y	= $geo_player[1] - $offset_y

				l("Border Offset: " & $offset_x & ", " & $offset_y, $i+1)

				If $config[4] Then
					l("Merging window",$i+1)
					_WinAPI_SetParent($hwnd,$full)
				EndIf

				_WinAPI_SetWindowPos($hwnd, $HWND_BOTTOM, _
					$pos_x, $pos_y, _
					$geo_player[2], $geo_player[3], _
					$SWP_NOSIZE)

				; This doesn't work!
				; remove_style($hwnd, $WS_CAPTION)
				; remove_style($hwnd, $WS_BORDER)
				; remove_style($hwnd, $WS_DLGFRAME)
				; _WinAPI_RedrawWindow($hwnd)


				; Remove "GTA2" from the title, so it won't be
				; matched again
				WinSetTitle($hwnd,"", "Player "&($i+1))
				WinSetState($hwnd,"",@SW_SHOW)
				ExitLoop
			EndIf
		Next
	Next

	l("Windows SHOULD BE merged now")
	l("(it's alpha, don't expect too much :p)")

	l("Sending fake focus message")
	fake_focus()
EndFunc