; Alpha GUI, because we actually want a fullscreen gui. Reasons:
; - We could preload a full $controller_count network lobby in the background
; - We could make gamepad support for the GUI, so the users don't need to switch
;	between keyboard and gamepad
; - Looks much more streamlined

; TODO:
; - Add single player button

#NoTrayIcon

#include <ButtonConstants.au3>
#include <ComboConstants.au3>
#include <EditConstants.au3>
#include <GUIConstantsEx.au3>
#include <StaticConstants.au3>
#include <WindowsConstants.au3>
#include <Array.au3>


#include "../include/screen_layout/screen_layout_db.au3"
#include "../include/screen_layout/screen_layout_calc.au3"
#include "../include/logging.au3"


; When $activate is not set, the function returns the
; active button handler
Func alphagui_buttons_active($array, $activate = Null)
	Local $i
	For $i = 0 To UBound($array) - 1
		If $activate == Null Then
			If BitAND(GUICtrlGetState($array[$i]), $GUI_DISABLE) Then
				Return $array[$i]
			EndIf
		Else
			If $array[$i] == $activate Then
				GUICtrlSetState($array[$i],$GUI_DISABLE)
			Else
				GUICtrlSetState($array[$i], $GUI_ENABLE)
			EndIf
		EndIf
	Next
	Return Null
EndFunc

Func alphagui_buttons_active_value($array)
	Local $active = alphagui_buttons_active($array)
	If $active == Null Then Return ""
	Return GUICtrlRead($active)
EndFunc

; returns true, when a button has been clicked
Func alphagui_buttons_handle_msg($array, $msg)
	For $i=0 To Ubound($array) -1
		If $msg == $array[$i] Then
			alphagui_buttons_active($array, $array[$i])
			Return True
		EndIf
	Next
	Return False
EndFunc

Func alphagui_buttons_screenlayout($player_count, $btn_layouts_old = NULL)
	Local $variations = screen_layout_db_variation_count($player_count -1)
	Local $btn_layouts[$variations]
	Local $i

	; Delete old buttons, if there are any
	If $btn_layouts_old <> NULL Then
		For $i=0 To UBound($btn_layouts_old) -1
			GUICtrlDelete($btn_layouts_old[$i])
		Next
	EndIf

	; Draw new buttons
	For $i= 0 To $variations -1
		$btn_layouts[$i] = GUICtrlCreateButton($i, 80+$i*30, 125, 25, 25)
	Next

	; Set the first one to active
	alphagui_buttons_active($btn_layouts, $btn_layouts[0])
	return $btn_layouts
EndFunc

Func alphagui_screen_preview_draw($player_count, $variation, $preview_old = NULL)
	Local $preview[$player_count]
	Local $i

	; Delete controls, if there are any
	If $preview_old <> NULL Then
		For $i=0 To UBound($preview_old) -1
			GUICtrlDelete($preview_old[$i])
		Next
	EndIf

	For $i = 0 To $player_count -1
		; Func screen_layout_calc($screen_w, $screen_h, $border_size_px, _
		; $player_count, $variation, $player)
		Local $geo = screen_layout_calc(172, 120, 4, $player_count-1, $variation, $i)
		Local $left = 80
		Local $top = 160
		Local $x = $geo[0]
		Local $y = $geo[1]
		Local $w = $geo[2]
		Local $h = $geo[3]

		$preview[$i] = GUICtrlCreateLabel("",$left + $x, $top + $y, _
			$w, $h, $SS_BLACKFRAME)
	Next

	Return $preview
EndFunc

#cs
	This GUI exits the program, if it gets closed.
	When the player clicks on start, it returns a
	$gameinfo array (see arrays.txt).
