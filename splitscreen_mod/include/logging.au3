#include-once
#include "statuswin.au3"

Func l($text, $i = -1)
	Global $statuswin
	If $i <> -1 Then $text = "#"&$i&": " & $text
	ConsoleWrite($text & @crlf)

	If $statuswin Then
		statuswin_log($statuswin, $text)
	EndIf
EndFunc