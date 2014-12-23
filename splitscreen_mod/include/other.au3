#include <SendMessage.au3>
#include-once

; Send all GTA2 windows the YOU-ARE-IN-FOCUS message so they will
; keep on playing sound - even if they are not in focus anymore.
; Without this fix, having sound in all windows at the same time
; is pure luck and will not work with wine for example.
; Credits to sqozz for finding this!

Global $config

Func fake_focus()
	l("Sending fake focus messages to all windows...")
	Local $i
	For $i=0 To $config[7]
		_SendMessage(WinGetHandle("Player "&($i+1)), 0x0007)
	Next
EndFunc

Func is_running_in_wine()
	RegRead("HKEY_CURRENT_USER\Software\Wine", "")
	If @error > 0 Then Return False
	Return True
EndFunc

