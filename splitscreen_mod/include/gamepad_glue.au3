#include <AutoItConstants.au3>
#include "logging.au3"

Func gamepad_glue()
	l("Starting gamepad glue...")
	Local $pid = Run(@ScriptDir & "\..\bin\sdl_controller_code.exe", _
		@ScriptDir & "\..\bin", @SW_HIDE, $STDOUT_CHILD)
	While ProcessExists($pid)
		Local $out = StdoutRead($pid)
		If $out <> "" Then l("GG: " & $out)
		Sleep(100)
	WEnd
	l("Gamepad glue has quit!")
EndFunc

gamepad_glue()