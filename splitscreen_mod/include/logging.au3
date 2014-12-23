#include-once
#include "statuswin.au3"

Func l($text, $i = -1)
	Global $statuswin
	If $i <> -1 Then $text = "#"&$i&": " & $text

   ; Add a prefix, to make the messages stand out from
   ; wine output
	ConsoleWrite("G2HR: "&$text & @crlf)

	If IsArray($statuswin) Then
		statuswin_log($statuswin, $text)
	EndIf
EndFunc