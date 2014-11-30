#include-once
#include <WindowsConstants.au3>
#include <GUIConstantsEx.au3>


Func statuswin_toggle()
	Global $statuswin
	If $statuswin[8] Then
		GUISetState(@SW_HIDE, $statuswin[0])
	Else
		GUISetState(@SW_SHOW, $statuswin[0])
	EndIf
	$statuswin[8] = Not $statuswin[8]
EndFunc


; Returns $statuswin (see arrays.txt)
Func statuswin_create()
	Local $statuswin[9]
	Local $x = @DesktopWidth - 450
	Local $y = @DesktopHeight - 140
	$statuswin[0] = GUICreate("G2HR: Status",400,90, $x, $y, _
		$WS_POPUP, BitOR($WS_EX_TOPMOST, $WS_EX_TOOLWINDOW))
	GUISetBkColor(0x000000)

	$statuswin[1] = GUICtrlCreateLabel("",10,10,380,80)
	GUICtrlSetColor(-1,0xffffff)
	GUICtrlSetFont(-1, 9, 400, 0, "Lucida Console")

	; Set the line values to null
	Local $i
	For $i=2 To 7
		$statuswin[$i] = Null
	Next

	$statuswin[8] = True
	WinSetTrans($statuswin[0],"",200)
	GUISetState(@SW_SHOW)

	Return $statuswin
EndFunc


Func statuswin_log(ByRef $statuswin, $line)
	Local $i
	Local $added = False
	For $i = 2 To 7
		If $statuswin[$i]== Null Then
			$statuswin[$i] = $line
			$added = True
			ExitLoop
		EndIf
	Next

	If Not $added Then
		For $i=2 To 6
			$statuswin[$i] = $statuswin[$i+1]
		Next
		$statuswin[7] = $line
	EndIf

	Local $string = ""
	For $i = 2 To 7
		If $statuswin[$i] == Null Then ExitLoop
		$string = $string & $statuswin[$i] & @CRLF
	Next

	GUICtrlSetData($statuswin[1],$string)

	Return $statuswin
EndFunc