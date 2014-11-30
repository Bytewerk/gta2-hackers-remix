#include <AutoItConstants.au3>
#include "logging.au3"

Func gamepad_glue()
	l("Starting gamepad glue...")
	Local $pid = Run(@ScriptDir & "\bin\sdl_controller_code.exe nonverbose", _
		@ScriptDir & "\bin", @SW_HIDE, $STDOUT_CHILD)
	While ProcessExists($pid)
		Local $out = StdoutRead($pid)
		If $out Then
			$lines = StringSplit($out,@CRLF)
			Local $i
			For $i = 1 To $lines[0]
				If Not $lines[$i] Then ContinueLoop
				l("GG: " & $lines[$i])
			Next
		EndIf
		Sleep(100)
	WEnd
	l("Gamepad glue has quit!")
EndFunc