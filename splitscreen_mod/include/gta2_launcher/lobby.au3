#include "..\logging.au3"
#include "lobby_controls.au3"

; $config: see arrays.txt
Func lobby_run($config)
	Local $cache = @AppDataDir & "\GTA2 Hackers Remix\Modified GTA2.exe Cache"
	Local $host_title = "GTA2 Multiplayer Setup"

	; Working dir: GTA2.exe path, cut off at the last '\' char
	Local $workingdir = StringMid($config[0], 1, _
		StringInStr($config[0], "\", 1, -1))

	For $i = 1 To $config[7]
		Local $param = "-j 127.0.0.1"
		If $i == 1 Then $param = "-c"

		; @SW_HIDE will hide both the network and the ingame window until they get manually activated.
		; Doesn't work on wine (so we hide the client network windows manually below).
		l("Launching GTA2 with parameter: "&$param, $i)
		Run($cache & "\Player" & $i & ".exe " & $param,$workingdir, @SW_HIDE)

		l("Waiting for network window. This may take a while!", $i)
		WinWait("Network GTA2", "")

		If $i == 1 Then
			WinSetTitle("Network GTA2", "",$host_title)

			ControlHide($host_title, "", $GTA2_LOBBY_REJECT)
			ControlHide($host_title, "", $GTA2_LOBBY_START)
			ControlHide($host_title, "", $GTA2_LOBBY_CANCEL)

			ControlMove($host_title, "", $GTA2_LOBBY_START, 30, 150, 160, 100)
			ControlSetText($host_title, "", $GTA2_LOBBY_START, "START IT ALREADY")
			ControlSetText($host_title, "", $GTA2_LOBBY_CHAT, @CRLF & "Remember: Respect is everything!" _
				& @CRLF & @CRLF & "...and also that you can only quit this mod properly with [F2]." _
				& @CRLF & @CRLF & "Set up the game as you like it, and click the start button when you're ready." _
				& @CRLF & @CRLF & "It will appear after all players have joined the lobby.")

			WinSetState($host_title,"",@SW_SHOW)
			WinActivate($host_title)
		Else
			WinSetTitle("Network GTA2", "", "GTA2: Player "&$i)

			; Wine ignores the @SW_HIDE passed as Run argument, see above.
			WinSetState("GTA2: Player "&$i, "", @SW_HIDE)
		EndIf
	Next
	l("All players have joined the lobby!")
	ControlShow($host_title, "", $GTA2_LOBBY_START)
	WinActivate($host_title)
	ControlFocus($host_title, "", $GTA2_LOBBY_START)

	While WinExists($host_title)
		Sleep(100)
	WEnd
EndFunc