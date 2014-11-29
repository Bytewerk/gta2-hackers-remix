#include-once

; For now, this just gets printed to STDOUT.
; We might create an extra debug gui, that can be toggled with a hotkey
; or something like that.
Func l($text, $i = -1)
	If $i <> -1 Then ConsoleWrite("Player "&$i&": ")
	ConsoleWrite($text & @crlf)
EndFunc