#include <AutoItConstants.au3>
#include "installer_globals.au3"

; I have no idea why, but if finding the HWND once and then trying to read
; the title, it always fails! Patches welcome.
Func nogui_callback_status($pid, $prefix)
	While ProcessExists($pid)
		Local $list = WinList()
		For $i=0 To $list[0][0]
			If StringInStr($list[$i][0],$prefix,1) Then
				Return StringMid($list[$i][0], StringLen($prefix)+1)
			EndIf
		Next
		Sleep(100)
	WEnd
EndFunc

; Runs the callback function with the c-backend and returns the final status
; as string.
; Note: instead of commandline arguments, we use environment variables.
; Otherwise we'd have to build a commandline and escape spaces etc. in the
; nogui installer's path (which could be anywhere!)
Func nogui_callback($action, $parameter_name, $parameter_value, $progress_bar)
	EnvSet("G2HR_INSTALL_ACTION", $action)
	EnvSet($parameter_name, $parameter_value)
	ProcessClose($installer_nogui_path)

	Local $pid = Run($installer_nogui_path)
	If $action == "cleanup" Then Exit

	Local $prefix = "<G2HR_INSTALLER_STATUS>"
	Local $old = ""
	While ProcessExists($pid)
		Local $new = nogui_callback_status($pid, $prefix)
		If $old <> $new Then
			Local $split = StringSplit($new," ")
			If $split[0] < 2 Then ContinueLoop

			If $split[1] == "PERCENT:" Then
				GUICtrlSetData($progress_bar, $split[2])
			ElseIf $split[1] == "DONE:" Then
				Return StringMid($new,7)
			EndIf
			$old = $new
		EndIf
		Sleep(100)
	WEnd
EndFunc