#ce
Func alphagui()
	Local $gui = GUICreate("GTA2 Splitscreen Mod", 601, 351, 243, 146)

	; Title
	GUICtrlCreateLabel("GTA2 Hackers Remix: Splitscreen Mod ALPHA", 20, 30, 537, 25)
	GUICtrlSetFont(-1, 16, 400, 0, "Lucida Console")
	; Left side
	GUICtrlCreateLabel("Players", 20, 100, 38, 17)
	GUICtrlCreateLabel("Layout", 20, 130, 36, 17)
	GUICtrlCreateLabel("Map", 300, 100, 25, 17)

	; Draw layout buttons
	Local $btn_layouts = alphagui_buttons_screenlayout(2)

	; Draw player buttons
	Local $btn_players[5]
	For $i = 0 To 4
		$btn_players[$i] = GUICtrlCreateButton($i+2, 80+$i*30, 95, 25, 25)
	Next
	alphagui_buttons_active($btn_players, $btn_players[0])
	Local $preview = alphagui_screen_preview_draw(2, 0)

	; Right side
	$combo_map = GUICtrlCreateCombo("Hidden Surprise", 400, 95, 170, 25, BitOR($CBS_DROPDOWN,$CBS_AUTOHSCROLL))
	GUICtrlCreateLabel("Game Type", 300, 130, 59, 17)

	Local $btn_gametypes = ["Frags", "Points", "Tag"]
	For $i = 0 To 2
		$btn_gametypes[$i] = GUICtrlCreateButton($btn_gametypes[$i], _
			400 + $i * 60, 125, 50, 25)
	Next
	alphagui_buttons_active($btn_gametypes,$btn_gametypes[0])

	$lbl_win_condition = GUICtrlCreateLabel("Frags to Win", 300, 160, 64, 17)
	$npt_win_condition = GUICtrlCreateInput("10", 400, 155, 170, 21, BitOR($GUI_SS_DEFAULT_INPUT,$ES_NUMBER))
	GUICtrlCreateLabel("Time Limit (minutes)", 300, 190, 96, 17)
	$npt_time_limit = GUICtrlCreateInput("5", 400, 185, 170, 21)
	GUICtrlCreateLabel("Cops enabled", 300, 220, 69, 17)


	Local $btn_cops = ["Yes", "No"]
	For $i = 0 To 1
		$btn_cops[$i] = GUICtrlCreateButton($btn_cops[$i], _
			400 + $i * 60, 215, 50, 25)
	Next
	alphagui_buttons_active($btn_cops,$btn_cops[0])

	; Start button and some links
	$btn_start = GUICtrlCreateButton("GTA2", 400, 300, 170, 25)
	$link_gtamp = GUICtrlCreateLabel("gtamp thread", 160, 312, 100, 17)
	GUICtrlSetColor(-1, 0x0000FF)
	$link_bytewerk = GUICtrlCreateLabel("bytewerk", 80, 312, 47, 17)
	GUICtrlSetColor(-1, 0x0000FF)
	$link_github = GUICtrlCreateLabel("github", 20, 312, 33, 17)
	GUICtrlSetColor(-1, 0x0000FF)
	GUISetState(@SW_SHOW)


	While 1
		Local $msg = GUIGetMsg()

		If $msg = $GUI_EVENT_CLOSE Then _
			Exit
		If $msg = $link_github Then _
			ShellExecute("https://github.com/Bytewerk/gta2-hackers-remix")
		If $msg = $link_bytewerk Then _
			ShellExecute("http://bytewerk.org")
		If $msg = $link_gtamp Then _
			ShellExecute("http://gtamp.com/forum/viewtopic.php?t=776")


		; return $gameinfo (see arrays.txt)
		If $msg = $btn_start Then
			Local $gameinfo[7]
			$gameinfo[0] = alphagui_buttons_active_value($btn_players)
			$gameinfo[1] = alphagui_buttons_active_value($btn_layouts)
			$gameinfo[2] = GUICtrlRead($combo_map)
			$gameinfo[3] = alphagui_buttons_active_value($btn_gametypes)
			$gameinfo[4] = GUICtrlRead($npt_win_condition)
			$gameinfo[5] = GUICtrlRead($npt_time_limit)
			$gameinfo[6] = (alphagui_buttons_active_value($btn_cops) == "Yes")
			GUIDelete()
			Return $gameinfo
		EndIf


		alphagui_buttons_handle_msg($btn_cops,$msg)

		If alphagui_buttons_handle_msg($btn_gametypes,$msg) Then
			Local $active = alphagui_buttons_active_value($btn_gametypes)
			If $active == "Frags" Or $active == "Points" Then
				GUICtrlSetData($lbl_win_condition,$active & " to Win")
				GUICtrlSetState($lbl_win_condition,$GUI_SHOW)
				GUICtrlSetState($npt_win_condition,$GUI_SHOW)
			Else
				GUICtrlSetState($lbl_win_condition,$GUI_HIDE)
				GUICtrlSetState($npt_win_condition,$GUI_HIDE)
			EndIf
		EndIf

		Local $layout_redraw = False
		If alphagui_buttons_handle_msg($btn_players, $msg) Then
			$btn_layouts = alphagui_buttons_screenlayout(alphagui_buttons_active_value($btn_players), $btn_layouts)
			$layout_redraw = True
		EndIf

		If alphagui_buttons_handle_msg($btn_layouts, $msg) Or $layout_redraw Then
			$preview = alphagui_screen_preview_draw(alphagui_buttons_active_value($btn_players), _
				alphagui_buttons_active_value($btn_layouts), _
				$preview)
		EndIf
	WEnd
EndFunc