#include <AutoItConstants.au3>
#include "logging.au3"
#include "other.au3"

Func gamepad_glue($config)
	l("Starting gamepad glue...")
	Local $pid = Run(@ScriptDir & "\bin\sdl_controller_code.exe nonverbose", _
		@ScriptDir & "\bin", @SW_HIDE, $STDOUT_CHILD)
	Local $i = 0
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

		; This is the main loop of the AutoIt3 code, while the
		; game is actually running. Make sure that every window
		; thinks, that it has focus (see other.au3). Doing this
		; every second is enough.
		;$i +=1
		;If $i == 10 Then
		;	fake_focus()
		;	$i = 0
		;EndIf

		Sleep(100)
	WEnd
	l("Gamepad glue has quit!")
EndFunc