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

Func alphagui_ischecked($ctrl)
    Return BitAND(GUICtrlRead($ctrl), $GUI_CHECKED) = $GUI_CHECKED
EndFunc




#cs
	This GUI exits the program, if it gets closed.
	When the player clicks on start, it returns a
	$gameinfo array (see arrays.txt).
#ce
Func alphagui(ByRef $config)
	Local $version = "0.3.1-splitscreen_mod"
	Local $gui = GUICreate("G2HR", 601, 351, 243, 146)

	; Title
	GUICtrlCreateLabel("GTA2 Hackers Remix: Splitscreen Mod ALPHA", 20, 30, 537, 25)
	GUICtrlSetFont(-1, 16, 400, 0, "Lucida Console")
	GUICtrlCreateLabel($version, 440, 48, 120, 17)

	; Left side
	GUICtrlCreateLabel("Players", 20, 100, 38, 17)
	GUICtrlCreateLabel("Layout", 20, 130, 36, 17)
	GUICtrlCreateLabel("Map", 300, 100, 25, 17)

	; Draw layout buttons
	Local $btn_layouts = alphagui_buttons_screenlayout($config[7])
	alphagui_buttons_active($btn_layouts, $btn_layouts[ $config[8] ])

	; Draw player buttons
	Local $btn_players[5]
	For $i = 0 To 4
		$btn_players[$i] = GUICtrlCreateButton($i+2, 80+$i*30, 95, 25, 25)
	Next
	alphagui_buttons_active($btn_players, $btn_players[ $config[7] - 2 ])
	Local $preview = alphagui_screen_preview_draw($config[7], $config[8])

	; Right side
	If Not $config[9] Then
	   GUICtrlCreateGroup("Splitscreen Hack Options", 280, 152, 281, 113)
	   $chk_merge = GUICtrlCreateCheckbox("Merge all GTA2 windows to a big one", 296, 176, 233, 17)
	   If $config[4] == "True" Then GUICtrlSetState(-1,$GUI_CHECKED)

	   GUICtrlCreateLabel("Sleep before merge:", 312, 202, 99, 17)
	   $ipt_merge_sleep = GUICtrlCreateInput($config[5], 424, 200, 57, 21, $ES_NUMBER)
	   GUICtrlCreateLabel("ms", 488, 202, 17, 17)
	   $ck_hide_taskbar = GUICtrlCreateCheckbox("Hide Windows Taskbar", 296, 232, 209, 17)
	   If $config[6] == "True" Then GUICtrlSetState(-1,$GUI_CHECKED)
   EndIf

	GUICtrlCreateLabel("Screen layouts with other aspect ratios than 4:3 will cause graphical and/or camera glitches.", 280, 96, 282, 33)

	; Start button and some links
	$btn_start = GUICtrlCreateButton("&GTA2", 400, 300, 170, 25)
	$link_gtamp = GUICtrlCreateLabel("gtamp thread", 160, 312, 100, 17)
	GUICtrlSetColor(-1, 0x0000FF)
	$link_bytewerk = GUICtrlCreateLabel("bytewerk", 80, 312, 47, 17)
	GUICtrlSetColor(-1, 0x0000FF)
	$link_github = GUICtrlCreateLabel("github", 20, 312, 33, 17)
	GUICtrlSetColor(-1, 0x0000FF)

	GUISetState(@SW_SHOW)

	While 1
		Local $msg = GUIGetMsg()

		If $msg = $GUI_EVENT_CLOSE Then
			l("Shutting down...")
			Exit
		EndIf
		If $msg = $link_github Then _
			ShellExecute("https://github.com/Bytewerk/gta2-hackers-remix")
		If $msg = $link_bytewerk Then _
			ShellExecute("http://bytewerk.org")
		If $msg = $link_gtamp Then _
			ShellExecute("http://gtamp.com/forum/viewtopic.php?t=776")

		; Start: Write all information back to $config and return
		If $msg = $btn_start Then
		   If Not $config[9] Then
			   $config[4] = alphagui_ischecked($chk_merge)
			   $config[5] = GUICtrlRead($ipt_merge_sleep)
			   $config[6] = alphagui_ischecked($ck_hide_taskbar)
			Else
			   $config[4] = False
			   $config[5] = False
			   $config[6] = False
			EndIf
			$config[7] = alphagui_buttons_active_value($btn_players)
			$config[8] = alphagui_buttons_active_value($btn_layouts)
			GUIDelete()
			Return
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